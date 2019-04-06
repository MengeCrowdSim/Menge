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

#ifndef __VEL_COMP_CONTEXT_DATABASE_H__
#define __VEL_COMP_CONTEXT_DATABASE_H__

#include "MengeVis/Runtime/VisElementDatabase.h"

// forward declaration
namespace Menge {
namespace BFSM {
class VelComponent;
}
}  // namespace Menge

namespace MengeVis {
namespace Runtime {
class VisAgent;
namespace VCContext {
class VelCompContext;
}  // namespace VCContext
   /*!
    *	@brief		The database of registered agent contexts.
    */
typedef VisElementDB<VCContext::VelCompContext, Menge::BFSM::VelComponent> VCContextDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Declrations of explicit specializations
template <>
VCContext::VelCompContext*
Runtime::VisElementDB<VCContext::VelCompContext, Menge::BFSM::VelComponent>::getDefaultElement();

template <>
void Runtime::VisElementDB<VCContext::VelCompContext, Menge::BFSM::VelComponent>::addBuiltins();

template <>
std::string
Runtime::VisElementDB<VCContext::VelCompContext, Menge::BFSM::VelComponent>::getElementName();
#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Runtime
}  // namespace MengeVis

#endif  // __VEL_COMP_CONTEXT_DATABASE_H__
