#include "MengeVis/Runtime/VCContext/VelCompContextDatabase.h"
#include "MengeVis/Runtime/VCContext/VelCompContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace MengeVis {

	namespace {
		// TODO: Define a null velocity component context that works on defaults.
		// TODO: Make it so this works despite the fact that I can't use "delete" in this global
		//	member.
		Runtime::VCContext::VelCompContext _defaultContext;
	}	// namespace

	// Specialization
	template<> Runtime::VCContext::VelCompContext *
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getDefaultElement() {
		return &_defaultContext;
	}

	template<> void
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::addBuiltins() {
	}

	template<> std::string
		Runtime::VisElementDB<Runtime::VCContext::VelCompContext,
							  Menge::BFSM::VelComponent>::getElementName() {
		return "velocity_component_context";
	}
}
#endif	// DOXYGEN_SHOULD_SKIP_THIS