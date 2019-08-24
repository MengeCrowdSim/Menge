#include "MengeVis/Runtime/GoalRenderer/CircleGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalCircle.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"
#include "MengeVis/Runtime/VisElementException.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::BFSM::CircleGoal;
using Menge::Math::CircleShape;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//          Implementation of CircleGoalRenderer
/////////////////////////////////////////////////////////////////////

std::string CircleGoalRenderer::getElementName() const { return CircleGoal::NAME; }

/////////////////////////////////////////////////////////////////////

void CircleGoalRenderer::doDrawGeometry() const {
  const CircleGoal* goal = dynamic_cast<const CircleGoal*>(_goal);
  if (goal == 0x0) {
    throw VisElementException("Attempting to draw goal of type " + _goal->getStringId() +
                              " with circle goal renderer.");
  }
  const CircleShape& shape = *static_cast<const CircleShape*>(goal->getGeometry());
  drawGeometry(shape, RigidTransform2());
}
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis