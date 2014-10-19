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

#include "Goals/GoalOBB.h"
#include "graphCommon.h"
#include "PrefVelocity.h"

namespace Menge  {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBGoal
		/////////////////////////////////////////////////////////////////////

		OBBGoal::OBBGoal():Goal(), _pivot(0.f,0.f), _size(0.f,0.f), _rot0(1.f,0.f), _rot1(0.f,1.f) {}

		/////////////////////////////////////////////////////////////////////

		float OBBGoal::squaredDistance( const Vector2 & pt ) const {
			// First rotate so that the OBB is an AABB then use the same logic as
			//	with the AABB
			Vector2 disp = pt - _pivot;
			float cosTheta = _rot0._x;
			float sinTheta = _rot1._x;
			// the LOCAL x- and y- coordinates of the nearest point, initialized to the
			//	local value of the query point.
			float X = disp.x() * cosTheta + disp.y() * sinTheta;
			float Y = disp.y() * cosTheta - disp.x() * sinTheta;
			
			// determine quadrant
			const float minX = 0.f;
			const float maxX = _size._x;
			const int xCoord = (int)(X > maxX ) - (int)( X < minX );
			const float minY = 0.f;
			const float maxY = _size._y;
			const int yCoord = (int)(Y > maxY ) - (int)( Y < minY );

			if ( xCoord == 0 && yCoord == 0 ) {
				return 0.f;
			} else {
				// the x- and y- coordinates of the nearest point
				X = ( xCoord == -1 ) * minX + ( xCoord == 1 ) * maxX;
				Y = ( yCoord == -1 ) * minY + ( yCoord == 1 ) * maxY;
				return absSq( Vector2( X, Y ) - disp );
			}
		}

		/////////////////////////////////////////////////////////////////////

		void OBBGoal::setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const {
			// First rotate so that the OBB is an AABB then use the same logic as
			//	with the AABB
			Vector2 disp = q - _pivot;
			float cosTheta = _rot0._x;
			float sinTheta = _rot1._x;
			// the LOCAL x- and y- coordinates of the nearest point, initialized to the
			//	local value of the query point.
			float X = disp.x() * cosTheta + disp.y() * sinTheta;
			float Y = disp.y() * cosTheta - disp.x() * sinTheta;
			
			// based on the voronoi regions of the AABB
			//	 (-1,1)  |        (0,1)        |  (1,1)
			// ----------------------------------------
			//	         |                     |
			//	 (-1, 0) |        (0,0)        |  (1,0)
			//	         |                     |
			// ----------------------------------------
			//	 (-1,-1) |       (0,-1)        |  (1,-1)
			//

			// the x- and y-coordintes of the left and right directions
			float xL, xR, yL, yR;	
			
			// determine quadrant
			const float D = 2.f * r;
			const float minX = r;
			const float maxX = _size._x - r;
			const int xCoord = (int)(X > maxX ) - (int)( X < minX );
			const float minY = r;
			const float maxY = _size._y - r;
			const int yCoord = (int)(Y > maxY ) - (int)( Y < minY );

			if ( xCoord == 0 && yCoord == 0 ) {
				// inside the region already
				directions.setSingle( Vector2(0.f, 0.f) );
				directions.setTarget( q );
			} else {
				int dimensions = 2;
				if ( _size._x < D ) {
					X = _size._x * 0.5f;
					xL = X;
					xR = X;
					--dimensions;
				} else {
					int LMaxXtest = ( yCoord == 1 ) | ( ( xCoord == 1 ) & ( yCoord == 0 ) );
					xL = maxX * LMaxXtest + ( 1 - LMaxXtest ) * minX;
					int RMaxXtest = ( yCoord == -1 ) | ( ( xCoord == 1 ) & ( yCoord == 0 ) );
					xR = maxX * RMaxXtest + ( 1 - RMaxXtest ) * minX;
					X = ( xCoord == -1 ) * minX + ( xCoord == 1 ) * maxX + ( xCoord == 0 ) * X;
				}
				if ( _size._y < D ) {
					Y = _size._y * 0.5f;
					yL = Y;
					yR = Y;
					--dimensions;
				} else {
					int LMaxYtest = ( xCoord == -1 ) | ( ( xCoord == 0 ) & ( yCoord == 1 ) );
					yL = maxY * LMaxYtest + ( 1 - LMaxYtest ) * minY;
					int RMaxYtest = ( xCoord == 1 ) | ( ( xCoord == 0 ) & ( yCoord == 1 ) );
					yR = maxY * RMaxYtest + ( 1 - RMaxYtest ) * minY;
					Y = ( yCoord == -1 ) * minY + ( yCoord == 1 ) * maxY + ( yCoord == 0 ) * Y;
				}

				Vector2 localPt( X, Y );
				Vector2 targetPt( _pivot + Vector2( localPt * _rot0, localPt * _rot1 ) );
				directions.setTarget( targetPt );
				Vector2 prefDir( norm( targetPt - q ) );
				if ( dimensions ) {
					// there is actually a span
					localPt.set( xL, yL );
					Vector2 leftPt( _pivot + Vector2( localPt * _rot0, localPt * _rot1 ) );
					localPt.set( xR, yR );
					Vector2 rightPt( _pivot + Vector2( localPt * _rot0, localPt * _rot1 ) );
					directions.setSpan( norm( leftPt - q ),
										norm( rightPt - q ),
										prefDir );
				} else {
					directions.setSingle( prefDir );
				}
			}
		}
		
