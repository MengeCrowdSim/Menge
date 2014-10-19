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
 *	@file		GoalSelectorSet.h
 *	@brief		The definition of the base class for handling goal selectors which 
 *				operate on single goal sets. 
 */

#ifndef __GOAL_SELECTOR_SET_H__
#define __GOAL_SELECTOR_SET_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorFactory.h"
#include "GoalSet.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		This is a base class -- not to be instantiated.  It provides
		 *				the functional basis for GoalSelectors which use a GoalSet
		 *				as the underlying data structure.
		 */
		class MENGE_API SetGoalSelector : public GoalSelector {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			SetGoalSelector();

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
			virtual void lockResources() { _goalSet->lockRead(); }

			/*! 
			 *	@brief		Allows the goal selector to release previously locked
			 *				resources.
			 *
			 *	Should be used in conjunction with lockResources.
			 */
			virtual void releaseResources() { _goalSet->releaseRead(); }

			/*!	
			 *	@brief		The goal set associated with this goal selector.
			 *				During parsing, it contains the id of the goal set.
			 *				After FSM construction, it contains a pointer to the
			 *				actual goal set.
			 */
			union {
				size_t		_goalSetID;		///< The identifier for the goal set
				GoalSet *	_goalSet;		///< The pointer to the goal set.
			};
		};

		/*!
		 *	@brief		Factory for the SetGoalSelector.
		 */
		class MENGE_API SetGoalSelectorFactory : public GoalSelectorFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			SetGoalSelectorFactory();

		protected:
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
		};
	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_SET_H__