#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"

#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"
#include "MengeVis/SceneGraph/graphCommon.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//          Implementation of GoalRenderer
/////////////////////////////////////////////////////////////////////

void GoalRenderer::drawGL() const {
  // TODO: Write the type of the goal as well as displaying the assignment and
  //  capacity of the goal.
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glColor4f(0.75f, 0.1f, 0.75f, 0.25f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  doDrawGeometry();

  glDisable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  doDrawGeometry();

  glPopAttrib();
}

/////////////////////////////////////////////////////////////////////

void GoalRenderer::doDrawGeometry() const {
  drawPoint(_goal->getCentroid(), RigidTransform2());
}
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
