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
 @file       ZanlungoAgent.h
 @brief      Contains the Zanlungo::Agent class.

 Implements Zanlungo's 2011 pedestrian model "Social Force Model with Explicit Collision
 Prediction".
 */

#ifndef __ZANLUNGO_AGENT_H__
#define __ZANLUNGO_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace Zanlungo {
/*!
 @brief		Agent definition for the Zanlungo pedestrian model.
 */
class Agent : public Menge::Agents::BaseAgent {
 public:
  /*!
   @brief		Default constructor.
   */
  Agent();

  /*!
   @brief      Destroys this agent instance.
   */
  ~Agent();

  /*!
   @brief		Computes the time to interaction.
   @param		tti		The time to interaction. Call by reference means that this value is set to the
                  time to interaction.
   @returns	True if there is interaction.
   */
  bool computeTTI(float& tti) const;

  /*!
   @brief		Compute the force due to another agent.
   @param		other			A pointer to a neighboring agent.
   @param		T_i				The time to interaction.
   @returns	The force imparted by the other agent on this agent.
   */
  Menge::Math::Vector2 agentForce(const Agent* other, float T_i) const;

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
   @brief		Reports the field of view for the agent.
   */
  inline float getFOV() const { return 3.14159265f; }  // 180 degrees

  /*!
   @brief			Computes the velocities based on right of way

   @param			otherVel		  The other agent's current velocity. The right-of-way-relative other
                            velocity will be set in this variable.
   @param			otherPrefVel	The other agent's preferred velocity.
   @param			otherPriority	The other agent's priority value
   @param			vel				    This agent's right-of-way-relative velocity will be placed in this
                            variable.
   @returns		The signed right of way value. (Negative implies other has right of way, positive
              implies this agent has right of way.)
   */
  float rightOfWayVel(Menge::Math::Vector2& otherVel, const Menge::Math::Vector2& otherPrefVel,
                      float otherPriority, Menge::Math::Vector2& vel) const;

  /*!
   @brief		The mass of the agent
   */
  float _mass;
};
}  // namespace Zanlungo

#endif  // __ZANLUNGO_AGENT_H__
