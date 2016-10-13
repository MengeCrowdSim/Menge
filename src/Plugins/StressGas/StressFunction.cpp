#include "StressFunction.h"
#include "AgentStressor.h"


namespace StressGAS {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of StressFunction
	/////////////////////////////////////////////////////////////////////

	StressFunction::~StressFunction() {
		delete _stressor;
	}

	/////////////////////////////////////////////////////////////////////

	void StressFunction::initialize( const StressFunction * func ) {
		_stressLevel = func->_stressLevel;
		_stressor->setBaseline( func->_stressor );
	}

	//////////////////////////////////////////////////////////////////
	
	void StressFunction::processStress() {
		float stress = 0.f;
		switch ( _mode ) {
			case StressFunctionMode::ACTIVE:
				updateStress();
				break;
			case StressFunctionMode::COOLING:
				coolDown();
				if ( _stressLevel == 0.f ) _mode = FINISHED;
				break;
			case StressFunctionMode::PAUSED:
			case StressFunctionMode::FINISHED:
				return;
		}
		_stressor->applyStress( _stressLevel, _agent );
	}

	/////////////////////////////////////////////////////////////////////
	
	void StressFunction::coolDown() {
		_stressLevel -= _coolDownRate;
		if ( _stressLevel < 0.f ) _stressLevel = 0.f;
	}

	/////////////////////////////////////////////////////////////////////
	
	void StressFunction::undoStress() {
		_stressor->applyBaseline( _agent );
	}

} // namespace StressGAS