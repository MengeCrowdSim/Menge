/*!
 *	@file		CircleGoalRenderer.h
 *	@brief		The definition of the renderer that draws a circle goal the OpenGL context.
 */

#ifndef __CIRCLE_GOAL_RENDERER_H__
#define __CIRCLE_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {
			/*!
			 *	@brief		Class responsible for rendering circle goals to the OpenGL context.
			 */
			class MENGEVIS_API CircleGoalRenderer : public GoalRenderer {
			public:
				/*!
				 *	@brief		The value used to store this element in the visual element database.
				 *				This string value should correspond to the getStringId method of the
				 *				corresponding simulation element.
				 */
				virtual std::string getElementName() const;

			protected:
				/*!
				 *	@brief		The method for handling the details of a particular goal.
				 *
				 *	This is called twice by drawGL().  If there is an expensive operation, it would
				 *  be best to be cached because it will be called twice every time a goal is
				 *	drawn.
				 */
				virtual void doDrawGeometry() const;
			};
		}	// namespace GoalVis
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __CIRCLE_GOAL_RENDERER_H__
