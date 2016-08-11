
/*!
 *	@file		GoalSelectorNearest.h
 *	@brief		The definition of the nearest goal selector.
 */

#ifndef __GOAL_SELECTOR_SECOND_NEAREST_H__
#define __GOAL_SELECTOR_SECOND_NEAREST_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelectorSet.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		The nearest goal selector selects the goal from a goal set that
		 *				is nearest to the agent in terms of Euclidian distance.
		 */
		class MENGE_API SecondNearestGoalSelector : public SetGoalSelector {
		public:
			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;
		};

		/*!
		 *	@brief		Factory for the NearestGoalSelector.
		 */
		class MENGE_API SecondNearestGoalSelectorFactory : public SetGoalSelectorFactory {
		public:
			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "second_nearest"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector.  Assigns the agent the goal in the given " \
						"goal set that is *nearest* the agent (in Euclidian distance).";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new SecondNearestGoalSelector(); }
		};
	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_NEAREST_H__
