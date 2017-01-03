/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFarthestNM.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Tasks/NavMeshLocalizerTask.h"
#include "MengeCore/resources/PathPlanner.h"
#include "MengeCore/resources/Route.h"
#include "MengeCore/Runtime/os.h"

#include <cassert>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of FarthestNMGoalSelector
		/////////////////////////////////////////////////////////////////////
		
		FarthestNMGoalSelector::FarthestNMGoalSelector() : SetGoalSelector(), _navMesh(0x0),
														   _localizer(0x0) {
		}

		/////////////////////////////////////////////////////////////////////
		
		Goal * FarthestNMGoalSelector::getGoal( const Agents::BaseAgent * agent ) const {
			const size_t GOAL_COUNT = _goalSet->size();
			if ( GOAL_COUNT == 0 ) {
				logger << Logger::ERR_MSG;
				logger << "FarthestNMGoalSelector was unable to provide a goal for agent ";
				logger << agent->_id << ".  There were no available goals in the goal set.";
				return 0x0;
			}
			
			// 1. Determine which node the agent is in.  
			unsigned int start = _localizer->updateLocation( agent, true );
			
			if ( start == NavMeshLocation::NO_NODE ) {
				logger << Logger::ERR_MSG;
				logger << "Nav Mesh Goal Selector trying to find a goal for an agent who is not "
					"on the navigation mesh: Agent " << agent->_id << ".";
				throw GoalSelectorException();
			}

			float agentDiameter = 2.f * agent->_radius;

			Goal * bestGoal = 0x0;
			float bestDist = 0.f;

			for ( size_t i = 0; i < GOAL_COUNT; ++i ) {
				Goal * testGoal = _goalSet->getIthGoal( i );
				unsigned int testNode = _localizer->getNode( testGoal->getCentroid() );
				if ( testNode == NavMeshLocation::NO_NODE ) {
					// silently skip it.  The centroid is not on the mesh
					continue;
				}
				PortalRoute * route = _localizer->getPlanner()->getRoute( start, testNode,
																		  agentDiameter );
				float length = route->getLength();
				if ( length > bestDist ) {
					bestDist = length;
					bestGoal = testGoal;
				}
			}
			// 2. For each goal in the set, determine which node the center is in (NavMeshLocalizer)
			//		a. For each start-end goal pair, determine the shortest passable path.  (PathPlanner)
			//		b. based on _getNearest, take the nearest/farthest.
			//
			if ( bestGoal == 0x0 ) {
				logger << Logger::ERR_MSG;
				logger << "Nav mesh Goal Selector was unable to find a path from agent ";
				logger << agent->_id << " to any goal in its goal set.";
				throw GoalSelectorException();
			}
			return bestGoal;
		}

		/////////////////////////////////////////////////////////////////////
		
		BFSM::Task * FarthestNMGoalSelector::getTask() {
			return new NavMeshLocalizerTask( _navMesh->getName(), true /*usePlanner*/ );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of FarthestNMGoalSelectorFactory
		/////////////////////////////////////////////////////////////////////

		FarthestNMGoalSelectorFactory::FarthestNMGoalSelectorFactory() : SetGoalSelectorFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool FarthestNMGoalSelectorFactory::setFromXML( GoalSelector * selector,
														TiXmlElement * node,
														const std::string & behaveFldr ) const {
			FarthestNMGoalSelector * nmgs = dynamic_cast< FarthestNMGoalSelector * >( selector );
			assert( nmgs != 0x0 &&
					"Trying to set attributes of a farthest navigation mesh-based goal selector "
					"on an incompatible object" );

			if ( ! SetGoalSelectorFactory::setFromXML( nmgs, node, behaveFldr ) ) return false;

			// create full path to the nav mesh resource
			std::string fName;
			std::string path = os::path::join( 2,
											   behaveFldr.c_str(),
											   _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			// nav mesh
			NavMeshPtr nmPtr;
			try {
				nmPtr = loadNavMesh( fName );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG;
				logger << "Couldn't instantiate the navigation mesh referenced on line ";
				logger << node->Row() << ".";
				return false;
			}
			nmgs->setNavMesh( nmPtr );
			// nav mesh localizer
			NavMeshLocalizerPtr nmlPtr;
			try {
				nmlPtr = loadNavMeshLocalizer( fName, true );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG;
				logger << "Couldn't instantiate the navigation mesh localizer required by the "
					"goal selector on line " << node->Row() << ".";
				return false;
			}
			nmgs->setNavMeshLocalizer( nmlPtr );

			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge
