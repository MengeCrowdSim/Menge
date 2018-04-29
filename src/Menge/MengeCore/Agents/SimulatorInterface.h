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
 @file    SimulatorInterface.h
 @brief    The definition of the interface of the simulator required by the finite state machine.
 */
#ifndef __SIMULATOR_INTERFACE_H__
#define __SIMULATOR_INTERFACE_H__

#include "MengeCore/Agents/XMLSimulatorBase.h"
#include "MengeCore/Core.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"

#include <vector>

namespace Menge {

// Forward declarations
namespace BFSM {
class FSM;
}

namespace Agents {
// forward declaration
class BaseAgent;
class Elevation;
class Obstacle;
class SCBWriter;
class SpatialQuery;

/*!
 @brief    The basic simulator interface required by the fsm.
 */
class MENGE_API SimulatorInterface : public XMLSimulatorBase {
 public:
  /*!
   @brief    Default constructor.
   */
  SimulatorInterface();

  /*!
   @brief    Destructor
   */
  virtual ~SimulatorInterface();

  /*!
   @brief    Sets the BFSM for the simulator.

   @param    fsm    The BFSM for this simulation.
   */
  void setBFSM(BFSM::FSM* fsm);

  /*!
   @brief    Get a pointer to the simulator's BFSM.

   @returns  The simulator's BFSM.
   */
  BFSM::FSM* getBFSM() { return _fsm; }

  /*!
   @brief    Advances the simulator state the logical time step.

   @returns    True if the simulator can advance further (e.g., agents not in final state and not
              reached maximum simulation time.)
   */
  bool step();

  /*!
   @brief      Returns the count of agents in the simulation.

   @returns    The count of agents in the simulation.
   */
  virtual size_t getNumAgents() const = 0;

  /*!
   @brief      Accessor for agents.

   @param      agentNo    The number of the agent who is to be retrieved. This is *not* the same as
                          the agent identifier. It is merely the local index of the agent in the
                          simulator's local store.
   @returns    A pointer to the agent.
   */
  virtual BaseAgent* getAgent(size_t agentNo) = 0;

  /*!
   @brief      Const accessor for agents.

   @param      agentNo    The number of the agent who is to be retrieved. This is *not* the same as
                          the agent identifier. It is merely the local index of the agent in the
                          simulator's local store.
   @returns    A pointer to the agent.
   */
  virtual const BaseAgent* getAgent(size_t agentNo) const = 0;

  /*!
   @brief    After all agents and all obstacles have been added to the scene does the work to finish
            preparing the simulation to be run.

   This work is performed when the simulator is done being initialized. If a particular new
   pedestrian simulator requires particular finalization work, this function should be sub-classed
   and the parent class's version of the function should be explicitly called before any additional
   work is performed.
   */
  virtual void finalize();

  /*!
   @brief      Returns the elevation of the given agent.

   @param      agent    The agent.
   @returns    The elevation for the given agent.
   */
  float getElevation(const BaseAgent* agent) const;

  /*!
   @brief      Returns the elevation of the x-z position.

   @param      point    The x-z point.
   @returns    The elevation at the given point.
   */
  float getElevation(const Math::Vector2& point) const;

  /*!
   @brief    Set the elevation instance of the simulator

   @param    elevation    The elevation object.
   */
  void setElevationInstance(Elevation* elevation);

  /*!
   @brief    Set the elevation instance of the simulator

   @returns  The elevation instance.
   */
  Elevation* getElevationInstance() { return _elevation; }

  /*!
   @brief    Reports if the elevation has been set.

   @returns  True if the elevation has been set, false otherwise.
   */
  bool hasElevation() const { return _elevation != 0x0; }

  /*!
   @brief    Sets the spatial query instance of the simulator.

   @param    spatialQuery    The spatial query object.
   */
  void setSpatialQuery(SpatialQuery* spatialQuery);

  /*!
   @brief    Get the spatial query instance of the simulator.

   @returns    A pointer to The spatial query object.
   */
  SpatialQuery* getSpatialQuery() { return _spatialQuery; };

