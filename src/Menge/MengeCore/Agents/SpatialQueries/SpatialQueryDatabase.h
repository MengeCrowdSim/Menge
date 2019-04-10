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
 @file    SpatialQueryDatabase.h
 @brief    Central database for querying available spatial query implementations.

 For spatial queries to be used in simulation, they must register themselves into the
 SpatialQueryDatabase. This is done via the PluginEngine.
 */

#ifndef __SPATIAL_QUERY_DATABASE_H__
#define __SPATIAL_QUERY_DATABASE_H__

#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace Agents {
/*!
 @brief    The database of registered spatial query implementations.
 */
typedef ElementDB<SpatialQueryFactory, SpatialQuery> SpatialQueryDB;

}  // namespace Agents

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<Agents::SpatialQueryFactory, Agents::SpatialQuery>::addBuiltins();
template <>
std::string ElementDB<Agents::SpatialQueryFactory, Agents::SpatialQuery>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

#endif  // __SPATIAL_QUERY_DATABASE_H__
