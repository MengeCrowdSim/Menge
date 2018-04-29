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

#ifndef __POINT_GOAL_RENDERER_H__
#define __POINT_GOAL_RENDERER_H__

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {
/*!
 @brief    Class responsible for rendering point goals to the OpenGL context.
 */
class MENGEVIS_API PointGoalRenderer : public GoalRenderer {
 public:
  /*!
   @brief   The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const;
};
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __POINT_GOAL_RENDERER_H__
