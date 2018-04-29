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

// A system drives attributes of a graph
//    Before drawing a graph, systems get evaluated to make sure what's being drawn
//    is "correct" and "updated".
// Systems are relatively lazy.  Their update can be called, but only if the system KNOWS it needs
//     to be updated, will it update.

/*!
 @file    System.h
 @brief   The mechanism for evolving a scene w.r.t. time.
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <list>
#include <string>
#include "MengeCore/MengeException.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {

/*!
 @brief   Exception class for indicating that a system is done.

 When a system will no longer change the the scene with calls to System::updateScene(), it throws
 the SystemStopException. The caller is responsible for catching and reacting appropriately.
 */
class MENGEVIS_API SystemStopException : public Menge::MengeException {
 public:
  /*!
   @brief   Default constructor.
   */
  SystemStopException() : Menge::MengeException() {}
};

/*!
 @brief   An animation system.
 
 Responsible for updating the scene based based on increasing time values. This is a purely virtual
 class. It must be sub-classed to be used.
 */
class MENGEVIS_API System {
 public:
  /*!
   @brief   Default constructor.
   */
  System() {}

  /*!
   @brief   Virtual destructor.
   */
  virtual ~System() {}

  /*!
   @brief   Finalizing function for the system.

   When the viewer wants to exit, it calls this method on the scene to give the scene any last
   minute things it needs to do (such as outputting data). The scene calls the systems finish
   method.
   */
  virtual void finish() {}

  /*!
   @brief   Updates the state of the scene graph with the global time.

   @param   time    The time for which the system should be set.
   @returns True if the system changed the scene such that it should be redrawn.
   */
  virtual bool updateScene(float time) = 0;
};

/*!
 @brief   An STL list of System objects.
 */
typedef std::list<System*> SystemList;

/*!
 @brief   An iterator for the SystemList.
 */
typedef SystemList::iterator SystemListItr;

/*!
 @brief   A const iterator for the SystemList.
 */
typedef SystemList::const_iterator SystemListCItr;

}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __SYSTEM_H__
