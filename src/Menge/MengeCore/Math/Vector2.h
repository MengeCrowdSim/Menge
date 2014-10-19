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

/*!
 *	@file		Vector2.h
 *	@brief		Definition of a vector in R2
 */

#ifndef __VECTOR2_H__
#define	__VECTOR2_H__

#include "CoreConfig.h"
#include <cmath>
#include <ostream>
#include "Logger.h"
#include "Math/consts.h"

#ifdef _MSC_VER
	// To export templated classes and functions, requires declaring specializations
	// extern.  This became standard in C++11, but prior to that it is considered a
	//	"microsoft extension".  This silences the warnings.
	#pragma warning( disable : 4231 )
#endif

namespace Menge {

	namespace Math {
		/*!
		 *  @brief			Computes the square of a float.
		 *  @param			a               The float to be squared.
		 *  @returns		The square of the float.
		 */
		inline MENGE_API float sqr(float a) {
			return a * a;
		}

		/*!
		 *	@brief		Templated vector in R2
		 */
		template <class Type> 
		class MENGE_API Vector2d {
		public:
			/////////////////////////////    CONSTRUCTORS      ////////////////////////////
			/*!
			 *	@brief		Default constructor.  It does NOT initialize the fields
			 */
			inline Vector2d() {}

			/*!
			 *	@brief		Constructor with arguments
			 *
			 *	@param		x		The x-value of the vector.		
			 *	@param		y		The y-value of the vector.
			 */
			inline Vector2d( Type x, Type y ): _x(x), _y(y) {}

			/*!
			 *	@brief		Copy constructor
			 *
			 *	@param		v		The vector to copy from.
			 */
			inline Vector2d( const Vector2d & v ): _x(v._x), _y(v._y) {}

			/////////////////////////////    GETTERS/SETTERS      ////////////////////////////

			/*!
			 *	@brief		Get the x-value
			 *
			 *	@returns	The x-value of the vector.
			 */
			inline Type x() const { return _x; }

			/*!
			 *	@brief		Get the y-value
			 *
			 *	@returns	The y-value of the vector.
			 */
			inline Type y() const { return _y; }

			/*!
			 *	@brief		Set the x- and y-values from scalar values
			 *
			 *	@param		x		The value for the x-component
			 *	@param		y		The value for the y-component
			 */
			inline void set( Type x, Type y ) {
				_x = x;
				_y = y;
			}

			/*!
			 *	@brief		Set the x- and y-values from a vector
			 *
			 *	@param		v		The vector containing the x- and y-values
			 */
			inline void set( const Vector2d & v ) {
				_x = v._x;
				_y = v._y;
			}

			/*!
			 *	@brief		Set the x-value
			 *
			 *	@param		x		The x-value.
			 */
			inline void setX( Type x ) {
				_x = x;
			}

			/*!
			 *	@brief		Set the y-value
			 *
			 *	@param		y		The y-value.
			 */
			inline void setY( Type y ) {
				_y = y;
			}

			/*!
			 *	@brief		Set the vector to zero
			 */
			inline void zero() {
				_x = _y = (Type)0;
			}

			/////////////////////////////    MATH OPERATIONS      ////////////////////////////

			/*!
			 *	@brief		Vector negation.  Creates a new vector which is the
			 *				negation of this vector
			 *
			 *	@returns	The vector in the opposite direction of this vector
			 */
			inline Vector2d<Type> operator-() const {
				return Vector2d<Type>( -_x, -_y );
			}

			/*!
			*  @brief		Computes the dot product of this vector with the
			*				given vector.
			*
			*  @param		v		The two-dimensional vector with which the
			*                       dot product should be computed.
			*  @returns		The dot product of this two-dimensional vector with the
			*				specified two-dimensional vector.
			*/
			inline float operator*(const Vector2d& v) const {
				return _x * v._x + _y * v._y;
			}

			/*!
			*  @brief		Computes the scalar multiplication of this
			*				vector with the given scalar value.
			*
			*  @param		s               The scalar value
			*  @returns		The scalar multiplication of this vector
			*				with the specified scalar value.
			*/
			inline Vector2d<Type> operator*( float s ) const {
				return Vector2d<Type>( _x * s, _y * s );
			}

