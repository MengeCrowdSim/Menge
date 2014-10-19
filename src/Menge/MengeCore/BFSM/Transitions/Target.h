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
 *	@file		Target.h
 *	@brief		The basis for determing what an active transition leads to.
 */

#ifndef __TARGET_H__
#define __TARGET_H__

#include "CoreConfig.h"
#include "Element.h"
#include <map>
#include <string>

namespace Menge {

	// forward declarations
	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {
		// forward declarations
		class State;
		class Goal;

		/*!
		 *	@brief		The base class for transition targets.
		 *
		 *	A transition target is the state a transition moves an agent to.
		 *	In the simplest case, the transition simply connects two states and
		 *	when the transition is active, the agent moves from the source state
		 *	to the destination state.  
		 *
		 *	However, Target's can be more complex.  The destination can be one of
		 *	a set of states selected by arbitrary criteria (such as probability).
		 *	The target is responsible for providing a next state according to its
		 *	criteria upon request.
		 */
		class MENGE_API TransitionTarget : public Element {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			TransitionTarget();

			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		tgt		TransitionTarget to copy.
			 */
			TransitionTarget( const TransitionTarget & tgt );

		protected:
			/*!
			 *	@brief		Protected destructor.
			 */
			virtual ~TransitionTarget();

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
			virtual void onEnter( Agents::BaseAgent * agent ){}

			/*!
			 *	@brief		Called when an agent exits the state with this transition.
			 *
			 *	@param		agent		The agent who left the state.
			 */
			virtual void onLeave( Agents::BaseAgent * agent ){}

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
			virtual State * nextState( Agents::BaseAgent * agent ) = 0;	

			/*!
			 *	@brief		Performs any necessary connections to the target state(s).
			 *
			 *	Sub-classes must override this because a transition must transition to
			 *	a state.  See the sub-classes SingleTransition for an example.
			 *
			 *	@param		stateMap		A mapping from state names to state pointers.
			 *	@returns	True if connection was successful, false otherwise.
			 */
			virtual bool connectStates( std::map< std::string, State * > & stateMap ) = 0;

			/*!
			 *	@brief		Create a copy of this target.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this target - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual TransitionTarget * copy() = 0;
		};

		/////////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A TransitionTarget which assumes there is only one destination
		 *				state.
		 *
		 *	This will always return a single state, regardless of the agent.
		 */
		class MENGE_API SingleTarget : public TransitionTarget {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SingleTarget();

			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		tgt		The transition target to copy.
			 */
			SingleTarget( const SingleTarget & tgt );

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		toName		The name of the destination (aka "to") node.
			 */
			SingleTarget( const std::string & toName );

			// TOOD: Change the goalPoint into a Goal instance
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
			virtual State * nextState( Agents::BaseAgent * agent ) { return _next; }	

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

			
		protected:
			/*! 
			 *	@brief		The name of the state to which this transition leads.
			 */
			 std::string	_nextName;

			/*! 
			*	@brief		a pointer to the state to which this transition leads.
			*/
			State *	_next;
		};

	}	// namespace BFSM
}	// namespace Menge
#endif	// __TARGET_H__
