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
 *	@file		VisAgent.h
 *	@brief		Simple, cylindrical visualization for agents.
 */

#ifndef __VIS_AGENT_H__
#define __VIS_AGENT_H__

#include "CoreConfig.h"
#include "GLNode.h"
#include "Select.h"

namespace Menge {

	// forward declaration
	namespace Agents {
		class BaseAgent;
	}

	/*!
	 *	@brief		The basic agent visualization class: a selectable cylinder.
	 *
	 *	This is the basic visualization of simulation agents in the visualization 
	 *  context.  The agents are drawn as 3D cylinders by default.  This class can
	 *	be sub-classed adn the drawGL method can be overridden to provide a different
	 *	visualization mechanism.
	 */
	class MENGE_API VisAgent : public SceneGraph::GLNode, public SceneGraph::Selectable {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		agent		The agent to be visualized.
		 */
		VisAgent( Agents::BaseAgent * agent );

		/*!
		 *	@brief		Draw the agent into the 3D world.
		 *
		 *	@param		select		Defines if the drawing is being done for selection
		 *							purposes (true) or visualization (false).
		 */
		virtual void drawGL( bool select=false );

		/*! 
		 *	@brief		Returns a Agents::BaseAgent pointer of the associated simulation
		 *				agent.
		 *
		 *	@returns	A pointer to the visualization agent's simulation agent.
		 */
		Agents::BaseAgent * getAgent() { return _agent; }

		/*!
		 *	@brief		Set the 3D position of the visual agent.
		 *
		 *	@param		x		The x-position of the agent (in world coordinates).
		 *	@param		y		The y-position of the agent (in world coordinates).
		 *	@param		z		The z-position of the agent (in world coordinates).
		 */
		inline void setPosition( float x, float y, float z ) { _pos.set( x, y, z ); }

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
		Agents::BaseAgent *	_agent;

		/*!
		 *	@brief		The position in R3 of the logical agent.
		 */
		Vector3	_pos;
	};
}	// namespace Menge

#endif //__VIS_AGENT_H__
