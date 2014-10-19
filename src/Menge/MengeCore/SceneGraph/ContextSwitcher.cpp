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

#include "ContextSwitcher.h"

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR ContextSwitcher
		///////////////////////////////////////////////////////////////////////////

		ContextSwitcher::ContextSwitcher(): Context(), _activeContext(0x0) {
		}

		///////////////////////////////////////////////////////////////////////////

		ContextSwitcher::~ContextSwitcher() {
			if ( _activeContext ) {
				_activeContext->deactivate();
			}
			KeyContextMap::iterator itr = _contexts.begin();
			for ( ; itr != _contexts.end(); ++itr ) {
				delete itr->second;
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void ContextSwitcher::addContext( Context * context, SDLKey key ) {
			KeyContextMap::iterator itr = _contexts.find( key );
			if ( itr != _contexts.end() ) {
				delete itr->second;
			}
			_contexts[ key ] = context;
		}

		///////////////////////////////////////////////////////////////////////////

		void ContextSwitcher::newGLContext() {
			KeyContextMap::iterator itr = _contexts.begin();
			for ( ; itr != _contexts.end(); ++itr ) {
				itr->second->newGLContext();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		bool ContextSwitcher::switchContexts( Context * context ) {
			if ( _activeContext != context ) {
				if ( _activeContext ) {
					_activeContext->deactivate();
				}
				_activeContext = context;
				if ( _activeContext ) {
					_activeContext->activate();
				}
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////

		void ContextSwitcher::update() {
			if ( _activeContext ) {
				_activeContext->update();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		ContextResult ContextSwitcher::handleKeyboard( SDL_Event & e ) {
			// The work flow is like this:
			//		Escape is the only key that the switcher will definitely interpret
			//		If the escape key is selected, the active context is cleared
			//	If the key is not escape:
			//		If there is an active context, it gets the chance to handle it
			//		If the active context doesn't handle it, then the switcher tries
			//			switching
			//		otherwise, it returns back to the caller
			ContextResult result( false, false );
			SDLMod mods = e.key.keysym.mod;
			bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
			bool hasAlt = ( mods & KMOD_ALT ) > 0;
			bool hasShift = ( mods & KMOD_SHIFT ) > 0;
			bool noMods = !(hasCtrl || hasAlt || hasShift );
			if ( e.type == SDL_KEYDOWN ) {
				if ( e.key.keysym.sym == SDLK_ESCAPE && noMods) {
					bool changed = switchContexts( 0x0 );
					result.set( true, changed );
				}
				
				if ( ! result.isHandled() ) {
					if ( _activeContext ) {
						result = _activeContext->handleKeyboard( e );
					} else {
						KeyContextMap::iterator itr = _contexts.find( e.key.keysym.sym );
						if ( itr != _contexts.end() ) {
							Context * ctx = itr->second;
							bool changed = switchContexts( ctx );

							return ContextResult( true, changed );
						}
					}
				}
			}
			return result;
		}

		///////////////////////////////////////////////////////////////////////////

		ContextResult ContextSwitcher::handleMouse( SDL_Event & e ) {
			if ( _activeContext ) {
				return _activeContext->handleMouse( e );
			} else {
				return ContextResult( false, false );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void ContextSwitcher::drawGL( int vWidth, int vHeight ) {
			if ( _activeContext ) {
				_activeContext->drawGL( vWidth, vHeight );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		bool ContextSwitcher::selectGL( const GLScene * scene, const GLCamera & camera, int vWidth, int vHeight, int * selectPoint ) {
			if ( _activeContext ) {
				return _activeContext->selectGL( scene, camera, vWidth, vHeight, selectPoint );
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////

	}	// namespace SceneGraph
}	// namespace Menge