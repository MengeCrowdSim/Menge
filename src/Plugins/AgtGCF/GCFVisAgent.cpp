#include "GCFVisAgent.h"
#include "GCFAgent.h"
#include "GCFSimulator.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeVis/SceneGraph/shapes.h"

namespace GCF {

using Menge::Agents::BaseAgent;
using MengeVis::Runtime::VisAgent;

////////////////////////////////////////////////////////////////
//			Implementation of GCFVisAgent
////////////////////////////////////////////////////////////////

VisAgent* GCFVisAgent::moveToClone() {
  VisAgent* agt = new GCFVisAgent();
  agt->setElement(_agent);
  _agent = 0x0;
  return agt;
}

////////////////////////////////////////////////////////////////

void GCFVisAgent::drawAgent(float r, float g, float b) const {
  const Agent* agt = dynamic_cast<const Agent*>(_agent);
  if (agt != nullptr) {
    if (Simulator::SPEED_COLOR) {
      float speed = abs(_agent->_vel);
      float frac = speed / _agent->_prefSpeed;
      frac = frac > 1.f ? 1.f : frac;
      r = 1.f - frac;
      g = frac;
      b = 0.f;
    }

    const Vector2& pos = agt->_ellipse.getCenter();
    glPushMatrix();
    const Vector2& orient = agt->_ellipse.getOrientation();
    float angle = atan2(orient.y(), orient.x()) * 180.f / 3.141597f;
    glRotatef(angle, 0.f, 0.f, 1.f);
    float major = agt->_ellipse.getMajor();
    float minor = agt->_ellipse.getMinor();

    glScalef(major, minor, 1.f);
    MengeVis::SceneGraph::Cylinder::drawCylinder(1.f, 1.72f, r, g, b, 1.f);

    glPopMatrix();
  }
}

////////////////////////////////////////////////////////////////

bool GCFVisAgent::doValidateAgent(const BaseAgent* agent) {
  const Agent* agt = dynamic_cast<const Agent*>(agent);
  return agt != 0x0;
}

}  // namespace GCF
