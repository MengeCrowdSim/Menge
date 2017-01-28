/*!
 *	@file		OBBGoalRenderer.h
 *	@brief		The definition of the renderer that draws an OBB goal the OpenGL context.
 */

#ifndef __OBB_GOAL_RENDERER_H__
#define __OBB_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {
			/*!
			 *	@brief		Class responsible for rendering OBB goals to the OpenGL context.
			 */
			class MENGEVIS_API OBBGoalRenderer : public GoalRenderer {
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

#endif	// __OBB_GOAL_RENDERER_H__
