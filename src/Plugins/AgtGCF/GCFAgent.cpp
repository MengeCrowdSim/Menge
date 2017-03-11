/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "GCFAgent.h"
#include "GCFSimulator.h"
#include "MengeCore/Math/geomQuery.h"

namespace GCF {

	using Menge::Agents::BaseAgent;
	using Menge::Agents::Obstacle;
	using Menge::Math::Vector2;

	////////////////////////////////////////////////////////////////
	//					Implementation of Helper Function
	////////////////////////////////////////////////////////////////

	float hermite_interp(float t, float x1, float x2, float y1, float y2, float dy1, float dy2) {
		assert( t >= x1 && t <= x2 && "Can only interpolate values inside the range" );
		assert( x2 > x1 && "Intervals must be defined as x1 < x2" );

		float scale = x2 - x1;
		t = ( t - x1 ) / scale;
		float t2 = t * t;
		float t3 = t2 * t;
		float h1 = 2 * t3 - 3 * t2 + 1;
		float h2 = -2 * t3 + 3 * t2;
		float h3 = t3 - 2 * t2 + t;
		float h4 = t3 - t2;
		float left = y1 * h1 + dy1 * h3 * scale;
		float right = y2 * h2 + dy2 * h4 * scale;
		return left + right;
	}

	////////////////////////////////////////////////////////////////
	//					Implementation of GCF::Agent
	////////////////////////////////////////////////////////////////

	const std::string Agent::NAME = "gcf";

	////////////////////////////////////////////////////////////////

	Agent::Agent(): Agents::BaseAgent(), _ellipse() {
		_aMin = 0.18f;
		_aRate = 0.53f;
		_bMax = 0.25f;
		_bGrowth = 0.05f;
	}

	////////////////////////////////////////////////////////////////

	Agent::~Agent() {
	}

	////////////////////////////////////////////////////////////////

	void Agent::initialize() {
		Agents::BaseAgent::initialize();
		updateEllipse();
	}

	////////////////////////////////////////////////////////////////

	void Agent::updateOrient(float timeStep) {
		// This stabilizes orientation
		//		As the agent slows down, the target orientation becomes preferred direction
		float speed = sqrt( absSq( _vel ) );
		float frac = speed < _prefSpeed ? sqrtf( speed / _prefSpeed ) : 1.f;
		
		Vector2 prefDir = _velPref.getPreferred();
		Vector2 moveDir = _vel / speed;

		Vector2 newOrient = frac * moveDir + ( 1.f - frac ) * prefDir;
		newOrient.normalize();
		const float MAX_ANGLE_CHANGE = timeStep * _maxAngVel;
		float maxCt = cos( MAX_ANGLE_CHANGE );
		float ct = newOrient * _orient;
		if ( ct < maxCt ) {
			// changing direction at a rate greater than _maxAngVel
			float maxSt = sin( MAX_ANGLE_CHANGE );
			if ( det( _orient, newOrient ) > 0.f ) {
				// rotate _orient left
				_orient.set( maxCt * _orient._x - maxSt * _orient._y,
					         maxSt * _orient._x + maxCt * _orient._y );
			} else {
				// rotate _orient right
				_orient.set( maxCt * _orient._x + maxSt * _orient._y,
					         -maxSt * _orient._x + maxCt * _orient._y );
			}
		}
	}

	////////////////////////////////////////////////////////////////
	
	void Agent::postUpdate() {
		updateEllipse();
	}

	////////////////////////////////////////////////////////////////

	void Agent::computeNewVelocity() {
#if 1
		Vector2 force( driveForce() );	// driving force
		const float SPEED = abs( _vel );
		const float PREF_SPEED = abs( _velPref.getPreferredVel() );

		// Pedestrians
		for ( size_t j = 0; j < _nearAgents.size(); ++j ) {
			const BaseAgent * otherBase = _nearAgents[j].agent;
			const Agent * const other = static_cast< const Agent *>( otherBase );
			float effDist, K_ij, response, velScale, magnitude;
			Vector2 forceDir;
			if ( getRepulsionParameters( other, effDist, forceDir, K_ij,
				                         response, velScale, magnitude ) == 0 ) {
				force += forceDir * magnitude;
			}
		}

		// Obstacles
		if ( SPEED > 0.0001f ) {
			// No obstacle force if basically stationary
			for ( size_t i = 0; i < _nearObstacles.size(); ++i ) {
				const Menge::Agents::Obstacle * obst = _nearObstacles[ i ].obstacle;
				if ( obst->length() < 0.1f ) {
					continue;	// ignore short obstacles
				}
				// force from three points: nearest point, and point along wall in front
				//	and point along wall behind.
				Vector2 nearPt;	// gets set by distanceSqToPoint
				float distSq;	// gets set by distanceSqToPoint
				if ( obst->distanceSqToPoint( _pos, nearPt, distSq ) ==
					 Menge::Agents::Obstacle::LAST ) continue;

				// No force if the agent is ON the point
				if ( distSq < 0.0001f ) continue;

				Vector2 disp = nearPt - _pos;
				float dist = sqrtf( distSq );
				Vector2 dir = disp / dist;

				// test visibility
				float cosTheta = _orient * dir;
				// No force if the point is more than 90 degrees away from
				//	movement direction
				if ( cosTheta < 0.f ) continue;
				
				// This is an APPROXIMATION of the actual distance to the wall
				float boundDist = _ellipse.approximateMinimumDistance( nearPt );
				float Bij = 1.f - dist / boundDist;

				//// No force if the point lies inside the ellipse
				if ( Bij > 0.f ) return;
				force += dir * ( Bij * PREF_SPEED );
			}
		}

		// We're assuming unit mass
		_velNew = _vel + force * Simulator::TIME_STEP;
#else
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
#endif
	}

