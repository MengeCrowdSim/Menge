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

#include "SpatialQueryNavMesh.h"
#include "Tasks/NavMeshLocalizerTask.h"
#include "BaseAgent.h"
#include "NavMeshLocalizer.h"
#include "NavMeshEdge.h"
#include "NavMeshNode.h"
#include "os.h"
#include <queue>
#include <cassert>
#include <list>

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////
		//					Implementation of HELPERS
		////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A visiblity cone.  In order for an agent to be visible, they must lie
		 *				within the visibility cone.
		 *
		 */
		class VisibilityCone {
		public:
			/*!
			 *	@brief		Constructor - the visiblity cone is defined by two vectors.
			 *				The cone is assumed to be the smaller angle subtending the two
			 *				vectors.  The constructor orders them appropriately.
			 *
			 *	@param		dir0		One bound on the cone.
			 *	@param		dir1		The other bound on the cone.
			 */
			VisibilityCone( const Vector2 & dir0, const Vector2 & dir1 ) {
				if ( det( dir0, dir1 ) > 0 ) {
					_right = dir0;
					_left = dir1;
				} else {
					_right = dir1;
					_left = dir0;
				}
			}

			/*!
			 *	@brief		Reports if any portion of a line segment lies within the visibility cone.
			 *
			 *	@param		p0		One end of the line segment.
			 *	@param		p1		The other end of the line segment.
			 *	@returns	A boolean which indicates if any portion of the segment lies in the cone
			 *				(true) and is visible, or if it lies outside and is not visible (false).
			 */
			bool isVisible( const Vector2 & p0, const Vector2 & p1 ) const {
				// if either point is visible, then it is true
				float right0 = det( p0, _right );
				float left0 = det( _left, p0 );
				if ( right0 <= 0 && left0 <= 0 ) return true;

				float right1 = det( p1, _right );
				float left1 = det( _left, p1 );
				if ( right1 <= 0 && left1 <= 0 ) return true;

				// otherwise, if the two points lie outside the cone on opposite sides
				//		then the cone intersects the center.
				if ( right0 > 0 && left1 > 0 ) {
					return det( p1 - p0, -p0 ) > 0.f;
				} else if ( right1 > 0 && left0 > 0 ) {
					return det( p0 - p1, -p1 ) > 0.f;
				}
				return false;
			}

			/*! 
			 *	@brief		Reports if the point lies within the visibility cone.
			 *
			 *	@param		p		The point to test.
			 *	@returns	A boolean which indicates if the point lies in the cone (true) and
			 *				is visible, or if it lies outside and is not visible (false).
			 */
			bool isVisible( const Vector2 & p ) const {
				if ( det( p, _right ) > 0 ) return false;
				if ( det( _left, p ) > 0 ) return false;
				return true;
			}

			/*!
			 *	@brief		Intersects this cone with the given cone, changing the extent
			 *				of this cone to adhere to the intersected span.
			 *
			 *	@param		cone		The cone to intersect this cone with.
			 *	@returns	A boolean reporting if there is a non-empty intersection (true)
			 *				or if there is no proper intersection (false).
			 */
			bool intersect( const VisibilityCone & cone ) {
				Vector2 iRight( det( _right, cone._right ) > 0 ? cone._right : _right );
				Vector2 iLeft( det( _left, cone._left ) > 0 ? _left : cone._left );
				if ( det( iRight, iLeft ) > 0 ) {
					_right.set( iRight );
					_left.set( iLeft );
					return true;
				} else {
					return false;
				}
			}

			/*!
			 *	@brief		The left bound of the cone.
			 */
			Vector2 _left;

			/*!
			 *	@brief		the right bound of the cone.
			 */
			Vector2 _right;
		};



		/*!
		 *	@brief		An entry of a nav mesh node including the distance to
		 *				the node.  Used in the queue to search nearby nodes.
		 */
		class NeighborEntry {
		public:
			/*!
			 *	@brief		Constructor
			 *
			 *	@param		distSq			The squared distance from some reference point
			 *								to the nearest point on the mesh polygon.
			 *	@param		cone			The visibility cone, in which all agents and portals must lie,
			 *								to be considered viable candidates.
			 *	@param		nodeID			The identifier of the navigation mesh node.
			 */
			NeighborEntry( float distSq, const VisibilityCone & cone, unsigned int nodeID ): _distSq(distSq), _cone(cone), _nodeID(nodeID) {
			}

			/*!
			 *	@brief		Comparator - provided for creating a min_heap.
			 *
			 *	@param		entry			The other entry against which to compare this entry.
			 *	@returns	True if the squared distance for this entry is less that of the given
			 *				entry.
			 */
			bool operator<( const NeighborEntry & entry ) const {
			   return _distSq < entry._distSq;
			}

			/*!
			 *	@brief		The squared distance from some reference point to the node.
			 */
			float _distSq;

			/*!
			 *	@brief		The cone of visibility in which all agents and portals must lie
			 *				to be considered a viable candidate.
			 */
			VisibilityCone _cone;

			/*!
			 *	@brief		The identifier of the node.
			 */
			unsigned int _nodeID;
		};

		////////////////////////////////////////////////////////////////
		//					Implementation of NavMeshSpatialQuery
		////////////////////////////////////////////////////////////////

		NavMeshSpatialQuery::NavMeshSpatialQuery():_localizer(0x0) {
		}

		////////////////////////////////////////////////////////////////

		void NavMeshSpatialQuery::setAgents( const std::vector< BaseAgent * > & agents ) {
			_agents.insert( _agents.begin(), agents.begin(), agents.end() );
		}

		////////////////////////////////////////////////////////////////

		void NavMeshSpatialQuery::agentQuery( ProximityQuery *filter ) const {
			float range = filter->getMaxAgentRange();
			agentQuery(filter, range);
		}
		void NavMeshSpatialQuery::agentQuery( ProximityQuery *filter, float &rangeSq) const {
			Vector2 pt = filter->getQueryPoint();
			unsigned int currNode = _localizer->getNode( pt);
			assert( currNode != NavMeshLocation::NO_NODE && "Can't use NavMesh for spatial query if the point isn't on the mesh" );

			// This does not need any synchronization elements
			//	The writing and the reading happen in two, independent computational
			//	stages.  (i.e., the writing to the node occupancy happens in a task.)
			//	This is all read-only operations and can be done simultaneously.
			const OccupantSet * occupants = _localizer->getNodeOccupants( currNode );
			if ( occupants->size() > 1 ) {
				OccupantSetCItr itr = occupants->begin();
				for ( ; itr != occupants->end(); ++itr ) {
					
					const BaseAgent * candidate = _agents[ *itr ];
					float distSq = absSq( candidate->_pos - pt );
					if ( distSq <= rangeSq ) {
						// NOTE: This call might change rangeSq; it may shrink based on the most distant neighbor
						filter->filterAgent( candidate, distSq );

						rangeSq = filter->getMaxAgentRange();
					}

					
				}
			}

			NavMeshPtr navMesh = _localizer->getNavMesh();
			// Track which nodes have been visited
			std::set< unsigned int > visited;
			visited.insert( (unsigned int) currNode );
			// now create a min heap of nearby navigation mesh nodes to explore for neighbor candidates
			std::list< NeighborEntry > queue;

			// seed the queue with this node's adjacent nodes
			const NavMeshNode & node = navMesh->getNode( (unsigned int)currNode );
			const size_t EDGE_COUNT = node.getEdgeCount();
			for ( size_t e = 0; e < EDGE_COUNT; ++e ) {
				const NavMeshEdge * edge = node.getEdge( e );
				const NavMeshNode * otherNode = edge->getOtherByID( currNode );
				visited.insert( otherNode->getID() );
				float distSq = edge->getSqDist( pt );
				if ( distSq <= rangeSq ) {	
					queue.push_back( NeighborEntry( distSq, VisibilityCone( edge->getP0() - pt, edge->getP1() - pt ), otherNode->getID() ) );
					//cones.push_back( VisibilityCone( edge->getP0() - P, edge->getP1() - P ) );
					// edge is close enough that portions of the node are reachable
					//queue.push_back( NeighborEntry( distSq, otherNode->getID() ) );
				}
			}

			while ( queue.size() > 0 ) {
				NeighborEntry nbrEntry = *(queue.begin());
				queue.pop_front();
				if ( nbrEntry._distSq > rangeSq ) continue;

				const OccupantSet * occupants = _localizer->getNodeOccupants( nbrEntry._nodeID );
				if ( occupants->size() > 0 ) {
					OccupantSetCItr itr = occupants->begin();
					for ( ; itr != occupants->end(); ++itr ) {
						const BaseAgent * candidate = _agents[ *itr ];
						Vector2 disp( candidate->_pos - pt );
						float distSq = absSq( disp );
						if ( distSq <= rangeSq ) {
							if ( nbrEntry._cone.isVisible( disp ) ) {
								filter->filterAgent( candidate, distSq );
							}

							rangeSq = filter->getMaxAgentRange();
							
						}

					}
				}

				const NavMeshNode & node = navMesh->getNode( nbrEntry._nodeID );
				const size_t EDGE_COUNT = node.getEdgeCount();
				for ( size_t e = 0; e < EDGE_COUNT; ++e ) {
					const NavMeshEdge * edge = node.getEdge( e );

					const NavMeshNode * otherNode = edge->getOtherByID( nbrEntry._nodeID );
					if ( visited.find( otherNode->getID() ) != visited.end() ) continue;
					visited.insert( otherNode->getID() );
					
					float distSq = edge->getSqDist(pt );
					if ( distSq <= rangeSq ) {	
						Vector2 disp1 = edge->getP0() - pt;
						Vector2 disp2 = edge->getP1() - pt;
						VisibilityCone cone( disp1, disp2 );
						if ( cone.intersect( nbrEntry._cone ) ) {
							queue.push_back( NeighborEntry( distSq, cone, otherNode->getID() ) );
						}
					}
				}
			}	
		}

		////////////////////////////////////////////////////////////////

		bool NavMeshSpatialQuery::queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const {
			return true;
		}

		////////////////////////////////////////////////////////////////

		void NavMeshSpatialQuery::processObstacles() {
			// Compute obstacle convexity -- this assumes all closed polygons
			NavMeshPtr navMesh = _localizer->getNavMesh();
			const unsigned int OBST_COUNT = static_cast< unsigned int >( navMesh->getObstacleCount() );
			for ( unsigned int o = 0; o < OBST_COUNT; ++o ) {
				NavMeshObstacle & obst = navMesh->getObstacle( o );
				if ( obst._prevObstacle ) {
					obst._isConvex = leftOf( obst._prevObstacle->getP0(), obst.getP0(), obst.getP1() ) >= 0;
				} else {
					obst._isConvex = true;
				}
			}
		}

		////////////////////////////////////////////////////////////////
		void NavMeshSpatialQuery::obstacleQuery( ProximityQuery *filter) const {
			float range = filter->getMaxObstacleRange();
			obstacleQuery(filter, range);
		}

		void NavMeshSpatialQuery::obstacleQuery( ProximityQuery *filter,  float rangeSq ) const {
			Vector2 pt  = filter->getQueryPoint();
			size_t currNode = _localizer->getNode( pt );

			assert( currNode != NavMeshLocation::NO_NODE && "Can't use NavMesh for spatial query if the point isn't on the mesh" );

			const NavMeshPtr navMesh = _localizer->getNavMesh();
			const NavMeshNode & node = navMesh->getNode( (unsigned int)currNode );
			const size_t OBST_COUNT = node.getObstacleCount();
			for ( size_t o = 0; o < OBST_COUNT; ++o ) {
				const NavMeshObstacle * obst = node.getObstacle( o );
				if ( obst->pointOutside( pt ) ) {
					float distance = distSqPointLineSegment(obst->getP0(), obst->getP1(), pt);
					filter->filterObstacle( obst, distance );
				}
			}
			
		}

		/////////////////////////////////////////////////////////////////////
		
		BFSM::Task * NavMeshSpatialQuery::getTask() {
			return new BFSM::NavMeshLocalizerTask( _localizer->getNavMesh()->getName(), false /*usePlanner*/ );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshVCFactory
		/////////////////////////////////////////////////////////////////////

		NavMeshSpatialQueryFactory::NavMeshSpatialQueryFactory() : SpatialQueryFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool NavMeshSpatialQueryFactory::setFromXML( SpatialQuery * sq, TiXmlElement * node, const std::string & specFldr ) const {
			NavMeshSpatialQuery * nmsq = dynamic_cast< NavMeshSpatialQuery * >( sq );
			assert( nmsq != 0x0 && "Trying to set attributes of a navigation mesh spatial query component on an incompatible object" );

			if ( ! SpatialQueryFactory::setFromXML( nmsq, node, specFldr ) ) return false;

			// get the file name
			std::string fName;
			std::string path = os::path::join( 2, specFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			// nav mesh localizer
			NavMeshLocalizerPtr nmlPtr;
			try {
				nmlPtr = loadNavMeshLocalizer( fName, true );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the navigation mesh localizer required by the spatial query on line " << node->Row() << ".";
				return false;
			}
			nmsq->setNavMeshLocalizer( nmlPtr );

			return true;
		}
	}	// namespace Agents
}	// namespace Menge