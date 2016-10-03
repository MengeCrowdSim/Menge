/*!
 *	@file		STRESSManager.h
 *	@brief		Manages stress values for agents
 */

#ifndef __STRESS_GAS_MANAGER_H__
#define __STRESS_GAS_MANAGER_H__

#include "FSM.h"
#include "BaseAgent.h"
#include <map>
#include <vector>

using namespace Menge;

namespace StressGAS {

	/*!
     * @class StressManager
     *
     * @brief Manages stress applied to agents over time. Determines if stress needs to be applied, and does so.
     *
	 * This class contains several maps to initial agent values, and agents under effect
	 *
	 */
	class StressManager {
	public:
		/*!
		 * @brief default constructor
		 */
		StressManager();

		/*!
		 * @brief default destructor
		 */
		~StressManager();

		/*!
		 * @brief adds an agent to the stress system
		 *
		 *
		 * @param agent The BaseAgent that will be stressed
		 */
		void addAgent(Agents::BaseAgent  * agent);

		/*!
		 * @brief removes an agent from the stress system, and resets it if necessary
		 *
		 *
		 * @param agent The BaseAgent that will removed.
		 */
		void removeAgent(Agents::BaseAgent  * agent);

		/*!
		 * @brief reset an agent from the stress system
		 *
		 *
		 * @param agent The BaseAgent that will reset
		 */
		void resetAgent(Agents::BaseAgent  * agent);


		/*!
		 * @brief checks if stress needs to be applied, does so if necessary
		 *
		 *
		 */
		void updateStress();

		/*!
		 * @brief adds stress to the provided agent
		 * @param agent The BaseAgent that get stress
		 *
		 */
		void stressAgent(size_t AgentID);

		/*!
		 * @brief Function to determine if an agent is in the stress system
		 *
		 *
		 * @param agent The BaseAgent to check
		 * @returns True if the given agent is in the system
		 */
		bool isInSystem(const Agents::BaseAgent  * agent);

		
	protected:

		/*
		 * @brief: vector of agents in the stress system
		 */
		std::map<size_t, Agents::BaseAgent *> _agents;
		
		/*
		 * @ brief: map of agents to initial neighbor_distance
		 */
		std::map<size_t, float> _agent_initialNeighborDistance;

		/*
		 * @ brief: map of agents to initial maxNeighbors
		 */
		std::map<size_t, float> _agent_initialMaxNeighbors;

		/*
		 * @ brief: map of agents to initial radius
		 */
		std::map<size_t, float> _agent_initialRadius;

		/*
		 * @ brief: map of agents to initial pref_speed
		 */
		std::map<size_t, float> _agent_initialPrefSpeed;
		
		/*
		 * @ brief: map of agents to initial planning_horizon, fails gracefully 
		 *          if not using pedvo
		 */
		std::map<size_t, float> _agent_initialPlanningHorizon;


		/*
		 * @ brief:the current stress level of the agent
		 */
		std::map<size_t, float> _agent_stress;

		/*
		 * @ brief:the next time to stress the agent
		 */
		std::map<size_t, float> _agent_nextStress;

		/*
		 * @ brief: max change to NeighborDistance from stress
		 */
		float _deltaNeighborDistance;

		/*
		 * @ brief: max change to  maxNeighbors by this much
		 */
		float _deltaNaxNeighbors;

		/*
		 * @ brief: max change to  PrefSpeed by this much
		 */
		float _deltaPrefSpeed;

		/*
		 * @ brief: max change to  Radius by this much
		 */
		float _deltaRadius;

		/*
		 * @ brief: max change to  planningHorizon by this much
		 */
		float _deltaPlanningHorizon;

		/*
		 * @ brief:the amount of time that must pass before stressing
		 */
		float _stressStep;

		/*
		 * @ brief:the amount of stress level to change each step
		 */
		float _stressDelta;

		/*
		 * @ brief:the max amount of stress level
		 */
		float _stressMax;

	}; //end class

};
#endif