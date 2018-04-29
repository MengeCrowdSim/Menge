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
 @file    AgentGenerator.h
 @brief   The definition of the agent generator element. Defines the intial numbers and positions
          of agents in the simulation.
 */
#ifndef __AGENT_GENERATOR_H__
#define __AGENT_GENERATOR_H__

#include "MengeCore/PluginEngine/Element.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {
// forward declaration
class BaseAgent;

/*!
 @brief    Exception class for agent generator computation.
 */
class MENGE_API AgentGeneratorException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  AgentGeneratorException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AgentGeneratorException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal agent generator exception.
 */
class MENGE_API AgentGeneratorFatalException : public AgentGeneratorException,
                                               public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  AgentGeneratorFatalException()
      : MengeException(), AgentGeneratorException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  AgentGeneratorFatalException(const std::string& s)
      : MengeException(s), AgentGeneratorException(), MengeFatalException() {}
};

/*!
 @brief    The base class inital agent generation.

 This is an abstract class, primarily defining the agent generation abstraction. Essentially,
 the AgentGenerator produces a set of agent positions.  Its properties should be sufficient to
 produce a count of agents with defined positions.

 The base class provides the definition (parsing and utilization) of a noise parameter which can
 be applied on top of the sub-class's implementation. The direction of perturbation is uniformly
 distributed on the plane; the user defines the magnitude of the perturbation. To make use of
 the spatial noise, the AgentGenerator must be instantiated in the scene specification file as
 in the following example:

 ```xml
 <Generator type="TYPE_NAME" ...
  displace_dist="u" displace_min="0.0" displace_max="0.5"
 />
 ```

 The noise is a standard Menge distribution with a "displace_" prefix.  In this example, a 
 uniform noise is applied to the computed position.  (The ellipses take the place of the 
 type-specific parameters for the TYPE_NAME generator class.)
 */
class MENGE_API AgentGenerator : public Element {
 public:
  /*!
   @brief    Constructor
   */
  AgentGenerator();

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~AgentGenerator();

 public:
  /*!
   @brief    Reports the number of agents created.

   @returns  The number of agents this generator creates.
   */
  virtual size_t agentCount() = 0;

  /*!
   @brief    Sets the ith position to the given agent.

   @param    i      The index of the requested position in the sequence.
   @param    agt    A pointer to the agent whose position is to be set.
   @throws   AgentGeneratorException if the index, i, is invalid.
   */
  virtual void setAgentPosition(size_t i, BaseAgent* agt) = 0;

  /*!
   @brief    Sets the Generators noise generation.

   The AgentGenerator takse position of the float generator provided and will delete it up on
   its destruction.

   @param    gen    The generator.
   */
  void setNoiseGenerator(Math::FloatGenerator* gen);

  /*!
   @brief    Perturbs the given point according to the given noise generator.

   @param    pos    The position to perturb.
   @returns  The perturbed point.
   */
  Math::Vector2 addNoise(const Math::Vector2& pos);

 protected:
  /*!
   @brief    The generator for displacement amount (defaults to zero).
   */
  Math::FloatGenerator* _disp;

  /*!
   @brief    The generator for angular displacement.
   */
  Math::FloatGenerator* _dir;
};

}  // namespace Agents
}  // namespace Menge
#endif  // __AGENT_GENERATOR_H__
