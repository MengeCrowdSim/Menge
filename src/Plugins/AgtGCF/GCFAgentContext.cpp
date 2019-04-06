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

#include "GCFAgentContext.h"
#include <iomanip>
#include <sstream>
#include "GCFAgent.h"
#include "GCFSimulator.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"

namespace GCF {

using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Math::Vector2;
using MengeVis::Runtime::BaseAgentContext;
using MengeVis::SceneGraph::ContextResult;
using MengeVis::SceneGraph::TextWriter;

const int SAMPLE_COUNT = 50;

////////////////////////////////////////////////////////////////
//			Implementation of GCFAgentContext
////////////////////////////////////////////////////////////////

AgentContext::AgentContext()
    : BaseAgentContext(),
      _showForce(false),
      _onlyForceDir(true),
      _forceObject(0),
      _responses(0x0) {}

////////////////////////////////////////////////////////////////

AgentContext::~AgentContext() {
  if (_responses != 0x0) delete[] _responses;
}

////////////////////////////////////////////////////////////////

void AgentContext::setElement(const MengeVis::Runtime::VisAgent* agent) {
  BaseAgentContext::setElement(agent);
  _forceObject = 0;
  const Agent* agt = dynamic_cast<const Agent*>(agent);
  if (_responses != nullptr) {
    delete[] _responses;
    _responses = 0x0;
  }
  initResponses(agt);
}

////////////////////////////////////////////////////////////////

ContextResult AgentContext::handleKeyboard(SDL_Event& e) {
  ContextResult result = BaseAgentContext::handleKeyboard(e);

  if (!result.isHandled()) {
    SDL_Keymod mods = SDL_GetModState();
    bool hasCtrl = (mods & KMOD_CTRL) > 0;
    bool hasAlt = (mods & KMOD_ALT) > 0;
    bool hasShift = (mods & KMOD_SHIFT) > 0;
    bool noMods = !(hasCtrl || hasAlt || hasShift);

    if (e.type == SDL_KEYDOWN) {
      if (noMods) {
        if (e.key.keysym.sym == SDLK_f) {
          _showForce = !_showForce;
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_l) {
          _onlyForceDir = !_onlyForceDir;
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_UP) {
          if (_showForce && _selected) {
            const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
            assert(agt != 0x0 && "GCF context trying to work with a non-GCF agent");
            if (cycleSingleForce(agt, true)) result.set(true, true);
          }
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          if (_showForce && _selected) {
            const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
            assert(agt != 0x0 && "GCF context trying to work with a non-GCF agent");
            if (cycleSingleForce(agt, false)) result.set(true, true);
          }
        }
      }
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////

void AgentContext::update() {
  BaseAgentContext::update();
  if (_selected && _forceObject) {
    const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
    assert(agt != 0x0 && "GCF context trying to work with a non GCF agent");
    if (_forceObject > 0) {
      int NBR_COUNT = (int)agt->_nearAgents.size();
      if (_forceObject > NBR_COUNT) {
        _forceObject = NBR_COUNT;
      }
    } else {  // _forceObject < 0
      int OBST_COUNT = (int)agt->_nearObstacles.size();
      if (-_forceObject > OBST_COUNT) {
        _forceObject = -OBST_COUNT;
      }
    }
  }
}

////////////////////////////////////////////////////////////////

void AgentContext::drawUIGL(int vWidth, int vHeight, bool select) {
  BaseAgentContext::drawUIGL(vWidth, vHeight, select);
  if (!select && _selected) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, vWidth, 0.0, vHeight, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    const float MAX_Y = _responses[1];
    glTranslatef(vWidth * 0.75f + 10, 0.f + 10, 0.f);
    glScalef(vWidth / (Simulator::MAX_AGENT_DIST * 4), vHeight / (MAX_Y * 4), 1.f);
    glColor3f(1.f, 1.f, 1.f);

    // horz/vert axes
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.f, 0.f, MAX_Y);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(Simulator::MAX_AGENT_DIST, 0.f, 0.f);
    glEnd();

    // Tick marks
    glBegin(GL_LINES);
    glVertex3f(Simulator::MAX_AGENT_DIST, 0.f, MAX_Y);
    glVertex3f(Simulator::MAX_AGENT_DIST, 0.f, 0.f);
    glVertex3f(Simulator::MAX_AGENT_DIST - Simulator::AGENT_INTERP_WIDTH, 0.f, MAX_Y);
    glVertex3f(Simulator::MAX_AGENT_DIST - Simulator::AGENT_INTERP_WIDTH, 0.f, 0.f);
    glVertex3f(Simulator::AGENT_INTERP_WIDTH, 0.f, MAX_Y);
    glVertex3f(Simulator::AGENT_INTERP_WIDTH, 0.f, 0.f);
    glEnd();

    glColor3f(0.75f, 0.8f, 0.75f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < SAMPLE_COUNT * 2; i += 2) {
      glVertex3f(_responses[i], _responses[i + 1], 0.f);
    }
    glEnd();

    glPopAttrib();
    glPopMatrix();
    //		draw a character
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
}

////////////////////////////////////////////////////////////////

void AgentContext::draw3DGL(bool select) {
  BaseAgentContext::draw3DGL(select);
  if (!select && _selected) {
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT |
                 GL_POLYGON_BIT);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
    assert(agt != 0x0 && "GCF context trying to work with a non GCF agent");
    initResponses(agt);
    drawForces(agt);
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////

std::string AgentContext::agentText(const Agents::BaseAgent* agent) const {
  const Agent* agt = dynamic_cast<const Agent*>(agent);
  std::string m = BaseAgentContext::agentText(agt);

  std::stringstream ss;
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
  ss << "\n_________________________";
  ss << "\nNear agents: " << agent->_nearAgents.size();
  ss << "\nNear obstacles: " << agent->_nearObstacles.size();
  ss << "\nDraw (F)orces";
  if (_onlyForceDir) {
    ss << "\n  Scaled (L)ength";
  } else {
    ss << "\n  Unit (L)ength";
  }
  if (_showForce && _selected) {
    ss << "\n    (up/down arrow to change)";
    if (_forceObject == 0) {
      ss << "\n     All forces";
    } else if (_forceObject > 0) {
      const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
      assert(agt != 0x0 && "GCF context trying to work with a non GCF agent");
      const Agent* other = static_cast<const Agent*>(agt->getNeighbor(_forceObject - 1));
      float force = -1;  // abs( agt->agentForce( other ) );
      ss << "\n     Agent " << other->_id << ": " << force << " N";
    } else if (_forceObject < 0) {
      const Agent* agt = dynamic_cast<const Agent*>(_selected->getAgent());
      assert(agt != 0x0 && "GCF context trying to work with a non GCF agent");
      const Agents::Obstacle* obst = agt->getObstacle(-_forceObject - 1);
      float force = -1;  // abs( agt->obstacleForce( obst ) );
      ss << "\n     Obstacle " << obst->_id << ": " << force << " N";
    }
  }

  return m + ss.str();
}

////////////////////////////////////////////////////////////////

bool AgentContext::cycleSingleForce(const Agent* agt, bool forward) {
  int NBRS = (int)agt->_nearAgents.size();
  int OBST = (int)agt->_nearObstacles.size();
  if (NBRS | OBST) {
    _forceObject += forward ? 1 : -1;
    if (_forceObject < -OBST) {
      _forceObject = NBRS;
    } else if (_forceObject > NBRS) {
      _forceObject = -OBST;
    }
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////

void AgentContext::drawForces(const Agent* agt) {
  if (_showForce && _selected) {
    glPushMatrix();
    // Draw driving force
    glColor4f(0.1f, 1.f, 0.1f, 1.f);
    Vector2 driveForce(agt->driveForce());
    float mag = abs(driveForce);
    drawForce(agt, driveForce / mag, mag, "D");

    // Draw repulsive forces
    if (_forceObject == 0) {
      // draw forces for all agents
      const int NBRS = (int)agt->_nearAgents.size();
      for (int i = 0; i < NBRS; ++i) {
        const Agent* other = static_cast<const Agent*>(agt->getNeighbor(i));
        singleAgentForce(agt, other);
      }
      // draw forces for all obstacles
      const int OBSTS = (int)agt->_nearObstacles.size();
      for (int i = 0; i < OBSTS; ++i) {
        const Agents::Obstacle* obst = agt->getObstacle(i);
        singleObstacleForce(agt, obst, 0.f);
      }
    } else if (_forceObject > 0) {
      // single agent
      const Agent* other = static_cast<const Agent*>(agt->getNeighbor(_forceObject - 1));
      singleAgentForce(agt, other, 0.f);
    } else {
      // draw obstacle
      const Agents::Obstacle* obst = agt->getObstacle(-_forceObject - 1);
      singleObstacleForce(agt, obst, 0.f);
    }

    glPopMatrix();
  }
}

////////////////////////////////////////////////////////////////

void AgentContext::singleAgentForce(const Agent* agt, const Agent* other, float thresh) {
  // Vector2 force = agt->agentForce( other );
  Vector2 force(2.f, 0.f);
  float effDist, K_ij, response, velScale, magnitude;
  agt->getRepulsionParameters(other, effDist, force, K_ij, response, velScale, magnitude);
  if (magnitude > thresh) {
    std::stringstream ss;
    ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    ss << other->_id;
    glColor4f(0.65f, 0.65f, 1.f, 1.f);
    drawForce(agt, force, magnitude, ss.str());
    // Label the source agent
    writeAlignedText(ss.str(), other->_pos, TextWriter::CENTERED, true);
  }
}

////////////////////////////////////////////////////////////////

void AgentContext::singleObstacleForce(const Agent* agt, const Agents::Obstacle* obst,
                                       float thresh) {
  Vector2 force = agt->obstacleForce(obst);
  float forceMag = abs(force);
  if (forceMag > thresh) {
    // Draw the force line
    std::stringstream ss;
    ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    ss << obst->_id;
    glColor4f(1.f, 0.65f, 0.65f, 1.f);
    drawForce(agt, force, forceMag, ss.str());

    // Highlight the obstacle
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(3.f);
    glBegin(GL_LINES);
    glVertex3f(obst->getP0().x(), obst->getP0().y(), H);
    glVertex3f(obst->getP1().x(), obst->getP1().y(), H);
    glEnd();
    glPopAttrib();

    // Label the source agent
    Vector2 midPoint = obst->midPt();
    writeText(ss.str(), midPoint, true);
  }
}

////////////////////////////////////////////////////////////////

void AgentContext::drawForce(const Agent* agt, const Vector2& forceDir, float forceMag,
                             const std::string& label) {
  // This is for printing force magnitude and source
  const float FORCE_RADIUS = 4 * agt->_radius;
  Vector2 forceEnd = agt->_pos + forceDir * (_onlyForceDir ? FORCE_RADIUS : forceMag);
  glBegin(GL_LINES);
  glVertex3f(agt->_pos.x(), agt->_pos.y(), H);
  glVertex3f(forceEnd.x(), forceEnd.y(), H);
  glEnd();
  // annotate illustration
  std::stringstream ss;
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);
  // Label the source
  if (label.size() > 0) {
    ss << label << ": ";
  }
  ss << forceMag << " N";
  writeTextRadially(ss.str(), forceEnd, forceDir, true);
}

////////////////////////////////////////////////////////////////

void AgentContext::initResponses(const Agent* agt) {
  if (_responses == 0x0) {
    float dd = Simulator::MAX_AGENT_DIST / SAMPLE_COUNT;
    _responses = new float[(SAMPLE_COUNT + 1) * 2];
    int idx = 0;
    for (int i = 0; i <= SAMPLE_COUNT; ++i, idx += 2) {
      float d = dd * i;
      float r = agt->computeDistanceResponse(d);
      _responses[idx] = d;
      _responses[idx + 1] = r;
    }
  }
}

}  // namespace GCF
