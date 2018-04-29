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
 @file     FSMEnumeration.h
 @brief    Enumerated types for the State class.
 */

#ifndef __STATE_ENUM_H__
#define __STATE_ENUM_H__

#include "MengeCore/Runtime/Logger.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The set of properties on which a PropertyAction can be used.
 */
enum PropertyOperand {
  NO_PROPERTY,    ///< The "NULL" property - indicating no valid property.
  MAX_SPEED,      ///< The agent's maximum speed.
  MAX_ACCEL,      ///< The agent's maximum, isotropic acceleration.
  PREF_SPEED,     ///< The agent's preferred speed.
  MAX_ANGLE_VEL,  ///< The agent's maximum angular velocity
  NEIGHBOR_DIST,  ///< The agent's neighbor distance
  PRIORITY,       ///< The agent's priority
  RADIUS          ///< The agent's radius
};

/*!
 @brief    An enumeration for defining inital velocity for agents
 */
enum InitVelEnum {
  ZERO_INIT_VEL,  ///< The agents will start with the zero velocity
  PREF_INIT_VEL   ///< The agents will start with their preferred velocity
};

}  // namespace BFSM

/*!
 @brief    Friend function for printing string versions of the PropertyOperand enum

 @param    out    The stream to write the string to.
 @param    op    The ProperyOperand to write.
 @returns  A reference to the output stream.
 */
Logger& operator<<(Logger& out, const BFSM::PropertyOperand op);

}  // namespace Menge

#endif  // __STATE_ENUM_H__
