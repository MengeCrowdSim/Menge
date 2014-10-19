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

#include "ZanlungoAgent.h"
#include "ZanlungoSimulator.h"
#include "Math/geomQuery.h"
#include "Math/consts.h"

namespace Zanlungo {
	////////////////////////////////////////////////////////////////
	//					Implementation of Zanlungo::Agent
	////////////////////////////////////////////////////////////////

	Agent::Agent(): Agents::BaseAgent() {
		_mass = 80.f;
	}

	////////////////////////////////////////////////////////////////

	Agent::~Agent() {
	}

	////////////////////////////////////////////////////////////////

	void Agent::computeNewVelocity() {
		// 1. Compute T_i
		float T_i;
		bool interacts = computeTTI( T_i );

		const float TAU = Simulator::REACTION_TIME;
		Vector2 force( ( _velPref.getPreferredVel() - _vel ) * _mass / TAU );
		
		if ( interacts ) {
			// if T_i never got set, there are no interactions to do
			const float SPEED = abs( _vel );
			const float B = Simulator::FORCE_DISTANCE;
			//const float MAG = Simulator::AGENT_SCALE * SPEED / T_i;
			for ( size_t j = 0; j < _nearAgents.size(); ++j ) {
				// 2. Use T_i to compute the direction
				const Agents::BaseAgent * otherBase = _nearAgents[j].agent;
				const Agent * const other = static_cast< const Agent *>( otherBase );
				force += agentForce( other, T_i );
			}
			// obstacles
			Vector2 futurePos = _pos + _vel * T_i;
			const float OBST_MAG = Simulator::OBST_SCALE * SPEED / T_i;
			for ( size_t obs = 0; obs < _nearObstacles.size(); ++obs ) {
				Vector2 nearPt;		// set by call to distanceSqToPoint
				float d2;			// set by call to distanceSqToPoint
				if ( _nearObstacles[ obs ].obstacle->distanceSqToPoint( futurePos, nearPt, d2 ) == Agents::Obstacle::LAST ) continue;
				Vector2 D_ij = futurePos - nearPt;
				float dist = abs( D_ij );
				D_ij /= dist;
				dist -= _radius;
				force += D_ij * ( OBST_MAG * expf( -dist / B ) );
			}
		}

		Vector2 acc = force / _mass;
		_velNew = _vel + acc * Simulator::TIME_STEP;
	}

	////////////////////////////////////////////////////////////////

	Vector2 Agent::agentForce( const Agent * other, float T_i ) const {
		float D = Simulator::FORCE_DISTANCE;
		// Right of way-dependent calculations
		Vector2 myVel = _vel;
		Vector2 hisVel = other->_vel;
		float weight = 1.f - rightOfWayVel( hisVel, other->_velPref.getPreferredVel(), other->_priority, myVel );
		
		const Vector2 relVel = myVel - hisVel;

		Vector2 futPos = _pos + myVel * T_i;
		Vector2 otherFuturePos = other->_pos + hisVel * T_i;
		Vector2 D_ij = futPos - otherFuturePos;

		// If the relative velocity is divergent do nothing
		if ( D_ij * ( _vel - other->_vel ) > 0.f ) return Vector2(0.f,0.f);
		float dist = abs( D_ij );
		D_ij /= dist;
		if ( weight > 1.f ) {
			// Other agent has right of way
			float prefSpeed = other->_velPref.getSpeed();
			Vector2 perpDir;
			bool interpolate = true;
			if ( prefSpeed < 0.0001f ) {
				// he wants to be stationary, accelerate perpinduclarly to displacement
				Vector2 currRelPos = _pos - other->_pos;
				perpDir.set( -currRelPos.y(), currRelPos.x() );
				if ( perpDir * _vel < 0.f ) perpDir.negate();
			} else {
				// He's moving somewhere, accelerate perpindicularly to his preferred direction
				// of travel.  
				const Vector2 prefDir( other->_velPref.getPreferred() );
				if ( prefDir * D_ij > 0.f ) {
					perpDir.set( -prefDir.y(), prefDir.x() );	// perpendicular to preferred velocity
					if ( perpDir * D_ij < 0.f ) perpDir.negate();
				} else {
					interpolate = false;
				}
			}
			// spherical linear interpolation
			if ( interpolate ) {
				float sinTheta = det( perpDir, D_ij );
				if ( sinTheta < 0.f ) {
					sinTheta = -sinTheta;
				} 
				if ( sinTheta > 1.f ) {
					sinTheta = 1.f;	// clean up numerical error arising from determinant
				}
				D_ij.set( slerp( weight - 1.f, D_ij, perpDir, sinTheta ) );
			}
		} 
		dist -= ( _radius + other->_radius );
		float magnitude = weight * Simulator::AGENT_SCALE * abs( _vel - other->_vel ) / T_i;
		const float MAX_FORCE = 1e15f;
		if ( magnitude >= MAX_FORCE ) {
			magnitude = MAX_FORCE;
		}
		//float magnitude = weight * Simulator::AGENT_SCALE * abs( myVel - hisVel ) / T_i;
		// 3. Compute the force
		return D_ij * ( magnitude * expf( -dist / D ) );
	}

