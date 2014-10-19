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

#include "Goals/GoalAABB.h"
#include "graphCommon.h"
#include "PrefVelocity.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBGoal
		/////////////////////////////////////////////////////////////////////

		AABBGoal::AABBGoal():Goal(), _minPt(0.f,0.f), _size(0.f,0.f) {}

		/////////////////////////////////////////////////////////////////////

		float AABBGoal::squaredDistance( const Vector2 & pt ) const {
			// determine quadrant
			float minX = _minPt._x;
			float maxX = minX + _size._x;
			const int xCoord = (int)(pt._x > maxX ) - (int)( pt._x < minX );
			float minY = _minPt._y;
			float maxY = minY + _size._y;
			const int yCoord = (int)(pt._y > maxY ) - (int)( pt._y < minY );

			if ( xCoord == 0 && yCoord == 0 ) {
				return 0.f;
			} else {
				// the x- and y- coordinates of the nearest point
				float X, Y;		
				X = ( xCoord == -1 ) * minX + ( xCoord == 1 ) * maxX;
				Y = ( yCoord == -1 ) * minY + ( yCoord == 1 ) * maxY;
				return absSq( Vector2( X, Y ) - pt );
			}
		}

		/////////////////////////////////////////////////////////////////////

		void AABBGoal::setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const {
			// based on the voronoi regions of the AABB
			//	 (-1,1)  |        (0,1)        |  (1,1)
			// ----------------------------------------
			//	         |                     |
			//	 (-1, 0) |        (0,0)        |  (1,0)
			//	         |                     |
			// ----------------------------------------
			//	 (-1,-1) |       (0,-1)        |  (1,-1)
			//

			// the x- and y- coordinates of the nearest point
			float X, Y;
			// the x- and y-coordintes of the left and right directions
			float xL, xR, yL, yR;	
			
			// determine quadrant
			const float D = 2.f * r;
			float minX = _minPt._x + r;
			float maxX = minX + _size._x - D;
			const int xCoord = (int)(q._x > maxX ) - (int)( q._x < minX );
			float minY = _minPt._y + r;
			float maxY = minY + _size._y - D;
			const int yCoord = (int)(q._y > maxY ) - (int)( q._y < minY );

			if ( xCoord == 0 && yCoord == 0 ) {
				// inside the region already
				directions.setSingle( Vector2(0.f, 0.f) );
				directions.setTarget( q );
			} else {
				int dimensions = 2;
				if ( _size._x < D ) {
					X = _minPt._x + _size._x * 0.5f;
					xL = X;
					xR = X;
					--dimensions;
				} else {
					int LMaxXtest = ( yCoord == 1 ) | ( ( xCoord == 1 ) & ( yCoord == 0 ) );
					xL = maxX * LMaxXtest + ( 1 - LMaxXtest ) * minX;
					int RMaxXtest = ( yCoord == -1 ) | ( ( xCoord == 1 ) & ( yCoord == 0 ) );
					xR = maxX * RMaxXtest + ( 1 - RMaxXtest ) * minX;
					X = ( xCoord == -1 ) * minX + ( xCoord == 1 ) * maxX + ( xCoord == 0 ) * q._x;
				}
				if ( _size._y < D ) {
					Y = _minPt._y + _size._y * 0.5f;
					yL = Y;
					yR = Y;
					--dimensions;
				} else {
					int LMaxYtest = ( xCoord == -1 ) | ( ( xCoord == 0 ) & ( yCoord == 1 ) );
					yL = maxY * LMaxYtest + ( 1 - LMaxYtest ) * minY;
					int RMaxYtest = ( xCoord == 1 ) | ( ( xCoord == 0 ) & ( yCoord == 1 ) );
					yR = maxY * RMaxYtest + ( 1 - RMaxYtest ) * minY;
					Y = ( yCoord == -1 ) * minY + ( yCoord == 1 ) * maxY + ( yCoord == 0 ) * q._y;
				}

				Vector2 targetPt = Vector2( X, Y );
				directions.setTarget( targetPt );
				Vector2 prefDir( norm( targetPt - q ) );
				if ( dimensions ) {
					// there is actually a span
					directions.setSpan( norm( Vector2( xL, yL ) - q ),
										norm( Vector2( xR, yR ) - q ),
										prefDir );
				} else {
					directions.setSingle( prefDir );
				}
			}
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 AABBGoal::getTargetPoint( const Vector2 & q, float r ) const {
			// based on the voronoi regions of the AABB
			//	 0 |         1          |  2
			// ----------------------------------------
			//	   |                    |
			//	 3 |         4          |  5
			//	   |                    |
			// ----------------------------------------
			//	 6 |         7          |  8
			//
			//	Approximated by simply offsetting the sides by r.


			float X = q._x;
			float Y = q._y;
			const float D = 2.f * r;

			if ( _size._x < D ) {
				X = _minPt._x + _size._x * 0.5f;
			} else {
				float minX = _minPt._x + r;
				float maxX = minX + _size._x - D;
				if ( q._x < minX ) {
					X = minX;
				} else if ( q._x > maxX ) {
					X = maxX;
				}
			}
			if ( _size._y < D ) {
				Y = _minPt._y + _size._y * 0.5f;
			} else {
				float minY = _minPt._y + r;
				float maxY = minY + _size._y - D;
				if ( q._y < minY ) {
					Y = minY;
				} else if ( q._y > maxY ) {
					Y = maxY;
				}
			}

			return Vector2( X, Y );		
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 AABBGoal::getCentroid() const {
			return _minPt + Vector2( _size.x() * 0.5f, _size.y() * 0.5f ); 
		}

		/////////////////////////////////////////////////////////////////////

		void AABBGoal::drawGLGeometry() const {
			glBegin( GL_POLYGON );
				glVertex3f( _minPt.x(), 0.f, _minPt.y() );
				glVertex3f( _minPt.x() + _size.x(), 0.f, _minPt.y() );
				glVertex3f( _minPt.x() + _size.x(), 0.f, _minPt.y() + _size.y() );
				glVertex3f( _minPt.x(), 0.f, _minPt.y() + _size.y() );
				glVertex3f( _minPt.x(), 0.f, _minPt.y() );
			glEnd();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBGoalFactory
		/////////////////////////////////////////////////////////////////////

		AABBGoalFactory::AABBGoalFactory() : GoalFactory() {
			_minXID = _attrSet.addFloatAttribute( "min_x", true /*required*/ );
			_minYID = _attrSet.addFloatAttribute( "min_y", true /*required*/ );
			_maxXID = _attrSet.addFloatAttribute( "max_x", true /*required*/ );
			_maxYID = _attrSet.addFloatAttribute( "max_y", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool AABBGoalFactory::setFromXML( Goal * goal, TiXmlElement * node, const std::string & behaveFldr ) const {
			
			AABBGoal * aabbGoal = dynamic_cast< AABBGoal * >( goal );
			assert( aabbGoal != 0x0 && "Trying to set AABB goal attributes on an incompatible object." );
			if ( ! GoalFactory::setFromXML( aabbGoal, node, behaveFldr ) ) return false;

			aabbGoal->setMinimum( _attrSet.getFloat( _minXID ), _attrSet.getFloat( _minYID ) );
			aabbGoal->setMaximum( _attrSet.getFloat( _maxXID ), _attrSet.getFloat( _maxYID ) );
			return true;
		}
		
		/////////////////////////////////////////////////////////////////////

		
	}	// namespace BFSM
}	// namespace Menge
