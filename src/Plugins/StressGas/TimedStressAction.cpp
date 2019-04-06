#include "TimedStressAction.h"

#include "TimedStressFunction.h"

#include "MengeCore/Agents/BaseAgent.h"

namespace StressGAS {

using Menge::Agents::BaseAgent;
using Menge::BFSM::Action;

/////////////////////////////////////////////////////////////////////
//                   Implementation of TimedStressAction
/////////////////////////////////////////////////////////////////////

StressFunction* TimedStressAction::makeStressFunction(BaseAgent* agent, AgentStressor* stressor,
                                                      float coolDuration) {
  return new TimedStressFunction(_duration->getValue(), agent, stressor, coolDuration);
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//                   Implementation of TimedStressActionFactory
/////////////////////////////////////////////////////////////////////

TimedStressActionFactory::TimedStressActionFactory() {
  _durationId = _attrSet.addFloatDistAttribute("duration_", true, 10.f, 1.f);
}

/////////////////////////////////////////////////////////////////////
//
bool TimedStressActionFactory::setFromXML(Action* action, TiXmlElement* node,
                                          const std::string& behaveFldr) const {
  TimedStressAction* pAction = dynamic_cast<TimedStressAction*>(action);
  assert(pAction != 0x0 && "Trying to set stress action properties on an incompatible object");
  if (!BaseStressActionFactory::setFromXML(action, node, behaveFldr)) {
    return false;
  }

  // extract stress duration
  if (pAction->_duration != 0x0) delete pAction->_duration;
  pAction->_duration = _attrSet.getFloatGenerator(_durationId);

  return true;
}
}  // namespace StressGAS
