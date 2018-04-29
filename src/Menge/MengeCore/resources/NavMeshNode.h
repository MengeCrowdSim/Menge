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
 @file      NavMeshNode.h
 @brief     Defines the "node" of the adjacency graph in a navigation mesh. The node corresponds to
            a polygon in the mesh.
 */

#ifndef __NAV_MESH_NODE_H__
#define __NAV_MESH_NODE_H__

#include <fstream>
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/resources/NavMeshPoly.h"

namespace Menge {

// Forward declarations
class NavMesh;
class NavMeshEdge;
class PathPlanner;
class NavMeshObstacle;

/*!
 @brief    The navigation mesh adjacency graph node. It corresponds to a convex polygon in the
          navigation mesh.
 */
class MENGE_API NavMeshNode {
 public:
  /*!
   @brief    Constructor
   */
  NavMeshNode();

  /*!
   @brief    Destructor
   */
  ~NavMeshNode();

  /*!
   @brief    Assignment operator
   */
  NavMeshNode& operator=(const NavMeshNode& n);

  /*!
   @brief    Sets the node identifier.

   @param    id    The identifier to set this node to.
   */
  void setID(unsigned int id) { _id = id; }

  /*!
   @brief    Retrive the identifier for this node.
   */
  unsigned int getID() const { return _id; }

  /*!
   @brief    Sets the node's center value.

   @param    c    The point value to set.
   */
  inline void setCenter(const Math::Vector2& c) { _center.set(c); }

  /*!
   @brief    Reports the center (centroid) of the node's polygon.

   @returns  The position of the polygon's centroid.
   */
  Math::Vector2 getCenter() const { return _center; }

  /*!
   @brief    Reports the center (centroid) of the node's polygon.

   @returns  The position of the polygon's centroid.
   */
  Math::Vector3 getCenter3D() const {
    return Math::Vector3(_center.x(), _poly.getElevation(_center), _center.y());
  }

  /*!
   @brief    Returns the number of vertices in the node's polygon.

   @returns  The number of vertices in the polygon.
   */
  size_t getVertexCount() const { return _poly._vertCount; }

  /*!
   @brief    Sets this node's polygon to the given vertex array.

   The polygon can then evaluate its indices with respect to this array of vertex values.

   @param    vertices  An array of Vector2's defining the polygon vertices.
   */
  void setVertices(const Math::Vector2* vertices) { _poly._vertices = vertices; }

  /*!
   @brief    Gets the index of the ith vertex in this node's polygon

   @param    i    The local index of the vertex in the node's polygon.
   @returns  The global index in the navigation mesh of the ith vertex in this node's polygon.
   */
  unsigned int getVertexID(size_t i) const { return _poly._vertIDs[i]; }

  /*!
   @brief    Returns the number of obstacles connected to this node.

   @returns  The number of obtacles in on this node.
   */
  size_t getObstacleCount() const { return _obstCount; }

  /*!
   @brief    Returns a const pointer to the ith obstacle in the node.

   @param    i    The local index of the desired obstacle.
   @returns  A const pointer to the ith obstacle.
   */
  const NavMeshObstacle* getObstacle(size_t i) const { return _obstacles[i]; }

  /*!
   @brief    Returns a const pointer to the ith obstacle in the node.

   @param    i    The local index of the desired obstacle.
   @returns  A pointer to the ith obstacle.
   */
  NavMeshObstacle* getObstacle(size_t i) { return _obstacles[i]; }

  /*!
   @brief    Reports the number of nodes adjacent to this node.
   */
  size_t getNeighborCount() const { return _edgeCount; }

  /*!
   @brief    Returns a pointer to the ith neighbor.

   @param    i    The index of the neighboring node to retrieve. The index is defined in the range
                [0, N-1], where this node has N neighbors.
   @returns  A pointer to the ith neighboring node.
   */
  const NavMeshNode* getNeighbor(size_t i) const;

  /*!
   @brief    Reports the number of edges on the node.

   @returns  The number of edges.
   */
  size_t getEdgeCount() const { return _edgeCount; }

  /*!
   @brief    Retrieves the ith edge connected to this node.

   @param    i    The local index of the desired edge.
   @returns  A pointer to the ith edge.
   */
  NavMeshEdge* getEdge(size_t i) { return _edges[i]; }

  /*!
   @brief    Retrieves a const pointer to the ith edge connected to this node.

   @param    i    The local index of the desired edge.
   @returns  A pointer to the ith edge.
   */
  const NavMeshEdge* getEdge(size_t i) const { return _edges[i]; }

  /*!
   @brief    Returns the pointer to the edge connecting this node with the node whose identifier is
            given.

   @param    nodeID    The id of the desired adjacent node.
   @returns  The pointer to the edge connecting the two nodes. If the nodes are not connected, NULL
            is returned.
   */
  NavMeshEdge* getConnection(unsigned nodeID);

  /*!
   @brief    Reports if the given point is inside the polygon

   @param    point    The point to test.
   @returns  A boolean reporting if the point lies inside the polygon (true) or outside (false ).
   */
   // TODO: What about the boundary?
  bool containsPoint(const Math::Vector2& point) const { return _poly.containsPoint(point); }

  /*!
   @brief    Sets the node properties from a node definition in the given ascii file stream.

   @param    f    The input file stream.
   @returns  A boolean indicating successful parsing (true) or failure (false).
   */
  bool loadFromAscii(std::ifstream& f);

  /*!
   @brief    Computes the height based on this node's polygon
   */
  inline float getElevation(const Math::Vector2& p) const { return _poly.getElevation(p); }

  /*!
   @brief    Computes the gradient based on this node's polygon
   */
  inline Math::Vector2 getGradient() const { return _poly.getGradient(); }

  friend class NavMesh;
  friend class NavMeshEdge;
  friend class PathPlanner;

 protected:
  /*!
   @brief    An array of edges connecting to other nodes
   */
  NavMeshEdge** _edges;

  /*!
   @brief    The number of edges connecting to this node.
   */
  size_t _edgeCount;

  /*!
   @brief    An array of obstacles connected to this node.
   */
  NavMeshObstacle** _obstacles;

  /*!
   @brief    The number of obstacles connecting to this node.
   */
  size_t _obstCount;

  /*!
   @brief    The "position" of the node - used to compute distance between node and goal
            (A* heuristic).
   */
  Math::Vector2 _center;

  /*!
   @brief    The polygon associated with this node
   */
  NavMeshPoly _poly;

  /*!
   @brief    The identifier of this node
   */
  unsigned int _id;
};
}  // namespace Menge

#endif  // __NAV_MESH_NODE_H__
