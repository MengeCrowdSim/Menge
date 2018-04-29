/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file    ORCATypeAgentContext.h
 @brief   A basic context for interacting with and displaying ORCA-type agent parameters.

 ORCA-type agents are those that solve w.r.t. a set of linear constraints this context gives some
 basic functionality for displaying those constraints.
 */

#ifndef __ORCA_TYPE_AGENT_CONTEXT_H__
#define __ORCA_TYPE_AGENT_CONTEXT_H__

#include <iomanip>
#include <sstream>
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"
#include "MengeVis/SceneGraph/shapes.h"

namespace MengeVis {
namespace Runtime {
/*!
 @brief   Context class for displaying various aspects of the ORCA-type agent computation.
 */
template <class Agent>
class ORCATypeAgentContext : public BaseAgentContext {
 public:
  /*!
   @brief   Default constructor.
   */
  ORCATypeAgentContext();

  /*!
   @brief    Sets the agent for this context.

   This method works in conjunction with the VisElementDatabase. When this visualization element is
   triggered, the database will supply the triggering element.

   @param   agent   The agent to interact with.
   */
  virtual void setElement(VisAgent* agent);

  /*!
   @brief   Returns the name of the context for display.

   @returns   The name of this context.
   */
  virtual std::string contextName() const { return "UNDEFINED ORCA TYPE"; }

  /*!
   @brief   The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const { return "undefined_orca"; }

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual SceneGraph::ContextResult handleKeyboard(SDL_Event& e);

  /*!
   @brief Allow the context to update any time-dependent state it might have to the given global
          time.
   */
  virtual void update();

 protected:
  /*!
   @brief    Draw context elements into the 3D world.

   @param   select    Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  virtual void draw3DGL(bool select = false);

  /*!
   @brief   Helper function for drawing a halfplane

   @param   line    The line object which defines the half plane.
   @param   pos     The relative position to draw the plane. Typically the position of the agent on
                    which the half-plane is applied.
   @param   r       The red component of the half plane color.
   @param   g       The green component of the half plane color.
   @param   b       The blue component of the half plane color.
   @param   h       Value, on the verical-axis (in world coordinates), of the plane on which to draw
                    the half plane.
   */
  void drawHalfPlane(const Menge::Math::Line& line, const Menge::Math::Vector2& pos, float r,
                     float g, float b, float h) const;

  /*!
   @brief   Draws the given ORCA line for the given agent

   @param   agent     A pointer to the agent to whom this line applies.
   @param   line      The actual line.
   @param   isAgent   A boolean reporting if the orca line comes from an agent. true --> agent,
                      false --> obstacle.
   */
  void drawORCALine(const Agent* agent, const Menge::Math::Line& line, bool isAgent) const;

  /*!
   @brief   Draw the optimized velocity for the current set of orca lines

   @param   agent   A pointer to the agent for which the velocity is drawn computeNewVelocity() will
                    be called on the agent.
   */
  void drawOptVelocity(Agent* agent) const;

  /*!
   @brief   Creates a formatted string to be printed in the context for a particular agent.

   @param   agent   A pointer to the agent for which the information is displayed.
   @returns A formatted string for display in the context's 2D gui.
   */
  virtual std::string agentText(const Menge::Agents::BaseAgent* agent) const;

  /*!
   @brief   Determines if the ORCA lines are drawn.
   */
  bool _showOrcaLines;

  /*!
   @brief   Function for drawing the ORCA lines acting on `agt`.

   @param   agt     A pointer to the agent whose ORCA lines will be drawn.
   */
  void drawORCALines(const Agent* agt) const;

  /*!
   @brief   Determines if the ORCA line construction is visualized.
   */
  bool _visualizeORCA;

  /*!
   @brief   The agent to visualize.
   */
  size_t _visNbrID;

