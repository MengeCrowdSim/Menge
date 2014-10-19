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

#include "graphCommon.h"
#include "GLGroundPlane.h"
#include <math.h>

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//					Implementation of GLGroundPlane             
		///////////////////////////////////////////////////////////////////////////

		GLGroundPlane::GLGroundPlane( float width, float height, float majorDist, int minorCount ): GLNode(),
			_width( width ), _height( height ), _majorDist( majorDist ), _minorCount( minorCount ), GL_ID( 0 ),
		_lineColor(0.f, 0.f, 0.f) {
		}

		///////////////////////////////////////////////////////////////////////////

		GLGroundPlane::~GLGroundPlane(void) {
			if ( GL_ID ) clearGL();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::setWidth( float w ) { 
			if ( _width != w ) clearGL();
			_width = w; 
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::setHeight( float h ) { 
			if ( _height != h ) clearGL();
			_height = h; 
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::setMajorDistance( float dist ) { 
			if ( _majorDist != dist ) clearGL();
			_majorDist = dist; 
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::setMinorCount( int count ) { 
			if ( _minorCount != count ) clearGL();
			_minorCount = count; 
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::drawGL( bool select ) {
			if ( !select ) {
				glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
				glDisable(GL_LIGHTING);
				glDisable(GL_CULL_FACE);
				glDisable( GL_TEXTURE_2D );
				glEnable( GL_BLEND );
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthMask( false );
				glCallList( GL_ID );
				glDepthMask( true );
				glPopAttrib();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::plotGL() {
			float halfWidth = (float)_width * 0.5f;
			float halfHeight = (float)_height * 0.5f;

			const float DEPTH = 0.f;
			float gridMaxW = floor( halfWidth / _majorDist ) * _majorDist;
			float gridMaxH = floor( halfHeight / _majorDist ) * _majorDist;

			glPushAttrib( GL_CURRENT_BIT );
			// minor lines
			glColor4f( _lineColor.x(), _lineColor.y(), _lineColor.z(), 0.05f );
			glLineWidth( 1.0f );
			float minorDist = _majorDist / ( _minorCount + 1 );
			// horizontal lines
			float val = -gridMaxH;
			glBegin( GL_LINES );
			int lineCount = 0;
			while ( val <= gridMaxH ) {
				if ( lineCount % ( _minorCount + 1 ) ) {
					glVertex3f( -gridMaxW, DEPTH, val );
					glVertex3f( gridMaxW, DEPTH, val );
				}
				lineCount += 1;
				val += minorDist;
			}
			// vertical lines
			val = -gridMaxW;
			lineCount = 0;
			while ( val <= gridMaxW ) {
				if ( lineCount % ( _minorCount + 1 ) ) {
					glVertex3f( val, DEPTH, -gridMaxH );
					glVertex3f( val, DEPTH, gridMaxH );
				}
				lineCount += 1;
				val += minorDist;
			}

			// major lines
			glColor4f( _lineColor.x(), _lineColor.y(), _lineColor.z(), 0.1f );
			glLineWidth( 2.0f );
			// horizontal
			val = -gridMaxH;
			while ( val <= gridMaxH ) {
				glVertex3f( -gridMaxW, DEPTH, val );
				glVertex3f( gridMaxW, DEPTH, val );
				val += _majorDist;
			}
			// vertical
			val = -gridMaxW;
			while ( val <= gridMaxW ) {
				glVertex3f( val, DEPTH, -gridMaxH );
				glVertex3f( val, DEPTH, gridMaxH );
				val += _majorDist;
			}
			
			// center lines
			glColor4f( _lineColor.x(), _lineColor.y(), _lineColor.z(), 0.2f );
			glLineWidth( 3.0f );
			// vertical
			glVertex3f( 0.0f, DEPTH, -gridMaxH );
			glVertex3f( 0.0f, DEPTH, gridMaxH );
			// horizontal
			glVertex3f( -gridMaxW, DEPTH, 0.0f );
			glVertex3f( gridMaxW, DEPTH, 0.0f );

			glEnd();
			glPopAttrib();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::initGL() {
			if ( GL_ID ) {
				clearGL();
			}
			GL_ID = glGenLists( 1 );
			glNewList( GL_ID, GL_COMPILE );
			// draw grid
			plotGL();
			glEndList ();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::clearGL() {
			glDeleteLists( GL_ID, 1 );
			GL_ID = 0;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::newContext() {
			GL_ID = 0;
			initGL();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLGroundPlane::setLineColor( float r, float g, float b ) {
			_lineColor.set( r, g, b );
			initGL();
		}

	}	// namespace SceneGraph
}	// namespace Menge