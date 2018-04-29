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
 @file		DummyAgent.h
 @brief		The definition of a simple "dummy" agent.
 */

#ifndef __DUMMY_AGENT_H__
#define __DUMMY_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace Dummy {
/*!
 @brief		The "dummy" agent class.
 *
 The dummy agent does nothing clever or even correct. Given the preferred velocity, its final
 velocity is simply a random perturbation of the preferred velocity.
 */
class Agent : public Menge::Agents::BaseAgent {
 public:
  /*!
   @brief		Constructor
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
};
}  // namespace Dummy

#endif  // __DUMMY_AGENT_H__
