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
 @file    AgentGeneratorDatabase.h
 @brief    Central database for querying available agent generator implementations.

 For agent generators to be used in simulation, they must register themselves into the
 AgentGeneratorDatabase.  This is done via the PluginEngine.
 */

#ifndef __AGENT_GENERATOR_DATABASE_H__
#define __AGENT_GENERATOR_DATABASE_H__

#include "MengeCore/Agents/AgentGenerators/AgentGenerator.h"
#include "MengeCore/Agents/AgentGenerators/AgentGeneratorFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace Agents {
/*!
 @brief    The database of registered agent generator implementations.
 */
typedef ElementDB<AgentGeneratorFactory, AgentGenerator> AgentGeneratorDB;

}  // namespace Agents

// Declarations of explicit specialization
#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
void ElementDB<Agents::AgentGeneratorFactory, Agents::AgentGenerator>::addBuiltins();

template <>
std::string ElementDB<Agents::AgentGeneratorFactory, Agents::AgentGenerator>::getElementName();
#endif  // DOXYGEN_SHOULD_SKIP_THIS
}  // namespace Menge

#endif  // __AGENT_GENERATOR_DATABASE_H__
