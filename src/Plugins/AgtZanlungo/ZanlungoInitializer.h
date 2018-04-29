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
 @file		ZanlungoInitializer.h
 @brief		The AgentInitializer for the Zanlungo simulator.
 */
#ifndef __ZANLUNGO_INITIALIZER_H__
#define __ZANLUNGO_INITIALIZER_H__

#include "MengeCore/Agents/AgentInitializer.h"

namespace Zanlungo {
/*!
 @brief		Class which determines the agent properties for each new Zanlungo agent.
 */
class AgentInitializer : public Menge::Agents::AgentInitializer {
 public:
  /*!
   @brief		Constructor.

   The values for each agent take a hard-coded default values.
   */
  AgentInitializer();

  /*!
   @brief		Copy Constructor.

   @param		init		The initializer to copy values from
   */
  AgentInitializer(const AgentInitializer& init);

  /*!
   @brief		Destructor.
   */
  virtual ~AgentInitializer();

  /*!
   @brief		Sets the properties of the given agent based on the initializer's values.

   This needs to be overridden by sub-classes. The first thing the sub-class should do is
   dynamic_cast the argument to its expected type to make sure it is the proper agent type. If not,
   this should be considered failure. Then it should set its unique properties an+d then call the
   super class's setProperties function.

   @param		agent		The agent whose properties are to be set.
   @returns	True if the properties were set successfully, false otherwise.
   */
  virtual bool setProperties(Menge::Agents::BaseAgent* agent);

  /*!
   @brief		Sets all generators to default values.

   Resets all number generators to default const values. This assumes that all required number
   generators already exist and will delete them appropriately. *Do not* call this in the
   constructor.
   */
  virtual void setDefaults();

  /*!
   @brief		Creates a copy of this AgentInitializer instance.

   @returns	A pointer to a new AgentInitializer with all of the same values as this. The caller is
            responsible for freeing up the new instance.
   */
  virtual AgentInitializer* copy() const { return new AgentInitializer(*this); }

 protected:
  /*!
   @brief		Reports if this AgentInitializer cares about the given AgentSet property XML tag.

   This is the mechanism by which new sub-classes can extend the parameter space. Each pedestrian
   model which introduces new per-agent properties that must override this function. However, the
   overriden function must, in turn, call the parent class if it doesn't consider the tag relevant,
   giving the parent class a chance to determine if the tag is relevant. This is the mechanism by
   which derived classes will also benefit from the `<Zanlungo>` parameter set.
   *
   @param		tagName			The tag to test for relevancy.
   @returns	True if the tag is relevant, false otherwise.
   */
  virtual bool isRelevant(const ::std::string& tagName);

  /*!
   @brief		Defines a constant value for an agent property as specified by the attribute of an agent
            property tag.

   Derived classes should override this function, but possibly call the parent class's
   implementation. First, it should test to see if the paramName is expected by the derived class.
   If so, the derived class can determine fail or accept. If it is not expected, it should call the
   parent class's implementation and returns its value.

   @param			paramName		A string containing the parameter name.
   @param			value			  A string containing the value for the parameter.
   @returns		The result of the parse: failure, ignored, or accepted.
   */
  virtual Menge::Agents::AgentInitializer::ParseResult setFromXMLAttribute(
      const ::std::string& paramName, const ::std::string& value);

  /*!
   @brief		Process the given `<Property .../>` tag.

   As a pre-condition to this function, the XML node contains a `<Property.../>` tag and has been
   confirmed to have, at least, a name attribute. Nothing else about the tag has been validated.

   If the property name is unexpected, it will be ignored. If it is expected, this function will
   attempt to interpret the XML tag as a number distribution for a valid agent attribute. If it can
   do so, it is successful, if it can't, it fails.

   @param		propName		The extractd "name" property from the Property tag.
   @param		node			  The XML node for the Property tag.
   @returns	True if parsing was successful.
   */
  virtual ParseResult processProperty(::std::string propName, TiXmlElement* node);

  /*!
   @brief		The mass of the agent.
   */
  Menge::Math::FloatGenerator* _mass;
};
}  // namespace Zanlungo

#endif  // __ZANLUNGO_INITIALIZER_H__
