#include "MengeVis/Runtime/VCContext/NavMeshVCContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/VelocityComponents/VelCompNavMesh.h"
#include "MengeCore/resources/NavMeshEdge.h"
#include "MengeCore/resources/NavMeshObstacle.h"
#include "MengeCore/resources/PortalPath.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <iomanip>
#include <sstream>

namespace MengeVis {
namespace Runtime {
namespace VCContext {

using Menge::NavMeshEdge;
using Menge::NavMeshNode;
using Menge::NavMeshObstacle;
using Menge::NavMeshPtr;
using Menge::PortalPath;
using Menge::Agents::BaseAgent;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::Goal;
using Menge::BFSM::NavMeshVelComponent;
using Menge::Math::Vector2;
using Menge::Math::Vector3;

/////////////////////////////////////////////////////////////////////
//                   Implementation of NavMeshVCContext
/////////////////////////////////////////////////////////////////////

NavMeshVCContext::NavMeshVCContext()
    : VelCompContext(), _vc(0x0), _drawCenters(false), _drawNodeIDs(false) {}

/////////////////////////////////////////////////////////////////////

void NavMeshVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const NavMeshVelComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a navmesh velocity component context"
        " with an invalid value: either null or wrong "
        "velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string NavMeshVCContext::getElementName() const { return NavMeshVelComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string NavMeshVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Navigation mesh velocity component";
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
  const NavMeshPtr navMesh = _vc->getNavMesh();
  ss << "\n" << indent << "    Num. vertices: " << navMesh->getVertexCount();
  ss << "\n" << indent << "    Num. polygons: " << navMesh->getNodeCount();
  ss << "\n" << indent << "    Num. edges: " << navMesh->getEdgeCount();
  if (_drawCenters) {
    ss << "\n" << indent << "    Hide polygon (Ctrl-c)enters";
  } else {
    ss << "\n" << indent << "    Draw polygon (Ctrl-c)enters";
  }
  if (_drawNodeIDs) {
    ss << "\n" << indent << "    Hide polygon (Ctrl-i)ds";
  } else {
    ss << "\n" << indent << "    Draw polygon (Ctrl-i)ds";
  }
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

SceneGraph::ContextResult NavMeshVCContext::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result(false, false);

  SDL_Keymod mods = SDL_GetModState();
  bool hasCtrl = (mods & KMOD_CTRL) > 0;
  bool hasAlt = (mods & KMOD_ALT) > 0;
  bool hasShift = (mods & KMOD_SHIFT) > 0;
  bool noMods = !(hasCtrl || hasAlt || hasShift);

  if (e.type == SDL_KEYDOWN) {
    if (hasCtrl && !hasAlt && !hasShift) {
      if (e.key.keysym.sym == SDLK_c) {
        _drawCenters = !_drawCenters;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_i) {
        _drawNodeIDs = !_drawNodeIDs;
        result.set(true, true);
      }
    }
  }

  return result;
}

/////////////////////////////////////////////////////////////////////

void NavMeshVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  getOpenGLView();
  drawNavMesh();
  drawPath(agt, goal);

  // draw the preferred velocity
  const NavMeshPtr navMesh = _vc->getLocalizer()->getNavMesh();
  unsigned int NODE_ID = _vc->getLocalizer()->getNode(agt);
  float elevation = navMesh->getElevation(NODE_ID, agt->_pos);
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos, elevation);
}

////////////////////////////////////////////////////////////////////////////

