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
 @file      SpatialQueryKDTree.h
 @brief     A spatial query object based on Jur van den Berg's <i>k</i>d-tree as defined in the
            RVO2 library (http://gamma.cs.unc.edu/RVO2).

 This spatial query implementation uses a <i>k</i>d-tree for agents and a bsp-tree for obstacles.
 The BSP Tree changes the input obstacle set.  Single line segments can end up cut into two or more
 pieces. This *may* have a deleterious effect on simulation.
 */

#ifndef __SPATIAL_QUERY_KD_TREE_H__
#define __SPATIAL_QUERY_KD_TREE_H__

#include "MengeCore/Agents/SpatialQueries/AgentKDTree.h"
#include "MengeCore/Agents/SpatialQueries/ObstacleKDTree.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryFactory.h"

namespace Menge {

namespace Agents {

/*!
 @brief    Spatial query object.
 
 Used to determine obstacles and agents near an agent -- based on a <i>k</i>d-tree.
 */
class MENGE_API BergKDTree : public SpatialQuery {
 public:
  /*!
   @brief      Constructor.
   */
  explicit BergKDTree() : SpatialQuery() {}

  // Agent operations

  /*!
   @brief      Define the set of agents on which <i>k</i>d-tree will query.

   @param    agents    The set of agents in the simulator to be managed.
   */
  virtual void setAgents(const std::vector<BaseAgent*>& agents) { _agentTree.setAgents(agents); }

  /*!
   @brief      Allows the spatial query structure to update its knowledge of the agent positions.
   */
  virtual void updateAgents() { _agentTree.buildTree(); };

  /*!
   @brief      Performs an agent based proximity query.

   @param      query    A pointer to the proximity query to be performed.
   */
  virtual void agentQuery(ProximityQuery* query) const { _agentTree.agentQuery(query); }

  // Obstacle operations

  /*!
   @brief      Do the necessary pre-computation to support obstacle definitions.
   */
  virtual void processObstacles() { _obstTree.buildTree(_obstacles); }

  /*!
   @brief      Perform an obstacle based proximity query.

   @param      query    A pointer to the proximity query to be performed.

   */
  virtual void obstacleQuery(ProximityQuery* query) const { _obstTree.obstacleQuery(query); }

  /*! @brief  Implementation of SpatialQuery::linkIsTraversible().  */
  bool linkIsTraversible(const Math::Vector2& q1, const Vector2& q2, float radius) const override {
    return _obstTree.linkIsTraversible(q1, q2, radius);
  }

  /*!
   @brief      Queries the visibility between two points within a specified radius.

   @param      q1        The first point between which visibility is to be tested.
   @param      q2        The second point between which visibility is to be tested.
   @param      radius    The radius within which visibility is to be tested.
   @returns    True if q1 and q2 are mutually visible within the radius.
   */
  virtual bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const {
    return _obstTree.queryVisibility(q1, q2, radius);
  }

 protected:
  /*!
   @brief      A kd-tree for the agent queries.
   */
  AgentKDTree _agentTree;

  /*!
   @brief      A kd-tree for the obstacle queries.
   */
  ObstacleKDTree _obstTree;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the BergKDTree.
 */
class MENGE_API BergKDTreeFactory : public SpatialQueryFactory {
 public:
  /*!
   @brief    The name of the spatial query implemenation.

   The spatial query's name must be unique among all registered spatial query components. Each
   spatial query factory must override this function.

   @returns  A string containing the unique spatial query name.
   */
  virtual const char* name() const { return "kd-tree"; }

  /*!
   @brief    A description of the spatial query.

   Each spatial query factory must override this function.

   @returns  A string containing the spatial query description.
   */
  virtual const char* description() const {
    return "Performs spatial queries by creating a kd-tree on the agents and a bsp "
           "tree on the obstacles.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's spatial query implementation.

   All SpatialQueryFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding spatial query type. The various field values of the instance will be set in
   a subsequent call to SpatialQueryFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated SpatialQuery class.
   */
  SpatialQuery* instance() const { return new BergKDTree(); }
};
}  // namespace Agents
}  // namespace Menge
#endif  //__SPATIAL_QUERY_KD_TREE_H__
