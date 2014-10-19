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

/*!
 *	@file		Geometry2D.h
 *	@brief		Definition of various shapes for defining spatial relationships.
 */

#ifndef __GEOMETRY_2D_H__
#define __GEOMETRY_2D_H__

#include "mengeCommon.h"

namespace Menge {

	namespace Math {

		/*!
		 *	@brief		Abstract 2d Geometry class for FSM queries.
		 *
		 *	Supports queries to determine if points lie inside/outside a particular
		 *	shape.
		 */
		class MENGE_API Geometry2D {
		public:
			/*!
			 *	@brief		Constructor
			 */
			Geometry2D(){}

			/*!
			 *	@brief		Simple destructor
			 */
			virtual ~Geometry2D(){}

			/*!
			 *	@brief		Determine if the point is inside the shape based on
			 *				the instance properties.
			 *
			 *	@param		pt		The point to test.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const = 0;

			/*! 
			 *	@brief		Determine if the point is inside the shape, not located at the
			 *				instance value, but at the given position.  Definition of "at the
			 *				given position" is defined by each shape.
			 *
			 *	@param		pt		The point to test.
			 *	@param		pos		The "position" of the shape.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt, const Vector2 & pos ) const = 0;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Circle shape
		 */
		class MENGE_API CircleShape : public Geometry2D {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			CircleShape(): _center(0.f,0.f), _radSqd(1.f) {}

			/*!
			 *	@brief		Constructor
			 *
			 *	@param		center		The position of the circle's center.
			 *	@param		radius		The radius of the circle.
			 */
			CircleShape( const Vector2 & center, float radius ):Geometry2D(), _center(center), _radSqd(radius*radius) {}
			
			/*!
			 *	@brief		Copy constructor
			 *
			 *	@param		shape		The shape to copy from.
			 */
			CircleShape( const CircleShape & shape );

			/*!
			 *	@brief		Initializes this shape as an translated version of the input shape.
			 *
			 *	@param		shape		The shape to copy from.
			 *	@param		offset		defines an offset from the copied CircleShape for the new CircleShape center
			 */
			CircleShape( const CircleShape & shape, const Vector2 & offset );

			/*!
			 *	@brief		Construct an offset version of this shape.
			 *
			 *	@param		pt		The offset value.
			 *	@returns	A new CircleShape offset from this one by the vector pt.
			 */
			CircleShape operator+( const Vector2 & pt );

			/*!
			 *	@brief		Sets the radius of the circle.
			 *
			 *	@param		radius		The circle's radius.
			 */
			void setRadius( float radius ) { _radSqd = radius * radius; }

			/*!
			 *	@brief		Sets the center of the circle.
			 *
			 *	@param		center		The circle's center.
			 */
			void setCenter( const Vector2 & center ) { _center.set( center ); }

			/*!
			 *	@brief		Sets the properties of the circle.
			 *
			 *	@param		center		The circle's center.
			 *	@param		radius		The circle's radius.
			 */
			void set( const Vector2 & center, float radius ) { _center.set( center ); _radSqd = radius * radius; }

			/*!
			 *	@brief		Determine if the point is inside the shape based on
			 *				the instance properties.
			 *
			 *	@param		pt		The point to test.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const;

			/*! 
			 *	@brief		Determine if the point is inside a circle, centered on the
			 *				given position.
			 *
			 *	@param		pt		The point to test.
			 *	@param		pos		The position of the circle's center.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt, const Vector2 & pos ) const;

		protected:
			/*!
			 *	@brief		Center of the circle
			 */
			Vector2 _center;

			/*!
			 *	@brief		Squared radius of the circle
			 */
			float	_radSqd;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Axis-aligned bounding box
		 */
		class MENGE_API AABBShape : public Geometry2D {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			AABBShape();

			/*!
			 *	@brief		Constructor
			 *
			 *	@param		minPt		The minimum values of the bounding box along
			 *							the x- and y-axes, respectively.
			 *	@param		maxPt		The maximum values of the bounding box along
			 *							the x- and y-axes, respectively.
			 */
			AABBShape( const Vector2 & minPt, const Vector2 & maxPt );
			
			/*!
			 *	@brief		Copy constructor
			 *
			 *	@param		shape		The shape to copy from.
			 */
			AABBShape( const AABBShape & shape );

			/*!
			 *	@brief		Initializes this shape as an translated version of the input shape.
			 *
			 *	@param		shape		The shape to copy from.
			 *	@param		offset		defines an offset from the copied AABBShape for the new AABBShape
			 */
			AABBShape( const AABBShape & shape, const Vector2 & offset );

			/*!
			 *	@brief		Construct an offset version of this shape.
			 *
			 *	@param		pt		The offset value.
			 *	@returns	A new AABBShape offset from this one by the vector pt.
			 */
			AABBShape operator+( const Vector2 & pt );