		/////////////////////////////////////////////////////////////////////

		Vector2 OBBGoal::getTargetPoint( const Vector2 & q, float r ) const {
			// First rotate so that the OBB is an AABB then use the same logic as
			//	with the AABB
			Vector2 disp = q - _pivot;
			float cosTheta = _rot0._x;
			float sinTheta = _rot1._x;
			float X = disp.x() * cosTheta + disp.y() * sinTheta;
			float Y = disp.y() * cosTheta - disp.x() * sinTheta;
			
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


			const float D = 2.f * r;

			if ( _size._x < D ) {
				X = _size._x * 0.5f;
			} else {
				float minX = r;
				float maxX = _size._x - r;
				if ( X < minX ) {
					X = minX;
				} else if ( X > maxX ) {
					X = maxX;
				}
			}
			if ( _size._y < D ) {
				Y = _size._y * 0.5f;
			} else {
				float minY = r;
				float maxY = _size._y - r;
				if ( Y < minY ) {
					Y = minY;
				} else if ( Y > maxY ) {
					Y = maxY;
				}
			}

			Vector2 localPt( X, Y );

			return _pivot + Vector2( localPt * _rot0, localPt * _rot1 );	
		}

		/////////////////////////////////////////////////////////////////////

		Vector2 OBBGoal::getCentroid() const {
			return _pivot + _rot0 * ( _size.x() * 0.5f ) + _rot1 * ( _size.y() * 0.5f );
		}

		/////////////////////////////////////////////////////////////////////

		void OBBGoal::drawGLGeometry() const{
			Vector2 c( _size.x(), 0.f );
			Vector2 c1( c * _rot0, c * _rot1 );
			c.set( _size );
			Vector2 c2( c * _rot0, c * _rot1 );
			c.set( 0.f, _size.y() );
			Vector2 c3( c * _rot0, c * _rot1 );

			glPushMatrix();
			glTranslatef( _pivot.x(), 0.f, _pivot.y() );
			glBegin( GL_POLYGON );
				glVertex3f( 0.f, 0.f, 0.f );
				glVertex3f( c1.x(), 0.f, c1.y() );
				glVertex3f( c2.x(), 0.f, c2.y() );
				glVertex3f( c3.x(), 0.f, c3.y() );
				glVertex3f( 0.f, 0.f, 0.f );
			glEnd();
			glPopMatrix();
		}

		/////////////////////////////////////////////////////////////////////

		void OBBGoal::setRotation( float angle ) {
			float cTheta = cos( angle );
			float sTheta = sin( angle );
			_rot0.set( cTheta, -sTheta );
			_rot1.set( sTheta, cTheta );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBGoalFactory
		/////////////////////////////////////////////////////////////////////

		OBBGoalFactory::OBBGoalFactory() : GoalFactory() {
			_xID = _attrSet.addFloatAttribute( "x", true /*required*/ );
			_yID = _attrSet.addFloatAttribute( "y", true /*required*/ );
			_wID = _attrSet.addFloatAttribute( "width", true /*required*/ );
			_hID = _attrSet.addFloatAttribute( "height", true /*required*/ );
			_aID = _attrSet.addFloatAttribute( "angle", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool OBBGoalFactory::setFromXML( Goal * goal, TiXmlElement * node, const std::string & behaveFldr ) const {
			
			OBBGoal * obbGoal = dynamic_cast< OBBGoal * >( goal );
			assert( obbGoal != 0x0 && "Trying to set OBB goal attributes on an incompatible object." );
			if ( ! GoalFactory::setFromXML( obbGoal, node, behaveFldr ) ) return false;

			obbGoal->setPivot( _attrSet.getFloat( _xID ), _attrSet.getFloat( _yID ) );
			obbGoal->setSize( _attrSet.getFloat( _wID ), _attrSet.getFloat( _hID ) );
			obbGoal->setRotation( _attrSet.getFloat( _aID ) * DEG_TO_RAD );

			return true;
		}
		
		/////////////////////////////////////////////////////////////////////

		
	}	// namespace BFSM
}	// namespace Menge 