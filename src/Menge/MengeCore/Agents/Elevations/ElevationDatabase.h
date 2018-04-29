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
 @file    ElevationDatabase.h
 @brief    Central database for querying available elevation implementations.

 For elevations to be used in simulation, they must register themselves into the ElevationDatabase.
 This is done via the PluginEngine.
 */

#ifndef __ELEVATION_DATABASE_H__
#define __ELEVATION_DATABASE_H__

#include "MengeCore/Agents/Elevations/Elevation.h"
#include "MengeCore/Agents/Elevations/ElevationFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace Agents {
/*!
 @brief    The database of registered elevation implementations.
 */
typedef ElementDB<ElevationFactory, Elevation> ElevationDB;

}  // namespace Agents

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<Agents::ElevationFactory, Agents::Elevation>::addBuiltins();
template <>
std::string ElementDB<Agents::ElevationFactory, Agents::Elevation>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS
}  // namespace Menge

#endif  // __ELEVATION_DATABASE_H__
