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

#include "HelbingAgent.h"
#include "HelbingSimulator.h"
#include "Math/geomQuery.h"

namespace Helbing {
	////////////////////////////////////////////////////////////////
	//					Implementation of Helbing::Agent
	////////////////////////////////////////////////////////////////

	// mass = 80 Kg comes from Helbing's 2000 paper
	Agent::Agent(): Agents::BaseAgent() {
		_mass = 80.f;
	}

	////////////////////////////////////////////////////////////////

	Agent::~Agent() {
	}

	////////////////////////////////////////////////////////////////

	void Agent::computeNewVelocity() {
		Vector2 force( drivingForce() );
		for ( size_t i = 0; i < _nearAgents.size(); ++i ) {
			const Agents::BaseAgent * otherBase = _nearAgents[i].agent;
			const Agent * const other = static_cast< const Agent *>( otherBase );
			
			force += agentForce( other );
		}

		for ( size_t obs = 0; obs < _nearObstacles.size(); ++obs ) {
			const Agents::Obstacle * obst = _nearObstacles[ obs ].obstacle;
			force += obstacleForce( obst );
		}
		Vector2 acc = force / _mass;
		_velNew = _vel + acc * Simulator::TIME_STEP;
	}

	////////////////////////////////////////////////////////////////

	Vector2 Agent::agentForce( const Agent * other ) const {
		/* compute right of way */
		float rightOfWay = fabs( _priority - other->_priority );
		if ( rightOfWay >= 1.f ) {
			rightOfWay = 1.f;
		}

		const float D = Simulator::FORCE_DISTANCE;
		Vector2 normal_ij = _pos - other->_pos;
		float distance_ij = abs( normal_ij ); 
		normal_ij /= distance_ij;
		float Radii_ij = _radius + other->_radius; 
		
		float AGENT_SCALE = Simulator::AGENT_SCALE;
		float D_AGT = D;
		
		// Right of way-dependent calculations
		// Compute the direction perpinduclar to preferred velocity (on the side 
		//		of the normal force.

		Vector2 avoidNorm( normal_ij );
		if ( rightOfWay ) {
			Vector2 perpDir;
			if ( _priority < other->_priority ) {
				// his advantage
				D_AGT += ( rightOfWay * rightOfWay ) * _radius * .5f;	// Note: there is no symmetric reduction on 
												// the other side
				// modify normal direction
				//	The perpendicular direction should always be in the direction that gets the
				//	agent out of the way as easily as possible
				float prefSpeed = other->_velPref.getSpeed();
				if ( prefSpeed < 0.0001f ) {
					// he wants to be stationary, accelerate perpinduclarly to displacement
					perpDir.set( -normal_ij.y(), normal_ij.x() );
					if ( perpDir * _vel < 0.f ) perpDir.negate();
				} else {
					// He's moving somewhere, accelerate perpindicularly to his preferred direction
					// of travel.  
					const Vector2 prefDir( other->_velPref.getPreferred() );
					perpDir.set( -prefDir.y(), prefDir.x() );	// perpendicular to preferred velocity
					if ( perpDir * normal_ij < 0.f ) perpDir.negate();
				}
				// spherical linear interpolation
				float sinTheta = det( perpDir, normal_ij );
				if ( sinTheta < 0.f ) {
					sinTheta = -sinTheta;
				} 
				if ( sinTheta > 1.f ) {
					sinTheta = 1.f;	// clean up numerical error arising from determinant
				}
				avoidNorm.set( slerp( rightOfWay, normal_ij, perpDir, sinTheta ) );
			}
		}
		float mag = (AGENT_SCALE * expf((Radii_ij - distance_ij)/ D_AGT ));
		const float MAX_FORCE = 1e15f;
		if ( mag >= MAX_FORCE ) {
			mag = MAX_FORCE;
		}
		Vector2 force( avoidNorm * mag );

		if (distance_ij < Radii_ij) {
			Vector2 f_pushing( 0.f, 0.f ); 
			Vector2 f_friction( 0.f, 0.f );
			// pushing
			Vector2 tangent_ij( normal_ij.y(), -normal_ij.x() ); 

			f_pushing = normal_ij * ( Simulator::BODY_FORCE * (Radii_ij - distance_ij)); 
			f_friction = tangent_ij * (Simulator::FRICTION * (Radii_ij - distance_ij)) * fabs(( other->_vel - _vel ) * tangent_ij);// / distance_ij;
			force += f_pushing + f_friction;
		}
		return force;
	}

	////////////////////////////////////////////////////////////////

	Vector2 Agent::obstacleForce( const Agents::Obstacle * obst ) const {
		const float D = Simulator::FORCE_DISTANCE;
		const float OBST_MAG = Simulator::OBST_SCALE;
		Vector2 nearPt;	// set by distanceSqToPoint
		float distSq;	// set by distanceSqToPoint
		if ( obst->distanceSqToPoint( _pos, nearPt, distSq ) == Agents::Obstacle::LAST ) return Vector2(0.f,0.f);
		float dist = sqrtf( distSq );
		Vector2 forceDir( ( _pos - nearPt ) / dist );
		
		Vector2 force = forceDir * (OBST_MAG * exp( ( _radius - dist ) / D)); 

		// pushing, friction
		if (dist < _radius) { // intersection has occurred
			Vector2 f_pushing(0.f, 0.f);
			Vector2 f_friction(0.f, 0.f);

			Vector2 tangent_io( forceDir.y(), -forceDir.x()); 

			// make sure direction is opposite i's velocity
			if ( ( tangent_io * _vel) < 0.f) {
				tangent_io.negate();
			}

			f_pushing = forceDir * (Simulator::BODY_FORCE * ( _radius  - dist ) ); 

			// friction
			f_friction = tangent_io * Simulator::FRICTION * ( _radius - dist ) * ( _vel * tangent_io);
			force += f_pushing - f_friction;
		}
		return force;
	}

	////////////////////////////////////////////////////////////////

	Vector2 Agent::drivingForce() const { 
		return ( _velPref.getPreferredVel() - _vel ) * ( _mass / Simulator::REACTION_TIME );
	}

	////////////////////////////////////////////////////////////////

}	// namespace Helbing