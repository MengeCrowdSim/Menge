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

#include "Goals/GoalCircle.h"
#include "shapes.h"
#include "PrefVelocity.h"

namespace Menge {

	namespace BFSM {
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of CircleGoal
		/////////////////////////////////////////////////////////////////////

		CircleGoal::CircleGoal():Goal(), _center(0.f,0.f), _radius(1.f) {}

		/////////////////////////////////////////////////////////////////////

		float CircleGoal::squaredDistance( const Vector2 & pt ) const {
			float d = abs( pt - _center );
			float perimD = d - _radius;
			return perimD * perimD;
		}

		/////////////////////////////////////////////////////////////////////

		void CircleGoal::setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const {
			const float TARGET_R = _radius - r;
			if ( TARGET_R < 0 ) {
				// circle isn't big enough to fit agent -- treat it like a point goal
				Vector2 dir( norm( _center - q ) );
				directions.setSingle( dir );
				directions.setTarget( _center );
			} else {
				// Circle is large enough to form a span
				const float TARGET_R_SQD = TARGET_R * TARGET_R;
				Vector2 relPos = _center - q;
				const float distSq = absSq( relPos );
				if ( distSq < TARGET_R_SQD ) {
					// Goal reached -- I'm inside the effective circle -- current position is the goal
					//	and no movement is necessary
					directions.setSingle( Vector2( 0.f, 0.f ) );
					directions.setTarget( q );
				} else {
					// Outside the effective circle, a span is possible.
					float leg = sqrtf( distSq - TARGET_R_SQD );
					Vector2 left = Vector2( relPos.x() * leg - relPos.y() * TARGET_R, relPos.x() * TARGET_R + relPos.y() * leg ) / distSq;
					Vector2 right = Vector2( relPos.x() * leg + relPos.y() * TARGET_R, -relPos.x() * TARGET_R + relPos.y() * leg ) / distSq;
					const float dist = sqrtf( distSq );
					Vector2 dir = relPos / dist;
					directions.setSpan( left, right, dir );
					directions.setTarget( q + dir * ( dist - TARGET_R ) );
				}
			}
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 CircleGoal::getTargetPoint( const Vector2 & q, float r ) const { 
			const float THRESH = _radius - r;
			if ( THRESH > 0 ) {
				Vector2 disp( q - _center );
				float distSqd = absSq( disp );
				if ( distSqd < THRESH * THRESH ) {
					return q;
				} else {
					float dist = sqrtf( distSqd );
					Vector2 offset = disp * ( THRESH / dist );
					return _center + offset;
				}
			} else {
				return _center; 
			}
		} 

		/////////////////////////////////////////////////////////////////////

		void CircleGoal::drawGLGeometry() const{
			glPushMatrix();
			glTranslatef( _center.x(), 0.f, _center.y() );
			glScalef( _radius, _radius, _radius );
			SceneGraph::Circle::drawUnit();
			glPopMatrix();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of CircleGoalFactory
		/////////////////////////////////////////////////////////////////////

		CircleGoalFactory::CircleGoalFactory() : GoalFactory() {
			_xID = _attrSet.addFloatAttribute( "x", true /*required*/ );
			_yID = _attrSet.addFloatAttribute( "y", true /*required*/ );
			_rID = _attrSet.addFloatAttribute( "radius", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool CircleGoalFactory::setFromXML( Goal * goal, TiXmlElement * node, const std::string & behaveFldr ) const {
			
			CircleGoal * cGoal = dynamic_cast< CircleGoal * >( goal );
			assert( cGoal != 0x0 && "Trying to set circle goal attributes on an incompatible object." );

			if ( ! GoalFactory::setFromXML( cGoal, node, behaveFldr ) ) return false;

			cGoal->setCenter( _attrSet.getFloat( _xID ), _attrSet.getFloat( _yID ) );
			cGoal->setRadius( _attrSet.getFloat( _rID ) );

			return true;
		}
		
		/////////////////////////////////////////////////////////////////////

		
	}	// namespace BFSM
}	// namespace Menge
