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
 @file    FSM.h
 @brief    The definition of the behavior finite state machine.
 */

#ifndef __FSM_H__
#define __FSM_H__

// Finite-state machine used to compute preferred velocity
//  according to varying conditions

#include "MengeCore/BFSM/FSMDescrip.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/MengeException.h"

#include <cassert>
#include <map>
#include <vector>
#ifndef _MSC_VER
#include <string.h>
#endif

namespace Menge {
// forward declaration
namespace Agents {
class SimulatorInterface;
class BaseAgent;
}  // namespace Agents

namespace BFSM {

// Forward declaration
class FsmContext;
class State;
class Transition;
class Goal;
class GoalSet;
class Task;
class FSMDescrip;

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Generic base class for FSM exceptions.
 */
class FSMException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  FSMException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  FSMException(const std::string& s) : MengeException(s) {}
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Exception thrown when the FSM has an error which cannot be recovered from.
 */
class FSMFatalException : public FSMException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  FSMFatalException() : MengeException(), FSMException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  FSMFatalException(const std::string& s)
      : MengeException(s), FSMException(), MengeFatalException() {}
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Templated class for the behavior finite state machine.
 */
class MENGE_API FSM {
 public:
  /*!
   @brief    Constructor.

   @param    sim      The simulator to operate on.
   */
  FSM(Agents::SimulatorInterface* sim);

  /*!
   @brief    Destructor
   */
  ~FSM();

  /*!
   @brief    Collects all tasks from the FSM elements.
   */
  void collectTasks();

  /*!
   @brief    Adds a task to the set.

   Elements can blindly add tasks to the finite state machine (including adding a null pointer. If
   the pointer is null, no work is done.  If the pointer is a duplicate of a previous task, it will
   not be repeatedly added.

   The FSM takes possession of the task and is responsible for cleaning the memory.

   @param    task    A pointer to a task to add.
   */
  void addTask(Task* task);

  /*!
   @brief    Initializes the memory required for the number of agents included in the FSM.

   @param    count    The number of agents.
   */
  void setAgentCount(size_t count);

  /*!
   @brief    Advances the FSM based on the current state for the given agent.

   @param    agent    The agent to advance the FSM for.
   */
  void advance(Agents::BaseAgent* agent);

  /** @brief  Forcibly moves the given agent to the indicated state.

   Causes the `agent` to leave its current state and enter the given `target_state`. Whether or
   not this happens if the agent is already in the `target_state` depends on the `force`
   parameter.

   @param  agent          The agent whose state _may_ change.
   @param  target_state   The target state to move the agent to.
   @param  force          If true, the agent will leave/enter states even if it's already in the
                          target state. If false, agents already in the target state are
                          untouched.
   @returns  true if the agent's previous state was different from its final state.  */
  bool forceStateTransition(Agents::BaseAgent* agent, State* target_state, bool force_reentry);

  /*!
   @brief    Computes the preferred velocity for the given agent based on the FSM's record of which
            state the agent is in.
   */
  void computePrefVelocity(Agents::BaseAgent* agent);

  /*!
   @brief    Get the pointer for the node with the given identifier.

   @param    id    The state identifier.  Id is not validated.
   @returns  A pointer to the state with the given identifier.
   */
  State* getNode(size_t id) { return _nodes[id]; }

  /*!
   @brief    Returns the state with the given name.

   @param    name    The name of the desired state.
   @returns  A pointer to the desired state (if it is found), otherwise, returns null.
   */
  State* getNode(const std::string& name);

  /*!
   @brief    Reports the number of states in the FSM.

   @returns  The number of states in the fsm.
   */
  size_t getNodeCount() const { return _nodes.size(); }

  /*!
   @brief    Adds a state to the BFSM.

   @param    node    The state to add.
   @returns  A unique identifier for the newly added state.
   */
  size_t addNode(State* node);

  /*!
   @brief    Adds the given transition to the FSM.

   The transition already contains a pointer to the destination state (see Transition).

   @param    fromNode    The global identifier from the origin state.
   @param    t            The transition to add.
   @returns  True if the transition is successfully added.
   */
  bool addTransition(size_t fromNode, Transition* t);

  /*!
   @brief    Adds a goal to the indicated goal set.

   @param    goalSet    The id of the goal set which receives the goal.
   @param    goalID    The id of the goal in the goal set.
   @param    goal      The goal to add.
   @returns  A boolean reporting success (true) or failure (false)
   */
  bool addGoal(size_t goalSet, size_t goalID, Goal* goal);

