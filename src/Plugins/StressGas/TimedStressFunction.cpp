#include "TimedStressFunction.h"
#include "AgentStressor.h"
#include "Core.h"


namespace StressGAS {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of TimedStressFunction
	/////////////////////////////////////////////////////////////////////

	TimedStressFunction::TimedStressFunction( float duration, Agents::BaseAgent * agent, 
											  AgentStressor * stressor, float coolDuration ) :
											  StressFunction(agent, stressor, coolDuration)
	{
		_stressRate = 1.f / duration;
	}

	/////////////////////////////////////////////////////////////////////

	float TimedStressFunction::updateStress() {
		_stressLevel += Menge::SIM_TIME_STEP * _stressRate;
		if ( _stressLevel > 1.f ) {
			_stressLevel = 1.f;
		}
		return _stressLevel;
	}

} // namespace StressGAS