#include "GCFVisAgentFactory.h"
#include "GCFVisAgent.h"
#include "BaseAgent.h"


namespace GCF {

	////////////////////////////////////////////////////////////////
	//			Implementation of GCFVisAgentFactory
	////////////////////////////////////////////////////////////////

	VisAgent * GCFVisAgentFactory::makeVisAgent( Agents::BaseAgent * agent ) {
		if ( dynamic_cast<Agent*>(agent) == 0x0 ) throw new Menge::Agents::AgentException("GCFVisAgentFactory only works with GCF::Agent types");
		return new GCFVisAgent(agent);
	}

}	// namespace GCF