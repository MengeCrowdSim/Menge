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
 *	@file		GoalSelectorNearestNM.h
 *	@brief		The definition of the nearest navigation mesh goal selector. 
 */

#ifndef __GOAL_SELECTOR_NEAREST_NM_H__
#define __GOAL_SELECTOR_NEAREST_NM_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "GoalSelectors/GoalSelectorSet.h"
#include "NavMesh.h"
#include "NavMeshLocalizer.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		The nearest nav mesh goal selector selects the goal from a 
		 *				goal set that is nearest to the agent in terms of path length
		 *				through the navigation mesh.
		 */
		class MENGE_API NearestNMGoalSelector : public SetGoalSelector {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NearestNMGoalSelector();

			/*!
			 *	@brief		Interface function for acquiring per-agent goals.
			 *
			 *	@param		agent		The agent for whom a goal is extracted.
			 *	@returns	A pointer to a goal.  
			 *	// TODO: Figure out who owns this goal.
			 */
			virtual Goal * getGoal( const Agents::BaseAgent * agent ) const;

			/*!
			 *	@brief		Returns a pointer to the nav mesh localizer task.
			 *
			 *	@returns		A pointer to the nav mesh localizer task.  It is the responsibility
			 *					of the caller to free the memory of the provided task by
			 *					calling its destroy method.
			 */
			virtual BFSM::Task * getTask();

			/*!
			 *	@brief		Sets the navigation mesh pointer.
			 *
			 *	@param		nm		The managed pointer to the navigation mesh.
			 */
			void setNavMesh( const NavMeshPtr & nm ) { _navMesh = nm; }

			/*!
			 *	@brief		Returns a resource pointer to the underlying navigation mesh
			 *
			 *	@returns	The navigation mesh.
			 */
			NavMeshPtr getNavMesh() { return _navMesh; }

			/*!
			 *	@brief		Returns a resource pointer to the underlying navigation mesh localizer
			 *
			 *	@returns	The navigation mesh localizer.
			 */
			NavMeshLocalizerPtr getLocalizer() { return _localizer; }

			/*!
			 *	@brief		Sets the navigation mesh localizer pointer.
			 *
			 *	@param		nml		The managed pointer to the navigation mesh localizer.
			 */
			void setNavMeshLocalizer( const NavMeshLocalizerPtr & nml ) { _localizer = nml; }

		protected:
			/*!
			 *	@brief		The navigation mesh.
			 */
			NavMeshPtr	_navMesh;

			/*!
			 *	@brief		The localizer for the navigation mesh.
			 */
			NavMeshLocalizerPtr _localizer;
			
		};

		/*!
		 *	@brief		Factory for the NearestNMGoalSelector.
		 */
		class MENGE_API NearestNMGoalSelectorFactory : public SetGoalSelectorFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NearestNMGoalSelectorFactory();

			/*!
			 *	@brief		The name of the goal selector type.
			 *
			 *	The goal selector's name must be unique among all registered goal selectors.
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the unique goal selector name.
			 */
			virtual const char * name() const { return "nearest_nav_mesh"; }

			/*!
			 *	@brief		A description of the goal selector.
			 *
			 *	Each goal selector factory must override this function.
			 *
			 *	@returns	A string containing the goal selector description.
			 */
			virtual const char * description() const {
				return  "A goal selector.  Assigns the agent the goal in the given " \
						"goal set that is *nearest* to the agent based on shortest paths " \
						"through the navigation mesh.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal selector.
			 *
			 *	@returns		A pointer to a newly instantiated GoalSelector class.
			 */
			GoalSelector * instance() const { return new NearestNMGoalSelector(); }	

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
			 *	@brief		The identifier for the "file_name" string attribute.
			 */
			size_t	_fileNameID;
		};
	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_SELECTOR_NEAREST_NM_H__