	////////////////////////////////////////////////////////////////

	int Agent::getRepulsionParameters( const Agent * agent, float & effDist, Vector2 & forceDir,
		                               float & K_ij, float & response, float & velScale,
									   float & magnitude ) const {
		const float PREF_SPEED = abs( _velPref.getPreferredVel() );
		forceDir = _ellipse.ellipseCenterDisplace( agent->_ellipse );
		float centerDist = abs( forceDir );
		float dca = _ellipse.distanceOfClosestApproach( agent->_ellipse );
		effDist = centerDist - dca;

		float dist = abs( forceDir );
		assert( dist > 0.0001f && "Agents are on top of each other" );
		forceDir /= dist;

		if ( effDist >= Simulator::MAX_AGENT_DIST ) {
			// Agent too far away to give force.
			return 1;
		}

		// field of view
		K_ij = _orient * forceDir;

		// This represents 360 degree sensitivity, with the maximum sensitivity in the oriented
		//	direction fading to zero in the opposite direction
		//  remap [-1, 1] -> [-1, -0.1]
		K_ij = ( K_ij * 0.45f ) - 0.55f;
		
		// relative velocities
		Vector2 relVel = _vel - agent->_vel;

		float velWeight = relVel * forceDir; 
		velScale = Simulator::NU_AGENT * PREF_SPEED;
		if ( velWeight <= 0.f ) {
			// convergent velocity needs some extra pushing
			velScale -= velWeight / std::max( effDist, 0.01f ) ;
		}

		// force response
		response = computeDistanceResponse( effDist );

		magnitude = ( -K_ij * response * velScale * velScale );

		return 0;
	}

	////////////////////////////////////////////////////////////////

	float Agent::computeDistanceResponse( float effDist ) const {
		//	Evaluates a piece-wise hermite curve based on the value of effDist
		//		The boundaries of the function are defined by user parameters.
		//
		//          0.0        interpWidth          maxDist-interpWidth   maxDist
		//	     ----|-------------|--------------------------|--------------|----
		//       5   |     4       |            3             |      2       | 1
		const float maxDist = Simulator::MAX_AGENT_DIST;
		const float interpWidth = Simulator::AGENT_INTERP_WIDTH;
		const float maxForce = Simulator::MAX_AGENT_FORCE;
		
		// Distance to great to make a difference - region 1
		if ( effDist >= maxDist ) {
			return 0.f;
		}
		
		// Distance so close that a constant force should be applied. - region 5
		if ( effDist <= 0 ) {
			return 3.f * maxForce; 
		}
		
		// Aproaching maximum distance with linearly decreasing force - region 2
		const float dist_intpol_right = maxDist - interpWidth;
		if ( effDist > dist_intpol_right ) {
			float f = 1.f / dist_intpol_right;
			float fDeriv = -f * f;
			return hermite_interp( effDist, dist_intpol_right, maxDist, f, 0.f, fDeriv, 0.f );
		}

		// middle domain - simply inverse force - region 3
		if ( effDist > interpWidth ) {
			return 1.f / effDist;
		}
		// closest domain, smoothing converge to constant - region 4
		float f = 1.f / interpWidth;
		float fDeriv = -f * f;
		return hermite_interp( effDist, 0, interpWidth, 3.f * maxForce, f, 0, fDeriv );
	}

	////////////////////////////////////////////////////////////////

#if 0
	Vector2 Agent::obstacleForce( const Agents::Obstacle * obst ) const {
		const float D = Simulator::FORCE_DISTANCE;
		const float OBST_MAG = Simulator::OBST_SCALE;
		Vector2 nearPt;	// set by distanceSqToPoint
		float distSq;	// set by distanceSqToPoint
		if ( obst->distanceSqToPoint( _pos, nearPt, distSq ) ==
			Agents::Obstacle::LAST ) return Vector2(0.f,0.f);
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
			f_friction = tangent_io * Simulator::FRICTION
				         * ( _radius - dist ) * ( _vel * tangent_io);
			force += f_pushing - f_friction;
		}
		return force;
	}

#endif

	////////////////////////////////////////////////////////////////

	Vector2 Agent::driveForce() const { 
		return ( _velPref.getPreferredVel() - _vel ) / Simulator::REACTION_TIME;
	}

	////////////////////////////////////////////////////////////////

	void Agent::updateEllipse() {
		float speed = abs(_vel);
		// update ellipse
		_ellipse.setCenter( _pos );
		_ellipse.setOrientation( _orient );
		// compute major and minor axis values based on speed
		float major = _aMin + _aRate * speed;
		float minor = _bMax - _bGrowth * speed / 1.3f;
		_ellipse.setAxes( major, minor );
	}
}	// namespace GCF
