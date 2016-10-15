#ifndef __TIMED_STRESS_FUNCTION_H__
#define __TIMED_STRESS_FUNCTION_H__

#include "StressGasConfig.h"
#include "StressFunction.h"

namespace StressGAS {

	class EXPORT_API TimedStressFunction : public StressFunction {
	public:

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	duration			The duration for the agent to reach 100% stress.
		 *	@param	agent				The agent to apply stress to.
		 *	@param	stressor			The agent stressor to use on the agent.
		 *	@param	coolDuration		The amount of time (in seconds) required to cool down from
		 */
		TimedStressFunction( float duration, Agents::BaseAgent * agent, AgentStressor * stressor,
							 float coolDuration );

		/*!
		 *	@brief		Updates the stress level for this agent.
		 */
		virtual float updateStress();

	protected:

		/** Percentage of stress accumulated per second of simulation time. */
		float _stressRate;
	};
}	// namespace StressGAS

#endif	// __TIMED_STRESS_FUNCTION_H__