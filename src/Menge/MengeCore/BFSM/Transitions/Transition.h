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
 @file    Transition.h
 @brief    The definition of state transitions in the BFSM
 */

#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "MengeCore/BFSM/fsmCommon.h"

#include <map>

namespace Menge {

namespace BFSM {

class State;
class Condition;
class TransitionTarget;
class FSM;
class Goal;

/*!
 @brief    The transition between BFSM states.

 Transitions can key on arbitrary state. For each agent, the transition is "tested". If the
 condition is met, the transition is active and that reports to the state (which then "follows" the
 transition).
 */
class Transition {
 public:
  /*!
   @brief    Copy constructor.

   @param    trans    The transition to copy to this one.
   */
  Transition(const Transition& trans);

  /*!
   @brief    Constructor

   The transition takes ownership of the Condition and TransitionTarget given as parameters to the
   constructor and will destroy them in their destructor. As such, they should be unique.

   @param    condition    The condition instance for this transition.
   @param    target      The target instance for this transition.
   */
  Transition(Condition* condition, TransitionTarget* target);

  /*!
   @brief    Destructor.
   */
  ~Transition();

  /*!
   @brief    Performs any necessary connections to the "to" state.

   Sub-classes must override this because a transition must transition to a state. See the
   sub-classes SingleTransition for an example.

   @param      stateMap    A mapping from state names to state pointers.
   @returns    True if connection was successful, false otherwise.
   */
  bool connectStates(std::map<std::string, State*>& stateMap);

  /*!
   @brief    Called when an agent enters a state with this exiting transition.

   @param    agent    The agent who has entered the state which uses this transition.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  /*!
   @brief    Called when an agent exits the state with this transition.

   @param    agent    The agent who left the state.
   */
  void onLeave(Agents::BaseAgent* agent);

  /*!
   @brief    Tests to see if this transition's conditions are met.

   @param    agent    The agent to test the transition for.
   @param    goal    The agent's goal (although this may be ignored).
   @returns  A pointer to the next state if the test passes, otherwise returns NULL.
   */
  State* test(Agents::BaseAgent* agent, const Goal* goal);

  /*!
   @brief    Gets the tasks for all of the transitions target and condition.

   @param    fsm      A pointer to the fsm.  Tasks are fed to the fsm.
   */
  void getTasks(FSM* fsm);

  /*!
   @brief    Creats a deep copy of this transition.

   @returns  A new transition whose values are identical to this one.
   */
  Transition* copy();

 protected:
  /*!
   @brief    The Condition instance for this transition.
   */
  Condition* _condition;

  /*!
   @brief    The target for this transition.
   */
  TransitionTarget* _target;
};

/*!
 @brief    Parses a TinyXML element containing a transition specification

 @param    node          The TinyXML element.
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                        relative to this folder.
 @param    fromName      The name of the state from which the transition originates will be set in
                        this string.
 @returns    A pointer to the new transition implementation (NULL if no valid instance could be
            created).
 */
Transition* parseTransition(TiXmlElement* node, const std::string& behaveFldr,
                            std::string& fromName);

}  // namespace BFSM
}  // namespace Menge
#endif  // __TRANSITION_H__
