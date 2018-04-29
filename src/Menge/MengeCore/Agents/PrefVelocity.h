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
 @file    PrefVelocity.h
 @brief    The definition of a preferred velocity.
 */

#ifndef __PREF_VELOCITY_H__
#define __PREF_VELOCITY_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {

/*!
 @brief    The definition of a preferred velocity.

 The preferred velocity is actually a space of velocities defined by two things:
 - a preferred speed
 - a *span* of directions.
 The span of directions represents a contiguous set of directions which can be considered
 topologically equivalent with respect to the agent reaching its goal.

 For example, the shortest path may require an agent to pass down a hall way, but whether the agent
 walks down the left, middle, or side is irrelevant -- all three will get the agent to its final
 goal with negligible difference.
 */
class MENGE_API PrefVelocity {
 public:
  /*!
   @brief    Default constructor
   */
  PrefVelocity();

  /*!
   @brief    Constructor for setting a span of a single direction.

   @param    dir      The single direction.
   @param    speed    The preferred speed.
   @param    target  The target point from which the preferred direction arises.
   */
  PrefVelocity(const Math::Vector2& dir, float speed, const Math::Vector2& target);

  /*!
   @brief    Constructor for setting a full span.

   @param    left    The "left"-most extent of the span (see _left).
   @param    right   The "right"-most extent of the span (see _right).
   @param    pref    The preferred direction (see _preferred).
   @param    speed   The preferred speed.
   @param    target  The target point from which the preferred direction arises.
   */
  PrefVelocity(const Math::Vector2& left, const Math::Vector2& right, const Math::Vector2& pref,
               float speed, const Math::Vector2& target);

  /*!
   @brief    Copy constructor.

   @param    vel    An instance of preferred velocity.
   */
  PrefVelocity(const PrefVelocity& vel);

  /*!
   @brief    Assignment operator

   @param    vel    Another instance of PrefVelocity whose values will be copied into this.
   @returns  A reference to this preferred velocity.
   */
  PrefVelocity& operator=(const PrefVelocity& vel);

  /*!
   @brief    Returns the left extent of the span.

   @returns  The left extent.
   */
  inline Math::Vector2 getLeft() const { return _left; }

  /*!
   @brief    Returns the right extent of the span.

   @returns  The right extent.
   */
  inline Math::Vector2 getRight() const { return _right; }

  /*!
   @brief    Returns the preferred *direction* of the span.

   @returns  The preferred direction.
   */
  inline Math::Vector2 getPreferred() const { return _preferred; }

  /*!
   @brief    Returns the preferred *velocity" of the span.

   This is the preferred direction at the velocity's speed.

   @returns  A vector pointing in the preferred direction at the given speed.
   */
  inline Math::Vector2 getPreferredVel() const { return _preferred * _speed; }

  /*!
   @brief    Returns the speed of the preferred velocity.

   @returns  The speed of the preferred velocity.
   */
  inline float getSpeed() const { return _speed; }

  /*!
   @brief    Sets the speed.

   @param    speed  The speed.
   */
  inline void setSpeed(float speed) { _speed = speed; }

  /*!
   @brief    Sets the preferred velocity to be a single velocity.

   @param    dir    The single preferred direction.
   */
  inline void setSingle(const Math::Vector2& dir) { _left = _preferred = _right = dir; }

  /*!
   @brief    Gets the target of the preferred velocity.

   @returns  A singe point in space which corresponds to the preferred direction.
   */
  inline Math::Vector2 getTarget() const { return _target; }

  /*!
   @brief    Sets the target of the preferred velocity.

   @param    target    The preferred velocity's target.
   */
  inline void setTarget(const Math::Vector2& target) { _target = target; }

  /*!
   @brief    Sets the preferred velocity span.

   @param    left    The direction of the left-most extent of the arc.
   @param    right    The direction of the right-most extent of the arc.
   @param    preferred  The single most-preferred direction in the span.
   */
  void setSpan(const Math::Vector2& left, const Math::Vector2& right,
               const Math::Vector2& preferred);

  /*!
   @brief    Reports if the arc spans more than a single direction (by reporting whether the arc has
            area.)

   @returns  A boolean indicating if the span has area (i.e. spans more than a single direction.
   */
  inline bool hasArea() const { return _left * _right < 1.f; }

 protected:
  /*!
   @brief    The "left" extent of the preferred velocity span (i.e. det( right, left ) >= 0).
   
   This direction has unit length.
   */
  Math::Vector2 _left;

  /*!
   @brief    The "right" extent of the preferred velocity span (i.e. det( right, left ) >= 0).
   
   This direction has unit length.
   */
  Math::Vector2 _right;

  /*!
   @brief    The preferred speed (also the radius of the arc.
   */
  float _speed;

  /*!
   @brief    The preferred direction.
   
   All directions in the span may be topologically equivalent, but this direction is the "best".
   This value must lie within the span (i.e., det( right, preferred ) > 0 and
   det( preferred, left) > 0).
   */
  Math::Vector2 _preferred;

  /*!
   @brief    The immediate goal point -- this corresponds to the preferred direction.
   */
  Math::Vector2 _target;
};

}  // namespace Agents
}  // namespace Menge

#endif  // __PREF_VELOCITY_H__
