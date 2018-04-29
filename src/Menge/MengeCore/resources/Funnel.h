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

#ifndef __FUNNEL_H__
#define __FUNNEL_H__

/*!
 @file       Funnel.h
 @brief      The various classes for performing funnel algorithm on a portal path.

 The funnel algorithm is from: Lee and Preparata 1984 "Euclidean Shortest Paths in the Presence of
 Rectilinear Barriers"
 */

#include "MengeCore/mengeCommon.h"

#include <list>

namespace Menge {

using Menge::Math::Vector2;

/*!
 @brief    Determines if it uses simple funnel algorithm (no deque, just restart) or complex (using
          deque).
 */
// TODO: Determine which is faster.
//#define SIMPLE_FUNNEL

// FORWARD DECLARATION
class PortalPath;
class FunnelPlanner;

/*!
 @brief    The apex of the funnel.
 */
class FunnelApex {
 public:
  /*!
   @brief    Constructor

   @param    id      The index of the portal associated with this apex. If -1, it is the start
                    position.
   @param    point    The location of the start point.
   */
  FunnelApex(size_t id, const Vector2& point) : _id(id), _pos(point) {}

  /*!
   @brief    Sets the values of the apex

   @param    id      The index of the portal associated with this apex. If -1, it is the start
                    position.
   @param    point    The location of the start point.
   */
  inline void set(size_t id, const Vector2& point) {
    _id = id;
    _pos.set(point);
  }

  friend class FunnelPlanner;

 protected:
  /*!
   @brief    The identifier associated with this apex point.

   if -1, it is the start position, otherwise, a point extracted from the portal with the given id.
   */
  size_t _id;

  /*!
   @brief    The position of the apex.
   */
  Vector2 _pos;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The edge of a funnel.
 */
class FunnelEdge {
 public:
  /*!
   @brief    Default constructor.  No initialization.
   */
  FunnelEdge() {}

#ifdef SIMPLE_FUNNEL
  /*!
   @brief    Constructor.

   @param    id    The identifier of the portal from which this direction arises.
   @param    dir    The direction from the apex to the portal.
   */
  FunnelEdge(size_t id, const Vector2& dir) : _id(id), _dir(dir) {}
#else
  /*!
   @brief    Constructor.

   @param    id      The identifier of the portal from which this direction arises.
   @param    end      The identifier of the portal at which this wedge ends.
   @param    dir      The direction from the apex to the portal.
   @param    origin  The origin of this funnel "wedge".
   */
  FunnelEdge(size_t id, size_t end, const Vector2& dir, const Vector2& origin)
      : _id(id), _endID(end), _dir(dir), _origin(origin) {}
#endif

  /*!
   @brief    Reports if the given direction is to the left of this edge.

   @param    dir    The direction to test.
   @returns  A boolean reporting if the direction is to the left of this edge (true) or to the right
            (false).
   */
  inline bool isOnLeft(const Vector2& dir) const { return det(_dir, dir) > EPS; }

  /*!
   @brief    Reports if the given direction is to the right of this edge.

   @param    dir    The direction to test.
   @returns  A boolean reporting if the direction is to the right of this edge (true) or to the left
            (false).
   */
  inline bool isOnRight(const Vector2& dir) const { return det(dir, _dir) > EPS; }

  /*!
   @brief    Sets the properties of the funnel edge.

   @param    id    The id of the portal to which this funnel edge is associated.
   @param    dir    The direction of the edge.
   */
  inline void set(size_t id, const Vector2& dir) {
    _id = id;
    _dir.set(dir);
  }

  friend class FunnelPlanner;

 protected:
#ifdef SIMPLE_FUNNEL
  /*!
   @brief    The identifier of the portal for this direction
   */
  size_t _id;

#else
  /*!
   @brief    The identifier of the portal from which this wedge originates.
   */
  size_t _id;
  /*!
   @brief    The identifier of the portal that ENDS the wedge.
   */
  size_t _endID;

  /*!
   @brief    The origin of the wedge
   */
  Vector2 _origin;
#endif

  /*!
   @brief    The direction of this funnel edge
   */
  Vector2 _dir;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    The class that implements the funnel algorithm.
 */
class FunnelPlanner {
 public:
  /*!
   @brief    Constructor
   */
  FunnelPlanner();

  /*!
   @brief    Destructor
   */
  ~FunnelPlanner();

  /*!
   @brief    Computes the crossings for the given path based on the funnel algorithm.

   @param    radius      The radius of the agent.
   @param    startPos    The starting position of the path.
   @param    path      A pointer to a portal path.
   @param    startPortal    The portal in the path to start wtih
   */
  void computeCrossing(float radius, const Vector2& startPos, PortalPath* path,
                       size_t startPortal = 0);

#ifndef SIMPLE_FUNNEL
 protected:
  /*!
   @brief    The queue for the left side of the funnel.
   */
  std::list<FunnelEdge> _left;

  /*!
   @brief    The queue for the right side of the funnel.
   */
  std::list<FunnelEdge> _right;
#endif  // SIMPLE_FUNNEL
};
}  // namespace Menge

#endif  // __FUNNEL_H__
