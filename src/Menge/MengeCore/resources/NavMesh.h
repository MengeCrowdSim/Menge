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
 @file       NavMesh.h
 @brief      Defines the classes which maintain the navigation mesh data
 */

#ifndef __NAV_MESH_DATA_H__
#define __NAV_MESH_DATA_H__

#include "MengeCore/Agents/ObstacleSets/ObstacleVertexList.h"
#include "MengeCore/mengeCommon.h"
#include "MengeCore/resources/NavMeshObstacle.h"
#include "MengeCore/resources/Resource.h"

#include <map>
#include <vector>

namespace Menge {

namespace Agents {
class SimulatorInterface;
}

// forward declarations
class NavMesh;
class NavMeshNode;
class NavMeshEdge;
class NavMeshLocalizer;
class PathPlanner;

/*!
 @brief    A navigation mesh node group.

 It is a partitioning of the nodes in the navigation mesh into named groups. Each group contains a
 contiguous block of node identifiers.
 */
class NMNodeGroup {
 public:
  /*!
   @brief    Default constructor
   */
  NMNodeGroup() : _first(0), _last(0) {}

  /*!
   @brief    Constructor.

   @param    first    The index of the first node included in this group.
   @param    last    The index of the last node included in this group. The group spans the nodes 
                    [ _first, _last ], inclusive.
   */
  NMNodeGroup(unsigned int first, unsigned int last) : _first(first), _last(last) {}

  /*!
   @brief    Returns the global identifer of the group's ith node.

   @param    i    The ith index in this group.
   @returns  The id for the ith group; it is *not* checked against boundaries. It is the caller's
            responsibility to make sure that 0 <= i < groupSize().
   */
  inline size_t getGlobalId(unsigned int i) const { return _first + i; }

  /*!
   @brief    Reports the number of nodes in this group.
   */
  inline size_t groupSize() const { return static_cast<size_t>(_last - _first + 1); }

  /*!
   @brief    The first index in the group.
   */
  unsigned int _first;

  /*!
   @brief    The last index in the group.
   */
  unsigned int _last;
};

class NavMeshFactory;

/*!
 @brief    The class for defining a navigation mesh. A decomposition of the free space into a
          connected mesh of convex (or near convex) polygons used for global planning in planar or
          non-planar environments.

 It represents the minimum data required to do path planning. The navigation mesh is represented in
 2.5D. The boundaries of mesh's polygons are only defined on the ground plane. The nodes themselves
 include a definition of the polygon's plane off the plane.  Likewise, the vertices all lie on the
 plane.
 */
class MENGE_API NavMesh : public Resource {
 public:
  /*!
   @brief    Constructor

   @param    name    The name of the underlying navigation mesh.
   */
  NavMesh(const std::string& name);

 protected:
  /*!
   @brief    Destructor
   */
  ~NavMesh();

 public:
  /*!
   @brief    Clears the navigation mesh - removes edges and nodes.
   */
  void clear();

  /*!
   @brief    Returns a unique resource label to be used to identify different resource *types* which
            use the same underlying file data.
   */
  virtual const std::string& getLabel() const { return LABEL; }

  ////////////////////////////////////////////////////////////////
  //          Getters/Setters
  ////////////////////////////////////////////////////////////////

  /*!
   @brief    Reports the number of vertices in the navigation mesh.

   @returns  The number of vertices.
   */
  inline size_t getVertexCount() const { return _vCount; }

  /*!
   @brief    Reports the number of nodes in the navigation mesh.

   @returns  Thenumber of nodes.
   */
  inline size_t getNodeCount() const { return _nCount; }

  /*!
   @brief    Returns a reference to the ith node.

   @param    i    The index of the desired node. The validitity of the index is only tested in debug
                build.
   @returns  A reference to the ith node.
   */
  NavMeshNode& getNode(unsigned int i);

  /*!
   @brief    Returns a const reference to the ith node.

   @param    i    The index of the desired node. The validitity of the index is only tested in debug
                build.
   @returns  A const reference to the ith node.
   */
  const NavMeshNode& getNode(unsigned int i) const;

