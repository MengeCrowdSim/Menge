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
 @file    Element.h
 @brief    Base class for all Menge elements.
 */

#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "MengeCore/CoreConfig.h"

namespace Menge {

//// forward declarations
// namespace SceneGraph {
//  class GLNode;
//}

namespace BFSM {
class Task;
}

/*!
 @brief    The basic interface of extendible Menge Elements.

 A Menge element is a component of the Menge framework. A particular crowd simulator is defined by
 the instantiation of various types of elements.
 */
class MENGE_API Element {
 protected:
  /*!
   @brief    Protected destructor; virtual to work with subclassing.
   */
  virtual ~Element() {}

 public:
  /*!
   @brief    This supplants the destructor.

   In order to avoid potential problems in windows when dlls do not share the same c-runtime
   library, the destructor is held to be protected.  To garbage collect an Element, the destroy
   method should be called (which in turn, will call the destructor from its own memory space,
   averting run-time crashes).

   Once this has been called, the Element no longer exists. Calling methods or accessing members
   will produce indetermine behavior (most likely errors).
   */
  void destroy() { delete this; }

  /*!
   @brief    Return an optional task associated with this element.

   An element can have an accompanying Task for performing synchronized work. Most element
   implementations will not have a task.  If a task is required, override this function to return
   an appropriate instance of the required task.

   @returns    A pointer to the required task. It is the responsibility of the caller to free the
              memory of the provided task by calling its destroy method.
   */
  virtual BFSM::Task* getTask() { return 0x0; }

#if 0
  /*!
    @brief    Returns an optional visualization element associated with the element.

    This element will simply be added to the scene graph and will be destroyed by the scene graph.
    It should not be used if the visualization is context dependent.

    @returns    A pointer to the scene graph node element.
    */
  virtual SceneGraph::GLNode * getSGNode() { return 0x0; }
#endif
};

}  // namespace Menge

#endif  // __ELEMENT_H__
