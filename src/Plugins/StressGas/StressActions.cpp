#include "StressActions.h"
#include "BaseAgent.h"
#include "StressGlobals.h"
#include "StressTasks.h"

//#include "FsmGlobals.h"
//#include "SimulatorInterface.h"
//#include "FSM.h"
//#include "fsm/GoalSet.h"
//#include <iostream>

namespace StressGAS {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of PropertyXAction
	/////////////////////////////////////////////////////////////////////

	RegisterStressAgentAction::RegisterStressAgentAction() : Action() {
	}

	/////////////////////////////////////////////////////////////////////

	RegisterStressAgentAction::~RegisterStressAgentAction() {

	}

	/////////////////////////////////////////////////////////////////////

	void RegisterStressAgentAction::onEnter( Agents::BaseAgent * agent ) {
		STRESS_MANAGER->addAgent(agent);
	}

	/////////////////////////////////////////////////////////////////////

	void RegisterStressAgentAction::leaveAction( Agents::BaseAgent * agent ) {
		STRESS_MANAGER->resetAgent(agent);
		STRESS_MANAGER->removeAgent(agent);
	}

	/////////////////////////////////////////////////////////////////////
	BFSM::Task * RegisterStressAgentAction::getTask(){
		return new StressTask();	
	};

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of RegisterProxyAgentActionFactory
	/////////////////////////////////////////////////////////////////////

	bool RegisterStressAgentActionFactory::setFromXML( BFSM::Action * action, TiXmlElement * node, const std::string & behaveFldr ) const {
		RegisterStressAgentAction * pAction = dynamic_cast< RegisterStressAgentAction * >( action );
		assert( pAction != 0x0 && "Trying to set stress action properties on an incompatible object" );
		if ( ! BFSM::ActionFactory::setFromXML( action, node, behaveFldr ) ) {
			return false;
		}
		
		//there are none
		return true;
	}

	


}	// namespace Boeing
