#ifndef __DISTANCE_STRESS_FUNCTION_H__
#define __DISTANCE_STRESS_FUNCTION_H__

#include "StressGasConfig.h"
#include "StressFunction.h"

// forward declaration
namespace Menge {
	namespace Math {
		class Geometry2D;
	}
}

namespace StressGAS {

	/**
	 *	@brief		The StressFunction that defines stress levels based on distance to a region.
	 *
	 *	An agent's stress level is simply a function of the distance to a specified region and
	 *	is defined as:
	 *	
	 *						0		if d > outer
	 *	    stress(d) =		f(d)	if inner < d < outer
	 *						1		if d < inner
	 *	    
	 *	where `d` is the distance to the region, `outer` is the maximum distance of effect,
	 *	`inner` is the distance of full effect, and `f` is a function that maps `d` in the range
	 *	[inner, outer] to the range [0, 1] (a strictly decreasing function).
	 *	
	 *	Currently, only `linear` and `quadratic` functions are supported.
	 */
	class EXPORT_API DistanceStressFunction : public StressFunction {
	public:

		/**  The function for mapping distance to stress */
		enum StressInterpEnum {
			LINEAR,		///< Stress increases linearly as distance decreases.
			QUADRATIC	///< Stress increases quadratically as distance decreases.
		};

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	region				The region to compute distance from; this function does
		 *								*not* own the geometry.
		 *	@param	inner				The distance to which stress is 100%.
		 *	@param	outer				The maximum distance beyond which stress is zero.
		 *	@param	funcType			The distance to stress function.
		 *	@param	agent				The agent to apply stress to.
		 *	@param	stressor			The agent stressor to use on the agent.
		 *	@param	coolDuration		The amount of time (in seconds) required to cool down from
		 */
		DistanceStressFunction( Menge::Math::Geometry2D const * const region,
								float inner, float outer, StressInterpEnum funcType,
								Menge::Agents::BaseAgent * agent, AgentStressor * stressor,
								float coolDuration );

		/*!
		 *	@brief		Updates the stress level for this agent.
		 *
		 *	@returns	The stress value for the function's agent.
		 */
		virtual float updateStress();

	protected:

		/** The definition of the stress region. */
		Menge::Math::Geometry2D const * const _region;

		/** The distance of full stress effect. */
		float _inner;

		/** The distance of no stress effect. */
		float _outer;

		/**	The function mapping distance to stress */
		StressInterpEnum _funcType;
	};
}	// namespace StressGAS

#endif	// __DISTANCE_STRESS_FUNCTION_H__
