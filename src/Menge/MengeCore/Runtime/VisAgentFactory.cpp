#include "VisAgentFactory.h"
#include "VisAgent.h"
#include "BaseAgent.h"

namespace Menge {

	////////////////////////////////////////////////////////////////
	//					Implementation of VisAgentFactory
	////////////////////////////////////////////////////////////////

	VisAgent * VisAgentFactory::makeVisAgent( Agents::BaseAgent * agent ) {
		return new VisAgent( agent );
	}

}	// namespace Menge