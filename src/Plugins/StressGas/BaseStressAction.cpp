#include "BaseStressAction.h"

#include <algorithm>

#include "AgentStressor.h"
#include "StressGlobals.h"
#include "StressTask.h"

#include "MengeCore/Agents/BaseAgent.h"

namespace StressGAS {

using Menge::logger;
using Menge::Logger;
using Menge::Agents::BaseAgent;
using Menge::BFSM::Action;
using Menge::BFSM::ActionFactory;
using Menge::BFSM::Task;

/////////////////////////////////////////////////////////////////////
//                   Implementation of BaseStressAction
/////////////////////////////////////////////////////////////////////

const float BaseStressAction::DEFAULT_COOL_DURATION = 5.f;  ///< 5 seconds to cool from 100%

/////////////////////////////////////////////////////////////////////

BaseStressAction::~BaseStressAction() {
  if (_deltaNeighborDist != 0x0) delete _deltaNeighborDist;
  if (_deltaMaxNeighbors != 0x0) delete _deltaMaxNeighbors;
  if (_deltaRadius != 0x0) delete _deltaRadius;
  if (_deltaTimeHorizon != 0x0) delete _deltaTimeHorizon;
  if (_deltaPrefSpeed != 0x0) delete _deltaPrefSpeed;
  if (_coolDuration != 0x0) delete _coolDuration;
}

/////////////////////////////////////////////////////////////////////

void BaseStressAction::onEnter(BaseAgent* agent) {
  AgentStressor* stressor = new AgentStressor(
      _deltaNeighborDist->getValue(), _deltaMaxNeighbors->getValue(), _deltaRadius->getValue(),
      _deltaPrefSpeed->getValue(), _deltaTimeHorizon->getValue());

  // TODO (curds01): define the cool down value in XML
  StressFunction* newFunc = makeStressFunction(agent, stressor, _coolDuration->getValue());
  StressFunction* func = STRESS_MANAGER->getStressFunction(agent);
  if (func != 0x0) {
    newFunc->initialize(func);
  }

  STRESS_MANAGER->setStressFunction(agent, newFunc);
}

/////////////////////////////////////////////////////////////////////

void BaseStressAction::leaveAction(BaseAgent* agent) {
  StressFunction* func = 0x0;
  switch (_exitBehavior) {
    case RESET:
      func = STRESS_MANAGER->popStressFunction(agent);
      func->undoStress();
      delete func;
      break;
    case COOL_DOWN:
      func = STRESS_MANAGER->getStressFunction(agent);
      func->setMode(COOLING);
      break;
    case PAUSE:
      func = STRESS_MANAGER->getStressFunction(agent);
      func->setMode(PAUSED);
      break;
    case CONTINUE:
      break;
  }
}

/////////////////////////////////////////////////////////////////////

Task* BaseStressAction::getTask() { return new StressTask(); };

/////////////////////////////////////////////////////////////////////
//                   Implementation of BaseStressActionFactory
/////////////////////////////////////////////////////////////////////

BaseStressActionFactory::BaseStressActionFactory() {
  _neighborDistId = _attrSet.addFloatDistAttribute("neighbor_dist_", false /*required*/,
                                                   AgentStressor::DEFAULT_NEIGHBOR_DIST_DELTA, 1.f);
  _maxNeighborsId = _attrSet.addFloatDistAttribute("max_neighbors_", false /*required*/,
                                                   AgentStressor::DEFAULT_MAX_NEIGHBORS_DELTA, 1.f);
  _radiusId = _attrSet.addFloatDistAttribute("radius_", false /*required*/,
                                             AgentStressor::DEFAULT_RADIUS_DELTA, 1.f);
  _prefSpeedId = _attrSet.addFloatDistAttribute("pref_speed_", false /*required*/,
                                                AgentStressor::DEFAULT_PREF_SPEED_DELTA, 1.f);
  _timeHorizonId = _attrSet.addFloatDistAttribute("time_horizon_", false /*required*/,
                                                  AgentStressor::DEFAULT_TIME_HORIZON_DELTA, 1.f);
  _exitBehaviorId = _attrSet.addStringAttribute("exit_behavior", true /*required*/, "");
  _coolDurationId = _attrSet.addFloatDistAttribute("cool_duration_", false /*required*/,
                                                   BaseStressAction::DEFAULT_COOL_DURATION, 1.f);
}

/////////////////////////////////////////////////////////////////////

bool BaseStressActionFactory::setFromXML(Action* action, TiXmlElement* node,
                                         const std::string& behaveFldr) const {
  BaseStressAction* sAction = dynamic_cast<BaseStressAction*>(action);
  assert(sAction != 0x0 && "Trying to set stress action properties on an incompatible object");
  if (!ActionFactory::setFromXML(action, node, behaveFldr)) {
    return false;
  }

  if (sAction->_deltaNeighborDist != 0x0) delete sAction->_deltaNeighborDist;
  sAction->_deltaNeighborDist = _attrSet.getFloatGenerator(_neighborDistId);

  if (sAction->_deltaMaxNeighbors != 0x0) delete sAction->_deltaMaxNeighbors;
  sAction->_deltaMaxNeighbors = _attrSet.getFloatGenerator(_maxNeighborsId);

  if (sAction->_deltaRadius != 0x0) delete sAction->_deltaRadius;
  sAction->_deltaRadius = _attrSet.getFloatGenerator(_radiusId);

  if (sAction->_deltaTimeHorizon != 0x0) delete sAction->_deltaTimeHorizon;
  sAction->_deltaTimeHorizon = _attrSet.getFloatGenerator(_timeHorizonId);

  if (sAction->_deltaPrefSpeed != 0x0) delete sAction->_deltaPrefSpeed;
  sAction->_deltaPrefSpeed = _attrSet.getFloatGenerator(_prefSpeedId);

  std::string mode = _attrSet.getString(_exitBehaviorId);
  std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

  // exit behavior
  if (mode == "reset")
    sAction->_exitBehavior = RESET;
  else if (mode == "cool down")
    sAction->_exitBehavior = COOL_DOWN;
  else if (mode == "pause")
    sAction->_exitBehavior = PAUSE;
  else if (mode == "continue")
    sAction->_exitBehavior = CONTINUE;
  else {
    logger << Logger::WARN_MSG
           << "Parameter \"exit_behavior\" has an unrecognizable "
              "value: "
           << mode
           << ".  Should be one of: reset, cool down, pause, or "
              "continue.  Defaulting to reset.";
    sAction->_exitBehavior = RESET;
  }

  if (sAction->_coolDuration != 0x0) delete sAction->_coolDuration;
  sAction->_coolDuration = _attrSet.getFloatGenerator(_coolDurationId);

  return true;
}
}  // namespace StressGAS
