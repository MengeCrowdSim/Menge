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
 @file    StateContext.h
 @brief   The definition of a basic UI context for finite state machine *states*.
 */

#ifndef __STATE_CONTEXT_H__
#define __STATE_CONTEXT_H__

#include "MengeCore/mengeCommon.h"
#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"
#include "MengeVis/Runtime/VCContext/VelCompContext.h"
#include "MengeVis/SceneGraph/Context.h"
#include "MengeVis/VisConfig.h"

namespace Menge {
namespace BFSM {
class State;
}
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace MengeVis {

// forward declaration
namespace Runtime {
class StateContext;

/*!
 @brief   A map from state ids to statecontexts to facilitate lookups for a visualized agent
 */
typedef HASH_MAP<size_t, Runtime::StateContext*> StateContextMap;

/*!
 @brief   Base context for finite state machine states.

 This differs from the standard scene graph context by being dependent on an input agent.
 */
class StateContext : public SceneGraph::Context {
 public:
  /*!
   @brief   Default constructor.
   */
  StateContext();

  /*!
   @brief   Destructor.
   */
  virtual ~StateContext();

  /*!
   @brief   Sets the state this context works on.

   @param   state   The state to represent with this context.
   */
  void setState(Menge::BFSM::State* state);

  /*!
   @brief   Give the context the opportunity to respond to a keyboardevent.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual SceneGraph::ContextResult handleKeyboard(SDL_Event& e);

  /*!
   @brief   Provides a string to be printed in the display as a UI element detailing state
            information.

   @param   indent    An optional string representing indentation to be applied to the text. It is
                      prefixed at the start of each line.
   @returns The string for printing on the UI layer.
   */
  std::string getUIText(const std::string& indent = "") const;

  /*!
   @brief   Draw context elements into the 3D world.

   @param   agt         The particular agent for which the FSM is being visualized.
   @param   drawVC      Draw the velocity component
   @param   drawTrans   Draw the transition
   */
  virtual void draw3DGL(const Menge::Agents::BaseAgent* agt, bool drawVC, bool drawTrans);

 protected:
  /*!
   @brief   The underlying finite state machine state.
   */
  Menge::BFSM::State* _state;

  /*!
   @brief   The value used to indicate that no id is selected.

   Used in conjunction with the _activeVC and _activeTransition
   */
  static size_t NO_ACTIVE_ID;

  /*!
   @brief   The velocity component context for this state.
   */
  VCContext::VelCompContext* _vcContext;

  /*!
   @brief   The goal renderer for this state.
   */
  GoalVis::GoalRenderer* _goalRenderer;

  /*!
   @brief   The id of the "active" transition.

   This is the index of the transition which is currently being visualized in the context.
   */
  size_t _activeTransition;
};
}  // namespace Runtime
}  // namespace MengeVis
#endif  // __STATE_CONTEXT_H__
