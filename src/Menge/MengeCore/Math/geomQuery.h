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
 @file       geomQuery.h
 @brief      Various mathematical operations and queries on geometry
 */

#ifndef __GEOM_QUERY_H__
#define __GEOM_QUERY_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"

namespace Menge {

/*!
 @namespace  Menge::Math
 @brief    The namespace for math primitives for simulation and visualization.
 */
namespace Math {
/*!
 @brief    Computes the time to collision between a ray and a circle

 This is a special-case test.  It assumes the ray originates from the origin of the world.

 @param    dir        A vector in R2 describing the direction (from the origin) of the ray.  (Does not
                    need to be normalized)
 @param    center    The position of the circle center.
 @param    radius    The radius of the circle.
 @returns  The expected "time" to collision ("infinity" if there is no collision).
 */
MENGE_API float rayCircleTTC(const Vector2& dir, const Vector2& center, float radius);

/*!
 @brief    Perform spherical linear interpolation between two vectors

 @param    t          The blend parameter. `t` must lie in the interval [0, 1].
 @param    p0        The first vector to interpolate (assumes ||p0|| = 1.0.
 @param    p1        The first vector to interpolate (assumes ||p1|| = 1.0.
 @param    sinTheta  The sine of the angle between the two vectors.
 @returns  The interpolated vector.
 */
MENGE_API Vector2 slerp(float t, const Vector2& p0, const Vector2& p1, float sinTheta);

}  // namespace Math
}  // namespace Menge

#endif  // __GEOM_QUERY_H__
