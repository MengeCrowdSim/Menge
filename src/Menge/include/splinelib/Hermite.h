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

#ifndef __HERMITE_H__
#define __HERMITE_H__

#include "HermitePoint.h"
#include <iostream>
#include <cassert>

// If this is set to zero: the curve won't be printed.
//	Any postive integer will compute the printed hermite at
//	CURVE_SAMPLES number of uniformly distributed values over the
//	domain of the hermite
#define CURVE_SAMPLES 0

namespace Spline {

	// Class for an arbitrary-dimensioned hermite curve mapping from real -> R^T
	template <class Vector>
	class HermiteCurve {
	public:
		HermiteCurve():_cvCount(0), _cvs(0x0) {}
		~HermiteCurve();

		HermiteCurve & operator=( HermiteCurve & crv );

		// Evaluates the curve at parameter value t
		Vector evaluate( float t ) const;

		// Evaluates the tangent of the curve at parameter value t
		Vector evalTangent( float t ) const;

		// Evaluates the curve linearly between points
		Vector evaluateLinear( float t ) const;

		// Solves the inverse, given the interpolated value, determines what value will find it
		//		The paramter value is put in the provided argument param
		//		the function returns boolean if it was able to find such a value, 
		//		if it returns false, the param value is undetermined and meaningless
		// THIS ALGORITHM ASSUMES a 1-to-1 function.  More particularly, a monotonically increasing
		//		function.  It uses bijection.  Otherwise the inverse for the cubic function
		//		is ill-defined.
		bool inverse( const Vector & value, float & param, float threshold = 0.0001f ) const;
		
		// Returns the smallest parameter for which this curve is defined
		float minT() const;
        
		// Returns the largets parameter for which this curve is defined
		float maxT() const;

		// add a point to the curve
		void addPoint( float t, const Vector & value, const Vector & tangent );

		// insert a point into the curve
		//		returns the index of the insertion
		int insertPoint( float t, const Vector & value, const Vector & tangent );

		// clear the vector
		void clear();

		// Number of keys
		inline int keyCount() const { return _cvCount; }

		// return the param of a key
		inline float keyParam( int index ) const { return _cvs[ index]._t; }

		// Set the value of a key
		inline void setKeyParam( int index, float t ) { _cvs[index]._t = t; }

		// Return the value of a key
		inline Vector keyValue( int index ) const { return _cvs[ index ]._value; }

		// Set the value of a key
		inline void setKeyValue( int index, Vector value ) { _cvs[index]._value = value; }

		// offsets the value of a key
		inline void offsetKeyParam( int index, Vector value ) { _cvs[index]._t += value; }

		// offsets the value of a key
		inline void offsetKeyValue( int index, Vector value ) { _cvs[index]._value += value; }

		// offsets the value of a key
		inline void offsetKeyTangent( int index, Vector value ) { _cvs[index]._tangent += value; }

		// scales the tangent by the given value
		inline void scaleKeyTangent( int index, float scale ) { _cvs[index]._tangent *= scale; }

		// set the tangent of a key
		inline void setKeyTangent( int index, Vector tan ) { _cvs[index]._tangent = tan; }

		// Return the tangent of a key
		inline Vector keyTangent( int index ) { return _cvs[ index ]._tangent; }

		// Sets the value and tangent of a key
		inline void setKeyValTan( int index, Vector val, Vector tan ) { _cvs[index]._value = val; _cvs[index]._tangent = tan; }

		// Calc tangent for the given index based on surrounding values
		void calcTangent( int index, bool periodic=false );

		// Calculate a 2nd-order tangent for non-uniform key distribution
		void calcTangent2( int index, bool periodic=false );

		// finding keys
		// searches for the index of the key with the paramter closest to t
		int findClosestIndex( float t ) const;
		// searches for the param value closest to the provided parameter t
		float findClosestParam( float t ) const;

		// functions for removing keys - they report true if a key was removed
		// remove key based on param value
		bool removeParam( float t );
		// remove the ith key
		bool removeKey( int i );

		// Compute new position value
		//		Given the pair (t, val) provides a value that will cause the function to evaluate val at t.
		//		The value is what the p value at the START of the applicable interval should be.
		Vector valueConstraint0( float t, Vector val );
		//		This does the same, except solving for P_1 (instead of P_0)
		Vector valueConstraint1( float t, Vector val );

