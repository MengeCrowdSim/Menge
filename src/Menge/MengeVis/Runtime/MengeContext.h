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

#ifndef __MENGE_CONTEXT_H__
#define __MENGE_CONTEXT_H__

#include "MengeVis/Runtime/StateContext.h"
#include "MengeVis/SceneGraph/Context.h"

namespace Menge {
namespace Agents {
class SimulatorInterface;
}
}  // namespace Menge

namespace MengeVis {
/*!
 @namespace   MengeVis::Runtime
 @brief       The namespace that contains runtime components (contexts, visual elements, etc.) for
              visualization.
 */
namespace Runtime {
// forward declarations
// namespace AgentContext {
class BaseAgentContext;
//}
class VelCompContext;
class VisAgent;

/*!
 @brief   This context provides the main interface for interaction with the Menge simulation.

 It creates a union of various other contexts: agent, velocity component, transition (not yet
 supported), etc.
 */
class MENGEVIS_API MengeContext : public SceneGraph::SelectContext {
 public:
  /*!
   @brief   Constructor.

   @param   sim   The simulator the context will interact with.
   */
  MengeContext(Menge::Agents::SimulatorInterface* sim);

  /*!
   @brief   Virtual destructor.
   */
  virtual ~MengeContext();

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual SceneGraph::ContextResult handleKeyboard(SDL_Event& e);

  /*!
   @brief   The draw function for the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   */
  virtual void drawGL(int vWidth, int vHeight);

  /*!
   @brief   Performs selection based on a click on screen space.

   Uses the OpenGL selection mechanism.

   @param   scene         The scene to select in.
   @param   camera        The camera.
   @param   vWidth        The width of the viewport.
   @param   vHeight       The height of the viewport.
   @param   selectPoint   The point (in screen space) at which object selection should take place.
   @returns A boolean indicating whether a redraw needs to take place.
   */
  virtual bool selectGL(const SceneGraph::GLScene* scene, const SceneGraph::GLCamera& camera,
                        int vWidth, int vHeight, int* selectPoint);

  /*!
   @brief   Called when the context is activated.
   */
  virtual void activate();

 protected:
  /*!
   @brief   Draw UI elements into the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   @param   select    Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  virtual void drawUIGL(int vWidth, int vHeight, bool select = false);

  /*!
   @brief   Causes the context to update its state based on the currentselected state.

   @returns True if the update caused the context to change what it operates on.
   */
  bool updateSelected();

 private:
  /*! @brief  The simulator interface the context works on. */
  Menge::Agents::SimulatorInterface* _sim;

  /*! @brief    Defines the state of keyboard processing in this context. */
  enum KeyboardState {
    DEFAULT_ST,       ///< The default operating state
    TYPE_AGENT_ID_ST  ///< Typing-in-an-agent-id selection
  };

  /*! @brief    The keyboard state of the context. */
  KeyboardState _state;

  // TODO: How much of this should be const?
  /*! @brief    The agent context for the current selectd agent. */
  BaseAgentContext* _agentContext;

  /*! @brief    The state context for the current selected agent. */
  StateContext _stateContext;

  // TODO: transition context

  /*! @brief    The currently selected visualization agent. */
  VisAgent* _selected;

  /*! @brief    A pointer to the agents in the scene. */
  VisAgent** _visAgents;

  /*! @brief    The number of agents in the scene. */
  size_t _agtCount;

  /*! @brief    Determines if the velocity component is displayed in the 3D context. */
  bool _drawVC;

  /*! @brief    Determines if the transition is displayed in the 3D context. */
  bool _drawTrans;

  /*! @brief    The maximum number of digits for typing. */
  static const unsigned int MAX_TYPE_DIGITS = 10;

  /*! @brief    The character array for typing numbers into. */
  char _digits[MAX_TYPE_DIGITS + 1];

  /*! @brief    Number of typed digits. */
  unsigned int _digitCount;

  /*! @brief    Begins the agent typing state. */
  void beginIDTyping();

  /*! @brief    Finish agent typing state. */
  void finishIDTyping();

  /*! @brief    Cancels the agent typing state. */
  void cancelIDTyping();

  /*! @brief    Draws the id typing state. */
  void drawIDTyping();

  /*! @brief    Adds a digit to the typed value. */
  void addIDDigit(const char digit);
};
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __MENGE_CONTEXT_H__