  /*!
   @brief   The function that draws the visualization of the orca construction.

   @param   agt   A pointer to the agent for whom the *computation* of a single ORCA line is
                  illustrated.
   */
  void visORCA(const Agent* agt) const;
};

template <class Agent>
ORCATypeAgentContext<Agent>::ORCATypeAgentContext()
    : _showOrcaLines(false), _visualizeORCA(false), _visNbrID(0) {}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::setElement(VisAgent* agent) {
  // TODO: Make this NVI
  BaseAgentContext::setElement(agent);
  _visNbrID = 0;
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::update() {
  if (this->_selected && _visNbrID) {
    const Agent* agt = dynamic_cast<const Agent*>(this->_selected->getAgent());
    if (_visNbrID > 0) {
      size_t NBR_COUNT = agt->_nearAgents.size();
      if (_visNbrID > NBR_COUNT) {
        _visNbrID = NBR_COUNT;
      }
    }
  }
}

////////////////////////////////////////////////////////////////

template <class Agent>
SceneGraph::ContextResult ORCATypeAgentContext<Agent>::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result = BaseAgentContext::handleKeyboard(e);
  if (!result.isHandled()) {
    SDL_Keymod mods = SDL_GetModState();
    bool hasCtrl = (mods & KMOD_CTRL) > 0;
    bool hasAlt = (mods & KMOD_ALT) > 0;
    bool hasShift = (mods & KMOD_SHIFT) > 0;
    bool noMods = !(hasCtrl || hasAlt || hasShift);
    if (e.type == SDL_KEYDOWN) {
      if (noMods) {
        if (e.key.keysym.sym == SDLK_c) {
          _showOrcaLines = !_showOrcaLines;
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_z) {
          _visualizeORCA = !_visualizeORCA;
          _visNbrID = 0;
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_UP) {
          if (_visualizeORCA && this->_selected) {
            const Agent* agt = dynamic_cast<const Agent*>(this->_selected->getAgent());
            ++_visNbrID;
            if (_visNbrID >= agt->_nearAgents.size()) _visNbrID = 0;
            result.set(true, true);
          }
        } else if (e.key.keysym.sym == SDLK_DOWN) {
          if (_visualizeORCA && this->_selected) {
            const Agent* agt = dynamic_cast<const Agent*>(this->_selected->getAgent());
            if (_visNbrID == 0)
              _visNbrID = agt->_nearAgents.size() - 1;
            else
              --_visNbrID;
            result.set(true, true);
          }
        }
      }
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::draw3DGL(bool select) {
  BaseAgentContext::draw3DGL(select);
  if (!select && this->_selected) {
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT |
                 GL_POLYGON_BIT);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    const Agent* agt = dynamic_cast<const Agent*>(this->_selected->getAgent());
    drawORCALines(agt);
    visORCA(agt);
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////

template <class Agent>
std::string ORCATypeAgentContext<Agent>::agentText(const Menge::Agents::BaseAgent* agt) const {
  const Agent* agent = dynamic_cast<const Agent*>(agt);
  std::string m = BaseAgentContext::agentText(agent);
  std::stringstream ss;
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(2);

  ss << "\n_________________________";
  ss << "\nDraw OR(C)A lines";
  if (_showOrcaLines) {
    const size_t LINE_COUNT = agent->_orcaLines.size();
    const size_t AGT_COUNT = agent->_nearAgents.size();
    const size_t OBST_COUNT = LINE_COUNT - AGT_COUNT;
    ss << "\n    " << OBST_COUNT << " obstacle lines";
    ss << "\n    " << AGT_COUNT << " agent lines";
  }
  ss << "\nVisuali(z)e ORCA";
  if (_visualizeORCA) {
    if (agent->_nearAgents.size() == 0) {
      ss << "\n    No nearby agents.";
    } else {
      size_t id = (agent->_nearAgents[_visNbrID].agent)->_id;
      ss << "\n    Showing agent: " << id << " (up/down arrow to change)";
    }
  }
  return m + ss.str();
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::drawHalfPlane(const Menge::Math::Line& line,
                                                const Menge::Math::Vector2& pos, float r, float g,
                                                float b, float h) const {
  const float DIST = 35.f;
  Menge::Math::Vector2 norm(-line._direction.y(), line._direction.x());
  Menge::Math::Vector2 p0 = line._point + line._direction * DIST + pos;
  Menge::Math::Vector2 p1 = p0 - norm * DIST;
  Menge::Math::Vector2 p2 = p1 - line._direction * (2 * DIST);
  Menge::Math::Vector2 p3 = p2 + norm * DIST;

  glColor4f(r, g, b, 0.1f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_BLEND);

  glBegin(GL_QUADS);
  glVertex3f(p0.x(), p0.y(), h);
  glVertex3f(p1.x(), p1.y(), h);
  glVertex3f(p2.x(), p2.y(), h);
  glVertex3f(p3.x(), p3.y(), h);
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINES);
  glVertex3f(p0.x(), p0.y(), h);
  glVertex3f(p3.x(), p3.y(), h);
  glEnd();
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::drawORCALines(const Agent* agent) const {
  if (_showOrcaLines && this->_selected) {
    Agent* agt = const_cast<Agent*>(agent);
    agt->computeORCALines();
    const size_t LINE_COUNT = agt->_orcaLines.size();
    const size_t FIRST_AGENT = LINE_COUNT - agt->_nearAgents.size();
    const float DIST = 35.f;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    // Obstacle color
    glColor4f(0.75f, 0.75f, 0.75f, 0.1f);
    glBegin(GL_QUADS);
    for (size_t i = 0; i < LINE_COUNT; ++i) {
      // Agent color
      if (i == FIRST_AGENT) {
        glColor4f(1.f, 0.f, 0.f, 0.1f);
      }
      const Menge::Math::Line& line = agt->_orcaLines[i];
      // Find the nearest point on the orca line to the agent -- use that as the center
      Menge::Math::Vector2 norm(-line._direction.y(), line._direction.x());
      float t = line._direction * (-line._point);
      Menge::Math::Vector2 nearPt = line._point + t * line._direction;
      Menge::Math::Vector2 p0 = nearPt + line._direction * DIST + agt->_pos;
      Menge::Math::Vector2 p1 = p0 - norm * DIST;
      Menge::Math::Vector2 p2 = p1 - line._direction * (2 * DIST);
      Menge::Math::Vector2 p3 = p2 + norm * DIST;

      glVertex3f(p0.x(), p0.y(), this->H);
      glVertex3f(p1.x(), p1.y(), this->H);
      glVertex3f(p2.x(), p2.y(), this->H);
      glVertex3f(p3.x(), p3.y(), this->H);
    }
    glEnd();
    glDisable(GL_BLEND);

    glColor4f(0.75f, 0.75f, 0.75f, 0.1f);
    glBegin(GL_LINES);
    for (size_t i = 0; i < LINE_COUNT; ++i) {
      if (i == FIRST_AGENT) {
        glColor4f(1.f, 0.f, 0.f, 0.1f);
      }
      const Menge::Math::Line& line = agt->_orcaLines[i];
      float t = line._direction * (-line._point);
      Menge::Math::Vector2 nearPt = line._point + t * line._direction;
      Menge::Math::Vector2 p0 = nearPt + line._direction * DIST + agt->_pos;
      Menge::Math::Vector2 p1 = nearPt - line._direction * DIST + agt->_pos;
      glVertex3f(p0.x(), p0.y(), this->H);
      glVertex3f(p1.x(), p1.y(), this->H);
    }
    glEnd();
    // Label the orca lines from agents
    glColor4f(1.f, 0.f, 0.f, 1.f);
    for (size_t i = FIRST_AGENT; i < LINE_COUNT; ++i) {
      std::stringstream ss;
      const Menge::Agents::BaseAgent* nbr = agent->_nearAgents[i - FIRST_AGENT].agent;
      ss << nbr->_id;
      Menge::Math::Vector2 d = agent->_orcaLines[i].nearestPt(Menge::Math::Vector2(0.f, 0.f));
      Menge::Math::Vector2 p = d + agent->_pos;
      this->writeTextRadially(ss.str(), p, d, true);
      this->writeAlignedText(ss.str(), nbr->_pos, SceneGraph::TextWriter::CENTERED, true);
    }
  }
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::visORCA(const Agent* agt) const {
  if (_visualizeORCA && this->_selected) {
    if (agt->_nearAgents.size() > 0) {
      Menge::Math::Vector2 velPref = agt->_velPref.getPreferredVel();
      const float RAD_TO_DEG = 180.f * 3.1415927f;
      glColor3f(0.1f, 1.f, 0.1f);
      Agent* agent = const_cast<Agent*>(agt);
      agent->computeORCALines();
      const Agent* nbr = static_cast<const Agent*>(agent->_nearAgents[_visNbrID].agent);
      float R = agent->_radius + nbr->_radius;
      Menge::Math::Vector2 disp = nbr->_pos - agent->_pos;
      float dist = abs(disp);
      Menge::Math::Vector2 dir = disp / dist;
      Menge::Math::Vector2 perp(-dir.y(), dir.x());

      // Compute the tangent portions of the minkowski sum
      float cosPhi = R / dist;
      float sinPhi = sqrtf(1 - cosPhi * cosPhi);
      float cx = cosPhi * -dir.x();
      float sx = sinPhi * -dir.x();
      float cy = cosPhi * -dir.y();
      float sy = sinPhi * -dir.y();

      Menge::Math::Vector2 r0 = disp + R * Menge::Math::Vector2(cx - sy, sx + cy);
      Menge::Math::Vector2 l0 = disp + R * Menge::Math::Vector2(cx + sy, -sx + cy);
      // modify the positions of r0 and l0 so that they project onto the center
      //
      float l = dist / (r0 * dir);
      r0 *= l;
      l0 *= l;
      r0 += agent->_pos;
      l0 += agent->_pos;

      // What's the closest circle?
      const float TAU = agent->_timeHorizon;
      float minVel = dist / TAU;
      float Rmin = R / TAU;
      Menge::Math::Vector2 center(agent->_pos + dir * minVel);
      // First, draw leading circle
      glPushMatrix();
      glTranslatef(center.x(), center.y(), this->H);
      SceneGraph::Circle::drawCircle(Rmin, 0.1f, 1.f, 0.1f, 0.75f, GL_LINE);
      glPopMatrix();

      Menge::Math::Vector2 r1 = center + Rmin * Menge::Math::Vector2(cx - sy, sx + cy);
      Menge::Math::Vector2 l1 = center + Rmin * Menge::Math::Vector2(cx + sy, -sx + cy);

      glBegin(GL_LINES);
      glVertex3f(r0.x(), r0.y(), this->H);
      glVertex3f(r1.x(), r1.y(), this->H);
      glVertex3f(l0.x(), l0.y(), this->H);
      glVertex3f(l1.x(), l1.y(), this->H);
      glEnd();

      // Use right of way to compute velocities
      float row = agent->_priority - nbr->_priority;
      Menge::Math::Vector2 agtVel = agent->_vel;
      Menge::Math::Vector2 nbrVel = nbr->_vel;
      Menge::Math::Vector2 nbrVelPref = nbr->_velPref.getPreferredVel();
      if (row > 0.f) {
        // agent's advantage
        row = row > 1.f ? 1.f : row;
        if (dir * velPref > dir * agent->_vel) {
          agtVel = velPref * row + (1.f - row) * agent->_vel;
        }
      } else if (row < 0.f) {
        // nbr's advantage
        row = row < -1.f ? 1.f : -row;
        if (dir * nbrVelPref < dir * nbr->_vel) {
          nbrVel = nbrVelPref * row + (1.f - row) * nbr->_vel;
        }
      }

      // Other guy's velocity
      glColor3f(0.1f, 0.1f, 0.8f);
      glBegin(GL_LINES);
      glVertex3f(nbr->_pos.x(), nbr->_pos.y(), this->H);
      glVertex3f(nbr->_pos.x() + nbrVel.x(), nbr->_pos.y() + nbrVel.y(), this->H);
      glEnd();
      this->writeTextRadially("v_j", nbr->_pos + nbrVel, nbrVel, true);

      // My velocity
      glColor3f(0.1f, 0.8f, 0.1f);
      glBegin(GL_LINES);
      glVertex3f(agent->_pos.x(), agent->_pos.y(), this->H);
      glVertex3f(agent->_pos.x() + agtVel.x(), agent->_pos.y() + agtVel.y(), this->H);
      glEnd();
      this->writeTextRadially("v_i", agent->_pos + agtVel, agtVel, true);

      // Relative velocity
      glColor3f(0.1f, 0.8f, 0.8f);
      glBegin(GL_LINES);
      Menge::Math::Vector2 rel = agtVel - nbrVel;
      glVertex3f(agent->_pos.x(), agent->_pos.y(), this->H);
      glVertex3f(agent->_pos.x() + rel.x(), agent->_pos.y() + rel.y(), this->H);
      glEnd();
      this->writeTextRadially("v_ij", agent->_pos + rel, rel, true);

      // Draw the ORCA line
      // Determine which line it is
      size_t NBR_COUNT = agent->_nearAgents.size();
      size_t FIRST_NBR = agent->_orcaLines.size() - NBR_COUNT;
      drawORCALine(agent, agent->_orcaLines[FIRST_NBR + _visNbrID], true);

      // optimized velocity in transformed space
      drawOptVelocity(agent);
    }
  }
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::drawORCALine(const Agent* agent, const Menge::Math::Line& line,
                                               bool isAgent) const {
  if (isAgent) {
    drawHalfPlane(line, agent->_pos, 1.f, 0.f, 0.f, this->H);
  } else {
    drawHalfPlane(line, agent->_pos, 0.75f, 0.75f, 0.75f, this->H);
  }
}

////////////////////////////////////////////////////////////////

template <class Agent>
void ORCATypeAgentContext<Agent>::drawOptVelocity(Agent* agent) const {
  // Draw the optimized velocity (transformed and untransformed
  agent->computeNewVelocity();
  // NORMAL space
  glPushAttrib(GL_POINT_BIT);
  glPointSize(3.f);
  glColor3f(0.2f, 0.2f, 1.f);
  glBegin(GL_POINTS);
  glVertex3f(agent->_pos.x() + agent->_velNew.x(), agent->_pos.y() + agent->_velNew.y(), this->H);
  glEnd();
  this->writeTextRadially("  v_new  ", agent->_pos + agent->_velNew, agent->_velNew, true);
}
}  // namespace Runtime
}  // namespace MengeVis
#endif  // __ORCA_TYPE_AGENT_CONTEXT_H__