  /*!
   @brief    Get a const reference to the spatial query instance of the simulator.

   @returns    A const reference to the spatial query object.
   */
  const SpatialQuery& getSpatialQuery() const { return *_spatialQuery; }

  /*!
   @brief    Reports if the spatial query has been set.

   @returns  True if the elevation has been set, false otherwise.
   */
  bool hasSpatialQuery() const { return _spatialQuery != 0x0; }

  /*!
   @brief      Returns the global time of the simulation.

   @returns    The present global time of the simulation (zero initially).
   */
  inline float getGlobalTime() const { return _globalTime; }

  /*!
   @brief      Sets the time step of the simulation.

   @param      timeStep        The time step of the simulation. Must be positive.
   */
  inline void setTimeStep(float timeStep) {
    LOGICAL_TIME_STEP = timeStep;
    updateEffTimeStep();
  }

  /*!
   @brief      Sets the number of intermediate computation sub steps to take.

   For the given sim time step, this number of sub steps will be taken. This decreases the effective
   time step, but the simulation state to the outside world is only reported at the simulation's
   *official* time step rate.

   @param      subSteps      The number of sub steps to take.
   */
  inline void setSubSteps(size_t subSteps) {
    SUB_STEPS = subSteps;
    updateEffTimeStep();
  }

  /*!
   @brief      Returns the logical time step of the simulation.

   @returns    The present time step of the simulation.
   */
  inline float getTimeStep() const { return LOGICAL_TIME_STEP; }

  /*!
   @brief    Reports the number of simulation substeps to take.

   @returns  The number of substeps to take.
   */
  inline size_t getSubSteps() const { return SUB_STEPS; }

  /*!
   @brief    Sets the maximum length allowed for the simulation to run.

   @param    duration    The maximum duration of the simulation (in simulation time). After this time
                        has elapsed, the system no longer updates.
   */
  inline void setMaxDuration(float duration) { _maxDuration = duration; }

  /*!
   @brief    Sets the trajectory output state.

   @param    outFileName        The path to the file to write trajectories to.
   @param    scbVersion        The version of scb file to write.
   @returns  True if the SCB writer has been successfully configured.
   */
  bool setOutput(const std::string& outFileName, const std::string& scbVersion);

 protected:
  /*!
   @brief       Lets the simulator perform a simulation step and updates the two-dimensional _p and
                two-dimensional velocity of each agent.
   */
  virtual void doStep() = 0;

  /*!
   @brief    Updates the effective time step -- how large an actual simulation time step is due to
            computation sub-steps.

   */
  void updateEffTimeStep() { SIM_TIME_STEP = TIME_STEP = LOGICAL_TIME_STEP / (1.f + SUB_STEPS); }

  /*!
   @brief    The logical simulation time step.
   
   The simulation's state is communicated to the outside world at this time step. In practice,
   sub-steps can decrease the effective time step.
   */
  static float LOGICAL_TIME_STEP;

  /*!
   @brief    The effective simulation time step - takes into account time step and computation
            sub-steps.
   */
  static float TIME_STEP;

  /*!
   @brief    The number of intermediate steps taken between subsequent simulation time steps.
   */
  static size_t SUB_STEPS;

  /*!
   @brief    The total accumulated simulation time.
   */
  float _globalTime;

  /*!
   @brief    Data structure for reporting the elevation data of agents.

   This allows the simulation to be more than 2D (and more than topologically planar.)
   */
  Elevation* _elevation;

  /*!
   @brief    The data structure used to perform spatial queries.
   */
  SpatialQuery* _spatialQuery;

  /*!
   @brief    The behavior finite state machine for the simulator.
   */
  BFSM::FSM* _fsm;

  /*!
   @brief    The optional scb writer (if an output file has been successfully specified.
   */
  SCBWriter* _scbWriter;

  /*!
   @brief    Indicates if the simulation is running.
   */
  bool _isRunning;

  /*!
   @brief    Maximum length of simulation time to compute (in simulation time).
   */
  float _maxDuration;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __SIMULATOR_INTERFACE_H__
