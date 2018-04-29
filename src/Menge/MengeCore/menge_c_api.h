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
 @file    menge_c_api.h
 @brief   Provides a limited c-style API to access Menge's functionality.
 */

#ifndef __MENGE_C_API__
#define __MENGE_C_API__

#include "MengeCore/CoreConfig.h"
#ifndef _WIN32
#include <cstddef>
#endif

extern "C" {
/*!   @name    Simulator management
 @brief    Functions for initializing and working with the simulator.
 */
//@{

/*!
 @brief   Initializes a simulator.  The simulator uses the given pedestrian `model` type and is
          initialized using the given behavior file and scene file.

 @param    behaveFile    Path to the behavior file.
 @param    sceneFile     Path to the scene specification file.
 @param    model         Name of the model type to use.
 @param    pluginPath    Optional path to location of plugin directories. If not provided, plugins
                         will *not* be loaded.
 @returns  True if initialization was successful.
 */
MENGE_API bool InitSimulator(const char* behaveFile, const char* sceneFile, const char* model,
                             const char* pluginPath = 0x0);

/*!
 @brief    Sets the time step for the simulator.
 */
MENGE_API void SetTimeStep(float timeStep);

/*!
 @brief    Advances the state of the simulator one time step.

 @returns  True if the simulation can keep running.
 */
MENGE_API bool DoStep();

//@}

/*! @name   FSM introspection */
//@{

/*!
 @brief    Reports the name of the state with the given id.

 @param    state_id   The id of the desired state.
 @returns  A pointer to the c-string of the state's name. Nullptr if state_id does not refer to a
           valid state.
 */
MENGE_API const char* GetStateName(size_t state_id);

/*!
 @brief    Reports the number of states in the BFSM.
 */
MENGE_API size_t StateCount();

//@}

/*! @name   Agent functions
 @brief   Functions for querying the state of the simulator agents.
 */
//@{

/*!
 @brief   Reports the number of agents in the simulation.

 @returns The agent count.
 */
MENGE_API size_t AgentCount();

/*!
 @brief   Reports the 3D position of the indicated agent.

 @param[in]   i    The index of the desired agent.
 @param[out]  x    The position's x-component.
 @param[out]  y    The position's y-component.
 @param[out]  z    The position's z-component.
 @returns     True if the values were successfully set.
 */
MENGE_API bool GetAgentPosition(size_t i, float* x, float* y, float* z);

/*!
 @brief   Reports the 3D velocity of the indicated agent.

 @param[in]   i    The index of the desired agent.
 @param[out]  x    The velocity's x-component.
 @param[out]  y    The velocity's y-component.
 @param[out]  z    The velocity's z-component.
 @returns     True if the values were successfully set.
 */
MENGE_API bool GetAgentVelocity(size_t i, float* x, float* y, float* z);

/*!
 @brief   Reports the 2D preferred velocity of the indicated agent.

 @param[in]   i    The index of the desired agent.
 @param[out]  x    The preferred velocity's x-component.
 @param[out]  y    The preferred velocity's y-component.
 @returns     True if the values were successfully set.
 */
MENGE_API bool GetAgentPrefVelocity(size_t i, float* x, float* y);

/*!
 @brief   Reports the id of the state the indicated agent is currently in.

 @param[in]   i         The index of the desired agent.
 @param[out]  state_id  The id of the state the agent is currently in.
 @returns     True if the values were successfully set.
 */
MENGE_API bool GetAgentState(size_t i, size_t* state_id);

/*!
 @brief   Reports the 2D orientation of the indicated agent. It is the facing direction of the
          agent, projected onto the xz plane.

 @param[in]   i    The index of the desired agent.
 @param[out]  x    The orient's x-component.
 @param[out]  y    The orient's y-component.
 @returns     True if the values were successfully set.
 */
MENGE_API bool GetAgentOrient(size_t i, float* x, float* y);

/*!
 @brief   Reports the agent class for this particular class.

 @param   i   The index of the desired agent.
 @returns The agent's class (-1 if it can't be found).
 */
MENGE_API int GetAgentClass(size_t i);

/*!
 @brief   Reports the radius of the given agent.

 @param   i   The index of the desired agent.
 @returns     The agent's radius (negative for errors).
 */
MENGE_API float GetAgentRadius(size_t i);

//@}

/*! @name   External triggers.
 @brief   The interface for working with external triggers.

 External triggers allow code _external_ to the simulator code to change the simulator state. They
 are event triggers that are exposed to the outside world and can be fired off by arbitrary code.
 */
//@{

/*!
 @brief   Reports the number of external triggers exposed in the simulator.
 @returns The number of external triggers available.
 */
MENGE_API int ExternalTriggerCount();

/*!
 @brief   The name of the iᵗʰ external trigger. If `i` is _not_ a valid trigger index, `nullptr` is
          returned.

 @param   i   The index of the desired external trigger. Must be <= the value returned by
              ExternalTriggerCount().
 @returns The name of the iᵗʰ trigger, or null if `i` is invalid.
 */
MENGE_API const char* ExternalTriggerName(int i);

/*!
 @brief   Fires the trigger of the given name.

 If the name does not refer to a valid external trigger, nothing happens.

 @param triggerName   The name of the trigger to fire.
 */
MENGE_API void FireExternalTrigger(const char* triggerName);
//@}

/*!
 @brief   Report the total number of obstacles in the simulation.

 @returns  The total number of obstacles.
 */
MENGE_API size_t ObstacleCount();

/*!
 @brief   Given the index of one obstacle, reports the index of the next obstacle in the loop.
 
 @param   i   The index of the query obstacle.
 @returns The index of the obstacle's whose point p0 is the same as this obstacles p1.
 */
MENGE_API size_t GetNextObstacle(size_t i);

/*!
 @brief   Given the index of an obstacle, returns both endpoints, p0 and p1, that make up the
          obstacle.

 @param    i     The index of the query obstacle
 @param    x0    The x-position of end point p0, set by this function.
 @param    y0    The y-position of end point p0, set by this function.
 @param    z0    The z-position of end point p0, set by this function.
 @param    x1    The x-position of end point p1, set by this function.
 @param    y1    The y-position of end point p1, set by this function.
 @param    z1    The z-position of end point p1, set by this function.
 @returns  True if the values have been properly set.
 */
MENGE_API bool GetObstacleEndPoints(size_t i, float* x0, float* y0, float* z0, float* x1, float* y1,
                                    float* z1);

/*!
 @brief   Given the index of an obstacle, returns its first endpoint, p0.

 @param    i     The index of the query obstacle
 @param    x0    The x-position of end point p0, set by this function.
 @param    y0    The y-position of end point p0, set by this function.
 @param    z0    The z-position of end point p0, set by this function.
 @returns  True if the values have been properly set.
 */
MENGE_API bool GetObstacleP0(size_t i, float* x0, float* y0, float* z0);

/*!
 @brief   Given the index of an obstacle, returns its second endpoints, p1.

 @param    i     The index of the query obstacle
 @param    x1    The x-position of end point p1, set by this function.
 @param    y1    The y-position of end point p1, set by this function.
 @param    z1    The z-position of end point p1, set by this function.
 @returns  True if the values have been properly set.
 */
MENGE_API bool GetObstacleP1(size_t i, float* x1, float* y1, float* z1);
}

#endif  // __MENGE_C_API__
