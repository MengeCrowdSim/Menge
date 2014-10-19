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

#include "NavMeshNode.h"
#include "NavMeshEdge.h"
#include "NavMeshObstacle.h"
#include "Logger.h"
#include <cassert>
#include <iostream>

namespace Menge {

	//////////////////////////////////////////////////////////////////////////////////////
	//						Implementation of NavMeshNode
	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshNode::NavMeshNode(): _edges(0x0), _edgeCount(0), _obstacles(0x0), _obstCount(0),_center(), _poly() {
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshNode::~NavMeshNode() {
		if ( _edges ) {
			delete [] _edges;
		}
		if ( _obstacles ) {
			delete [] _obstacles;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshNode & NavMeshNode::operator=( const NavMeshNode & n ) {
		// Copy edges
		// If I already have memory and it is too small, delete and create
		if ( _edgeCount < n._edgeCount ) {
			if ( _edgeCount > 0 ) delete [] _edges;
			_edges = new NavMeshEdge*[ n._edgeCount ];
		}

		_edgeCount = n._edgeCount;
		for ( unsigned int e = 0; e < _edgeCount; ++e ) {
			_edges[e] = n._edges[e];
		}

		// Copy obstacles
		if ( _obstCount < n._obstCount ) {
			if ( _obstCount > 0 ) delete [] _edges;
			_obstacles = new NavMeshObstacle*[ n._obstCount ];
		}
		_obstCount = n._obstCount;
		for ( unsigned int o = 0; o < _obstCount; ++o ) {
			_obstacles[o] = n._obstacles[o];
		}

		_center = n._center;
		_poly = n._poly;
		_id = n._id;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	const NavMeshNode * NavMeshNode::getNeighbor( size_t i ) const {
		assert( i < _edgeCount && "Trying to access a neighbor with an invalid index" );
		return _edges[ i ]->getOtherByPtr( this );
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshEdge * NavMeshNode::getConnection( unsigned int nodeID ) {
		for ( size_t e = 0; e < _edgeCount; ++e ) {
			NavMeshEdge * edge = _edges[ e ];
			NavMeshNode * neighbor = edge->getOtherByPtr( this );
			if ( neighbor->_id == nodeID ) {
				return edge;
			}
		}
		return 0x0;
	}


	//////////////////////////////////////////////////////////////////////////////////////

	#ifdef _WIN32
	// This disables a 64-bit compatibility warning - pushing a 32-bit value into a 64-bit value.
	// This can cause problems with SIGN EXTENSION.
	// In this case, I know the value in being put into the pointer slot is an unsigned
	//	int, so sign extension is not a problem.  Plus, they never get interpreted as
	//	pointers.  These indices are eventually mapped to REAL pointers.
	#pragma warning( disable : 4312 )
	#endif
	bool NavMeshNode::loadFromAscii( std::ifstream & f ) {
		// center
		float cx, cy;
		if ( ( f >> cx >> cy ) ) {
			_center.set( cx, cy );
		} else {
			logger << Logger::ERR_MSG << "\tError in parsing nav mesh: Unable to read center of node.\n";
			return false;
		}

		// polygon
		if ( ! _poly.loadFromAscii( f ) ) {
			logger << Logger::ERR_MSG << "\tError in parsing nav mesh: Badly formatted node polygon for node.\n";
			return false;
		}

		// edges
		if ( ! ( f >> _edgeCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing nav mesh: unable to read the edge count for a node.\n";
			return false;
		}
		_edges = new NavMeshEdge*[ _edgeCount ];
		for ( size_t e = 0; e < _edgeCount; ++e ) {
			unsigned int eID;
			if ( ! ( f >> eID ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: unable to read the " << (e + 1) << "th edge for a node.\n";
				return false;
			}
			// This is a cheat -- I'm storing the index now, to convert it to a pointer later.
			_edges[ e ] = ( NavMeshEdge *)eID;
		}

		// obstacles
		if ( ! ( f >> _obstCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing nav mesh: unable to read the adjacent obstacle count.\n";
			return false;
		}
		_obstacles = new NavMeshObstacle*[ _obstCount ];
		for ( size_t o = 0; o < _obstCount; ++o ) {
			unsigned int oID;
			if ( ! ( f >> oID ) ) {
				logger << Logger::ERR_MSG << "Error in parsing nav mesh: unable to read the " << (o + 1) << "th adjacent obstacle.\n";
				return false;
			}
			// This is a cheat -- I'm storing the index now, to convert it to a pointer later.
			_obstacles[ o ] = ( NavMeshObstacle *)oID;
		}
		return true;
	}
	#ifdef _WIN32
	#pragma warning( default : 4312 )
	#endif
}	// namespace Menge