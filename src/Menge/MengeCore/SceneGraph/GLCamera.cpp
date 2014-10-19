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
#include "GLCamera.h"
#include <GL/glu.h>
#include <math.h>

#include <iostream>

namespace Menge {

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	const Vector3 DEFAULT_POS = Vector3( 0.f, 1.f, -5.f );
	const Vector3 DEFAULT_TARGET = Vector3();
	const Vector3 DEFAULT_UP = Vector3(0.0, 1.0, 0.0 );

#endif	// DOXYGEN_SHOULD_SKIP_THIS

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//			            Implementation for  GLCamera                     
		///////////////////////////////////////////////////////////////////////////

		GLCamera::GLCamera(): _type(PERSP), _position(DEFAULT_POS), _target( DEFAULT_TARGET ), _up(DEFAULT_UP),
													_farPlane( 100.f ), _nearPlane(0.01f), _fov(45.f), _viewWidth(640), _viewHeight(480), _dirtyProj(false), _orthoHWidth(1.f) {}

		///////////////////////////////////////////////////////////////////////////

		GLCamera::GLCamera( const Vector3 & p): _type(PERSP), _position(p), _target( DEFAULT_TARGET ), _up(DEFAULT_UP),
													_farPlane( 100.f ), _nearPlane(0.01f), _fov(45.f), _viewWidth(640), _viewHeight(480), _dirtyProj(false), _orthoHWidth(1.f) {}

		///////////////////////////////////////////////////////////////////////////

		GLCamera::GLCamera( const Vector3 & p, const Vector3 & t): _type(PERSP), _position(p), _target( t ), _up(DEFAULT_UP),
													_farPlane( 100.f ), _nearPlane(0.01f), _fov(45.f), _viewWidth(640), _viewHeight(480), _dirtyProj(false), _orthoHWidth(1.f) {}

		///////////////////////////////////////////////////////////////////////////

