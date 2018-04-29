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

#include "MengeCore/Agents/BaseAgent.h"

#include "MengeCore/Agents/Obstacle.h"

namespace Menge {

namespace Agents {

using Math::Vector2;

////////////////////////////////////////////////////////////////
//          Implementation of BaseAgent
////////////////////////////////////////////////////////////////

BaseAgent::BaseAgent() {
  _maxSpeed = 2.5f;
  _maxAccel = 2.f;
  _prefSpeed = 1.34f;
  _pos = Vector2(0.f, 0.f);
  _vel = Vector2(0.f, 0.f);
  _velPref = PrefVelocity(Vector2(1.f, 0.f), _prefSpeed, Vector2(0.f, 0.f));
  _velNew = Vector2(0.f, 0.f);
  _orient = Vector2(1.f, 0.f);
  _maxAngVel = TWOPI;  // 360 degrees/sec
  _maxNeighbors = 10;
  _neighborDist = 5.f;
  _nearAgents.clear();
  _nearObstacles.clear();
  _class = 0;
  _obstacleSet = 0xFFFFFFFF;
  _priority = 0.f;
  _id = 0;
  _radius = 0.19f;
}

////////////////////////////////////////////////////////////////

void BaseAgent::initialize() { _orient = _velPref.getPreferred(); }

////////////////////////////////////////////////////////////////

void BaseAgent::update(float timeStep) {
  float delV = abs(_vel - _velNew);
  // Check to see if new velocity violates acceleration constraints...
  // TODO: Make the acceleration constraint respect collisions (particularly with
  //    obstacles.  I.e. confirm that the new velocity STILL won't collide
  //    with the neighboring obstacles.
  //  The slick way to do this is to replace the float in the _nearObstacles to
  //    represent the minimum acceleration required to avoid collision with the
  //    obstacle in the next time step.  Then I can simply take the larger of the
  ///    user-define max acceleration and the smallest required acceleration
  if (delV > _maxAccel * timeStep) {
    float w = _maxAccel * timeStep / delV;
    _vel = (1.f - w) * _vel + w * _velNew;
  } else {
    _vel = _velNew;
  }
  _pos += _vel * timeStep;

  updateOrient(timeStep);
  postUpdate();
}

////////////////////////////////////////////////////////////////

void BaseAgent::updateOrient(float timeStep) {
  // This stabilizes orientation
  //  As the agent slows down, the target orientation becomes preferred direction.
  //  We define a threshold as a fraction of preferred speed.
  //  If the agents speed is at or above that threshold, orientation is defined by the
  //  movement vector.
  //  If the speed is zero, the orientation is the direction of preferred velocity.
  //  The transition function is designd such that the transition from movement direction
  //  to preferred movement direction falls off slowly (initially) and rapidly at low
  // speeds.
  float speed = abs(_vel);
  const float speedThresh = _prefSpeed / 3.f;
  Vector2 newOrient(_orient);  // by default new is old
  Vector2 moveDir = _vel / speed;
  if (speed >= speedThresh) {
    newOrient = moveDir;
  } else {
    float frac = sqrtf(speed / speedThresh);
    Vector2 prefDir = _velPref.getPreferred();
    // prefDir *can* be zero if we've arrived at goal.  Only use it if it's non-zero.
    if (absSq(prefDir) > 0.000001f) {
      newOrient = frac * moveDir + (1.f - frac) * prefDir;
      newOrient.normalize();
    }
  }

  // TODO(curds01): At low speeds, small movement perturbations cause radically different
  // orientation changes.  It seems *reasonable* to scale maximum angular velocity
  // by the travel speed (in some sense) to prevent this.  HOWEVER, this would break
  // agents that have a sense of facing direction that they actively control.

  // Now limit angular velocity.
  const float MAX_ANGLE_CHANGE = timeStep * _maxAngVel;
  float maxCt = cos(MAX_ANGLE_CHANGE);
  float ct = newOrient * _orient;
  if (ct < maxCt) {
    // changing direction at a rate greater than _maxAngVel
    float maxSt = sin(MAX_ANGLE_CHANGE);
    if (det(_orient, newOrient) > 0.f) {
      // rotate _orient left
      _orient.set(maxCt * _orient._x - maxSt * _orient._y, maxSt * _orient._x + maxCt * _orient._y);
    } else {
      // rotate _orient right
      _orient.set(maxCt * _orient._x + maxSt * _orient._y,
                  -maxSt * _orient._x + maxCt * _orient._y);
    }
  } else {
    _orient.set(newOrient);
  }
}

////////////////////////////////////////////////////////////////

void BaseAgent::setPreferredVelocity(PrefVelocity& velocity) {
  std::vector<BFSM::VelModifier*>::iterator vItr = _velModifiers.begin();
  for (; vItr != _velModifiers.end(); ++vItr) {
    (*vItr)->adaptPrefVelocity(this, velocity);
  }

  // set my velocity to be the given one
  _velPref = velocity;
}

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////

void BaseAgent::computeNewVelocity() { throw AgentImplementationException(); }

////////////////////////////////////////////////////////////////

void BaseAgent::insertAgentNeighbor(const BaseAgent* agent, float distSq) {
  if (this != agent) {
    if (_nearAgents.size() != _maxNeighbors || distSq <= getMaxAgentRange()) {
      if (_nearAgents.size() != _maxNeighbors) {
        _nearAgents.push_back(NearAgent(distSq, agent));
      }
      size_t i = _nearAgents.size() - 1;
      while (i != 0 && distSq < _nearAgents[i - 1].distanceSquared) {
        _nearAgents[i] = _nearAgents[i - 1];
        --i;
      }
      _nearAgents[i] = NearAgent(distSq, agent);
    }
  }
}

////////////////////////////////////////////////////////////////

void BaseAgent::insertObstacleNeighbor(const Obstacle* obstacle, float distSq) {
  // the assumption is that two obstacle neighbors MUST have the same classID
  if (obstacle->_class & _obstacleSet) {
    if (distSq < _neighborDist * _neighborDist) {
      _nearObstacles.push_back(NearObstacle(distSq, obstacle));

      size_t i = _nearObstacles.size() - 1;
      while (i != 0 && distSq < _nearObstacles[i - 1].distanceSquared) {
        _nearObstacles[i] = _nearObstacles[i - 1];
        --i;
      }
      _nearObstacles[i] = NearObstacle(distSq, obstacle);
    }
  }
}

///////////////////////////////////////////////////////////
void BaseAgent::addVelModifier(BFSM::VelModifier* v) { _velModifiers.push_back(v); }

///////////////////////////////////////////////////////////

void BaseAgent::startQuery() {
  _nearAgents.clear();
  _nearObstacles.clear();
};

///////////////////////////////////////////////////////////

void BaseAgent::filterAgent(const BaseAgent* agent, float distance) {
  insertAgentNeighbor(agent, distance);
};

///////////////////////////////////////////////////////////

void BaseAgent::filterObstacle(const Obstacle* obstacle, float distance) {
  insertObstacleNeighbor(obstacle, distance);
};

///////////////////////////////////////////////////////////

float BaseAgent::getMaxAgentRange() {
  if (_nearAgents.size() == _maxNeighbors) {
    return _nearAgents.back().distanceSquared;
  }

  return _neighborDist * _neighborDist;
}

}  // namespace Agents
}  // namespace Menge
