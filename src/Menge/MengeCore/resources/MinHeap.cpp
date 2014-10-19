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

#include "MinHeap.h"
#include <iostream>
#ifndef _MSC_VER
#include <string.h>
#endif

namespace Menge {

	//////////////////////////////////////////////////////////////////////////////////////
	//						Implementation of MinHeap
	//////////////////////////////////////////////////////////////////////////////////////

	AStarMinHeap::AStarMinHeap( unsigned int * heap, float * data, bool * state, unsigned int * path, size_t N ) {
		_f = data;
		_g = _f + N;
		_h = _g + N;
		_inHeap = state;
		_visited = _inHeap + N;
		_heap = heap;
		_cameFrom = path;
		initialize( N );
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void AStarMinHeap::initialize( size_t N ) {
		// This code assumes that the f-value is the first block of the data
		//	and _inHeap is the first block of the state
		// TODO: Is it stritly necessary to initialize the values as a block?
		//		Can't they be implictly initialized by the value of _minIndex
		memset( _f, 0x7f, sizeof( float ) * 3 * N );
		memset( _inHeap, 0x0, sizeof( bool ) * 2 * N );
		// note: don't need to initialize the heap array
		_minIdx = _nextFree = 0;
		_minKey = 0.f;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	unsigned int AStarMinHeap::pop() {
		unsigned int returnVal = _heap[ _minIdx ];
		_inHeap[ returnVal ] = false;
		_visited[ returnVal ] = true;
		// Swap into this slot as necessary
		--_nextFree;
		if ( _minIdx != _nextFree ) {
			_heap[ _minIdx ] = _heap[ _nextFree ];
		}
		// Identify the new minimum
		_minIdx = 0;
		_minKey = _f[_heap[0]];
		for ( unsigned int i = 1; i < _nextFree; ++i ) {
			const unsigned int x = _heap[i];
			if ( _f[ x ] < _minKey ) {
				_minIdx = i;
				_minKey = _f[ x ];
			}
		}
		return returnVal;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void AStarMinHeap::push( unsigned int x ) {
		if ( _f[ x ] < _minKey ) {
			_minIdx = _nextFree;
			_minKey = _f[ x ];
		}
		_inHeap[ x ] = true;
		_heap[ _nextFree ] = x;
		++_nextFree;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void AStarMinHeap::changeF( unsigned int val, float key ) {
		// Only has an impact if this introduces a new minimum
		if ( key < _minKey ) {
			_minKey = key;
			for ( unsigned int i = 0; i < _nextFree; ++i ) {
				if ( _heap[ i ] == val ) {
					_minIdx = i;
					break;
				}
			}
		} 
		_f[ val ] = key;
	}

}	// namespace Menge