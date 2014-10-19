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

#include "NavMeshPoly.h"
#include "Logger.h"

namespace Menge {

	//////////////////////////////////////////////////////////////////////////////////////
	//						Implementation of NavMeshPoly
	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshPoly::NavMeshPoly(): _vertIDs(0x0), _vertCount(0), _A(0.f), _B(0.f), _C(0.f) {
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshPoly::~NavMeshPoly() {
		if ( _vertIDs ) {
			delete [] _vertIDs;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	NavMeshPoly & NavMeshPoly::operator=( const NavMeshPoly & p ) {
		_vertCount = p._vertCount;
		if ( _vertIDs ) delete [] _vertIDs;
		_vertIDs = new unsigned int[ _vertCount ];
		memcpy( &_vertIDs[0], &p._vertIDs[0], _vertCount * sizeof( unsigned int ) );
		_vertices = p._vertices;
		_A = p._A;
		_B = p._B;
		_C = p._C;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	bool NavMeshPoly::containsPoint( const Vector2 & point ) const {
		// classic polygon test by counting intersections between polygon and a line segment
		//	connecting the point and some infinitely distant point (specifically, a point
		//	with the same y-value as point, but x = -infinity.
		//	An even number means the point is outside, odd means inside.
		//	Have to handle the special case where the segment intersects a vertex carefully
		//		(otherwise it accidentally gets counted twice.)

		const float X = point.x();
		const float Y = point.y();

		// Test the bounding area
		// TODO: Why is this commented out?
		//if ( X < _minX ||
		//	 X > _maxX ||
		//	 Y < _minY ||
		//	 Y > _maxY ) return false;
		int count = 0;	// number of intersections
		for ( size_t e = 0; e < _vertCount; ++e ) {
			const Vector2 & p0 = _vertices[ _vertIDs[e] ];
			
			if ( p0.y() == Y && p0.x() <= X ) {
				// There is a special case here where the line passes through the point 
				//	tangentially to the polygon (i.e. it doesn't cut into the polygon.
				//
				//	   a\    /b
				//       \  /
				//       c\/______x
				//
				// The line segment through the test point x intersects point c, but the
				//	line segment doesn't cut through the polygon.  Counting this would be
				//	incorrect.
				//	
				// The solution is to test points a and b and make sure they lie on opposite
				//	sides of the line through x.  If they do, it counts
				//
				// However if the test point IS the vertex, then it DOES count.
				if ( p0.x() == X ) {
					// the point is in the polygon
					//	this is slightly fragile -- the point will register as inside any 
					//	polygon built on this point.
					//
					// A similar problem exists if x lies on the boundary of the polygon -- both
					//	 polygons will consider it to be "inside".
					return true;
				} else {
					size_t prev = e == 0 ? (_vertCount - 1 ) : ( e - 1 );
					size_t next = e == ( _vertCount - 1 ) ? 0 : ( e + 1 ); 
					float pY = _vertices[ _vertIDs[ prev ] ].y();
					float nY = _vertices[ _vertIDs[ next ] ].y();
					// if both y-values lie on the same side of the line, it is incidental contact.
					//	Don't count the contact
					//	There can be problems with signed zero values.  Otherwise, probably safe.
					if ( ( pY > Y && nY > Y ) || ( pY < Y && nY < Y ) ) continue;
				}
				// If the line segment intersects the first point, count it and move on.
				++count;
				continue;
			}

			const size_t next = ( e + 1 ) % _vertCount;
			const Vector2 & p1 = _vertices[ _vertIDs[ next ] ];
			// simple cases in which there can be no intersection
			if ( ( p0.y() > Y && p1.y() >= Y ) ||		// polysegment above line
				 ( p0.y() < Y && p1.y() <= Y ) ||		// polysegment below line
				 ( p0.x() > X && p1.x() > X ) ) {		// polysegment to right of test line

				 continue;
			}
			// legitimate intersection test
			// compute where, between y0 and y1, I'll find Y
			float t = Y - p0.y();
			float x0 = p0.x();
			float dx = p1.x() - x0;
			
			t /= p1.y() - p0.y();
			float x = x0 + t * dx;
			if ( x <= X ) {	// this includes if (X,Y) lies on the line between the two vertices
				++count;
			}
		}
		return (count & 0x1) == 1;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	float NavMeshPoly::getElevation( const Vector2 & point ) const {
		return _A * point.x() + _B * point.y() + _C;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	bool NavMeshPoly::loadFromAscii( std::ifstream & f ) {
		if ( !( f >> _vertCount ) )  {
			logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- unable to determine number of vertices in polygon!" ;
			return false;
		}
		if ( _vertIDs ) delete [] _vertIDs;
		_vertIDs = new unsigned int[ _vertCount ];
		for ( size_t i = 0; i < _vertCount; ++i ) {
			if ( ! ( f >> _vertIDs[ i ] ) ) {
				logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- number of vertices does not match declared number!" ;
				return false;
			}
		}
		if ( ! ( f >> _A >> _B >> _C ) ) {
			logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- malformed plane definition!" ;
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	bool NavMeshPoly::loadFromBinary( std::ifstream & f ) {
		// TODO: This can lead to problems.  If the size of size_t changes,
		//	but the file spec doesn't, this will read the wrong amount of data.
		unsigned int data;
		f.read( (char*)&data, sizeof( int ) );
		_vertCount = static_cast< size_t >( data );
		if ( f.fail() ) {
			logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- unable to determine number of vertices in polygon!" ;
			return false;
		}
		if ( _vertIDs ) delete [] _vertIDs;
		_vertIDs = new unsigned int[ _vertCount ];
		f.read( (char*)&_vertIDs[0], static_cast< std::streamsize >( _vertCount ) * sizeof( unsigned int ) );
		if ( f.fail() ) {
			logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- number of vertices does not match declared number!" ;
			return false;
		}
		const int FLOAT_COUNT = 3;
		float fData[ FLOAT_COUNT ];
		f.read( (char*)&fData[0], FLOAT_COUNT * sizeof( float ) );
		if ( f.fail() ) {
			logger << Logger::ERR_MSG << "Malformed navigation mesh polygon -- malformed plane definition!" ;
			return false;
		}
		_A = fData[0];
		_B = fData[1];
		_C = fData[2];
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMeshPoly::initialize( size_t vCount, unsigned int * ids, float A, float B, float C ) {
		_vertCount = vCount;
		if ( _vertIDs ) {
			delete [] _vertIDs;
		}
		_vertIDs = new unsigned int[ vCount ];
		memcpy( _vertIDs, ids, sizeof( unsigned int ) * vCount );
		_A = A;
		_B = B;
		_C = C;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void NavMeshPoly::setBB( const Vector2 *vertices ) {
		_minX = _minY = 1e6f;
		_maxX = _maxY = -_minX;
		for ( size_t v = 0; v < _vertCount; ++v ) {
			const Vector2 & p0 = vertices[ _vertIDs[v] ];
			if ( p0.x() < _minX ) _minX = p0.x();
			if ( p0.x() > _maxX ) _maxX = p0.x();
			if ( p0.y() < _minY ) _minY = p0.y();
			if ( p0.y() > _maxY ) _maxY = p0.y();
		}
	}
}	// namespace Menge