			/*!
			*  @brief		Computes the scalar division of this
			*				vector with the given scalar value.
			*
			*  @param		s               The scalar value
			*  @returns		The scalar division of this vector
			*				with the specified scalar value.
			*/
			inline Vector2d<Type> operator/( float s ) const {
				const float invS = 1.f / s;
				return Vector2d<Type>( _x * invS, _y * invS );
			}

			/*!
			*  @brief		Computes the vector sum of this vector with
			*				the given vector.
			*
			*  @param		v				The second operand.
			*  @returns		The vector sum of this vector with the
			*				given vector.
			*/
			inline Vector2d operator+( const Vector2d & v ) const {
			  return Vector2d<Type>( _x + v._x, _y + v._y );
			}

			/*!
			*  @brief		Computes the vector difference of this vector with
			*				the given vector.
			*
			*  @param		v				The second operand.
			*  @returns		The vector difference of this vector with the
			*				given vector.
			*/
			inline Vector2d operator-( const Vector2d & v ) const {
			  return Vector2d<Type>( _x - v._x, _y - v._y );
			}

			/*!
			*  @brief		Reports if this vector is the same as the given
			*				vector.
			*
			*  @param		v				The second operand.
			*  @returns		True if the vectors are exactly identical, False
			*				otherwise
			*/
			inline bool operator==( const Vector2d & v ) const {
			  return _x == v._x && _y == v._y;
			}

			/*!
			*  @brief		Reports if this vector is the different from the given
			*				vector.
			*
			*  @param		v				The second operand.
			*  @returns		True if the vectors are at all different,
			*				false otherwise
			*/
			inline bool operator!=( const Vector2d & v ) const {
			  return _x != v._x || _y != v._y;
			}

			/////////////////////////////    IN-PLACE MATH OPERATIONS      ////////////////////////////

			/*!
			 *	@brief		Perform in-place scalar multiplication on this vector.
			 *
			 *	@param		s			The scalar mutliplicand.
			 *	@return		A reference to the vector.
			 */
			inline Vector2d & operator*=( float s ) {
				_x *= s;
				_y *= s;
				return *this;
			}

			/*!
			 *	@brief		Perform in-place scalar division on this vector.
			 *
			 *	@param		s			The scalar divisor.
			 *	@return		A reference to the vector.
			 */
			inline Vector2d & operator/=( float s ) {
				const float invS = 1.f / s;
				_x *= invS;
				_y *= invS;
				return *this;
			}

			/*!
			 *	@brief		Perform in-place vector addition on this vector.
			 *
			 *	@param		v			The second vector operand.
			 *	@return		A reference to the vector.
			 */
			inline Vector2d & operator+=( const Vector2d & v ) {
				_x += v._x;
				_y += v._y;
				return *this;
			}

			/*!
			 *	@brief		Perform in-place vector subtraction on this vector.
			 *
			 *	@param		v			The second vector operand.
			 *	@return		A reference to the vector.
			 */
			inline Vector2d & operator-=( const Vector2d & v ) {
				_x -= v._x;
				_y -= v._y;
				return *this;
			}

			/*!
			 *	@brief		Negate the vector in place
			 */
			inline void	negate() {
				_x = -_x;
				_y = -_y;
			}

			/*!
			 * @brief		Normalize the vector in place
			 */
			inline void normalize() {
				float len = sqrtf( _x * _x + _y * _y );
				if ( len > EPS ) {
					_x /= len;
					_y /= len;
				} else {
					_x = _y = 0.f;
				}
			}

			/////////////////////////////    GEOMETRIC OPERATIONS      ////////////////////////////

			/*!
			 *	@brief		Compute the magnitude (aka length) of the vector
			 *
			 *	@returns	The magnitude of the vector.
			 */
			inline Type Length() const {
				return sqrt( _x * _x + _y * _y );
			}

