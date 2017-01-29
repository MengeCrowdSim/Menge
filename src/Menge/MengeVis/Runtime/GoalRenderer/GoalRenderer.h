#ifndef __GOAL_RENDERER_H__
#define __GOAL_RENDERER_H__

#include "MengeVis/VisConfig.h"

#include <string>

// Forward declaration

namespace Menge {
	namespace BFSM {
		class Goal;
	}
}

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {
			/*!
			 *	@brief		Class responsible for rendering a goal to the OpenGL context.
			 *
			 *	Each unique goal should be associated with a GoalRenderer that knows how to draw
			 *	it.  The GoalRenderer should be registered in the GoalRendererDB so that at
			 *	runtime, the right renderer can be associated with the right goal.
			 */
			class MENGEVIS_API GoalRenderer {
			public:
				/*!
				 *	@brief		Sets the agent for this context.
				 *
				 *	This method works in conjunction with the VisElementDatabase. When this
				 *	visualization element is triggered, the database will supply the triggering
				 *	element.
				 *
				 *	@param		agent		The agent to interact with.
				 */
				virtual void setElement( Menge::BFSM::Goal * goal ) { _goal = goal; }

				/*!
				 *	@brief		The value used to store this element in the visual element database.
				 *				This string value should correspond to the getStringId method of the
				 *				corresponding simulation element.
				 */
				virtual std::string getElementName() const { return "default"; }

				/*! 
				 *	@brief		The method to draw the associated geometry to the 3D context.
				 *
				 *	This draws the region as a shaded region with a highlighted outline.  It will
				 *	call doDrawGeometry() twice -- once for the filled region and once for the
				 *	outline.
				 */
				void drawGL() const;

			protected:
				/*!
				 *	@brief		The method for handling the details of a particular goal.
				 *				
				 *	This is called twice by drawGL().  If there is an expensive operation, it would
				 *  be best to be cached because it will be called twice every time a goal is
				 *	drawn.
				 */
				virtual void doDrawGeometry() const;

				/*! The goal to visualize. */
				Menge::BFSM::Goal * _goal;
			};
		}	// namespace GoalVis
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __GOAL_RENDERER_H__
