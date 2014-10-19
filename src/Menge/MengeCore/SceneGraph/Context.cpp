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

#include "Context.h"
#include "GLScene.h"
#include "graphCommon.h"
#include "Select.h"
#include "GL/glu.h"
#include <sstream>

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR Context
		///////////////////////////////////////////////////////////////////////////

		void Context::uiSetup( int vWidth, int vHeight ) {
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0.0, vWidth, 0.0, vHeight, -1.0f, 1.0f );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			
			glPushAttrib( GL_ENABLE_BIT );
			glDisable( GL_DEPTH_TEST );
		}

		///////////////////////////////////////////////////////////////////////////

		void Context::uiShutdown() {
			glPopAttrib();
			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
		}

		////////////////////////////////////////////////////////////////////////////

		void Context::getOpenGLView() {
			glGetDoublev( GL_MODELVIEW_MATRIX, &_modViewMat[0] );
			glGetDoublev( GL_PROJECTION_MATRIX, &_projMat[0] );
			glGetIntegerv( GL_VIEWPORT, &_viewMat[0] );
		}

		////////////////////////////////////////////////////////////////////////////

		void Context::writeText( const std::string & txt, const Vector2 & pos, bool currColor ) const {
			writeText( txt, Vector3( pos.x(), 0.f, pos.y() ), currColor );
		}

		////////////////////////////////////////////////////////////////////////////

		void Context::writeText( const std::string & txt, const Vector3 & pos, bool currColor ) const {
			TextWriter * writer = TextWriter::Instance();
			double winx, winy, winz;
			gluProject( pos.x(), pos.y(), pos.z(), _modViewMat, _projMat, _viewMat, &winx, &winy, &winz );
			writer->printText( txt, (float)winx, (float)winy, 15, currColor );
		}

		////////////////////////////////////////////////////////////////////////////

		void Context::writeTextRadially( const std::string & txt, const Vector2 & pos, const Vector2 & dir, bool currColor ) const {
			TextWriter * writer = TextWriter::Instance();
			double winx, winy, winz;
			gluProject( pos.x(), 0.f, pos.y(), _modViewMat, _projMat, _viewMat, &winx, &winy, &winz );
			float x = (float)winx;
			float y = (float)winy;
			Vector2 delta = pos + dir;
			gluProject( delta.x(), 0.f, delta.y(), _modViewMat, _projMat, _viewMat, &winx, &winy, &winz );
			float dx = (float)winx - x;
			float dy = (float)winy - y;
			
			float mag = sqrtf( dx * dx + dy * dy );
			float xWeight = 0.f;
			float yWeight = 0.f;
			if ( mag > 0.0001f) {
				dx /= mag;
				dy /= mag;
				const float SQRT_2 =  0.707106781f;	// sqrt(2)/2
				if ( fabs( dx ) >= SQRT_2 ) {
					if ( dx < 0.f ) {
						xWeight = 1.f;
					}
					yWeight = ( SQRT_2 - dy ) / ( 2.f * SQRT_2 );
				} else {							// y is the dominant direction
					if ( dy < 0.f ) {
						yWeight = 1.f;
					}
					xWeight = ( SQRT_2 - dx ) / ( 2.f * SQRT_2 );
				}
			}
			writer->printAnchorText( txt, x, y, xWeight, yWeight, 15, currColor );
		}	

		////////////////////////////////////////////////////////////////////////////

		void Context::writeAlignedText( const std::string & txt, const Vector2 & pos, TextWriter::Alignment align, bool currColor ) const {
			TextWriter * writer = TextWriter::Instance();
			double winx, winy, winz;
			gluProject( pos.x(), 0.f, pos.y(), _modViewMat, _projMat, _viewMat, &winx, &winy, &winz );
			writer->printText( txt, align, (float)winx, (float)winy, 15, currColor );
		}

		////////////////////////////////////////////////////////////////////////////

		void Context::writeToScreen( const std::string & txt, TextWriter::Alignment align, int fontSize, float hPad, float vPad, bool currColor, bool trans ) const {
			// draw text
			TextWriter * writer = TextWriter::Instance();
			writer->printAlignText( txt, align, fontSize, hPad, vPad, currColor, trans );
		}

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR SelectContext
		///////////////////////////////////////////////////////////////////////////

		bool SelectContext::selectGL( const GLScene * scene, const GLCamera & camera, int vWidth, int vHeight, int * selectPoint ) {
			// perform selection
			glPushAttrib( GL_ENABLE_BIT );
			glDisable( GL_LIGHTING );
			glDisable( GL_TEXTURE_2D );
			
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

			camera.setSelectMat( selectPoint );
			camera.setGLView();

			bool selChanged = false;

			Selectable::selectStart();
			drawUIGL( vWidth, vHeight, true );
			selChanged = Selectable::selectEnd();
			
			// Nothing selected on the UI
			if ( Selectable::getSelectedName() == 0 ) {
				// Treat all 3D objects (UI and scene) the same
				Selectable::selectStart();
				draw3DGL( true );
				GLNodeListCItr itr = scene->_nodes.begin();
				for ( ; itr != scene->_nodes.end(); ++itr ) {
					(*itr)->drawGL( true );
				}
				selChanged = Selectable::selectEnd();
			}
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			glPopMatrix();
			
			glPopAttrib();		
			return selChanged;
		}
	}	// namespace SceneGraph
}	// namespace Menge
