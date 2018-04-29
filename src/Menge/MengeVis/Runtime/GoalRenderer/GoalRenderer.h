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
#ifndef __GOAL_RENDERER_H__
#define __GOAL_RENDERER_H__

#include "MengeVis/VisConfig.h"

#include <string>

// Forward declaration

namespace Menge {
namespace BFSM {
class Goal;
}
}  // namespace Menge

namespace MengeVis {
namespace Runtime {

/*!
 @namespace   MengeVis::Runtime::GoalVis
 @brief   The namespace that contains context structures for visualizing goal state.
          (@see Menge::BFSM::Goal)
 */
namespace GoalVis {
/*!
 @brief   Class responsible for rendering a goal to the OpenGL context.

 Each unique goal should be associated with a GoalRenderer that knows how to draw it. The
 GoalRenderer should be registered in the GoalRendererDB so that at runtime, the right renderer can
 be associated with the right goal.
 */
class MENGEVIS_API GoalRenderer {
 public:
  /*!
   @brief   Sets the agent for this context.

   This method works in conjunction with the VisElementDatabase. When this visualization element is
   triggered, the database will supply the triggering element.

   @param   goal    The goal to interact with.
   */
  virtual void setElement(const Menge::BFSM::Goal* goal) { _goal = goal; }

  /*!
   @brief   The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const { return "default"; }

  /*!
   @brief   The method to draw the associated geometry to the 3D context.

   This draws the region as a shaded region with a highlighted outline. It will call
   doDrawGeometry() twice -- once for the filled region and once for the outline.
   */
  void drawGL() const;

 protected:
  /*!
   @brief   The method for handling the details of a particular goal.

   This is called twice by drawGL(). If there is an expensive operation, it would be best to be
   cached because it will be called twice every time a goal is drawn.
   */
  virtual void doDrawGeometry() const;

  /*! 
   @brief   The goal to visualize.
   */
  const Menge::BFSM::Goal* _goal;
};
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __GOAL_RENDERER_H__
