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

#include "XformMatrix.h"

#include <math.h>
#include "graphCommon.h"

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR XformMatrix
		///////////////////////////////////////////////////////////////////////////

		XformMatrix::XformMatrix():_trans(0.f, 0.f, 0.f),
								   _scale(1.f, 1.f, 1.f ),
								   _rot( 0.f, 0.f, 0.f ),
								   _rotAxis( 0.f, 0.f, 0.f ) {
			setDirty();
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::setRotAxisDeg( const Vector3 & vec ) { 
			_rotAxis = vec * DEG_TO_RAD; 
			updateRotAxisMat();
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::setRotAxisRad( const Vector3 & vec ) { 
			_rot = vec; 
			updateRotAxisMat();
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::updateRotAxisMat() {
			setDirty( MAT | IMAT );
			float cx = cosf( _rotAxis.x() );
			float cy = cosf( _rotAxis.y() );
			float cz = cosf( _rotAxis.z() );
			float sx = sinf( _rotAxis.x() );
			float sy = sinf( _rotAxis.y() );
			float sz = sinf( _rotAxis.z() );

			_rotAxisMat.setRow( 0, cy*cz, cy*sz, -sy, 0 );
			_rotAxisMat.setRow( 1, sx*sy*cz - cx*sz, cx*cz + sx*sy*sz, sx*cy, 0 );
			_rotAxisMat.setRow( 2, sx*sz + cx*sy*cz, cx*sy*sz - sx*cz, cx*cy, 0 );
			_rotAxisMat.setRow( 3, 0, 0, 0, 1 );  
			_rotAxisIMat.setAsTranspose( _rotAxisMat );
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::translationMatrix( Matrix4x4 & mat ) {
			mat.identity();
			mat.setRow( 3, _trans );
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::translationInverseMatrix( Matrix4x4 & mat ) {
			mat.identity();
			mat.setRow( 3, -_trans.x(), -_trans.y(), -_trans.z() );
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::scaleMatrix( Matrix4x4 & mat ) {
			mat.identity();
			mat.setDiagonal( _scale );
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::scaleInverseMatrix( Matrix4x4 & mat ) {
			mat.identity();
			mat.setDiagonal( 1.f / _scale.x(), 1.f / _scale.y(), 1.f / _scale.z() );
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::rotationMatrix( Matrix4x4 & mat ) {
			if ( !isClean( ROT_MAT ) ) {
				setClean( ROT_MAT );
				// assumes XYZ rotation
				_rotMat.setRow( 3, 0.f, 0.f, 0.f, 1.f );
				_rotMat(0, 3) = _rotMat(1, 3) = _rotMat(2,3) = 0.f;
				
				float cx = cosf( _rot.x() );
				float sx = sinf( _rot.x() );
				float cy = cosf( _rot.y() );
				float sy = sinf( _rot.y() );
				float cz = cosf( _rot.z() );
				float sz = sinf( _rot.z() );

				_rotMat(0, 0) = cz * cy;
				_rotMat(0, 1) = cy * sz;
				_rotMat(0, 2) = -sy;
				_rotMat(1, 0) = cz * sx * sy - sz * cx;
				_rotMat(1, 1) = sx * sy * sz + cx * cz;
				_rotMat(1, 2) = sx * cy;
				_rotMat(2, 0) = cz * cx * sy + sx * sz;
				_rotMat(2, 1) = sz * cx * sy - sx * cz;
				_rotMat(2, 2) = cx * cy;
			}

			mat = _rotMat;
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::rotationInverseMatrix( Matrix4x4 & mat ) {
			rotationMatrix( mat );
			mat.transpose();
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::getMatrix( Matrix4x4 & mat ) {
			// Matrix is a composition of 5 matrices:
			//	SCALE * rotAxisInverse * ROTATE * rotAxis * TRANSLATE
			// re-written (with cached values) as:
			//	SCALE * PRE-ROTATE * ROTATE * POST-ROTATE * TRANSLATE
			if ( !isClean( MAT ) ) {
				setClean( MAT );
				
				Matrix4x4 temp1(false), temp2(false), rotMat(false);
				rotationMatrix( rotMat );
				temp1.scale( _scale, _rotAxisIMat );
				temp2.product3x3( rotMat, _rotAxisMat );
				_mat.product3x3( temp1, temp2 );
				_mat.translateRotation( _trans );
			}
			mat = _mat;
		}

		///////////////////////////////////////////////////////////////////////////

		void XformMatrix::getInverseMatrix( Matrix4x4 & mat ) {
			// Inverse matrix is a composition of 5 matrices:
			//	(SCALE * rotAxisInverse * ROTATE * rotAxis * TRANSLATE)^-1
			// re-written (with cached values) as:
			//	(SCALE * PRE-ROTATE * ROTATE * POST-ROTATE * TRANSLATE)^-1
			//  = T^-1 * PRE-ROTATE * R^-1 * POST-ROTATE * S^-1
			//		-- NOTE: PRE-ROTATE^-1 = POST-ROTATE (and vice versa)
			if ( !isClean( IMAT ) ) {
				setClean( IMAT );
				Matrix4x4 temp1(false), temp2(false), rotMat(false);
				rotationInverseMatrix( rotMat );
				temp1.product3x3( _rotAxisIMat, rotMat );
				// the scale matrix can be left or right multiplied to the same effect
				temp2.scaleRight( _scale, _rotAxisMat );
				_iMat.product3x3( temp1, temp2 );
				_iMat.translateRotationLeft( -_trans );
			}
			mat = _iMat;
		}

		///////////////////////////////////////////////////////////////////////////

	}	// namespace SceneGraph
}	// namespace Menge