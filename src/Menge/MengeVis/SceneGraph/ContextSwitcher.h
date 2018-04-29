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
 @file    ContextSwitcher.h
 @brief   The definition of a context which allows the ability to select between multiple contexts.
 */

#ifndef __CONTEXT_SWITCHER_H__
#define __CONTEXT_SWITCHER_H__

#include <map>
#include "MengeVis/SceneGraph/Context.h"

namespace MengeVis {
namespace SceneGraph {
/*!
 @brief   A mapping from a SDL key value and a context.
 */
typedef std::map<SDL_Keycode, Context*> KeyContextMap;

/*!
 @brief   Special context for switching between multiple contexts.

 Each context tied to a keyboard shortcut.
 */
class MENGEVIS_API ContextSwitcher : public Context {
 public:
  /*!
   @brief   Constructor.
   */
  ContextSwitcher();

  /*!
   @brief   Destructor.
   */
  virtual ~ContextSwitcher();

  /*!
   @brief   Add a context and it's "hotkey".

   @param   context   The context to add.
   @param   key       The key which activates the context.
   */
  void addContext(Context* context, SDL_Keycode key);

  /*!
   @brief   Switch to the indicated context.

   @param   context   The newly active context.
   @returns A boolean reporting if a change is actually made (true) or not (false).
   */
  bool switchContexts(Context* context);

  /*!
   @brief   Allow the context to update its time-dependent state to the given global time.
   */
  virtual void update();

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
  virtual bool selectGL(const GLScene* scene, const GLCamera& camera, int vWidth, int vHeight,
                        int* selectPoint);

  /*!
   @brief   Callback for when the OpenGL context is changed.
   */
  virtual void newGLContext();

  /*!
   @brief   Give the context the opportunity to respond to a mouse event.

   @param   e   The SDL event with the mouse event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual ContextResult handleMouse(SDL_Event& e);

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual ContextResult handleKeyboard(SDL_Event& e);

 protected:
  /*!
   @brief   The currently active context.
   */
  Context* _activeContext;

  /*!
   @brief   A mapping for key to context.
   */
  KeyContextMap _contexts;
};
}  // namespace SceneGraph
}  // namespace MengeVis

#endif  // __CONTEXT_SWITCHER_H__