  /*!
   @brief    Returns a reference to the ith node in the given group.

   @param    grp    The group to retrieve the node from.
   @param    i      The index of the desired node. The validitity of the index is only tested in debug
                  build.
   @returns  A reference to the ith node.
   */
  NavMeshNode& getNode(const NMNodeGroup* grp, unsigned int i);

  /*!
   @brief    Returns a const reference to the ith node in the given group.

   @param    grp    The group to retrieve the node from.
   @param    i      The index of the desired node. The validitity of the index is only tested in debug
                  build.
   @returns  A const reference to the ith node.
   */
  const NavMeshNode& getNode(const NMNodeGroup* grp, unsigned int i) const;

  /*!
   @brief    Reports the number of edges in the navigation mesh.

   @returns  The integer number of edges.
   */
  inline size_t getEdgeCount() const { return _eCount; }

  /*!
   @brief    Returns a reference to the ith edge.

   @param    i    The index of the desired edge. The validitity of the index is only tested in debug 
                build.
   @returns  A reference to the ith edge.
   */
  NavMeshEdge& getEdge(unsigned int i);

  /*!
   @brief    Returns a const reference to the ith edge.

   @param    i    The index of the desired edge. The validitity of the index is only tested in debug
                build.
   @returns  A const reference to the ith edge.
   */
  const NavMeshEdge& getEdge(unsigned int i) const;

  /*!
   @brief    Returns the number of obstacles connected to this node.

   @returns  The number of attached obstacles.
   */
  size_t getObstacleCount() const { return _obstCount; }

  /*!
   @brief    Returns a reference to the ith obstacle.

   @param    i    The index of the desired obstacle. The validity of the index is only tested in debug
                build.
   @returns  A reference to the ith obstacle.
   */
  NavMeshObstacle& getObstacle(unsigned int i);

  /*!
   @brief    Returns a const reference to the ith obstacle.

   @param    i    The index of the desired obstacle. The validity of the index is only tested in debug
                build.
   @returns  A reference to the ith obstacle.
   */
  const NavMeshObstacle& getObstacle(unsigned int i) const;

  /*!
   @brief    Returns a pointer to the array of vertices.

   @returns  The pointer to the array of vertices.
   */
  inline Math::Vector2* getVertices() { return &_vertices[0]; }

  /*!
   @brief    Returns a const pointer to the array of vertices.

   @returns  The const pointer to the array of vertices.
   */
  inline const Math::Vector2* getVertices() const { return &_vertices[0]; }

  /*!
   @brief    Returns a pointer to the node group associated with the given group name.

   @param    grpName    The name of the group to find.
   @returns  A pointer to the node group. Null if the name isn't found.
   */
  const NMNodeGroup* getNodeGroup(const std::string& grpName) const;

  ////////////////////////////////////////////////////////////////
  //          Geometric queries
  ////////////////////////////////////////////////////////////////

  /*!
   @brief    Computes the elevation at a particular point based on the given node's geometry.

   @param    nodeID    The node to use in computing elevation.
   @param    p          The point to evaluate the elevation for.
   @returns  The elevation at point p based on the geometry of nodeID.
   */
  float getElevation(unsigned int nodeID, const Math::Vector2& p) const;

  /*!
   @brief    Computes the gradient at a particular point based on the given node's geometry.

   @param    nodeID    The node to use in computing gradient.
   @param    p          The point to evaluate the gradient for.
   @returns  The gradient at point p based on the geometry of nodeID.
   */
  Math::Vector2 getGradient(unsigned int nodeID, const Math::Vector2& p) const;

  ////////////////////////////////////////////////////////////////
  //          Construction functions
  ////////////////////////////////////////////////////////////////

  /*!
   @brief    Parses a navigation mesh definition and returns a pointer to it.

   This function works in conjunction with the ResourceManager. That is why it returns a pointer,
   not to a NavMesh, but to a Resource. The ResourceManager uses it to load and instantiate
   VectorField instances.

   @param    fileName    The path to the file containing the NavMesh definition.
   @returns  A pointer to the new NavMesh (if the file is valid), NULL if invalid.
   */
  static Resource* load(const std::string& fileName);

