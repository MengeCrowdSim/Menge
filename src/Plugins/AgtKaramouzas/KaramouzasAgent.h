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
 @file       KaramouzasAgent.h
 @brief      Contains the Karamouzas::Agent class.

 Implements Karamouzas's 2009 pedestrian model "A Predictive Collision Avoidance Model for
            Pedestrian Simulation"
 */

#ifndef __KARAMOUZAS_AGENT_H__
#define __KARAMOUZAS_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace Karamouzas {
/*!
 @brief		The agent definition for the Karamouzas 2009 agent.
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
   @brief		The personal space (in meters) of the agent
   */
  float _perSpace;

  /*!
   @brief		The anticipation time (in seconds) of the agent
   */
  float _anticipation;
};
}  // namespace Karamouzas
#endif  // __KARAMOUZAS_AGENT_H__
