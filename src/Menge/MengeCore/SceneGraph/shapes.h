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
 *	@file		shapes.h
 *	@brief		A library of simple renderable OpenGL shapes
 */

#ifndef __SHAPES_H__
#define __SHAPES_H__

// A collection of renderable shapes
#include "CoreConfig.h"
#include "Select.h"
#include "GLNode.h"

#include "graphCommon.h"

namespace Menge {

	namespace SceneGraph {

		/*!
		 *	@brief		Initializes the static functions for all shapes (as appropriate).
		 *
		 *				When a new shape is created with a static "init" function, a call
		 *				to an initialization function should be added into this function 
		 *				(defined in shapes.cpp). This allows for the drawing of shapes via 
		 *				static functions as well as by instances.
		 *
		 *				It means the draw function doesn't have to test at each draw 
		 *				command if it has been properly initialized.
		 */
		MENGE_API void initShapes();

		/*!
		 *	@brief		Emits OpenGL commands to create a sphere in OpenGL
		 *
		 *	The sphere is constructed with N longitudinal and latitudinal points.  The
		 *	sphere has normals sufficient to support lighting.
		 *
		 *	@param		samples		The number of longitudinal and latitudinal samples.
		 */
		MENGE_API void glSphere( int samples );

		/*!
		 *	@brief		The basic shape class - a selectable GLNode.
		 */
		class MENGE_API Shape: public GLNode, public Selectable {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			Shape(): GLNode(), Selectable(), _r(1.f), _g(1.f), _b(1.f), _style(GL_FILL) {}

			/*!
			 *	@brief		Constructor with color and drawing style.
			 *
			 *	@param		r		The red component of the shape's color (in the range [0, 1]).
			 *	@param		g		The green component of the shape's color (in the range [0, 1]).
			 *	@param		b		The blue component of the shape's color (in the range [0, 1]).
			 *	@param		s		OpenGL render style: GL_FILL, GL_LINE, or GL_POINT.
			 */
			Shape( float r, float g, float b, GLenum s=GL_FILL ): GLNode(), Selectable(), _r(r), _g(g), _b(b), _style(s) {}
			
			/*!
			 *	@brief		Set the shape color from a vector.
			 *
			 *	@param		vec		The rgb color, where each channel lies in the range [0, 1].
			 */
			void setColor( const Vector3 & vec ) { _r=vec.x(); _g=vec.y(); _b=vec.z(); }

			/*!
			 *	@brief		Set the shape color from three separate values.
			 *
			 *	@param		r		The red component of the shape's color (in the range [0, 1]).
			 *	@param		g		The green component of the shape's color (in the range [0, 1]).
			 *	@param		b		The blue component of the shape's color (in the range [0, 1]).
			 */
			void setColor( float r, float g, float b ) { _r=r; _g=g; _b=b; }

			/*!
			 *	@brief		Set the shape's render style.
			 *
			 *	@param		style		The desired render style.  Should be one of GL_FILL, GL_LINE, or GL_POINT.
			 */
			void setStyle( GLenum style ) { _style = style; }

		protected:
			/*!
			 *	@brief		The red component of the shape's color.
			 */
			float	_r;
			
			/*!
			 *	@brief		The green component of the shape's color.
			 */
			float	_g;
			
			/*!
			 *	@brief		The blue component of the shape's color.
			 */
			float	_b;

			/*!
			 *	@brief		The shape's OpenGL rendering style.  Should be one of GL_FILL, GL_LINE, or GL_POINT.
			 */
			GLenum	_style;
		};

		/*!
		 *	@brief		A GLNode for drawing circles.  The circle is drawn around
		 *				the world space origin, lying on the x-z plane.
		 */
		class MENGE_API Circle : public Shape {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			Circle():Shape(), _radius(1.f){}
			
			/*!
			 *	@brief		Constructor with color.
			 *
			 *	@param		r		The red component of the circle's color (in the range [0, 1]).
			 *	@param		g		The green component of the circle's color (in the range [0, 1]).
			 *	@param		b		The blue component of the circle's color (in the range [0, 1]).
			 */
			Circle(float r, float g, float b):Shape(r,g,b), _radius(1.f) {}

			/*!
			 *	@brief		Function for drawing a circle into the context.
			 *
			 *	The circle is drawn on the x-z OpenGL plane, centered on the origin.  To change
			 *	position or orientation, the draw call should be preceeded by appropriate transforms.
			 *
			 *	@param		select		True if the circle is being drawn for selection purposes, 
			 *							false otherwise.
			 */
			void drawGL( bool select=false );

