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
 @file    PedVOAgentContext.h
 @brief   A basic context for interacting with and displaying PedVO agent parameters.
 */

#ifndef __PEDVO_AGENT_CONTEXT_H__
#define __PEDVO_AGENT_CONTEXT_H__

#include "MengeCore/PedVO/PedVOAgent.h"
#include "MengeVis/Runtime/AgentContext/ORCATypeAgentContext.h"

namespace MengeVis {
namespace Runtime {
/*!
 @brief    Declaration of ORCA-type agent context for PedVO agents.
 */
typedef ORCATypeAgentContext<PedVO::Agent> PedVOAgentContext;

////////////////////////////////////////////////////////////////
//      Implementation of PedVOAgentContext
////////////////////////////////////////////////////////////////

// Specialization
template <>
std::string ORCATypeAgentContext<PedVO::Agent>::contextName() const {
  return "PedVO";
}

////////////////////////////////////////////////////////////////

template <>
std::string ORCATypeAgentContext<PedVO::Agent>::getElementName() const {
  return PedVO::Agent::NAME;
}

////////////////////////////////////////////////////////////////

/*!
 @brief   Draw the optimized velocity for the current set of orca lines

 @param   agent   A pointer to the agent for which the velocity is drawn. computeNewVelocity() will
                  be called on the agent.
 */
template <>
inline void ORCATypeAgentContext<PedVO::Agent>::drawOptVelocity(PedVO::Agent* agent) const {
  // Draw the optimized velocity (transformed and untransformed
  agent->computeNewVelocity();
  glPushAttrib(GL_POINT_BIT);
  glPointSize(3.f);
  // NORMAL space
  glColor3f(0.2f, 0.2f, 1.f);
  glBegin(GL_POINTS);
  glVertex3f(agent->_pos.x() + agent->_velNew.x(), agent->_pos.y() + agent->_velNew.y(), H);
  glEnd();
  writeTextRadially("  v_new  ", agent->_pos + agent->_velNew, agent->_velNew, true);

  Menge::Math::Vector2 prefDir(agent->_velPref.getPreferred());
  Menge::Math::Vector2 n(-prefDir.y(), prefDir.x());
  float vx = agent->_velNew * prefDir;
  float vy = agent->_velNew * n;
  vy /= agent->_turningBias;
  glColor3f(1.f, 0.1f, 1.f);
  glBegin(GL_POINTS);
  glVertex3f(agent->_pos.x() + vx, agent->_pos.y() + vy, H);
  glEnd();
  Menge::Math::Vector2 vn(vx, vy);
  writeTextRadially("  v_new^x", agent->_pos + vn, vn, true);
  glPopAttrib();
}

////////////////////////////////////////////////////////////////

/*!
 @brief   Draws the given ORCA line for the given agent.

 @param   agent     A pointer to the agent to whom this line applies.
 @param   line      The actual line.
 @param   isAgent   A boolean reporting if the orca line comes from an agent.  true --> agent,
                    false --> obstacle.
*/
template <>
void ORCATypeAgentContext<PedVO::Agent>::drawORCALine(const PedVO::Agent* agent,
                                                      const Menge::Math::Line& line,
                                                      bool isAgent) const {
  if (agent->_turningBias != 1.f) {
    // Transform line into new line
    Menge::Math::Line lEuclid;  // the line transformed back into Euclidian space
    lEuclid._point = line._point;
    lEuclid._direction = line._direction;
    Menge::Math::Line lAffine;  // the line (with scale) rotated back into Euclidian
    float prefSpeed = agent->_velPref.getSpeed();
    // Transformation is dependent on prefSpeed being non-zero
    bool rotated = false;
    if (prefSpeed > Menge::EPS) {
      Menge::Math::Vector2 prefDir(agent->_velPref.getPreferred());
      Menge::Math::Vector2 n(prefDir.y(), prefDir.x());
      // rotate and scale all of the lines
      float turnInv = 1.f / agent->_turningBias;

      // scale
      Menge::Math::Vector2 p(line._point.x(), line._point.y() * agent->_turningBias);
      Menge::Math::Vector2 d(line._direction.x(), line._direction.y() * agent->_turningBias);
      // rotate
      float px = p * prefDir;
      float py = p * n;
      float dx = d * prefDir;
      float dy = d * n;
      // set
      lEuclid._point.set(px, py);
      lEuclid._direction.set(dx, dy);

      px = line._point * prefDir;
      py = line._point * n;
      dx = line._direction * prefDir;
      dy = line._direction * n;
      lAffine._point.set(px, py);
      lAffine._direction.set(dx, dy);
      rotated = true;
    }
    if (rotated) {
      if (isAgent) {
        drawHalfPlane(lAffine, agent->_pos, 1.f, 0.5f, 0.f, H);
        if (!_showOrcaLines) drawHalfPlane(lEuclid, agent->_pos, 1.f, 0.f, 0.f, H);
      } else {
        drawHalfPlane(lAffine, agent->_pos, 0.5f, 0.5f, 0.5f, H);
        if (!_showOrcaLines) drawHalfPlane(lEuclid, agent->_pos, 0.75f, 0.75f, 0.75f, H);
      }
    } else {
      if (isAgent) {
        drawHalfPlane(lEuclid, agent->_pos, 1.f, 0.f, 0.f, H);
      } else {
        drawHalfPlane(lEuclid, agent->_pos, 0.75f, 0.75f, 0.75f, H);
      }
    }

  } else {
    if (isAgent) {
      drawHalfPlane(line, agent->_pos, 1.f, 0.f, 0.f, H);
    } else {
      drawHalfPlane(line, agent->_pos, 0.75f, 0.75f, 0.75f, H);
    }
  }
}
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __PEDVO_AGENT_CONTEXT_H__
