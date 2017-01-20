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

			/*!
			*	@brief		The database of registered agent contexts.
			*/
			typedef VisElementDB<VelCompContext, Menge::BFSM::VelComponent> VCContextDB;
		}	// namespace VCContext
	}	// namespace Runtime

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	// Declrations of explicit specializations
	template<> Runtime::VCContext::VelCompContext *
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getDefaultElement();

	template<> void
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::addBuiltins();

	template<> std::string
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getElementName();
#endif	// DOXYGEN_SHOULD_SKIP_THIS
}

#endif	// __VEL_COMP_CONTEXT_DATABASE_H__
