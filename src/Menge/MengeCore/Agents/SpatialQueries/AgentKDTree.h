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

#ifndef __AGENT_KD_TREE_H__
#define __AGENT_KD_TREE_H__

/*!
 @file       AgentKDTree.h
 @brief      Contains the definition of the AgentKDTree class. Performs spatial queries for agents.
 */

// STL
#include "MengeCore/Agents/SpatialQueries/ProximityQuery.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"

#include <cstddef>
#include <vector>

namespace Menge {

/*!
 @namespace  Menge::Agents
 @brief    The namespace that contains the basic simulation mechanisms.
 */
namespace Agents {

// FORWARD DECLARATIONS
class BaseAgent;

// TODO: Adapt this so that the number of agents can be changed -- i.e. removing and
//  introducing new agents in the simulation

/*!
 @brief    A <i>k</i>d-tree for performing nearest-neighbor searches.

 The agents are partitioned according to a greedy partitioning algorithm.
 */
class MENGE_API AgentKDTree {
 private:
  /*!
   @brief      A node in the <i>k</i>d-tree -- a group of one or more agents and their extents.
   */
  struct AgentTreeNode {
    /*!
    @brief      The beginning node number.
    */
    size_t _begin;

    /*!
    @brief      The ending node number.
    */
    size_t _end;

    /*!
    @brief      The left node number.
    */
    size_t _left;

    /*!
    @brief      The maximum x-coordinate.
    */
    float _maxX;

    /*!
    @brief      The maximum y-coordinate.
    */
    float _maxY;

    /*!
    @brief      The minimum x-coordinate.
    */
    float _minX;

    /*!
    @brief      The minimum y-coordinate.
    */
    float _minY;

    /*!
    @brief      The right node number.
    */
    size_t _right;
  };

 public:
  /*!
   @brief      Constructs an Agent <i>k</i>d-tree instance.
   */
  explicit AgentKDTree();

  /*!
   @brief      Define the set of agents on which <i>k</i>d-tree will query.
   */
  void setAgents(const std::vector<BaseAgent*>& agents);

  /*!
   @brief      Builds a <i>k</i>d-tree on the set of agents.
   */
  void buildTree();

  /*!
   @brief      Gets agents within a range, and passes them to the supplied filter.
   @param      filter          a pointer for the filter object
   */
  void agentQuery(ProximityQuery* filter) const;

 protected:
  /*!
   @brief      Does the full work of constructing the <i>k</i>d-tree.

   @param    begin  The index of the first agent in the region of the tree.
   @param    end    The index of the last (just outside).  So, the agents in this branch are in the
                    interval [begin, end)
   @param    node   The index of the node to build.
   */
  void buildTreeRecursive(size_t begin, size_t end, size_t node);

  /*!
   @brief      Computes the agent neighbors of the specified agent by doing a recursive search.

   @param   filter          The spatial query filter to use.
   @param   pt              The start point for the query.
   @param   rangeSq         The squared range around the agent.
   @param   node            The current node to search in.
   */
  void queryTreeRecursive(ProximityQuery* filter, Math::Vector2 pt, float& rangeSq,
                          size_t node) const;

  /*!
   @brief    The agents being partitioned by the <i>k</i>d-tree.
   */
  std::vector<const BaseAgent*> _agents;

  /*!
   @brief    The tree structure.
   */
  std::vector<AgentTreeNode> _tree;

  /*!
   @brief    The maximum number of agents allowed in a tree leaf node.
   */
  static const size_t MAX_LEAF_SIZE = 10;
};

}  // namespace Agents
}  // namespace Menge

#endif  // __AGENT_KD_TREE_H__
