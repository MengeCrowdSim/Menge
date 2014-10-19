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

#include "shapes.h"
#include "GLContextManager.h"

namespace Menge {

	namespace SceneGraph {

		void initShapes() {
			Circle::init();
			Cylinder::init();
		}

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR Shape primitives
		///////////////////////////////////////////////////////////////////////////

		void glSphere( int samples ) {
			float * c = new float[ samples * 4 ];
			float * s = c + samples;
			float * c_2 = s + samples;
			float * s_2 = c_2 + samples;

			float angle = TWOPI / samples;
			float angle2 = PI / (samples - 1);
			for ( int i = 0; i < samples; ++i ) {
				c[ i ] = cos( i * angle );
				s[ i ] = sin( i * angle );
				c_2[ i ] = cos( i * angle2 );
				s_2[ i ] = sin( i * angle2 );
			}

			// render sphere
			glBegin( GL_TRIANGLE_FAN );
			glNormal3f( 0.f, 1.f, 0.f );
			glVertex3f( 0.f, 1.f, 0.f );
			float sx = s_2[ 1 ];
			float cx = c_2[ 1 ];
				
			for ( int i = 0; i <= samples; ++i ) {
				// vertical normal rotated around the x-axis (towards positive z)
				//		[ -sx cx 0 ]^T
				// That rotated around vertical axis
				//		[ cysx  cx  -sxsy ]	// this is both the normal and the position
				int idx = i % samples;
				float sy = s[idx];
				float cy = c[idx];
				glNormal3f( cy * sx, cx, -sx * sy );
				glVertex3f( cy * sx, cx, -sx * sy );
			}
			glEnd();

			// center strips
			for ( int i = 1; i < samples - 1; ++i ) {
				glBegin( GL_QUAD_STRIP );
				float cx1 = c_2[ i ];
				float sx1 = s_2[ i ];
				float cx2 = c_2[ i + 1 ];
				float sx2 = s_2[ i + 1 ];
				for ( int j = 0; j <= samples; ++j ) {
					int idx = j % samples;
					float sy = s[idx];
					float cy = c[idx];
					glNormal3f( cy * sx1, cx1, -sx1 * sy );
					glVertex3f( cy * sx1, cx1, -sx1 * sy );
					glNormal3f( cy * sx2, cx2, -sx2 * sy );
					glVertex3f( cy * sx2, cx2, -sx2 * sy );
					
				}
				glEnd();
			}
			// bottom fan
			glBegin( GL_TRIANGLE_FAN );
			glNormal3f( 0.f, -1.f, 0.f );
			glVertex3f( 0.f, -1.f, 0.f );
			sx = s_2[ samples - 1 ];
			cx = c_2[ samples - 1 ];
				
			for ( int i = 0; i <= samples; ++i ) {
				// vertical normal rotated around the x-axis (towards positive z)
				//		[ -sx cx 0 ]^T
				// That rotated around vertical axis
				//		[ cysx  cx  -sxsy ]	// this is both the normal and the position
				int idx = i % samples;
				float sy = s[idx];
				float cy = c[idx];
				glNormal3f( cy * sx, cx, -sx * sy );
				glVertex3f( cy * sx, cx, -sx * sy );
			}
			glEnd();
			delete [] c;
		}

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR Circle Shape
		///////////////////////////////////////////////////////////////////////////

		bool Circle::IS_REGISTERED = false;
		int Circle::GL_ID = 0;

		///////////////////////////////////////////////////////////////////////////

		void Circle::newGLContext() {
			GL_ID = glGenLists( 1 );

			glNewList( GL_ID, GL_COMPILE );
			const int SAMPLE_COUNT = 24;
			
			// Simple circle
			float dTheta = TWOPI / ( SAMPLE_COUNT - 1 );
			glBegin( GL_POLYGON );
			for ( int i = 0; i < SAMPLE_COUNT; ++i ) {
				float theta = i * dTheta;
				float x = cos( theta );
				float y = sin( theta );
				glVertex3f( x, 0.f, y );
			}
			glEnd();

			glEndList();
		}

		///////////////////////////////////////////////////////////////////////////

