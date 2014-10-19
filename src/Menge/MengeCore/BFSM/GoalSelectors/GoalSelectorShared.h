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
 *	@file		GoalSelectorShared.h
 *	@brief		The definition of the "shared" goal selector. 
 */

#ifndef __GOAL_SELECTOR_SHARED_H__
#define __GOAL_SELECTOR_SHARED_H__

#include "fsmCommon.h"
#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorFactory.h"

namespace Menge {

	// forward declarations
	namespace Agents {
		class SimulatorInterface;
	}

	namespace BFSM {
		// forward declarations
		class SharedGoalSelectorFactory;
		class FSM;
		class FSMDescrip;
		FSM * buildFSM( FSMDescrip & fsmDescrip, Agents::SimulatorInterface * sim, bool VERBOSE );

		/*!
		 *	@brief		The shared goal selector is a stub goal selector
		 *				used strictly internally for bookkeeping.  It is used
		 *				to implement shared goal selectors between states.  It
		 *				is not intended to be sub-classed..
		 */
		class SharedGoalSelector : public GoalSelector {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			SharedGoalSelector();

			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;

			friend class SharedGoalSelectorFactory;
			friend FSM * buildFSM( FSMDescrip & fsmDescrip, Agents::SimulatorInterface * sim, bool VERBOSE );
		protected:
			/*!
			 *	@brief		The name of the state which will provide the actual
			 *				goal selector instance.
			 */
			std::string _stateName;

			/*!
			 *	@brief		The line number this was defined on (for use with
			 *				logger messages.
			 */
			int _lineNo;
		};

		/*!
		 *	@brief		Factory for the SharedGoalSelector.
		 */
		class SharedGoalSelectorFactory : public GoalSelectorFactory { 
		public:
			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "shared"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector stub.  Used to realize the \"shared\" " \
					"goal selector semantics.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new SharedGoalSelector(); }	

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
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_SHARED_H__