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
 *	@file		Condition.h
 *	@brief		The basis for determining the conditions under which
 *				transitions become "active" (and are taken).
 */
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Element.h"

namespace Menge {

	// forward declarations
	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {
		// forward declaration
		class Goal;

		/*!
		 *	@brief		The base class for transition conditions.
		 *
		 *	This class is an abstract class used to define arbitrary conditions
		 *	for finite-state-machine transitions.
		 */
		class MENGE_API Condition : public Element {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Condition() : Element() {}

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			Condition( const Condition & cond ){}

		protected:
			/*!
			 *	@brief		Protected destructor.
			 */
			virtual ~Condition() {}

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
			 *	@brief		Reports if the conditions have been met.
			 *
			 *	This function defines the main functionality of a definition.
			 *	Given the current agent and its internal state, it determines if
			 *	the conditions have been met.  This should be overridden by each
			 *	sub-class.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	True if the condition has been met, false otherwise.
			 */
			virtual bool conditionMet( Agents::BaseAgent * agent, const Goal * goal ) = 0;

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy() = 0;
		};

	}	// namespace BFSM
}	// namespace Menge
#endif // __CONDITION_H__