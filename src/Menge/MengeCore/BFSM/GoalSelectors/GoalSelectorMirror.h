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
 *	@file		GoalSelectorMirror.h
 *	@brief		The definition of the "mirror" goal selector. 
 */

#ifndef __GOAL_SELECTOR_MIRROR_H__
#define __GOAL_SELECTOR_MIRROR_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorFactory.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		The mirror goal selector makes a goal by reflecting the agent's
		 *				position around the world's origin.  The reflection across the x-
		 *				and y-axes can be individually specified.
		 *
		 *	Setting the mirror settings to *not* reflect over either axis is akin to creating
		 *	the IdentityGoalSelector.
		 */
		class MENGE_API MirrorGoalSelector : public GoalSelector {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			MirrorGoalSelector();

			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;

			/*!
			 *	@brief		Sets the mirroring of the goal selector.
			 *
			 *	@param		mirrorX		Determines if the position is mirrored over the x-axis.
			 *	@param		mirrorY		Determines if the position is mirrored over the y-axis.
			 */
			inline void setMirror( bool mirrorX, bool mirrorY ) { _mirrorX = mirrorX; _mirrorY = mirrorY; }

			/*!
			 *	@brief		Sets the x-mirroring of the goal selector.
			 *
			 *	@param		mirror		Determines if the position is mirrored over the axis.
			 */
			inline void setMirrorX( bool mirror ) { _mirrorX = mirror; }

			/*!
			 *	@brief		Sets the y-mirroring of the goal selector.
			 *
			 *	@param		mirror		Determines if the position is mirrored over the axis.
			 */
			inline void setMirrorY( bool mirror ) { _mirrorY = mirror; }

			/*!
			 *	@brief		Returns the mirror x state.
			 *
			 *	@returns	The mirror x state of the selector.
			 */
			inline bool getMirrorX() const { return _mirrorX; }

			/*!
			 *	@brief		Returns the mirror y state.
			 *
			 *	@returns	The mirror y state of the selector.
			 */
			inline bool getMirrorY() const { return _mirrorY; }

		protected:
			/*!
			 *	@brief		Determines if the agent's position is reflected over the
			 *				x-axis (true) or not (false).
			 */
			bool	_mirrorX;
			
			/*!
			 *	@brief		Determines if the agent's position is reflected over the
			 *				y-axis (true) or not (false).
			 */
			bool	_mirrorY;
		};

		/*!
		 *	@brief		Factory for the MirrorGoalSelector.
		 */
		class MENGE_API MirrorGoalSelectorFactory : public GoalSelectorFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			MirrorGoalSelectorFactory();

			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "mirror"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector.  The goal an agent gets is the reflection " \
						"of the agent's position across the world's origin over the "\
						"world's x- and/or y-axes as specified.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new MirrorGoalSelector(); }	

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
			 *	@brief		The identifier for the "mirror_x" bool attribute.
			 */
			size_t	_mirrorXID;
		
			/*!
			 *	@brief		The identifier for the "mirror_y" bool attribute.
			 */
			size_t	_mirrorYID;
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_MIRROR_H__