  /*!
   @brief    Allocates memory for the given number of vertices.

   All previous vertices will be deleted.

   @param    count    The number of vertices.
   */
  void setVertexCount(size_t count);

  /*!
   @brief    Sets the value of the ith vertex.

   @param    i    The index of the vertex. Validity of the index is only tested in the debug *build.
   @param    x    The x-value of the vertex.
   @param    y    The y-value of the vertex.
   */
  void setVertex(unsigned int i, float x, float y);

  /*!
   @brief    Allocates memory for the given number of nodes. 
   
   All previous nodes will be deleted.

   @param    count    The number of nodes.
   */
  void setNodeCount(size_t count);

  /*!
   @brief    Allocates memory for the given number of edges.

   All previous edges will be deleted.

   @param    count    The number of edges.
   */
  void setEdgeCount(size_t count);

  /*!
   @brief    Allocates memory for the given number of obstacles.

   All previous obstacles will be delete.d

   @param    count    The number of obstacles.
   */
  void setObstacleCount(size_t count);

  /*!
   @brief    After initializing the navigation mesh's components this function needs to be called to
            make the mesh functional.
   */
  bool finalize();

  /*!
   @brief    Adds a group of polygons to the navigation mesh.

   The group name must be unique from all other group names in the navigation mesh. The number of
   polygons in the group extends the number of reported nodes in the mesh (see
   NavMesh::getNodeCount()). Even if the nodes are not yet defined at this point, they are allocated
   and considered viable.

   @param    grpName      The name of the group.
   @param    grpSize      The number of nodes in the group.
   @returns  Returns a boolean reporting success (true) or failure (false).
   */
  bool addGroup(const std::string& grpName, size_t grpSize);

  // TODO(curds01): Why is this commented out?
  ///*
  // *  @brief    Adds the navigation mesh's obstacles to the simulator.
  // *
  // *  This should only be used if the navigation mesh is used as a spatial query.
  // *
  // *  @param    simulator    The simulator which receives the obstacles.
  // *
  // void addObstacles( Agents::SimulatorInterface * simulator );

  /*!
   @brief    Gets the navigation mesh's obstacles for the simulator.

   @returns    Vertex lists of the various obstacles
   */
  std::vector<Agents::ObstacleVertexList> getObstacles();

  /*!
   @brief    The unique label for this data type to be used with resource management.
   */
  static const std::string LABEL;

  friend class NavMeshFactory;
  friend class PathPlanner;

 protected:
  /*!
   @brief    The number of vertices
   */
  size_t _vCount;

  /*!
   @brief    An array containing all vertices.
   */
  Math::Vector2* _vertices;

  /*!
   @brief    The number of nodes (aka polygons in the mesh).
   */
  size_t _nCount;

  /*!
   @brief    An array containing all nodes.
   */
  NavMeshNode* _nodes;

  /*!
   @brief    The number of edges (aka portals or shared edges between polygons).
   */
  size_t _eCount;

  /*!
   @brief    An array containing all edges.
   */
  NavMeshEdge* _edges;

  /*!
   @brief    The number of obstacles in the scene
   */
  size_t _obstCount;

  /*!
   @brief    An array of obstacles.
   */
  NavMeshObstacle* _obstacles;

  /*!
   @brief    The mapping from node group name to an instance of a NMNodeGroup.
   */
  std::map<const std::string, NMNodeGroup> _nodeGroups;
};

/*!
 @brief    The definition of the managed pointer for NavMesh data
 */
typedef ResourcePtr<NavMesh> NavMeshPtr;

/*!
 @brief        Loads the navigation mesh of the given name

 @param  fileName  The name of the file containing the navigation mesh definition.
 @returns      The NavMeshPtr containing the data.
 @throws        A ResourceException if the data is unable to be *instantiated.
 */
NavMeshPtr loadNavMesh(const std::string& fileName) throw(ResourceException);

}  // namespace Menge

#endif  // __NAV_MESH_DATA_H__
