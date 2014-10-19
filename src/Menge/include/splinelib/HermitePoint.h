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

#ifndef __HERMITEPOINT_H__
#define __HERMITEPOINT_H__

#include <iostream>
#include <math.h>

namespace Spline {

	// A class for 1-dimensional hermite curves.  I.e. the function maps real -> real
	template <typename DataVector>
	class HermitePoint {
	public:
		HermitePoint(): _t(0.f), _value(DataVector()), _tangent(DataVector()){}
		HermitePoint( float t, const DataVector & val, const DataVector & tan ): _t(t), _value(val), _tangent(tan) {}

		HermitePoint & operator=( const HermitePoint & pt );

		bool operator==( const HermitePoint & pt );

		// Evaluate the portion of the hermite function which uses this point
		//	as the FIRST point in the curve
		DataVector evaluateAsFirst( float s3, float s2, float s, float scale=1.f ) const;

		// Evaluate the portion of the hermite function which uses this point
		//	as the SECOND point in the curve
		DataVector evaluateAsSecond( float s3, float s2, float s, float scale=1.f ) const;

		// Evaluate the tangent in the portion of the hermite function which uses this point
		//	as the FIRST point in the curve
		DataVector evalTanAsFirst( float s2, float s, float scale=1.f ) const;

		// Evaluate the tangent in the portion of the hermite function which uses this point
		//	as the SECOND point in the curve
		DataVector evalTanAsSecond( float s2, float s, float scale=1.f ) const;

		// Stream the Hermite Point to the stream
		template <typename TDataVector> friend std::ostream & operator << ( std::ostream & out, const HermitePoint<TDataVector> & v );

		// Allow hermite points to be sorted based on parameter
		template <typename TDataVector> friend bool pointCompare( const HermitePoint<TDataVector> & v1, const HermitePoint<TDataVector> & v2 );

		float _t;
		DataVector _value;
		DataVector _tangent;
	};

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	bool HermitePoint<DataVector>::operator==( const HermitePoint<DataVector> & pt ) {
		// this threshold only works for floats
		return  fabs(_t - pt._t) < 1e-5 && fabs(_value - pt._value) < 1e-5 && fabs(_tangent - pt._tangent ) < 1e-5;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	HermitePoint<DataVector> & HermitePoint<DataVector>::operator=( const HermitePoint<DataVector> & pt ) {
		_t = pt._t;
		_value = pt._value;
		_tangent = pt._tangent;
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	DataVector HermitePoint<DataVector>::evaluateAsFirst( float s3, float s2, float s, float scale ) const {
		float h1 = 2 * s3 - 3 * s2 + 1;
		float h3 = s3 - 2 * s2 + s;
		return _value * h1 + _tangent * ( h3 * scale );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	DataVector HermitePoint<DataVector>::evaluateAsSecond( float s3, float s2, float s, float scale ) const {
		float h2 = -2 * s3 + 3 * s2;
		float h4 = s3 -  s2;
		return _value * h2 + _tangent * ( h4 * scale );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	DataVector HermitePoint<DataVector>::evalTanAsFirst( float s2, float s, float scale ) const {
		float h1 = 6 * s2 - 6 * s;
		float h3 = 3 * s2 - 4 * s + 1;
		return _value * h1 + _tangent * ( h3 * scale );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	DataVector HermitePoint<DataVector>::evalTanAsSecond( float s2, float s, float scale ) const {
		float h2 = -6 * s2 + 6 * s;
		float h4 = 3 * s2 - 2 * s;
		return _value * h2 + _tangent * ( h4 * scale );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	std::ostream & operator << ( std::ostream & out, const HermitePoint<DataVector> & v ) {
		std::cout << "t(" << v._t << "), val(" << v._value << "), tan(" << v._tangent << ")";
		return out;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class DataVector>
	bool pointCompare( const HermitePoint<DataVector> & v1, const HermitePoint<DataVector> & v2 ) {
		return v1._t < v2._t;
	}
}

#endif	// __HERMITEPOINT_H__
