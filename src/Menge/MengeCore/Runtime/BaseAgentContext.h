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
 *  @file       BaseAgentContext.h
 *  @brief      A basic context for interacting with and displaying
 *				basic agent parameters
 */

#ifndef __BASE_AGENT_CONTEXT_H__
#define __BASE_AGENT_CONTEXT_H__

#include "CoreConfig.h"
#include "Context.h"
#include <string>

namespace Menge {

	namespace SceneGraph {
		class GLScene;
		class GLCamera;
	}

	namespace BFSM {
		class FsmContext;
	};

	namespace Agents {
		class BaseAgent;
	}

	class VisAgent;

	/*!
	 *	@brief		Context class for displaying various characteristics
	 *				of the Agents::BaseAgent class.
	 */
	class MENGE_API BaseAgentContext : public SceneGraph::SelectContext {
	public:
		/*!
		 *	@brief		Definition of the state of the context.
		 */
		enum BACState {
			DEFAULT_ST,			///< The default operating state
			TYPE_AGENT_ID_ST	///< Typing in an agent selection
		};

		/*!
		 *	@brief		Construtor
		 *
		 *	@param		agents		An array of pointers to VisAgent instances.
		 *	@param		agtCount	The number of agents contained in the array.
		 *	@param		fsmCtx		A context to visualize an agent's BFSM state.
		 */
		BaseAgentContext( VisAgent ** agents, size_t agtCount, BFSM::FsmContext * fsmCtx=0x0 );

		/*!
		 *	@brief		Virtual destructor.
		 */
		virtual ~BaseAgentContext();

		/*!
		 *	@brief		Sets the fsm context
		 *
		 *	@param		ctx			The context for the finite state machine.
		 */
		void setFSMContext( BFSM::FsmContext * ctx ) { _fsmCtx = ctx; }

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
		 *	@brief		The draw function for the context.
		 *
		 *	@param		vWidth		The width of the viewport (in pixels).
		 *	@param		vHeight		The height of the viewport (in pixels).
		 */
		virtual void drawGL( int vWidth, int vHeight );
		
		/*!
		 *	@brief		Performs selection based on a click on screen space.
		 *				Uses the OpenGL selection mechanism.
		 *
		 *	@param		scene			The scene to select in.
		 *	@param		camera			The camera.
		 *	@param		vWidth			The width of the viewport.
		 *	@param		vHeight			The height of the viewport.
		 *	@param		selectPoint		The point (in screen space) at which object selection
		 *								should take place.
		 *	@returns	A boolean indicating whether a redraw needs to take place.
		 */
		virtual bool selectGL( const SceneGraph::GLScene * scene, const SceneGraph::GLCamera & camera, int vWidth, int vHeight, int * selectPoint );

		/*!
		 *	@brief		Called when the context is activated.
		 */
		virtual void activate();
			
	protected:
		/*!
		 *	@brief		Draw UI elements into the context.
		 *
		 *	@param		vWidth		The width of the viewport (in pixels).
		 *	@param		vHeight		The height of the viewport (in pixels).
		 *	@param		select		Defines if the drawing is being done for selection
		 *							purposes (true) or visualization (false).
		 */
		virtual void drawUIGL( int vWidth, int vHeight, bool select=false );

		/*!
		 *	@brief		Draw context elements into the 3D world.
		 *
		 *	@param		select		Defines if the drawing is being done for selection
		 *							purposes (true) or visualization (false).
		 */
		virtual void draw3DGL( bool select=false );

		/*!
		 *	@brief		Returns the name of the context for display.
		 *
		 *	@returns		The name of this context.
		 */
		virtual std::string contextName() const { return "BaseAgent"; }

		/*!
		 *	@brief		Creates a formatted string to be printed in the context
		 *				for a particular agent
		 *
		 *	@param		agent		The agent whose data is to be displayed.
		 *	@returns	A formatted string for display in the context's 2D gui.
		 */
		virtual std::string agentText( const Agents::BaseAgent * agent ) const;

		/*!
		 *	@brief		The drawing depth for the 3D elements
		 */
		static const float Y;

		/*!
		 *	@brief		The currently selected visualization agent.
		 */
		VisAgent * _selected;

		/*!
		 *	@brief		The state of the context.
		 */
		BACState	_state;

		/*!
		 *	@brief		Determines if the neighbor distance is rendered	
		 */
		bool	_showNbrRadius;

		/*!
		 *	@brief		Function for drawing neighbor radius
		 */
		void drawNbrRadius( const Agents::BaseAgent * agt );

		/*!
		 *	@brief		Determines if the neighbors are indicated in the view
		 */
		bool	_showNbr;

		/*!
		 *	@brief		Function for drawing neighbor distances
		 */
		void drawNeighbors( const Agents::BaseAgent * agt );
		
		/*!
		 *	@brief		Determines if the circle of maximum speed is displayed
		 */
		bool	_showMaxSpd;

		/*!
		 *	@brief		Function for drawing neighbor distances
		 */
		void drawMaxSpeed( const Agents::BaseAgent * agt );
		
		/*!
		 *	@brief		Determines if the current velocity is displayed
		 */
		bool	_showVel;

		/*!
		 *	@brief		Function for drawing current velocity
		 */
		void drawVelocity( const Agents::BaseAgent * agt );
		
		/*!
		 *	@brief		Determines if the preferred velocity is displayed
		 */
		bool	_showPrefVel;

		/*!
		 *	@brief		Function for drawing the preferred velocity
		 */
		void drawPrefVelocity( const Agents::BaseAgent * agt );

		/*!
		 *	@brief		Determines if the orientation of the agent is displayed.
		 */
		bool	_showOrient;

		/*!
		 *	@brief		Function for drawing current orientation
		 */
		void drawOrientation( const Agents::BaseAgent * agt );

		/*!
		 *	@brief		A pointer to the agents in the scene
		 */
		VisAgent **	_visAgents;

		/*!
		 *	@brief		The number of agents in the scene
		 */
		size_t	_agtCount;

		/*!
		 *	@brief		The maximum number of digits for typing.
		 */
		static const unsigned int MAX_TYPE_DIGITS = 10;

		/*!
		 *	@brief		The character array for typing numbers into.
		 */
		char	_digits[ MAX_TYPE_DIGITS + 1 ];

		/*!
		 *	@brief		Number of typed digits.
		 */
		unsigned int	_digitCount;

		/*!
		 *	@brief		Begins the agent typing state.
		 */
		void beginIDTyping();

		/*!
		 *	@brief		Finish agent typing state.
		 */
		void finishIDTyping();

		/*!
		 *	@brief		Cancels the agent typing state.
		 */
		void cancelIDTyping();

		/*!
		 *	@brief		Draws the id typing state
		 */
		void drawIDTyping();

		/*!
		 *	@brief		Adds a digit to the typed value
		 */
		void addIDDigit( const char digit );

		/*!
		 *	@brief		An optional finite state machine context to visualize
		 *				the computation of agent behavior.
		 */
		BFSM::FsmContext * _fsmCtx;
	};

}	// namespace Menge

#endif	// __BASE_AGENT_CONTEXT_H__
