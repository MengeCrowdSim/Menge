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
 @file    EventEffect.h
 @brief    The definition of the event effect -- defines the response to a triggered response.
 */

#ifndef __EVENT_EFFECT_H__
#define __EVENT_EFFECT_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

namespace Menge {
// forward declaration
class EventTarget;
class EventEffectFactory;

/*!
 @brief    The definition of a response to an event.

  Event effects determine how the simulation changes due to a triggered event. The %EventEffect
  works in conjunction with the EventTarget class. The effect is the *operator* and the target is
  the *operand*. The two are, in some sense, coupled. Ultimately, effects can change any aspect of
  the simulator state. However, any *particular* effect can only operate on a particular aspect of
  the simulator (e.g., velocity components, agents, states, etc.) That means event effects must be
  partnered with *compatible* EventTarget instances.

  The %EventEffect interface provides a means for defining what kind of targets are compatible and
  for applying its operation onto the target itself.
*/
class MENGE_API EventEffect : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  EventEffect() : Element() {}

  /*!
   @brief    Reports if the given target is compatible with this effect.

   Each effect can only work on certain types of targets. This function reports if the given target
   works with this effect.

   @param    target    The target instance to test.
   @returns  True if the target is a valid argument to EventEffect::apply.
   */
  virtual bool isCompatible(EventTarget* target) = 0;

  /*!
   @brief    Applies the effect to the simulation target.

   Not all targets work with all effects.  The target passed here must have previously passed the
   EventEffect::isCompatible test to work.

   @param    target    The target to apply the event to.
   */
  virtual void apply(EventTarget* target) = 0;

  /*!
   @brief    Allows the event effect to finish initializing itself from its parsed state to its
            running state.

   @throws    EventException if there is a problem finalizing.
   */
  virtual void finalize() {}

  friend class EventEffectFactory;
};

}  // namespace Menge

#endif  // __EVENT_EFFECT_H__
