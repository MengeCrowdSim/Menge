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
 @file    FSMDescrip.h
 @brief    The definition of behavior finite state machines.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "MengeCore/BFSM/StateDescrip.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeCore/Runtime/os.h"
#include "MengeCore/resources/ResourceManager.h"
#include "MengeCore/resources/VectorField.h"

#include "thirdParty/tinyxml.h"

#include <list>
#include <map>
#include <string>

namespace Menge {

// forward declarations
namespace Agents {
class SimulatorInterface;
}

namespace BFSM {
// Forward declaration
class FSM;
class GoalSet;
class Task;
class State;
class Transition;
class StateDescrip;
class VelModifier;

/*!
 @brief    The full description of the behavioral finite state machine
 */
class FSMDescrip {
 public:
  /*!
   @brief    Default constructor.
   */
  FSMDescrip();

  /*!
   @brief    Destructor.
   */
  ~FSMDescrip();

  /*!
   @brief    Instantiates a state based on the description.

   This is a behavior operation, because the behavior is responsible for:
   - tracking its states by name,
   - confirming no State name conflicts, and
   - connecting states with transitions.

   @param    sData    The description of the state to create.
   @returns  A pointer to the created state.  If there are two states with the same name *in this
   behavior*, NULL is returned.
   */
  State* addState(StateDescrip* sData);

  /*!
   @brief    Initializes the configuration from an xml file.

   @param    xmlName    The path to the xml file containing the behavior specification.
   @param    verbose    Dictates whether the parsing is verbose.
   @returns  True if parsing was successful.
   */
  bool loadFromXML(const std::string& xmlName, bool verbose);

  /*!
   @brief    Creates an FSM instance from the config file.

   @param    fsmDescrip  The behavior configuration description.
   @param    sim          An instance of the simulator.
   @param    VERBOSE      Dictates whether the construction process is verbose (true) or not (false).
   */
  friend FSM* buildFSM(FSMDescrip& fsmDescrip, Agents::SimulatorInterface* sim, bool VERBOSE);

  /*!
   @brief    Friend operator for printing the behavior configuration to an output stream.

   @param    out            The logger.
   @param    fsmDescrip    A behavior configuration to write to the stream.
   @returns  The provided output stream.
   */
  friend Logger& operator<<(Logger& out, const FSMDescrip& fsmDescrip);

 protected:
  /*!
   @brief    Adds the transition(s) associated with the given name.

   @param    name    The name of the state from which the transition originates. Alternatively, a
                    comma-separated list.
   @param    trans    The transition to add.  The FSMDescrip takes ownership of the transition.
   */
  void addTransition(const std::string& name, Transition* trans);

  /*!
   @brief    List of state descriptions.
   */
  std::list<StateDescrip*> _states;

  /*!
   @brief    List of transitions.
   */
  std::map<std::string, std::list<Transition*> > _transitions;

  /*!
   @brief    Map of state names to states.
   */
  std::map<std::string, State*> _stateNameMap;

  /*!
   @brief    A map from agent class id to a behavior description instance.
   */
  std::list<Task*> _tasks;

  /*!
   @brief    A mapping of goal sets to goals.

   The goal sets are represented by their id.  The goals for each goal set are stored in a map,
   mapping the goal's local id to a descriptor for that goal.  This is to facilitate set additions.
   */
  std::map<size_t, GoalSet*> _goalSets;

  /*!
   @brief    A list of velocity modifiers to be applied to all states in the simulator.
   */
  std::vector<VelModifier*> _velModifiers;

  /*!
   @brief    The folder in which the behavior specification file appears
   */
  std::string _behaviorFldr;
};
}  // namespace BFSM
}  // namespace Menge

#endif  // __CONFIG_H__
