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
 *	@file		GoalSelectorExplicit.h
 *	@brief		The definition of the explicit goal selector - specifying a target
 *				goal from a set explicitly. 
 */

#ifndef __GOAL_SELECTOR_EXCPLICIT_H__
#define __GOAL_SELECTOR_EXCPLICIT_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorFactory.h"

namespace Menge {

	namespace BFSM {
		// forward declaration
		class Goal;

		/*!
		 *	@brief		The explicit goal selector makes extracts a specific goal from
		 *				a specific goal set -- both explicitly stated.
		 */
		class MENGE_API ExplicitGoalSelector : public GoalSelector {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			ExplicitGoalSelector();

			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;

			/*!
			 *	@brief		Gives the instance the opportunity to set the goal set.
			 *
			 *	This does nothing for instances which don't require a goal set.  Otherwise,
			 *	it extracts its required goal set.
			 *
			 *	@param		goalSets	A mapping from goal set identifier to goal set pointers.
			 */
			void setGoalSet( std::map< size_t, GoalSet * > & goalSets );

			/*!
			 *	@brief		Sets the goal set identifier.
			 *
			 *	@param		id		The goal set's id that belongs to this selector.
			 */
			void setGoalSetID( size_t id ) { _goalSetID = id; }

			/*!
			 *	@brief		Sets the goal identifier.
			 *
			 *	@param		id		The goal's id that belongs to this selector.
			 */
			void setGoalID( size_t id ) { _goalID = id; }

		protected:
			/*!
			 *	@brief		The id of the goal set to draw from.
			 */
			size_t	_goalSetID;

			/*!	
			 *	@brief		The goal associated with this goal selector.
			 *				During parsing, it contains the id of the goal.
			 *				After FSM construction, it contains a pointer to the
			 *				actual goal.
			 */
			union {
				size_t		_goalID;		///< The identifier for the goal
				Goal *		_goal;			///< The pointer to the goal.
			};
		};

		/*!
		 *	@brief		Factory for the ExplicitGoalSelector.
		 */
		class MENGE_API ExplicitGoalSelectorFactory : public GoalSelectorFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ExplicitGoalSelectorFactory();

			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "explicit"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector.  The goal an agent gets is " \
						"explicitly enumeraged, goal set and goal id.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new ExplicitGoalSelector(); }	

			/*!
			 *	@brief		Given a pointer to a GoalSelector instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	@param		selector	A pointer to the goal whose attributes are to be set.
			 *	@param		node		The XML node containing the goal selector attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the goal selector references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( GoalSelector * selector, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "goal_set" size_t attribute.
			 */
			size_t	_goalSetID;
		
			/*!
			 *	@brief		The identifier for the "goal" size_t attribute.
			 */
			size_t	_goalID;
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_EXCPLICIT_H__