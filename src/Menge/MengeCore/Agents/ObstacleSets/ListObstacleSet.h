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
 *	@file		ListObstacleSet.h
 *	@brief		An abstract class for any obstacle set which will need an _obstacles list. Provides transformations from vertex vectors
 */

#ifndef __LIST_OBSTACLE_SET_H__
#define __LIST_OBSTACLE_SET_H__

#include "mengeCommon.h"
#include "ObstacleSets/ObstacleSet.h"
#include "ObstacleSets/ObstacleSetFactory.h"
#include "ObstacleSets/ObstacleVertexList.h"
#include "Obstacle.h"
#include <vector>

namespace Menge {

	namespace Agents {

		/*!
		 *	@brief		Definition of obstacle set class which produces obstacles based on
		 *				explicit definition in an XML file.
		 */
		class MENGE_API ListObstacleSet : public ObstacleSet {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ListObstacleSet();

		protected:
			/*!
			 *	@brief		Destructor
			 */
			~ListObstacleSet();

		public:
			/*!
			 *	@brief		Reports the number of obstacles in the set
			 *
			 *	@returns	The number of obstacles in this set
			 */
			virtual size_t obstacleCount() { return _obstacles.size(); }

			/*!
			 *	@brief		Get the ith obstacle.
			 *
			 *	@param		i		The index of the requested obstacle.
			 *	@returns	The Obstacle object
			 *	@throws		ObstacleSetException if the index, i, is invalid.
			 */
			virtual Obstacle * getObstacle( size_t i );
			
			/*!
			 *	@brief		Adds an obstacle  to the generator
			 *
			 *	@param		o		The Obstacle Object to be added
			 *	@returns	true if the obstacle was added successfully
			 *	@throws		ObstacleSetException if the obstacle is malformed
			 */
			bool addObstacle(Obstacle * o);

			/*!
			 *	@brief		Adds an obstacle  to the generator from a list of vertices
			 *
			 *	@param		o 		ObstacleVertexList struct representing the incoming obstacle
			 *	@returns	true if the obstacle was added successfully
			 *	@throws		ObstacleSetException if the obstacle is malformed
			 */
			bool addObstacle(ObstacleVertexList o);
			
			friend class ExplicitObstacleSetFactory;

		protected:
			/*!
			 *	@brief		The obstacles in an internal list
			 */
			std::vector< Obstacle *>	_obstacles;
		};

		
	}	// namespace Agents
}	// namespace Menge
#endif	// __EXPLICIT_OBSTACLE_SET_H__