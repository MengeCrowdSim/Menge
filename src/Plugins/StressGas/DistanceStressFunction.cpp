#include "DistanceStressFunction.h"
#include "AgentStressor.h"


namespace StressGAS {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of DistanceStressFunction
	/////////////////////////////////////////////////////////////////////

	DistanceStressFunction::DistanceStressFunction( SceneGraph::Shape const * const region,
													float inner, float outer, 
													StressInterpEnum funcType,
													Agents::BaseAgent * agent, 
													AgentStressor * stressor,
													float coolDuration ) :
													StressFunction( agent, stressor, coolDuration ),


	{
		
	}

	/////////////////////////////////////////////////////////////////////

	float DistanceStressFunction::updateStress() {
		float d = 0.f;	// todo compute distance
		if ( d > _outer ) return 0.f;
		if ( d < _inner ) return 1.f;

		float frac = ( d - _inner ) / ( _outer - _inner );
		switch ( _funcType ) {
			case LINEAR:
				return frac;
			case QUADRATIC:
				return frac * frac;
		}
	}

} // namespace StressGAS