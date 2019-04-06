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

#include "AircraftAction.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/BFSM/Actions/PropertyAction.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/GoalSet.h"

#include <iostream>

namespace Aircraft {

using Menge::Logger;
using Menge::logger;
using Menge::Agents::BaseAgent;
using Menge::BFSM::Action;
using Menge::BFSM::ActionFactory;
using Menge::BFSM::MAX_ACCEL;
using Menge::BFSM::MAX_ANGLE_VEL;
using Menge::BFSM::MAX_SPEED;
using Menge::BFSM::NEIGHBOR_DIST;
using Menge::BFSM::NO_PROPERTY;
using Menge::BFSM::PREF_SPEED;
using Menge::BFSM::PRIORITY;
using Menge::BFSM::RADIUS;

/////////////////////////////////////////////////////////////////////
//                   Implementation of PropertyXAction
/////////////////////////////////////////////////////////////////////

PropertyXAction::PropertyXAction()
    : Action(),
      _xOrigin(0.f),
      _originValue(0.f),
      _scale(0.f),
      _property(NO_PROPERTY),
      _originalMap(),
      _lock() {}

/////////////////////////////////////////////////////////////////////

PropertyXAction::~PropertyXAction() { _originalMap.clear(); }

/////////////////////////////////////////////////////////////////////

void PropertyXAction::onEnter(BaseAgent* agent) {
  float value = (agent->_pos.x() - _xOrigin) * _scale + _originValue;
  _lock.lock();
  switch (_property) {
    case MAX_SPEED:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_maxSpeed;
      agent->_maxSpeed = value;
      break;
    case MAX_ACCEL:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_maxAccel;
      agent->_maxAccel = value;
      break;
    case PREF_SPEED:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_prefSpeed;
      agent->_prefSpeed = value;
      break;
    case MAX_ANGLE_VEL:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_maxAngVel;
      agent->_maxAngVel = value;
      break;
    case NEIGHBOR_DIST:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_neighborDist;
      agent->_neighborDist = value;
      break;
    case PRIORITY:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_priority;
      agent->_priority = value;
      break;
    case RADIUS:
      if (_undoOnExit) _originalMap[agent->_id] = agent->_radius;
      agent->_radius = value;
      break;
    case NO_PROPERTY:
      // NO_PROPERTY is considered a no-op.
      break;
  }
  _lock.release();
}

/////////////////////////////////////////////////////////////////////

void PropertyXAction::leaveAction(BaseAgent* agent) {
  if (_undoOnExit) {
    _lock.lock();
    std::map<size_t, float>::iterator itr = _originalMap.find(agent->_id);
    assert(itr != _originalMap.end() &&
           "An agent is exiting a state that it apparently never entered");
    float value = itr->second;
    _originalMap.erase(itr);
    _lock.release();
    switch (_property) {
      case MAX_SPEED:
        agent->_maxSpeed = value;
        break;
      case MAX_ACCEL:
        agent->_maxAccel = value;
        break;
      case PREF_SPEED:
        agent->_prefSpeed = value;
        break;
      case MAX_ANGLE_VEL:
        agent->_maxAngVel = value;
        break;
      case NEIGHBOR_DIST:
        agent->_neighborDist = value;
        break;
      case PRIORITY:
        agent->_priority = value;
        break;
      case RADIUS:
        agent->_radius = value;
        break;
      case NO_PROPERTY:
        // NO_PROPERTY is considered a no-op.
        break;
    }
  }
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of PropertyXActFactory
/////////////////////////////////////////////////////////////////////

bool PropertyXActFactory::setFromXML(Action* action, TiXmlElement* node,
                                     const std::string& behaveFldr) const {
  PropertyXAction* pAction = dynamic_cast<PropertyXAction*>(action);
  assert(pAction != 0x0 && "Trying to set property action properties on an incompatible object");
  if (!ActionFactory::setFromXML(action, node, behaveFldr)) {
    return false;
  }
  // set the target property
  const char* pName = node->Attribute("property");
  if (!pName) {
    logger << Logger::ERR_MSG << "The property action defined on line " << node->Row();
    logger << " did not define the \"property\" attribute.";
    return false;
  }
  pAction->_property = Menge::parsePropertyName(pName);
  if (pAction->_property == NO_PROPERTY) {
    logger << Logger::ERR_MSG << "The set property x action defined online ";
    logger << node->Row();
    logger << " specified an invalid value for the \"property\" attribute.";
    return false;
  }

  double d;
  // Get the x-origin value
  if (node->Attribute("origin", &d)) {
    pAction->_xOrigin = (float)d;
  } else {
    logger << Logger::WARN_MSG << "The set property x action defined on line ";
    logger << node->Row();
    logger << " did not define the \"origin\" attribute.  Using the default value 0.0.";
    pAction->_xOrigin = 0.f;
  }

  // Get the origin value
  if (node->Attribute("origin_value", &d)) {
    pAction->_originValue = (float)d;
  } else {
    logger << Logger::WARN_MSG << "The set property x action defined on line ";
    logger << node->Row() << " did not define the \"origin_value\" attribute.  ";
    logger << "Using the default value 0.0.";
    pAction->_originValue = 0.f;
  }

  // Get the scale value
  if (node->Attribute("scale", &d)) {
    pAction->_scale = (float)d;
  } else {
    logger << Logger::WARN_MSG << "The set property x action defined on line ";
    logger << node->Row() << " did not define the \"scale\" attribute.  ";
    ;
    logger << "Using the default value 0.0.";
    pAction->_scale = 0.f;
  }
  return true;
}
}  // namespace Aircraft
