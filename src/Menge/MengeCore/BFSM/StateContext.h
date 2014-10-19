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
 *	@file		StateContext.h
 *	@brief		The definition of a basic UI context for finite
 *				state machine *states*.
 */

#ifndef __STATE_CONTEXT_H__
#define	__STATE_CONTEXT_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Context.h"
#include "VelocityComponents/VelCompContext.h"

namespace Menge {

	// forward declaration
	namespace BFSM {
		class StateContext;
	}

	/*!
	 *  @brief  A map from state ids to statecontexts to facilitate lookups for a visualized agent
	 */
	typedef HASH_MAP< size_t, BFSM::StateContext * > StateContextMap;

	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {

		// Forward declarations
		class State;

		/*!
		 *	@brief		Base context for finite state machine states.
		 *
		 *	This differs from the standard scene graph context by being
		 *	dependent on an input agent.  
		 */
		class StateContext : public SceneGraph::Context {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		state		A pointer to the underlying fsm state.
			 *							The context will *not* delete the state.
			 */
			StateContext( State * state );

			/*!
			 *	@brief		Destructor.
			 */
			virtual ~StateContext();

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
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing state information.
			 *
			 *	@param		indent		An optional string representing indentation to be
			 *							applied to the text.  It is prefixed at the start
			 *							of each line.
			 *	@returns	The string for printing on the UI layer.
			 */
			std::string getUIText( const std::string & indent="" ) const;

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		drawVC		Draw the velocity component
			 *	@param		drawTrans	Draw the transition
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, bool drawVC, bool drawTrans );

		protected:
			/*!
			 *	@brief		The underlying finite state machine state.
			 */
			State * _state;

			/*!
			 *	@brief		The value used to indicate that no id is selected.
			 *		
			 *	Used in conjunction with the _activeVC and _activeTransition
			 */
			static size_t NO_ACTIVE_ID;

			/*!
			 *	@brief		The velocity component context for this state.
			 */
			VelCompContext * _vcContext;

			/*!
			 *	@brief		The id of the "active" transition.
			 *				
			 *	This is the index of the transition which is currently being
			 *	visualized in the context.
			 */
			size_t _activeTransition;
			
		};
	}	// namespace BFSM
}	// namespace Menge
#endif	// __STATE_CONTEXT_H__
