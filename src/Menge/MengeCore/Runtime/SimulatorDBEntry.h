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
 @file  SimulatorDBEntry.h
 @brief  Definition of an entry into the simulator database.

 This provides the mechanism by which the executable can learn about new pedestrian
 models/simulators without any significant code contortions.
 */

#ifndef __SIMULATOR_DB_ENTRY_H__
#define __SIMULATOR_DB_ENTRY_H__

#include "MengeCore/CoreConfig.h"

#include <iostream>
#include <string>

namespace Menge {

class SimSystem;
class BaseAgentContext;
namespace Agents {
class AgentInitializer;
class Integrator;
class SimulatorInterface;
}  // namespace Agents
namespace SceneGraph {
class GLScene;
}
namespace BFSM {
class FSMDescrip;
class FSM;
}  // namespace BFSM

/*!
 @brief    An entry in the simulator database.

 Every pedestrian model must define and register a SimulatorDBEntry. The entry provides brief and
 long descriptions of the pedestrian model to display in response to command-line queries.
 Furthermore, they are responsible for instantiating simulators, behavior FSM, and SimSystem
 (although, this is done in the base class and <i>not</i> the derived classes.

 A derived class should do the following:
 - Inherit from the SimulatorDBEntry
 - Implement the following functions:
 - SimulatorDBEntry::briefDescription
 - SimulatorDBEntry::longDescription
 - SimulatorDBEntry::viewName
 - SimulatorDBEntry::commandLineName
 - SimulatorDBEntry::getNewSimulator
 - Optionally implement the following functionsl
 - SimulatorDBEntry::contextFromSystem if the pedestrian model provides a unique sub-class of
   Agents::BaseAgentContext.
 - SimulatorDBEntry::getAgentInitalizer if the pedestrian model has custom per-agent attributes to
   parse (in addition to the common parameters).
 */
class MENGE_API SimulatorDBEntry {
 public:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~SimulatorDBEntry() {}

  /*!
   @brief    Gives a brief description of the simulator.

   @returns  A brief description of the simulator and pedestrian model.
   */
  virtual ::std::string briefDescription() const = 0;

  /*!
   @brief    Gives a long description of the simulator.

   @returns  A long description of the simulator and pedestrian model.
   */
  virtual ::std::string longDescription() const = 0;

  /*!
   @brief    Gives a label to apply to the interactive viewer.

   @returns  The name for display on the interactive viewer.
   @returns  A newly instantiated simulator instance of a type appropriate for this database entry.
   */
  virtual ::std::string viewerName() const = 0;

  /*!
   @brief    Gives a unique name to be used as a command-line parameter.

   This name MUST satisfy two constraints:
   - It must contain no spaces.
   - It must be unique from that used by all other simulators.

   @returns  A single string (with no spaces) that can be used as a command line parameter to
            uniquely identify this model.
   */
  virtual ::std::string commandLineName() const = 0;

  /*!
   @brief    Returns a pointer to this model's Simulator instance.

   This must be overridden by a derived class

   @returns  A newly instantiated simulator instance of a type appropriate for this database entry.
   */
  virtual Agents::SimulatorInterface* getNewSimulator() = 0;

  /*!
   @brief    Returns a new simulator.

   @param    agentCount    The number of the agents in the system.
   @param    simTimeStep    The simulator's time step (for updating the sim system).
   @param    subSteps      The number of computation sub-steps to take.
   @param    simDuration    The maximum duration to allow the simulation to run.
   @param    behaveFile    The full path to the xml <i>behavior</i> specification.
   @param    sceneFile      The full path to the xml <i>scene</i> specification.
   @param    outFile        The full path to the output file to write the agent trajectories. If the
                          empty string, no output file will be written.
   @param    scbVersion    The scb version to write.
   @param    verbose        Determines if the initialization process prints status
   @returns  A pointer to the resultant SimulatorInterface. If there is an error, NULL is returned.
   */
  Agents::SimulatorInterface* getSimulator(size_t& agentCount, float& simTimeStep, size_t subSteps,
                                           float simDuration, const std::string& behaveFile,
                                           const std::string& sceneFile, const std::string& outFile,
                                           const std::string& scbVersion, bool verbose);

  /*!
   @brief    Reports the current run-time of an instantiated simulation.

   This is only meaningful if called *after* getSimulatorSystem.

   @returns  The current run-time of the instantiated simulation. If no simulation has been
            instantiated, -1 is returned.
   */
  float simDuration() const;

 protected:
#if 0
    /*!
     @brief    Returns a pointer to an agent context appropriate to the corresponding simulator.

     If the provided system is not, in fact, a pointer to a SimSystem for the appropriate simulator
     type, this function will report failure. Furthermore, the default implementation is to return a
     BaseAgentContext. If the simulator comes with a novel context, this function should be
     overridden in the derived SimulatorDBEntry.

     @param    simSystem    The system which tracks the agents. This should be the same system which
                          was returned by a call to SimulatorDBEntry::getSimulatorSystem.
     @returns  A pointer to the appropriate agent context. If the system is of the wrong type (or if
              there is any other problem), NULL is returned.
     */
    virtual BaseAgentContext * contextFromSystem( SimSystem * simSystem );
#endif
  /*!
   @brief    Provides an AgentInitializer appropriate to this simulator class.

   Each derived database entry must provide the appropriate AgentInitializer for its simulator type,
   in order to fully process all of the corresponding XML data to initialize the agent population.

   @returns  A pointer to an agent initializer. The caller is responsible for freeing up the memory.
   */
  virtual Agents::AgentInitializer* getAgentInitalizer() const;

  /*!
   @brief    Creates the simulator.

   @param    sceneFileName    The full path to the simulation scene specification.
   @param    VERBOSE          Determines if the initialization outputs status and information to the
                            console. True outputs, false remains silent.
   @returns  A pointer to the instantiated simulator. If there is an error, NULL is returned.
   */
  Agents::SimulatorInterface* initSimulator(const std::string& sceneFileName, bool VERBOSE);

  /*!
   @brief    Creates the finite state machine and finalizes simulator and fsm

   @param    behaveFile    String containing the full path to the behavior file.
   @param    sim            Pointer to the simulator interface to be used in conjunction with the FSM.
   @param    VERBOSE        Flag for verbose output.
   @returns    A pointer to the instantiated finite state machine for the simulator. If there is an
              error, NULL is returned.
   */
  BFSM::FSM* initFSM(const std::string& behaveFile, Agents::SimulatorInterface* sim, bool VERBOSE);

  /*!
   @brief    Finalizes the finite state machine and simulator in preparation for execution.

   @param    sim    A pointer to the simulator.
   @param    fsm    The finite state machine that goes with the simulator.
   @returns  A boolean reporting if finalization was successful (true) or not (false).
   */
  bool finalize(Agents::SimulatorInterface* sim, BFSM::FSM* fsm);

  /*!
   @brief    A pointer to the simulator.
   
   The database entry is not responsible for deleting it unless there is an error in initialization.
   */
  Agents::SimulatorInterface* _sim;

  /*!
   @brief    A pointer to the behavior finite state machine.
   
   The database entry is *not* responsible for deleting it unless there is an error in
   initialization.
   */
  BFSM::FSM* _fsm;
};
}  // namespace Menge

#endif  // __SIMULATOR_DB_ENTRY_H__
