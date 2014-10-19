/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "KaramouzasAgent.h"
#include "KaramouzasSimulator.h"
#include "Math/geomQuery.h"
#include <list>

namespace Karamouzas {
	////////////////////////////////////////////////////////////////
	//					Implementation of Karamouzas::Agent
	////////////////////////////////////////////////////////////////

	Agent::Agent(): Agents::BaseAgent() { 
		_perSpace = 1.f;
		_anticipation = 3.f;
	}

	////////////////////////////////////////////////////////////////

	Agent::~Agent() {
	}

	////////////////////////////////////////////////////////////////

	void Agent::computeNewVelocity() {
		const float EPSILON = 0.01f; // this eps from Ioannis
		const float FOV = Simulator::COS_FOV_ANGLE;
		
		Vector2 force( ( _velPref.getPreferredVel() - _vel )  / Simulator::REACTION_TIME );
		const float SAFE_DIST = Simulator::WALL_DISTANCE + _radius;
		const float SAFE_DIST2 = SAFE_DIST * SAFE_DIST;
		for ( size_t o = 0; o < _nearObstacles.size(); ++o ) {
			// TODO: Interaction with obstacles is, currently, defined strictly 
			//	by COLLISIONS.  Only if I'm going to collide with an obstacle is
			//	a force applied.  This may be too naive.
			//	I'll have to investigate this.
			const Agents::Obstacle * obst = _nearObstacles[ o ].obstacle;
			Vector2 nearPt;		// set by distanceSqToPoint
			float sqDist;		// set by distanceSqToPoint
			if ( obst->distanceSqToPoint( _pos, nearPt, sqDist ) == Agents::Obstacle::LAST ) continue;
			if ( SAFE_DIST2 > sqDist ) {
				// A repulsive force is actually possible
				float dist = sqrtf( sqDist );
				float num = SAFE_DIST - dist;
				float distMradius = ( dist - _radius ) < EPSILON ? EPSILON : dist - _radius;
				float denom = powf( distMradius, Simulator::WALL_STEEPNESS );
				Vector2 dir = norm( _pos - nearPt );
				float mag = num / denom;
				force += dir * mag;
			}
		}

		Vector2 desVel = _vel + force * Simulator::TIME_STEP;
		float desSpeed = abs( desVel );
		force.set( 0.f, 0.f );
//#if 0
//		// iteratively evaluate neighbors
//#else
		// Weight all neighbors
		bool colliding = false;
		int collidingCount = Simulator::COLLIDING_COUNT;
		bool VERBOSE = false; // _id == 1;
		if ( VERBOSE ) std::cout << "Agent " << _id << "\n";
		float totalTime = 1.f;
		std::list< std::pair< float, const Agent  * > > collidingSet;
		for ( size_t j = 0; j < _nearAgents.size(); ++j ) {
			const Agents::BaseAgent * otherBase = _nearAgents[j].agent;
			const Agent * const other = static_cast< const Agent *>( otherBase );
			float circRadius = _perSpace + other->_radius;
			Vector2 relVel = desVel - other->_vel;
			Vector2 relPos = other->_pos - _pos;

			if ( absSq( relPos ) < circRadius * circRadius) { ///collision!
				if (!colliding) {
					colliding = true;
					collidingSet.clear();
				}
				collidingSet.push_back(std::pair< float, const Agent * >(.0f, other ) );
				if ( static_cast<int>(collidingSet.size()) > collidingCount ) ++collidingCount;
				continue;
			}

			// TODO: evalute field of view
			//		If relPos is not within the field of view around preferred direction, continue
			Vector2 relDir = norm( relPos );
			if ( ( relDir * _orient ) < FOV ) continue;
			float tc = rayCircleTTC( relVel, relPos, circRadius );
			if ( tc < _anticipation && !colliding ) {
				if ( VERBOSE ) std::cout << "\tAgent " << other->_id << " t_c: " << tc << "\n";
				//totalTime += tc;
				// insert into colliding set (in order)
				std::list< std::pair< float, const Agent  * > >::iterator itr = collidingSet.begin();
				while ( itr != collidingSet.end() && tc > itr->first ) ++itr;
				collidingSet.insert( itr, std::pair< float, const Agent * >( tc, other ) );
			}
		}
		//if ( collidingSet.size() > 0 ) {
		int count = 0;
		std::list< std::pair< float, const Agent * > >::iterator itr = collidingSet.begin();
		for ( int i = 0; i < collidingCount; ++i, ++itr ) {
			if ( itr == collidingSet.end() ) break;
			const Agent * const other = itr->second;
			float tc = itr->first;
			// future positions
			Vector2 myPos = _pos + desVel * tc;
			Vector2 hisPos = other->_pos + other->_vel * tc;
			Vector2 forceDir = myPos - hisPos;
			//float futureDist = abs( forceDir );
			//forceDir /= futureDist;
			//float D = desSpeed * tc + futureDist - _radius - other->_radius;
			float fDist = abs( forceDir );
			forceDir /= fDist;
			float collisionDist = fDist - _radius - other->_radius;
			float D = std::max( desSpeed * tc + (collisionDist < 0 ? 0 : collisionDist), EPSILON );

			// determine magnitude
			
			float mag;
			if ( D < Simulator::D_MIN ) {
				mag = Simulator::AGENT_FORCE * Simulator::D_MIN / D;
			} else if ( D < Simulator::D_MID ) {
				mag = Simulator::AGENT_FORCE;
			} else if ( D < Simulator::D_MAX ) {
				//D -= Simulator::D_MID;
				//mag =  D * Simulator::AGENT_FORCE / ( Simulator::D_MAX - Simulator::D_MID ) + Simulator::D_MID ;
				mag =  Simulator::AGENT_FORCE * ( Simulator::D_MAX - D ) / ( Simulator::D_MAX - Simulator::D_MID ) ;
			} else {
				continue;	// magnitude is zero
			}
			float weight = pow( colliding ? 1.f : 0.8f, count++ );
			//float weight = ( totalTime - tc  ) / totalTime; //1.f / ( tc * totalTime );
			if ( VERBOSE ) std::cout << "\tAgent " << other->_id << " magnitude: " << mag << " weight: " << weight << " total force: " << ( mag * weight ) << " D: " << D << "\n";
			force += forceDir * ( mag * weight );
		}
		// Add some noise to avoid deadlocks and introduce variation
		//float angle = rand() * 2.0f * M_PI / RAND_MAX;
		float angle = rand() * 2.0f * 3.1415f / RAND_MAX;
	    float dist = rand() * 0.001f / RAND_MAX;
	    force += dist*Vector2(cos(angle), sin(angle)); 
		// do we need a drag force?
		 
		 // Cap the force to maxAccel
		if (abs(force)> _maxAccel) force = norm( force ) * _maxAccel;
	
	    _velNew = desVel + force * Simulator::TIME_STEP;	// assumes unit mass
	}

}	// namespace Karamouzas
