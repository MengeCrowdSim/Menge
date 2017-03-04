/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 *	@file		VisAgent.h
 *	@brief		Simple, cylindrical visualization for agents.
 */

#ifndef __VIS_AGENT_H__
#define __VIS_AGENT_H__

#include "MengeCore/Math/vector.h"
#include "MengeVis/VisConfig.h"
#include "MengeVis/SceneGraph/GLNode.h"
#include "MengeVis/SceneGraph/Select.h"

// forward declaration
namespace Menge {
	namespace Agents {
		class BaseAgent;
	}
}

namespace MengeVis {
	namespace Runtime {
		/*!
		 *	@brief		The basic agent visualization class: a selectable cylinder.
		 *
		 *	This is the basic visualization of simulation agents in the visualization
		 *  context.  The agents are drawn as 3D cylinders by default.  This class can
		 *	be sub-classed adn the drawGL method can be overridden to provide a different
		 *	visualization mechanism.
		 */
		class MENGEVIS_API VisAgent : public SceneGraph::GLNode, public SceneGraph::Selectable {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		agent		The agent to be visualized.
			 */
			VisAgent( Menge::Agents::BaseAgent * agent );

			/*!
			 *	@brief		Draw the agent into the 3D world.
			 *
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void drawGL( bool select = false );

			/*!
			 *	@brief		Returns a Agents::BaseAgent pointer of the associated simulation
			 *				agent.
			 *
			 *	@returns	A pointer to the visualization agent's simulation agent.
			 */
			const Menge::Agents::BaseAgent * getAgent() const { return _agent; }

			/*!
			 *	@brief		Set the 3D position of the visual agent.
			 *
			 *	@param		x		The x-position of the agent (in world coordinates).
			 *	@param		y		The y-position of the agent (in world coordinates).
			 *	@param		z		The z-position of the agent (in world coordinates).
			 */
			inline void setPosition( float x, float y, float z ) { _pos.set( x, y, z ); }

			/*!
			 *	@brief		String identifier of the underlying agent.
			 *
			 *	This permits mapping of visual element to the appropriate agent context.
			 *
			 *	@returns	The string identifier for the agent this visualization agent contains.
			 */
			std::string getStringId() const;

		protected:
			/*!
			 *	@brief		Defines the color of the cylinder.
			 *
			 *	The color is computed and returned via the three floats passed in as
			 *	parameters.
			 *
			 *	@param		r		The red component of the cylinder.
			 *	@param		g		The green component of the cylinder.
			 *	@param		b		The blue component of the cylinder.
			 */
			void getColor( float & r, float & g, float & b );

			/*!
			 *	@brief		The logical agent being visualized.
			 */
			Menge::Agents::BaseAgent *	_agent;

			/*!
			 *	@brief		The position in R3 of the logical agent.
			 */
			Menge::Math::Vector3	_pos;
		};
	}	// namespace Runtime
}	// namespace MengeVis

#endif //__VIS_AGENT_H__
