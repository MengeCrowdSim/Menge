#include "DistanceStressFunction.h"
#include "AgentStressor.h"

#include "Core.h"

namespace StressGAS {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of DistanceStressFunction
	/////////////////////////////////////////////////////////////////////

	DistanceStressFunction::DistanceStressFunction( Math::Geometry2D const * const region,
													float inner, float outer, 
													StressInterpEnum funcType,
													Agents::BaseAgent * agent, 
													AgentStressor * stressor,
													float coolDuration ) :
													StressFunction( agent, stressor, coolDuration ),
													_region(region), _inner(inner), _outer(outer),
													_funcType(funcType)
	{}

	/////////////////////////////////////////////////////////////////////

	float DistanceStressFunction::updateStress() {
		// natural cool down
		_stressLevel -= _coolDownRate * Menge::SIM_TIME_STEP;

		// increase to distance if greater
		float target = 0.f;
		float d = sqrt( _region->squaredDistance( _agent->_pos ) );
		if ( d > _outer ) target = 0.f;
		else if ( d < _inner ) target = 1.f;
		else {
			target = 1.f - ( d - _inner ) / ( _outer - _inner );
			switch ( _funcType ) {
				case LINEAR:
					break;
				case QUADRATIC:
					target *= target;
			}
		}
		
		_stressLevel = ( target > _stressLevel ) ? target : _stressLevel;
		return _stressLevel;
	}

} // namespace StressGAS