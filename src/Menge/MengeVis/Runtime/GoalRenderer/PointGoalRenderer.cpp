#include "MengeVis/Runtime/GoalRenderer/PointGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalPoint.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::BFSM::PointGoal;

/////////////////////////////////////////////////////////////////////
//          Implementation of PointGoalRenderer
/////////////////////////////////////////////////////////////////////

std::string PointGoalRenderer::getElementName() const { return PointGoal::NAME; }

}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis