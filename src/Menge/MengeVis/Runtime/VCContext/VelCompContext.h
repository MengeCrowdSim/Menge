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
 *	@file		VelCompContext.h
 *	@brief		The definition of a basic UI context for finite
 *				state machine *velocity components*.
 */

#ifndef __VEL_COMP_CONTEXT_H__
#define	__VEL_COMP_CONTEXT_H__

#include "MengeVis/SceneGraph/Context.h"

#include <vector>

// Forward declarations
namespace Menge {
	namespace Agents {
		class BaseAgent;
		class PrefVelocity;
	}

	namespace BFSM {
		class VelComponent;
		class Goal;
	}
}

namespace MengeVis {
	namespace Runtime {
		namespace VCContext {

			/*!
			 *	@brief		Base context for finite state machine velocity components.
			 *
			 *	This differs from the standard scene graph context by being dependent on an input
			 *	agent.
			 */
			class MENGEVIS_API VelCompContext : public SceneGraph::Context {
			public:
				/*!
				 *	@brief		Constructor.
				 */
				VelCompContext() : _vc( 0x0 ) {}

			//	/*!
			//	 *	@brief		This supplants the destructor.
			//	 *
			//	 *	In order to preserve potential problems in windows when dlls do not share the
			//	 *	same c-runtime library, the destructor is held to be private.  To garbage
			//	 *	collect an VelComponent, the destroy method should be called (which in turn,
			//	 *	will call the destructor from its own memory space, averting run-time crashes).
			//	 *
			//	 *	Once this has been called, the VelComponent no longer exists.  Calling methods
			//	 *	or accessing members will produce indetermine behavior (most likely errors).
			//	 */
			//	void destroy() { delete this; }

			//protected:
				/*!
				 *	@brief		Destructor.
				 */
				virtual ~VelCompContext(){}

			public:

				/*!
				 *	@brief		The value used to store this element in the visual element database.
				 *				This string value should correspond to the getStringId method of the
				 *				corresponding simulation element.
				 */
				virtual std::string getElementName() const;

				/*!
				 *	@brief		Sets the agent for this context.
				 *
				 *	This method works in conjunction with the VisElementDatabase. When this
				 *	visualization element is triggered, the database will supply the triggering
				 *	element.
				 *
				 *	@param		agent		The agent to interact with.
				 */
				virtual void setElement( const Menge::BFSM::VelComponent * vc ) { _vc = vc; }

				/*!
				 *	@brief		Provides a string to be printed in the display as a UI element
				 *				detailing velocity component information.
				 *
				 *	@param		indent		An optional string representing indentation to be
				 *							applied to the text. It is prefixed at the start of
				 *							each line.
				 *	@returns	The string for printing on the UI layer.
				 */
				virtual std::string getUIText( const std::string & indent = "" ) const;

				/*!
				 *	@brief		Draw context elements into the 3D world.
				 *
				 *	This should never be called in select mode.
				 *
				 *	@param		agt			The particular agent for which the FSM is being
				 *							visualized.
				 *	@param		goal		The agent's goal (although this may be ignored).
				 */
				virtual void draw3DGL( const Menge::Agents::BaseAgent * agt,
									   const Menge::BFSM::Goal * goal ) {}
				// TODO: Consider having this actually *draw* something.

			protected:
				/*!
				 *	@brief		Draws the preferred velocity in a consistent way
				 *
				 *	@param		pVel		The preferred velocity of the agent computed by the vel
				 *							component.
				 *	@param		pos			The position of the velocity vector's origin.
				 *	@param		elevation	The optional elevation of the velocity.
				 */
				void drawPrefVel( const Menge::Agents::PrefVelocity & pVel,
								  const Menge::Math::Vector2 & pos, float elevation = 0.f );

				/*!
				 *	@brief		Draws the 2d goal point in a consistent manner
				 *
				 *	@param		goalPoint		The position of the agent's goal
				 *	@param		agent			The agent
				 */
				void drawGoal( const Menge::Math::Vector2 & goalPoint,
							   const Menge::Agents::BaseAgent * agent ) const;

				/*!
				 *	@brief		Draws the 3d goal point in a consistent manner
				 *
				 *	@param		agtRadius		The agent's radius
				 *	@param		goalPoint		The position of the goal
				 */
				void drawGoal( const Menge::Math::Vector3 & goalPoint,
							   float agtRadius = 0.19f ) const;

				/*! The velocity component interacting with this context. */
				const Menge::BFSM::VelComponent * _vc;
			};
		}	// namespace VCContext
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __VEL_COMP_CONTEXT_H__
