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
 @file    AgentPropertyEffect.h
 @brief   The definition of an event effect that modifies agent properties.
 */

#ifndef __AGENT_PROPERT_EFFECT_H__
#define __AGENT_PROPERT_EFFECT_H__

#include "MengeCore/Agents/AgentPropertyManipulator.h"
#include "MengeCore/Agents/Events/AgentEventEffect.h"
#include "MengeCore/Agents/Events/EventEffectFactory.h"

namespace Menge {

// forward declaration
namespace Agents {
class BaseAgent;
}

/*!
 @brief    The base class event effect changes agent properties.

 This is an abstract class and must be sub-classed.  To create different types of event effects,
 simply specialize this templated class with a different type of AgentPropertyManipulator.
 */
template <class Manipulator>
class MENGE_API AgentPropertyEffect : public AgentEventEffect {
 public:
  /*!
   @brief    Constructor
   */
  AgentPropertyEffect() : AgentEventEffect(), _manip() {}

  /*!
   @brief    Returns a pointer to the manipulator.
   */
  Manipulator* getManipulator() { return &_manip; }

 protected:
  /*!
   @brief    The actual work of the effect.

   Sub-classes should implement this.  It is the action to be taken for each agent.

   @param    agent    The agent to operate on.
   */
  virtual void agentEffect(Agents::BaseAgent* agent) { _manip.manipulate(agent); }

  /*!
   @brief    The manipulator responsible for changing agent properties.
   */
  Manipulator _manip;
};

//////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for agent property event effects.
 */
template <class Manipulator>
class MENGE_API AgentPropertyEffectFactory : public EventEffectFactory {
 public:
  /*!
   @brief    Constructor.
   */
  AgentPropertyEffectFactory() : EventEffectFactory() {
    _propertyID = _attrSet.addStringAttribute("property", true /*required*/);
    _generatorID = _attrSet.addFloatDistAttribute("", true /*required*/, 0.f, 1.f);
  }

 protected:
  /*!
   @brief    Given a pointer to an AgentPropertyEffect instance, sets the appropriate fields from
   the provided XML node.

   It is assumed that the value of the `type` attribute is this AgentPropertyEffect's type. (i.e.
   EventEffectFactory::thisFactory has already been called and returned true.) If sub-classes of
   EventEffectFactory introduce *new* EventEffect parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    effect       A pointer to the effect whose attributes are to be set.
   @param    node         The XML node containing the event effect attributes.
   @param    behaveFldr   The path to the behavior file.  If the action references resources in the
                          file system, it should be defined relative to the behavior file location.
                          This is the folder containingthat path.
   @returns  True if successful.
   */
  virtual bool setFromXML(EventEffect* effect, TiXmlElement* node,
                          const std::string& behaveFldr) const {
    AgentPropertyEffect<Manipulator>* aEffect =
        dynamic_cast<AgentPropertyEffect<Manipulator>*>(effect);
    assert(aEffect != 0x0 &&
           "Trying to set agent event effect properties on an "
           "incompatible object");

    // This parses the target
    if (!EventEffectFactory::setFromXML(aEffect, node, behaveFldr)) return false;

    // Configure manipulator
    AgentPropertyManipulator* manip = aEffect->getManipulator();

    BFSM::PropertyOperand prop = parsePropertyName(_attrSet.getString(_propertyID));
    manip->setProperty(prop);
    if (prop == BFSM::NO_PROPERTY) {
      logger << Logger::ERR_MSG << "The property event effect defined on line ";
      logger << node->Row();
      logger << " specified an invalid value for the \"property\" attribute";
      return false;
    }
    manip->setGenerator(_attrSet.getFloatGenerator(_generatorID));

    return true;
  }

  /*!
   @brief    The identifier for the "property" string attribute.
   */
  size_t _propertyID;

  /*!
   @brief    The identifier for the float distribution attribute.
   */
  size_t _generatorID;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the SetPropertyEffect.
 */
class MENGE_API SetAgentPropertyEffectFactory
    : public AgentPropertyEffectFactory<SetPropertyManipulator> {
 public:
  /*!
   @brief    The name of the effect.

   The effect's name must be unique among all registered effect. Each effect factory must override
   this function.

   @returns  A string containing the unique effect name.
   */
  virtual const char* name() const { return "set_agent_property"; }

  /*!
   @brief    A description of the effect.

   Each effect factory must override this function.

   @returns  A string containing the effect description.
   */
  virtual const char* description() const {
    return "Causes the specified property to be *replaced* by the user-defined value";
  };

 protected:
  /*!
   @brief    Create an instance of this class's effect.

   All EventEffectFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding event effect type.  The various field values of the instance will be set in
   a subsequent call to EventEffectFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated EventEffect class.
   */
  EventEffect* instance() const { return new AgentPropertyEffect<SetPropertyManipulator>(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the OffsetPropertyEffect.
 */
class MENGE_API OffsetAgentPropertyEffectFactory
    : public AgentPropertyEffectFactory<OffsetPropertyManipulator> {
 public:
  /*!
   @brief    The name of the effect.

   The effect's name must be unique among all registered effect. Each effect factory must override
   this function.

   @returns  A string containing the unique effect name.
   */
  virtual const char* name() const { return "offset_agent_property"; }

  /*!
   @brief    A description of the effect.

   Each effect factory must override this function.

   @returns  A string containing the effect description.
   */
  virtual const char* description() const {
    return "Causes the specified property to be offset by the user-defined value";
  };

 protected:
  /*!
   @brief    Create an instance of this class's effect.

   All EventEffectFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding event effect type.  The various field values of the instance will be set in
   a subsequent call to EventEffectFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated EventEffect class.
   */
  EventEffect* instance() const { return new AgentPropertyEffect<OffsetPropertyManipulator>(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ScalePropertyEffect.
 */
class MENGE_API ScaleAgentPropertyEffectFactory
    : public AgentPropertyEffectFactory<ScalePropertyManipulator> {
 public:
  /*!
   @brief    The name of the effect.

   The effect's name must be unique among all registered effect. Each effect factory must override
   this function.

   @returns  A string containing the unique effect name.
   */
  virtual const char* name() const { return "scale_agent_property"; }

  /*!
   @brief    A description of the effect.

   Each effect factory must override this function.

   @returns  A string containing the effect description.
   */
  virtual const char* description() const {
    return "Causes the specified property to be scaled by the user-defined value";
  };

 protected:
  /*!
   @brief    Create an instance of this class's effect.

   All EventEffectFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding event effect type.  The various field values of the instance will be set in
   a subsequent call to EventEffectFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated EventEffect class.
   */
  EventEffect* instance() const { return new AgentPropertyEffect<ScalePropertyManipulator>(); }
};

}  // namespace Menge

#endif  // __AGENT_PROPERT_EFFECT_H__
