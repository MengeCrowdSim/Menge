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

#ifndef __STRESS_MANAGER_H__
#define __STRESS_MANAGER_H__

#include <map>
#include <vector>

#include "StressFunction.h"

#include "MengeCore/Runtime/ReadersWriterLock.h"
#include "macros.h"

// Forward declration
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace StressGAS {

/*!
@class StressManager

@brief	Manages stress applied to agents over time. Determines if stress needs to be applied, and
        does so.

This class contains several maps to initial agent values, and agents under effect.
 */
class StressManager {
 public:
  /*!
   @brief	Default constructor
   */
  StressManager();

  /*!
   @brief	Destructor
   */
  ~StressManager();

  /*!
   @brief	Checks if stress needs to be applied, does so if necessary
   */
  void updateStress();

  /*!
   @brief	Adds stress to the provided agent.
   @param	agentId		The identifier for the agent that gets stress.
   */
  void stressAgent(size_t agentId);

  /*!
   @brief	Reports if the given agent is in the stress system.

   @param	agent	The BaseAgent to check.
   @returns	True if the given agent is in the system
   */
  bool isInSystem(const Menge::Agents::BaseAgent* agent);

  /*!
   @brief	Reports the stress function for the given agent.

   @param	agent	The agent to query -- assumed to be non-null.
   @return	Null if there is no stress function for this agent, otherwise a pointer to that agent's
            stress function.
   */
  StressFunction* getStressFunction(const Menge::Agents::BaseAgent* agent);

  /*!
   @brief	Sets stress function for the given agent. If there is currently a function it will be
          deleted and removed.

   @param 		  	  agent	  The agent -- assumed to be non-null.
   @param [in,out]	func 	  The function to register for this agent; must be non-null but this is
                            not tested.
   */
  void setStressFunction(const Menge::Agents::BaseAgent* agent, StressFunction* func);

  /*!
   @brief	Pops the stress function described by agent.

   @param	agent	  The agent to query -- assumed to be non-null.
   @return	Null if the agent has no registered function; otherwise the registered stress function
            will be removed from the manager.
   */
  StressFunction* popStressFunction(const Menge::Agents::BaseAgent* agent);

 protected:
  /*! The set of agents which receive stress and their corresponding stress functions. */
  HASH_MAP<const Menge::Agents::BaseAgent*, StressFunction*> _stressFunctions;

  /*! A lock for managing access to the function map. */
  Menge::ReadersWriterLock _lock;
};
}  // namespace StressGAS
#endif  // __STRESS_MANAGER_H__
