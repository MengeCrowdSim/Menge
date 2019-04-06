#include "StressTask.h"
#include "StressGlobals.h"

#include "MengeCore/BFSM/FSM.h"

namespace StressGAS {

using Menge::BFSM::FSM;
using Menge::BFSM::TaskException;

/////////////////////////////////////////////////////////////////////
//                   Implementation of DensityGridTask
/////////////////////////////////////////////////////////////////////

StressTask::StressTask() : Task() {}

/////////////////////////////////////////////////////////////////////

void StressTask::doWork(const FSM* fsm) throw(TaskException) {
  StressGAS::STRESS_MANAGER->updateStress();
}

/////////////////////////////////////////////////////////////////////

std::string StressTask::toString() const { return "Stress Task"; }

/////////////////////////////////////////////////////////////////////

bool StressTask::isEquivalent(const Task* task) const {
  const StressTask* other = dynamic_cast<const StressTask*>(task);
  if (other == 0x0) {
    return false;
  } else {
    return true;
  }
}
}  // namespace StressGAS
