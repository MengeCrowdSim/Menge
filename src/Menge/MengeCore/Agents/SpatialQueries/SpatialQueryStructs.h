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
 @file       SpatialQueryStructs.h
 @brief      Structs for storing results from spatial queries
 */

#ifndef __SPATIAL_QUERY_STRUCTS_H__
#define __SPATIAL_QUERY_STRUCTS_H__

#include "MengeCore/CoreConfig.h"

#include <exception>
#include <vector>

namespace Menge {

namespace Agents {

// forward declaration
class BaseAgent;
class Obstacle;

/*!
 @brief  The definition of a proximal agent -- an agent "near" a point.
 */
struct NearAgent {
  /*!
   @brief    The squared distance from the query point to the agent.
   */
  float distanceSquared;

  /*!
   @brief    A pointer to the agent at the indicated distance.
   */
  const BaseAgent* agent;

  /*!
   @brief    Constructor

   @param    sqdDist      The squared distance of the agent from a test point.
   @param    agt          The agent.
   */
  NearAgent(float sqdDist, const BaseAgent* agt) : distanceSquared(sqdDist), agent(agt){};
};

/*!
 @brief struct to store obstacle proximity query results.
 */
struct NearObstacle {
  /*!
   @brief    The squared distance from the query point to the obstacle.
   */
  float distanceSquared;

  /*!
   @brief    A pointer to the obstacle at the indicated distance.
   */
  const Obstacle* obstacle;

  /*!
   @brief    Constructor

   @param    sqdDist    The squared distance of the obstacle from a test point.
   @param    obs        The obstacle.
   */
  NearObstacle(float sqdDist, const Obstacle* obs) : distanceSquared(sqdDist), obstacle(obs){};
};

}  // namespace Agents
}  // namespace Menge
#endif  // __SPATIAL_QUERY_STRUCTS_H__
