#include "MengeVis/Runtime/GoalRenderer/OBBGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalOBB.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"
#include "MengeVis/Runtime/VisElementException.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::BFSM::OBBGoal;
using Menge::Math::OBBShape;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//					Implementation of OBBGoalRenderer
/////////////////////////////////////////////////////////////////////

std::string OBBGoalRenderer::getElementName() const { return OBBGoal::NAME; }

/////////////////////////////////////////////////////////////////////

void OBBGoalRenderer::doDrawGeometry() const {
  const OBBGoal* goal = dynamic_cast<const OBBGoal*>(_goal);
  if (goal == 0x0) {
    throw VisElementException("Attempting to draw goal of type " + _goal->getStringId() +
                              " with OBB goal renderer.");
  }
  const OBBShape& obb = *static_cast<const OBBShape*>(goal->getGeometry());
  drawGeometry(obb, RigidTransform2());
}

}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
