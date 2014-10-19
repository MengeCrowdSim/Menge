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

#ifndef __AGENT_KD_TREE_H__
#define __AGENT_KD_TREE_H__

/*!
 *  @file       AgentKDTree.h
 *  @brief      Contains the definition of the AgentKDTree class.
 *				Performs spatial queries for agents
 */

// STL
#include "CoreConfig.h"
#include <vector>
#include <cstddef>
#include "SpatialQueries/ProximityQuery.h"


namespace Menge {

	/*!
	 *	@namespace	Agents
	 *	@brief		The namespace that contains the basic simulation mechanisms.
	 */
	namespace Agents {

		// FORWARD DECLARATIONS
		class BaseAgent;

		// TODO: Adapt this so that the number of agents can be changed -- i.e. removing and
		//	introducing new agents in the simulation

		/*!
		 *	@brief		A <i>k</i>d-tree for performing nearest-neighbor searches.
		 *
		 *				The agents are partitioned according to a greedy partitioning algorithm.
		 */
		class MENGE_API AgentKDTree {
		private:
			/*!
			 *  @brief      A node in the <i>k</i>d-tree -- a group of one or more agents and their extents.
			 */
			struct AgentTreeNode {
				/*!
				*  @brief      The beginning node number.
				*/
				size_t _begin;

				/*!
				*  @brief      The ending node number.
				*/
				size_t _end;

				/*!
				*  @brief      The left node number.
				*/
				size_t _left;

				/*!
				*  @brief      The maximum x-coordinate.
				*/
				float _maxX;

				/*!
				*  @brief      The maximum y-coordinate.
				*/
				float _maxY;

				/*!
				*  @brief      The minimum x-coordinate.
				*/
				float _minX;

				/*!
				*  @brief      The minimum y-coordinate.
				*/
				float _minY;

				/*!
				*  @brief      The right node number.
				*/
				size_t _right;
			};
		public:
			/*!
			 *  @brief      Constructs an Agent <i>k</i>d-tree instance.
			 */
			explicit AgentKDTree();

			/*!
			 *  @brief      Define the set of agents on which <i>k</i>d-tree will query.
			 */
			void setAgents( const std::vector< BaseAgent * > & agents );	

			/*!
			 *  @brief      Builds a <i>k</i>d-tree on the set of agents.
			 */
			void buildTree();	

			/*!
			 *  @brief      gets agents within a range, and passes them to the supplied filter
			 *  @param      filter          a pointer for the filter object
			 */
			void agentQuery( ProximityQuery *filter) const;

		protected:
			/*!
			 *  @brief      Does the full work of constructing the <i>k</i>d-tree.
			 *
			 *	@param		begin		The index of the first agent in the region of the tree.
			 *	@param		end			The index of the last (just outside).  So, the 
			 *							agents in this branch are in the interval [begin, end)
			 *	@param		node		The index of the node to build.
			 */
			void buildTreeRecursive(size_t begin, size_t end, size_t node);

			/*!
			 *  @brief      Computes the agent neighbors of the specified agent by doing a
			 *				recursive search.
			 *
			 *  @param      filter          spatial query filter to use
			 *  @param      pt              the start point for the query
			 *  @param      rangeSq         The squared range around the agent.
			 *	@param		node			The current node to search in.
			 */
			void queryTreeRecursive( ProximityQuery *filter, Vector2 pt, float& rangeSq, size_t node) const;

			/*!
			 *	@brief		The agents being partitioned by the <i>k</i>d-tree.
			 */
			std::vector< const BaseAgent * > _agents;

			/*!
			 *	@brief		The tree structure.
			 */
			std::vector<AgentTreeNode> _tree;

			/*!
			 *	@brief		The maximum number of agents allowed in a tree leaf node.
			 */
			static const size_t MAX_LEAF_SIZE = 10;
		};
	}		// namespace Agents
}	// namespace Menge

#endif	// __AGENT_KD_TREE_H__
