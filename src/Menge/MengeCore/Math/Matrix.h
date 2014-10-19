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
 *	@file		Matrix.h
 *	@brief		The definition of a 4x4 transformation matrix.
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "CoreConfig.h"
#include "vector.h"
#include <string.h>
#include <iostream>

namespace Menge {

	namespace Math {
		/*!
		 *	@brief		Basic 4x4 matrix of floats.
		 *
		 *	Functions predominantly come in the form result.op( operand1, operand2 ) to limit 
		 *	implicit data copying.  The operations is performed on the parameter, and the
		 *	result is stored in the instance calling the operation.
		 *	
		 *	The data is stored column-major data -- i.e. the data is organized:
		 *		[ [x-axis   0]
		 *		  [y-axis   0]
		 *		  [z-axis   0]
		 *		  [tx ty tz 1] ]
		 *	It's assumed that multiplication with vectors is LEFT-multiplication by row vectors
		 *	i.e. q = p * M  (where q & p are vectors and M is matrix).	
		 */
		class MENGE_API Matrix4x4 {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	This constructor initializes the matrix to be the identity.
			 */
			Matrix4x4();				

			/*!
			 *	@brief		Non-initializing constructor.
			 *
			 *	By constructing with an arbitrary boolean, the matrix will
			 *	*not* be initialized.
			 *
			 *	@param		garbage		The ignored boolean.
			 */
			Matrix4x4( bool garbage ){}	// DOESN'T initialize data

			/*!
			 *	@brief		Sets the matrix to the identity matrix.
			 */
			void identity();

			/*!
			 *	@brief		Index operation.
			 *
			 *	There is no run-time check on the index values.
			 *
			 *	@param		row			The row index (should be in the range [0, 3]).
			 *	@param		col			The column index (should be in the range [0, 3]).
			 *	@returns	The value of the matrix at (row, col).
			 */
			inline float operator()( int row, int col ) const { return _matData[row][col]; }
			
			/*!
			 *	@brief		Reference index operation.
			 *
			 *	There is no run-time check on the index values.
			 *
			 *	@param		row			The row index (should be in the range [0, 3]).
			 *	@param		col			The column index (should be in the range [0, 3]).
			 *	@returns	A reference to the matrix entry at (row, col).
			 */
			inline float & operator()( int row, int col ) { return _matData[row][col]; }

			/*!
			 *	@brief		Set the values of an entire row of the matrix.
			 *
			 *	@param		row		The index of the row to set (should be in the range [0, 3]).
			 *	@param		v0		The value for the first column.
			 *	@param		v1		The value for the second column.
			 *	@param		v2		The value for the third column.
			 *	@param		v3		The value for the fourth column.
			 */
			void setRow( int row, float v0, float v1, float v2, float v3 = 1.f );
			
			/*!
			 *	@brief		Set the values of an entire row of the matrix.
			 *
			 *	@param		row		The index of the row to set (should be in the range [0, 3]).
			 *	@param		vec		The value for the first, second, and third columns.
			 *	@param		v3		The value for the fourth column.
			 */
			void setRow( int row, const Vector3 & vec, float v3 = 1.f );
			
			/*!
			 *	@brief		Multiplies the given matrix by an implicit scale matrix
			 *				and stores it here.
			 *
			 *	The scale vector parameter is <sx, sy, sz>, which implicitly defines
			 *	a scale transformation matrix S (with sx, sy, sz, 1 on the diagonal and zeros
			 *	everywhere else). 
			 *	We then perform the matrix *left* multiplication: S * m and assign it to this
			 *	matrix.
			 *
			 *	@param		scale		A vector of scale amounts along the three axes
			 *							<sx, sy, sz>.
			 *	@param		m			The matrix to scale: perform S * m.
			 */
			void scale( const Vector3 & scale, Matrix4x4 & m );

			/*!
			 *	@brief		Multiplies the given matrix by an implicit scale matrix
			 *				and stores it here.
			 *
			 *	The scale vector parameter is <sx, sy, sz>, which implicitly defines
			 *	a scale transformation matrix S (with sx, sy, sz, 1 on the diagonal and zeros
			 *	everywhere else). 
			 *	We then perform the matrix *right* multiplication: m * S and assign it to this
			 *	matrix.
			 *
			 *	@param		scale		A vector of scale amounts along the three axes
			 *							<sx, sy, sz>.
			 *	@param		m			The matrix to scale: perform m * S.
			 */
			void scaleRight( const Vector3 & scale, Matrix4x4 & m );

			/*!
			 *	@brief		Compute the trace of the 4x4 matrix.
			 *
			 *	The trace of the matrix is the product of the values on the matrix's diagonal.
			 *
			 *	@returns	The trace of the matrix.
			 */
			inline float trace() const {
				return _matData[0][0] * _matData[1][1] * _matData[2][2] * _matData[3][3];
			};

