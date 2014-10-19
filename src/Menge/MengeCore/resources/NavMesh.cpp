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

#include "NavMesh.h"
#include "NavMeshNode.h"
#include "NavMeshEdge.h"
#include "NavMeshObstacle.h"
#include <cassert>
#include "Logger.h"

#include "BaseAgent.h"
#include "SimulatorInterface.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of NavMesh
	/////////////////////////////////////////////////////////////////////

	const std::string NavMesh::LABEL("navmesh");

	/////////////////////////////////////////////////////////////////////

	NavMesh::NavMesh( const std::string & name ):Resource(name), _vCount(0), _vertices(0x0), _nCount(0), _nodes(0x0), _eCount(0), _edges(0x0), _obstCount(0), _obstacles(0x0), _nodeGroups() {
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMesh::~NavMesh() {
		clear();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::clear() {
		if ( _vCount ) {
			_vCount = 0;
			delete [] _vertices;
			_vertices = 0x0;
		}

		if ( _nCount ) {
			_nCount = 0;
			delete [] _nodes;
			_nodes = 0x0;
		}

		if ( _eCount ) {
			_eCount = 0;
			delete [] _edges;
			_edges = 0x0;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshNode & NavMesh::getNode( unsigned int i ) {
		assert( i < _nCount && "Tried to access invalid node index" );
		return _nodes[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	const NavMeshNode & NavMesh::getNode( unsigned int i ) const {
		assert( i < _nCount && "Tried to access invalid node index" );
		return _nodes[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshEdge & NavMesh::getEdge( unsigned int i ) {
		assert( i < _eCount && "Tried to access invalid edge index" );
		return _edges[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	const NavMeshEdge & NavMesh::getEdge( unsigned int i ) const {
		assert( i < _eCount && "Tried to access invalid edge index" );
		return _edges[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshObstacle & NavMesh::getObstacle( unsigned int i ) {
		assert( i < _obstCount && "Tried to access invalid obstacle index" );
		return _obstacles[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	const NavMeshObstacle & NavMesh::getObstacle( unsigned int i ) const {
		assert( i < _obstCount && "Tried to access invalid obstacle index" );
		return _obstacles[ i ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	float NavMesh::getElevation( unsigned int nodeID, const Vector2 & p ) const {
		const NavMeshNode & node = _nodes[ nodeID ];
		return node.getElevation( p );
	}

	//////////////////////////////////////////////////////////////////////////////////////

	Vector2 NavMesh::getGradient( unsigned int nodeID, const Vector2 & p ) const {
		const NavMeshNode & node = _nodes[ nodeID ];
		return node.getGradient();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::setVertexCount( size_t count ) {
		if ( _vCount ) {
			delete [] _vertices;
		}
		_vCount = count;
		_vertices = new Vector2[ _vCount ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::setVertex( unsigned int i, float x, float y ) {
		assert( i < _vCount && "Accessing a vertex outside of the range of valid values." );
		_vertices[i].set( x, y );
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::setNodeCount( size_t count ) {
		if ( _nCount ) {
			delete [] _nodes;
		}
		_nCount = count;
		_nodes = new NavMeshNode[ _nCount ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::setEdgeCount( size_t count ) {
		if ( _eCount ) {
			delete [] _edges;
		}
		_eCount = count;
		_edges = new NavMeshEdge[ _eCount ];
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMesh::setObstacleCount( size_t count ) {
		if ( _obstCount ) {
			delete [] _obstacles;
		}
		_obstCount = count;
		_obstacles = new NavMeshObstacle[ _obstCount ];
	}

	//////////////////////////////////////////////////////////////////////////////////////
	 
	bool NavMesh::addGroup( const std::string & grpName, size_t grpSize ) {
		if ( _nodeGroups.find( grpName ) != _nodeGroups.end() ) {
			logger << Logger::ERR_MSG << "NavMesh has two groups with the same name: " << grpName << "!\n";
			return false;
		}
		size_t first = _nCount;
		size_t last = first + grpSize - 1;
		_nodeGroups[ grpName ] = NMNodeGroup( static_cast< unsigned int>( first ), static_cast< unsigned int >( last ) );

		// Now extend the node memory
		NavMeshNode * tmpNodes = new NavMeshNode[ _nCount + grpSize ];
		if ( _nCount ) {
			for ( size_t i = 0; i < _nCount; ++i ) {
				tmpNodes[ i ] = _nodes[ i ];
			}
			delete [] _nodes;
		}
		_nCount += grpSize;
		_nodes = tmpNodes;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	#ifdef _WIN32
	// This disables a 64-bit compatibility warning - pushing a 64-bit value into a 32-bit value.  
	//	In this case, I know the value in the pointers that are being re-interpreted as 
	//  unsigned ints are REALLY just unsigned ints, so it is safe.
	#pragma warning( disable : 4311 )
	#endif
	bool NavMesh::finalize() {
		// All of the edge indices in the nodes need to be replaced with pointers
		// All of the obstacle indices in the nodes need to be replaced with pointers.
		for ( size_t n = 0; n < _nCount; ++n ) {
			NavMeshNode & node = _nodes[ n ];
			for ( size_t e = 0; e < node._edgeCount; ++e ) {
				// TODO: This might not work in building 64-bit code.  
				//		The pointer will be larger than the unsigned int.  But as I'm pushing an unsigned
				//		int into a pointer slot, it'll probably be safe.  Needs to be tested.

				unsigned int eID = reinterpret_cast< unsigned int >( node._edges[ e ] );
				assert( eID >= 0 && eID < _eCount && "Finalizing invalid edge id from node" );
				node._edges[ e ] = &_edges[ eID ];
			}
			for ( size_t o = 0; o < node._obstCount; ++o ) {
				unsigned int oID = reinterpret_cast< unsigned int >( node._obstacles[ o ] );
				assert( oID >= 0 && oID < _obstCount && "Finalizing invalid obstacle id for node" );
				node._obstacles[ o ] = &_obstacles[ oID ];
			}
			node._id = static_cast< unsigned int >( n );
			node._poly.setBB( _vertices );
		}

		// All of the node indices in the edges need to be replaced with pointers
		for ( size_t e = 0; e < _eCount; ++e ) {
			NavMeshEdge & edge = _edges[ e ];
			unsigned int nID = reinterpret_cast< unsigned int >( edge._node0 );
			assert( nID >= 0 && nID < _nCount && "Finalizing invalid node id from edge" );
			edge._node0 = &_nodes[ nID ];
			
			nID = reinterpret_cast< unsigned int >( edge._node1 );
			assert( nID >= 0 && nID < _nCount && "Finalizing invalid node id from edge" );
			edge._node1 = &_nodes[ nID ];
			// compute edge distance
			edge._distance = abs( edge._node0->getCenter() - edge._node1->getCenter() );

			// Confirm that point is on the left when looking from node0
			if ( det( edge._dir, edge._node0->_center - edge._point ) > 0.f ) {
				NavMeshNode * tmp = edge._node0;
				edge._node0 = edge._node1;
				edge._node1 = tmp;
			}
		}

		std::vector< bool > processed( _obstCount, false );
		for ( unsigned int o = 0; o < (unsigned int)_obstCount; ++o ) {
			_obstacles[ o ]._id = o;
			if ( processed[ o ] ) continue;
			const unsigned int START = o;
			unsigned int curr = o;
			while ( curr != NavMeshObstacle::NO_NEIGHBOR_OBST && !processed[ curr ] ) {
				processed[ curr ] = true;
				NavMeshObstacle & obst = _obstacles[ curr ];
				unsigned int nID = reinterpret_cast< unsigned int>( obst._node );
				assert( nID < _nCount && "Finalizing invalid node id from obstacle" );
				obst._node = &_nodes[ nID ];

				nID = reinterpret_cast< unsigned int >( obst._nextObstacle );
				assert( ( nID < _obstCount || nID == NavMeshObstacle::NO_NEIGHBOR_OBST ) && "Finalizing invalid obstacle index for next obstacle" );
				if ( nID == NavMeshObstacle::NO_NEIGHBOR_OBST ) {
					obst._nextObstacle = 0x0;
				} else {
					obst._nextObstacle = &_obstacles[ nID ];
					//	Wire up "_prevObstacle" with the previous obstacle
					_obstacles[ nID ]._prevObstacle = &_obstacles[ curr ];
				}
				curr = nID;
			}
			// set open/closed
			if ( curr == NavMeshObstacle::NO_NEIGHBOR_OBST ||
				curr != START ) {	// set open
				Agents::Obstacle * obst = &_obstacles[ START ];
				obst->setClosedState( false );
				while ( obst->_nextObstacle != 0x0 ) {
					obst = obst->_nextObstacle;
					obst->setClosedState( false );
				}
			}
		}

		return true;
	}
	#ifdef _WIN32
	#pragma warning( default : 4311 )
	#endif

	//////////////////////////////////////////////////////////////////////////////////////
	 
	//void NavMesh::addObstacles( Agents::SimulatorInterface * simulator ) {
	//	// Construct each contiguous obstacle

	//	std::vector< bool > processed( _obstCount, false );
	//	std::vector< Vector2 > vertices;
	//	for ( size_t o = 0; o < _obstCount; ++o ) {
	//		if ( processed[ o ] ) continue;
	//		Agents::Obstacle * start = &_obstacles[ o ];
	//		Agents::Obstacle * curr = start;
	//		
	//		bool closed = true;
	//		while ( curr != 0x0 && !processed[ curr->_id ] ) {
	//			processed[ curr->_id ] = true;
	//			curr = curr->_nextObstacle;
	//		}
	//		
	//		vertices.clear();
	//		// set open/closed
	//		if ( curr == 0x0 ||
	//			curr != start ) {	// set open
	//			closed = false;
	//			curr = start;
	//			while ( curr->_prevObstacle ) {
	//				processed[ curr->_id ] = true;
	//				curr = curr->_prevObstacle;
	//			}
	//			vertices.push_back( curr->getP0() );
	//			while ( curr->_nextObstacle ) {
	//				vertices.push_back( curr->getP0() );
	//			}
	//			vertices.push_back( curr->getP1() );
	//		} else {
	//			vertices.push_back( curr->getP0() );
	//			curr = curr->_nextObstacle;
	//			while ( curr != start ) {
	//				vertices.push_back( curr->getP0() );
	//				curr = curr->_nextObstacle;
	//			}
	//		}
	//		// This will cause the obstacle to be considered visible to ALL agents
	//		//	unless their obstacle set is set to 0.
	//		const unsigned int ALL_OBST_CLASSES = 0xffffffff;
	//		// Assume it's a loop
	//		
	//		//TODO: FIX THIS
	//		//simulator->getSpatialQuery()->addObstacle( vertices, closed, ALL_OBST_CLASSES );
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////////////////
	 
	std::vector<Agents::ObstacleVertexList> NavMesh::getObstacles() {
		
		//build a vector of obstaclevertexlists
		std::vector<Agents::ObstacleVertexList> obsList;

		// Construct each contiguous obstacle
		std::vector< bool > processed( _obstCount, false );
		for ( size_t o = 0; o < _obstCount; ++o ) {
			if ( processed[ o ] ) continue;
			Agents::ObstacleVertexList obs; //temp object for the list


			Agents::Obstacle * start = &_obstacles[ o ];
			Agents::Obstacle * curr = start;
			
			while ( curr != 0x0 && !processed[ curr->_id ] ) {
				processed[ curr->_id ] = true;
				curr = curr->_nextObstacle;
			}
			
			
			// set open/closed
			if ( curr == 0x0 ||
				curr != start ) {	// set open
				obs.closed = false;
				curr = start;
				while ( curr->_prevObstacle ) {
					processed[ curr->_id ] = true;
					curr = curr->_prevObstacle;
				}
				obs.vertices.push_back( curr->getP0() );
				while ( curr->_nextObstacle ) {
					obs.vertices.push_back( curr->getP0() );
				}
				obs.vertices.push_back( curr->getP1() );
			} else {
				obs.vertices.push_back( curr->getP0() );
				curr = curr->_nextObstacle;
				while ( curr != start ) {
					obs.vertices.push_back( curr->getP0() );
					curr = curr->_nextObstacle;
				}
			}

			//add it to the list
			obsList.push_back(obs);
		}

		return obsList;
	}

	/////////////////////////////////////////////////////////////////////

	Resource * NavMesh::load( const std::string & fileName ) {
		// TODO: Change this to support comments.
		std::ifstream f;
		f.open( fileName.c_str(), std::ios::in );

		if ( !f.is_open() ) {
			logger << Logger::ERR_MSG << "Error opening navigation mesh file: " << fileName << ".";
			return 0x0;
		}

		// load vertices
		unsigned int vertCount;
		if ( ! ( f >> vertCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing nav mesh: file didn't start with an int (vertex count).";
			return 0x0; 
		}

		NavMesh * mesh = new NavMesh( fileName );
		mesh->setVertexCount( vertCount );
		float x, y;
		for ( unsigned int v = 0; v < vertCount; ++v ) {
			if ( ! ( f >> x >> y ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: format error for vertex " << ( v + 1 ) << ".";
				mesh->destroy();
				return 0x0;
			}
			mesh->setVertex( v, x, y );
		}

		// load edges
		unsigned int edgeCount;
		if ( ! ( f >> edgeCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing nav mesh: didn't find edge count where expected.";
			mesh->destroy();
			return 0x0; 
		}
		mesh->setEdgeCount( edgeCount );
		for ( unsigned int e = 0; e < edgeCount; ++e ) {
			NavMeshEdge & edge = mesh->getEdge( e );
			if ( ! edge.loadFromAscii( f, mesh->_vertices ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: format error for edge " << ( e + 1 ) << ".";
				mesh->destroy();
				return 0x0;
			}
		}

		// load obstacles
		unsigned int obstCount;
		if ( ! ( f >> obstCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing nav mesh: didn't find obstacle count where expected.";
			mesh->destroy();
			return 0x0; 
		}
		mesh->setObstacleCount( obstCount );
		for ( unsigned int o = 0; o < obstCount; ++o ) {
			NavMeshObstacle & obst = mesh->getObstacle( o );
			if ( ! obst.loadFromAscii( f, mesh->_vertices ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: format error for obstacle " << ( o + 1 ) << ".";
				mesh->destroy();
				return 0x0;
			}
		}

		unsigned int totalN = 0;
		unsigned int n = 0;
		// load node group
		while ( ! f.eof() ) {
			std::string grpName;
			if ( ! ( f >> grpName ) ) {
				if ( f.eof() ) {
					break;
				} else {
					logger << Logger::ERR_MSG << "Error in parsing nav mesh: Missing node group name.";
					mesh->destroy();
					return 0x0;
				}
			} 
			// load nodes
			unsigned int nCount;
			if ( ! ( f >> nCount ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: Node group " << grpName << " doesn't specify node count.";
				mesh->destroy();
				return 0x0; 
			}

			totalN += nCount;
			mesh->addGroup( grpName, nCount );
			assert( totalN == mesh->getNodeCount() && "Data management problem -- tracked node count does not match in-memory node count" );

			for ( ; n < totalN; ++n ) {
				NavMeshNode & node = mesh->getNode( n );
				if ( ! node.loadFromAscii( f ) ) {
					logger << Logger::ERR_MSG << "Error in parsing nav mesh: Poorly formatted definition for node " << ( n + 1 ) << ".";
					mesh->destroy();
					return 0x0;
				}
				
				node.setID( n );
				node.setVertices( mesh->getVertices() );
			}
		}

		if ( !mesh->finalize() ) {
			mesh->destroy();
			return 0x0;
		}
		return mesh;
	}

	/////////////////////////////////////////////////////////////////////

	NavMeshPtr loadNavMesh( const std::string & fileName ) throw ( ResourceException ) {
		Resource * rsrc = ResourceManager::getResource( fileName, &NavMesh::load, NavMesh::LABEL );
		if ( rsrc == 0x0 ) {
			logger << Logger::ERR_MSG << "No resource available.";
			throw ResourceException();
		}
		NavMesh * nm = dynamic_cast< NavMesh * >( rsrc );
		if ( nm == 0x0 ) {
			logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a navigation mesh.";
			throw ResourceException();
		}
		
		return NavMeshPtr( nm );
	}
}	// namespace Menge