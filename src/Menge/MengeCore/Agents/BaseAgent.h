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

#ifndef __BASE_AGENT_H__
#define __BASE_AGENT_H__

/*!
 @file       BaseAgent.h
 @brief      Contains the BaseAgent class - the underlying class which defines the basic
 functionality for all shared agents.
 */

// UTILS
#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/Agents/SpatialQueries/ProximityQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryStructs.h"
#include "MengeCore/Agents/XMLSimulatorBase.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/mengeCommon.h"

#include <list>
#include <vector>

namespace Menge {

namespace Agents {

class Obstacle;

/*!
 @brief    Exception for BaseAgent problems.
 */
class MENGE_API AgentException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  AgentException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AgentException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal agent exception.
 */
class MENGE_API AgentFatalException : public AgentException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  AgentFatalException() : MengeException(), AgentException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AgentFatalException(const std::string& s)
      : MengeException(s), AgentException(), MengeFatalException() {}
};

/*!
 @brief    Special agent exception - used for non-implemented functionality.
 */
class MENGE_API AgentImplementationException : public AgentFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  AgentImplementationException() : AgentFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AgentImplementationException(const std::string& s) : AgentFatalException(s) {}
};

/*!
 @brief      Defines the basic agent properties and functionality that all simulation agents share.
 */
class MENGE_API BaseAgent : public ProximityQuery {
 public:
  /*!
   @brief    Default constructor.
   */
  BaseAgent();

  /*!
   @brief    Initializes the agent

   Subclasses should call their parent's implementation of initialize.
   */
  virtual void initialize();

  /*!
   @brief      Updates the two-dimensional position and two-dimensional velocity of this agent.

   @param    timeStep    The time step that will be taken.
   */
  void update(float timeStep);

  /*!
   @brief    Updates the orientation.

   This is guaranteed to be called after the current velocity and position have been integrated.

   @param    timeStep    The duration of the simulation time step.
   */
  virtual void updateOrient(float timeStep);

  /*!
   @brief    Method for sub-classes to perform additional update work

   This is the last thing called by the update method.  When this is called, position, velocity,
   and orientation will be updated in the base agent class.
   */
  virtual void postUpdate() {}

  /*!
   @brief    Given preferred velocity and neighboring agents and obstacles compute a new velocity.

   This should be overriden by child classes to give unique behaviors. Each pedestrian model is
   uniquely defined by how it computes its new velocity and this is the critical class.

   Trying to instantiate a BaseAgent will cause an exception to be thrown when calling this
   function.
   */
  void computeNewVelocity();

  /*!
   @brief      Returns a pointer to the neighbor with given index

   @param      idx    The index of the desired agent.  This index is *not* validated.
   @returns    Pointer to the neighboring agent.
   */
  const BaseAgent* getNeighbor(int idx) const { return _nearAgents[idx].agent; }

  /*!
   @brief      Returns a pointer to the obstacle with given index

   @param      idx    The index of the desired obstacle.  This index is *not* validated.
   @returns    Pointer to the nearby obstacle.
   */
  const Obstacle* getObstacle(int idx) const { return _nearObstacles[idx].obstacle; }

  /*!
   @brief      set the agents preferred velocity to the input velocity.

   @param      velocity to be applied to the agent.
   */
  void setPreferredVelocity(PrefVelocity& velocity);

  /*!
   @brief    Add an velocity modifier to the agent

   @param    v    The modifier to add
   */
  void addVelModifier(BFSM::VelModifier* v);

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const = 0;

  // Properties of a basic agent
  /*!
   @brief    The maximum speed the agent can take.
   */
  float _maxSpeed;

  /*!
   @brief    The maximum acceleration the agent can experience (interpreted isotropically).
   */
  float _maxAccel;

  /*!
   @brief    The preferred speed of the agent
   */
  float _prefSpeed;

  /*!
   @brief    The current 2D position of the agent
   */
  Math::Vector2 _pos;

  /*!
   @brief    The current 2D velocity of the agent
   */
  Math::Vector2 _vel;

  /*!
   @brief    The 2D preferred velocity of the agent
   */
  PrefVelocity _velPref;

  /*!
   @brief    The new velocity computed in computeNewVelocity.

   This exists to allow the agents to be updated in parallel while preserving order-of-evaluation
   independence.
   */
  Math::Vector2 _velNew;

  /*!
   @brief    The orientation vector (the direction the agent is facing which is not necessarily the
   same direction as the instantaneous velocity.

   Not all pedestrian models require orientation in their calculation of a new velocity. However,
   by introducing the property here, we accomplish two things:
     - All agents which *do* require orientation can update their orientation in a common
       mechanism, making comparisons them less dependent on differences in orientation computation
       and focusing on differences in the new velocity computation.
     - Second, it provides orientation information to the output trajectories for later
       visualization.
   */
  Math::Vector2 _orient;

