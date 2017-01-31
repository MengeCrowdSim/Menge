/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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
 *  @file       ZanlungoAgentContext.h
 *  @brief      A basic context for interacting with and displaying
 *				Zanlungo agent parameters
 */

#ifndef __ZANLUNGO_AGENT_CONTEXT_H__
#define __ZANLUNGO_AGENT_CONTEXT_H__

#include "ZanlungoAgent.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"

namespace Zanlungo {
	// forward declarations
	class Agent;

	/*!
	 *	@brief		The context for displaying the computational aspects of the
	 *				Zanlungo model (see Agents::Zanlungo::Agent).
	 */
	class AgentContext : public MengeVis::Runtime::BaseAgentContext {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		AgentContext();

		/*!
		 *	@brief		Sets the agent for this context.
		 *
		 *	This method works in conjunction with the VisElementDatabase. When this
		 *	visualization element is triggered, the database will supply the triggering
		 *	element.
		 *
		 *	@param		agent		The agent to interact with.
		 */
		virtual void setElement( MengeVis::Runtime::VisAgent * agent );

		/*!
		 *	@brief		Returns the name of the context for display.
		 *
		 *	@returns		The name of this context.
		 */
		virtual std::string contextName() const { return "Zanlungo 2010"; }

		/*!
		 *	@brief		The value used to store this element in the visual element database.
		 *				This string value should correspond to the getStringId method of the
		 *				corresponding simulation element.
		 */
		virtual std::string getElementName() const { return Agent::NAME; }

		/*!
		 *	@brief		Give the context the opportunity to respond to a keyboard
		 *				event.
		 *
		 *	@param		e		The SDL event with the keyboard event data.
		 *	@returns	A ContextResult instance reporting if the event was handled and
		 *				if redrawing is necessary.
		 */
		virtual MengeVis::SceneGraph::ContextResult handleKeyboard( SDL_Event & e );

	protected:
		/*!
		 *	@brief		Draw context elements into the 3D world.
		 *
		 *	@param		select		Defines if the drawing is being done for selection
		 *							purposes (true) or visualization (false).
		 */
		virtual void draw3DGL( bool select=false );

		/*!
		 *	@brief		Creates a formatted string to be printed in the context
		 *				for a particular agent.
		 *
		 *	@param		agent		The agent whose data is to be displayed.
		 *	@returns	A formatted string for display in the context's 2D gui.
		 */
		virtual std::string agentText( const Menge::Agents::BaseAgent * agent ) const;

		/*!
		 *	@brief		Determines whether the time to interaction is displayed
		 */
		bool	_showTTI;

		/*!
		 *	@brief		The id of the time to interaction structure shown.
		 *				If zero, all agents, if positive, a single agent
		 *				if negative, an obstacle
		 */
		int		_ttiObject;

		/*!
		 *	@brief		Function for illustrating time to interaction computation.
		 *
		 *	@param		agt			The agent whose time to interaction is displayed.
		 */
		void drawTTI( const Agent * agt );

		/*!
		 *	@brief		Determines if the force vectors are drawn
		 */
		bool		_showForce;

		/*!
		 *	@brief		Function for drawing the force vectors acting on agt
		 *
		 *	@param		agt			The agent whose forces are displayed.
		 */
		void drawForce( const Agent * agt );

		/*!
		 *	@brief		Draw the force vector from srcAgent acting on agt
		 *
		 *	@param		agt			The agent on which the force is acting
		 *	@param		other		The agent imparting the force
		 *	@param		TTI			Time to interaction
		 */
		void singleAgentForce( const Agent * agt, const Agent * other, float TTI );

		/*!
		 *	@brief		Draws the orientation of the agent
		 *
		 *	@param		agt			The agent whose orientation displayed.
		 */
		void drawOrientationFan( const Agent * agt );
	};
}	// namespace Zanlungo

#endif // __ZANLUNGO_AGENT_CONTEXT_H__
