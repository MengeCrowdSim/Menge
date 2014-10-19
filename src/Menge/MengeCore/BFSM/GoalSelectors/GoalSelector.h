/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

/*!
 *  @file       GoalSelector.h
 *  @brief      Defines the mechanism for selecting a goal for an agent.
 */

#ifndef __GOAL_SELECTOR_H__
#define	__GOAL_SELECTOR_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Element.h"
#include "ReadersWriterLock.h"
#include "MengeException.h"
#include <map>

// forward declaration
class TiXmlElement;

namespace Menge {

	// forward declaration
	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {

		// forward declaration
		class GoalSet;
		class Goal;

		/*!
		 *	@brief		Exception class for goal generation.
		 */
		class MENGE_API GoalSelectorException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalSelectorException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			GoalSelectorException( const std::string & s ): MengeException(s) {}

		};
		
		/*!
		 *	@brief		Exception thrown when the goal selector has an error which cannot be
		 *				recovered from.
		 */
		class GoalSelectorFatalException : public GoalSelectorException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalSelectorFatalException() : MengeException(), GoalSelectorException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			GoalSelectorFatalException( const std::string & s ): MengeException(s), GoalSelectorException(), MengeFatalException() {}
		};

		///////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The base, abstract class for selecting per-agent goals
		 */
		class MENGE_API GoalSelector : public Element {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalSelector():Element(), _persistent(false), _assignedGoals() {}

		protected:
			/*!
			 *	@brief		Default virtual destructor.
			 */
			virtual ~GoalSelector(){}

		public:
			/*! 
			 *	@brief		Uses the goal selector to assign a goal to the 
			 *				given agent.
			 *
			 *	This is the main interface for assiging goals to agents (while
			 *	maintaing bookkeping for capacity and persistence.)  It calls
			 *	the GoalSelector::getGoal method to actually determine which
			 *	goal is to be assigned.  
			 *
			 *	@param		agent		The agent for whom a goal is assigned.
			 *	@returns	A pointer to the assigned goal.
			 */
			Goal *	assignGoal( const Agents::BaseAgent * agent );

			/*! 
			 *	@brief		Informs the goal selector that the agent is done with
			 *				the goal.
			 *
			 *	The behavior of this function depends on the persistence of the
			 *	goal selector.  If the goal selector is persistent, the goal will
			 *	not actually be *freed* and capacity will remain as indicated.
			 *	If it is not persistent, the goal's capacity will be freed up.
			 *
			 *	@param		agent		The agent who is finished using the goal.
			 *	@param		goal		The goal the agent was previously assigned.
			 */
			void freeGoal( const Agents::BaseAgent * agent, Goal * goal );

			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const = 0;

			/*!
			 *	@brief		Gives the instance the opportunity to set the goal set.
			 *
			 *	This does nothing for instances which don't require a goal set.  Otherwise,
			 *	it extracts its required goal set.
			 *
			 *	@param		goalSets	A mapping from goal set identifier to goal set pointers.
			 */
			virtual void setGoalSet( std::map< size_t, GoalSet * > & goalSets ){}

			/*!
			 *	@brief		Sets the persistence of the goal.
			 *
			 *	@param		state		The new persistence state.
			 */
			inline void setPersistence( bool state ) { _persistent = state; }

			/*!
			 *	@brief		Reports the persistence of the goal.
			 *
			 *	@returns	True if the goal selector is persistent, false otherwise.
			 */
			inline bool getPersistence() const { return _persistent; }

		protected:
			/*!
			 *	@brief		Allows the goal selector to lock any resources it
			 *				requires.
			 *
			 *	This is primarily here so that GoalSelectors which use shared
			 *	resources have a chance to lock them (see SetGoalSelector).
			 *	A call to lockResources should always be followed by a call
			 *	to releeaseResources.
			 */
			virtual void lockResources() {}

			/*! 
			 *	@brief		Allows the goal selector to release previously locked
			 *				resources.
			 *
			 *	Should be used in conjunction with lockResources.
			 */
			virtual void releaseResources() {}

			/*!
			 *	@brief		Determines if the GoalSelector maintains persistent goals.
			 *
			 *	If the goal selector is persistent, it means that when an agent leaves the
			 *	state with the corresponding goal selector, the goal is not released and
			 *	if the agent returns to the state, it receives its previous goal.
			 */
			bool	_persistent;

			/*!
			 *	@brief		A mapping from agent id to assigned goals.
			 *				This will only contain meaningful values in one of two cases:
			 *					- If the selector is persistent.
			 *					- If compiled in debug mode (and then node freeing will
			 *					be tested against this map).
			 */
			HASH_MAP< size_t, Goal * >	_assignedGoals;

			/*!
			 *	@brief		The lock to maintain readers-writer access to the
			 *				_assignedGoals
			 */
			ReadersWriterLock	_lock;
		};

		/*!
		 *	@brief		Parses a TinyXML element containing a goal selector specification
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new goal selector.
		 */
		GoalSelector * parseGoalSelector( TiXmlElement * node, const std::string & behaveFldr );

	}	// namespace BFSM
}	// namespace Menge
#endif	 //__GOAL_SELECTOR_H__