  /*!
   @brief    The agent's maximum angular velocity (in radians/sec) -- used for controlling the
            changes in agent orientation.
   */
  float _maxAngVel;

  /*!
   @brief    The number of nearby agents used to plan dynamic respones.
   */
  size_t _maxNeighbors;

  /*!
   @brief    The maximum distance at which another agent will be considered for a response.
   */
  float _neighborDist;

  /*!
   @brief    The population class for this agent.

   Used to define behavior and visualization properties.
   */
  size_t _class;

  /*!
   @brief    A mask indicating the obstacles with compatible ids which this agent can see.

   This is a bitwise mask such that if the ith bit is 1, obstacles with id 2^i are visible.
   */
  size_t _obstacleSet;

  /*!
   @brief    The priority of each agent.

   The relative priority of agents determines aspects of their interaction behavior.
   */
  float _priority;

  /*!
   @brief    A globally unique identifier for each agent.
   */
  size_t _id;

  /*!
   @brief    The agent's radius.

   If the agent is represented as a circle, then this is simply the circle's radius. If the agent
   is represented as an ellipse, then this is the radius perpendicular to the orientation. Other
   geometries should provide their own interpretation.

   At a minimum, it is used to determine sideways clearance.
   */
  float _radius;

  /*!
   @brief  a set of velocity modifiers to be set with the agent. Allows for intermediate velocity
   changes

   */
  std::vector<BFSM::VelModifier*> _velModifiers;

  /*!
   @brief    The nearby agents to which the agent should respond.

   Each pair consists of distance between the agent positions, squared and the pointer to the
   neigboring agent.
   */
  std::vector<NearAgent> _nearAgents;

  /*!
   @brief    The nearby obstacles to which the agent should respond.

   Each pair consists of distance between agent position and wall, squared and the pointer to the
   wall.
   */
  std::vector<NearObstacle> _nearObstacles;

  /*!
   @brief      Inserts an agent neighbor into the set of neighbors of this agent.

   @param      agent          A pointer to the agent to be inserted.
   @param      distSq         The distance to the indicated agent
   */
  void insertAgentNeighbor(const BaseAgent* agent, float distSq);

  /*!
   @brief      Inserts a static obstacle neighbor into the set of neighbors of this agent.

   @param      obstacle       A pointer to the obstacle to be inserted
   @param      distSq         The distance to the indicated obstacle
   */
  void insertObstacleNeighbor(const Obstacle* obstacle, float distSq);

  // TODO: Ultimately, this should go into an intention filter and not the agent itself
  /*!
   @brief    Sets the density sensitivity parameters.

   @param    stride    The stride factor.  The physical component capturing height and the physical
                      relationship between speed and stride length.
   @param    buffer    The stride buffer.  The psychological buffer required beyond that needed for
                      stride length.
   */
  virtual void setStrideParameters(float stride, float buffer) {}

  // Methods needed for a spatial query filter to work

  /*!
   @brief     Clears the result vectors. Resets the filter
   */
  virtual void startQuery();

  /*!
   @brief      Filters an agent and determines if it needs to be in the near set.

   @param      agent    The agent to consider.
   @param      distance  The distance to the agent.
   */
  virtual void filterAgent(const BaseAgent* agent, float distance);

  /*!
   @brief      Filters an obstacle and determines if it needs to be in the near set.

   @param      obstacle  The obstacle to consider.
   @param      distance  The distance to the obstacle.
   */
  virtual void filterObstacle(const Obstacle* obstacle, float distance);

  /*!
   @brief      Gets the start point for the query.

   @returns    The query point for this filter.
   */
  virtual Math::Vector2 getQueryPoint() { return _pos; };

  /*!
   @brief     Updates the max agent query range if conditions inside the filter are met. Typically,
              we don't shrink the query range until the result set is full.

   @returns  The Max query range. Typically this is the initial range unless some special conditions
            are met.
   */
  virtual float getMaxAgentRange();

  /*!
   @brief     Updates the max query obstacle range if conditions inside the filter are met.
              Typically, we don't shrink the query range until the result set is full.

   @returns  The Max query range. Typically this is the initial range unless some special conditions
            are met.
   */
  virtual float getMaxObstacleRange() { return _neighborDist * _neighborDist; };
};

}  // namespace Agents
}  // namespace Menge
#endif  // __BASE_AGENT_H__
