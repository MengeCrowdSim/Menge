/*!
 *	@file		StressManager.h
 *	@brief		Manages stress values for agents
 */

#include "FSM.h"
#include "BaseAgent.h"
#include "PedVO/PedVOAgent.h"
#include <map>
#include <vector>
#include "StressManager.h"
#include "Core.h"

namespace StressGAS {

	////////////////////////////////////////////////////////////////
	//             Implementation of StressGasManager
	////////////////////////////////////////////////////////////////
	
	StressManager::StressManager() : _deltaNeighborDistance(0), _deltaNaxNeighbors(-.04f),
		_deltaPlanningHorizon(.04f), _deltaRadius(0.f), _deltaPrefSpeed(.66f), _stressStep(3.0f),
		_stressDelta(.1f), _stressMax(.8f) {
	
	};

	////////////////////////////////////////////////////////////////
	
	StressManager::~StressManager(){	
	};

	////////////////////////////////////////////////////////////////
	
	void StressManager::addAgent(Agents::BaseAgent  * agent){
		
		//if the agent isn't in the system
		if (!isInSystem(agent)){
			_agents[agent->_id] = agent;
			_agent_initialMaxNeighbors[agent->_id] = static_cast<float>( agent->_maxNeighbors );
			_agent_initialNeighborDistance[agent->_id] = agent->_neighborDist;
			_agent_initialPrefSpeed[agent->_id] = agent->_prefSpeed;
			_agent_initialRadius[agent->_id] = agent->_radius;
			_agent_stress[agent->_id] = 0.0f;
			_agent_nextStress[agent->_id] = Menge::SIM_TIME + _stressStep;
			
			PedVO::Agent *agt = dynamic_cast<PedVO::Agent *>(agent);
			if (agt != 0x0){
				_agent_initialPlanningHorizon[agent->_id] = agt->_timeHorizon;
			}
		}
		
		
	};

	////////////////////////////////////////////////////////////////
	
	void StressManager::removeAgent(Agents::BaseAgent  * agent){
		//if the agent isn't in the system
		if (isInSystem(agent)){
			_agents.erase(agent->_id);
			_agent_initialMaxNeighbors.erase(agent->_id);
			_agent_initialNeighborDistance.erase(agent->_id);
			_agent_initialPrefSpeed.erase(agent->_id);
			_agent_initialRadius.erase(agent->_id);
			_agent_stress.erase(agent->_id);
			_agent_nextStress.erase(agent->_id);
			
			PedVO::Agent *agt = dynamic_cast<PedVO::Agent *>(agent);
			if (agt != 0x0){
				_agent_initialPlanningHorizon.erase(agent->_id);
			}
		}
	};

	////////////////////////////////////////////////////////////////
	
	void StressManager::resetAgent(Agents::BaseAgent  * agent){
		//if the agent isn't in the system
		if (isInSystem(agent)){
			 agent->_maxNeighbors = static_cast<size_t>( std::floor(_agent_initialMaxNeighbors[agent->_id]));
			 agent->_neighborDist = _agent_initialNeighborDistance[agent->_id];
			 agent->_prefSpeed =_agent_initialPrefSpeed[agent->_id] = agent->_prefSpeed;
			 agent->_radius = _agent_initialRadius[agent->_id];
			_agent_stress[agent->_id] = 0.0f;
			_agent_nextStress[agent->_id] = Menge::SIM_TIME + _stressStep;
			
			PedVO::Agent *agt = dynamic_cast<PedVO::Agent *>(agent);
			if (agt != 0x0){
				agt->_timeHorizon = _agent_initialPlanningHorizon[agent->_id];
			}
		}
		
	};


	////////////////////////////////////////////////////////////////
	
	void StressManager::updateStress(){
		std::map<size_t,Agents::BaseAgent *>::iterator agentIterator = _agents.begin();
		for (;agentIterator != _agents.end(); ++agentIterator){
			if (Menge::SIM_TIME - _agent_nextStress[agentIterator->first] >= _stressStep){
				stressAgent(agentIterator->first);
			}
		}
			
	};

	////////////////////////////////////////////////////////////////
	
	void StressManager::stressAgent(size_t agentID){
		Agents::BaseAgent *agent = _agents[agentID];
		if (_agent_stress[agentID] < _stressMax){
			_agent_stress[agentID] += _stressDelta;
 			agent->_maxNeighbors = static_cast<size_t>(std::floor(_agent_initialMaxNeighbors[agent->_id] + _agent_stress[agentID] * _deltaNaxNeighbors));
			agent->_neighborDist = _agent_initialNeighborDistance[agent->_id] + _agent_stress[agentID] * _deltaNeighborDistance;
			agent->_prefSpeed =_agent_initialPrefSpeed[agent->_id] = agent->_prefSpeed + _agent_stress[agentID] * _deltaPrefSpeed;
			agent->_radius = _agent_initialRadius[agent->_id] + _agent_stress[agentID] * _deltaRadius;
			_agent_nextStress[agent->_id] = _agent_nextStress[agent->_id] +  _stressStep;
			
			PedVO::Agent *agt = dynamic_cast<PedVO::Agent *>(agent);
			if (agt != 0x0){
				agt->_timeHorizon = _agent_initialPlanningHorizon[agent->_id] + _agent_stress[agentID] * _deltaPlanningHorizon;
			}
		}
	};

	////////////////////////////////////////////////////////////////
	
	bool StressManager::isInSystem(const Agents::BaseAgent  * agent) {
		return (_agents.find(agent->_id) != _agents.end());
	};

};	// namespace StressGAS
