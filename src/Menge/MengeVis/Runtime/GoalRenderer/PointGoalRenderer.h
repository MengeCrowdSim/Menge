/*!
 *	@file		PointGoalRenderer.h
 *	@brief		The definition of the renderer that draws a point goal the OpenGL context.
 */

#ifndef __POINT_GOAL_RENDERER_H__
#define __POINT_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {
			/*!
			 *	@brief		Class responsible for rendering point goals to the OpenGL context.
			 */
			class MENGEVIS_API PointGoalRenderer : public GoalRenderer {
				// Currently does nothing special beyond the base implementation.
			};
		}	// namespace GoalVis
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __POINT_GOAL_RENDERER_H__
