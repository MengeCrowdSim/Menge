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
 @file		HelbingAgent.h
 @brief		The agent specification for the pedestrian model based on the Helbing et al., 2000 paper.
 */

#ifndef __HELBING_AGENT_H__
#define __HELBING_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace Helbing {
/*!
 @brief		Agent definition for the Helbing pedestrian model.
 */
class Agent : public Menge::Agents::BaseAgent {
 public:
  /*!
   @brief		A variant of the copy constructor.
   */
  Agent();

  /*!
   @brief      Destroys this agent instance.
   */
  ~Agent();

  /*!
   @brief      Computes the new velocity of this agent.
   */
  void computeNewVelocity();

  /*!
  @brief		Used by the plugin system to know what artifacts to associate with agents of this type.

  Every sub-class of must return a globally unique value if it should be associated with unique
  artifacts.
  */
  virtual std::string getStringId() const { return NAME; }

  /*! @brief	The name identifier for this agent type. */
  static const std::string NAME;

  /*!
   @brief		Compute the force due to another agent.
   @param		other			A pointer to a neighboring agent.
   @returns	The force imparted by the other agent on this agent.
   */
  Menge::Math::Vector2 agentForce(const Agent* other) const;

  /*!
   @brief		Compute the force due to a nearby obstacle.
   @param		obst			A pointer to the obstacle.
   @returns	The force imparted by the obstacle on this agent.
   */
  Menge::Math::Vector2 obstacleForce(const Menge::Agents::Obstacle* obst) const;

  /*!
   @brief		Computes the driving force for the agent.
   @returns	The vector corresponding to the agent's driving force.
   */
  Menge::Math::Vector2 drivingForce() const;

  /*!
   @brief		The mass of the agent.
   */
  float _mass;
};
}  // namespace Helbing

#endif  // __HELBING_AGENT_H__
