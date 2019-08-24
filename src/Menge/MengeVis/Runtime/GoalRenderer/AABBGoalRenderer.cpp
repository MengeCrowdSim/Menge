#include "MengeVis/Runtime/GoalRenderer/AABBGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalAABB.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"
#include "MengeVis/Runtime/VisElementException.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::BFSM::AABBGoal;
using Menge::Math::AABBShape;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//          Implementation of AABBGoalRenderer
/////////////////////////////////////////////////////////////////////

std::string AABBGoalRenderer::getElementName() const { return AABBGoal::NAME; }

/////////////////////////////////////////////////////////////////////

void AABBGoalRenderer::doDrawGeometry() const {
  const AABBGoal* goal = dynamic_cast<const AABBGoal*>(_goal);
  if (goal == 0x0) {
    throw VisElementException("Attempting to draw goal of type " + _goal->getStringId() +
                              " with AABB goal renderer.");
  }
  const AABBShape& shape = *static_cast<const AABBShape*>(goal->getGeometry());
  drawGeometry(shape, RigidTransform2());
}
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
