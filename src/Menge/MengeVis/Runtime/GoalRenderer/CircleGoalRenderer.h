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

#ifndef __CIRCLE_GOAL_RENDERER_H__
#define __CIRCLE_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {
/*!
 @brief   Class responsible for rendering circle goals to the OpenGL context.
 */
class MENGEVIS_API CircleGoalRenderer : public GoalRenderer {
 public:
  /*!
   @brief   The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const;

 protected:
  /*!
   @brief   The method for handling the details of a particular goal.

   This is called twice by drawGL().  If there is an expensive operation, it would be best to be
   cached because it will be called twice every time a goal is drawn.
   */
  virtual void doDrawGeometry() const;
};
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __CIRCLE_GOAL_RENDERER_H__
