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
 @file    EventTarget.h
 @brief    The definition of the base event target class.  Defines what an event effect operates on.
 */

#ifndef __EVENT_TARGET_H__
#define __EVENT_TARGET_H__

#include "MengeCore/Core.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

namespace Menge {
// forward declaration
namespace Agents {
class BaseAgent;
}

/*!
 @brief    The base class for all event targets.

 An event target defines the operand of an event effect i.e., if the event effect defines how
 things change, the target defines which elements undergo the effect.
 */
class MENGE_API EventTarget : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  EventTarget() : Element(), _lastUpdate(-1.f) {}

  /*!
   @brief    Allows the event target to finish initializing itself from its parsed state to its
            running state.

   @throws    EventException if there is a problem finalizing.
   */
  virtual void finalize() {}

  /*!
   @brief    Gives the target a chance to update its knowledge of the target entities.

   If a sub-class needs to do particular computation to evaluate the target, it should be
   implemented here.
   */
  virtual void update() { _lastUpdate = SIM_TIME; }

 protected:
  /*!
   @brief    The global time that the target was last updated.
   */
  float _lastUpdate;
};

}  // namespace Menge
#endif  // __EVENT_TARGET_H__