	////////////////////////////////////////////////////////////////

	bool Agent::computeTTI( float & T_i ) const {
		const float COS_FOV = -0.8f;// cos( HALFPI );// cos( PI / 4.f ); // 
		bool interacts = false;
		T_i = INFTY;
		//std::cout << "\n";
#define COLLIDE_PRIORITY
#ifdef COLLIDE_PRIORITY
		float t_collision = T_i;
#endif
		for ( size_t j = 0; j < _nearAgents.size(); ++j ) {
			const Agents::BaseAgent * otherBase = _nearAgents[j].agent;
			const Agent * const other = static_cast< const Agent *>( otherBase );

			// Right of way-dependent calculations
			Vector2 myVel = _vel;
			Vector2 hisVel = other->_vel;
			rightOfWayVel( hisVel, other->_velPref.getPreferredVel(), other->_priority, myVel );

			const Vector2 relVel = myVel - hisVel;
			Vector2 relPos = _pos - other->_pos;
//	This define determines if additional prediction code is executed
//		The original zanlungo model does not include performing exact collisions
//		between disks.  It simply estimates the time to interaction based on
//		projection of center on preferred velocity.
//		This define includes a precise test of ray-circle intersection to test
//		to see if the relative velocity intersects the minkowski sum of this
//		agent with its neighbor.  It makes the respones far more robust.
#define PRECISE
#ifdef PRECISE
			float circRadius = _radius + other->_radius;
			// first test to see if there's an actual collision imminent
			float contactT = rayCircleTTC( relVel, -relPos, circRadius );
			//std::cout << "\tColliding with " << other->_id << " at " << contactT << "\n";
#ifdef COLLIDE_PRIORITY
			if ( contactT < t_collision ) {
				// the ray intersects the circle -- actual collision is possible
				t_collision = contactT;
				interacts = true;
			} else if ( t_collision == INFTY ) { 
#else
			if ( contactT < T_i ) {
				// the ray intersects the circle -- actual collision is possible
				T_i = contactT;
				interacts = true;
			} else { 
#endif	// COLLIDE_PRIORITY
#endif	// PRECISE
				// no collision possible, see if getting close is possible
#ifdef ZANLUNGO_FOV
				float dist = abs( relPos );
				float relSpeed = abs( relVel );
				// This is the angle between relative position and ORIENTATION
				float cosTheta = -( relPos * _orient ) / dist;
				if ( cosTheta >= COS_FOV ) {
					// This is the angle between relative position and relative velocity
					cosTheta = relPos * relVel / ( -dist * relSpeed );
					float t_ij = cosTheta * dist / relSpeed;
					//std::cout << "\tApproaching " << other->_id << " at " << t_ij << "\n";
					if ( t_ij < T_i ) {
						T_i = t_ij;
						interacts = true;
					}
				}
#else
				// note: relPos points from other agent to this agent, so they need to point in OPPOSITE directions for convergence
				float dp = -( relPos * relVel );  
				if ( dp > 0.f ) { 
					float t_ij = dp / absSq( relVel );
					if ( t_ij < T_i ) {
						T_i = t_ij;
						interacts = true;
					}
				}
#endif
#ifdef PRECISE
			}
#endif
		}
		// Compute time to interaction for obstacles
		for ( size_t obs = 0; obs < _nearObstacles.size(); ++obs ) {
			// TODO: Interaction with obstacles is, currently, defined strictly 
			//	by COLLISIONS.  Only if I'm going to collide with an obstacle is
			//	a force applied.  This may be too naive.
			//	I'll have to investigate this.
			float t = _nearObstacles[ obs ].obstacle->circleIntersection( _vel, _pos, _radius );
			if ( t < T_i ) {
				T_i = t;
				interacts = true;
			}
		}
#ifdef COLLIDE_PRIORITY
		if ( t_collision < INFTY ) T_i = t_collision;
#endif
		if ( T_i < Simulator::TIME_STEP ) {
			T_i = Simulator::TIME_STEP;
		}

		return interacts;
	}

	////////////////////////////////////////////////////////////////

	float Agent::rightOfWayVel( Vector2 & otherVel, const Vector2 & otherPrefVel, float otherPriority, Vector2 & vel ) const {
		float rightOfWay = _priority - otherPriority;
		rightOfWay = ( rightOfWay < -1.f ) ? -1.f : (rightOfWay > 1.f) ? 1.f : rightOfWay;
		if ( rightOfWay < 0.f ) {
			float R2 = sqrtf( -rightOfWay ); //rightOfWay * rightOfWay; // -rightOfWay; // 
			vel = _vel;
			otherVel += R2 * ( otherPrefVel - otherVel );
			return -R2;
		} else if ( rightOfWay > 0.f ) {
			float R2 = sqrtf( rightOfWay ); //rightOfWay * rightOfWay; // rightOfWay; // 
			vel = _vel + R2 * ( _velPref.getPreferredVel() - _vel );
			return R2;
		} else {
			vel = _vel;
			return 0.f;
		}
	}

}	// namespace Zanlungo
