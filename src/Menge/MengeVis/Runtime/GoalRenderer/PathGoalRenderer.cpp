#include "MengeVis/Runtime/GoalRenderer/PathGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalPath.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"
#include "MengeVis/Runtime/VisElementException.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::BFSM::PathGoal;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//          Implementation of PathGoalRenderer
/////////////////////////////////////////////////////////////////////

std::string PathGoalRenderer::getElementName() const { return PathGoal::NAME; }

/////////////////////////////////////////////////////////////////////

void PathGoalRenderer::doDrawGeometry() const {
  const PathGoal* goal = dynamic_cast<const PathGoal*>(_goal);
  if (goal == 0x0) {
    throw VisElementException("Attempting to draw goal of type '" + _goal->getStringId() +
                              "' with path goal renderer.");
  }

  const Vector2& p_WGo = goal->origin();
  drawGeometry(*goal->getGeometry(), RigidTransform2(p_WGo));
}

}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
