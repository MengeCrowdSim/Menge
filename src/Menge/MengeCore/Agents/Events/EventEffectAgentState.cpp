#include "MengeCore/Agents/Events/EventEffectAgentState.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Events/EventException.h"
#include "MengeCore/Agents/StateSelectors/StateSelectorDatabase.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/Core.h"

namespace Menge {

using Agents::BaseAgent;
using Agents::StateSelectorDB;
using BFSM::State;
using std::string;

/////////////////////////////////////////////////////////////////////
//          Implementation of EventEffectAgentState
/////////////////////////////////////////////////////////////////////

EventEffectAgentState::EventEffectAgentState() : AgentEventEffect(), _selector(nullptr) {}

/////////////////////////////////////////////////////////////////////

EventEffectAgentState::~EventEffectAgentState() {
  if (_selector) _selector->destroy();
}

/////////////////////////////////////////////////////////////////////

void EventEffectAgentState::agentEffect(BaseAgent* agent) {
  State* nextState = StateForAgent();
  State* currState = ACTIVE_FSM->getCurrentState(agent);
  if (_reenter || currState != nextState) {
    currState->leave(agent);
    nextState->enter(agent);
    ACTIVE_FSM->setCurrentState(agent, nextState->getID());
  }
}

/////////////////////////////////////////////////////////////////////

State* EventEffectAgentState::StateForAgent() const {
  string stateName = _selector->getState();
  // Get the state pointer from the BFSM.
  auto state = ACTIVE_FSM->getNode(stateName);
  if (!state) throw EventFatalException("Trying to switch to an unknown state: " + stateName);
  return state;
}

/////////////////////////////////////////////////////////////////////
//          Implementation of EventEffectAgentStateFactory
/////////////////////////////////////////////////////////////////////

EventEffectAgentStateFactory::EventEffectAgentStateFactory() : EventEffectFactory() {
  _reenterID = _attrSet.addIntAttribute("reenter", false /*required*/, 0);
}

/////////////////////////////////////////////////////////////////////

bool EventEffectAgentStateFactory::setFromXML(EventEffect* effect, TiXmlElement* node,
                                              const std::string& behaveFldr) const {
  EventEffectAgentState* aEffect = dynamic_cast<EventEffectAgentState*>(effect);
  assert(aEffect != 0x0 &&
         "Trying to set agent state event effect on an "
         "incompatible object");

  // This parses the target
  if (!EventEffectFactory::setFromXML(aEffect, node, behaveFldr)) return false;

  // Extract the StateSelector from the XML.
  auto selectorNode = node->FirstChildElement("StateSelector");
  assert(selectorNode != nullptr &&
         "The 'set_agent_state' event effect requries a "
         "StateSelector child.");

  aEffect->_selector = StateSelectorDB::getInstance(selectorNode, behaveFldr);
  if (aEffect->_selector == 0x0) {
    logger << Logger::ERR_MSG
           << "Unable to instantiate the state selector "
              "from specification line "
           << selectorNode->Row() << ".";
    return false;
  }
  aEffect->_reenter = _attrSet.getInt(_reenterID) != 0;
  return true;
}

}  // namespace Menge
