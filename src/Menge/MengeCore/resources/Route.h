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
 @file       Route.h
 @brief      The definition of a basic route through a navigation mesh
 */

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <vector>
#include "MengeCore/resources/WayPortal.h"

namespace Menge {

// FORWARD DECLARATIONS
class PathPlanner;
class NavMeshEdge;

/*!
 @brief    The definition of a route through a navigation mesh from a start to an end node.
 */
class MENGE_API PortalRoute {
 public:
  /*!
   @brief    Constructor

   @param    start    The index of the start node of the route.
   @param    end      The index of the end node of the rotue.
   */
  PortalRoute(unsigned int start, unsigned int end);

  /*!
   @brief    Destructor.
   */
  ~PortalRoute();

  /*!
   @brief    Returns the identifier for the first node on the route.

   @returns  The first node's index.
   */
  inline unsigned int getStartNode() const { return _startNode; }

  /*!
   @brief    Returns the identifier for the destination node on the route.

   @returns  The destination node's index.
   */
  inline unsigned int getEndNode() const { return _endNode; }

  /*!
   @brief    Reports the number of portals on the route.

   @returns  The number of portals on the route.
   */
  inline size_t getPortalCount() const { return _portals.size(); }

  /*!
   @brief    Reports the node for the ith portal.

   @param    i    The index of the portal.
   @returns  The index of the node the agent should be in when moving toward the ith portal.
   */
  inline unsigned int getPortalNode(size_t i) const { return _portals[i]._nodeID; }

  /*!
   @brief    Gets the ith portal.

   @param    i    The path-local index of the portal.
   @returns  A pointer to the ith portal.
   */
  WayPortal* getPortal(size_t i) { return &_portals[i]; }

  /*!
   @brief    Gets a const version of the ith portal.

   @param    i    The path-local index of the portal.
   @returns  A pointer to the ith portal.
   */
  const WayPortal* getPortal(size_t i) const { return &_portals[i]; }

  /*!
   @brief    Appends a way portal to the path.

   @param    edge    The edge forming the portal
   @param    node    The node in which the agent should be traveling toward the edge
   */
  void appendWayPortal(const NavMeshEdge* edge, unsigned int node);

  /*!
   @brief    Reports if the two routes have the same envelope.

   Two routes are equivalent if they have the same sequence of nav mesh nodes. It is assumed that
   they have the same start and end nodes.

   @param    route    The route to compare with this.
   @returns  True if the two routes are equivalent.
   */
  bool isEquivalent(const PortalRoute* route);

  /*!
   @brief    Report the length of the path.
   */
  float getLength() const { return _length; }

  friend class PathPlanner;

 protected:
  /*!
   @brief    The start node.
   */
  unsigned int _startNode;

  /*!
   @brief    The end node.
   */
  unsigned int _endNode;

  /*!
   @brief    The maximum width an agent can be to take this route
   */
  float _maxWidth;

  /*!
   @brief    The smallest agent width for which this has proven to be the optimal path.
   */
  float _bestSmallest;

  /*!
   @brief    The length of the route
   */
  float _length;

  /*!
   @brief    The list of portals to pass through along the route
   */
  std::vector<WayPortal> _portals;
};
}  // namespace Menge

#endif  // __ROUTE_H__
