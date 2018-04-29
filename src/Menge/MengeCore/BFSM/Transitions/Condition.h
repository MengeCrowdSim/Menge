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
 @file    Condition.h
 @brief    The basis for determining the conditions under which transitions become "active" (and
          are taken).
 */
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

namespace Menge {

// forward declarations
namespace Agents {
class BaseAgent;
}

namespace BFSM {
// forward declaration
class Goal;

/*!
 @brief    The base class for transition conditions.

 This class is an abstract class used to define arbitrary conditions for finite-state-machine
 transitions.
 */
class MENGE_API Condition : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  Condition() : Element() {}

  /*!
   @brief    Copy Constructor.

   @param    cond    The condition to copy from.
   */
  Condition(const Condition& cond) {}

 protected:
  /*!
   @brief    Protected destructor.
   */
  virtual ~Condition() {}

 public:
  /*!
   @brief    Called when an agent enters a state with this exiting transition.

   Sub-classes should use this function as the opportunity to cache any particular per-agent data.

   @param    agent    The agent who has entered the state which uses this transition.
   */
  virtual void onEnter(Agents::BaseAgent* agent) {}

  /*!
   @brief    Called when an agent exits the state with this transition.

   @param    agent    The agent who left the state.
   */
  virtual void onLeave(Agents::BaseAgent* agent) {}

  /*!
   @brief    Reports if the conditions have been met.

   This function defines the main functionality of a definition. Given the current agent and its
   internal state, it determines if the conditions have been met.  This should be overridden by
   each sub-class.

   @param   agent   The agent to test the transition for.
   @param   goal    The agent's goal (although this may be ignored).
   @returns  True if the condition has been met.
   */
  virtual bool conditionMet(Agents::BaseAgent* agent, const Goal* goal) = 0;

  /*!
   @brief    Create a copy of this condition.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this condition - such that there is no shared objects between this
              and its copy.
   */
  virtual Condition* copy() = 0;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __CONDITION_H__
