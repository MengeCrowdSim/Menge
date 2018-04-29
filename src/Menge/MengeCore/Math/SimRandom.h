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
 @file    SimRandom.h
 @brief    Functions for calculating normally distributed values.

 This code taken from John Burkardt (see SimRandom.cpp).
 */

#ifndef __SIM_RANDOM_H__
#define __SIM_RANDOM_H__

namespace Menge {

namespace Math {
/*!
 @brief    Generates a quasi-thread-safe, normally distributed random number.

 This algorithm generates two normally distributed values at a time. To be thread-safe, it could
 simply dismiss every second value. Alternatively, we can provide both values an place the burden on
 the caller to handle both.

 @param    a      The mean of the probability distribution field (pdf).
 @param    b      The standard deviation of the pdf.
 @param    val1  The first normally distributed value generated.
 @param    val2  The second normally distributed value generated.
 @param    seed  A seed for the random number generator. The value in this integer changes with every
                call.
 */
void r4_normalR(float a, float b, float& val1, float& val2, int* seed);

/*!
 @brief    The Box-Muller method for computing two normally distributed values at the same time (with
          mean of 0 and standard deviation of 1.

 @param    val1    The first normally distributed value generated.
 @param    val2    The second normally distributed value generated.
 @param    seed    A seed for the random number generator.
 @returns  The normally distributed number.
 */
void r4_normal_01R(float& val1, float& val2, int* seed);

/*!
 @brief    A uniformly generated random number in the range [0, 1]

 @param    seed    A seed for the random number generator.
 @returns  The uniformly distributed number.
 */
float r4_uniform_01(int* seed);

}  // namespace Math
}  // namespace Menge
#endif  //__SIM_RANDOM_H__