void NavMeshVCContext::drawNavMesh() const {
  const NavMeshPtr navMesh = _vc->getNavMesh();
  unsigned int nCount = static_cast<unsigned int>(navMesh->getNodeCount());

  const Vector2* vertices = navMesh->getVertices();
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_POINT_BIT | GL_DEPTH_BUFFER_BIT);

  // shaded regions
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.5f, 0.8f, 1.f, 0.25f);
  for (unsigned int n = 0; n < nCount; ++n) {
    const NavMeshNode& node = navMesh->getNode(n);
    size_t vCount = node.getVertexCount();
    glBegin(GL_POLYGON);
    for (size_t v = 0; v < vCount; ++v) {
      unsigned int vID = node.getVertexID(v);
      const Vector2& p = vertices[vID];
      float elevation = node.getElevation(p);
      glVertex3f(p.x(), p.y(), elevation);
    }
    glEnd();
  }

  // outlines
  glDisable(GL_BLEND);
  // edges
  unsigned int eCount = static_cast<unsigned int>(navMesh->getEdgeCount());
  glBegin(GL_LINES);
  for (unsigned int e = 0; e < eCount; ++e) {
    const NavMeshEdge& edge = navMesh->getEdge(e);
    const NavMeshNode* node = edge.getFirstNode();
    Vector2 p0 = edge.getP0();
    float h = node->getElevation(p0);
    glVertex3f(p0.x(), p0.y(), h);
    Vector2 p1 = edge.getP1();
    h = node->getElevation(p1);
    glVertex3f(p1.x(), p1.y(), h);
  }
  glEnd();

  if (_drawCenters) {
    // centers
    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_POINT_BIT);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glPointSize(5.f);
    glBegin(GL_POINTS);
    for (unsigned int n = 0; n < nCount; ++n) {
      const NavMeshNode& node = navMesh->getNode(n);
      Vector2 p = node.getCenter();
      float elevation = node.getElevation(p);
      glVertex3f(p.x(), p.y(), elevation);
    }
    glEnd();
    glPopAttrib();
  }

  if (_drawNodeIDs) {
    drawNodeIDs();
  }

  // obstacles
  glColor3f(1.f, 0.1f, 0.1f);
  unsigned int oCount = static_cast<unsigned int>(navMesh->getObstacleCount());
  glBegin(GL_LINES);
  for (unsigned int o = 0; o < oCount; ++o) {
    const NavMeshObstacle& obst = navMesh->getObstacle(o);
    const NavMeshNode* node = obst.getNode();
    Vector2 p0 = obst.getP0();
    float h = node->getElevation(p0);
    glVertex3f(p0.x(), p0.y(), h);
    Vector2 p1 = obst.getP1();
    h = node->getElevation(p1);
    glVertex3f(p1.x(), p1.y(), h);
  }
  glEnd();

  glPopAttrib();
}

////////////////////////////////////////////////////////////////////////////

void NavMeshVCContext::drawPath(const BaseAgent* agt, const Goal* goal) const {
  const size_t ID = agt->_id;
  PortalPath* path = _vc->getLocalizer()->getPath(ID);
  if (path) {
    NavMeshPtr navMesh = _vc->getNavMesh();
    unsigned int NODE_ID = _vc->getLocalizer()->getNode(agt);
    Vector2 prev(agt->_pos);
    float height = navMesh->getElevation(NODE_ID, prev);
    glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);

    glColor3f(1.f, 1.f, 0.f);
    glLineWidth(3.f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(prev.x(), prev.y(), height);
    // iterate through the rest
    const size_t P_COUNT = path->getWayPointCount();
    size_t i = path->getCurrentPortal();
    if (i < P_COUNT) {
      NODE_ID = path->getNode(i);
      prev.set(path->getWayPoint(i));
      height = navMesh->getElevation(NODE_ID, prev);
      ++i;
    }
    Vector2 p;
    for (; i < P_COUNT; ++i) {
      NODE_ID = path->getNode(i);
      p.set(path->getWayPoint(i));
      float h = navMesh->getElevation(NODE_ID, p);
      if (absSq(p - prev) > 0.001f) {
        glVertex3f(prev.x(), prev.y(), height);
      }
      prev.set(p);
      height = h;
    }
    glVertex3f(prev.x(), prev.y(), height);

    const Goal* goal = path->getGoal();
    Vector2 goalPoint = goal->getTargetPoint(prev, agt->_radius);
    height = navMesh->getElevation(path->getEndNode(), goalPoint);
    glVertex3f(goalPoint.x(), goalPoint.y(), height);

    glEnd();
    glPopAttrib();

    // draw goal
    Vector3 goal3D(goalPoint.x(), goalPoint.y(), height);
    drawGoal(goal3D, agt->_radius);
  }
}

////////////////////////////////////////////////////////////////////////////

void NavMeshVCContext::drawNodeIDs() const {
  const NavMeshPtr navMesh = _vc->getNavMesh();
  unsigned int nCount = static_cast<unsigned int>(navMesh->getNodeCount());

  glColor4f(0.5f, 0.8f, 1.f, 1.f);
  for (unsigned int n = 0; n < nCount; ++n) {
    std::stringstream ss;
    ss << n;
    const NavMeshNode& node = navMesh->getNode(n);
    Vector3 p = node.getCenter3D();
    writeText(ss.str(), p, true /*currColor*/);
  }
}
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis
