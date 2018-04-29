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
 @file		GCF.h
 @brief		The definition of the Generalized Centrifugal Force pedestrian plug-in.

 This is inspired by the paper by Chraibi et al (Generalized Centrifugal Force model.
 However, the implementation differs in several critical ways:

 1) The "facing" direction is not simply the moving direction. In fact, this model has a unique
    computation for orientation based on its movement speed.
 2) Rather than having a facing direction that simply cuts off at 90 degrees to the side of the
    facing direction, forces are generated for agents in all directions, however, the magnitudes of
    those forces is modulated by how near the facing direction the neighbor is -- the closer to
    directly in front, the larger the force.
 3) The repulsion force is maxed out at 3X the user-specified maximum agent force. To insure a
    well-behaved response function, the interpolation width is constrained such that the response
    force only grows monotonically.
 4) The magnitude of the velocity scale (v_ij) is not the dot product of the relative velocity and
    orientation, but of the velocity and the *force* direction. Essentially, this factor goes to
    zero if the agents are already diverging. Only if the agents are *converging* does this factor
    increase.
 */

#ifndef __GCF_H__
#define __GCF_H__

#include "GCFAgent.h"
#include "GCFSimulator.h"

#endif  //__GCF_H__