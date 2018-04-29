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
 @file    Action.h
 @brief    The definition of actions that are taken as agents enter states.

 Actions are associated with behavior states.  When an agent enters the corresponding
 behavior state, the action is taken on the agent.  It is typically used to modify
 agent parameters, consistent with what the state represents.  By default the agent
 should restore the agents parameter values when the agent leaves the FSM state.
 However, a flag can deactivate this behavior.
 */

#ifndef __ACTION_H__
#define __ACTION_H__

#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/Element.h"

// forward declaration
class TiXmlElement;

namespace Menge {

/*!
 @namespace Menge::BFSM
 @brief  The namespace contains the Behavior Finite State Machine (BFSM) definition
 */
namespace BFSM {

// forward declaration
class ActionFactory;

/*!
 @brief    The abstract definition of an action.

 An action is explicitly executed upon entering a state and has the option of undoing its effect
 upon exiting the state.
 */
class MENGE_API Action : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  Action() : Element(), _undoOnExit(true) {}

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~Action() {}

 public:
  /*!
   @brief    Upon entering the state, this is called -- it is the main work of the action.

   This is a purely virtual function.  Any instantiable Action sub-class must *explicitly* account
   for this function.

   @param    agent    The agent to act on.
   */
  virtual void onEnter(Agents::BaseAgent* agent) = 0;

  /*!
   @brief    The work to do upon state exit.

   @param    agent    The agent to act on.
   */
  void onLeave(Agents::BaseAgent* agent);

  friend class ActionFactory;

 protected:
  /*!
   @brief    The work to do when reseting an agent up on exit reset.

   The reset action is guaranteed to be called *before* the general leave action, if the action has
   been configured to reset.

   @param[in,out]  agent    If non-null, the action should take whatever actions are necessary to
                            reset the action's effects.
   */
  virtual void resetAction(Agents::BaseAgent* agent) {}

  /*!
   @brief   Work that will be done *unconditionally* when an agent leaves the state  to which this
            action belongs.

   @param    agent    The agent to act on.
   */
  virtual void leaveAction(Agents::BaseAgent* agent){};

  /*!
   @brief    Determines if the action undoes itself on exiting the state.
   */
  bool _undoOnExit;
};

/*!
 @brief    Parses a TinyXML element containing an action specification

 @param    node          The TinyXML element
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                         relative to this folder.
 @returns  A pointer to the new action .
 */
Action* parseAction(TiXmlElement* node, const std::string& behaveFldr);

}  // namespace BFSM
}  // namespace Menge

#endif  // __ACTION_H__
