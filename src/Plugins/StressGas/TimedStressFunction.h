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
#ifndef __TIMED_STRESS_FUNCTION_H__
#define __TIMED_STRESS_FUNCTION_H__

#include "StressFunction.h"
#include "StressGasConfig.h"

// forward declaration
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace StressGAS {

/**
 @brief		The StressFunction that defines stress accumulation based on the passage of time.

 Agents simply accumulate stress as time passes.  The rate at which they accumulate stress
 is:

    `\delta t` / `duration`

 where `\delta` t is the size of the time step and `duration` is the total time a user would
 require to go from zero stress to full stress.
 */
class EXPORT_API TimedStressFunction : public StressFunction {
 public:
  /**
   @brief	Constructor.

   @param	duration			  The duration for the agent to reach 100% stress.
   @param	agent				    The agent to apply stress to.
   @param	stressor			  The agent stressor to use on the agent.
   @param	coolDuration		The amount of time (in seconds) required to cool down from.
   */
  TimedStressFunction(float duration, Menge::Agents::BaseAgent* agent, AgentStressor* stressor,
                      float coolDuration);

  /*!
   @brief		Updates the stress level for this agent.

   @returns	The stress value for the function's agent.
   */
  virtual float updateStress();

 protected:
  /** Percentage of stress accumulated per second of simulation time. */
  float _stressRate;
};
}  // namespace StressGAS

#endif  // __TIMED_STRESS_FUNCTION_H__