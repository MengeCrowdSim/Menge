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

#ifndef __VIS_OBSTACLE_H__
#define __VIS_OBSTACLE_H__

/*!
 @file    VisObstacle.h
 @brief   The node for visualizing a simulation obstacle.
 */
#include "MengeCore/Math/Vector3.h"
#include "MengeVis/SceneGraph/GLNode.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {
namespace Runtime {
/*!
 @brief   A simple class for drawing a simulation obstacle (line segment).

 The obstacle is drawn with lines in 3D space.
 */
class MENGEVIS_API VisObstacle : public SceneGraph::GLNode {
 public:
  /*!
   @brief   Constructor.

   @param   p0    The first end point of the line segment.
   @param   p1    The second end point of the line segment.
   */
  VisObstacle(const Menge::Math::Vector3& p0, const Menge::Math::Vector3& p1);

  /*!
   @brief   Draws the object into the OpenGL context.

   @param   select    Determines if the object is being drawn in a selection context (true) or
                      visualization false).
   */
  void drawGL(bool select = false);

 protected:
  /*!
   @brief   The line segment's first end point.
   */
  Menge::Math::Vector3 _p0;

  /*!
   @brief   The line segment's second end point.
   */
  Menge::Math::Vector3 _p1;
};
}  // namespace Runtime
}  // namespace MengeVis
#endif  //__VIS_OBSTACLE_H__
