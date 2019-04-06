#pragma once
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

/**
  @file   change_state_effect.h
  @brief  The definition of event effect that moves the target agents to a new state.
 */

#include "MengeCore/Agents/Events/AgentEventEffect.h"
#include "MengeCore/Agents/Events/EventEffectFactory.h"

namespace Menge {

// forward declaration
class ChangeStateEffectFactory;

namespace Agents {
class BaseAgent;
}  // namespace Agents

namespace BFSM {
class State;
}  // namespace BFSM

/** @brief  An event effect that moves the target agents to the effect's associated state.
 */
class MENGE_API ChangeStateEffect : public AgentEventEffect {
 public:
  /** @brief  Implementation of EventEffect::finalize().  */
  void finalize() override;

  friend class ChangeStateEffectFactory;

 protected:
  /** @brief  Implements AgentEventEffect::agentEffect().  */
  void agentEffect(Agents::BaseAgent* agent) override;

  /** @brief  If true, agents will be forced to re-enter the target state if they are already in
              that state.  */
  bool _force_reentry{false};

  /** @brief  The name of the requested state.  */
  std::string _stateName;

  /** @brief  The state to move agents into -- defined during finalization.  */
  BFSM::State* _state{nullptr};
};

//////////////////////////////////////////////////////////////////////////

/** @brief  The factory for ChangeStateEffect event effects.  */
class MENGE_API ChangeStateEffectFactory final : public EventEffectFactory {
 public:
  /** @brief  Constructor.  */
  ChangeStateEffectFactory();

  /** @brief  Implements EventEffectFactory::name().  */
  const char* name() const override { return "change_state"; }

  /** @brief  Implements EventEffectFactory::description().  */
  const char* description() const override {
    return "Event effect that moves the target agents to the specified state.";
  };

 protected:
  /** @brief  Implements EventEffectFactory::setFromXML().  */
  bool setFromXML(EventEffect* effect, TiXmlElement* node,
                  const std::string& behaveFldr) const override;

  /** @brief  Implements EventEffectFactory::instance().  */
  EventEffect* instance() const override { return new ChangeStateEffect(); }

  /** @brief  The identifier for the "state" string attribute.  */
  size_t _stateID;

  /** @brief  The identifier for the "force_reentry* bool attribute.  */
  size_t _forceID;
};

}  // namespace Menge
