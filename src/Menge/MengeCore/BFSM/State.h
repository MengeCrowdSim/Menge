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
 @file    State.h
 @brief    The definition of the BFSM state nodes.
 */

#ifndef __FSMNODE_H__
#define __FSMNODE_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/Transitions/Transition.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#include <cassert>
#include <set>
#include <vector>

namespace Menge {

namespace BFSM {

// forward declaration
class GoalSelector;
class Goal;
class FSM;

/*!
 @brief    Exception class for BFSM states.
 */
class MENGE_API StateException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  StateException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  StateException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    Exception thrown when the state has an error which cannot be recovered from.
 */
class StateFatalException : public StateException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  StateFatalException() : MengeException(), StateException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  StateFatalException(const std::string& s)
      : MengeException(s), StateException(), MengeFatalException() {}
};

///////////////////////////////////////////////////////////////////

/*!
 @brief    The basic state of the behavior finite state machine.

 A state node in the finite state machine for determining "behaviors". The state requires a velocity
 component to determine how the preferred velocity is computed (see VelComponent). State's can also
 include actions, taken on an agent when it leaves the state and an optional corresponding action
 upon leaving the state (see Action). Agents leave the state based on transitions (see Transition).

 State's can be marked as "final". Agents *can* transition out of final states. However, in any
 given time step, when all agents are in final states the simulation stops.
 */
class State {
  /*!
   @brief    Counter on total states created.

   Gives each state a globally unique identifier.
   */
  static size_t COUNT;

  /*!
   @brief    A zero-vector to use with goal positions.
   */
  static Math::Vector2 NULL_POINT;

 public:
  /*!
   @brief    Constructor.

   @param    name    The name of this state.
   */
  State(const std::string& name);

  /*!
   @brief    Destructor.
   */
  ~State();

  /*!
   @brief    Gets the tasks for all of the state's FSM elements.

   @param    fsm      A pointer to the fsm.  Tasks are fed to the fsm.
   */
  void getTasks(FSM* fsm);

  /*!
   @brief    Modifies the input preferred velocity to reflect a velocity for the agent specified.

   @param    agent      The agent for which a preferred velocity is computed.
   @param    velocity  The preferred velocity object (by reference) to modify reflecting the agent's
                      new velocity.
   */
  void getPrefVelocity(Agents::BaseAgent* agent, Agents::PrefVelocity& velocity);

  /*!
   @brief   Attempts to update `this` State's velocity component for the given `agent` (e.g., 
            `agent` is following a moving goal).
   */
  void updateVelCompForMovingGoals(Agents::BaseAgent* agent);

  /*!
   @brief    Set whether the state is final or not.

   @param    isFinal    If true, the state is set to be final, if false, not.
   */
  inline void setFinal(bool isFinal) { _final = isFinal; }

  /*!
   @brief    Reports if the state is final or not.

   @returns  True if the state is a final state, false otherwise.
   */
  inline bool getFinal() const { return _final; }

  /*!
   @brief    Test the transitions out of this state for the given agent.

   The transitions will be tested in priority order. The first transition which is found to be
   active will be taken.

   @param    agent    The agent to test w.r.t. the transitions.
   @returns    A pointer to the next state if a transition is active, otherwise, it returns NULL,
              meaning the agent remains in this state.
   */
  State* testTransitions(Agents::BaseAgent* agent);

  /*!
   @brief    Automatically called when an agent enters the state.

   @param    agent    The agent who entered the state.
   */
  virtual void enter(Agents::BaseAgent* agent);

  /*!
   @brief    Automatically called when an agent leaves the state.

   @param    agent    The agent who left the state.
   */
  virtual void leave(Agents::BaseAgent* agent);

  /*!
   @brief    Add a transition to the state.

   It is assumed that the transition is already correctly connected to the states as is required
   (the "to" and "from" states). The order the transitions are added inherently defines their
   priority.

   @param    t    The transition to add.
   */
  void addTransition(Transition* t) { transitions_.push_back(t); }

