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
 @file    ORCAAgentContext.h
 @brief   A basic context for interacting with and displaying ORCA agent parameters
 */

#ifndef __ORCA_AGENT_CONTEXT_H__
#define __ORCA_AGENT_CONTEXT_H__

#include "MengeCore/Orca/ORCAAgent.h"
#include "MengeVis/Runtime/AgentContext/ORCATypeAgentContext.h"

namespace MengeVis {
namespace Runtime {
/*!
 @brief   Specialization of the ORCA context for ORCA::Agent.
 */
typedef ORCATypeAgentContext<ORCA::Agent> OrcaAgentContext;

////////////////////////////////////////////////////////////////
//      Implementation of ORCAAgentContext
////////////////////////////////////////////////////////////////

// Specialization
template <>
inline std::string ORCATypeAgentContext<ORCA::Agent>::contextName() const {
  return "ORCA";
}

////////////////////////////////////////////////////////////////

template <>
std::string ORCATypeAgentContext<ORCA::Agent>::getElementName() const {
  return ORCA::Agent::NAME;
}
}  // namespace Runtime
}  // namespace MengeVis
#endif  // __ORCA_AGENT_CONTEXT_H__
