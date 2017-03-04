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
 *  @file       KNearestQuery.h
 *  @brief      Spatial Query which stores the k-nearest agents and obstacles
 */
#ifndef __K_NEAREST_QUERY_H__
#define	__K_NEAREST_QUERY_H__

// UTILS
#include "MengeCore/Agents/SpatialQueries/ProximityQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryStructs.h"
#include "MengeCore/Math/Vector2.h"

#include <vector>

namespace Menge {

	namespace Agents {

		/*!
		 *	@brief		The K-Nearest query class
		 *
		 *	A spatial query which returns the k nearest agents and obstacles in the spatial query
		 *      The query doesn't know where the point it, it is given distances
		 *      in this case, K is set to the maxAgentResults and maxObstacleResults
		 *     
		 */
		class KNearestQuery : public ProximityQuery {
		public:
			/*!
			 *	@brief		default Constructor.
			 */
			KNearestQuery() : ProximityQuery(), _maxAgentResultDistance( 0.0f ),
							  _maxObstacleResultDistance( 0.0f ), _queryPoint( 0, 0 ),
							  _initialRange( 100 ) {}

			/*!
			 *  @brief      Filters an agent and determines if it is within the set of k-nearest
			 *				neighbors.
			 *
			 *  @param      agent     the agent to consider
			 *  @param      distSq	  the squared distance to the agent
			 */
			void filterAgent( const BaseAgent * agent, float distSq );

			/*!
			 *  @brief      Filters an obstacle and determines if it is within the set of k-nearest
			 *				neighbors.
			 *
			 *  @param      obstacle    the obstacle to consider
			 *  @param      distSq		the squared distance to the obstacle
			 */
			void filterObstacle( const Obstacle * obstacle, float distSq );

			/*!
			 *  @brief      sets the max number of agent results for this query to store
			 *
			 *  @param      results         the number of agent results to store
			 */
			void setMaxAgentResults( size_t results ){ _maxAgentResults = results; }

			/*!
			 *  @brief      sets the max number of obstacle results for this query to store
			 *
			 *  @param      results         the number of obstacle results to store
			 */
			void setMaxObstacleResults( size_t results ){ _maxObstacleResults = results; }

			/*!
			 *  @brief      sets the query point
			 *
			 *  @param     point       the query point to be stored
			 */
			void setQueryPoint( Math::Vector2 point ){ _queryPoint = point; }
	        
			/*!
			 *  @brief      sets the query point
			 *
			 *  @param     range       the squared range to search for nearby candidates
			 */
			void setQueryRangeSq( float range ){ _initialRange = range; }
	        
			/*!
			 *  @brief      gets the max number of agent results for this query to store
			 *
			 *  @returns      the number of agent results to store
			 */
			size_t maxAgentResults(){ return _maxAgentResults; }

			/*!
			 *  @brief      gets the max number of obstacle results for this query to store
			 *
			 *  @returns      the number of obstacle results to store
			 */
			size_t maxObstacleResults(){ return _maxObstacleResults; }
	        
			/*!
			 *  @brief      gets the number of agent results stored currently
			 *
			 *  @returns         the number of agent results in the query
			 */
			size_t agentResultCount(){ return _agentResults.size(); }
	        
			/*!
			 *  @brief      gets the number of obstacle results stored currently
			 *
			 *  @returns         the number of obstacle results in the query
			 */
			size_t obstacleResultCount(){ return _obstacleResults.size(); }
			
			/*!
			 *  @brief     clears the result vectors. Resets the query
			 */
			void startQuery();

			/*!
			 *  @brief      gets the start point for the query
			 *
			 *   @returns    the query point for this query
			 */
			virtual Math::Vector2 getQueryPoint() { return _queryPoint; }

			/*!
			 *  @brief      gets the ith agent result
			 *
			 *  @param      i		the index of the agent to get
			 *  @returns    the result pair in question
			 */
			NearAgent getAgentResult( size_t i ) { return _agentResults[i]; }

			/*!
			 *  @brief      gets the ith obstacle result
			 *
			 *  @param      i		the index of the obstacle to get
			 *  @returns    the result pair in question
			 */
			NearObstacle getObstacleResult( size_t i ) { return _obstacleResults[i]; }

			/*!
			 *  @brief      Updates the max agent query range if conditions inside the query are
			 *              met typically, we don't shrink the query range until the result set is
			 *				full.
			 *
			 *  @returns    the new query distance. Typically this is the initial value.d
			 */
			virtual float getMaxAgentRange();

			/*!
			 *  @brief      updates the max query obstacle range if conditions inside the query are
			 *				met typically, we don't shrink the query range until the result set is
			 *				full.
			 *
			 *  @returns    the new query distance. Typically this is the initial value.
			 */
			virtual float getMaxObstacleRange() { return _initialRange; };

		protected:
			/*!
			 *  @brief   the max number of agent results to store
			 */
			size_t _maxAgentResults;
			
			/*!
			 *  @brief   the max number of obstacle results to store
			 */
			size_t _maxObstacleResults;

			/*!
			 *  @brief   the max distance to an agent result. Useful for informing the spatial
			 *           query to stop searching further.
			 */
			float _maxAgentResultDistance;

			/*!
			 *  @brief   the max distance to an obstacle result. Useful for informing the spatial
			 *           query to stop searching further.
			 */
			float _maxObstacleResultDistance;

			/*!
			 *  @brief   vector of pairs. Each pair contains a distance to an agent, and the agent
			 */
			std::vector<NearAgent> _agentResults;
			
			/*!
			 *  @brief   vector of pairs. Each pair contains a distance to an obstacle, and the obstacle
			 */
			std::vector<NearObstacle> _obstacleResults;

			/*!
			 *  @brief   the start point for the query
			 */
			Math::Vector2 _queryPoint;

			/*!
			 *    @brief  the base max range of queries
			 */
			float _initialRange;
		};
	}	// namespace Agents
}	// namespace Menge
#endif	// __K_NEAREST_QUERY_H__