		GLCamera::GLCamera( const Vector3 & p, const Vector3 & t, const Vector3 & u): _type(PERSP), _position(p), _target( t ), _up(u),
													_farPlane( 100.f ), _nearPlane(0.01f), _fov(45.f), _viewWidth(640), _viewHeight(480), _dirtyProj(false), _orthoHWidth(1.f) {}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setOrtho( float scaleFactor ) {
			_type = ORTHO;
			// Compute the ortho width
			_orthoHWidth = 0.5f * _position.distance( _target ) / scaleFactor ;
			_dirtyProj = true;
		} 

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setPersp() {
			_type = PERSP;
			_position = ( -2.f * _orthoHWidth ) * targetDir() + _target;
			_dirtyProj = true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setActive() {
			_dirtyProj = true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setPosition( const Vector3 & p ) {
			_position = p;
			_orthoHWidth = _position.distance( _target ) * 0.5f;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setPosition( float x, float y, float z ) {
			_position.set( x, y, z );
			_orthoHWidth = _position.distance( _target ) * 0.5f;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setTarget( const Vector3 & t ) {
			_target = t;
			_orthoHWidth = _position.distance( _target ) * 0.5f;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setTarget( float x, float y, float z ) {
			_target.set( x, y, z );
			_orthoHWidth = _position.distance( _target ) * 0.5f;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setUp( const Vector3 & u ) {
			_up = u;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setUp( float x, float y, float z ) {
			_up.set( x, y, z );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setViewport( int w, int h, int left, int bottom ) {
			_viewWidth = w;
			_viewHeight = h;
			glViewport( left, bottom, w, h );
			_dirtyProj = true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setProjMat( int w, int h ) const {
			_viewWidth = w;
			_viewHeight = h;
			glViewport( 0, 0, w, h );
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			_setProjMat();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setSelectMat( int * selectPoint ) const {
			const int PICK_SIZE = 5;
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			int viewport[4] = { 0, 0, _viewWidth, _viewHeight };
			gluPickMatrix( selectPoint[0], _viewHeight - selectPoint[1], PICK_SIZE, PICK_SIZE, viewport );
			_setProjMat();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::_setProjMat() const {
			if ( _type == PERSP ) {
				gluPerspective( _fov, (float)_viewWidth/(float)_viewHeight, _nearPlane, _farPlane );
			} else {
				float halfHeight = _orthoHWidth * (float)_viewHeight / (float)_viewWidth;
				glOrtho( -_orthoHWidth, _orthoHWidth,
						-halfHeight, halfHeight,
						_nearPlane, _farPlane );
			}
			glMatrixMode( GL_MODELVIEW );	
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::setGLView() const {
			// TODO: Make this vary according to perspective vs. ortho
			if ( _dirtyProj ) {
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				_setProjMat();
				_dirtyProj = false;
			}
			glLoadIdentity();
			gluLookAt( _position[0], _position[1], _position[2],
							   _target[0], _target[1], _target[2],
							   _up[0], _up[1], _up[2] );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::viewXAxis( bool downPositive ) {
			float camDist = _position.distance( _target );
			if ( downPositive ) {
				_position.set( _target.x() - camDist, _target.y(), _target.z() );
			} else {
				_position.set( _target.x() + camDist, _target.y(), _target.z() );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::viewYAxis( bool downPositive ) {
			float camDist = _position.distance( _target );
			if ( downPositive ) {
				_position.set( _target.x(), _target.y() - camDist, _target.z() + 0.01f );
			} else {
				_position.set( _target.x(), _target.y() + camDist, _target.z() + 0.01f );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::viewZAxis( bool downPositive ) {
			float camDist = _position.distance( _target );
			if ( downPositive ) {
				_position.set( _target.x(), _target.y(), _target.z() - camDist );
			} else {
				_position.set( _target.x(), _target.y(), _target.z() + camDist );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::pan( float angle ) {
			Vector3 targetDisp = _target - _position;
			targetDisp = targetDisp.rotateY( angle );
			_target = _position + targetDisp;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::tilt( float angle ) {
			Vector3 targetDisp = _target - _position;
			targetDisp = targetDisp.rotateV( angle, getRightDir() );
			_target = _position + targetDisp;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::truck( float amount ) {
			float scale;
			if ( _type == PERSP ) {
				scale = targetDistance();
			} else {
				scale = 2.f * _orthoHWidth;
			}
			Vector3 offset = getRightDir() * ( amount * scale );
			_target += offset;
			_position += offset;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::crane( float amount ) {
			float scale;
			if ( _type == PERSP ) {
				scale = targetDistance();
			} else {
				scale = 2.f * _orthoHWidth;
			}
			Vector3 offset = getActualUpDir() * ( amount * scale );
			_target += offset;
			_position += offset;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::dolly ( float amount ) {
			Vector3 offset = targetDir() * amount;
			_target += offset;
			_position += offset;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::dollyPlane( float amount ) {
			Vector3 offset = targetDir() * amount;
			offset[1] = 0.0;
			_target += offset;
			_position += offset;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::zoom( float amount ) {
			if ( _type == PERSP ) {
				float tgtDist = targetDistance();
				float scaledAmount = amount * tgtDist / 100.f;
				if ( (tgtDist - scaledAmount <= 0.1f ) ) 
					scaledAmount = tgtDist - 0.1f;
				Vector3 offset = targetDir() * scaledAmount;
				_position += offset;
			} else {
				float scaledAmount = amount * _orthoHWidth / 100.f;
				if ( _orthoHWidth - scaledAmount <= 0.1f ) {
					scaledAmount = _orthoHWidth - 0.1f;
				}
				_orthoHWidth -= scaledAmount;
				_dirtyProj = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////


		void GLCamera::orbitVerticalAxis( float angle ) {
			Vector3 cameraDir = _position - _target;
			cameraDir = cameraDir.rotateY( angle );
			_position = _target + cameraDir;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::orbitHorizontalAxis( float angle ) {
			Vector3 cameraDisp = _position - _target;
			Vector3 cameraDir = cameraDisp;
			cameraDir.normalize();
			float oldAngle = acos( cameraDir * Vector3( 0.0f, 1.0f, 0.0f ) );
			if ( oldAngle - angle >= PI ) {
				angle = oldAngle - PI  + 0.01f;
			} else if ( oldAngle - angle <= 0.0f ) {
				angle = oldAngle - 0.01f;
			}
			cameraDisp = cameraDisp.rotateV( angle, getRightDir() );
			_position = _target + cameraDisp;
		}

		///////////////////////////////////////////////////////////////////////////

		float GLCamera::targetDistance() const {
			return _target.distance( _position );
		}

		///////////////////////////////////////////////////////////////////////////

		Vector3 GLCamera::targetDir() const {
			Vector3 temp = _target - _position;
			temp.normalize();
			return temp;
		}

		///////////////////////////////////////////////////////////////////////////

		Vector3 GLCamera::getActualUpDir() const{
			Vector3 front = _target - _position;
			Vector3 right = front.cross( _up );
			right = right.cross( front );
			right.normalize();
			return right;
		}

		///////////////////////////////////////////////////////////////////////////

		Vector3 GLCamera::getRightDir() const{
			Vector3 targetDir = _target - _position;
			targetDir = targetDir.cross( _up);
			targetDir.normalize();
			return targetDir;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLCamera::outputState() const {
			std::cout << "<Camera";
			std::cout << " xpos=\"" << _position._x << "\"";
			std::cout << " ypos=\"" << _position._y << "\"";
			std::cout << " zpos=\"" << _position._z << "\"";
			std::cout << " xtgt=\"" << _target._x << "\"";
			std::cout << " ytgt=\"" << _target._y << "\"";
			std::cout << " ztgt=\"" << _target._z << "\"";
			std::cout << " far=\"" << _farPlane << "\"";
			std::cout << " near=\"" << _nearPlane << "\"";
			if ( _type == PERSP ) {
				std::cout << " fov=\"" << _fov << "\"";
			} else {
				std::cout << " fov=\"0.0\"";
				std::cout << " orthoScale=\"" << getOrthoScaleFactor() << "\"";
			}
			std::cout << " />\n";
		}

		///////////////////////////////////////////////////////////////////////////

		float GLCamera::getOrthoScaleFactor() const {
			float dist = targetDistance();
			return 0.5f * dist / _orthoHWidth;
		}

	}	// namespace SceneGraph

}	// namespace Menge