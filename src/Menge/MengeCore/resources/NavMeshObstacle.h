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
 @file    NavMeshObstacle.h
 @brief    Specification for obstacles in a navigation mesh file.
 */

#ifndef __NAV_MESH_OBSTACLE__
#define __NAV_MESH_OBSTACLE__

#include <fstream>
#include "MengeCore/Agents/Obstacle.h"

namespace Menge {

// FORWARD DECLARATIONS
class NavMeshNode;
class NavMesh;

/*!
 @brief    Specification of an obstacle.
 
 It is the same as a pedModel specification but includes a pointer to a node to which it is
 attached.
 */
class MENGE_API NavMeshObstacle : public Agents::Obstacle {
 public:
  /*!
   @brief    The index value if the obstacle has no neighboring obstacle
   */
  static size_t NO_NEIGHBOR_OBST;

  /*!
   @brief    Constructor
   */
  NavMeshObstacle() : Agents::Obstacle(), _node(0x0) {}

  /*!
   @brief    Sets the obstacle properties from an obstacle definition in the given ascii file stream.

   @param    f      The input file stream.
   @param    vertices  The array of vertices into which the definition indexes.
   @returns  A boolean indicating successful parsing (true) or failure (false).
   */
  bool loadFromAscii(std::ifstream& f, Math::Vector2* vertices);

  /*!
   @brief    Retrieve the nav mesh node to which this obstacle is adjacent.

   @returns  A pointer to the node.
   */
  inline const NavMeshNode* getNode() const { return _node; }

  friend class NavMeshNode;
  friend class NavMesh;

 protected:
  /*!
   @brief    A nav mesh node to which this obstacle is attached.

   It could be attached to multiple, but this is the node for which it serves as an edge.
   */
  NavMeshNode* _node;
};
}  // namespace Menge

#endif  // __NAV_MESH_OBSTACLE__
