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
 @file    GLContextManager.h
 @brief   The mechanism by which OpenGL memory structures are restored after an OpenGL context
          change.
 */

#ifndef __GL_CONTEXT_MANAGER_H__
#define __GL_CONTEXT_MANAGER_H__

#include <list>
#include "MengeVis/VisConfig.h"

namespace MengeVis {

/*!
 @brief   Handles updating the scene graph when there is a new OpenGL context.

 Through this class and its static members, all of the resources used in the scene graph are
 refreshed when the OpenGL context changes.

 Each resource registers a refresh callback  to the manager and then the viewer, when changin GL
 Contexts, calls newGLContext() to have the resources update.

 If any new resources are added to the SceneGraph which have OpenGL-dependent memory (such as
 textures, display lists, arrays of data, etc.) that class must provide a static callback method and
 register it so its data can be restored in the new context (e.g. ImageData, Circle, Cylinder,
 etc.).
 */
class MENGEVIS_API GLContextManager {
  /*!
   @brief   A list of callbacks for performing context changes.
   */
  static std::list<void (*)()> _rsrcCallbacks;

 public:
  /*!
   @brief   Register a new class callback to the context manager.

   @param   newGLContext    A pointer to the callback function.
   */
  static void addCallback(void (*newGLContext)());

  /*!
   @brief   Called when the OpenGL context changes - triggers all of the registered callbacks.
   */
  static void newGLContext();
};
}  // namespace MengeVis
#endif  // __GL_CONTEXT_MANAGER_H__