  /*!
   @brief    Retrieves the given goal from the given goal set.

   @param    goalSet    The identifier of the goal set.
   @param    goalID    The identifier of the goal.
   @returns  A pointer to the corresponding goal.  If no such goal exists NULL is returned.
   */
  const Goal* getGoal(size_t goalSet, size_t goalID);

  /*!
   @brief    Retrives the given goal set.

   @param    goalSetID    The identifier of the goal set.
   @returns  A pointer to the corresponding goal set.  If no such goal set exists, NULL is returned.
   */
  const GoalSet* getGoalSet(size_t goalSetID);

  /*!
   @brief    Update the fsm state by one time step

   @returns  A boolean reporting if all agents are in a final state (true) or not (false).
   */
  bool doStep();

  /*!
   @brief    Sets the current state for the given agent.

   @param    agt      The agent whose BFSM state gets set.
   @param    currNode  The unique identifier of the desired state (returned by FSM::addNode).
   */
  void setCurrentState(Agents::BaseAgent* agt, size_t currNode);

  /*!
   @brief    Gets a pointer to the state the agent is currently in.

   @param    agt      The agent.
   @returns  A pointer to the agent's current state.
   */
  State* getCurrentState(const Agents::BaseAgent* agt) const;

  /*!
   @brief    Reports the state the given agent is currently in.

   @param    agent    A pointer to the agent whose state id is returned.
   @returns  The id of the state the given agent is in.
   */
  size_t getAgentStateID(const Agents::BaseAgent* agent) const;

  /*!
   @brief    Reports the state the given agent is currently in.

   @param    agentID    The unique id of the agent whose state id is returned.
   @returns  The id of the state the given agent is in.
   */
  size_t getAgentStateID(size_t agentID) const;

  /*!
   @brief    Reports if all agents are in final states (i.e. the simulation is
   done.)

   @returns  True if all agents are in a final state, false otherwise.
   */
  bool allFinal() const;

  /*!
   @brief    Retrieve the simulator
   */
  inline const Agents::SimulatorInterface* getSimulator() const { return _sim; }

  /*!
   @brief    Performs the work in the FSM's tasks.
   */
  void doTasks();

  /*!
   @brief    Gives every moving goal a chance to update its position.

   @param  time_step  The amount of time to advance the goal's position.
   */
  void moveGoals(float time_step);

  /*!
   @brief    Returns the number of registered tasks.
   */
  size_t getTaskCount() const { return _tasks.size(); }

  /*!
   @brief    Finalize the FSM
   */
  void finalize();

  /*!
   @brief    Returns the BFSM Context for this FSM

   A new FsmContext will be constructed upon each call to this function. It is the responsibility
   of the caller to make sure it gets deleted.

   @returns  A pointer to a valid fsm context.
   */
  FsmContext* getContext();

  /*!
   @brief    Returns the goal sets defined in the simulator.

   @returns  A reference to the goal set map.
   */
  std::map<size_t, GoalSet*>& getGoalSets() { return _goalSets; }

  /*!
   @brief    Add an velocity modifier to the FSM

   @param    v    The modifier to add
   */
  void addVelModifier(VelModifier* v) { _velModifiers.push_back(v); }

  friend FSM* buildFSM(FSMDescrip& fsmDescrip, Agents::SimulatorInterface* sim, bool VERBOSE);

 protected:
  /*!
   @brief    The simulator on which the FSM acts.
   */
  Agents::SimulatorInterface* _sim;

  /*!
   @brief    Number of agents attached to the state machine

   This should be the same as the number of agents in the simulator.
   */
  size_t _agtCount;

  /*!
   @brief    The active state for each agent in the system.
   */
  State** _currNode;

  /*!
   @brief    The states in the BFSM.
   */
  std::vector<State*> _nodes;

  /*!
   @brief    The set of tasks to perform at each time step
   */
  std::vector<Task*> _tasks;

  /*!
   @brief    Mapping from goal set identifier to GoalSet.
   */
  std::map<size_t, GoalSet*> _goalSets;

  /*!
   @brief    A list of velocity modifiers to be applied to all states in the simulator.
   */
  std::vector<VelModifier*> _velModifiers;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Templated function which builds the behavior fsm from the behavior configuration given.

 @param    fsmDescrip    The Behavior configuration.
 @param    sim            The simulator which uses the BFSM.
 @param    VERBOSE        The optional argument which causes the construction process to report
                        actions as it goes.
 @returns  A pointer to the resultant FSM.  If there is an error in construction, NULL is returned.
 */
FSM* buildFSM(FSMDescrip& fsmDescrip, Agents::SimulatorInterface* sim, bool VERBOSE = false);

}  // namespace BFSM
}  // namespace Menge
#endif  // __FSM_H__
