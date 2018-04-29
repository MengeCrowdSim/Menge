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
 @file       KNearestQuery.h
 @brief      Spatial Query which stores the k-nearest agents and obstacles
 */
#ifndef __K_NEAREST_QUERY_H__
#define __K_NEAREST_QUERY_H__

// UTILS
#include "MengeCore/Agents/SpatialQueries/ProximityQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryStructs.h"
#include "MengeCore/Math/Vector2.h"

#include <vector>

namespace Menge {

namespace Agents {

/*!
 @brief    The K-Nearest query class

 A spatial query which returns the k nearest agents and obstacles in the spatial query. The query
 doesn't know where the point it, it is given distances in this case, K is set to the
 maxAgentResults and maxObstacleResults.

 */
class KNearestQuery : public ProximityQuery {
 public:
  /*!
   @brief    default Constructor.
   */
  KNearestQuery()
      : ProximityQuery(),
        _maxAgentResultDistance(0.0f),
        _maxObstacleResultDistance(0.0f),
        _queryPoint(0, 0),
        _initialRange(100) {}

  /*!
   @brief      Filters an agent and determines if it is within the set of k-nearest neighbors.

   @param      agent    The agent to consider
   @param      distSq   The squared distance to the agent
   */
  void filterAgent(const BaseAgent* agent, float distSq);

  /*!
   @brief      Filters an obstacle and determines if it is within the set of k-nearest neighbors.

   @param      obstacle   The obstacle to consider
   @param      distSq     The squared distance to the obstacle
   */
  void filterObstacle(const Obstacle* obstacle, float distSq);

  /*!
   @brief      Sets the max number of agent results for this query to store.

   @param      results    The number of agent results to store.
   */
  void setMaxAgentResults(size_t results) { _maxAgentResults = results; }

  /*!
   @brief      Sets the max number of obstacle results for this query to store.

   @param      results    The number of obstacle results to store.
   */
  void setMaxObstacleResults(size_t results) { _maxObstacleResults = results; }

  /*!
   @brief     Sets the query point.

   @param     point     The query point to be stored.
   */
  void setQueryPoint(Math::Vector2 point) { _queryPoint = point; }

  /*!
   @brief     Sets the query point.

   @param     range     The squared range to search for nearby candidates.
   */
  void setQueryRangeSq(float range) { _initialRange = range; }

  /*!
   @brief     Gets the max number of agent results for this query to store.

   @returns   The number of agent results to store.
   */
  size_t maxAgentResults() { return _maxAgentResults; }

  /*!
   @brief     Gets the max number of obstacle results for this query to store.

   @returns   The number of obstacle results to store.
   */
  size_t maxObstacleResults() { return _maxObstacleResults; }

  /*!
   @brief     Gets the number of agent results stored currently.

   @returns   The number of agent results in the query.
   */
  size_t agentResultCount() { return _agentResults.size(); }

  /*!
   @brief     Gets the number of obstacle results stored currently.

   @returns   The number of obstacle results in the query.
   */
  size_t obstacleResultCount() { return _obstacleResults.size(); }

  /*!
   @brief     Clears the result vectors. Resets the query.
   */
  void startQuery();

  /*!
   @brief      Gets the start point for the query.

   @returns    The query point for this query.
   */
  virtual Math::Vector2 getQueryPoint() { return _queryPoint; }

  /*!
   @brief      Gets the ith agent result.

   @param      i    The index of the agent to get.
   @returns    The result pair in question.
   */
  NearAgent getAgentResult(size_t i) { return _agentResults[i]; }

  /*!
   @brief      Gets the ith obstacle result.

   @param      i    The index of the obstacle to get.
   @returns    The result pair in question.
   */
  NearObstacle getObstacleResult(size_t i) { return _obstacleResults[i]; }

  /*!
   @brief     Updates the max agent query range if conditions inside the query are met typically,
              we don't shrink the query range until the result set is full.

   @returns   The new query distance. Typically this is the initial value.
   */
  virtual float getMaxAgentRange();

  /*!
   @brief     Updates the max query obstacle range if conditions inside the query are met typically,
              we don't shrink the query range until the result set is full.

   @returns   The new query distance. Typically this is the initial value.
   */
  virtual float getMaxObstacleRange() { return _initialRange; };

 protected:
  /*!
   @brief   The max number of agent results to store.
   */
  size_t _maxAgentResults;

  /*!
   @brief   The max number of obstacle results to store.
   */
  size_t _maxObstacleResults;

  /*!
   @brief   The max distance to an agent result. Useful for informing the spatial query to stop
            searching further.
   */
  float _maxAgentResultDistance;

  /*!
   @brief   The max distance to an obstacle result. Useful for informing the spatial query to stop
            searching further.
   */
  float _maxObstacleResultDistance;

  /*!
   @brief   Pairs of agents and distance to agent.
   */
  std::vector<NearAgent> _agentResults;

  /*!
   @brief   Pairs of obstacles and distance to objstacle.
   */
  std::vector<NearObstacle> _obstacleResults;

  /*!
   @brief   The start point for the query.
   */
  Math::Vector2 _queryPoint;

  /*!
   @brief   The base max range of queries.
   */
  float _initialRange;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __K_NEAREST_QUERY_H__
