/*!
 *	@file		AABBGoalRenderer.h
 *	@brief		The definition of the renderer that draws an AABB goal the OpenGL context.
 */

#ifndef __AABB_GOAL_RENDERER_H__
#define __AABB_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {
			/*!
			 *	@brief		Class responsible for rendering AABB goals to the OpenGL context.
			 */
			class MENGEVIS_API AABBGoalRenderer : public GoalRenderer {
			public:
				/*!
				 *	@brief		The value used to store this element in the visual element database.
				 *				This string value should correspond to the getStringId method of the
				 *				corresponding simulation element.
				 */
				virtual std::string getElementName() const { return "aabb"; }

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

#endif	// __AABB_GOAL_RENDERER_H__
