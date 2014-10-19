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
 *	@file		TargetProb.h
 *	@brief		Defines a transition target probabilistically.
 */

#ifndef __TARGET_PROB_H__
#define	__TARGET_PROB_H__

#include "CoreConfig.h"
#include "Target.h"
#include "TargetFactory.h"
#include "fsmCommon.h"
#include <list>

namespace Menge {

	namespace BFSM {

		// forward declarations
		class State;
		class ProbTargetFactory;

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The definition of the probabalistic target. 
		 *
		 *	Typically, the FSM will be deterministic in that if an agent is in
		 *	a current state, with a particualr condition being true, it will
		 *	transition to a deterministic state.  The probabalistic target allows
		 *	the transition to be implicitly connected to a number of states.
		 *	Upon transitioning, the destination state is computed randomly
		 *	from the set.
		 */
		class MENGE_API ProbTarget : public TransitionTarget {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ProbTarget();

			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		tgt		The target to copy.
			 */
			ProbTarget( const ProbTarget & tgt );

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
			
			friend class ProbTargetFactory;
		protected:
			/*!
			 *	@brief		The random number generator for selecting the next state.
			 */
			UniformFloatGenerator _randNum;

			/*!
			 *	@brief		The total weight of all the target states.
			 *
			 *				This gets set after a call to connectStates.
			 */
			float	_totalWeight;

			/*!
			 *	@brief		The set of target state *names* and their relative weights.
			 */
			std::list< std::pair< float, std::string > > _targetNames;

			/*!
			 *	@brief		The set of target states and their corresponding relative
			 *				weights.
			 */
			std::map< State *, float > _targets;	
		};
		
		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for creating the ProbTarget
		 */
		class MENGE_API ProbTargetFactory : public TargetFactory {
		public:
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "prob"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The probabalistic transition target.  This allows a state to"\
					" transition to a randomly selected member of a set of states. "\
					"The state selected is based on weighted probabilities.";
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
			 *	@returns		A pointer to a newly instantiated TransitionTarget class.
			 */
			virtual TransitionTarget * instance() const { return new ProbTarget(); }

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
			 *	@param		target			A pointer to the transition target whose attributes are to be set.
			 *	@param		node			The XML node containing the condition attributes.
			 *	@param		behaveFldr		The path to the behavior file.  If the condition references
			 *								resources in the file system, it should be defined relative
			 *								to the behavior file location.  This is the folder containing
			 *								that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( TransitionTarget * target, TiXmlElement * node, const std::string & behaveFldr ) const;
		};
		
	}	// namespace BFSM
}	// namespace Menge
#endif	// __TARGET_PROB_H__