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
 *	@file		CondAuto.h
 *	@brief		The definition of the automatic condition.
 */

#ifndef __COND_AUTO_H__
#define __COND_AUTO_H__

#include "CoreConfig.h"
#include "Condition.h"
#include "ConditionFactory.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The definition of the "auto" condition. 
		 *
		 *	The auto condition always evaluates to true.  It can be used
		 *	to automatically progress from one state to another -- or as a default
		 *	case when multiple transitions are possible and the transition with
		 *	the auto condition should be taken when no other transition proved
		 *	to be valid.
		 */
		class MENGE_API AutoCondition : public Condition {
		public:
			/*!
			 *	@brief		The AutoCondition is always met -- it is a tautology.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	True if the condition has been met, false otherwise.
			 */
			virtual bool conditionMet( Agents::BaseAgent * agent, const Goal * goal ) { return true; }

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy() { return new AutoCondition(); }
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for creating the AutoCondition
		 */
		class MENGE_API AutoCondFactory : public ConditionFactory {
		public:
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "auto"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The tautological condition.  It always evaluates to true.  "\
					"If tested, the transition will be active.";
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
			 *	@returns		A pointer to a newly instantiated Condition class.
			 */
			virtual Condition * instance() const { return new AutoCondition(); }
		};

	}	// namespace BFSM
}	// namespace Menge
#endif // __COND_AUTO_H__