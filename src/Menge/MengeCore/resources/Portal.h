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
 @file    Portal.h
 @brief    The definition of a portal - a line which spans a clear area between obstacles.
 */

#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {

/*!
 @brief    A simple portal definition.

 A portal is simply a line segment - a region through which agents can pass.
 */
class MENGE_API Portal {
 public:
  /*!
   @brief    Default constructor.
   */
  Portal() : _p0(0.f, 0.f), _p1(0.f, 0.f), _mag(0.f), _dir(1.f, 0.f) {}

  /*!
   @brief    Constructor specifying portal geometry.

   @param    x0    The x-position of the first end point.
   @param    y0    The y-position of the first end point.
   @param    x1    The x-position of the second end point.
   @param    y1    The y-position of the second end point.
   */
  Portal(float x0, float y0, float x1, float y1);

  /*!
   @brief    Sets the geometry of the portal

   @param    x0    The x-position of the first end point.
   @param    y0    The y-position of the first end point.
   @param    x1    The x-position of the second end point.
   @param    y1    The y-position of the second end point.
   */
  void set(float x0, float y0, float x1, float y1);

  /*!
   @brief    Sets the geometry of the portal

   @param    p0    The position of the first end point.
   @param    p1    The position of the second end point.
   */
  void set(const Math::Vector2& p0, const Math::Vector2& p1);

  /*!
   @brief    Computes the neareset *clearable* point w.r.t. the portal

   Given the position and radius of an agent, computes the best clearable goal point. Clearable
   means that the straight-line path between the agent's current position and that goal point does
   NOT intersect either end point of the goal.

   This is important because we assume that the end points attach to obstacles and if the end point
   intersects the path, that means the preferred velocity is leading the agent into a collision with
   the obstacle.

   @param    pos        The position of the agent.
   @param    radius    The radius of the agent.
   @returns  The nearest *clearable* point.
   */
  Math::Vector2 nearestPoint(const Math::Vector2& pos, float radius);

  /*!
   @brief    The first end point of the portal line segment.
   */
  Math::Vector2 _p0;

  /*!
   @brief    The second end point of the portal line segment.
   */
  Math::Vector2 _p1;

  /*!
   @brief    The distance between endpoints (i.e., length of the portal).
   */
  float _mag;

  /*!
   @brief    The direction from the first to the second end point of the portal.
   */
  Math::Vector2 _dir;
};
}  // namespace Menge
#endif  // __PORTAL_H__
