/* -----------------------------------------------------------------------------
splinelib - Spline library 
Copyright (C) 2010  Sean Curtis

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
------------------------------------------------------------------------------*/

#ifndef __CATMULLROM_H__
#define __CATMULLROM_H__

#include "Hermite.h"
#include <algorithm>

namespace Spline {

	// Class for an arbitrary-dimensioned hermite curve mapping from real -> R^T
	template <class Vector>
	class CatmullRomCurve : public HermiteCurve< Vector > {
	public:
		CatmullRomCurve():HermiteCurve<Vector>() {}

		// add a point to the curve
		void addPoint( float t, const Vector & value );
		void addPoint( float t, const Vector & value, const Vector & tangent );

		int insertPoint( float t, const Vector & value );
		int insertPoint( float t, const Vector & value, const Vector & tangent );

		// Creates the tangents for the curve
		void createTangents( bool periodic=false );

	};

	typedef CatmullRomCurve< float > CatmullRom1D;

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void CatmullRomCurve<Vector>::addPoint(  float t, const Vector & value ) {
		// NOTE: THIS IS AMAZING!
		//		When instantiating with CatmullRomCurve<float>, Vector()
		//		creates a float initialzied to zero.
		HermiteCurve<Vector>::addPoint( t, value, Vector() );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void CatmullRomCurve<Vector>::addPoint(  float t, const Vector & value, const Vector & tangent ) {
		HermiteCurve<Vector>::addPoint( t, value, tangent );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	int CatmullRomCurve<Vector>::insertPoint(  float t, const Vector & value ) {
		// NOTE: THIS IS AMAZING!
		//		When instantiating with CatmullRomCurve<float>, Vector()
		//		creates a float initialzied to zero.
		return HermiteCurve<Vector>::insertPoint( t, value, Vector() );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	int CatmullRomCurve<Vector>::insertPoint(  float t, const Vector & value, const Vector & tangent ) {
		return HermiteCurve<Vector>::insertPoint( t, value, tangent );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void CatmullRomCurve<Vector>::createTangents( bool periodic ) {
		assert( this->_cvCount > 1 && "Can't create tangents for catmullrom curve with less than 2 keys" );

		if ( periodic ) {
			assert( this->_cvCount >= 3 && "Can't perform periodic without at least three keys" );
			int last = this->_cvCount - 1;

			// initial tangent
			this->_cvs[0]._tangent = (this->_cvs[1]._value - this->_cvs[last - 1]._value ) / ( this->_cvs[1]._t - this->_cvs[0]._t  + this->_cvs[last]._t - this->_cvs[last-1]._t );

			// internal tangets
			for ( int i = 1; i < last; ++i ) {
				float a = this->_cvs[i-1]._t;
				float b = this->_cvs[i+1]._t;
				this->_cvs[i]._tangent = (this->_cvs[i+1]._value - this->_cvs[i-1]._value ) / ( b - a );
			}

			// final tangent		
			this->_cvs[last]._tangent = this->_cvs[0]._tangent;
		} else {
			int last = this->_cvCount - 1;

			// initial tangent
			this->_cvs[0]._tangent = (this->_cvs[1]._value - this->_cvs[0]._value ) / ( this->_cvs[1]._t - this->_cvs[0]._t );

			// internal tangets
			for ( int i = 1; i < last; ++i ) {
				float a = this->_cvs[i-1]._t;
				float b = this->_cvs[i+1]._t;
				this->_cvs[i]._tangent = (this->_cvs[i+1]._value - this->_cvs[i-1]._value ) / ( b - a );
			}

			// final tangent		
			this->_cvs[last]._tangent = (this->_cvs[last]._value - this->_cvs[last-1]._value ) / ( this->_cvs[last]._t - this->_cvs[last-1]._t );
		}
	}
}

#endif	// __CATMULLROM_H__
