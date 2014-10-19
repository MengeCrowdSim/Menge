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
 *	@file		GoalSelectorOffset.h
 *	@brief		The definition of the "offset" goal selector. 
 */

#ifndef __GOAL_SELECTOR_OFFSET_H__
#define __GOAL_SELECTOR_OFFSET_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorFactory.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		The offset goal selector makes a goal by applying an offset value
		 *				to the agent's position at assignment time.  The offsets can be
		 *				defined with a distribution.
		 */
		class MENGE_API OffsetGoalSelector : public GoalSelector {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			OffsetGoalSelector();

		protected:
			/*!
			 *	@brief		Destructor.
			 */
			~OffsetGoalSelector();

		public:
			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;

			/*!
			 *	@brief		Set the 2D number distribution for the goal selector.
			 *
			 *	The goal selector takes ownership of the provided 2D number generator
			 *	and will delete it upon its own destruction.
			 *
			 *	@param		gen		A pointer to the 2D generator.  
			 */
			void setDistribution( Vec2DGenerator * gen ) { _2DVel = gen; }

		protected:
			/*!
			 *	@brief		A vector distribution for the offset value.
			 */
			mutable Vec2DGenerator * _2DVel;	
		};

		/*!
		 *	@brief		Factory for the OffsetGoalSelector.
		 */
		class MENGE_API OffsetGoalSelectorFactory : public GoalSelectorFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			OffsetGoalSelectorFactory();

			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "offset"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector.  The goal an agent gets is the agent's " \
						"position offset by some user-specified 2D distribution.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new OffsetGoalSelector(); }	

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
			 *	@brief		The identifier for the 2D vector generator attribute.
			 */
			size_t	_offsetID;
		};
	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_OFFSET_H__