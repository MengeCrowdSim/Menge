/*!
 *	@file		VelCompContextDatabase.h
 *	@brief		The definition of the database for all velocity component contexts.
 */

#ifndef __VEL_COMP_CONTEXT_DATABASE_H__
#define __VEL_COMP_CONTEXT_DATABASE_H__

#include "MengeVis/Runtime/VisElementDatabase.h"

// forward declaration
namespace Menge {
	namespace BFSM {
		class VelComponent;
	}
}

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
	template<> VCContext::VelCompContext *
		Runtime::VisElementDB<VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getDefaultElement();

	template<> void
		Runtime::VisElementDB<VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::addBuiltins();

	template<> std::string
		Runtime::VisElementDB<VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getElementName();
#endif	// DOXYGEN_SHOULD_SKIP_THIS

 }	// namespace Runtime
}  // namespace MengeVis

#endif	// __VEL_COMP_CONTEXT_DATABASE_H__