			/*!
			 *	@brief		Static function for drawing circles in the context with out instances.
			 *
			 *	The circle is drawn on the x-z OpenGL plane, centered on the origin.  To change
			 *	position or orientation, the draw call should be preceeded by appropriate transforms.
			 *
			 *	@param		radius	The radius of the circle.
			 *	@param		r		The red component of the shape's color (in the range [0, 1]).
			 *	@param		g		The green component of the shape's color (in the range [0, 1]).
			 *	@param		b		The blue component of the shape's color (in the range [0, 1]).
			 *	@param		a		The alpha componet of the shape's color (in the range [0, 1]).
			 *	@param		style	OpenGL render style: GL_FILL, GL_LINE, or GL_POINT.
			 */
			static void drawCircle( float radius, float r, float g, float b, float a, GLenum style=GL_FILL );

			/*!
			 *	@brief		Simply draws the underlying primitive
			 */
			static void drawUnit();

			/*!
			 *	@brief		Initializes the OpenGL primitives for drawing the circle.
			 *				
			 *	A call to this function has been added to initShapes.
			 */
			static void init();

		protected:
			/*!
			 *	@brief		The radius of the circle (in world space units).
			 */
			float	_radius;

			/*!
			 *	@brief		Determines if the Circle::newGLContext has been registered.
			 */
			static bool IS_REGISTERED;
			
			/*!
			 *	@brief		The function to call when an OpenGL context has changed.
			 */
			static void newGLContext();

			/*!
			 *	@brief		OpenGL display list for drawing a circle.
			 */
			static int  GL_ID;
		};

		/*!
		 *	@brief		A GLNode for drawing cylinders.  The cylinder has unit
		 *				radius and unit height and the bottom of the cylinder
		 *				sits on the origin.  The height is along the y-axis.
		 */
		class MENGE_API Cylinder : public Shape {
		public:
			/*!
			 *	@brief		Default constructor.  Unit size and default shape color
			 */
			Cylinder():Shape(), _radius(1.f), _height(1.f){}

			/*!
			 *	@brief		Size constructor.	Sets the size of the cylinder to the
			 *				specified radius and height and default shape color.
			 *	@param		radius		The radius of the cylinder.
			 *	@param		height		The height of the cylinder.
			 */
			Cylinder( float radius, float height ):Shape(), _radius(radius), _height(height){}

			/*!
			 *	@brief		Full constructor.  Sets the size and color of the cylinder
			 *	@param		radius		The radius of the cylinder.
			 *	@param		height		The height of the cylinder.
			 *	@param		r			The red component of the color (in the range [0,1] ).
			 *	@param		g			The green component of the color (in the range [0,1] ).
			 *	@param		b			The blue component of the color (in the range [0,1] ).
			 */
			Cylinder( float radius, float height, float r, float g, float b ):Shape(r,g,b), _radius(radius), _height(height){}

			/*!
			 *	@brief		The method for drawing the cylinder instance.
			 *	@param		select		Determines if the cylinder is being drawn in selection mode.
			 */
			void drawGL( bool select=false );

			/*!
			 *	@brief		Draw a cylinder without access to an instance
			 *
			 *	@param		radius		The radius of the cylinder.
			 *	@param		height		The height of the cylinder.
			 *	@param		r			The red component of the color (in the range [0,1] ).
			 *	@param		g			The green component of the color (in the range [0,1] ).
			 *	@param		b			The blue component of the color (in the range [0,1] ).
			 *	@param		a			The alpha component of the color (in the range [0,1] ).
			 *	@param		style		The draw style of the shape - enumerations consistent with glPolygonMode are
			 *							acceptable arguments.
			 */
			static void drawCylinder( float radius, float height, float r, float g, float b, float a, GLenum style=GL_FILL );

			/*!
			 *	@brief		The initialization of the cylinder construction.
			 */
			static void init();

		protected:
			/*!
			 *	@brief		The radius of the cylinder.
			 */
			float	_radius;

			/*!
			 *	@brief		The height of the cylinder.
			 */
			float	_height;

			// This functionality works with the context manager
			/*!
			 *	@brief		Reports if the class has been registered with the context manager
			 */
			static bool IS_REGISTERED;
			
			/*!
			 *	@brief		Called when the context changes.
			 */
			static void newGLContext();
			
			/*!
			 *	@brief		The identifier for the display list for this primitive.
			 */
			static int  GL_ID;
		};
	}
}	// namespace Menge
#endif  // __SHAPES_H__
