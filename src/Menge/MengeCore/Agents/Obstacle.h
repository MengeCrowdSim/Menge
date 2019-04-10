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
 @file       Obstacle.h
 @brief      Contains the Obstacle class.
 */

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {

/*!
@brief      Defines static obstacles in the simulation.
*/
class MENGE_API Obstacle {
 public:
  /*!
   @brief    An enumeration to define the type of nearest point - first, middle, last.
   */
  enum NearTypeEnum { FIRST, MIDDLE, LAST };

  /*!
   @brief      Constructs a static obstacle instance.
   */
  Obstacle();

  /*!
   @brief      Destroys this static obstacle instance.
   */
  ~Obstacle();

  /*!
   @brief    Retrieves the normal of the obstacle
   @returns  The normal
   */
  inline Math::Vector2 normal() const { return Math::Vector2(_unitDir.y(), -_unitDir.x()); }

  /*!
   @brief    Retrieve the first point on the obstacle.
   */
  inline const Math::Vector2& getP0() const { return _point; }

  /*!
   @brief    Retrieve the obstacle's mid-point.
   */
  inline const Math::Vector2 midPt() const { return _point + (0.5f * _length) * _unitDir; }

  /*!
   @brief    Retrieve the second point on the obstacle.
   */
  Math::Vector2 getP1() const;

  /*!
   @brief    Returns the next obstacle in sequence
   */
  const Obstacle* next() const { return _nextObstacle; }

  /*!
   @brief    Computes the squared distance from the obstacle to the given point.

   Also sets the value of the point in the provided Vector2

   @param    pt        The point whose distance is to be evaluated.
   @param    nearPt    The position on the obstacle which is nearest to the test point will be set
                      here.
   @param    distSq    The squared distance to the line (i.e. ||pt - nearPt||^2) is placed inside
                      this parameter.
   @returns  The classificaiton of what the nearest point is - first, middle, or last.
   */
  NearTypeEnum distanceSqToPoint(const Math::Vector2& pt, Math::Vector2& nearPt,
                                 float& distSq) const;

  /*!
   @brief    Determines if a circle, moving along a ray, will intersect the obstacle.

   @param    dir      The direction of motion.
   @param    start    The starting point of the circle.
   @param    radius  The radius of the circle.
   @returns  The time to collision (a large value representing infinity if no collision is possible.)
   */
  float circleIntersection(const Math::Vector2& dir, const Math::Vector2& start,
                           float radius) const;

  /*!
   @brief    Returns the length of the obstacle

   @returns  The length.
   */
  inline float length() const { return _length; }

  /*!
   @brief    Reports if the given point lies on the obstacle

   @param    pt    The point to test
   @returns  True if pt is on the obstacle, false otherwise
   */
  bool pointOnObstacle(const Math::Vector2& pt) const;

  /*!
   @brief    Reports if the given point is on the "outside" of the obstacle.
   
   This definition depends on whether the obstacle is double-sided or not.

   @param    point    The point to test.
   @returns  A boolean reporting if the point lies on the outside of this obstacle (true) or not
            (false).
   */
  // NOTE: This test is "safe" because if _doubleSided is true, the leftOf test doesn't
  //    get performed.  If it is false, then _nextObstacle must point to a valid
  //    obstacle.
  inline bool pointOutside(const Math::Vector2& point) const {
    return _doubleSided || (leftOf(_point, getP1(), point) < 0.f);
  }

  /*!
   @brief    Reports if the obstacle is convext at _point.

   Takes into account the double-sidedness of the obstacle and the side of the obstacle the agent is
   on.

   @param    agtOnRight    Indicate if the agent is on the right side of the obstacle (true) or the
                          left (false).
   @returns  True if the obstacle is convex, false otherwise.
   */
  inline bool p0Convex(bool agtOnRight) const {
    return agtOnRight ? _isConvex : _doubleSided && !_isConvex;
  }

  /*!
   @brief    Reports if the obstacle is convext at _point + _length * _unitDir.

   Takes into account the double-sidedness of the obstacle and the side of the obstacle the agent is
   on.

   @param    agtOnRight    Indicate if the agent is on the right side of the obstacle (true) or the
                          left (false).
   @returns  True if the obstacle is convex, false otherwise.
   */
  // NOTE: The only way for _nextObstacle to be NULL is for this to be double sided.
  //    And end points of double-sided obstacles are always convex.
  inline bool p1Convex(bool agtOnRight) const {
    return _nextObstacle == 0x0
               ? true
               : (agtOnRight ? _nextObstacle->_isConvex : _doubleSided && _nextObstacle->_isConvex);
  }

  /*!
   @brief    Sets the obstacle's closed state.

   If closed, it is part of a sequence of obstacles that form a closed polygon. If open, it is part
   of a polyline.

   @param    closed    Defines the obstacle as part of a closed polygon (true), or a closed polygon
                      (false).
   */
  inline void setClosedState(bool closed) { _doubleSided = !closed; }

  /*!
   @brief    Reports if the obstacle is double sided.

   If double sided (true) then the obstacle should be included in the nearby obstacles, *regardless*
   of which side of the line the agent lies.
   */
  bool _doubleSided;

  /*!
   @brief    Reports if the obstacle is convex around the obstacle's point (_point).
   */
  bool _isConvex;

  /*!
   @brief    Pointer to the next obstacle in the greater obstacle structure.

   If the obstacle is open, it could be NULL.
   */
  Obstacle* _nextObstacle;

  /*!
   @brief    The point from which the obstacle is defined.
   */
  Math::Vector2 _point;

  /*!
   @brief    Pointer to the previous obstacle in the greater obstacle structure.

   If the obstacle is open, it could be NULL.
   */
  Obstacle* _prevObstacle;

  /*!
   @brief    The direction the obstacle extends from the originating point.
   */
  Math::Vector2 _unitDir;

  /*!
   @brief    The distance in the direction the obstacle extends.
   */
  float _length;

  /*!
   @brief    A unique identifier for this obstacle.
   */
  size_t _id;

  /*!
   @brief The class of obstacle, used so agents can ignore/include obstacles
   */
  size_t _class;
};

/*!
   @brief      Computes the squared distance from a line segment with the specified endpoints to a
               specified point.

   @param      a               The first endpoint of the line segment.
   @param      b               The second endpoint of the line segment.
   @param      c               The point to which the squared distance is to be calculated.
   @returns    The squared distance from the line segment to the point.
   */
inline MENGE_API float distSqPointLineSegment(const Math::Vector2& a, const Math::Vector2& b,
                                              const Math::Vector2& c) {
  const float r = ((c - a) * (b - a)) / absSq(b - a);

  if (r < 0.0f) {
    return absSq(c - a);
  } else if (r > 1.0f) {
    return absSq(c - b);
  } else {
    return absSq(c - (a + r * (b - a)));
  }
}
}  // namespace Agents
}  // namespace Menge
#endif  // __OBSTACLE_H__
