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

#include "FsmContext.h"
#include "BaseAgent.h"
#include "FSM.h"
#include "State.h"
#include "StateContext.h"
#include <cassert>
#include <sstream>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of FsmContext
		/////////////////////////////////////////////////////////////////////

		FsmContext::FsmContext( FSM * fsm ):_fsm(fsm), _currStateCtx(0x0), _drawVC(true), _drawTrans(true) {
		}

		/////////////////////////////////////////////////////////////////////

		FsmContext::~FsmContext() {
			StateContextMap::iterator itr = _states.begin();
			for ( ; itr != _states.end(); ++itr ) {
				delete itr->second;
			}
		}

		/////////////////////////////////////////////////////////////////////

		SceneGraph::ContextResult FsmContext::handleKeyboard( SDL_Event & e ) {
			SceneGraph::ContextResult result( false, false );

			SDLMod mods = e.key.keysym.mod;
			bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
			bool hasAlt = ( mods & KMOD_ALT ) > 0;
			bool hasShift = ( mods & KMOD_SHIFT ) > 0;
			bool noMods = !(hasCtrl || hasAlt || hasShift );

			if ( e.type == SDL_KEYDOWN ) {
				if ( hasCtrl && e.key.keysym.sym == SDLK_v ) {
					_drawVC = !_drawVC;
					result.set( true, true );
				} else if ( hasAlt && e.key.keysym.sym == SDLK_t ) {
					_drawTrans = !_drawTrans;
					result.set( true, true );
				}
			}

			if ( ! result.isHandled() && _currStateCtx ) {
				result = _currStateCtx->handleKeyboard( e );
			}
			return result;
		}

		/////////////////////////////////////////////////////////////////////

		void FsmContext::drawUIGL( const Agents::BaseAgent * agt, int vWidth, int vHeight, bool select ) {
			if ( !select ) {
				std::stringstream ss;
				if ( agt ) {
					ss << "BFSM -- agent " << agt->_id << " selected";
					const State * state = _fsm->getCurrentState( agt );
					StateContextMap::const_iterator itr = _states.find( state->getID() );
					if ( itr == _states.end() ) {
						ss << "\n   in state with no context: " << state->getName() << "(" << state->getID() << ")";
						_currStateCtx = 0x0;
					} else { 
						_currStateCtx = itr->second;
						ss << "\n" << _currStateCtx->getUIText( "   " );
					}
				} else {
					_currStateCtx = 0x0;
					ss << "BFSM -- no agent selected";
				}
				writeToScreen( ss.str(), SceneGraph::TextWriter::RIGHT_TOP, 15, 10.f, 10.f );
			}
		}

		/////////////////////////////////////////////////////////////////////

		void FsmContext::draw3DGL( const Agents::BaseAgent * agt, bool select ) {
			if ( !select && agt ) {
				const State * state = _fsm->getCurrentState( agt );
				StateContextMap::const_iterator itr = _states.find( state->getID() );
				if ( itr != _states.end() ) {
					itr->second->draw3DGL( agt, _drawVC, _drawTrans );
				}
			}
		}

		/////////////////////////////////////////////////////////////////////

		void FsmContext::addStateContext( size_t id, StateContext * context ) {
			assert( _states.find( id ) == _states.end() && "Tried to register multiple contexts for one state" );
			_states[ id ] = context;
		}

	}	// namespace BFSM
}	// namespace Menge