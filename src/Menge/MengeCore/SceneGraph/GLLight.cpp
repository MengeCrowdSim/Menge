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

#include "GLLight.h"
#include "graphCommon.h"

namespace Menge {

	namespace SceneGraph {

		////////////////////////////////////////////////////////////////
		//			Implementation of GLLight
		////////////////////////////////////////////////////////////////

		GLLight::GLLight() {
			setDefaults();
		}

		////////////////////////////////////////////////////////////////

		void GLLight::setDefaults() {
			_diff[0] = _diff[1] = _diff[2] = _diff[3] = 1.f;
			_pos[0] = _pos[1] = _pos[2] = 1.f;
			_pos[3] = 0.0;	// directional light
		}

		////////////////////////////////////////////////////////////////

		void GLLight::setColor( float r, float g, float b, float a ) {
			_diff[0] = r;
			_diff[1] = g;
			_diff[2] = b;
			_diff[3] = a;
		}

		////////////////////////////////////////////////////////////////

		void GLLight::setPosition( float x, float y, float z, float w ) {
			_pos[0] = x;
			_pos[1] = y;
			_pos[2] = z;
			if ( w >= 0.f ) {
				_pos[3] = w;
			}
		} 

		////////////////////////////////////////////////////////////////

		void GLLight::setLightType( LightType lightType ) {
			switch( lightType ) {
				case POINT:
					_pos[3] = 1.f;
					break;
				case DIRECTIONAL:
					_pos[3] = 0.f;
					break;
			}
		}

		////////////////////////////////////////////////////////////////

		void GLLight::initGL( int i, LightSpace space ) const {
			if ( _space == space ) {
				glEnable( GL_LIGHT0 + i );
				glLightfv( GL_LIGHT0 + i, GL_DIFFUSE, &_diff[0] );
				glLightfv( GL_LIGHT0 + i, GL_POSITION, &_pos[0] );
				// todo: add ambient and specular
				//glLightfv( GL_LIGHT0 + i, GL_AMBIENT, &_ambient[0] );
				//glLightfv( GL_LIGHT0 + i, GL_SPECULAR, &_specular[0] );
			}
		}
	}	// namespace SceneGraph
}	// namespace Menge