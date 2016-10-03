#include "StressTasks.h"
#include "StressGlobals.h"
#include "BaseAgent.h"
//#include "FSM.h"
//#include "MengeBase\SimulatorInterface.h"


namespace StressGAS {
	/////////////////////////////////////////////////////////////////////
	//                   Implementation of DensityGridTask
	/////////////////////////////////////////////////////////////////////

	StressTask::StressTask(): Task() {
	}

	/////////////////////////////////////////////////////////////////////

	void StressTask::doWork( const BFSM::FSM * fsm ) throw( BFSM::TaskException ) {
		
		//call the stress system update
		StressGAS::STRESS_MANAGER->updateStress();
	}
	/////////////////////////////////////////////////////////////////////

	std::string StressTask::toString() const {
		// TODO: include the name of the navigation mesh
		return "Stress Task";
	}

	/////////////////////////////////////////////////////////////////////

	bool StressTask::isEquivalent( const Task * task ) const {
		const StressTask * other = dynamic_cast< const StressTask * >( task );
		if ( other == 0x0 ) {
			return false;
		} else {
			return true;
		}
	}
	

}	// namespace BFSM 
