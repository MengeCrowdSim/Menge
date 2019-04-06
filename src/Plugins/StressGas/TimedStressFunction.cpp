#include "TimedStressFunction.h"
#include "AgentStressor.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Core.h"

namespace StressGAS {

using Menge::Agents::BaseAgent;

/////////////////////////////////////////////////////////////////////
//                   Implementation of TimedStressFunction
/////////////////////////////////////////////////////////////////////

TimedStressFunction::TimedStressFunction(float duration, BaseAgent* agent, AgentStressor* stressor,
                                         float coolDuration)
    : StressFunction(agent, stressor, coolDuration) {
  _stressRate = 1.f / duration;
}

/////////////////////////////////////////////////////////////////////

float TimedStressFunction::updateStress() {
  _stressLevel += Menge::SIM_TIME_STEP * _stressRate;
  if (_stressLevel > 1.f) {
    _stressLevel = 1.f;
  }
  return _stressLevel;
}

}  // namespace StressGAS