			/*!
			 *	@brief		Compute the distance from this vector to another point
			 *
			 *	@param		p			The point whose distance from this vector
			 *							(interpreted as a point) is to be computed
			 *	@returns	The distance between this vector and p.
			 */
			inline float distance( const Vector2d & p ) const {
				float dx = _x - p._x;
				float dy = _y - p._y;
				return sqrtf( dx * dx + dy * dy );
			}

			/*!
			 *	@brief		Compute the distance from this vector to another point
			 *
			 *	@param		x			The x-value of the other point.
			 *	@param		y			The y-value of the other point.
			 *	@returns	The distance between this vector and (x, y).
			 */
			inline float distance( float x, float y ) const {
				float dx = _x - x;
				float dy = _y - y;
				return sqrtf( dx * dx + dy * dy );
			}

			/*!
			 *	@brief		Compute the squared-distance from this vector to another point
			 *
			 *	@param		p			The point whose distance from this vector
			 *							(interpreted as a point) is to be computed
			 *	@returns	The squared-distance between this vector and p.
			 */
			inline float distanceSq( const Vector2d & p ) const {
				float dx = _x - p._x;
				float dy = _y - p._y;
				return dx * dx + dy * dy ;
			}

			/*!
			 *	@brief		Compute the squared-distance from this vector to another point
			 *
			 *	@param		x			The x-value of the other point.
			 *	@param		y			The y-value of the other point.
			 *	@returns	The squared-distance between this vector and (x, y).
			 */
			inline float distanceSq( float x, float y ) const {
				float dx = _x - x;
				float dy = _y - y;
				return dx * dx + dy * dy;
			}
			
			/*!
			 *	@brief		x-component of the vector
			 */
			Type	_x;

			/*!
			 *	@brief		y-component of the vector
			 */
			Type	_y;
		};

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template class MENGE_API Vector2d<float>;

		/*!
		 *	@brief		Declaration of a floating point vector in R2.
		 */
		typedef Vector2d<float>  Vector2;

		/////////////////////////////    UTILITY FUNCTIONS      ////////////////////////////


