/*
 * CondQuad.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: michael
 */

#include "CondFollow.h"

#include "Core.h"
#include "BaseAgent.h"
#include <cmath>
//TODO:include file with _sim reference(get agents ability)
#include "SimSystem.h"
#include "SimulatorDBEntry.h"
#include "Goals/Goal.h"
#include "GoalSet.h"
#include "FSM.h"
#include "Math/Vector2.h"
namespace Menge {

	namespace BFSM {


		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of FollowTargetCondition
		///////////////////////////////////////////////////////////////////////////
		FollowTargetCondition::FollowTargetCondition(): Condition(),_acceptedRadius(0.f), _followGoalSetID(-1){

		}


		///////////////////////////////////////////////////////////////////////////
		FollowTargetCondition::FollowTargetCondition( const FollowTargetCondition & cond ) {
					_acceptedRadius = cond._acceptedRadius;
					_followGoalSetID = cond._followGoalSetID;
		}

		///////////////////////////////////////////////////////////////////////////

		FollowTargetCondition::~FollowTargetCondition() {
			//TODO:What is this for?
		}

		///////////////////////////////////////////////////////////////////////////

		void FollowTargetCondition::onEnter( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		void FollowTargetCondition::onLeave( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		bool FollowTargetCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			const GoalSet * gs = Menge::SimulatorDBEntry::_fsm->getGoalSet(this->_followGoalSetID);
			if(gs == NULL)
			{
				logger<<"null goal set";
				return false;
			}
			size_t num_goals=gs->size();
			Menge::Math::Vector2 pos=agent->_pos;
			for(size_t i=0;i<num_goals;i++)
			{
			    const Goal* g=gs->getIthGoal(i);
			    float dist=g->squaredDistance(pos);
			    if(dist<=_acceptedRadius)
			       return true;
			}

			return false;
		}

		///////////////////////////////////////////////////////////////////////////

		Condition * FollowTargetCondition::copy() {
			return new FollowTargetCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of FollowTargetCondFactory
		/////////////////////////////////////////////////////////////////////
	    FollowTargetCondFactory::FollowTargetCondFactory() : ConditionFactory() {
			_distanceID = _attrSet.addFloatAttribute("distance", true /*required*/, 0.0 );
			_followGoalSetIDID = _attrSet.addIntAttribute("goalSetToFollow", true, 99 );

		}

		///////////////////////////////////////////////////////////////////////////

		bool FollowTargetCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			FollowTargetCondition * pCond = dynamic_cast< FollowTargetCondition * >( condition );
			assert( pCond != 0x0 && "Trying to set the properties of a follow condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( pCond, node, behaveFldr ) ) return false;
                        float d=_attrSet.getFloat( _distanceID);
			pCond->setMinDistance( d*d);
			pCond->setGoalsToFollow( _attrSet.getInt(_followGoalSetIDID));

			return true;
		}
	}	// namespace BFSM
}	// namespace Menge
