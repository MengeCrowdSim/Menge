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
 @file    ObstacleVertexList.h
 @brief   Defines a struct for passing around obstacles as lists of vertices.
 */

#ifndef __OBSTACLE_VERTEX_LIST__
#define __OBSTACLE_VERTEX_LIST__

#include "MengeCore/mengeCommon.h"

#include <vector>

namespace Menge {

namespace Agents {
/*!
 @brief   A strcuture for passing vertices and closed tags for obstacles
 */
struct ObstacleVertexList {
  /*!
   @brief   The list of vertices for the obstacle.
   */
  std::vector<Math::Vector2> vertices;

  /*!
   @brief   Indicates if the obstacle is closed (true) or open (false).
   */
  bool closed;
};
}  // namespace Agents
}  // namespace Menge

#endif  // __OBSTACLE_VERTEX_LIST__
