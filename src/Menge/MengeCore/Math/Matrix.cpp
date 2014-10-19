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

#include "Matrix.h"
#include <cstring>
#include <cassert>

namespace Menge {

	namespace Math {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR Matrix4x4
		///////////////////////////////////////////////////////////////////////////

		Matrix4x4 IDENTITY4x4;

		///////////////////////////////////////////////////////////////////////////

		Matrix4x4::Matrix4x4() {
			identity();
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::identity() {
			memset(_flatData, 0, sizeof(float)*16);
			_matData[0][0] = _matData[1][1] = _matData[2][2] = _matData[3][3] = 1.f;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::setRow( int row, float v0, float v1, float v2, float v3 ) {
			assert( row >= 0 && row < 4 && "Invalid row index!\n" );
			_matData[row][0] = v0;
			_matData[row][1] = v1;
			_matData[row][2] = v2;
			_matData[row][3] = v3;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::setRow( int row, const Vector3 & vec, float v3 ) {
			assert( row >= 0 && row < 4 && "Invalid row index!\n" );
			_matData[row][0] = vec._x;
			_matData[row][1] = vec._y;
			_matData[row][2] = vec._z;
			_matData[row][3] = v3;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::scale( const Vector3 & scale, Matrix4x4 & m ) {
			_matData[0][0] = m._matData[0][0] * scale._x;
			_matData[0][1] = m._matData[0][1] * scale._x;
			_matData[0][2] = m._matData[0][2] * scale._x;
			_matData[0][3] = m._matData[0][3] * scale._x;

			_matData[1][0] = m._matData[1][0] * scale._y;
			_matData[1][1] = m._matData[1][1] * scale._y;
			_matData[1][2] = m._matData[1][2] * scale._y;
			_matData[1][3] = m._matData[1][3] * scale._y;

			_matData[2][0] = m._matData[2][0] * scale._z;
			_matData[2][1] = m._matData[2][1] * scale._z;
			_matData[2][2] = m._matData[2][2] * scale._z;
			_matData[2][3] = m._matData[2][3] * scale._z;

			_matData[3][0] = m._matData[3][0];
			_matData[3][1] = m._matData[3][1];
			_matData[3][2] = m._matData[3][2];
			_matData[3][3] = m._matData[3][3];
			
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::scaleRight( const Vector3 & scale, Matrix4x4 & m ) {
			_matData[0][0] = m._matData[0][0] * scale._x;
			_matData[1][0] = m._matData[1][0] * scale._x;
			_matData[2][0] = m._matData[2][0] * scale._x;
			_matData[3][0] = m._matData[3][0] * scale._x;

			_matData[0][1] = m._matData[0][1] * scale._y;
			_matData[1][1] = m._matData[1][1] * scale._y;
			_matData[2][1] = m._matData[2][1] * scale._y;
			_matData[3][1] = m._matData[3][1] * scale._y;

			_matData[0][2] = m._matData[0][2] * scale._z;
			_matData[1][2] = m._matData[1][2] * scale._z;
			_matData[2][2] = m._matData[2][2] * scale._z;
			_matData[3][2] = m._matData[3][2] * scale._z;

			_matData[3][0] = m._matData[3][0];
			_matData[3][1] = m._matData[3][1];
			_matData[3][2] = m._matData[3][2];
			_matData[3][3] = m._matData[3][3];
			
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::translateRotation( const Vector3 & trans ) {
			_matData[3][0] = trans._x;
			_matData[3][1] = trans._y;
			_matData[3][2] = trans._z;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::translateRotationLeft( const Vector3 & trans ) {
			_matData[3][0] = _matData[0][0] * trans._x + _matData[1][0] * trans._y + _matData[2][0] * trans._z;
			_matData[3][1] = _matData[0][1] * trans._x + _matData[1][1] * trans._y + _matData[2][1] * trans._z;
			_matData[3][2] = _matData[0][2] * trans._x + _matData[1][2] * trans._y + _matData[2][2] * trans._z;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::setDiagonal( float v0, float v1, float v2, float v3 ) {
			_matData[0][0] = v0;	
			_matData[1][1] = v1;
			_matData[2][2] = v2;
			_matData[3][3] = v3;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::setDiagonal( const Vector3 & vec, float v3 ) {
			_matData[0][0] = vec._x;	
			_matData[1][1] = vec._y;
			_matData[2][2] = vec._z;
			_matData[3][3] = v3;
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::product( const Matrix4x4 & m1, const Matrix4x4 & m2 ) {
			_matData[0][0] = m1._matData[0][0] * m2._matData[0][0] + m1._matData[0][1] * m2._matData[1][0] + m1._matData[0][2] * m2._matData[2][0] + m1._matData[0][3] * m2._matData[3][0];
			_matData[0][1] = m1._matData[0][0] * m2._matData[0][1] + m1._matData[0][1] * m2._matData[1][1] + m1._matData[0][2] * m2._matData[2][1] + m1._matData[0][3] * m2._matData[3][1];
			_matData[0][2] = m1._matData[0][0] * m2._matData[0][2] + m1._matData[0][1] * m2._matData[1][2] + m1._matData[0][2] * m2._matData[2][2] + m1._matData[0][3] * m2._matData[3][2];
			_matData[0][3] = m1._matData[0][0] * m2._matData[0][3] + m1._matData[0][1] * m2._matData[1][3] + m1._matData[0][2] * m2._matData[2][3] + m1._matData[0][3] * m2._matData[3][3];

			_matData[1][0] = m1._matData[1][0] * m2._matData[0][0] + m1._matData[1][1] * m2._matData[1][0] + m1._matData[1][2] * m2._matData[2][0] + m1._matData[1][3] * m2._matData[3][0];
			_matData[1][1] = m1._matData[1][0] * m2._matData[0][1] + m1._matData[1][1] * m2._matData[1][1] + m1._matData[1][2] * m2._matData[2][1] + m1._matData[1][3] * m2._matData[3][1];
			_matData[1][2] = m1._matData[1][0] * m2._matData[0][2] + m1._matData[1][1] * m2._matData[1][2] + m1._matData[1][2] * m2._matData[2][2] + m1._matData[1][3] * m2._matData[3][2];
			_matData[1][3] = m1._matData[1][0] * m2._matData[0][3] + m1._matData[1][1] * m2._matData[1][3] + m1._matData[1][2] * m2._matData[2][3] + m1._matData[1][3] * m2._matData[3][3];

			_matData[2][0] = m1._matData[2][0] * m2._matData[0][0] + m1._matData[2][1] * m2._matData[1][0] + m1._matData[2][2] * m2._matData[2][0] + m1._matData[2][3] * m2._matData[3][0];
			_matData[2][1] = m1._matData[2][0] * m2._matData[0][1] + m1._matData[2][1] * m2._matData[1][1] + m1._matData[2][2] * m2._matData[2][1] + m1._matData[2][3] * m2._matData[3][1];
			_matData[2][2] = m1._matData[2][0] * m2._matData[0][2] + m1._matData[2][1] * m2._matData[1][2] + m1._matData[2][2] * m2._matData[2][2] + m1._matData[2][3] * m2._matData[3][2];
			_matData[2][3] = m1._matData[2][0] * m2._matData[0][3] + m1._matData[2][1] * m2._matData[1][3] + m1._matData[2][2] * m2._matData[2][3] + m1._matData[2][3] * m2._matData[3][3];

			_matData[3][0] = m1._matData[3][0] * m2._matData[0][0] + m1._matData[3][1] * m2._matData[1][0] + m1._matData[3][2] * m2._matData[2][0] + m1._matData[3][3] * m2._matData[3][0];
			_matData[3][1] = m1._matData[3][0] * m2._matData[0][1] + m1._matData[3][1] * m2._matData[1][1] + m1._matData[3][2] * m2._matData[2][1] + m1._matData[3][3] * m2._matData[3][1];
			_matData[3][2] = m1._matData[3][0] * m2._matData[0][2] + m1._matData[3][1] * m2._matData[1][2] + m1._matData[3][2] * m2._matData[2][2] + m1._matData[3][3] * m2._matData[3][2];
			_matData[3][3] = m1._matData[3][0] * m2._matData[0][3] + m1._matData[3][1] * m2._matData[1][3] + m1._matData[3][2] * m2._matData[2][3] + m1._matData[3][3] * m2._matData[3][3];
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::product3x3( const Matrix4x4 & m1, const Matrix4x4 & m2 ) {
			_matData[0][3] = _matData[1][3] = _matData[2][3] = _matData[3][0] = _matData[3][1] = _matData[3][2] = 0.f;
			_matData[3][3] = 1.f;

			_matData[0][0] = m1._matData[0][0] * m2._matData[0][0] + m1._matData[0][1] * m2._matData[1][0] + m1._matData[0][2] * m2._matData[2][0];
			_matData[0][1] = m1._matData[0][0] * m2._matData[0][1] + m1._matData[0][1] * m2._matData[1][1] + m1._matData[0][2] * m2._matData[2][1];
			_matData[0][2] = m1._matData[0][0] * m2._matData[0][2] + m1._matData[0][1] * m2._matData[1][2] + m1._matData[0][2] * m2._matData[2][2];
			
			_matData[1][0] = m1._matData[1][0] * m2._matData[0][0] + m1._matData[1][1] * m2._matData[1][0] + m1._matData[1][2] * m2._matData[2][0];
			_matData[1][1] = m1._matData[1][0] * m2._matData[0][1] + m1._matData[1][1] * m2._matData[1][1] + m1._matData[1][2] * m2._matData[2][1];
			_matData[1][2] = m1._matData[1][0] * m2._matData[0][2] + m1._matData[1][1] * m2._matData[1][2] + m1._matData[1][2] * m2._matData[2][2];

			_matData[2][0] = m1._matData[2][0] * m2._matData[0][0] + m1._matData[2][1] * m2._matData[1][0] + m1._matData[2][2] * m2._matData[2][0];
			_matData[2][1] = m1._matData[2][0] * m2._matData[0][1] + m1._matData[2][1] * m2._matData[1][1] + m1._matData[2][2] * m2._matData[2][1];
			_matData[2][2] = m1._matData[2][0] * m2._matData[0][2] + m1._matData[2][1] * m2._matData[1][2] + m1._matData[2][2] * m2._matData[2][2];

			_matData[3][0] = m1._matData[3][0] * m2._matData[0][0] + m1._matData[3][1] * m2._matData[1][0] + m1._matData[3][2] * m2._matData[2][0];
			_matData[3][1] = m1._matData[3][0] * m2._matData[0][1] + m1._matData[3][1] * m2._matData[1][1] + m1._matData[3][2] * m2._matData[2][1];
			_matData[3][2] = m1._matData[3][0] * m2._matData[0][2] + m1._matData[3][1] * m2._matData[1][2] + m1._matData[3][2] * m2._matData[2][2];
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::setAsTranspose( Matrix4x4 & m1 ) {
			_matData[0][0] = m1._matData[0][0];
			_matData[0][1] = m1._matData[1][0];
			_matData[0][2] = m1._matData[2][0];
			_matData[0][3] = m1._matData[3][0];

			_matData[1][0] = m1._matData[0][1];
			_matData[1][1] = m1._matData[1][1];
			_matData[1][2] = m1._matData[2][1];
			_matData[1][3] = m1._matData[3][1];

			_matData[2][0] = m1._matData[0][2];
			_matData[2][1] = m1._matData[1][2];
			_matData[2][2] = m1._matData[2][2];
			_matData[2][3] = m1._matData[3][2];

			_matData[3][0] = m1._matData[0][3];
			_matData[3][1] = m1._matData[1][3];
			_matData[3][2] = m1._matData[2][3];
			_matData[3][3] = m1._matData[3][3];
		}

		///////////////////////////////////////////////////////////////////////////

		void Matrix4x4::transpose() {
			float t;
			t = _matData[0][1];
			_matData[0][1] = _matData[1][0];
			_matData[1][0] = t;
			t = _matData[0][2];
			_matData[0][2] = _matData[2][0];
			_matData[2][0] = t;
			t = _matData[0][3];
			_matData[0][3] = _matData[3][0];
			_matData[3][0] = t;

			t = _matData[1][2];
			_matData[1][2] = _matData[2][1];
			_matData[2][1] = t;
			t = _matData[1][3];
			_matData[1][3] = _matData[3][1];
			_matData[3][1] = t;

			t = _matData[2][3];
			_matData[2][3] = _matData[3][2];
			_matData[3][2] = t;
		}

		///////////////////////////////////////////////////////////////////////////

		Logger & operator << ( Logger & out, const Matrix4x4 & mat ) {
			out << mat._matData[0][0] << " " << mat._matData[0][1] << " " << mat._matData[0][2] << " " << mat._matData[0][3] << "\n";
			out << mat._matData[1][0] << " " << mat._matData[1][1] << " " << mat._matData[1][2] << " " << mat._matData[1][3] << "\n";
			out << mat._matData[2][0] << " " << mat._matData[2][1] << " " << mat._matData[2][2] << " " << mat._matData[2][3] << "\n";
			out << mat._matData[3][0] << " " << mat._matData[3][1] << " " << mat._matData[3][2] << " " << mat._matData[3][3];
			return out;
		}

		///////////////////////////////////////////////////////////////////////////
	 }	// namespace Math
 }	// namespace Menge