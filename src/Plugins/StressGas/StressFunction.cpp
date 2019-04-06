#include "StressFunction.h"
#include "AgentStressor.h"

#include "MengeCore/Core.h"

namespace StressGAS {

/////////////////////////////////////////////////////////////////////
//                   Implementation of StressFunction
/////////////////////////////////////////////////////////////////////

StressFunction::~StressFunction() { delete _stressor; }

/////////////////////////////////////////////////////////////////////

void StressFunction::initialize(const StressFunction* func) {
  _stressLevel = func->_stressLevel;
  _stressor->setBaseline(func->_stressor);
}

//////////////////////////////////////////////////////////////////

void StressFunction::processStress() {
  float stress = 0.f;
  switch (_mode) {
    case ACTIVE:
      updateStress();
      break;
    case COOLING:
      coolDown();
      if (_stressLevel == 0.f) _mode = FINISHED;
      break;
    case PAUSED:
    case FINISHED:
      return;
  }
  _stressor->applyStress(_stressLevel, _agent);
}

/////////////////////////////////////////////////////////////////////

void StressFunction::coolDown() {
  _stressLevel -= _coolDownRate * Menge::SIM_TIME_STEP;
  if (_stressLevel < 0.f) {
    _stressLevel = 0.f;
    _mode = FINISHED;
  }
}

/////////////////////////////////////////////////////////////////////

void StressFunction::undoStress() { _stressor->applyBaseline(_agent); }

}  // namespace StressGAS
