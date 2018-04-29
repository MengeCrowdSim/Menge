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
 @file		GCFAgent.h
 @brief		The agent specification for the pedestrian model based on the Chraibi et al., 2010 paper.
 */

#ifndef __GCF_AGENT_H__
#define __GCF_AGENT_H__

#include "Ellipse.h"
#include "MengeCore/Agents/BaseAgent.h"

using namespace Menge;

namespace GCF {
/*!
 @brief		Agent definition for the generalized centrifugal force pedestrian model.
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
   @brief		Initializes the agent

   Subclasses should call their parent's implementation of initialize.
   */
  virtual void initialize();

  /*!
   @brief		Method for sub-classes to perform additional update work

   This is the last thing called by the update method.  When this is called, position, velocity, and
   orientation will be updated in the base agent class.
   */
  virtual void postUpdate();

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
   @brief			Provides the various parameters of the pedestrian repulsive force between this agent
              and the given agent.

   Helper for visualization.

   @param		agent		  The agent to compute the various repulsion parameters for.
   @param		effDist		The "effective" distance between the closest points on the agent boundaries.
   @param		forceDir	The direction of the force.
   @param		K_ij		  The field of view parameter between agents.
   @param		response	The force strength based on effective distance.
   @param		velScale	The scale on the force magnitude based on the relative velocity.
   @param		magnitude	The overall magnitude of the repulsive force.
   @returns	An int indicating the computation success:
      0 = repulsive force exists.
      1 = agent too far away.
      2 = agent out of field of view
   */
  int getRepulsionParameters(const Agent* agent, float& effDist, Menge::Math::Vector2& forceDir,
                             float& K_ij, float& response, float& velScale, float& magnitude) const;

  /*!
   @brief		Compute the force due to a nearby obstacle.
   @param		obst			A pointer to the obstacle.
   @returns	The force imparted by the obstacle on this agent.
   */
  Menge::Math::Vector2 obstacleForce(const Agents::Obstacle* obst) const;

  /*!
  @brief		Computes component of the force magnitude based on effective distance..
  @param		effDist			The effective distance from ellipse center to force source.
  */
  float computeDistanceResponse(float effDist) const;

  /*!
   @brief		Computes the driving force for the agent.
   @returns	The vector corresponding to the agent's driving force.
   */
  Menge::Math::Vector2 driveForce() const;

  /*!
   @brief		Updates the ellipse to the current state.
   */
  void updateEllipse();

  /*!
   @brief		The ellipse representing the agent's volume.
   */
  Ellipse _ellipse;

  /*!
   @brief		The minimum value for the radius along the agent's facing direction.
   */
  float _aMin;

  /*!
   @brief		The rate of change of the value for the radius along the agent's facing direction.
   */
  float _aRate;

  /*!
   @brief		The maximum value for the radius along the agent's perpendicular direction.
   */
  float _bMax;

  /*!
   @brief		The rate of change of the value for the radius along the agent's perpendicular 
            direction.
   */
  float _bGrowth;
};
}  // namespace GCF

#endif
