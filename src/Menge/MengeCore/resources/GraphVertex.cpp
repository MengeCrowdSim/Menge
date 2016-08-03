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

#include "GraphVertex.h"
#include "GraphEdge.h"
#include <cassert>

namespace Menge {

	//////////////////////////////////////////////////////////////////////////////////////
	//						Implementation of GraphVertex
	//////////////////////////////////////////////////////////////////////////////////////

	GraphVertex::GraphVertex(): _edges(0x0), _edgeCount(0), _pos(), _id(-1) {
	}

	//////////////////////////////////////////////////////////////////////////////////////

	GraphVertex::~GraphVertex() {
		if ( _edges ) {
			delete [] _edges;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	GraphVertex & GraphVertex::operator=( const GraphVertex & v ) {
		// Copy edges
		// If I already have memory and it is too small, delete and create
		if ( _edgeCount < v._edgeCount ) {
			if ( _edgeCount > 0 ) delete [] _edges;
			_edges = new GraphEdge[ v._edgeCount ];
		}

		_edgeCount = v._edgeCount;
		for ( unsigned int e = 0; e < _edgeCount; ++e ) {
			_edges[e] = v._edges[e];
		}

		_pos = v._pos;
		_id = v._id;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	float GraphVertex::getDistance( size_t i ) const {
		assert( i < _edgeCount && "Trying to get distance to a neighbor with an invalid index." );
		return _edges[ i ].getDistance();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	bool GraphVertex::setEdge( const GraphEdge & edge, size_t i ) {
		if (i < _edgeCount) {
			_edges[i] = edge;
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	const GraphVertex * GraphVertex::getNeighbor( size_t i ) const {
		assert( i < _edgeCount && "Trying to access a neighbor with an invalid index" );
		return _edges[ i ].getNeighbor();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void GraphVertex::setDegree( size_t degree ) {
		if ( _edgeCount ) {
			delete [] _edges;
		}
		_edgeCount = degree;
		if ( _edgeCount ) _edges = new GraphEdge[ _edgeCount ];
	}
}	// namespace Menge