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

/*!
 @file    SimulatorState.h
 @brief    A snapshot of the simulator state.
 */
// TODO: Make this a complete snapshot.

#ifndef __SIMULATOR_STATE_H__
#define __SIMULATOR_STATE_H__

#include "MengeCore/mengeCommon.h"

#include <string>

namespace Menge {

namespace Agents {
/*!
 @brief    A class which caches the state of the simulator.

 This version of the class is, as yet, incomplete. Ultimately, it will store all critical parameters
 of the simulator such that the simulation can be restarted from this state.

 This version is currently used to determine agent's starting state.
 */
class SimulatorState {
 public:
  /*!
   @brief    Constructor.
   */
  SimulatorState();

  /*!
   @brief    Sets the state for the given agent.

   @param    id      The identifier of the agent.
   @param    stateName  The name of the state to which the agent belongs.
   */
  void setAgentState(size_t id, const std::string& stateName);

  /*!
   @brief    Reports the state name for the given agent.

   @param    id      The identifier of the agent.
   @returns  The name of this agent's start state.
   */
  const std::string getAgentState(size_t id) const;

 protected:
  /*!
   @brief    A mapping from agent id to state name
   */
  HASH_MAP<size_t, std::string> _agentStates;
};

}  // namespace Agents
}  // namespace Menge
#endif  // __SIMULATOR_STATE_H__