  /*!
   @brief    Sets the velocity component to the state.

   Once this is called, the State takes ownership of the velocity component and the caller should
   not destroy it.

   @param    vc    The velocity component to set.
   */
  void setVelComponent(VelComponent* vc) { _velComponent = vc; }

  /*!
   @brief    Retrieves the velocity component.
   */
  VelComponent* getVelComponent() { return _velComponent; }

  /*!
   @brief    Add an action to the state.

   @param    a    The action to add.
   */
  void addAction(Action* a) { actions_.push_back(a); }

  /*!
   @brief    Add an velocity modifier to the state

   @param    v    The modifier to add
   */
  void addVelModifier(VelModifier* v) { velModifiers_.push_back(v); }

  /*!
   @brief    Returns the *globally* unique state identifier.

   The identifier is unique w.r.t. all other states (although the same identifier may be used for
   other entities in their own contexts).

   @returns  The state's identifier.
   */
  size_t getID() const { return _id; }

  /*!
   @brief    Returns the name of the state.

   @returns  The state's name.
   */
  const std::string& getName() const { return _name; }

  /*!
   @brief    Returns the number of agents in this state.

   @returns    The number of agents in this state.
   */
  size_t getPopulation() const;

  /*!
   @brief    Sets the goal selector for the state

   Once the goal selector is assigned to the state, the state takes ownership of the memory and is
   responsible for freeing it.

   @param    selector  A pointer to the goal selector.
   */
  void setGoalSelector(GoalSelector* selector);

  /*!
   @brief    Returns a pointer to the goal selector.

   @returns  Pointer to the state's goal selector.
   */
  GoalSelector* getGoalSelector() { return _goalSelector; }

  /*!
   @brief    Clears the state's current goal selector.
   */
  void clearGoalSelector();

  /*!
   @brief    Get access to the state transitions.

   @returns  The transitions.
   */
  const std::vector<Transition*> getTransitions() const { return transitions_; }

  /*!
   @brief    Acquire a state goal.

   @param    goalId    The identifier for the desired goal
   @returns  The goal mapped to the id.
   */
  const Goal* getGoal(size_t goalId) { return _goals[goalId]; }

 protected:
  /*!
   @brief    Test the transitions out of this state, tracking cycles.

   The transitions will be tested in priority order. The first transition which is found to be
   active will be taken.

   @param    agent      The agent to test w.r.t. the transitions.
   @param    visited    The set of states visited during transition testing. Used to prevent cycles.
   @returns    A pointer to the next state if a transition is active, otherwise, it returns NULL,
              meaning the agent remains in this state.
   */
  State* testTransitions(Agents::BaseAgent* agent, std::set<State*>& visited);

  /*!
   @brief    The single velocity component associated with this state.
   */
  VelComponent* _velComponent;

  /*!
   @brief    A priority-ordered list of transitions to determine if the state changes.
   
   The order of the transitions in the implicitly defines the testing priority.
   */
  std::vector<Transition*> transitions_;

  /*!
   @brief    A priority-ordered list of velocity modifiers to determine if the state changes.
   
   The order of the modifierss in the implicitly defines the testing priority.
   */
  std::vector<VelModifier*> velModifiers_;

  /*!
   @brief    Actions to take upon entering and leaving the state.
   */
  std::vector<Action*> actions_;

  /*!
   @brief    Determines if the state is a final state (true), or not (false).
   */
  bool _final;

  /*!
   @brief    The goal selector for this state.
   */
  GoalSelector* _goalSelector;

  /*!
   @brief      A mapping from agent id to its per-agent goal.
   */
  HASH_MAP<size_t, Goal*> _goals;

  /*!
   @brief    The name of the state.
   */
  std::string _name;

  /*!
   @brief    The globally unique id of state
   */
  size_t _id;

  /*!
   @brief    The lock for accessing the goals.
   */
  ReadersWriterLock _goalLock;
};
}  // namespace BFSM
}  // namespace Menge

#endif  //__FSMNODE_H__
