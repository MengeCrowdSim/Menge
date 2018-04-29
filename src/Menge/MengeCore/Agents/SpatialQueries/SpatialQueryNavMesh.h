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
 @file    SpatialQueryNavMesh.h
 @brief    Definition of a spatial query structure based on a navigation mesh.
 */

#ifndef __SPATIAL_QUERY_NAV_MESH_H__
#define __SPATIAL_QUERY_NAV_MESH_H__

#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryFactory.h"
#include "MengeCore/resources/NavMesh.h"
#include "MengeCore/resources/NavMeshLocalizer.h"

#include <vector>

namespace Menge {

// forward declaration
namespace BFSM {
class Task;
}

namespace Agents {

/*!
 @brief    A spatial query structure based on a navigation mesh.
 */
class MENGE_API NavMeshSpatialQuery : public SpatialQuery {
 public:
  /*!
   @brief    Constructor.
   */
  NavMeshSpatialQuery();

  /*!
   @brief      Define the set of agents on which <i>k</i>d-tree will query.
   */
  virtual void setAgents(const std::vector<BaseAgent*>& agents);

  /*!
   @brief      Allows the spatial query structure to update its knowledge of the agent positions.

   This happens by the NavMeshLocalizer as an FSM task.
   */
  virtual void updateAgents() {}

  /*!
   @brief      Gets agents within a range, and passes them to the supplied filter.
   @param      query    A pointer to the proximity query to be performed.
   */
  virtual void agentQuery(ProximityQuery* query) const;

  /*!
   @brief      Performs an agent based proximity query.

   @param      query      A pointer to the proximity query to be performed.
   @param      rangeSq    The range to search (extracted and mutable).
   */
  virtual void agentQuery(ProximityQuery* query, float& rangeSq) const;

  // Obstacle operations

  /*!
   @brief      Do the necessary pre-computation to support obstacle definitions.
   */
  virtual void processObstacles();

  /*!
   @brief      Performs an obstacle based proximity query.
   @param      query    A pointer to the proximity query to be performed.
   */
  virtual void obstacleQuery(ProximityQuery* query) const;

  /*!
   @brief      Performs an obstacle based proximity query
   @param      query      A pointer to the proximity query to be performed.
   @param      rangeSq    The range to search (extracted and mutable).
   */
  virtual void obstacleQuery(ProximityQuery* query, float rangeSq) const;

  /*! @brief  Implementation of SpatialQuery::linkIsTraversible().  */
  bool linkIsTraversible(const Math::Vector2& q1, const Vector2& q2, float radius) const override;

  /*!
   @brief      Queries the visibility between two points within a specified radius.
   
   @note NOT CURRENTLY IMPLEMENTED!

   @param      q1       The first point between which visibility is to be tested.
   @param      q2       The second point between which visibility is to be tested.
   @param      radius   The radius within which visibility is to be tested.
   @returns    True if q1 and q2 are mutually visible within the radius.
   */
  virtual bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const;

  /*!
   @brief    Sets the navigation mesh localizer pointer.

   @param    nml    The managed pointer to the navigation mesh localizer.
   */
  void setNavMeshLocalizer(const NavMeshLocalizerPtr& nml) { _localizer = nml; }

  /*!
   @brief    Returns a pointer to the nav mesh localizer task.

   @returns    A pointer to the nav mesh localizer task. It is the responsibility of the caller to
              free the memory of the provided task by calling its destroy method.
   */
  virtual BFSM::Task* getTask();

  // TODO: Another version of this would be good where the inputs are an agent, and
  //    a point, and it uses the agent's position and radius.
 protected:
  /*!
   @brief    A vector of pointers to all the agents in the simulation
   */
  std::vector<BaseAgent*> _agents;

  /*!
   @brief    The localizer tied to the given navigation mesh.
   */
  NavMeshLocalizerPtr _localizer;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the NavMeshSpatialQuery.
 */
class MENGE_API NavMeshSpatialQueryFactory : public SpatialQueryFactory {
 public:
  /*!
   @brief    Constructor.
   */
  NavMeshSpatialQueryFactory();

  /*!
   @brief    The name of the spatial query implemenation.

   The spatial query's name must be unique among all registered spatial query components. Each
   spatial query factory must override this function.

   @returns  A string containing the unique spatial query name.
   */
  virtual const char* name() const { return "nav_mesh"; }

  /*!
   @brief    A description of the spatial query.

   Each spatial query factory must override this function.

   @returns  A string containing the spatial query description.
   */
  virtual const char* description() const {
    return "Performs spatial queries by operating on a navigation mesh.";
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
  SpatialQuery* instance() const { return new NavMeshSpatialQuery(); }

  /*!
   @brief    Given a pointer to an SpatialQuery instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this SpatialQuery's type (i.e.
   SpatialQueryFactory::thisFactory has already been called and returned true). If sub-classes of
   SpatialQueryFactory introduce *new* SpatialQuery parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    sq        A pointer to the spatial query whose attributes are to be set.
   @param    node      The XML node containing the elevation attributes.
   @param    specFldr  The path to the specification file. If the SpatialQuery references resources
                      in the file system, it should be defined relative to the specification file
                      location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(SpatialQuery* sq, TiXmlElement* node, const std::string& specFldr) const;

  /*!
   @brief    The identifier for the "file_name" string attribute.
   */
  size_t _fileNameID;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __SPATIAL_QUERY_NAV_MESH_H__
