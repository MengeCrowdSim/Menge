/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation
for educational, research, and non-profit purposes, without fee, and without a
written agreement is hereby granted, provided that the above copyright notice,
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North
Carolina at Chapel Hill. The software program and documentation are supplied "as is,"
without any accompanying services from the University of North Carolina at Chapel
Hill or the authors. The University of North Carolina at Chapel Hill and the
authors do not warrant that the operation of the program will be uninterrupted
or error-free. The end-user understands that the program was developed for research
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Math/Vector2.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"
#include "MengeVis/SceneGraph/GLScene.h"
#include "MengeVis/SceneGraph/Select.h"
#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/SceneGraph/shapes.h"

#include <iomanip>
#include <sstream>

namespace MengeVis {

namespace Runtime {

using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using namespace Menge::Math;
using MengeVis::SceneGraph::GLCamera;
using MengeVis::SceneGraph::GLScene;

////////////////////////////////////////////////////////////////////////////
//      Implementation of BaseAgentContext
////////////////////////////////////////////////////////////////////////////

const float BaseAgentContext::H = 0.01f;

////////////////////////////////////////////////////////////////////////////

BaseAgentContext::BaseAgentContext()
    : SceneGraph::SelectContext(),
      _selected(0x0),
      _showNbrRadius(false),
      _showNbr(false),
      _showMaxSpd(false),
      _showVel(false),
      _showPrefVel(false),
      _showOrient(false) {}

////////////////////////////////////////////////////////////////////////////

BaseAgentContext::~BaseAgentContext() {}

////////////////////////////////////////////////////////////////////////////

SceneGraph::ContextResult BaseAgentContext::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result(false, false);
  SDL_Keymod mods = SDL_GetModState();
  bool hasCtrl = (mods & KMOD_CTRL) > 0;
  bool hasAlt = (mods & KMOD_ALT) > 0;
  bool hasShift = (mods & KMOD_SHIFT) > 0;
  bool noMods = !(hasCtrl || hasAlt || hasShift);

