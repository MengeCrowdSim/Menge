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
 *  @file       HelbingAgentContext.h
 *  @brief      A basic context for interacting with and displaying
 *				Helbing agent parameters
 */

#ifndef __HELBING_AGENT_CONTEXT_H__
#define __HELBING_AGENT_CONTEXT_H__

#include "BaseAgentContext.h"
#include "Obstacle.h"

using namespace Menge;

namespace Helbing {
	// forward declaration
	class Agent;
	/*!
	 *	@brief		The context for displaying the computational aspects of the
	 *				Zanlungo model (see Agents::Helbing::Agent).
	 */
	class AgentContext : public BaseAgentContext {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		agents		An array of pointers to VisAgent instances for Helbing
		 *							agents.
		 *	@param		agtCount	The number of agents contained in the array.
		 */
		AgentContext( VisAgent ** agents, unsigned int agtCount );

		/*!
		 *	@brief		Returns the name of the context for display.
		 *
		 *	@returns		The name of this context.
		 */
		virtual std::string contextName() const { return "Helbing 2000"; }

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
		 *	@brief		Allow the context to update any time-dependent state it might have to
		 *				the given global time.
		 */
		virtual void update(); 

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
		 *				for a particular agent
		 *
		 *	@param		agent		The agent whose data is to be displayed.
		 *	@returns	A formatted string for display in the context's 2D gui.
		 */
		virtual std::string agentText( const Agents::BaseAgent * agent ) const;

		/*!
		 *	@brief		Determines if the force vectors are drawn
		 */
		bool		_showForce;

		/*!
		 *	@brief		The id of the forceObject to display.
		 *				If zero, all agents, if positive, a single agent
		 *				if negative, an obstacle
		 */
		int		_forceObject;

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
		 *	@param		thresh		The minimum force magnitude required to draw
		 */
		void singleAgentForce( const Agent * agt, const Agent * other, float thresh=0.5f );

		/*!
		 *	@brief		Draw the force vector from an obstacle acting on agt
		 *
		 *	@param		agt			The agent on which the force is acting
		 *	@param		obst		The obstacle imparting the force
		 *	@param		thresh		The minimum force magnitude required to draw
		 */
		void singleObstacleForce( const Agent * agt, const Agents::Obstacle * obst, float thresh=0.5f );

		/*!
		 *	@brief		Draws the given force on the given agent
		 *				It assumes the force color has already been set.
		 *
		 *	@param		agt			A pointer to the agent on which the force is acting
		 *	@param		force		The force vector to draw
		 *	@param		label		The label to apply to the force.
		 */
		void drawForce( const Agent * agt, const Vector2 & force, const std::string & label );

	};
}	// namespace Helbing
#endif	 // __HELBING_AGENT_CONTEXT_H__