		/*!
		 *  @brief      Computes the left scalar multiplication of a scalar and 2d vector.
		 *
		 *  @param      s               The scalar value with which the scalar
		 *                              multiplication should be computed.
		 *  @param      v          The two-dimensional vector with which the scalar
		 *                              multiplication should be computed.
		 *  @returns    The scalar multiplication of the two-dimensional vector with the
		 *              scalar value.
		 */
		template< class Type >
		inline MENGE_API Vector2d<Type> operator*( Type s, const Vector2d<Type>& v) {
			return Vector2d<Type>( s * v._x, s * v._y );
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API Vector2d<float> operator*( float s, const Vector2d<float>& v);

		/*!
		 *  @brief      Inserts the specified two-dimensional vector into the logger.
		 *
		 *  @param      logger          The logger into which the two-dimensional
		 *                              vector should be inserted.
		 *  @param      v				The two-dimensional vector which to insert into
		 *                              the output stream.
		 *  @returns    A reference to the output stream.
		 */
		template< class Type >
		inline MENGE_API Logger& operator<<(Logger& logger, const Vector2d<Type>& v ) {
			logger << "(" << v.x() << "," << v.y() << ")";
			return logger;
		}

		/*!
		 *  @brief      Inserts the specified two-dimensional vector into the specified
		 *              output stream.
		 *
		 *  @param      os              The output stream into which the two-dimensional
		 *                              vector should be inserted.
		 *  @param      v				The two-dimensional vector which to insert into
		 *                              the output stream.
		 *  @returns    A reference to the output stream.
		 */
		template< class Type >
		inline MENGE_API std::ostream& operator<<(std::ostream& os, const Vector2d<Type>& v ) {
			os << "(" << v.x() << "," << v.y() << ")";
			return os;
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API Logger& operator<<(Logger& os, const Vector2d<float>& v );

		/*!
		 *  @brief      Computes the length of a specified two-dimensional vector.
		 *
		 *  @param      v				The two-dimensional vector whose length is to be
		 *                              computed.
		 *  @returns    The length of the two-dimensional vector.
		 */
		template< class Type >
		inline MENGE_API Type abs( const Vector2d<Type>& v ) {
			return std::sqrt( v * v );
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API float abs( const Vector2d<float>& v );

		/*!
		 *  @brief      Computes the squared length of a specified two-dimensional
		 *              vector.
		 *
		 *  @param      v				The two-dimensional vector whose squared length
		 *                              is to be computed.
		 *  @returns    The squared length of the two-dimensional vector.
		 */
		template< class Type >
		inline MENGE_API Type absSq( const Vector2d<Type>& v ) {
			return v * v;
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API float absSq( const Vector2d<float>& v );

		/*!
		 *  @brief      Computes the determinant of a two-dimensional square matrix with
		 *              rows consisting of the specified two-dimensional vectors.
		 *
		 *  @param      v1				The top row of the two-dimensional square
		 *                              matrix.
		 *  @param      v2				The bottom row of the two-dimensional square
		 *                              matrix.
		 *  @returns    The determinant of the two-dimensional square matrix.
		 */
		template< class Type >
		inline MENGE_API Type det( const Vector2d<Type>& v1, const Vector2d<Type>& v2) {
			return v1.x() * v2.y() - v1.y() * v2.x();
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API float det( const Vector2d<float>& v1, const Vector2d<float>& v2);

		/*!
		 *  @brief      Computes the normalization of the specified two-dimensional
		 *              vector.
		 *
		 *  @param      vector          The two-dimensional vector whose normalization
		 *                              is to be computed.
		 *  @returns    The normalization of the two-dimensional vector.
		 */
		template< class Type >
		inline MENGE_API Vector2d<Type> norm(const Vector2d<Type>& vector) {
			float mag = abs( vector );
			if ( mag < EPS ) {
				// This may not be the "right" behavior.  I do this because the "normalized" vector has unit
				//	length.  This guarantees that the result is always unit length.  Although it introduces other
				//	issues.
				return Vector2d<Type>(1.f, 0.f);
			} else {
				return vector / mag;
			}
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API Vector2d<float> norm(const Vector2d<float>& vector);

		/*!
		 *  @brief	Determines if two vectors are equal to within a threshhold
		 *
		 *  @param	v1				The first of two-dimensional vectors to be compared
		 *  @param	v2				The second of two-dimensional vectors to be compared
		 *  @param	threshSqd		The squared threshhold to test against 
		 *                          defaults to 3 decimal places (0.001)^2
		 *  @returns  A boolean: true if they are equivalent within a threshhold, 
		 *            false otherwise.
		 */
		template< class Type >
		inline MENGE_API bool equivalent( const Vector2d<Type> & v1, const Vector2d<Type> & v2, float threshSqd=0.000001f ) {
			return absSq( v1 - v2 ) < threshSqd;
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API bool equivalent( const Vector2d<float> & v1, const Vector2d<float> & v2, float threshSqd );

		/*! 
		 *  @brief      Computes the signed distance from a line connecting the
		 *              specified points to a specified point.
		 *  @param      a               The first point on the line.
		 *  @param      b               The second point on the line.
		 *  @param      c               The point to which the signed distance is to
		 *                              be calculated.
		 *  @returns    Positive when the point c lies to the left of the line ab.
		 */
		template< class Type >
		inline MENGE_API Type leftOf(const Vector2d<Type>& a, const Vector2d<Type>& b, const Vector2d<Type>& c) {
			return det(a - c, b - a);
		}

		/*!
		 *	@brief		Explicit specialization for shared library export.
		 *	@internal
		 */
		MATHEXTERN template MENGE_API float leftOf(const Vector2d<float>& a, const Vector2d<float>& b, const Vector2d<float>& c);
	}	// namespace Math
}	// namespace Menge

	#ifdef _MSC_VER
		// To export templated classes and functions, requires declaring specializations
		// extern.  This became standard in C++11, but prior to that it is considered a
		//	"microsoft extension".  This silences the warnings.
		#pragma warning( default : 4231 )
	#endif
#endif	//__VECTOR2_H__