  if (e.type == SDL_KEYDOWN) {
    if (noMods) {
      if (e.key.keysym.sym == SDLK_r) {
        _showNbrRadius = !_showNbrRadius;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_n) {
        _showNbr = !_showNbr;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_m) {
        _showMaxSpd = !_showMaxSpd;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_v) {
        _showVel = !_showVel;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_p) {
        _showPrefVel = !_showPrefVel;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_o) {
        _showOrient = !_showOrient;
        result.set(true, true);
      }
    }
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawGL(int vWidth, int vHeight) {
  getOpenGLView();
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  draw3DGL();
  glPopAttrib();
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::draw3DGL(bool select) {
  if (!select && _selected) {
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT |
                 GL_POLYGON_BIT);
    glLineWidth(2.f);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    const BaseAgent* agt = _selected->getAgent();
    drawNeighbors(agt);
    drawNbrRadius(agt);
    drawMaxSpeed(agt);
    drawVelocity(agt);
    drawPrefVelocity(agt);
    drawOrientation(agt);
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawNbrRadius(const BaseAgent* agt) {
  if (_showNbrRadius) {
    glPushAttrib(GL_POLYGON_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(agt->_pos.x(), agt->_pos.y(), H);
    float r = agt->_neighborDist;
    SceneGraph::Circle::drawCircle(r, 1.f, 0.75f, 0.85f, 0.05f);
    SceneGraph::Circle::drawCircle(r, 1.f, 0.75f, 0.85f, 1.f, GL_LINE);

    glPopMatrix();
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawNeighbors(const BaseAgent* agt) {
  if (_showNbr) {
    glColor4f(1.f, 1.f, 1.f, 1.f);

    const size_t NBR_COUNT = agt->_nearAgents.size();
    for (size_t i = 0; i < NBR_COUNT; ++i) {
      std::stringstream ss;
      ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
      ss << sqrtf(agt->_nearAgents[i].distanceSquared);
      const BaseAgent* nbr = agt->_nearAgents[i].agent;
      const Vector2& p = nbr->_pos;
      writeAlignedText(ss.str(), p, SceneGraph::TextWriter::CENTERED, true);
    }
    // Label the nearby obstacles
    glColor4f(0.5f, 1.f, 0.5f, 1.f);
    const size_t OBST_COUNT = agt->_nearObstacles.size();
    for (size_t i = 0; i < OBST_COUNT; ++i) {
      std::stringstream ss;
      const Obstacle* obst = agt->_nearObstacles[i].obstacle;
      ss << i;
      writeAlignedText(ss.str(), obst->midPt(), SceneGraph::TextWriter::CENTERED, true);
    }
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawMaxSpeed(const BaseAgent* agt) {
  if (_showMaxSpd) {
    glPushAttrib(GL_POLYGON_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(agt->_pos.x(), agt->_pos.y(), H);
    float r = agt->_maxSpeed;
    SceneGraph::Circle::drawCircle(r, 0.75f, 1.f, 0.85f, 0.05f);
    SceneGraph::Circle::drawCircle(r, 0.75f, 1.f, 0.85f, 1.f, GL_LINE);

    glPopMatrix();
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawVelocity(const BaseAgent* agt) {
  if (_showVel) {
    Vector2 vel = agt->_pos + agt->_vel;
    glDisable(GL_BLEND);
    glColor3f(0.9f, 0.45f, 0.1f);
    glBegin(GL_LINES);
    glVertex3f(agt->_pos.x(), agt->_pos.y(), H);
    glVertex3f(vel.x(), vel.y(), H);
    glEnd();

    writeTextRadially("v", vel, agt->_vel, true);
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawPrefVelocity(const BaseAgent* agt) {
  if (_showPrefVel) {
    Vector2 vel = agt->_pos + agt->_velPref.getPreferredVel();
    glDisable(GL_BLEND);
    glColor3f(0.75f, 0.f, 1.f);
    glBegin(GL_LINES);
    glVertex3f(agt->_pos.x(), agt->_pos.y(), H);
    glVertex3f(vel.x(), vel.y(), H);
    glEnd();

    writeTextRadially("v_p", vel, agt->_velPref.getPreferredVel(), true);
  }
}

////////////////////////////////////////////////////////////////////////////

void BaseAgentContext::drawOrientation(const BaseAgent* agt) {
  if (_showOrient) {
    Vector2 orient = agt->_pos + agt->_orient;
    glDisable(GL_BLEND);
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINES);
    glVertex3f(agt->_pos.x(), agt->_pos.y(), H);
    glVertex3f(orient.x(), orient.y(), H);
    glEnd();

    writeTextRadially("orient", orient, agt->_orient, true);
  }
}
////////////////////////////////////////////////////////////////////////////

std::string BaseAgentContext::agentText(const BaseAgent* agt) const {
  std::stringstream ss;
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(3);

  ss << "\nSelected: " << agt->_id;
  ss << "\nPosition: " << agt->_pos;
  ss << "\nNeighbo(r) radius: " << agt->_neighborDist;
  ss << "\n(N)eighbors: " << agt->_nearAgents.size();
  ss << "\n(M)ax Speed: " << agt->_maxSpeed;
  ss << "\n(O)rientation: " << agt->_orient;
  ss << "\n(V)elocity: " << agt->_vel << "(" << abs(agt->_vel) << ")";
  ss << "\n(P)ref. Velocity: " << agt->_velPref.getPreferredVel();
  ss << "(" << agt->_velPref.getSpeed() << ")";
  ss << "\nPriority: " << agt->_priority;
  ss << "\nClass: " << agt->_class;
  /*
          // Unused properties of a BaseAgent
          _radius
          _maxAccel
          _prefSpeed
          _maxNeighbors
          _maxAngVel
          */
  return ss.str();
}
}  // namespace Runtime
}  // namespace MengeVis
