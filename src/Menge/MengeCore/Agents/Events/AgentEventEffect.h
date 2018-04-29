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
 @file    AgentEventEffect.h
 @brief   The definition of the agent event effect -- defines the agent-base effect.
 */

#ifndef __AGENT_EVENT_EFFECT_H__
#define __AGENT_EVENT_EFFECT_H__

#include "MengeCore/Agents/Events/EventEffect.h"
#include "MengeCore/Agents/Events/EventEffectFactory.h"

namespace Menge {
// forward declaration
class AgentEventTarget;

namespace Agents {
class BaseAgent;
}

/*!
 @brief   The *abstract* event effect class that operates on sets of agents.

 This event effect operates on agents. It provides an interface to confirm it is combined with a
 compatible target type (AgentEventTarget). It also provides an implementation that iterates through
 the target set of operand agents. Sub-classes must implement the agentEffect() method to provide
 the details of its *specific* effect.
 */
class MENGE_API AgentEventEffect : public EventEffect {
 public:
  /*!
   @brief    Reports if the given target is compatible with this effect.

   Each effect can only work on certain types of targets.  This function reports if the given 
   target works with this effect.

   @param    target    The target instance to test.
   @returns  True if the target is a valid argument to EventEffect::apply.
   */
  virtual bool isCompatible(EventTarget* target);

  /*!
   @brief    Applies the effect to the simulation target.

   Not all targets work with all effects.  The target passed here must have previously passed the
   EventEffect::isCompatible test to work.

   @param    target    The target to apply the event to.
   */
  virtual void apply(EventTarget* target);

 protected:
  /*!
   @brief    The actual work of the effect.

   Sub-classes should implement this.  It is the action to be taken for each agent.

   @param    agent    The agent to operate on.
   */
  virtual void agentEffect(Agents::BaseAgent* agent) = 0;
};

}  // namespace Menge

#endif  // __AGENT_EVENT_EFFECT_H__
