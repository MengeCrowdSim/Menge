/*
 * CondQuad.h
 *
 *  Created on: Jul 18, 2016
 *      Author: michael
 */

#ifndef MENGE_MENGECORE_BFSM_TRANSITIONS_CONDFOLLOW_H_
#define MENGE_MENGECORE_BFSM_TRANSITIONS_CONDFOLLOW_H_

#include "CoreConfig.h"
#include "Transitions/Condition.h"
#include "Transitions/ConditionFactory.h"
#include "fsmCommon.h"
#include "ReadersWriterLock.h"
#include "SimulatorInterface.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The definition of the follow condition.
		 *
		 *	The follow condition becomes active when the agent is near a target
                 *      "follow" goal set. This can be used to simulate agents following a set of 
                 *      moving goals, e.g., quadcopters
		 */
		class MENGE_API FollowTargetCondition : public Condition {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			FollowTargetCondition();
			FollowTargetCondition(const FollowTargetCondition & cond);


			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy();

		protected:
			/*!
			 *	@brief		Constructor.
			 */
			~FollowTargetCondition();

		public:
			/*!
			 *	@brief		Called when an agent enters a state with this exiting transition.
			 *
			 *	Sub-classes should use this function as the opportunity to cache any
			 *	particular per-agent data.
			 *
			 *	@param		agent		The agent who has entered the state which uses
			 *							this transition.
			 */
			virtual void onEnter( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Called when an agent exits the state with this transition.
			 *
			 *	@param		agent		The agent who left the state.
			 */
			virtual void onLeave( Agents::BaseAgent * agent );

			/*!
			 *	@brief		check if agent is near a quad
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	True if the condition has been met, false otherwise.
			 */
			virtual bool conditionMet( Agents::BaseAgent * agent, const Goal * goal );
			virtual void setGoalsToFollow(int id){_followGoalSetID = id;}
			void setMinDistance( float dist ) { _acceptedRadius = dist; }
                         friend class FollowTargetCondFactory;
		protected:

			float _acceptedRadius;
            size_t	_followGoalSetID;

		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for creating the FollowTargetCondition
		 */
		class MENGE_API FollowTargetCondFactory : public ConditionFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			FollowTargetCondFactory();

			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "follow"; }
			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The Quad condition. returns true if agent is within a radius of one of quadrotors";
			}


		protected:
			/*!
			 *	@brief		Create an instance of this class's condition.
			 *
			 *	All ConditionFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding condition type.  The various field values
			 *	of the instance will be set in a subsequent call to ConditionFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated Action class.
			 */
			virtual Condition * instance() const { return new FollowTargetCondition(); }

			/*!
			 *	@brief		Given a pointer to an Condition instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this Tarnsitions's type.
			 *	(i.e. ConditionFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of ConditionFactory introduce *new* Condition parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		condition		A pointer to the condition whose attributes are to be set.
			 *	@param		node			The XML node containing the condition attributes.
			 *	@param		behaveFldr		The path to the behavior file.  If the condition references
			 *								resources in the file system, it should be defined relative
			 *								to the behavior file location.  This is the folder containing
			 *								that path.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;

			/*!
			 *	@brief		The identifier for the minimum distance.
			 */
			size_t	_distanceID;

			/*!
			 *	@brief		The identifier for the Goal set to follow.
			 */
			size_t	 _followGoalSetIDID;
		};

	}	// namespace BFSM
}	// namespace Menge
#endif //MENGE_MENGECORE_BFSM_TRANSITIONS_CONDFOLLOW_H_ 
