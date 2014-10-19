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

#include "GLScene.h"
#include "graphCommon.h"
#include "GLCamera.h"
#include "Select.h"
#include <vector>

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR GLScene
		///////////////////////////////////////////////////////////////////////////

		GLScene::GLScene():_uiContext(0x0) {
		}

		///////////////////////////////////////////////////////////////////////////
		GLScene::~GLScene() {
			GLNodeListItr itr = _nodes.begin();
			for ( ; itr != _nodes.end(); ++itr ) {
				delete (*itr);
			}
			
			SystemListItr sItr = _systems.begin();
			for ( ; sItr != _systems.end(); ++sItr ) {
				delete (*sItr);
			}
			
			if ( _uiContext ) delete _uiContext;
		}

		///////////////////////////////////////////////////////////////////////////
		
		void GLScene::finish() {
			SystemListItr sItr = _systems.begin();
			for ( ; sItr != _systems.end(); ++sItr ) {
				(*sItr)->finish();
			}
		}
		
		///////////////////////////////////////////////////////////////////////////
		
		void GLScene::newGLContext() {
			GLNodeListItr itr = _nodes.begin();
			for ( ; itr != _nodes.end(); ++itr ) {
				(*itr)->newContext();
			}
			if ( _uiContext ) {
				_uiContext->newGLContext();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLScene::drawGL( const GLCamera & camera, const GLLightVector & lights, int vWidth, int vHeight ) {
			glLoadIdentity();
			camera.setGLView();
			int LIGHT_COUNT = (int)lights.size();
			for ( int i = 0; i < LIGHT_COUNT; ++i ) {
				lights[ i ].initGL( i, GLLight::WORLD );
			}

			GLNodeListItr itr = _nodes.begin();
			
			for ( ; itr != _nodes.end(); ++itr ) {
				(*itr)->drawGL( false );
			}

			if ( _uiContext ) {
				_uiContext->drawGL( vWidth, vHeight );
			}
			
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLScene::selectGL( const GLCamera & camera, int vWidth, int vHeight, int * selectPoint ) {	
			if ( _uiContext ) {
				return _uiContext->selectGL( this, camera, vWidth, vHeight, selectPoint );
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLScene::updateScene( float time ) {
			bool redraw = false;
			SystemListItr itr = _systems.begin();
			for ( ; itr != _systems.end(); ++itr ) {
				redraw = (*itr)->updateScene( time ) || redraw;
			}
			if ( _uiContext ) _uiContext->update();
			return redraw;
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLScene::addNode( GLNode * node, GLDagNode * parent ) {
			if ( !parent ) {
				_nodes.push_back( node );		
			} else {
				parent->addChild( node );
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLScene::addSystem( System * system ) {
			_systems.push_back( system );
			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLScene::setContext( Context * context ) {
			if ( _uiContext ) {
				_uiContext->deactivate();
				delete _uiContext;
			}
			_uiContext = context;
			_uiContext->activate();
		}

		///////////////////////////////////////////////////////////////////////////

		ContextResult GLScene::handleMouse( SDL_Event & e ) {
			if ( _uiContext ) return _uiContext->handleMouse( e );
			return ContextResult( false, false );
		}

		///////////////////////////////////////////////////////////////////////////

		ContextResult GLScene::handleKeyboard( SDL_Event & e ) {
			if ( _uiContext ) return _uiContext->handleKeyboard( e );
			return ContextResult( false, false );
		}

	}	// namespace SceneGraph
}	// namespace Menge