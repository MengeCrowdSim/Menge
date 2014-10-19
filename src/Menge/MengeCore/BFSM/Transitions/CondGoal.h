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
 *	@file		CondGoal.h
 *	@brief		The definition of the goal reached condition.
 */

#ifndef __COND_GOAL_H__
#define __COND_GOAL_H__

#include "CoreConfig.h"
#include "Condition.h"
#include "ConditionFactory.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A spatial transition based on individual goal positions.
		 */
		class MENGE_API GoalCondition : public Condition {
		public:
			/*! 
			 *	@brief		Constructor.
			 */
			GoalCondition();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			GoalCondition( const GoalCondition & cond );

			/*!
			 *	@brief		The GoalCondition's condition is met when the
			 *				agent is within a distance to the goal.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	True if the condition has been met, false otherwise.
			 */
			virtual bool conditionMet( Agents::BaseAgent * agent, const Goal * goal );

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy();

			/*!
			 *	@brief		Set the minimum distance of approach.
			 *
			 *	@param		dist	The minimum distance to the goal to consider the condition met.
			 */
			void setMinDistance( float dist ) { _distSq = dist * dist; }

		protected:
			/*!
			 *	@brief		Minimum distance of approach (squared for efficiency).
			 */
			float	_distSq;
		};

		/*!
		 *	@brief		The factory for creating the GoalCondition
		 */
		class MENGE_API GoalCondFactory : public ConditionFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			GoalCondFactory();

			/*!
			 *	@brief		The name of the condition.
			 *
			 *	The condition's name must be unique among all registered condition.
			 *	Each condition factory must override this function.
			 *
			 *	@returns	A string containing the unique condition name.
			 */
			virtual const char * name() const { return "goal_reached"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The goal condition.  It becomes active when an agent reaches "\
					"a user-specified distance to the goal.";
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
			virtual Condition * instance() const { return new GoalCondition(); }

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
			 *	@brief		The identifier for the "distance" float attribute.
			 */
			size_t	_distanceID;
		};

	}	// namespace BFSM
}	// namespace Menge
#endif // __COND_GOAL_H__