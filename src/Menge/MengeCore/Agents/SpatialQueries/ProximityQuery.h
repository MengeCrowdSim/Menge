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
 @file      ProximityQuery.h
 @brief     The base class for all objects which actually perform filtering and store results from
            spatial queries
 */
#ifndef __PROXIMITY_QUERY_H__
#define __PROXIMITY_QUERY_H__

#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"

#include <exception>
#include <vector>

namespace Menge {

namespace Agents {

// Forward Declaration
class BaseAgent;

/*!
 @brief    The base class for filtering spatial queries according to proximity

 There are different types of spatial queries: nearest, k-nearest, k-nearest with minimum range,
 etc. The ProximityQuery class works in conjunction with a SpatialQuery implementation to define one
 of these types of spatial queries.  It serves as a means of directing the spatial query
 implementation in its search and of filtering and caching the results.

 The ProximityQuery can apply to proximal agents or obstacles.

 Valid sub-classes must provide implementations for both obstacles and agents.
 */
class MENGE_API ProximityQuery {
 public:
  /*!
   @brief    Default constructor.
   */
  ProximityQuery() {}

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~ProximityQuery() {}

 public:
  /*!
   @brief     Resets the query's state.
   */
  virtual void startQuery() = 0;

  /*!
   @brief      Gets the start point for the query.

   @returns    The query point for this Query.
   */
  virtual Math::Vector2 getQueryPoint() = 0;

  /*!
   @brief      Updates the max agent query range if conditions inside the Query are met typically,
               we don't shrink the query range until the result set is full.

   @returns  the current max query range
   */
  virtual float getMaxAgentRange() = 0;

  /*!
   @brief     Updates the max query obstacle range if conditions inside the Query are met
              typically, we don't shrink the query range until the result set is full.

   @returns    The current max query range.
   */
  virtual float getMaxObstacleRange() = 0;

  /*!
   @brief      Filters an agent and determines if it needs to be in the result set.

   @param      agent   The agent to consider.
   @param      distSq  The distance to the agent.
   */
  virtual void filterAgent(const BaseAgent* agent, float distSq) = 0;

  /*!
   @brief      filters an obstacle and determines if it needs to be in the result set

   @param      obstacle     The obstacle to consider.
   @param      distSq       The distance to the obstacle.
   */
  virtual void filterObstacle(const Obstacle* obstacle, float distSq) = 0;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __PROXIMITY_QUERY_H__
