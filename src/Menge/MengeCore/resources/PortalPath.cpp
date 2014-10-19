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

#include "PortalPath.h"
#include "WayPortal.h"
#include "BaseAgent.h"
#include "PathPlanner.h"
#include "NavMeshLocalizer.h"
#include "Funnel.h"
#include "Goals/Goal.h"
#include "fsmCommon.h"
#include "Core.h"
#include <cassert>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of PortalPath
	/////////////////////////////////////////////////////////////////////

	PortalPath::PortalPath( const Vector2 & startPos, const BFSM::Goal * goal, const PortalRoute * route, float agentRadius ): _route(route), _goal(goal), _currPortal(0), _waypoints(0x0), _headings(0x0) {
		computeCrossing( startPos, agentRadius );
	}

	/////////////////////////////////////////////////////////////////////

	PortalPath::~PortalPath() {
		if ( _waypoints ) delete [] _waypoints;
		if ( _headings ) delete [] _headings;
	}

	/////////////////////////////////////////////////////////////////////

	void PortalPath::setPreferredDirection( const Agents::BaseAgent * agent, float headingCos, Agents::PrefVelocity & pVel ) {
		const size_t PORTAL_COUNT = _route->getPortalCount();
		Vector2 dir;
		if ( _currPortal >= PORTAL_COUNT ) {
			// assume that the path is clear
			// TODO: See GoalVC
			_goal->setDirections( agent->_pos, agent->_radius, pVel );
			
			// speed
			Vector2 goalPoint = pVel.getTarget();
			Vector2 disp = goalPoint - agent->_pos;
			const float distSq = absSq( disp );
			float speed = agent->_prefSpeed;

			if ( distSq <= 0.0001f ) {
				// I've basically arrived -- speed should be zero.
				speed = 0.f;
			} else {
				const float speedSq = speed * speed;
				const float TS_SQD = SIM_TIME_STEP * SIM_TIME_STEP;
				if ( distSq / speedSq < TS_SQD ) {
					// The distance is less than I would travel in a single time step.
					speed = sqrtf( distSq ) / SIM_TIME_STEP;
				}
			}
			pVel.setSpeed( speed );
		} else {
			const WayPortal * portal = _route->getPortal( _currPortal );
			Vector2 goalDir( _waypoints[ _currPortal ] - agent->_pos );
			float dist = abs( goalDir );
			// If the displacement to the next way point is large enough
			//	(i.e., not essentially zero), use it, otherwise, peek
			//	into the next waypoint.
			//
			//	The goal is to always provide a goalDir to the portal
			//	that is well-defined and unit-length.
			bool bigEnough = dist >= EPS;
			if ( bigEnough ) {
				goalDir /= dist;
				if ( goalDir * _headings[ _currPortal ] < headingCos ) {
					// Heading has deviated too far recompute crossing
					FunnelPlanner planner;
					planner.computeCrossing( agent->_radius, agent->_pos, this, _currPortal );
					goalDir = _waypoints[ _currPortal ] - agent->_pos;
					dist = abs( goalDir );
					if ( bigEnough = ( dist >= EPS ) ) {
						goalDir /= dist;
					}
				}
			} 
			if ( ! bigEnough ) {
				// simply cross the wayportal perpendicularly
				//goalDir.set( portal->getCrossingDir( agent->_pos ) );
				if ( _currPortal + 1 < getPortalCount() ) {
					// calculate w.r.t. next waypoint
					goalDir = norm( _waypoints[ _currPortal + 1 ] - agent->_pos );
				} else {
					// calculate w.r.t. goal
					Vector2 gp;
					_goal->getTargetPoint( gp, agent->_radius );
					goalDir = norm( gp - agent->_pos );
				}
			}
			assert( abs( goalDir ) > EPS && "Providing a goal direction that is too small" );
			
			pVel.setTarget( _waypoints[ _currPortal ] );
			portal->setPreferredDirection( agent->_pos, agent->_radius, goalDir, pVel );
		}
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int PortalPath::updateLocation( const Agents::BaseAgent * agent, const NavMeshPtr & navMesh, const NavMeshLocalizer * localizer, PathPlanner * planner ) {
		// If off path, replan get a new route
		// TODO: If off "approach" vector, recompute crossing
		bool changed = false;
		unsigned int currNodeID = getNode();
		const NavMeshNode * currNode = &(navMesh->getNode( currNodeID ) );
		// test current location
		const Vector2 & p = agent->_pos;

		const unsigned int PORTAL_COUNT = static_cast< unsigned int >( _route->getPortalCount() );
		if ( ! currNode->containsPoint( p ) ) {
			// test to see if I've progressed to the next
			
			bool gotoNext = false;
			const NavMeshNode * nextNode = 0x0;
			if ( _currPortal + 1 < PORTAL_COUNT ) {
				// there is another way portal to test
				const WayPortal * nextPortal = _route->getPortal( _currPortal + 1 );
				size_t nextID = nextPortal->_nodeID;
				nextNode = &(navMesh->getNode( (unsigned int) nextID ) );
				gotoNext = nextNode->containsPoint( p );
			} else if ( _currPortal < PORTAL_COUNT ) {
				// the next node is the goal polygon
				nextNode = &(navMesh->getNode( (unsigned int) _route->getEndNode() ) );
				gotoNext = nextNode->containsPoint( p );
			}
			if ( gotoNext ) {
				// I've made progress, simply advance
				++_currPortal;	
				assert( _currPortal <= PORTAL_COUNT && "Incremented current portal larger than goal" );
				changed = true;
			}
			 else {
				const NavMeshNode * prevNode = 0x0;
				// test to see if I've gone backwards
				bool gotoPrev = false;
				if ( _currPortal > 0 ) {
					const WayPortal * prevPortal = _route->getPortal( _currPortal - 1 );
					size_t prevID = prevPortal->_nodeID;
					prevNode = &(navMesh->getNode( (unsigned int) prevID ) );
					gotoPrev = prevNode->containsPoint( p );
				}
				if ( gotoPrev ) {
					// back up to previous way portal in path
					--_currPortal;
					changed = true;
				} else {
					// Agent is not in current, previous or next polygons - agent got
					//	pushed off path - find a new path
					//	Path needs the nav mesh
					// Assume that I must be in a neighboring node (the alternative is
					//	catstrophic)
					// search current node's neighbors that aren't previous and aren't next
					const size_t NBR_COUNT = currNode->getNeighborCount();
					for ( size_t n = 0; n < NBR_COUNT; ++n ) {
						const NavMeshNode * node = currNode->getNeighbor( n );
						if ( node == nextNode || node == prevNode ) continue;
						if ( node->containsPoint( p ) ) {
							// find a new path from this node to the goal
							replan( p, node->getID(), _route->getEndNode(), agent->_radius, planner );
							changed = true;
						} 
					}

					// It is possible for the agent, in some cases, to advance several nodes in a single time step
					//	(e.g., when the navigation mesh has many long, skinny triangles and the agent steps across the
					//	narrow fan).
					//	In this case, the agent should search forwards along the path before blindly searching.

					//	TODO:
					//		If it gets "lost" at the beginning of a long path, I'm doing a bunch of wasted testing.
					//		Given how far the agent is from a particular portal, I know I should probably stop
					//		looking as the portals are only going to get farther.  So, that means the inside
					//		query should CHEAPLY compute some sense of distance to the polygon so I can drop
					//		out.
					if ( changed == false ) {
						size_t testPortal = _currPortal + 2;
						while ( testPortal < PORTAL_COUNT ) {
							const WayPortal * nextPortal = _route->getPortal( testPortal );
							size_t testID = nextPortal->_nodeID;
							const NavMeshNode * testNode = &(navMesh->getNode( (unsigned int) testID ) );
							if ( testNode->containsPoint( p ) ) {
								_currPortal = testPortal;
								changed = true;
								break;
							}
							++testPortal;
						}
					}
					if ( changed == false ) {
						// I exited the loop without finding an intermediate node -- test the goal node
						const NavMeshNode * testNode = &(navMesh->getNode( (unsigned int) _route->getEndNode() ) );
						if ( testNode->containsPoint( p ) ) {
							_currPortal = PORTAL_COUNT;
							changed = true;
						}
					}

					if ( !changed ) {
	#ifdef _DEBUG
						logger << Logger::WARN_MSG << "Agent " << agent->_id << " got pushed from its goal into a non-adjacent goal!!\n";
	#endif
						// do a full find path searching for the agent position
						float lastElevation = currNode->getElevation( p );
						unsigned int nodeID = localizer->findNodeBlind( p, lastElevation );
						if ( nodeID != NavMeshLocation::NO_NODE ) {
							try {
								replan( p, nodeID, _route->getEndNode(), agent->_radius, planner );
							} catch ( PathPlannerException ) {
								std::stringstream ss;
								ss << "Agent " << agent->_id << " trying to find a path from " << nodeID << " to " << _route->getEndNode() << ".  A* finished without a route!";
								throw PathPlannerException( ss.str() );
							}
						}
						changed = true;
					}
				}
			}
		}
		/*
		// TODO: Implement the idea of replanning the path based on getting pushed off
		//		approach vector
		if ( !changed && _currPortal < _route->getPortalCount() ) {
			// vector from crossing point to current position.
			//	examine angle between original approach vector and current approach vector.
			//	If the angle > some threshold, replan.
		}
		*/
		if ( _currPortal < _route->getPortalCount()  ) {
			return _route->getPortal( _currPortal )->_nodeID;
		} else {
			return _route->getEndNode();
		}
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int PortalPath::getNode() const { 
		if ( _currPortal == _route->getPortalCount() ) {
			return _route->getEndNode();
		} else {
			return _route->getPortalNode( _currPortal ); 
		}
	}

	/////////////////////////////////////////////////////////////////////

	void PortalPath::computeCrossing( const Vector2 & startPos, float agentRadius  ) {
		const size_t PORTAL_COUNT = _route->getPortalCount();
		if ( PORTAL_COUNT > 0 ) {
			assert( _waypoints == 0x0 && "Computing the crossing for a path that already exists" );
			_currPortal = 0;
			_waypoints = new Vector2[ PORTAL_COUNT ];
			_headings = new Vector2[ PORTAL_COUNT ];
			FunnelPlanner planner;
			planner.computeCrossing( agentRadius, startPos, this );
		} 
	}

	/////////////////////////////////////////////////////////////////////

	void PortalPath::replan( const Vector2 & startPos, unsigned int startNode, unsigned int endNode, float agentRadius, PathPlanner * planner ) {
		PortalRoute * route = planner->getRoute( startNode, _route->getEndNode(), agentRadius * 2.f );
		if ( _waypoints != 0x0 ) {
			delete [] _waypoints;
			_waypoints = 0x0;
			delete [] _headings;
			_headings = 0x0;
		}
		_currPortal = 0;
		_route = route;
		computeCrossing( startPos, agentRadius );
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 PortalPath::getWayPoint(size_t i) const {
		const size_t PORTAL_COUNT = _route->getPortalCount();
		assert( i >= 0 && i < PORTAL_COUNT && "Invalid index into the path!" );
		return _waypoints[ i ];
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int PortalPath::getNode( size_t i ) const {
		const size_t PORTAL_COUNT = _route->getPortalCount();
		assert( i >= 0 && i <= PORTAL_COUNT && "Invalid index into the path!" );
		if ( i < PORTAL_COUNT ) {
			return _route->getPortalNode( i );
		} else {
			return _route->getEndNode();
		}
	}

	/////////////////////////////////////////////////////////////////////

	void PortalPath::setWaypoints( size_t start, size_t end, const Vector2 & p0, const Vector2 & dir ) {
		for ( size_t i = start; i < end; ++i ) {
			_waypoints[ i ].set( _route->getPortal( i )->intersectionPoint( p0, dir ) );
			//_waypoints[ i ].set( p0 );
			_headings[ i ].set( dir );
		}
	}
}	// namespace Menge