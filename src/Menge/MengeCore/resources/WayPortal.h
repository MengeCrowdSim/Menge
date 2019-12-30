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
 @file       WayPortal.h
 @brief      Defines a way portal for a navigation mesh route/path.
 */

#ifndef __WAY_PORTAL_H__
#define __WAY_PORTAL_H__

#include "MengeCore/resources/NavMeshEdge.h"

namespace Menge {

// FORWARD DECLARATIONS
class PortalRoute;
class PortalPath;
namespace Agents {
class PrefVelocity;
}

/*!
 @brief    The basic structure defining a way "point" along a path through a navigation mesh.
 
 It consists of the navigation mesh edge to pass through and the node one should be in while heading
 to the edge.
 */
class MENGE_API WayPortal {
 public:
  /*!
   @brief    Constructor

   @param    edge      The NavigationMeshEdge which serves as the immediate goal for this WayPortal.
   @param    nodeID    The nav mesh node that an agent should be in while heading for this node.
   @param    p0IsLeft  Indicates if the p0 value of the edge is on the left, when faced from inside
                      the node.
   */
  WayPortal(const NavMeshEdge* edge, unsigned int nodeID, bool p0IsLeft);

  /*!
   @brief    Retrives the left physical vertex of the portal.

   @returns  The position of the left physical vertex, as viewed from inside _nodeID.
   */
  inline Math::Vector2 getLeft() const { return _p0IsLeft ? _edge->getP0() : _edge->getP1(); }

  /*!
   @brief    Retrives the position in the portal offset from the left physical vertex.

   @param    offset    The distance offset from the left physical vertex.
   @returns  The position offset from the left physical vertex, as viewed from inside _nodeID.
   */
  inline Math::Vector2 getLeft(float offset) const {
    return _p0IsLeft ? _edge->getP0(offset) : _edge->getP1(offset);
  }

  /*!
   @brief    Retrives the right physical vertex of the portal.

   @returns  The position of the right physical vertex, as viewed from inside _nodeID.
   */
  inline Math::Vector2 getRight() const { return _p0IsLeft ? _edge->getP1() : _edge->getP0(); }

  /*!
   @brief    Retrives the position in the portal offset from the right physical vertex.

   @param    offset    The distance offset from the right physical vertex.
   @returns  The position offset from the right physical vertex, as viewed from inside _nodeID.
   */
  inline Math::Vector2 getRight(float offset) const {
    return _p0IsLeft ? _edge->getP1(offset) : _edge->getP0(offset);
  }

  /*!
   @brief    Computes the intersection point of the given line with this portal.

   This algorithm ASSUMES that the line DOES intersect the portal. The behavior is undefined if they
   don't intersect.

   @param    point      A point on the line to test.
   @param    dir        A direction vector defining the position of the line.
   @returns  A point, on the portal, at which the line intersects.
   */
  Math::Vector2 intersectionPoint(const Math::Vector2& point, const Math::Vector2& dir) const;

  /*!
   @brief   Computes the parameterized projection of the point A onto the portal's edge.

   The edge of the way portal is defined by two points: (p0, p1). This edges likewise defines a
   line. The point A' is the projection of A onto that line. A' represnted by a single scalar
   parameter s.

   The *truncated* way portal is defined by points (p0', p1') such that p0' and p1' are inset from
   p0 and p1 toward the interior of the edge `clearance` units. The parameter s is defined w.r.t.
   these points:

      `A' = p0' + s * (p1' - p0')`

   The value s can range from negative infinity to positive infinity. A value in the range [0, 1]
   indicates that the point A projects to the interior of the *truncated* edge.

   @param p_WA        The query point A, measured and expressed in the world frame.
   @param clearance   The amount of required clearance.
   @returns  The parameterization of A's projection relative to the truncated edge.
   @pre  The length of this wayportal must be greater than 2 * clearance.
   */
  float clearanceParameter(const Math::Vector2& p_WA, float clearance) const;

  /*!
   @brief   Similar to clearanceParameter(const Math::Vector2&, float) except the point A' is
            defined as the intersection of the way portal's line with the line spanned by the points
            A and B.

   @param   p_WA      The first point on the line, measured and expressed in the world frame.
   @param   p_WB      The second point on the line, measured and expressed in the world frame.
   @param clearance   The amount of required clearance.
   @returns  The relative position of A' on the truncated way portal. A value in the range [0, 1]
             implies that A' lies on the trucanted portal.
   @pre  The length of this wayportal must be greater than 2 * clearance.
   */
  float clearanceParameter(const Math::Vector2& p_WA, const Math::Vector2& p_WB,
                           float clearance) const;

  /*!
   @brief   Computes the point on the clearance-truncated portal given the parameter `s`.

   @param   s           The fraction between the way points.
   @param   clearance   The amount of clearance from this way portal's end points.
   @returns The point on the truncated edge.
   @pre   The length of this wayportal must be greater than 2 * `clearance`.
  */
  Math::Vector2 clearPoint(float s, float clearance) const;

  /*!
   @brief    Sets the directions on the preferred velocity based on the wayportal.

   @param    pos        The position of the agent.
   @param    radius    The radius of the agent.
   @param    dir        Preferred direction of the agent.  Direction is NOT assumed to be of
                      unit-length.
   @param    pVel      The preferred velocity
   */
  void setPreferredDirection(const Math::Vector2& pos, float radius, const Math::Vector2& dir,
                             Agents::PrefVelocity& pVel) const;

  friend class PortalRoute;
  friend class PortalPath;

 protected:
  /*!
   @brief    The edge which serves as goal.
   */
  const NavMeshEdge* _edge;

  /*!
   @brief    The index of the node in which the agent is traveling toward the edge.
   
   It should be true that the specified node is attached/connected to the given edge.
   */
  unsigned int _nodeID;

  /*!
   @brief    Reports if the edge's P0 is the left vertex.

   Left is defined based on moving across the portal from the node indicated by _nodeID;
   */
  bool _p0IsLeft;
};

}  // namespace Menge
#endif  // __WAY_PORTAL_H__
