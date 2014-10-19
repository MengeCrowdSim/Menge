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
 *	@file		GLGroundPlane.h
 *	@brief		The definition of a grid visualization of the ground plane.
 */

#ifndef __GL_GROUND_PLANE_H__
#define __GL_GROUND_PLANE_H__

#include "CoreConfig.h"
#include "GLNode.h"

namespace Menge {

	namespace SceneGraph {

		/*!
		 *	@brief		A ground plane approximation -- z = 0, in world space.
		 *
		 *	The ground plane is a rectangular piece, centered on the origin, with user-specified
		 *	finite extent.  It is rendered as a grid of lines with a heavy center line, medium 
		 *	major lines, and light minor lines.  
		 */
		class MENGE_API GLGroundPlane: public GLNode {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		width		The extent of the plane along the world's x-axis.
			 *	@param		height		The extent of the plane along the world's y-axis.
			 *	@param		majorDist	The distance between major axis lines.
			 *	@param		minorCount	The number of minor lines between major axis lines.
			 */
			GLGroundPlane( float width, float height, float majorDist, int minorCount );

			/*!
			 *	@brief		Destructor.
			 */
			~GLGroundPlane(void);

			/*!
			 *	@brief		Updates the OpenGL constructs for the grid when the context changes.
			 */
			void newContext();
			
			/*!
			 *	@brief		Report the width of the ground plane (extent along world x-axis).
			 *
			 *	@returns	The plane segment's width.
			 */
			float getWidth() const { return _width; }

			/*!
			 *	@brief		Sets the width (the extent along the world's x-axis) of the plane.
			 *
			 *	@param		w		The width, in world space units, of the plane.
			 */
			void setWidth( float w );
			
			/*!
			 *	@brief		Report the height of the ground plane (extent along world y-axis).
			 *
			 *	@returns	The plane segment's height.
			 */
			float getHeight() const { return _height; }
			
			/*!
			 *	@brief		Sets the height (the extent along the world's y-axis) of the plane.
			 *
			 *	@param		h		The height, in world space units, of the plane.
			 */
			void setHeight( float h );
			
			/*!
			 *	@brief		Report the distance between major lines.
			 *
			 *	@returns	The distance between major lines on the plane.
			 */
			float getMajorDistance() const { return _majorDist; }
			
			/*!
			 *	@brief		Sets the distance between major lines.
			 *
			 *	@param		dist	The distance between major lines on the plane.
			 */
			void setMajorDistance( float dist );
			
			/*!
			 *	@brief		Report the number of minor lines between major lines.
			 *
			 *	@returns	The number of minor lines between major lines.
			 */
			int getMinorCount() const { return _minorCount; }
			
			/*!
			 *	@brief		Sets the number of minor lines between major lines.
			 *
			 *	@param		count	The number of minor lines between major lines.
			 */
			void setMinorCount( int count );

			/*!
			 *	@brief		Set the main color for the ground's lines.
			 *
			 *	@param		r		The red component of the color (in the range [0, 1] ).
			 *	@param		g		The green component of the color (in the range [0, 1] ).
			 *	@param		b		The blue component of the color (in the range [0, 1] ).
			 */
			void setLineColor( float r, float g, float b );

			/*!
			 *	@brief		Causes the plane to be drawn into the current OpenGL context.
			 *
			 *	@param		select		Indicates if it is being drawn for selection purposes
			 *							(true) or visualization (false).
			 */
			void drawGL( bool select=false );

		private:
			/*!
			 *	@brief		The width of the plane.  The extent of the plane along the
			 *				world's x-axis.
			 */
			float _width;
			
			/*!
			 *	@brief		The height of the plane.  The extent of the plane along the
			 *				world's y-axis.
			 */
			float _height;

			/*!
			 *	@brief		The distance between the medium-weight, major grid lines.
			 */
			float _majorDist;

			/*!
			 *	@brief		The number of light-weight, minor grid lines between major grid lines.
			 */
			int _minorCount;

			/*!
			 *	@brief		Emits the GL commands that draw the grid.
			 */
			void plotGL();

			/*!
			 *	@brief		Initializes the OpenGL state necessary for drawing the plane.
			 *				Should only be called after a valid OpenGL context has been
			 *				created.
			 */
			void initGL();	

			/*!
			 *	@brief		Frees up the plane's OpenGL resources from the current context.
			 */
			void clearGL();

			/*!
			 *	@brief		Identifier for this plane's OpenGL display list.  Each plane
			 *				gets its own display list.
			 */
			unsigned int GL_ID;
			
			/*!
			 *	@brief		The line color for this plane.
			 */
			Vector3	_lineColor;
		};

	}	// namespace SceneGraph
}	// namespace Menge
#endif // __GL_GROUND_PLANE_H__
