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
 @file    consts.h
 @brief    Some common mathematical constants.
 */

#ifndef __CONSTS_H__
#define __CONSTS_H__

#include "MengeCore/CoreConfig.h"

#include <math.h>

namespace Menge {

/*!
 @brief    A convenient definition of infinity.
 */
extern MENGE_API const float INFTY;

/*!
 @brief    pi.
 */
extern MENGE_API const float PI;

/*!
 @brief    2 * pi.
 */
extern MENGE_API const float TWOPI;

/*!
 @brief    pi / 2.
 */
extern MENGE_API const float HALFPI;

/*!
 @brief    Scale factor for converting degrees to radians.
 */
extern MENGE_API const float DEG_TO_RAD;

/*!
 @brief    Scale factor for converting radians to degrees.
 */
extern MENGE_API const float RAD_TO_DEG;

/*!
 @brief    Suitably small number for testing for functional zero values.
 */
extern MENGE_API const float EPS;

}  // namespace Menge

#endif  // __CONSTS_H__