		void Circle::init() {
			if ( ! IS_REGISTERED ) {
				GLContextManager::addCallback( &newGLContext );
				IS_REGISTERED = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void Circle::drawGL( bool select ) {
			if ( _visible ) {
				drawCircle( _radius, _r, _g, _b, 1.f, _style );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void Circle::drawCircle( float radius, float r, float g, float b, float a, GLenum style ) {
			glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT );
			// set up drawing
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable( GL_LIGHTING );
			glPolygonMode( GL_FRONT_AND_BACK, style );
			glColor4f(r,g,b,a);
			glPushMatrix();
			glScalef( radius, radius, radius );
			glCallList( GL_ID );
			glPopMatrix();
			glPopAttrib();
		}

		///////////////////////////////////////////////////////////////////////////

		void Circle::drawUnit() {
			glCallList( GL_ID );
		}

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR Cylinder Shape
		///////////////////////////////////////////////////////////////////////////

		bool Cylinder::IS_REGISTERED = false;
		int Cylinder::GL_ID = 0;

		///////////////////////////////////////////////////////////////////////////

		void Cylinder::newGLContext() {
			GL_ID = glGenLists( 1 );

			glNewList( GL_ID, GL_COMPILE );
			const int SAMPLE_COUNT = 24;
			// Collect the points
			
			float points[ SAMPLE_COUNT ][2];
			float dTheta = TWOPI / ( SAMPLE_COUNT - 1 );
			for ( int i = 0; i < SAMPLE_COUNT; ++i ) {
				float theta = i * dTheta;
				float x = cos( theta );
				float y = sin( theta );
				points[ i ][ 0 ] = x;
				points[ i ][ 1 ] = y;
			}

			// Simple cylinder
			// bottom face
			glBegin( GL_POLYGON );
			glNormal3f( 0.f, -1.f, 0.f );
			for ( int i = SAMPLE_COUNT - 1; i >= 0; --i ) {
				glVertex3f( points[ i ][ 0 ], 0.f, points[ i ][ 1 ] );
			}
			glEnd();
			
			// top face
			glNormal3f( 0.f, 1.f, 0.f );
			glBegin( GL_POLYGON );
			for ( int i = 0 ; i < SAMPLE_COUNT; ++i ) {
				glVertex3f( points[ i ][ 0 ], 1.f, points[ i ][ 1 ] );
			}
			glEnd();
			// walls
			glBegin( GL_TRIANGLE_STRIP );
			for ( int i = 0 ; i < SAMPLE_COUNT; ++i ) {
				glNormal3f( points[ i ][ 0 ], 0.f, points[ i ][ 1 ] );
				glVertex3f( points[ i ][ 0 ], 1.f, points[ i ][ 1 ] );
				glVertex3f( points[ i ][ 0 ], 0.f, points[ i ][ 1 ] );
			}
			glNormal3f( points[ 0 ][ 0 ], 0.f, points[ 0 ][ 1 ] );
			glVertex3f( points[ 0 ][ 0 ], 1.f, points[ 0 ][ 1 ] );
			glVertex3f( points[ 0 ][ 0 ], 0.f, points[ 0 ][ 1 ] );
			glEnd();

			glEndList();
		}

		///////////////////////////////////////////////////////////////////////////

		void Cylinder::init() {
			if ( ! IS_REGISTERED ) {
				GLContextManager::addCallback( &newGLContext );
				IS_REGISTERED = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void Cylinder::drawGL( bool select ) {
			if ( _visible ) {
				drawCylinder( _radius, _height, _r, _g, _b, 1.f, _style );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void Cylinder::drawCylinder( float radius, float height, float r, float g, float b, float a, GLenum style ) {
			glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT );
			// set up drawing
			glDisable( GL_TEXTURE_2D );
			if ( a < 1.f ) {
				glEnable( GL_BLEND );
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			glPolygonMode( GL_FRONT_AND_BACK, style );
			glColor4f(r,g,b,a);
			glPushMatrix();
			glScalef( radius, height, radius );
			glCallList( GL_ID );
			glPopMatrix();
			glPopAttrib();
		}
		
	}	// namespace SceneGraph
}	// namespace Menge
