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

#include "Geometry2D.h"
#include <cassert>

namespace Menge {

	namespace Math {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of CircleShape
		/////////////////////////////////////////////////////////////////////

		CircleShape::CircleShape( const CircleShape & shape ) {
			_center = shape._center;
			_radSqd = shape._radSqd;
		}

		/////////////////////////////////////////////////////////////////////

		CircleShape::CircleShape( const CircleShape & shape, const Vector2 & offset ) {
			_center = shape._center + offset;
			_radSqd = shape._radSqd;
		}

		/////////////////////////////////////////////////////////////////////

		CircleShape CircleShape::operator+( const Vector2 & pt ) {
			CircleShape circle( *this );
			circle._center += pt;
			return circle;
		}

		/////////////////////////////////////////////////////////////////////

		bool CircleShape::containsPoint( const Vector2 & pt ) const {
			float distSq = absSq( pt - _center );
			return distSq < _radSqd;
		}

		/////////////////////////////////////////////////////////////////////

		bool CircleShape::containsPoint( const Vector2 & pt, const Vector2 & pos ) const {
			float distSq = absSq( pt - pos );
			return distSq < _radSqd;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBShape
		/////////////////////////////////////////////////////////////////////

		AABBShape::AABBShape():Geometry2D(), _minPt(0.f,0.f), _maxPt(0.f,0.f), _halfSize(0.f,0.f) {
		}

		/////////////////////////////////////////////////////////////////////

		AABBShape::AABBShape( const Vector2 & minPt, const Vector2 & maxPt ):Geometry2D(), _minPt(minPt), _maxPt(maxPt) {
			assert( _minPt.x() <= _maxPt.x() && _minPt.y() <= _maxPt.y() && "Improper definition of AABB" );
			_halfSize = (maxPt-minPt) * 0.5f;
		}

		/////////////////////////////////////////////////////////////////////

		AABBShape::AABBShape( const AABBShape & shape ) {
			_minPt = shape._minPt;
			_maxPt = shape._maxPt;
			_halfSize = shape._halfSize;
		}

		/////////////////////////////////////////////////////////////////////

		AABBShape::AABBShape( const AABBShape & shape, const Vector2 & offset ) {
			_minPt = shape._minPt + offset;
			_maxPt = shape._maxPt + offset;
			_halfSize = shape._halfSize;
		}

		/////////////////////////////////////////////////////////////////////

		AABBShape AABBShape::operator+( const Vector2 & pt ) {
			AABBShape aabb( *this );
			aabb._minPt += pt;
			aabb._maxPt += pt;
			return aabb;
		}

		/////////////////////////////////////////////////////////////////////

		bool AABBShape::containsPoint( const Vector2 & pt ) const {
			const float x = pt.x();
			const float y = pt.y();
			return ( x >= _minPt.x() && x <= _maxPt.x() &&
					 y >= _minPt.y() && y <= _maxPt.y() );
		}

		/////////////////////////////////////////////////////////////////////

		bool AABBShape::containsPoint( const Vector2 & pt, const Vector2 & pos ) const {
			const float x = pt.x();
			const float y = pt.y();
			Vector2 minPt = pos - _halfSize;
			Vector2 maxPt = pos + _halfSize;
			return ( x >= minPt.x() && x <= maxPt.x() &&
					 y >= minPt.y() && y <= maxPt.y() );
		}

		/////////////////////////////////////////////////////////////////////

		void AABBShape::set( const Vector2 & minPt, const Vector2 & maxPt ) {
			_minPt.set( minPt );
			_maxPt.set( maxPt );
			_halfSize = ( _maxPt - _minPt ) * 0.5f;
		}

		/////////////////////////////////////////////////////////////////////

		void AABBShape::setSize( const Vector2 & size ) {
			_maxPt = _minPt + size;
			_halfSize = size * 0.5f;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBShape
		/////////////////////////////////////////////////////////////////////

		OBBShape::OBBShape():Geometry2D(), _pivot(0.f,0.f), _size(0.f,0.f), _cosTheta(1.f), _sinTheta(0.f) {
		}

		/////////////////////////////////////////////////////////////////////

		OBBShape::OBBShape( const Vector2 & pivot, const Vector2 & size, float angle ):Geometry2D(), _pivot(pivot), _size(size), _cosTheta(cos(angle)), _sinTheta(sin(angle)) {
		}

		/////////////////////////////////////////////////////////////////////

		OBBShape::OBBShape( const OBBShape & shape ) {
			_pivot = shape._pivot;
			_size = shape._size;
			_halfSize = shape._halfSize;
			_cosTheta = shape._cosTheta;
			_sinTheta = shape._sinTheta;
		}

		/////////////////////////////////////////////////////////////////////

		OBBShape::OBBShape( const OBBShape & shape, const Vector2 & offset ) {
			_pivot = shape._pivot + offset;
			_size = shape._size;
			_halfSize = shape._halfSize;
			_cosTheta = shape._cosTheta;
			_sinTheta = shape._sinTheta;
		}

		/////////////////////////////////////////////////////////////////////

		OBBShape OBBShape::operator+( const Vector2 & pt ) {
			OBBShape obb( *this );
			obb._pivot += pt;
			return obb;
		}

		/////////////////////////////////////////////////////////////////////

		bool OBBShape::containsPoint( const Vector2 & pt ) const {
			// Offset position to the space of the oriented bounding box.
			Vector2 disp = pt - _pivot;
			// rotate NEGATIVELY
			const float x = disp.x() * _cosTheta + disp.y() * _sinTheta;
			const float y = disp.y() * _cosTheta - disp.x() * _sinTheta;
			return ( x >= 0.f && x <= _size.x() &&
					 y >= 0.f && y <= _size.y() );
		}

		/////////////////////////////////////////////////////////////////////

		bool OBBShape::containsPoint( const Vector2 & pt, const Vector2 & pos ) const {
			// point relative to the center
			//	Scaled by 2 so I can use the size value directly to do the test
			Vector2 p = ( pt - pos ) * 2.f;
			// rotate it
			const float x = p.x() * _cosTheta + p.y() * _sinTheta;
			const float y = p.y() * _cosTheta - p.x() * _sinTheta;
			
			return ( x >= -_size.x() && x <= _size.x() &&
					 y >= -_size.y() && y <= _size.y() );
		}

		/////////////////////////////////////////////////////////////////////

		void OBBShape::set( const Vector2 & pivot, float width, float height, float angle ) {
			_pivot.set( pivot );
			_size.set( width, height );
			_cosTheta = cos( angle );
			_sinTheta = sin( angle );
		}

		/////////////////////////////////////////////////////////////////////

		void OBBShape::setAngle( float angle ) {
			_cosTheta = cos( angle );
			_sinTheta = sin( angle );
		}
	}	// namespace Math
}	// namespace Menge