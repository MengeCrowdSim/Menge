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
 @file		JohanssonAgent.h
 @brief		Definition of Johansson 2007 agent.
 */

#ifndef __JOHANSSON_AGENT_H__
#define __JOHANSSON_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace Johansson {
/*!
 @brief		Definition of Johansson 2007 agent.

 A social-force agent model. This assumes that all agents have unit-weight.
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
   @brief		The directional weight - repulsive force depends on direction to agent
   */
  float _dirWeight;
};
}  // namespace Johansson
#endif  // __JOHANSSON_AGENT_H__
