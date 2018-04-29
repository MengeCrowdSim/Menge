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
 @file    ExplicitAgentGenerator.h
 @brief    An agent generator which creates a set of agents based on an explicit enumeration of
          agent positions.
 */

#ifndef __EXPLICIT_AGENT_GENERATOR_H__
#define __EXPLICIT_AGENT_GENERATOR_H__

#include "MengeCore/Agents/AgentGenerators/AgentGenerator.h"
#include "MengeCore/Agents/AgentGenerators/AgentGeneratorFactory.h"
#include "MengeCore/mengeCommon.h"

#include <vector>

namespace Menge {

namespace Agents {
/*!
 @brief    Definition of agent generator class which produces agents based on explicit enumeration
          of agent positions in an XML file.

 To specify an explicit agent generator, use the following syntax:

 ```xml
 <Generator type="explicit">
   <Agent p_x="float" p_y="float"/>
   <Agent p_x="float" p_y="float"/>
   ...
   <Agent p_x="float" p_y="float"/>
 </Generator>
 ```

 The values `p_x` and `p_y` represent the intial position of the agent in simulation space.
 */
class MENGE_API ExplicitGenerator : public AgentGenerator {
 public:
  /*!
   @brief    Constructor
   */
  ExplicitGenerator();

  /*!
   @brief    Reports the number of agents created.

   @returns  The number of agents this generator creates.
   */
  virtual size_t agentCount() { return _positions.size(); }

  /*!
   @brief    Sets the ith position to the given agent.

   @param    i      The index of the requested position in the sequence.
   @param    agt    A pointer to the agent whose position is to be set.
   @throws    AgentGeneratorException if the index, i, is invalid.
   */
  virtual void setAgentPosition(size_t i, BaseAgent* agt);

  /*!
   @brief    Adds a position to the generator

   @param    p    The position to add.
   */
  void addPosition(const Vector2& p);

 protected:
  /*!
   @brief    The agent positions parsed from the file.
   */
  std::vector<Vector2> _positions;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for ExplicitGenerator
 */
class MENGE_API ExplicitGeneratorFactory : public AgentGeneratorFactory {
 public:
  /*!
   @brief    The name of the generator type.

   The generator's name must be unique among all registered agent generator components. Each agent
   generator factory must override this function.

   @returns  A string containing the unique elevation name.
   */
  virtual const char* name() const { return "explicit"; }

  /*!
   @brief    A description of the agent generator.

   Each agent generator factory must override this function.

   @returns  A string containing the agent generator description.
   */
  virtual const char* description() const {
    return "Agent generation is done via an explicit list of agent positions, given "
           "in the XML specification.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's agent generator implementation.

   All AgentGeneratorFactory sub-classes must override this by creating (on the heap) a new
   instance of its corresponding generator type.  The various field values of the instance will be
   set in a subsequent call to AgentGeneratorFactory::setFromXML. The caller of this function takes
   ownership of the memory.

   @returns    A pointer to a newly instantiated EleAgentGenerator class.
   */
  AgentGenerator* instance() const { return new ExplicitGenerator(); }

  /*!
   @brief    Given a pointer to a Goal Selector instance, sets the appropriate fields from the
   provided XML node.

   It is assumed that the value of the `type` attribute is this Goal Selector's type (i.e.
   GoalSelectorFactory::thisFactory has already been called and returned true). If sub-classes of
   GoalSelectorFactory introduce *new* GoalSelector parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    gen          A pointer to the goal selector whose attributes are to be set.
   @param    node        The XML node containing the goal attributes.
   @param    behaveFldr  The path to the behavior file. If the condition references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(AgentGenerator* gen, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    Parses an agent position from an \<Agent\> tag.

   @param    node    The XML node containing the agent definition.
   @returns  The 2D point defined in the \<Agent\> tag.
   @throws    AgentGeneratorException is the agent tag doesn't provide the required data.
   */
  Vector2 parseAgent(TiXmlElement* node) const;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __EXPLICIT_AGENT_GENERATOR_H__
