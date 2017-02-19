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
 *  @file       SpatialQueryStructs.h
 *  @brief      Structs for storing results from spatial queries
 */

#ifndef __SPATIAL_QUERY_STRUCTS_H__
#define	__SPATIAL_QUERY_STRUCTS_H__

#include "MengeCore/CoreConfig.h"

#include <vector>
#include <exception>

namespace Menge {

	namespace Agents {

		//forward declaration
		class BaseAgent;
		class Obstacle;

		/*!
		 * @brief	The definition of a proximal agent -- an agent "near" a point.
		 */
		struct  NearAgent {
			/*!
			 * @brief		The squared distance from the query point to the agent.
			 */
			float distanceSquared;

			/*!
			 * @brief		A pointer to the agent at the indicated distance.
			 */
			const BaseAgent * agent;

			/*!
			 * @brief		Constructor
			 *
			 * @param		sqdDist			The squared distance of the agent from a test point.
			 * @param		agt				The agent.
			 */
			NearAgent(float sqdDist, const BaseAgent * agt):distanceSquared(sqdDist), agent(agt){};
		};

		/*!
		 * @brief struct to store obstacle proximity query results.
		 */
		struct  NearObstacle{
			/*!
			 * @brief		The squared distance from the query point to the obstacle.
			 */
			float distanceSquared;

			/*!
			 * @brief		A pointer to the obstacle at the indicated distance.
			 */
			const Obstacle * obstacle;

			/*!
			 * @brief		Constructor
			 *
			 * @param		sqdDist		The squared distance of the obstacle from a test point.
			 * @param		obs			The obstacle.
			 */
			NearObstacle(float sqdDist, const Obstacle * obs):distanceSquared(sqdDist), obstacle(obs){};
		};

	}	// namespace Agents
}	// namespace Menge
#endif	// __SPATIAL_QUERY_STRUCTS_H__
