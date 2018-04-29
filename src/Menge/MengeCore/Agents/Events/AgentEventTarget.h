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
 @file    AgentEventTarget.h
 @brief   The definition of the event target for agents.
 */

#ifndef __AGENT_EVENT_TARGET_H__
#define __AGENT_EVENT_TARGET_H__

#include <vector>
#include "MengeCore/Agents/Events/EventTarget.h"
#include "MengeCore/Agents/Events/EventTargetFactory.h"

namespace Menge {
/*!
 @brief    The base class for all event targets which target agents.

 Event targets are fundamentally defined by what the target of the effect is. Each event target
 specifies a single kind of target. This event target produces a set of one or more agents to 
 effect.
 */
class MENGE_API AgentEventTarget : public EventTarget {
 public:
  /*!
   @brief    Returns an iterator to the beginning of the target's elements.
   */
  std::vector<Agents::BaseAgent*>::iterator begin() { return _elements.begin(); }

  /*!
   @brief    Returns an iterator to the end of the target's elements.
   */
  std::vector<Agents::BaseAgent*>::iterator end() { return _elements.end(); }

 protected:
  /*!
   @brief    The agents targeted by the event effect.
   */
  std::vector<Agents::BaseAgent*> _elements;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the AgentEventTarget.
 */
class MENGE_API AgentEventTargetFactory : public EventTargetFactory {};

}  // namespace Menge
#endif  // __AGENT_EVENT_TARGET_H__
