#include "MengeCore/Agents/Events/change_state_effect.h"

#include <sstream>

#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/Core.h"

namespace Menge {

/////////////////////////////////////////////////////////////////////
//           Implementation of ChangeStateEffect
/////////////////////////////////////////////////////////////////////

void ChangeStateEffect::finalize() {
  _state = ACTIVE_FSM->getNode(_stateName);
  if (_state == 0x0) {
    std::stringstream ss;
    ss << "Event event unable to to locate state " << _stateName << ".";
    EventSystem::finalizeException(ss.str());
  }
}

/////////////////////////////////////////////////////////////////////

void ChangeStateEffect::agentEffect(Agents::BaseAgent* agent) {
  ACTIVE_FSM->forceStateTransition(agent, _state, _force_reentry);
}

/////////////////////////////////////////////////////////////////////
//           Implementation of ChangeStateEffectFactory
/////////////////////////////////////////////////////////////////////

ChangeStateEffectFactory::ChangeStateEffectFactory() : EventEffectFactory() {
  _stateID = _attrSet.addStringAttribute("state", true);
  _forceID = _attrSet.addBoolAttribute("force_reentry", false);
}

/////////////////////////////////////////////////////////////////////

bool ChangeStateEffectFactory::setFromXML(EventEffect* effect, TiXmlElement* node,
                                          const std::string& behaveFldr) const {
  ChangeStateEffect* sEffect = dynamic_cast<ChangeStateEffect*>(effect);
  assert(sEffect != 0x0 &&
         "Trying to set attributes of a state event effect on an incompatible object");

  if (!EventEffectFactory::setFromXML(sEffect, node, behaveFldr)) return false;

  sEffect->_stateName = _attrSet.getString(_stateID);
  sEffect->_force_reentry = _attrSet.getBool(_forceID);

  return true;
}

}  // namespace Menge
