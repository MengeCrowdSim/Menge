/*!
 *	@file		AgentContextDatabase.h
 *	@brief		The definition of the database for all agent contexts.
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
		 *	@brief		The database of registered agent contexts.
		 */
		typedef VisElementDB<BaseAgentContext, VisAgent> AgentContextDB;
	}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	// Declrations of explicit specializations
	template<> Runtime::BaseAgentContext *
		Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getDefaultElement();

	template<> void
		Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::addBuiltins();

	template<> std::string
		Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getElementName();
#endif	// DOXYGEN_SHOULD_SKIP_THIS
}


#endif	// __AGENT_CONTEXT_DATABASE_H__
