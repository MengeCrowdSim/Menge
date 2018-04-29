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

#ifndef __STRESS_FUNCTION_H__
#define __STRESS_FUNCTION_H__

#include "AgentStressor.h"
#include "StressGasConfig.h"

// forward declaration
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace StressGAS {

// forward declaration
class StressManager;
class AgentStressor;

/*!
 @enum	StressFunctionMode

 @brief	The various states a stress function can be in.
 */
enum StressFunctionMode {
  ACTIVE,   ///< Stress is currently being evaluated.
  COOLING,  ///< Stress level is in cool-down mode.
  PAUSED,   ///< Stress computations are on hold.
  FINISHED  ///< Reports that the stressor has finished cooling down.
};

/*!
 @class	StressFunction

 @brief	The base class for stress functions. The stress function controls how an agent's stress
        changes based on simulator state.
 */
class EXPORT_API StressFunction {
 public:
  /*!
   @brief	Default constructor.

   @param	agent				    The agent to apply stress to.
   @param	stressor			  The agent stressor to use on the agent.
   @param	coolDuration		The amount of time (in seconds) required to cool down from 100% stress to
                          0%.
   */
  StressFunction(Menge::Agents::BaseAgent* agent, AgentStressor* stressor, float coolDuration)
      : _agent(agent),
        _mode(ACTIVE),
        _stressor(stressor),
        _coolDownRate(1.f / coolDuration),
        _stressLevel(0.f) {
    stressor->setBaseline(agent);
  }

  /*!
   @brief	Destructor.
   */
  virtual ~StressFunction();

  /*!
   @fn	float StressFunction::getStressLevel() const

   @brief	Gets stress level.
   @return	The stress level.
   */
  float getStressLevel() const { return _stressLevel; }

  /*!
   @brief	Initializes stress and baseline from the given stress function.

   @param	func	The function to read current settings from.
   */
  void initialize(const StressFunction* func);

  /*!
   @brief	Sets the active state for this function.

   If active, the agent's stress levels can change upon evaluation.

   @param	state	True to state.
   */
  void setMode(StressFunctionMode state) { _mode = state; }

  /*!
   @brief	Reports the functions active state.

   @return	True if the function is active.
   */
  StressFunctionMode getMode() const { return _mode; }

  /*! Reports true if the function is finished.

   @returns	True if the function is finished cooling down and will no longer create stress.
   */
  bool isFinished() const { return _mode == FINISHED; }

  /*!
   @brief	Evaluate the simulation state and modify agent stress level and state as appropriate.
   */
  void processStress();

  /*!
   @brief		Updates the stress level for this agent.

   @returns	The new stress value for the underlying agent.
   */
  virtual float updateStress() = 0;

  /*!
   @brief		Undoes the stress for the underlying agent.
   */
  void undoStress();

 private:
  /*!
   @brief		Reduces the stress level based on time.
   */
  void coolDown();

 protected:
  /*! @brief	The agent to operate on. */
  Menge::Agents::BaseAgent* _agent;

  /*! @brief	The stressor to apply to the agent. */
  AgentStressor* _stressor;

  /*! @brief	True indicates that stress will be computed. */
  StressFunctionMode _mode;

  /*! @brief	The agent's current stress level. */
  float _stressLevel;

  /*! @brief	The time it takes for the agent's stress level to cool down from 100% to 0%. */
  float _coolDownRate;
};
}  // namespace StressGAS

#endif  // __STRESS_FUNCTION_H__
