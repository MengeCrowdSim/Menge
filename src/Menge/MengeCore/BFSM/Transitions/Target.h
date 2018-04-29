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
 @file    Target.h
 @brief    The basis for determing what an active transition leads to.
 */

#ifndef __TARGET_H__
#define __TARGET_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

#include <map>
#include <string>

namespace Menge {

// forward declarations
namespace Agents {
class BaseAgent;
}

namespace BFSM {
// forward declarations
class State;
class Goal;

/*!
 @brief    The base class for transition targets.

 A transition target is the state a transition moves an agent to. In the simplest case, the
 transition simply connects two states and when the transition is active, the agent moves from the
 source state to the destination state.

 However, Target's can be more complex. The destination can be one of a set of states selected by
 arbitrary criteria (such as probability). The target is responsible for providing a next state
 according to its criteria upon request.
 */
class MENGE_API TransitionTarget : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  TransitionTarget();

  /*!
   @brief    Copy constructor.

   @param    tgt    TransitionTarget to copy.
   */
  TransitionTarget(const TransitionTarget& tgt);

 protected:
  /*!
   @brief    Protected destructor.
   */
  virtual ~TransitionTarget();

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
   @brief    Determines the next state for the given agent.

   This function defines the main functionality of the target. Given the current agent and the
   class's internal state, it determines the state to move the agent into. This should be overridden
   by each sub-class.

   @param    agent    The agent to test the transition for.
   @returns  A pointer to the next state.
   */
  virtual State* nextState(Agents::BaseAgent* agent) = 0;

  /*!
   @brief    Performs any necessary connections to the target state(s).

   Sub-classes must override this because a transition must transition to a state. See the
   sub-classes SingleTransition for an example.

   @param    stateMap    A mapping from state names to state pointers.
   @returns  True if connection was successful, false otherwise.
   */
  virtual bool connectStates(std::map<std::string, State*>& stateMap) = 0;

  /*!
   @brief    Create a copy of this target.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this target - such that there is no shared objects between this and
              its copy.
   */
  virtual TransitionTarget* copy() = 0;
};

/////////////////////////////////////////////////////////////////////////////////

/*!
 @brief    A TransitionTarget which assumes there is only one destination state.

 This will always return a single state, regardless of the agent.
 */
class MENGE_API SingleTarget : public TransitionTarget {
 public:
  /*!
   @brief    Default constructor.
   */
  SingleTarget();

  /*!
   @brief    Copy constructor.

   @param    tgt    The transition target to copy.
   */
  SingleTarget(const SingleTarget& tgt);

  /*!
   @brief    Constructor.

   @param    toName    The name of the destination (aka "to") node.
   */
  SingleTarget(const std::string& toName);

  // TOOD: Change the goalPoint into a Goal instance
  /*!
   @brief    Determines the next state for the given agent.

   This function defines the main functionality of the target. Given the current agent and the
   class's internal state, it determines the state to move the agent into. This should be overridden
   by each sub-class.

   @param    agent    The agent to test the transition for.
   @returns  A pointer to the next state.
   */
  virtual State* nextState(Agents::BaseAgent* agent) { return _next; }

  /*!
   @brief    Performs any necessary connections to the target state(s).

   Sub-classes must override this because a transition must transition to a state. See the
   sub-classes SingleTransition for an example.

   @param    stateMap    A mapping from state names to state pointers.
   @returns  True if connection was successful, false otherwise.
   */
  virtual bool connectStates(std::map<std::string, State*>& stateMap);

  /*!
   @brief    Create a copy of this target.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this target - such that there is no shared objects between this and
              its copy.
   */
  virtual TransitionTarget* copy();

 protected:
  /*!
   @brief    The name of the state to which this transition leads.
   */
  std::string _nextName;

  /*!
  @brief    A pointer to the state to which this transition leads.
  */
  State* _next;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __TARGET_H__
