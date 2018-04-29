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

#ifndef __LINE_H__
#define __LINE_H__

/*!
 @file       Line.h
 @brief      Contains the definition for an line used as a linear constraint (e.g. ORCA half plane).
 */

#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Math {

/*!
 @brief      Defines a directed line.
 */
class MENGE_API Line {
 public:
  /*!
   @brief    Default constructor.
   */
  Line();

  /*!
   @brief    Explicit constructor.

   @param    p    A point on the line.
   @param    d    A unit-length vector defining the direction of the line. The line is defined by p +
                dt, where t in the range [-infinity, infinity].
   */
  Line(const Vector2& p, const Vector2& d);

  /*!
   @brief    Returns the nearest point on the line to p

   @param    p      The point to which we want to find the nearest point.
   @returns  A point on the line that is closest to p.
   */
  Vector2 nearestPt(const Vector2& p) const;

  /*!
   @brief    A point on the directed line.
   */
  Vector2 _point;

  /*!
   @brief    The direction of the directed line.
   */
  Vector2 _direction;
};
}  // namespace Math
}  // namespace Menge
#endif  // __LINE_H__
