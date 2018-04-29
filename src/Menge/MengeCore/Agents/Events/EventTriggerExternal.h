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

#pragma once

/*!
 @file    EventTriggerExternal.h
 @brief    The definition of events which are triggered external of the simulation.
 */

#include "MengeCore/Agents/Events/EventTrigger.h"
#include "MengeCore/Agents/Events/EventTriggerFactory.h"

namespace Menge {
// forward declaration
class ExternalEvtTriggerFactory;

/*!
 @brief  The class for events that get triggered by code external to the simulator.

 This class is the mechanism through which larger systems can influence the simulation. External
 code can gain access to external triggers and aribtrarily "activate" them. At the next event
 evaluation (at the beginning of a time step), an activated trigger will have considered its
 condition to be met and the event response indicated for the event will be applied to the
 simulator. The activated state is cleared.

 @see EventSystem
 */
class MENGE_API ExternalEvtTrigger : public EventTrigger {
 public:
  /*!
   @brief    Constructor.
   */
  ExternalEvtTrigger() : EventTrigger(), _isActive(false) {}

  /*!
   @brief    Activates the trigger so that its condition is met at the beginning of the next
   simulation timestep.
   */
  void activate() { _isActive = true; }

  friend class ExternalEvtTriggerFactory;

 protected:
  /*!
   @brief    Evaluates the condition to see if it has been met.

   This must be implemented by all sub-classes.

   @returns    True if the condition has been met, false otherwise.
   */
  bool testCondition() override { return _isActive; }

  /*!
   @brief    An optional callback for when the trigger fired() method is called. Sub-classes can do
   any bookkeeping in response to having been fired.
   */
  void onFired() override { _isActive = false; }

  /*!
   @brief    The actiavted state
   */
  bool _isActive;
};

//////////////////////////////////////////////////////////////////////////

/*!
@brief    The factory for external event triggers.
*/
class MENGE_API ExternalEvtTriggerFactory : public EventTriggerFactory {
 public:
  /*!
  @brief    Constructor.
  */
  ExternalEvtTriggerFactory() {}

  /*!
   @brief    The name of the trigger type.

   The trigger's name must be unique among all registered triggers. Each trigger factory must
   override this function.

   @returns  A string containing the unique trigger name.
   */
  virtual const char* name() const { return "external"; }

  /*!
   @brief    A description of the event trigger.

   Each trigger factory must override this function.

   @returns  A string containing the trigger description.
   */
  virtual const char* description() const {
    return "Event trigger whose condition is determined by an entity external to the "
           "simulator.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's event trigger implementation.

   All EventTriggerFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding trigger type.  The various field values of the instance will be set in a
   subsequent call to EventTriggerFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated EventTrigger class.
   */
  EventTrigger* instance() const { return new ExternalEvtTrigger(); }

  /*!
   @brief    Given a pointer to an EventTrigger instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this EventTrigger's type (i.e.
   EventTrigger::thisFactory has already been called and returned true). If sub-classes of
   EventTriggerFactory introduce *new* EventTrigger parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    trigger    A pointer to the event trigger whose attributes are to be set.
   @param    node      The XML node containing the event trigger attributes.
   @param    specFldr  The path to the specification file. If the EventTrigger references resources
                      in the file system, it should be defined relative to the specification file
                      location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  bool setFromXML(EventTrigger* trigger, TiXmlElement* node,
                  const std::string& specFldr) const override;
};
}  // namespace Menge
