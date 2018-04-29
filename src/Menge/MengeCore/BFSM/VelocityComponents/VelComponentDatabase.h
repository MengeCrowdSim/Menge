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
 @file    VelComponentDatabase.h
 @brief    Central database for querying available behavior velocity components.

 For velocity components to be used in the finite state machine, they must register themselves into
 the VelCompDB. This is done via the PluginEngine.
 */

#ifndef __VEL_COMPONENT_DATABASE_H__
#define __VEL_COMPONENT_DATABASE_H__

#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {
namespace BFSM {
/*!
 @brief    The database of registered velocity component implementations.
 */
typedef ElementDB<VelCompFactory, VelComponent> VelCompDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<BFSM::VelCompFactory, BFSM::VelComponent>::addBuiltins();
template <>
std::string ElementDB<BFSM::VelCompFactory, BFSM::VelComponent>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

#endif  // __VEL_COMPONENT_DATABASE_H__
