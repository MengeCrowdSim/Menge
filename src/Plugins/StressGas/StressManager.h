/*!
 *	@file		StressSManager.h
 *	@brief		Manages stress values for agents
 */

#ifndef __STRESS_MANAGER_H__
#define __STRESS_MANAGER_H__

#include "FSM.h"
#include "BaseAgent.h"
#include <map>
#include <vector>

using namespace Menge;

namespace StressGAS {

	/*!
     * @class StressManager
     *
     * @brief	Manages stress applied to agents over time. Determines if stress needs to be applied, and does so.
     *
	 * This class contains several maps to initial agent values, and agents under effect
	 *
	 */
	class StressManager {
	public:
		/*!
		 * @brief	Default constructor
		 */
		StressManager();

		/*!
		 * @brief	Destructor
		 */
		~StressManager();

		/*!
		 * @brief	Adds an agent to the stress system.
		 *
		 *	Passing an agent that has already been registered does nothing.
		 *
		 * @param	agent		The BaseAgent that will be stressed.
		 */
		void addAgent(Agents::BaseAgent  * agent);

		/*!
		 * @brief	Removes an agent from the stress system, and resets it if necessary
		 *
		 *	Submitting an agent that is not currently stressed does nothing.
		 *
		 * @param	agent		The BaseAgent that will removed.
		 */
		void removeAgent(Agents::BaseAgent  * agent);

		/*!
		 * @brief	Reset a stressed agent to its initial pre-stressed state.
		 *
		 * @param	agent	The BaseAgent that will reset.
		 */
		void resetAgent(Agents::BaseAgent  * agent);

		/*!
		 * @brief	Checks if stress needs to be applied, does so if necessary
		 */
		void updateStress();

		/*!
		 * @brief	Adds stress to the provided agent.
		 * @param	agentId		The identifier for the agent that gets stress.
		 */
		void stressAgent(size_t agentId);

		/*!
		 * @brief	Reports if the given agent is in the stress system.
		 *
		 * @param	agent	The BaseAgent to check.
		 * @returns	True if the given agent is in the system
		 */
		bool isInSystem(const Agents::BaseAgent  * agent);
		
	protected:

		/*
		 * @brief	The set of agents in the stress system.
		 */
		std::map<size_t, Agents::BaseAgent *> _agents;
		
		/*
		 * @brief	The initial `_neighborDist` values for stressed agents.
		 */
		std::map<size_t, float> _agent_initialNeighborDistance;

		/*
		 * @brief	The initial `maxNeighbors` values for stressed agents.
		 */
		std::map<size_t, float> _agent_initialMaxNeighbors;

		/*
		 * @brief	The initial `_radius` values for stressed agents.
		 */
		std::map<size_t, float> _agent_initialRadius;

		/*
		 * @brief	The initial `_prefSpeed` values for stressed agents.
		 */
		std::map<size_t, float> _agent_initialPrefSpeed;
		
		/*
		 * @brief	The initial `_timeHorizon` values for stressed agents.
		 *			Only applies to pedvo agents.
		 * // TODO(curds01) 10/4/16 - Should this be PedVO only?  Or ORCA as well?
		 */
		std::map<size_t, float> _agent_initialPlanningHorizon;

		/*
		 * @brief	The current stress level values for stressed agents.
		 */
		std::map<size_t, float> _agent_stress;

		/*
		 * @brief	The simulation time at which to accumulate stress for each stressed agent.
		 */
		std::map<size_t, float> _agent_nextStress;

		/*
		 * @brief	The maximum change to `_neighborDist` at maximum stress.
		 */
		float _deltaNeighborDistance;

		/*
		 * @brief	The maximum change to `_maxNeighbors` at maximum stress.
		 */
		float _deltaNaxNeighbors;

		/*
		 * @brief	The maximum change to `_prefSpeed` at maximum stress.
		 */
		float _deltaPrefSpeed;

		/*
		 * @brief	The maximum change to `_radius` at maximum stress.
		 */
		float _deltaRadius;

		/*
		 * @brief	The maximum change to `_timeHorizon` at maximum stress.
		 */
		float _deltaPlanningHorizon;

		/*
		 * @brief	The time intervals at which stress accumulates.
		 */
		float _stressStep;

		/*
		 * @brief	The amount of stress levels change at each accumulation.
		 */
		float _stressDelta;

		/*
		 * @brief	The max amount of stress an agent can accumulate.
		 */
		float _stressMax;
	}; 
};
#endif	// __STRESS_MANAGER_H__