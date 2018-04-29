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
 @file  EventTrigger.h
 @brief  The definition of the basic event trigger mechanism.
 */

#ifndef __EVENT_TRIGGER_H__
#define __EVENT_TRIGGER_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

#include <string>

namespace Menge {
// forward declaration
class EventTriggerFactory;

/*!
 @brief    The base class for event triggers.

 An event trigger specifies the conditions under which an event is triggered (allowing registered
 event handlers to respond).
 */
class MENGE_API EventTrigger : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  EventTrigger() : Element(), _firePeriod(0.f), _lastFire(-1e6), _name() {}

  /*!
   @brief    Allows the trigger to finish initializing itself from its parsed state to its running
            state.

   @throws    EventException if there is a problem finalizing.
   */
  virtual void finalize() {}

  /*!
   @brief    Evaluates the condition to see if it has been met.

   @returns  True if the condition has been met, false otherwise.
   */
  bool conditionMet();

  /*!
   @brief    Informs the trigger that the associated event effects have been applied (i.e. the event
            has been fired).
   */
  void fired();

  /*!
   @brief    Reports the name of the trigger.
   @returns  The trigger's name.
   */
  const std::string& getName() const { return _name; }

  friend class EventTriggerFactory;

 protected:
  /*!
   @brief    Evaluates the condition to see if it has been met.

   This must be implemented by all sub-classes.

   @returns    True if the condition has been met, false otherwise.
   */
  virtual bool testCondition() = 0;

  /*!
   @brief    An optional callback for when the trigger fired() method is called.
   
   Sub-classes can do any bookkeeping in response to having been fired.
   */
  virtual void onFired(){};

  /*!
   @brief    The minimum time between two successive firings (in simulation seconds).
   */
  float _firePeriod;

  /*!
   @brief    The time of the last firing (global simulation time).
   */
  float _lastFire;

  /*!
   @brief    The name of the trigger.
   */
  std::string _name;
};

}  // namespace Menge
#endif  // __EVENT_TRIGGER_H__
