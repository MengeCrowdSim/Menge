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

#include "MengeVis/VisConfig.h"
#include "MengeVis/SceneGraph/Context.h"
#include <string>

// forward declarations
namespace Menge {
	namespace Agents {
		class BaseAgent;
	}
}

namespace MengeVis {
	// forward declarations
	namespace SceneGraph {
		class GLScene;
		class GLCamera;
	}

	namespace Runtime {
		// forward declarations
		class VisAgent;

		/*!
		 *	@brief		Context class for displaying various characteristics
		 *				of the Agents::BaseAgent class.
		 */
		class MENGEVIS_API BaseAgentContext : public SceneGraph::SelectContext {
		public:

			/*!
			 *	@brief		Default constructor
			 */
			BaseAgentContext();

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~BaseAgentContext();

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
			 *	@brief		Sets the agent for this context.
			 *
			 *	This method works in conjunction with the VisElementDatabase. When this
			 *	visualization element is triggered, the database will supply the triggering
			 *	element.
			 *
			 *	@param		agent		The agent to interact with.
			 */
			virtual void setElement( const VisAgent * agent ) { _selected = agent; }

			/*!
			*	@brief		Returns the name of the context for display.
			*
			*	@returns		The name of this context.
			*/
			virtual std::string contextName() const { return "BaseAgent"; }

			/*!
			 *	@brief		The value used to store this element in the visual element database.
			 *				This string value should correspond to the getStringId method of the
			 *				corresponding simulation element.
			 */
			virtual std::string getElementName() const { return "base_agent"; }

			/*!
			*	@brief		Creates a formatted string to be printed in the context
			*				for a particular agent
			*
			*	@param		agent		The agent whose data is to be displayed.
			*	@returns	A formatted string for display in the context's 2D gui.
			*/
			virtual std::string agentText( const  Menge::Agents::BaseAgent * agent ) const;

		protected:
			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void draw3DGL( bool select = false );

			/*!
			 *	@brief		The drawing depth for the 3D elements
			 */
			static const float Y;

			/*!
			 *	@brief		The currently selected visualization agent.
			 */
			const VisAgent * _selected;

			/*!
			 *	@brief		Determines if the neighbor distance is rendered
			 */
			bool	_showNbrRadius;

			/*!
			 *	@brief		Function for drawing neighbor radius
			 */
			void drawNbrRadius( const  Menge::Agents::BaseAgent * agt );

			/*!
			 *	@brief		Determines if the neighbors are indicated in the view
			 */
			bool	_showNbr;

			/*!
			 *	@brief		Function for drawing neighbor distances
			 */
			void drawNeighbors( const  Menge::Agents::BaseAgent * agt );

			/*!
			 *	@brief		Determines if the circle of maximum speed is displayed
			 */
			bool	_showMaxSpd;

			/*!
			 *	@brief		Function for drawing neighbor distances
			 */
			void drawMaxSpeed( const  Menge::Agents::BaseAgent * agt );

			/*!
			 *	@brief		Determines if the current velocity is displayed
			 */
			bool	_showVel;

			/*!
			 *	@brief		Function for drawing current velocity
			 */
			void drawVelocity( const Menge::Agents::BaseAgent * agt );

			/*!
			 *	@brief		Determines if the preferred velocity is displayed
			 */
			bool	_showPrefVel;

			/*!
			 *	@brief		Function for drawing the preferred velocity
			 */
			void drawPrefVelocity( const Menge::Agents::BaseAgent * agt );

			/*!
			 *	@brief		Determines if the orientation of the agent is displayed.
			 */
			bool	_showOrient;

			/*!
			 *	@brief		Function for drawing current orientation
			 */
			void drawOrientation( const Menge::Agents::BaseAgent * agt );
		};
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __BASE_AGENT_CONTEXT_H__
