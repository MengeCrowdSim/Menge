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
 *  @file       ProximityQuery.h
 *  @brief      The base class for all objects which actually perform filtering and store results
 *				from spatial queries
 */
#ifndef __PROXIMITY_QUERY_H__
#define	__PROXIMITY_QUERY_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Math/Vector2.h"

#include <vector>
#include <exception>

namespace Menge {

	namespace Agents {

		/*
		*    Forward Declaration
		*/
		class BaseAgent;

		/*!
		 *	@brief		The base class for filtering spatial queries according to proximity
		 *
		 *	There are different types of spatial queries: nearest, k-nearest, k-nearest with minimum range, etc.
		 *	The ProximityQuery class works in conjunction with a SpatialQuery implementation to 
		 *	define one of these types of spatial queries.  It serves as a means of directing the
		 *	spatial query implementation in its search and of filtering and caching the results.
		 *
		 *	The ProximityQuery can apply to proximal agents or obstacles.
		 *
		 *	Valid sub-classes must provide implementations for both obstacles and agents.
		 */
		class MENGE_API ProximityQuery {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ProximityQuery(){}

			protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~ProximityQuery() {}

		public:
			/*!
			 *  @brief     Resets the query's state.
			 */
			virtual void startQuery() = 0;

			/*!
			 *  @brief      gets the start point for the query ???
			 *
			 *  @returns    the query point for this Query
			 */
			virtual Math::Vector2 getQueryPoint() = 0;

			/*!
			 *  @brief      updates the max agent query range if conditions inside the Query are
			 *				met typically, we don't shrink the query range until the result set is
			 *				full.
			 *
			 *  @returns	the current max query range
			 */
			virtual float getMaxAgentRange() = 0;

			/*!
			 *  @brief      updates the max query obstacle range if conditions inside the Query are
			 *				met typically, we don't shrink the query range until the result set is
			 *				full.
			 *
			 *  @returns	the current max query range
			 */
			virtual float getMaxObstacleRange() = 0;

			/*!
			 *  @brief      filters an agent and determines if it needs to be in the result set
			 *
			 *              CHILDREN MUST OVERWRITE THIS
			 *  @param      agent     the agent to consider
			 *  @param      distSq  the distance to the agent
			 */
			virtual void filterAgent(const BaseAgent * agent, float distSq) = 0;

			/*!
			 *  @brief      filters an obstacle and determines if it needs to be in the result set
			 *
			 *              CHILDREN MUST OVERWRITE THIS
			 *  @param      obstacle     the obstacle to consider
			 *  @param      distSq  the distance to the obstacle
			 */
			virtual void filterObstacle(const Obstacle * obstacle, float distSq) = 0;
		};
	}	// namespace Agents
}	// namespace Menge
#endif	// __PROXIMITY_QUERY_H__
