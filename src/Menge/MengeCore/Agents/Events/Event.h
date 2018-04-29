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
 @file    Event.h
 @brief    The definition of the Menge Event.
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <vector>

// forward declaration
class TiXmlElement;

namespace Menge {
// forward declaration
class EventTrigger;
class EventResponse;
class Event;
class EventSystem;

///////////////////////////////////////////////////////////////////////

/*!
 @brief    Parses the xml specification of an event and returns a parsed event.

 The event instance will still need to be finalized.

 @param    node      The tiny xml node containing the event specification.
 @param    specFldr  The path to the specification file. All external resource references in the
                    spec file are defined relative to this folder.
 @returns  A new instance of an Event.  Null if there is a parsing error. The caller assumes
          responsibility to delete the instance.
 */
Event* parseEvent(TiXmlElement* node, const std::string& specFldr);

///////////////////////////////////////////////////////////////////////

/*!
 @brief    The definition of a Menge event.

 An event is defined by two components: a trigger, and a list of event *responses*. The trigger
 defines the circumstances which cause the event to "fire". The event can have multiple respones
 registered. When the event fires, each event response is triggered to modify the simulation.
 */
class Event {
 public:
  /*!
   @brief    Constructor.

   @param    name    The name of the event.
   */
  Event(const std::string& name);

  /*!
   @brief    Destructor.
   */
  ~Event();

  /*!
   @brief    Allows the event to finish initializing itself from its parsed state to its running
            state.

   @throws    EventFatalException if the event system has conservative configuration and there is a
              problem in the event specification.
   */
  void finalize();

  /*!
   @brief    Evaluates the event (and triggers handlers as necessary).
   */
  void evaluate();

  friend Event* parseEvent(TiXmlElement* node, const std::string& specFldr);
  friend class EventSystem;

 protected:
  /*!
   @brief    The name of the event.
   */
  std::string _name;

  /*!
   @brief    The trigger for the event.
   */
  EventTrigger* _trigger;

  /*!
   @brief    The effects registered to this event.
   */
  std::vector<EventResponse*> _responses;
};
}  // namespace Menge

#endif  // __EVENT_H__