			/*!
			 *	@brief		Compute the trace of the upper-left 3x3 sub-matrix.
			 *
			 *	The trace of the matrix is the product of the values on the matrix's diagonal.
			 *
			 *	@returns	The trace of the upper, left 3x3 sub-matrix.
			 */
			inline float trace3x3() const {
				return _matData[0][0] * _matData[1][1] * _matData[2][2];
			};

			/*!
			 *	@brief		Right-multiply this matrix by a translation matrix.
			 *
			 *	This should *only* be used if this matrix is known to have the vector <0,0,0,1> in
			 *	both the last row and the last column.  This method exploits that knowledge to
			 *	perform the matrix multiplication efficiently.
			 *	The result of the multiplication is written in place.
			 *	Essentially, this is an optimized version of M = M * T.  
			 *	Where T is *almost* the identity matrix, but with <tx, ty, tz, 0> on the bottom row.
			 *
			 *	@param		trans		The translation vector <tx, ty, tz>
			 */
			void translateRotation( const Vector3 & trans );
			
			/*!
			 *	@brief		Left-multiply this matrix by a translation matrix.
			 *
			 *	This should *only* be used if this matrix is known to have the vector <0,0,0,1> in
			 *	both the last row and the last column.  This method exploits that knowledge to
			 *	perform the matrix multiplication efficiently.
			 *	The result of the multiplication is written in place.
			 *	Essentially, this is an optimized version of M = T * M.  
			 *	Where T is *almost* the identity matrix, but with <tx, ty, tz, 0> on the bottom row.
			 *
			 *	@param		trans		The translation vector <tx, ty, tz>
			 */
			void translateRotationLeft( const Vector3 & trans );

			/*!
			 *	@brief		Sets the diagonal to the given values.
			 *
			 *	@param		v0		The value for M[0, 0].
			 *	@param		v1		The value for M[1, 1].
			 *	@param		v2		The value for M[2, 2].
			 *	@param		v3		The value for M[3, 3].
			 */
			void setDiagonal( float v0, float v1, float v2, float v3 = 1.f );
			
			/*!
			 *	@brief		Sets the diagonal to the given values.
			 *
			 *	@param		vec		The values for M[0, 0], M[1, 1], and M[2, 2].
			 *	@param		v3		The value for M[3, 3].
			 */
			void setDiagonal( const Vector3 & vec, float v3 = 1.f );

			/*!
			 *	@brief		Performs the matrix product and stores the result in this
			 *				matrix.
			 *
			 *	Computes the 4x4 matrix product: m1 * m2
			 *
			 *	@param		m1		The left-hand matrix.
			 *	@param		m2		The right-hand matrix.
			 */
			void product( const Matrix4x4 & m1, const Matrix4x4 & m2 );

			/*!
			 *	@brief		Computes a 3x3 matrix multiplication on the inputs
			 *				storing the result in this matrix.
			 *
			 *	Computes the 3x3 matrix product: m1 * m2.
			 *	The final column and row of this matrix are set to the vector
			 *	<0, 0, 0, 1>.
			 *
			 *	@param		m1		The left-hand matrix.
			 *	@param		m2		The right-hand matrix.
			 */
			void product3x3( const Matrix4x4 & m1, const Matrix4x4 & m2 );

			/*!
			 *	@brief		Set this matrix to be the transpose of the given matrix.
			 *
			 *	@param		m1		The matrix whose transpose is stored in this matrix.
			 */
			void setAsTranspose( Matrix4x4 & m1 );

			/*!
			 *	@brief		Transpose this matrix in-place.
			 */
			void transpose();

			/*!
			 *	@brief		Ouput the string-formatted matrix to an output stream.
			 *
			 *	@param		out		The output stream.
			 *	@param		mat		The matrix.
			 *	@returns	A reference to the output stream.
			 */
			friend Logger & operator << ( Logger & out, const Matrix4x4 & mat );

			/*!
			 *	@brief		Get a pointer to the underlying data as a flat array.
			 *
			 *	@returns	A pointer to the underlying, column-major data.
			 */
			inline float * getFlattened() { return _flatData; }

		private:
			/*!
			 *	@union		MatrixData
			 *	@brief		The underlying matrix data.
			 */
			union {
				float _flatData[16];	///< The data represented as a flat array.
				float _matData[4][4];	///< The data represented as a 2D array.
			};
		};

		/*!
		 *	@brief		A convenenient identity matrix.
		 */
		extern MENGE_API Matrix4x4 IDENTITY4x4;
	}	// namespace Math
}	// namespace Menge
#endif // __MATRIX_H__
