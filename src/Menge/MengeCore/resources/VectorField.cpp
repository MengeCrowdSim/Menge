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

#include "VectorField.h"
#include "ResourceManager.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of VectorField
	/////////////////////////////////////////////////////////////////////

	const std::string VectorField::LABEL("vecfield");

	/////////////////////////////////////////////////////////////////////

	VectorField::VectorField( const std::string & fileName ):Resource(fileName) {
		_resolution[0] = _resolution[1] = 0;
		_cellSize = 0.f;
		_data = 0x0;
	}

	/////////////////////////////////////////////////////////////////////

	VectorField::~VectorField() {
		freeDataArray();
	}

	/////////////////////////////////////////////////////////////////////

	void VectorField::setDimensions( float width, float height ) {
		float ratio = width / _cellSize;
		int colCount = (int)ratio;
		if ( ratio - colCount > 0.f ) ++colCount;
		ratio = height / _cellSize;
		int rowCount = (int)ratio;
		if ( ratio - rowCount > 0.f ) ++rowCount;
		_resolution[0] = rowCount;
		_resolution[1] = colCount;
	}

	/////////////////////////////////////////////////////////////////////

	void VectorField::initDataArray() {
		freeDataArray();
		const int ROW_COUNT = _resolution[0];
		const int COL_COUNT = _resolution[1];
		_data = new Vector2*[ ROW_COUNT ];
		for ( int r = 0; r < ROW_COUNT; ++r ) {
			_data[r] = new Vector2[ COL_COUNT ];
		}
	}

	/////////////////////////////////////////////////////////////////////

	void VectorField::freeDataArray() {
		if ( _data ) {
			for ( int r = 0; r < _resolution[0]; ++r ) {
				delete [] _data[r];
			}
			delete _data;
			_data = 0x0;
		}
	}

	/////////////////////////////////////////////////////////////////////

	void VectorField::getCell( const Vector2 & pos, int & r, int & c ) {
		assert( _data != 0x0 && "Requesting a field value without having field data" );
		Vector2 offset = pos - _minPoint;
		offset /= _cellSize;
		c = (int)offset.x();
		if ( c < 0 ) c = 0;
		else if ( c >= _resolution[1] ) c = _resolution[1] - 1;

		r = (int)offset.y();
		if ( r < 0 ) r = 0;
		else if ( r >= _resolution[0] ) r = _resolution[0] - 1;
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 VectorField::getFieldValue( int row, int col ) const {
		assert( row >= 0 && row < _resolution[0] && "Invalid row index" );
		assert( col >= 0 && col < _resolution[1] && "Invalid column index" );
		return _data[ row ][ col ];
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 VectorField::getFieldValue( const Vector2 & pos ) {
		assert( _data != 0x0 && "Requesting a field value without having field data" );
		int row, col;
		getCell( pos, row, col );

		return _data[ row ][ col ];
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 VectorField::getFieldValueInterp( const Vector2 & pos ) {
		// NOTE: this doesn't use anything smart when it comes to performing
		//	the bilinear interpolation.  The x and y components of the vector
		//	are interpolated independently
		assert( _data != 0x0 && "Requesting a field value without having field data" );
		int rowIndex, colIndex;
		getCell( pos, rowIndex, colIndex );

		// Compute center of target cell
		Vector2 center = Vector2( colIndex + 0.5f, rowIndex + 0.5f ) * _cellSize + _minPoint;
		Vector2 weight = ( pos - center ) / _cellSize;
		const float WX = weight.x();
		const float WY = weight.y();

		Vector2 value = _data[ rowIndex ][ colIndex ];

		// if I'm in the center its simple
		const int ROW_COUNT = _resolution[0];
		const int COL_COUNT = _resolution[1];
		if ( WX >= 0.f && colIndex < COL_COUNT - 1 ) {
			// interpolate with cells to the right
			value = _data[rowIndex][colIndex] * ( 1 - WX ) + _data[ rowIndex ][colIndex + 1] * WX;
			if ( WY >= 0.f && rowIndex < ROW_COUNT - 1 ) {
				Vector2 val2 = _data[rowIndex+1][colIndex] * ( 1 - WX ) + _data[ rowIndex+1][colIndex + 1] * WX;
				value *= (1 - WY );
				value += val2 * WY;
			} else if ( WY < 0 && rowIndex > 0 ) {
				Vector2 val2 = _data[rowIndex-1][colIndex] * ( 1 - WX ) + _data[ rowIndex-1][colIndex + 1] * WX;
				value *= (1 + WY );
				value -= val2 * WY;
			}
		} else if ( WX < 0.f && colIndex > 0 ) {
			// interpolate with cells to the right
			value = _data[rowIndex][colIndex] * ( 1 + WX ) - _data[ rowIndex ][colIndex - 1] * WX;
			if ( WY >= 0.f && rowIndex < ROW_COUNT - 1 ) {
				Vector2 val2 = _data[rowIndex+1][colIndex] * ( 1 + WX ) - _data[ rowIndex+1][colIndex - 1] * WX;
				value *= (1 - WY );
				value += val2 * WY;
			} else if ( WY < 0 && rowIndex > 0 ) {
				Vector2 val2 = _data[rowIndex-1][colIndex] * ( 1 + WX ) - _data[ rowIndex-1][colIndex - 1] * WX;
				value *= (1 + WY );
				value -= val2 * WY;
			}
		} else {
			if ( WY >= 0.f && rowIndex < ROW_COUNT - 1 ) {
				value = _data[rowIndex][colIndex] * ( 1 - WY ) + _data[ rowIndex+1 ][colIndex] * WY;
			} else if ( WY < 0 && rowIndex > 0 ) {
				value = _data[rowIndex][colIndex] * ( 1 + WY ) - _data[ rowIndex-1 ][colIndex] * WY;
			}
		}

		return value;
	}

	/////////////////////////////////////////////////////////////////////

	Resource * VectorField::load( const std::string & fileName ) {
		std::ifstream f;
		f.open( fileName.c_str(), std::ios::in );
		if ( ! f.is_open() ) {
			logger << Logger::ERR_MSG << "Error opening the VectorField file definition: " << fileName << "\n";
			return 0x0;
		}

		VectorField * field = new VectorField( fileName );
		f >> field->_resolution[0] >>field-> _resolution[1];
		f >> field->_cellSize;
		float x, y;
		f >> x >> y;
		field->_minPoint = Vector2( x, y );
		field->initDataArray();
		for ( int r = 0; r < field->_resolution[0]; ++r ) {
			for ( int c = 0; c < field->_resolution[1]; ++c ) {
				if ( f >> x >> y ) {
					field->_data[r][c] = Vector2( x, y );
				} else {
					logger << Logger::ERR_MSG << "Format error in the VectorField file definition: " << fileName << "\n";
					logger << "\tTried to read a vector at position: (" << r << ", " << c << "), but no data existed\n";
					field->destroy();
					f.close();
					return 0x0;
				}
			}
		}
		f.close();

		return field;
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 VectorField::getMaximumPoint() const {
		return getSize() + _minPoint;
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 VectorField::getSize() const {
		float width = _resolution[1] * _cellSize;
		float height = _resolution[0] * _cellSize;
		return Vector2( width, height );
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<< ( Logger & out, const VectorField & vf ) {
		out << "Vector Field:\n";
		out << "\tMinimum point:  " << vf._minPoint << "\n";
		out << "\tCell size:      " << vf._cellSize << "\n";
		out << "\t(width,height): " << vf.getSize() << "\n";
		return out;
	}

	/////////////////////////////////////////////////////////////////////

	VectorFieldPtr loadVectorField( const std::string & fileName ) throw ( ResourceException ) {
		Resource * rsrc = ResourceManager::getResource( fileName, &VectorField::load, VectorField::LABEL );
		if ( rsrc == 0x0 ) {
			logger << Logger::ERR_MSG << "No resource available\n";
			throw ResourceException();
		}
		VectorField * vf = dynamic_cast< VectorField * >( rsrc );
		if ( vf == 0x0 ) {
			logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a VectorField\n";
			throw ResourceException();
		}
		return VectorFieldPtr( vf );
	}
}	// namespace Menge