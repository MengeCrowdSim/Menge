#include "GoalSelectors/GoalSelectorSecondNearest.h"
#include "Goals/Goal.h"
#include "GoalSet.h"
#include "BaseAgent.h"
#include <cassert>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NearestGoalSelector
		/////////////////////////////////////////////////////////////////////

		Goal * SecondNearestGoalSelector::getGoal( const Agents::BaseAgent * agent ) const {
			assert( agent != 0x0 && "NearestGoalGenerator requires a valid base agent!" );
			const size_t GOAL_COUNT = _goalSet->size();
			if ( GOAL_COUNT == 0 ) {
				logger << Logger::ERR_MSG << "NearestGoalSelector was unable to provide a goal for agent " << agent->_id << ".  There were no available goals in the goal set.";
				return 0x0;
			}
			const Vector2 p = agent->_pos;

			Goal * bestGoal;

			bestGoal = _goalSet->getIthGoal( 0 );
			Goal * secondBestGoal;

			secondBestGoal = _goalSet->getIthGoal( 0 );
			Vector2 disp = bestGoal->getCentroid() - p;
			float bestDist = absSq( disp );
			for ( size_t i = 1; i < GOAL_COUNT; ++i ) {
				Goal * testGoal = _goalSet->getIthGoal( i );
				disp = testGoal->getCentroid() - p;
				float testDist = absSq( disp );
				if ( testDist < bestDist ) {
					bestDist = testDist;
					secondBestGoal=bestGoal;
					bestGoal = testGoal;
				}
			}

			return secondBestGoal;
		}
	}	// namespace BFSM
}	// namespace Menge
