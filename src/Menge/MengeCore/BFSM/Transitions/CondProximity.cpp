/*
 * CondProximity.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: michael
 */

#include "CondProximity.h"

#include "Core.h"
#include "BaseAgent.h"
#include <cmath>
//TODO:include file with _sim reference(get agents ability)
#include "SimSystem.h"
#include "SimulatorDBEntry.h"
namespace Menge {

	namespace BFSM {


		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of ProximityCondition
		///////////////////////////////////////////////////////////////////////////
		ProximityCondition::ProximityCondition(): Condition(),_acceptedRadius(0.f), _AgentToAvoidID(-1), _inc(0.00){

		}


		///////////////////////////////////////////////////////////////////////////
		ProximityCondition::ProximityCondition( const ProximityCondition & cond ) {
					_acceptedRadius = cond._acceptedRadius;
					_AgentToAvoidID = -1;
					_inc = 0.00;
		}

		///////////////////////////////////////////////////////////////////////////

		ProximityCondition::~ProximityCondition() {
			//TODO:What is this for?
		}

		///////////////////////////////////////////////////////////////////////////

		void ProximityCondition::onEnter( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		void ProximityCondition::onLeave( Agents::BaseAgent * agent ) {

		}

		///////////////////////////////////////////////////////////////////////////

		bool ProximityCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			const Agents::BaseAgent *avoidAgent = Menge::SimulatorDBEntry::_sim->getAgent(this->_AgentToAvoidID);
			if(avoidAgent == NULL)
			{
				logger<<"null void agent";
				return false;
			}
            if(_acceptedRadius>=3)
                        _inc+=0.0004;
            if(_acceptedRadius<20)
                        _acceptedRadius += 0.2;
			return std::sqrt(std::pow(std::abs(agent->_pos._x- avoidAgent->_pos._x),2)+std::pow(std::abs(agent->_pos._y -
					avoidAgent->_pos._y),2))<avoidAgent->_radius+_acceptedRadius+_inc;

		}

		///////////////////////////////////////////////////////////////////////////

		Condition * ProximityCondition::copy() {
			return new ProximityCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of ProximityCondFactory
		/////////////////////////////////////////////////////////////////////
	ProximityCondFactory::ProximityCondFactory() : ConditionFactory() {
			_distanceID = _attrSet.addFloatAttribute("distance", true /*required*/, 0.0 );
			_AgentToAvoidID = _attrSet.addIntAttribute("agentToAvoid", true, 0 );

		}

		///////////////////////////////////////////////////////////////////////////

		bool ProximityCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			ProximityCondition * pCond = dynamic_cast< ProximityCondition * >( condition );
			assert( pCond != 0x0 && "Trying to set the properties of a proximity condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( pCond, node, behaveFldr ) ) return false;

			pCond->setMinDistance( _attrSet.getFloat( _distanceID));
			//cycle through agents here to find disaster agent using agent to aboid in attrset, set the this_x and this_y from that thern return true

			pCond->setAgentToAvoid( _attrSet.getInt(_AgentToAvoidID)) ;


			return true;
		}
	}	// namespace BFSM
}	// namespace Menge
