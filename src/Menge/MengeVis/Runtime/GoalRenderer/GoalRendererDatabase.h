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

#ifndef __GOAL_RENDERER_DATABASE_H__
#define __GOAL_RENDERER_DATABASE_H__

#include "MengeVis/Runtime/VisElementDatabase.h"

// forward declaration
namespace Menge {
namespace BFSM {
class Goal;
}
}  // namespace Menge

namespace MengeVis {
namespace Runtime {
class VisAgent;
namespace GoalVis {
class GoalRenderer;
}
/*!
 @brief   The database of registered agent contexts.
 */
typedef VisElementDB<GoalVis::GoalRenderer, Menge::BFSM::Goal> GoalRendererDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Declrations of explicit specializations
template <>
GoalVis::GoalRenderer*
Runtime::VisElementDB<GoalVis::GoalRenderer, Menge::BFSM::Goal>::getDefaultElement();

template <>
void Runtime::VisElementDB<GoalVis::GoalRenderer, Menge::BFSM::Goal>::addBuiltins();

template <>
std::string Runtime::VisElementDB<GoalVis::GoalRenderer, Menge::BFSM::Goal>::getElementName();
#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Runtime
}  // namespace MengeVis

#endif  // __GOAL_RENDERER_DATABASE_H__
