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
 *	@file		Transition.h
 *	@brief		The definition of state transitions in the BFSM
 */

#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "fsmCommon.h"
#include <map>

namespace Menge {

	namespace BFSM {

		class State;
		class Condition;
		class TransitionTarget;
		class FSM;
		class Goal;

		/*!
		 *	@brief		The transition between BFSM states.
		 *
		 *	Transitions can key on arbitrary state.  For each agent, the 
		 *	transition is "tested".  If the condition is met, the transition is
		 *	active and that reports to the state (which then "follows" the transition).
		 */
		class Transition {
		public:
			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		trans		The transition to copy to this one.
			 */
			Transition( const Transition & trans );

			/*!
			 *	@brief		Constructor
			 *
			 *	The transition takes ownership of the Condition and TransitionTarget
			 *	given as parameters to the constructor and will destroy them in their
			 *	destructor.  As such, they should be unique.
			 *
			 *	@param		condition		The condition instance for this transition.
			 *	@param		target			The target instance for this transition.
			 */
			Transition( Condition * condition, TransitionTarget * target );

			/*!
			 *	@brief		Destructor.
			 */
			~Transition();

			/*!
			 *	@brief		Performs any necessary connections to the "to" state.
			 *
			 *	Sub-classes must override this because a transition must transition to
			 *	a state.  See the sub-classes SingleTransition for an example.
			 *
			 *	@param		stateMap		A mapping from state names to state pointers.
			 *	@returns	True if connection was successful, false otherwise.
			 */
			bool connectStates( std::map< std::string, State * > & stateMap );

			/*!
			 *	@brief		Called when an agent enters a state with this exiting transition.
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
			void onLeave( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Tests to see if this transition's conditions are met.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	A pointer to the next state if the test passes, otherwise
			 *				returns NULL.
			 */
			State * test( Agents::BaseAgent * agent, const Goal * goal );

			/*!
			 *	@brief		Gets the tasks for all of the transitions target and condition.
			 *
			 *	@param		fsm			A pointer to the fsm.  Tasks are fed to the fsm.
			 */
			void getTasks( FSM * fsm );	

			/*!
			 *	@brief		Creats a deep copy of this transition.
			 *
			 *	@returns	A new transition whose values are identical to this one.
			 */
			Transition * copy();

		protected:

			/*!
			 *	@brief		The Condition instance for this transition.
			 */
			Condition * _condition;

			/*!
			 *	@brief		The target for this transition.
			 */
			TransitionTarget * _target;
		};

		/*!
		 *	@brief		Parses a TinyXML element containing a transition specification
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@param		fromName		The name of the state from which the transition originates
		 *								will be set in this string.
		 *	@returns	A pointer to the new transition implementation (NULL if no valid
		 *				instance could be created).
		 */
		Transition * parseTransition( TiXmlElement * node, const std::string & behaveFldr, std::string & fromName );

	}	// namespace BFSM
}	// namespace Menge
#endif // __TRANSITION_H__
