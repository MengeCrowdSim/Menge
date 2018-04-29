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
 @file    EventSystem.h
 @brief    The definition of the core event system.
 */

#ifndef __EVENT_SYSTEM_H__
#define __EVENT_SYSTEM_H__

#include "MengeCore/mengeCommon.h"

#include <vector>

// forward declaration
class TiXmlElement;

namespace Menge {
// forward declaration

class Event;
class EventResponse;
class EventEffect;
class EventTarget;
Event* parseEvent(TiXmlElement* node, const std::string& specFldr);
class ExternalEvtTrigger;

/*!
 @brief  The main event engine -- causes event triggers to be evaluated and effects to be applied.

 The %EventSystem drives %Menge's event mechanism. Events are added to the simulator through the
 behavior specification. If no events are added to the simulator, the `EventSystem` tag can be
 omitted from the behavior specification. The following example shows the syntax for declaring the
 event system and provides a vague illustration for adding an event.

 ```{xml}
 <?xml version="1.0"?>
 <BFSM>
    <EventSystem conservative="1">
      <Target name="my_target" type="target_element" ... />
      <Effect name="my_effect" type="effect_element" ... />
      <Event name="my_event">
        <Trigger name="something_happened" type="trigger_element" />
        <Response effect="my_effect" target="my_target" />
      </Event>
    </EventSystem>
  ...
  </BFSM>
  ```

 The `<%EventSystem>` tag has a single property:
     - `conservative` determines how the event system handles errors in the declaration of events. If
       the value is non-null, the system is conservative and any errors in the definition of events
       are treated as *fatal* errors and the program exits (having written the cause of the problem
       to the log). If zero, the program will log a *warning*, omit the event, and proceed with
       simulation.

 */
class MENGE_API EventSystem {
 public:
  /*!
   @brief    Constructor.
   */
  EventSystem();

  /*!
   @brief    Destructor
   */
  ~EventSystem();

  /*!
   @brief    Adds an event to the system.

   The system becomes the owner of the event and will take responsibility for deleting it.

   @param    evt    The event to add to the system.
   */
  void addEvent(Event* evt) { _events.push_back(evt); }

  /*!
   @brief    Evaluates the registered events
   */
  void evaluateEvents();

  /*!
   @brief    Finalize the event system.
   */
  void finalize();

  /*!
   @brief    Parses events from an "Events" tag.

   @param    node        The node containing the event system description.
   @param    behaveFldr  The folder containing the behavior specification. all event paths are
                        defined relative to this folder.
   @returns  True if parsing was successful.
   */
  bool parseEvents(TiXmlElement* node, const std::string& behaveFldr);

  /*!
   @brief    Registers an external event trigger with the system.
   
   Any external event trigger that is not added explicitly will not be available to external
   systems upon query.

   @param    trigger    The external event trigger to add to the system.
   @throws    An event exception if the trigger does not have a unique name.
   @see listExternalTriggers()
   */
  void addExternalEventTrigger(ExternalEvtTrigger* trigger);

  /*!
   @brief    Provides the names of the external events registered with the event system.

   @returns  The list of trigger names.
   */
  std::vector<std::string> listExternalTriggers() const;

  /*!
   @brief    Activate the external event trigger indicated by name.

   If the name doesn't refer to a known external trigger, no action will be taken.

   @param name    The name of the trigger to activate.
   @returns  True if the trigger is activated.
   */
  bool activateExternalTrigger(const std::string& name);

  /*!
   @brief    Causes an event exception to be thrown based on the the system's tolerance for event
            configuration errors.

   @param    msg      The event associated with the problem.
   @throws    EventException if the system is *not* conservative; i.e., configuration errors will
              be noted, but simulation will continue.
   @throws    EventFatalException if the system *is* conservative.
   */
  static void finalizeException(const std::string& msg);

  /*!
   @brief    Global variable to indicate how event configuration errors should be handled.

   If true, event configuration errors result in fatal exceptions, if false they are merely
   warnings.
   */
  static bool CONSERVATIVE_SETUP;

  friend class Event;
  friend class EventEffect;
  friend class EventTarget;
  friend class EventResponse;
  friend Event* parseEvent(TiXmlElement* node, const std::string& specFldr);

 protected:
  /*!
   @brief    The events to process.
   */
  std::vector<Event*> _events;

  /*!
   @brief    A mapping from target names to targets.
   */
  HASH_MAP<std::string, EventTarget*> _targets;

  /*!
   @brief    A mapping from effect names to effects.
   */
  HASH_MAP<std::string, EventEffect*> _effects;

  /*!
   @brief    The external event triggers.
   */
  HASH_MAP<std::string, ExternalEvtTrigger*> _externalTriggers;
};
}  // namespace Menge

#endif  // __EVENT_SYSTEM_H__
