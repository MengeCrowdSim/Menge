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

#include "NavMeshLocalizer.h"
#include "NavMeshNode.h"
#include "PortalPath.h"
#include "BaseAgent.h"
#include "PathPlanner.h"
#include <limits>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of NavMeshLocation
	/////////////////////////////////////////////////////////////////////

	const unsigned int NavMeshLocation::NO_NODE = std::numeric_limits< unsigned int >::max();

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocation::setNode( unsigned int nodeID ) {
		if ( _hasPath ) {
			delete _path;
			_hasPath = false;
		}
		_nodeID = nodeID;
	}

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocation::clearPath() {
		if ( _hasPath ) {
			unsigned int node = _path->getNode();
			delete _path;
			_hasPath = false;
			_nodeID = (size_t)node;
		}
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocation::getNode() const {
		//TODO: NavMesh nodes should simply be size_ts and NOT unsigned ints.
		if ( _hasPath ) {
			return (unsigned int)_path->getNode();
		} else {
			return (unsigned int)_nodeID;
		}
	}

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocation::setPath( PortalPath * path ) {
		if ( _hasPath ) {
			delete _path;
		}
		_path = path;
		_hasPath = true;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of NavMeshLocalizer
	/////////////////////////////////////////////////////////////////////

	const std::string NavMeshLocalizer::LABEL("navmesh_localizer");

	/////////////////////////////////////////////////////////////////////

	NavMeshLocalizer::NavMeshLocalizer( const std::string & name ): Resource(name), _navMesh(0x0),_trackAll(false),_planner(0x0) {
		try {
			_navMesh = loadNavMesh( name );
		} catch ( ResourceException ) {
			logger << Logger::ERR_MSG << "Couldn't instantiate navigation mesh localizer for navigation mesh: " << name << "."; 
			throw ResourceException();
		}
		const size_t NODE_COUNT = _navMesh->getNodeCount();
		_nodeOccupants = new OccupantSet[ NODE_COUNT + 1 ];
	}

	/////////////////////////////////////////////////////////////////////

	NavMeshLocalizer::~NavMeshLocalizer() {
		delete [] _nodeOccupants;
	}

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocalizer::clearPath( size_t agentID ) {
		_locLock.lockRead();
		if ( _locations.count( agentID ) > 0 ) {
			_locations[ agentID ].clearPath();
		}
		_locLock.releaseRead();
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocalizer::getNode( const Agents::BaseAgent * agent ) const {
		unsigned int node = NavMeshLocation::NO_NODE;
		_locLock.lockRead();
		if ( _locations.count( agent->_id ) > 0 ) {
			node = _locations[ agent->_id ].getNode();
		}
		_locLock.releaseRead();
		return node;
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocalizer::getNode( const Vector2 & p ) const {
		return findNodeBlind( p );
	}

	/////////////////////////////////////////////////////////////////////

	PortalPath * NavMeshLocalizer::getPath( size_t id ) {
		PortalPath * path = 0x0;
		_locLock.lockRead();
		if ( _locations.count( id ) > 0 ) {
			if ( _locations[ id ].isPath() ) {
				path = _locations[ id ]._path;
			}
		} 
		_locLock.releaseRead();
		return path;
	}

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocalizer::setPath( size_t agentID, PortalPath * path ) { 
		_locLock.lockWrite();
		_locations[ agentID ].setPath( path ); 
		_locLock.releaseWrite();
	}

	/////////////////////////////////////////////////////////////////////

	void NavMeshLocalizer::setNode( size_t agentID, unsigned int nodeID ) { 
		_locLock.lockWrite();
		_locations[ agentID ].setNode( nodeID ); 
		_locLock.releaseWrite();
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocalizer::updateLocation( const Agents::BaseAgent * agent, bool force ) const {
		const size_t ID = agent->_id;
		// NOTE: This will create a default location instance if the agent didn't already
		//	have one
		_locLock.lockRead();
		NavMeshLocation & loc = _locations[ ID ];
		_locLock.releaseRead();
		unsigned int oldLoc = loc.getNode();
		unsigned int newLoc = oldLoc;
		if ( loc._hasPath ) {
			newLoc = loc._path->updateLocation( agent, _navMesh, this, _planner );
		} else { //if ( _trackAll || force ) {
			const Vector2 & p = agent->_pos;
			unsigned int oldNode = (unsigned int)loc._nodeID;
			if ( loc._nodeID == NavMeshLocation::NO_NODE ) {
				loc._nodeID = findNodeBlind( p );
			} else {
				const NavMeshNode & node = _navMesh->getNode( (unsigned int)loc._nodeID );
				if ( ! node.containsPoint( p ) ) {	// not in current node
					loc._nodeID = testNeighbors( node, p );
					if ( loc._nodeID == NavMeshLocation::NO_NODE ) {
						loc._nodeID = findNodeBlind( p );
					}
				}
			}
			if ( loc._nodeID == NavMeshLocation::NO_NODE ) {
				loc._nodeID = oldNode;
			}
			newLoc = (unsigned int)loc._nodeID;
		}
		
		if ( newLoc != oldLoc ) {
			if ( newLoc == NavMeshLocation::NO_NODE ) {
				newLoc = static_cast< unsigned int>( _navMesh->getNodeCount() );
			}

			// remove the agent from the set for oldLoc and place it in newLoc
			#pragma omp critical( NAV_MESH_LOCALIZER_MOVE_AGENT )
			{
				if ( oldLoc != NavMeshLocation::NO_NODE ) {
					OccupantSetItr fromItr = _nodeOccupants[ oldLoc ].find( ID );
					if ( fromItr != _nodeOccupants[ oldLoc ].end() ) {
						_nodeOccupants[ oldLoc ].erase( fromItr );
					} else if ( oldLoc != NavMeshLocation::NO_NODE ) {
						logger << Logger::ERR_MSG << "Trying to remove agent " << ID << " from node " << oldLoc << " but it has not been assigned to that node.";
						const size_t NCOUNT = _navMesh->getNodeCount();
						for ( size_t i = 0; i < NCOUNT; ++i ) {
							fromItr = _nodeOccupants[ i ].find( ID );
							if (  fromItr != _nodeOccupants[ i ].end() ) {
								logger << "\n\tFound the agent in node: " << i << ".";
								_nodeOccupants[ i ].erase( fromItr );
								break;
							}
						}
					}	
				}
				_nodeOccupants[ newLoc ].insert( ID );
			}
		}
		
		return newLoc;
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocalizer::findNodeBlind( const Vector2 & p, float tgtElev ) const {
		const unsigned int nCount =  static_cast< unsigned int >( _navMesh->getNodeCount() );
		float elevDiff = 1e6f;
		unsigned int maxNode = NavMeshLocation::NO_NODE;
		for ( unsigned int n = 0; n < nCount; ++n ) {
			const NavMeshNode & node = _navMesh->getNode( n );
			if ( node.containsPoint( p ) ) {
				float hDiff = fabs( node.getElevation( p ) - tgtElev );
				if ( hDiff < elevDiff ) {
					maxNode = n;
					elevDiff = hDiff;
				}
			}
		}
		return maxNode;
	}

	/////////////////////////////////////////////////////////////////////

	unsigned int NavMeshLocalizer::testNeighbors( const NavMeshNode & node, const Vector2 & p ) const {
		const unsigned int nCount = static_cast< unsigned int>( node.getNeighborCount() );
		for ( unsigned int n = 0; n < nCount; ++n ) {
			const NavMeshNode * nbr = node.getNeighbor( n );
			if ( nbr->containsPoint( p ) ) {
				return nbr->getID();
			}
		}
		return NavMeshLocation::NO_NODE;
	}
		
	/////////////////////////////////////////////////////////////////////

	Resource * NavMeshLocalizer::load( const std::string & fileName ) {
		NavMeshPtr mesh;
		try {
			mesh = loadNavMesh( fileName );
		} catch ( ResourceException ) {
			return 0x0;
		}
		NavMeshLocalizer * nml = new NavMeshLocalizer( fileName );
		nml->_navMesh = mesh;
		return nml;
	}

	/////////////////////////////////////////////////////////////////////

	NavMeshLocalizerPtr loadNavMeshLocalizer( const std::string & fileName, bool usePlanner ) throw ( ResourceException ) {
		Resource * rsrc = ResourceManager::getResource( fileName, &NavMeshLocalizer::load, NavMeshLocalizer::LABEL );
		if ( rsrc == 0x0 ) {
			logger << Logger::ERR_MSG << "No resource available.";
			throw ResourceException();
		}
		NavMeshLocalizer * nml = dynamic_cast< NavMeshLocalizer * >( rsrc );
		if ( nml == 0x0 ) {
			logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a navigation mesh localizer.";
			throw ResourceException();
		}

		if ( usePlanner ) {
			if ( nml->getPlanner() == 0x0 ) {
				PathPlanner * planner = new PathPlanner( nml->getNavMesh() );
				nml->setPlanner( planner );
			}
		}
		
		return NavMeshLocalizerPtr( nml );
	}
}	// namespace Menge