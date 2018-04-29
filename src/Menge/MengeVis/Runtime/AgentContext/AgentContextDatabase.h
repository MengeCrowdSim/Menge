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

#ifndef __AGENT_CONTEXT_DATABASE_H__
#define __AGENT_CONTEXT_DATABASE_H__

#include "MengeVis/Runtime/VisElementDatabase.h"

namespace MengeVis {
namespace Runtime {
// forward declaration
class BaseAgentContext;
class VisAgent;

/*!
 @brief   The database of registered agent contexts.
 */
typedef VisElementDB<BaseAgentContext, VisAgent> AgentContextDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Declrations of explicit specializations
template <>
Runtime::BaseAgentContext*
Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getDefaultElement();

template <>
void Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::addBuiltins();

template <>
std::string Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getElementName();
#endif  // DOXYGEN_SHOULD_SKIP_THIS
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __AGENT_CONTEXT_DATABASE_H__
