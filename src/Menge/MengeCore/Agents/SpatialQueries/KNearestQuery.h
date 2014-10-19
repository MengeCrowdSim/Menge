/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

/*!
 *  @file       KNearestQuery.h
 *  @brief      Spatial Query which stores the k-nearest agents and obstacles
 */
#ifndef __K_NEAREST_QUERY_H__
#define	__K_NEAREST_QUERY_H__

// UTILS
#include "ProximityQuery.h"
#include "SpatialQueryStructs.h"
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
			KNearestQuery() : ProximityQuery(), _maxAgentResultDistance(0.0f), _maxObstacleResultDistance(0.0f), _queryPoint(0,0), _initialRange(100){};


				/*!
			 *  @brief      filters an agent and determines if it is within the set of k-nearest neighbors
			 *
			 *  @param      agent     the agent to consider
			 *  @param      distSq	  the distance to the agent
			 */
			void filterAgent(const BaseAgent * agent, float distSq);

			/*!
			 *  @brief      filters an obstacle and determines if it is within the set of k-nearest neighbors
			 *
			 *  @param      obstacle     the obstacle to consider
			 *  @param      distSq  the distance to the obstacle
			 */
			void filterObstacle(const Obstacle * obstacle, float distSq);

			/*!
			 *  @brief      sets the max number of agent results for this query to store
			 *
			 *  @param      results         the number of agent results to store
			 */
			void setMaxAgentResults(size_t results){_maxAgentResults = results;}

			/*!
			 *  @brief      sets the max number of obstacle results for this query to store
			 *
			 *  @param      results         the number of obstacle results to store
			 */
			void setMaxObstacleResults(size_t results){_maxObstacleResults = results;}


			/*!
			 *  @brief      sets the query point
			 *
			 *  @param     point       the query point to be stored
			 */
			void setQueryPoint(Vector2 point){_queryPoint = point;}
	        
			/*!
			 *  @brief      sets the query point
			 *
			 *  @param     range       the squared range to search for nearby candidates
			 */
			void setQueryRangeSq(float range){_initialRange = range;}
	        
	        
			/*!
			 *  @brief      gets the max number of agent results for this query to store
			 *
			 *  @returns      the number of agent results to store
			 */
			size_t maxAgentResults(){ return _maxAgentResults;}

			/*!
			 *  @brief      gets the max number of obstacle results for this query to store
			 *
			 *  @returns      the number of obstacle results to store
			 */
			size_t maxObstacleResults(){ return _maxObstacleResults;}
	        
			/*!
			 *  @brief      gets the number of agent results stored currently
			 *
			 *  @returns         the number of agent results in the query
			 */
			size_t agentResultCount(){ return _agentResults.size();}
	        
			/*!
			 *  @brief      gets the number of obstacle results stored currently
			 *
			 *  @returns         the number of obstacle results in the query
			 */
			size_t obstacleResultCount(){ return _obstacleResults.size();}

			
			/*!
			 *  @brief     clears the result vectors. Resets the query
			 */
			void startQuery();

			/*!
			 *  @brief      gets the start point for the query
			 *
			 *   @returns    the query point for this query
			 */
			virtual Vector2 getQueryPoint() { return _queryPoint;};

			/*!
			 *  @brief      gets the ith agent result
			 *
			 *  @param      i		the index of the agent to get
			 *  @returns    the result pair in question
			 */
			NearAgent getAgentResult(size_t i) { return _agentResults[i];};

			/*!
			 *  @brief      gets the ith obstacle result
			 *
			 *  @param      i		the index of the obstacle to get
			 *  @returns    the result pair in question
			 */
			NearObstacle getObstacleResult(size_t i) { return _obstacleResults[i];};

			/*!
			 *  @brief      updates the max agent query range if conditions inside the query are met
			 *              typically, we don't shrink the query range until the result set is full
			 *
			 *  @returns    the new query distance. Typically this is the initial value.d
			 */
			virtual float getMaxAgentRange();

			/*!
			 *  @brief      updates the max query obstacle range if conditions inside the query are met
			 *              typically, we don't shrink the query range until the result set is full
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
			 *  @brief   the max distance to an agent result. Useful for informing the spatial query
			 *           to stop searching further
			 */
			float _maxAgentResultDistance;

			/*!
			 *  @brief   the max distance to an obstacle result. Useful for informing the spatial query
			 *           to stop searching further
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
			Vector2 _queryPoint;

			/*!
			 *    @brief  the base max range of queries
			 */
			float _initialRange;
		};
	}	// namespace Agents
}	// namespace Menge
#endif
