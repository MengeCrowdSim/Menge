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
 *	@file		FsmContext.h
 *	@brief		The definition of a basic UI context for finite
 *				state machine objects.
 */

#ifndef __FSM_CONTEXT_H__
#define	__FSM_CONTEXT_H__

#include "CoreConfig.h"
#include "Context.h"
#include "StateContext.h"

namespace Menge {

	// forward declarations
	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {

		// Forward declarations
		class FSM;
		class StateContext;

		/*!
		 *	@brief		Base context for finite state machine elements.
		 *
		 *	This differs from the standard scene graph context by being
		 *	dependent on an input agent.  
		 */
		class MENGE_API FsmContext : public SceneGraph::Context {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		fsm		A pointer to the underlying fsm.
			 *						The context will *not* delete the fsm.
			 */
			FsmContext( FSM * fsm );

			/*!
			 *	@brief		Destructor.
			 */
			virtual ~FsmContext();

			/*!
			 *	@brief		Give the context the opportunity to respond to a keyboard
			 *				event.
			 *
			 *	@param		e		The SDL event with the keyboard event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual SceneGraph::ContextResult handleKeyboard( SDL_Event & e );

			/*!
			 *	@brief		Draw UI elements into the context.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		vWidth		The width of the viewport (in pixels).
			 *	@param		vHeight		The height of the viewport (in pixels).
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void drawUIGL( const Agents::BaseAgent * agt, int vWidth, int vHeight, bool select=false );

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, bool select=false );

			/*!
			 *	@brief		Adds a state context to the fsm context.
			 *
			 *	Each call should provide a unique state.  In debug mode, this is tested using an
			 *	assertion.
			 *
			 *	These contexts will be deleted when the FsmContext is deleted.
			 *
			 *	@param		id			The globally unique id of the state.
			 *	@param		context		A pointer to the context for the given state.
			 */
			void addStateContext( size_t id, StateContext * context );

		protected:
			/*!
			 *	@brief		The underlying finite state machine.
			 */
			FSM * _fsm;

			/*!
			 *	@brief		Determines if the velocity component is displayed in the 3D context.
			 */
			bool	_drawVC;

			/*!
			 *	@brief		Determines if the transition is displayed in the 3D context.
			 */
			bool	_drawTrans;

			/*!
			 *	@brief		The context for the state currently being displayed.
			 */
			StateContext *	_currStateCtx;

			/*!
			 *	@brief		The contexts for the given states.
			 */
			StateContextMap	_states;

		};
	}	// namespace BFSM
}	// namespace Menge

#endif	// __FSM_CONTEXT_H__