			/*!
			 *	@brief		Determine if the point is inside the shape based on
			 *				the instance properties.
			 *
			 *	@param		pt		The point to test.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const;

			/*! 
			 *	@brief		Determine if the point is inside a AABB, centered on the
			 *				given position.
			 *
			 *	@param		pt		The point to test.
			 *	@param		pos		The position of the circle.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt, const Vector2 & pos ) const;

			/*!
			 *	@brief		Sets the extent of the AABB.
			 *
			 *	@param		minPt		The minimum point (along the x- and y-axes).
			 *	@param		maxPt		The maximum point (along the x- and y-axes).
			 */
			void set( const Vector2 & minPt, const Vector2 & maxPt );

			/*!
			 *	@brief		Sets the size of the AABB.
			 *
			 *	It implicitly changes the maximum extent of the box, keeping the
			 *	minimum point where it is.
			 *
			 *	@param		size		The width and height of the box.
			 */
			void setSize( const Vector2 & size );

		protected:
			/*!
			 *	@brief		The minimum extent of the bounding box
			 */
			Vector2 _minPt;

			/*!
			 *	@brief		The maximum extent of the bounding box
			 */
			Vector2 _maxPt;

			/*!
			 *	@brief		The size of the bounding box (along the x- and y- axes).
			 */
			Vector2	_halfSize;
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Oriented bounding box
		 */
		class MENGE_API OBBShape : public Geometry2D {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			OBBShape();

			/*!
			 *	@brief		Constructor
			 *
			 *	@param		pivot		The minimum values of the unrotated bounding box along
			 *							the x- and y-axes, respectively.  The bounding box
			 *							gets rotated around this point.
			 *	@param		size		The size of the bounding box along the LOCAL x- and
			 *							y-axes, respectively.
			 *	@param		angle		The angle of rotation (in radians).
			 */
			OBBShape( const Vector2 & pivot, const Vector2 & size, float angle );
			
			/*!
			 *	@brief		Copy constructor
			 *
			 *	@param		shape		The shape to copy from.
			 */
			OBBShape( const OBBShape & shape );

			/*!
			 *	@brief		Initializes this shape as an translated version of the input shape.
			 *
			 *	@param		shape		The shape to copy from.
			 *	@param		offset		offset vector from the copied shape
			 */
			OBBShape( const OBBShape & shape, const Vector2 & offset );

			/*!
			 *	@brief		Construct an offset version of this shape.
			 *
			 *	@param		pt		The offset value.
			 *	@returns	A new OBBShape offset from this one by the vector pt.
			 */
			OBBShape operator+( const Vector2 & pt );
			
			/*!
			 *	@brief		Determine if the point is inside the shape based on
			 *				the instance properties.
			 *
			 *	@param		pt		The point to test.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const;

			/*! 
			 *	@brief		Determine if the point is inside a OBB, centered on the
			 *				given position.
			 *
			 *	@param		pt		The point to test.
			 *	@param		pos		The position of the circle.
			 *	@returns	True if the point is inside the shape, false otherwise.
			 */
			virtual bool containsPoint( const Vector2 & pt, const Vector2 & pos ) const;

			/*!
			 *	@brief		Sets the extent of the OBB.
			 *
			 *	@param		pivot		The pivot point of the OBB
			 *	@param		width		The width of the unrotated box (length along the x-axis).
			 *	@param		height		The width of the unrotated box (length along the x-axis).
			 *	@param		angle		The angle of rotation (in radians) around the pivot point.
			 */
			void set( const Vector2 & pivot, float width, float height, float angle );

			/*!
			 *	@brief		Sets the size of the OBB.
			 *
			 *	The pivot point and angle remain unchanged.
			 *
			 *	@param		size		The width and height of the box.
			 */
			void setSize( const Vector2 & size ) { _size.set( size ); }

			/*!
			 *	@brief		Sets the angle of the OBB.
			 *
			 *	The pivot point and size remain unchanged.
			 *
			 *	@param		angle		The angle (in radians) the OBB is rotated around its pivot.
			 */
			void setAngle( float angle );

		protected:
			/*!
			 *	@brief		The minimum corners of the bounding box (the box is rotated around this point).
			 */
			Vector2 _pivot;
			
			/*!
			 *	@brief		The width and height of the box (along its local coordinate system).
			 */
			Vector2 _size;

			/*!
			 *	@brief		The half width and height of the box.
			 */
			Vector2	_halfSize;

			/*!
			 *	@brief		The cosine of the box's angle of rotation
			 *				(used for accelerating queries).
			 */
			float _cosTheta;
			
			/*!
			 *	@brief		The sine of the box's angle of rotation
			 *				(used for accelerating queries).
			 */
			float _sinTheta;
		};
	}	// namespace Math
}	// namespace Menge

#endif	// __GEOMETRY_2D_H__
