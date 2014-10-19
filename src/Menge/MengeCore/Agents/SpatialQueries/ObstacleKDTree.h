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

#ifndef __OBSTACLE_KD_TREE_H__
#define __OBSTACLE_KD_TREE_H__

/*!
 *  @file       ObstacleKDTree.h
 *  @brief      Contains the definition of the ObstacleKDTree class.
 *				Performs spatial queries for Obstacles
 */

// STL
#include <vector>

// PedModels
#include "CoreConfig.h"
#include "Obstacle.h"
#include "SpatialQueries/ProximityQuery.h"

namespace Menge {

	namespace Agents {

		// FORWARD DECLARATIONS
		class BaseAgent;

		// Forward declarations

		// TODO: This chops up obstacles for the kd-tree.  I need to be able to reconstruct them
		//		i.e. if a piece is close to the agent, I'd like to be able to provide the full
		//		original line obstacle.

		/*!
		 *  @brief      Defines an obstacle <i>k</i>d-tree node.
		 */
		struct ObstacleTreeNode {
			  /*!
			   *  @brief      The left obstacle tree node.
			   */
			  ObstacleTreeNode* _left;
		      
			  /*!
			   *  @brief      The obstacle number.
			   */
			  const Obstacle* _obstacle;
		      
			  /*!
			   *  @brief      The right obstacle tree node.
			   */
			  ObstacleTreeNode* _right;
			};

		/*!
		 *  @brief      Defines an obstacle <i>k</i>d-tree.
		 *
		 *	This structure will create a static <i>k</i>d-tree node on the provided
		 *	set of obstacles.  It will modify the obstacles in that some obstacles
		 *	may be cut.
		 */
		class MENGE_API ObstacleKDTree {
		public:
			/*!
			 *  @brief      Constructs an Obstacle <i>k</i>d-tree instance.
			 */
			explicit ObstacleKDTree();
			
			/*!
			 *  @brief      Destroys this kd-tree instance.
			 */
			~ObstacleKDTree();

			/*!
			 *  @brief      Builds an obstacle <i>k</i>d-tree on the given set of obstacles.
			 */
			void buildTree( const std::vector< Obstacle * > obstacles );	

			/*!
			 *  @brief      Computes the obstacles within range square of a point
			 *  @param      query          a pointer for the query to be performed
			
			 */
			void obstacleQuery( ProximityQuery *query) const;

			/*!
			 *  @brief      Queries the visibility between two points within a
			 *              specified radius.
			 *
			 *  @param      q1              The first point between which visibility is
			 *                              to be tested.
			 *  @param      q2              The second point between which visibility is
			 *                              to be tested.
			 *  @param      radius          The radius within which visibility is to be
			 *                              tested.
			 *  @returns    True if q1 and q2 are mutually visible within the radius;
			 *              false otherwise.
			 */
			bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const;

		protected:
			/*!
			 *  @brief      Does the full work of constructing the <i>k</i>d-tree.
			 *
			 *	@param		obstacles		The set of obstacles to construct this tree around
			 *	@returns	The root of the ObstacleKDTree for this set of obstacles
			 */
			ObstacleTreeNode* buildTreeRecursive( const std::vector<Obstacle*>& obstacles );

			/*!
			 *  @brief      Computes the obstacle neighbors of the specified point by doing a
			 *				recursive search.
			 *
			 *  @param      query           a pointer to the query being performed.
			 *	@param		pt				the starting point from the query
			 *  @param      rangeSq         The squared range around the agent.
			 *  @param      node	        The current node in the obstacle tree
			 
			 */
			void queryTreeRecursive( ProximityQuery *query, Vector2 pt, float& rangeSq, const ObstacleTreeNode* node) const;

			/*!
			 *	@brief		Perform the work, recursively, to determine if q1 can see q2, w.r.t. the obstacles.
			 *
			 *	@param		q1				The originating position.
			 *	@param		q2				The target position.
			 *	@param		radius			The radius within which visibility is to be tested.
			 *	@param		node			The root of the tree to recursively search.
			 *	@returns	True if q1 and q2 are mutually visible within the radius, false otherwise.
			 */
			bool queryVisibilityRecursive(const Vector2& q1, const Vector2& q2,
										  float radius, 
										  const ObstacleTreeNode* node) const;

			/*!
			 *	@brief			Recursively deletes the obstacle tree.
			 */
			void deleteTree();

			/*!
			 *	@brief			Recursively deletes an obstacle sub-tree.
			 *	@param			node		The root of the tree to delete.
			 */
			void deleteSubTree( ObstacleTreeNode * node );

			/*!
			 *	@brief			The set of obstacles managed by this query structure.
			 *	
			 *	This is *not* necessarily the same as the obstacles assigned.
			 *	The set of obstacles can change as some obstacles may be sub-divided
			 *	during the spatial decomposition.  This needs to be corrected.
			 */
			std::vector<Obstacle*> _obstacles;

			/*!
			 *	@brief			The query tree root.
			 */
			ObstacleTreeNode* _tree;
			
			/*!
			 *	@brief			The maximum number of obstacles allowed in a tree leaf node.
			 */
			static const size_t MAX_LEAF_SIZE = 10;
		};
	}	// namespace Agents
}	// namespace Menge

#endif	 // __OBSTACLE_KD_TREE_H__
