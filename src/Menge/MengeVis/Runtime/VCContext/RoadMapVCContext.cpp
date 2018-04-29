#include "MengeVis/Runtime/VCContext/RoadMapVCContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/VelocityComponents/VelCompRoadMap.h"
#include "MengeCore/resources/Graph.h"
#include "MengeCore/resources/GraphEdge.h"
#include "MengeCore/resources/GraphVertex.h"
#include "MengeCore/resources/NavMeshObstacle.h"
#include "MengeCore/resources/PortalPath.h"
#include "MengeCore/resources/RoadMapPath.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <iomanip>
#include <sstream>

namespace MengeVis {
namespace Runtime {
namespace VCContext {

using Menge::GraphEdge;
using Menge::GraphPtr;
using Menge::GraphVertex;
using Menge::PathMap;
using Menge::RoadMapPath;
using Menge::Agents::BaseAgent;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::Goal;
using Menge::BFSM::RoadMapVelComponent;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//                   Implementation of RoadMapVCContext
/////////////////////////////////////////////////////////////////////

RoadMapVCContext::RoadMapVCContext() : VelCompContext(), _vc(0x0) {}

/////////////////////////////////////////////////////////////////////

void RoadMapVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const RoadMapVelComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a road map velocity component "
        "context with an invalid value: either null or "
        "wrong velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string RoadMapVCContext::getElementName() const { return RoadMapVelComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string RoadMapVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Roadmap velocity component";
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

void RoadMapVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  // draw target
  glPushAttrib(GL_POINT_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);

  const GraphPtr graph = _vc->getRoadMap();

  // draw edges
  glColor3f(0.2f, 0.2f, 1.f);
  glLineWidth(1.5f);
  glBegin(GL_LINES);
  const size_t V_COUNT = graph->getVertexCount();
  for (size_t i = 0; i < V_COUNT; ++i) {
    const GraphVertex* v = graph->getVertex(i);
    const size_t vID = v->getID();
    const Vector2 vPos = v->getPosition();
    const size_t N_COUNT = v->getNeighborCount();
    for (size_t j = 0; j < N_COUNT; ++j) {
      const GraphVertex* u = v->getNeighbor(j);
      if (vID < u->getID()) {
        glVertex3f(vPos.x(), vPos.y(), 0.f);
        Vector2 uPos = u->getPosition();
        glVertex3f(uPos.x(), uPos.y(), 0.f);
      }
    }
  }
  glEnd();

  // draw nodes
  glColor3f(1.0f, 0.6f, 1.f);
  glPointSize(5.f);
  glBegin(GL_POINTS);
  for (size_t i = 0; i < V_COUNT; ++i) {
    const GraphVertex* v = graph->getVertex(i);
    const Vector2 vPos = v->getPosition();
    glVertex3f(vPos.x(), vPos.y(), 0.f);
  }
  glEnd();

  // draw path
  Vector2 tgtPoint;
  PathMap::const_iterator itr = _vc->getPathMap().find(agt->_id);
  if (itr != _vc->getPathMap().end()) {
    const RoadMapPath* path = itr->second;
    const size_t WP_COUNT = path->getWayPointCount();

    // draw the goal
    if (WP_COUNT > 0) {
      // nearest point to the goal from the last way point
      tgtPoint = goal->getTargetPoint(path->getWayPoint(WP_COUNT - 1), agt->_radius);
    } else {
      tgtPoint = goal->getTargetPoint(agt->_pos, agt->_radius);
    }

    const size_t TARGET = path->getTargetID();
    glColor3f(1.f, 1.f, 0.5f);
    glLineWidth(2.f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(agt->_pos.x(), agt->_pos.y(), 0.f);
    for (size_t i = TARGET; i < WP_COUNT; ++i) {
      Vector2 p = path->getWayPoint(i);
      glVertex3f(p.x(), p.y(), 0.f);
    }
    glVertex3f(tgtPoint.x(), tgtPoint.y(), 0.f);
    glEnd();
  } else {
    tgtPoint = goal->getTargetPoint(agt->_pos, agt->_radius);
  }

  // draw goal
  //  TODO: REALLY draw the goal
  drawGoal(tgtPoint, agt);

  // draw the preferred velocity
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos);

  glPopAttrib();
}
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis
