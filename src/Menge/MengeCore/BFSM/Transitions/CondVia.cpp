/*
 * CondQuad.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: michael
 */

#include "CondVia.h"

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
		//                   Implementation of ViaTargetCondition
		///////////////////////////////////////////////////////////////////////////
		ViaTargetCondition::ViaTargetCondition(): Condition(),_acceptedRadius(0.f), _viaGoalSetID(-1){

		}


		///////////////////////////////////////////////////////////////////////////
		ViaTargetCondition::ViaTargetCondition( const ViaTargetCondition & cond ) {
					_acceptedRadius = cond._acceptedRadius;
					_viaGoalSetID = cond._viaGoalSetID;
		}

		///////////////////////////////////////////////////////////////////////////

		ViaTargetCondition::~ViaTargetCondition() {
			//TODO:What is this for?
		}

		///////////////////////////////////////////////////////////////////////////

		void ViaTargetCondition::onEnter( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		void ViaTargetCondition::onLeave( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		bool ViaTargetCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			const GoalSet * gs = Menge::SimulatorDBEntry::_fsm->getGoalSet(this->_viaGoalSetID);
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

		Condition * ViaTargetCondition::copy() {
			return new ViaTargetCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of ViaTargetCondFactory
		/////////////////////////////////////////////////////////////////////
	    ViaTargetCondFactory::ViaTargetCondFactory() : ConditionFactory() {
			_distanceID = _attrSet.addFloatAttribute("distance", true /*required*/, 0.0 );
			_viaGoalSetIDID = _attrSet.addIntAttribute("goalsVia", true, 0 );

		}

		///////////////////////////////////////////////////////////////////////////

		bool ViaTargetCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			ViaTargetCondition * pCond = dynamic_cast< ViaTargetCondition * >( condition );
			assert( pCond != 0x0 && "Trying to set the properties of a timer condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( pCond, node, behaveFldr ) ) return false;
                        float d=_attrSet.getFloat( _distanceID);
			pCond->setMinDistance(d*d);
			pCond->setGoalsToVia( _attrSet.getInt(_viaGoalSetIDID));

			return true;
		}
	}	// namespace BFSM
}	// namespace Menge