		// These functions, like valueConstraint*, compute a value with given constraints
		//	In this case, it computes a TANGENT based on four constraints:
		//		the values of each key defining the interval
		//		the tangent of the other end of the interval
		//		and a value on the interior of the interval
		//		the "other end" is defined by which function is called
		//			the 0/1 in the function name indicates that this key is the
		//			start/end of the interval, respectively
		//void setConstrainedTangent0( int key, float constrainT, const Vector & constrainV );
		void setConstrainedTangent1( int key, float constrainT, const Vector & constrainV );

		template <class TVector> friend std::ostream & operator << ( std::ostream & out, const HermiteCurve<TVector> & v );

	//protected:
		// Evaluate the curve for a given value of t, having already determined that
		//	the paramter t is in the interval defined by the ith and i+1st keys
		Vector _evalInterval( int i, float t ) const;
		// Evaluate the curve for a given value of t, having already determined that
		//	the parameter t is in the interval defined by the ith and i+1st keys
		Vector _evalTanInternal( int i, float t ) const;
		// Number of cvs in this curve
		int		_cvCount;
		HermitePoint< Vector >	* _cvs;
	};

	typedef HermiteCurve< float > Hermite1D;

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	HermiteCurve<Vector>::~HermiteCurve() {
		if ( _cvs ) delete [] _cvs;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	HermiteCurve<Vector> & HermiteCurve<Vector>::operator=( HermiteCurve<Vector> & crv ) {
		if ( this != &crv ) {
			if ( _cvCount != crv._cvCount ) {
				_cvCount = crv._cvCount;
				delete [] _cvs;
				_cvs = new HermitePoint< Vector >[ _cvCount ];			
			}
			for ( int i = 0; i < _cvCount; ++i ) {
				_cvs[i] = crv._cvs[i];
			}
		}
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::_evalInterval( int i, float t ) const {
		float a = _cvs[i]._t;
		float b = _cvs[i+1]._t;
		float h = b - a;
		t = ( t - a ) / h;
		float t2 = t * t;
		float t3 = t2 * t;
		return _cvs[i].evaluateAsFirst( t3, t2, t, h ) + _cvs[i+1].evaluateAsSecond( t3, t2, t, h );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::_evalTanInternal( int i, float t ) const {
		float a = _cvs[i]._t;
		float b = _cvs[i+1]._t;
		float h = b - a;
		t = ( t - a ) / h;
		float t2 = t * t;
		return ( _cvs[i].evalTanAsFirst( t2, t, h ) + _cvs[i+1].evalTanAsSecond( t2, t, h ) ) / h;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::evaluate( float t ) const {
		assert( _cvCount > 0 && "Calling evaluate without any points causes an error" );

		int last = _cvCount - 1;
		// Check if the value is beyond the extents of the curve
		//		TODO: This assumes constant value beyond the boundaries
		//			  OPTIONALLY, allow other extrapolation methods (such as perioid, linear, etc.)
		if ( t <= _cvs[0]._t) {
            return _cvs[0]._value;
		} else if ( t >= _cvs[ last ]._t ) {
            return _cvs[ last ]._value;
		}

		// Determine the interval overwhich to evaluate t
		int i = 1;
		while ( i < last && _cvs[ i ]._t < t ) ++i;
		--i;
		return _evalInterval( i, t );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::evalTangent( float t ) const {
		assert( _cvCount > 0 && "Calling evaluate without any points causes an error" );

		int last = _cvCount - 1;
		// Check if the value is beyond the extents of the curve
		//		TODO: This assumes constant value beyond the boundaries
		//			  OPTIONALLY, allow other extrapolation methods (such as perioid, linear, etc.)
		if ( t <= _cvs[0]._t) {
            return Vector();//_cvs[0]._tangent;
		} else if ( t >= _cvs[ last ]._t ) {
            return Vector();//_cvs[ last ]._tangent;
		}

		// Determine the interval overwhich to evaluate t
		int i = 1;
		while ( i < last && _cvs[ i ]._t < t ) ++i;
		--i;
		return _evalTanInternal( i, t );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::evaluateLinear( float t ) const {
		assert( _cvCount > 0 && "Calling evaluate without any points causes an error" );

		int last = _cvCount - 1;
		// Check if the value is beyond the extents of the curve
		//		TODO: This assumes constant value beyond the boundaries
		//			  OPTIONALLY, allow other extrapolation methods (such as perioid, linear, etc.)
		if ( t <= _cvs[0]._t) {
            return _cvs[0]._value;
		} else if ( t >= _cvs[ last ]._t ) {
            return _cvs[ last ]._value;
		}

		// Determine the interval overwhich to evaluate t
		int i = 1;
		while ( i < last && _cvs[ i ]._t < t ) ++i;
		--i;

		// compute the interval parameters
		float a = _cvs[i]._t;
		float b = _cvs[i+1]._t;
		float h = b - a;
		t = ( t - a ) / h;
		return (1-t) * _cvs[i]._value + t * _cvs[i+1]._value;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void HermiteCurve<Vector>::addPoint(  float t, const Vector & value, const Vector & tangent ) {
		++_cvCount;
		HermitePoint< Vector > * newCvs = new HermitePoint< Vector >[ _cvCount ];
		if ( _cvCount > 1 ) {
			memcpy( newCvs, _cvs, (_cvCount - 1) * sizeof( HermitePoint<Vector> ) );
			delete [] _cvs;
		}
		newCvs[ _cvCount - 1 ] = HermitePoint<Vector>( t, value, tangent );
		_cvs = newCvs;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	int HermiteCurve<Vector>::insertPoint( float t, const Vector & value, const Vector & tangent ) {
		// assume it's already sorted
		++_cvCount;
		HermitePoint< Vector > * newCvs = new HermitePoint< Vector >[ _cvCount ];
		int d = 0;
		int s = 0;
		int insertIdx = -1;
		while ( d < _cvCount ) {
			if ( s < _cvCount - 1 && ( _cvs[s]._t < t || insertIdx != -1 ) ) {
				newCvs[ d ] = _cvs[s];
				++s;
			} else {
				newCvs[ d ] = HermitePoint< Vector >( t, value, tangent );
				insertIdx = d;
			}
			++d;
		}
		delete [] _cvs;
		_cvs = newCvs;

		return insertIdx;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	float HermiteCurve<Vector>::minT() const {
		assert( _cvCount > 0 && "Calling minT without any points causes an error" );
		return _cvs[0]._t;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	float HermiteCurve<Vector>::maxT() const {
		assert( _cvCount > 0 && "Calling maxT without any points causes an error" );
		return _cvs[ _cvCount - 1 ]._t;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void HermiteCurve<Vector>::clear() {
		if ( _cvs ) {
			_cvCount = 0;
			delete [] _cvs;
			_cvs = 0x0;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	std::ostream & operator << ( std::ostream & out, const HermiteCurve<Vector> & curve ) {
		out << curve._cvCount << "\n";
		for ( int i = 0; i < curve._cvCount; ++i ) {
			out << curve._cvs[i]._t << " " << curve._cvs[i]._value << " " << curve._cvs[i]._tangent << "\n";
		}
		//std::cout << "Hermite:";
		//for ( int i = 0; i < curve._cvCount; ++i ) {
		//	std::cout << "\n\t" << curve._cvs[i];
		//}
#if CURVE_SAMPLES
		float dt = (curve._cvs[ curve._cvCount - 1]._t - curve._cvs[0]._t) / (float)CURVE_SAMPLES;
		for ( int i = 0; i <= CURVE_SAMPLES; ++i ) {
			std::cout << "\n" << (i * dt ) << " " << curve.evaluate( i * dt );
		}
#endif
		return out;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void HermiteCurve<Vector>::calcTangent( int i, bool periodic ) {
		int prev, next;
		float h;
		int last = (int)_cvCount - 1;
		if ( i == 0 || i == last) {	// first
			if ( periodic ) {
				prev = last - 1;
				next = 1;
				h = _cvs[1]._t - _cvs[0]._t + _cvs[last]._t - _cvs[last-1]._t;
			} else {
				if ( i == 0 ) {
					_cvs[0]._tangent = (_cvs[1]._value - _cvs[0]._value ) / ( _cvs[1]._t - _cvs[0]._t );
				} else {
					_cvs[last]._tangent = (_cvs[last]._value - _cvs[last-1]._value ) / ( _cvs[last]._t - _cvs[last-1]._t );
				}
				return;
			}
		} else {		// middle
			prev = i - 1;
			next = i + 1;
			h = _cvs[next]._t - _cvs[prev]._t;
		}
		_cvs[i]._tangent = (_cvs[next]._value - _cvs[prev]._value ) / h;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void HermiteCurve<Vector>::calcTangent2( int i, bool periodic ) {
		float h0, h1;
		float f0, f2;
		int last = (int)_cvCount - 1;
		if ( i == 0 || i == last) {	// first
			if ( periodic ) {
				f0 = _cvs[ last - 1 ]._value;
				f2 = _cvs[ 1 ]._value;
				h0 = _cvs[0]._t - _cvs[1]._t;  // note, this LOOKS backwards but matches the taylor expansion
				h1 = _cvs[last]._t - _cvs[last-1]._t;
			} else {
				// if not periodic and on the boundary, simply do the forward/backward difference
				if ( i == 0 ) {
					_cvs[0]._tangent = (_cvs[1]._value - _cvs[0]._value ) / ( _cvs[1]._t - _cvs[0]._t );
				} else {
					_cvs[last]._tangent = (_cvs[last]._value - _cvs[last-1]._value ) / ( _cvs[last]._t - _cvs[last-1]._t );
				}
				return;
			}
		} else {		// middle
			f0 = _cvs[ i - 1 ]._value;
			f2 = _cvs[ i + 1 ]._value;
			h0 = _cvs[ i - 1 ]._t - _cvs[ i ]._t;  // note, this LOOKS backwards but matches the taylor expansion
			h1 = _cvs[ i + 1 ]._t - _cvs[ i ]._t;
		}
		float f1 = _cvs[i]._value;
		float h12 = h1 * h1;
		float h02 = h0 * h0;
		_cvs[i]._tangent = -(h02 * (f2 - f1) + h12 * (f1 - f0) ) / ( h1 * h0 * ( h1 - h0 ) );
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	int HermiteCurve<Vector>::findClosestIndex( float t ) const {
		int count = (int)_cvCount;
		int closest = 0;
		float dist = fabs( _cvs[0]._t - t );
		for ( int i = 1; i < count; ++i ) {
			float d = fabs( _cvs[i]._t - t );
			if ( d < dist ) {
				dist = d;
				closest = i;
			}
		}
		return closest;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	float HermiteCurve<Vector>::findClosestParam( float t ) const {
		int count = (int)_cvCount;
		float closest = _cvs[0]._t;
		float dist = fabs( _cvs[0]._t - t );
		for ( int i = 1; i < count; ++i ) {
			float d = fabs( _cvs[i]._t - t );
			if ( d < dist ) {
				dist = d;
				closest = _cvs[i]._t;
			}
		}
		return closest;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	bool HermiteCurve<Vector>::inverse( const Vector & value, float & param, float threshold ) const {
		// GIven that this assumes monotonically increasing values, I can target this a bit better
		// First, determine what interval the value is in
		assert( _cvCount > 0 && "Calling evaluate without any points causes an error" );

		int last = _cvCount - 1;
		// Check if the value is beyond the extents of the curve
		//		TODO: This assumes constant value beyond the boundaries
		//			  OPTIONALLY, allow other extrapolation methods (such as perioid, linear, etc.)
		if ( value < _cvs[0]._value ) {
            return false;
		} else if ( value > _cvs[ last ]._value ) {
            return false;
		}

		// Determine the interval overwhich to evaluate t
		int i = 1;
		while ( i < last && _cvs[ i ]._value < value ) ++i;
		--i;

		float low, high;
		low = _cvs[i]._t;
		high = _cvs[i+1]._t;
		param = ( low + high ) * 0.5f;
		Vector testVal = _evalInterval( i, param );
		while ( (high - low) > threshold && abs( testVal - value ) > threshold ) {
			if ( testVal > value ) {
				high = param;
				param = ( low + high ) * 0.5f;
				testVal = _evalInterval( i, param );
			} else {
				low = param;
				param = ( low + high ) * 0.5f;
				testVal = _evalInterval( i, param );
			}
		}

		return abs( testVal - value ) > threshold;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	bool HermiteCurve<Vector>::removeParam( float t ) {
		assert( _cvCount > 0 && "Trying to remove key from empty curve" );
		int key = -1;
		for ( int i = 0; i < _cvCount; ++i ) {
			if ( _cvs[i]._t == t ) {
				key = i;
				break;
			}
		}
		if ( key != -1 ) {
			return removeKey( key );
		}

		return false;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	bool HermiteCurve<Vector>::removeKey( int i ) {
		assert( i >= 0 && i < _cvCount && "Trying to remove a key which doesn't exist" );
		
		int newCount = _cvCount - 1;
		HermitePoint< Vector > * newCvs = new HermitePoint< Vector >[ newCount ];
		int s = 0, d = 0;
		while ( d < newCount) {
			if ( s != i ) {
				newCvs[ d ] = _cvs[ s ];
				++d;
			}
			++s;
		}
		delete [] _cvs;
		_cvs = newCvs;
		_cvCount = newCount;

		return false;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::valueConstraint0( float t, Vector val ) {
		int last = _cvCount - 1;
		// identify the interval
		// if I'm outside the domain of the curve, then I should set the end point to be val
		if ( t <= _cvs[0]._t || t >= _cvs[ last ]._t ) {
            return val;
		}

		// Determine the interval overwhich to evaluate t
		int i = 0;
		while ( i < last && _cvs[ i ]._t < t ) ++i;
		--i;

		// compute the interval parameters
		float a = _cvs[i]._t;
		float b = _cvs[i+1]._t;
		float h = b - a;
		t = ( t - a ) / h;
		// compute normalized t

		float t2 = t * t;
		float t3 = t2 * t;
		float right = _cvs[i+1].evaluateAsSecond( t3, t2, t, h );
		float h1 = 2 * t3 - 3 * t2 + 1;
		float h3 = t3 - 2 * t2 + t;
		float tgtVal = ( val - right - _cvs[i]._tangent * ( h3 * h ) ) / h1;

		return tgtVal;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	Vector HermiteCurve<Vector>::valueConstraint1( float t, Vector val ) {
		int last = _cvCount - 1;
		// identify the interval
		// if I'm outside the domain of the curve, then I should set the end point to be val
		if ( t <= _cvs[0]._t || t >= _cvs[ last ]._t ) {
            return val;
		}

		// Determine the interval overwhich to evaluate t
		int i = 0;
		while ( i < last && _cvs[ i ]._t < t ) ++i;
		--i;

		// compute the interval parameters
		float a = _cvs[i]._t;
		float b = _cvs[i+1]._t;
		float h = b - a;
		t = ( t - a ) / h;
		// compute normalized t

		float t2 = t * t;
		float t3 = t2 * t;
		float left = _cvs[i].evaluateAsFirst( t3, t2, t, h );
		float h2 = -2 * t3 + 3 * t2;
		float h4 = t3 - t2;
		float tgtVal = ( val - left - _cvs[i+1]._tangent * h4 * h ) / h2;

		return tgtVal;
	}

	///////////////////////////////////////////////////////////////////////////

	template <class Vector>
	void HermiteCurve<Vector>::setConstrainedTangent1( int key, float constrainT, const Vector & constrainV ) {
		assert( key > 0 && "Can't use the constraint with 0 as an ending key" );
		assert( key < _cvCount && "Invalid key" );

		// compute the interval parameters
		// compute normalized t
		float a = _cvs[key-1]._t;
		float b = _cvs[key]._t;
		float h = b - a;
		float t = ( constrainT - a ) / h;

		float t2 = t * t;
		float t3 = t2 * t;
		float left = _cvs[ key - 1 ].evaluateAsFirst( t3, t2, t, h );
		float h2 = ( 3 * t2 - 2 * t3 ) * _cvs[ key ]._value;
		float h3 = t3 - t2;
		_cvs[ key ]._tangent = ( constrainV - left - h2 ) / ( h3 * h );
	}
}

#endif	// __HERMITE_H__
