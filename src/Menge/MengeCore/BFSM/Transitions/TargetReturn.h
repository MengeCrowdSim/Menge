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
 @file    TargetReturn.h
 @brief    Defines a transition target that returns the agent to the state from which he entered this
          state.
 */

#ifndef __TARGET_RETURN_H__
#define __TARGET_RETURN_H__

#include "MengeCore/BFSM/Transitions/Target.h"
#include "MengeCore/BFSM/Transitions/TargetFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#include <list>

namespace Menge {

namespace BFSM {

// forward declarations
class State;
class ReturnTargetFactory;

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The definition of the return target.

 The return target causes the agent to go back to the state from which the agent transitioned to the
 current state. However, this isn't necessarily the *literal* previous state. The FSM can advance
 multiple states in a single time step. This target returns the agent to the state it was in at the
 beginning of this call to advance (and not the immediately previous state which was simply passed
 through.)
 */
class MENGE_API ReturnTarget : public TransitionTarget {
 public:
  /*!
   @brief    Constructor
   */
  ReturnTarget();

  /*!
   @brief    Copy constructor

   @param    tgt    The transition target to copy.
   */
  ReturnTarget(const ReturnTarget& tgt);

  /*!
   @brief    Called when an agent enters a state with this exiting transition.

   Sub-classes should use this function as the opportunity to cache any particular per-agent data.

   @param    agent    The agent who has entered the state which uses this transition.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  /*!
   @brief    Called when an agent exits the state with this transition.

   @param    agent    The agent who left the state.
   */
  virtual void onLeave(Agents::BaseAgent* agent);

  /*!
   @brief    Determines the next state for the given agent.

   This function defines the main functionality of the target. Given the current agent and the
   class's internal state, it determines the state to move the agent into. This should be overridden
   by each sub-class.

   @param    agent    The agent to test the transition for.
   @returns  A pointer to the next state.
   */
  virtual State* nextState(Agents::BaseAgent* agent);

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

  friend class ReturnTargetFactory;

 protected:
  /*!
   @brief    A mapping between an agent id and its return state.
   */
  std::map<size_t, State*> _targets;

  /*!
   @brief    Lock to protect _targets;
   */
  ReadersWriterLock _lock;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the ReturnTarget
 */
class MENGE_API ReturnTargetFactory : public TargetFactory {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "return"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Defines the transition target as the state the agent was in when "
           "it advanced to this state.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All TargetFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding condition type. The various field values of the instance will be set in a
   subsequent call to TargetFactory::setFromXML. The caller of this function takes ownership of the
   memory.

   @returns    A pointer to a newly instantiated TransitionTarget class.
   */
  virtual TransitionTarget* instance() const { return new ReturnTarget(); }
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __TARGET_RETURN_H__
