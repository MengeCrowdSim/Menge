/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2019 Sean Curtis

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

#pragma once

#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {
/*!
 @brief   Class responsible for rendering path goals to the OpenGL context.
 */
class MENGEVIS_API PathGoalRenderer final : public GoalRenderer {
 public:
  // Inherits docs from GoalRenderer::getElementName(). 
  std::string getElementName() const final;

 protected:
  // Inherits docs from GoalRnederer::doDrawGeometry().
  void doDrawGeometry() const final;
};
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
