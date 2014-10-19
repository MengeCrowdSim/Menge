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
 *	@file		TargetReturn.h
 *	@brief		Defines a transition target that returns the agent to the state from
 *				which he entered this state.
 */

#ifndef __TARGET_RETURN_H__
#define	__TARGET_RETURN_H__

#include "CoreConfig.h"
#include "Target.h"
#include "TargetFactory.h"
#include "fsmCommon.h"
#include "ReadersWriterLock.h"
#include <list>

namespace Menge {

	namespace BFSM {

		// forward declarations
		class State;
		class ReturnTargetFactory;

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The definition of the return target. 
		 *
		 *	The return target causes the agent to go back to the state from which
		 *	the agent transitioned to the current state.  However, this isn't necessarily
		 *	the *literal* previous state.  The FSM can advance multiple states in a single
		 *	time step.  This target returns the agent to the state it was in at the beginning
		 *	of this call to advance (and not the immediately previous state which was simply
		 *	passed through.)
		 */
		class MENGE_API ReturnTarget : public TransitionTarget {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ReturnTarget();

			/*!
			 *	@brief		Copy constructor
			 *
			 *	@param		tgt		The transition target to copy.
			 */
			ReturnTarget( const ReturnTarget & tgt);

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
			 *	@brief		Determines the next state for the given agent.
			 *
			 *	This function defines the main functionality of the target.
			 *	Given the current agent and the class's internal state, it determines 
			 *	The state to move the agent into.  This should be overridden by each
			 *	sub-class.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@returns	A pointer to the next state.
			 */
			virtual State * nextState( Agents::BaseAgent * agent );	

			/*!
			 *	@brief		Performs any necessary connections to the target state(s).
			 *
			 *	Sub-classes must override this because a transition must transition to
			 *	a state.  See the sub-classes SingleTransition for an example.
			 *
			 *	@param		stateMap		A mapping from state names to state pointers.
			 *	@returns	True if connection was successful, false otherwise.
			 */
			virtual bool connectStates( std::map< std::string, State * > & stateMap );

			/*!
			 *	@brief		Create a copy of this target.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this target - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual TransitionTarget * copy();
			
			friend class ReturnTargetFactory;
		protected:
			/*!
			 *	@brief		A mapping between an agent id and its return state.
			 */
			std::map< size_t, State * > _targets;

			/*!
			 *	@brief		Lock to protect _targets;
			 */
			ReadersWriterLock	_lock;
		
		};
		
		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for creating the ReturnTarget
		 */
		class MENGE_API ReturnTargetFactory : public TargetFactory {
		public:
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "return"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "Defines the transition target as the state the agent was in when "\
					   "it advanced to this state.";
			}
			
		protected:
			/*!
			 *	@brief		Create an instance of this class's condition.
			 *
			 *	All TargetFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding condition type.  The various field values
			 *	of the instance will be set in a subsequent call to TargetFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated TransitionTarget class.
			 */
			virtual TransitionTarget * instance() const { return new ReturnTarget(); }
		};
		
	}	// namespace BFSM
}	// namespace Menge
#endif	// __TARGET_RETURN_H__