#include "GCFVisAgentFactory.h"
#include "GCFVisAgent.h"
#include "MengeCore/Agents/BaseAgent.h"


namespace GCF {

	using Menge::Agents::BaseAgent;
	using Menge::Agents::AgentException;
	using MengeVis::Runtime::VisAgent;

	////////////////////////////////////////////////////////////////
	//			Implementation of GCFVisAgentFactory
	////////////////////////////////////////////////////////////////

	VisAgent * GCFVisAgentFactory::makeVisAgent( BaseAgent * agent ) {
		if ( dynamic_cast<Agent*>(agent) == 0x0 ) {
			throw new AgentException("GCFVisAgentFactory only works with GCF::Agent types");
		}
		return new GCFVisAgent(agent);
	}
}	// namespace GCF