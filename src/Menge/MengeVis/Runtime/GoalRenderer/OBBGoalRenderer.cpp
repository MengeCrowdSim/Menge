#include "MengeVis/Runtime/GoalRenderer/OBBGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalOBB.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/VisElementException.h"
#include "MengeVis/SceneGraph/graphCommon.h"

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
  const OBBShape* obb = static_cast<const OBBShape*>(goal->getGeometry());
  Vector2 size = obb->getSize();
  Vector2 pivot = obb->getPivot();

  // Compute the four corners in the world frame:
  //
  // c3 ___________ c2
  //   |           |
  //   |___________|
  //   O            c1
  //
  // O = <0, 0, 0>
  // c1 = <w, 0, 0>
  // c2 = <w, h, 0>
  // c3 = <0, h, 0>
  //
  Vector2 c(0.f, 0.f);
  Vector2 c0 = obb->convertToWorld(c);
  c.set(size.x(), 0.f);
  Vector2 c1 = obb->convertToWorld(c);
  c.set(size);
  Vector2 c2 = obb->convertToWorld(c);
  c.set(0.f, size.y());
  Vector2 c3 = obb->convertToWorld(c);

  glBegin(GL_POLYGON);
  glVertex3f(c0.x(), c0.y(), 0.f);
  glVertex3f(c1.x(), c1.y(), 0.f);
  glVertex3f(c2.x(), c2.y(), 0.f);
  glVertex3f(c3.x(), c3.y(), 0.f);
  glEnd();
}
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
