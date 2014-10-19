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
 *	@file		XformMatrix.h
 *	@brief		Defines the math of performing 3D transformation using a
 *				4x4 homgeneous matrix.
 */

#ifndef __XFORMMATRIX_H__
#define __XFORMMATRIX_H__

#include "CoreConfig.h"
#include "graphCommon.h"
#include <iostream>

namespace Menge {

	namespace SceneGraph {

		class Transform;

		/*!
		 *	@brief		The transformation matrix.
		 *
		 *	The transform matrix is the concatenation of *five* transformation matrices
		 *		SCALE * ROT_AXIS^-1 * ROTATE * ROT_AXIS * TRANSLATE
		 *	The scale, rotation, and translation matrices are self-explanatory.
		 *	They account for the changes in size, rotation around the orign, and re-positioning
		 *	of the node.  The transform also defines a "rotation axis".  The rotation
		 *	values can be defined around an arbitrary axis, rather than the node's local
		 *	space.  One can think of the rotation axis as a pre-rotation.
		 */
		class MENGE_API XformMatrix {
		public:
			/*!
			 *	@brief		Default constructor - identity matrix.
			 */
			XformMatrix();
			
			/*!
			 *	@brief		Set the translation of this node.
			 *
			 *	@param		vec		The 3D position of the transform.
			 */
			void setTranslation( const Vector3 & vec ) { _trans = vec; setDirty( MAT | IMAT ); }
			
			/*!
			 *	@brief		Offset the translation of this node.
			 *
			 *	@param		vec		The change to position of this node.
			 */
			void addTranslation( const Vector3 & vec ) { _trans += vec; setDirty( MAT | IMAT ); }
			
			/*!
			 *	@brief		Set the scale of this node.
			 *
			 *	@param		vec		The desired scale of this node.
			 */
			void setScale( const Vector3 & vec ) { _scale = vec; setDirty( MAT | IMAT ); }
			
			/*!
			 *	@brief		Set the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in degrees.
			 */
			void setRotationDeg( const Vector3 & vec ) { _rot = vec * DEG_TO_RAD; setDirty(); }

			/*!
			 *	@brief		Set the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in radians.
			 */
			void setRotationRad( const Vector3 & vec ) { _rot = vec; setDirty(); }

			/*!
			 *	@brief		Offsets the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. In this case, each axis is
			 *	summed independently.  Generally, because these are Euler angles, this will
			 *	will not lead to linear interpolation of orientation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in degrees.
			 */
			void addRotationDeg( const Vector3 & vec ) { _rot.SumScale( DEG_TO_RAD, vec ); setDirty(); }
			
			/*!
			 *	@brief		Offsets the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. In this case, each axis is
			 *	summed independently.  Generally, because these are Euler angles, this will
			 *	will not lead to linear interpolation of orientation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in radians.
			 */
			void addRotationRad( const Vector3 & vec ) { _rot += vec; setDirty(); }
			
			/*!
			 *	@brief		Set the orientation of this node's rotation axis.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The orientation of the rotation axis around the x-, y-, 
			 *						and z-axes, respectively in degrees.
			 */
			void setRotAxisDeg( const Vector3 & vec );
			
			/*!
			 *	@brief		Set the orientation of this node's rotation axis.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The orientation of the rotation axis around the x-, y-, 
			 *						and z-axes, respectively in radians.
			 */
			void setRotAxisRad( const Vector3 & vec );

			/*!
			 *	@brief		Comptues the rotation p
			 */
			//void rotatePivotMatrix( Matrix4x4 & mat );
			/*!
			 *	@brief		Computes the translation matrix.
			 *
			 *	@param		mat		Writes the translation matrix in the provided matrix.
			 */
			void translationMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the translation inverse matrix.
			 *
			 *	@param		mat		Writes the inverse translation matrix in the provided matrix.
			 */
			void translationInverseMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the scale matrix.
			 *
			 *	@param		mat		Writes the scale matrix in the provided matrix.
			 */
			void scaleMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the inverse scale matrix.
			 *
			 *	@param		mat		Writes the inverse scale matrix in the provided matrix.
			 */
			void scaleInverseMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the rotation matrix.
			 *
			 *	@param		mat		Writes the rotation matrix in the provided matrix.
			 */
			void rotationMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the inverse rotation matrix.
			 *
			 *	@param		mat		Writes the inverse rotation matrix in the provided matrix.
			 */
			void rotationInverseMatrix( Matrix4x4 & mat );

			/*!
			 *	@brief		Computes the transformation matrix.
			 *
			 *	@param		mat		Writes the matrix in the provided matrix.
			 */
			void getMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Computes the inverse transformation matrix.
			 *
			 *	@param		mat		Writes the inverse matrix in the provided matrix.
			 */
			void getInverseMatrix( Matrix4x4 & mat );

			friend class Transform; 

			/*!
			 *	@brief		Print the matrix to the output stream.
			 *
			 *	@param		out			The output stream.
			 *	@param		xformMat	The transformation matrix to print to the output stream.
			 */
			friend Logger & operator << ( Logger & out, const XformMatrix & xformMat );

		protected:

			/*!
			 *	@brief		The translation component of the transformation.
			 */
			Vector3	_trans;	
			
			/*!
			 *	@brief		The scale component of the transformation.
			 */
			Vector3 _scale;	
			
			/*!
			 *	@brief		The rotation component of the transformation.
			 *				Euler angles in radians.
			 */
			Vector3 _rot;	
			
			/*!
			 *	@brief		The rotation axis component of the transformation.
			 *				Pre-rotation Euler angles in radians.
			 */
			Vector3 _rotAxis;	

			/*!
			 *	@brief		Cached rotation axis matrix.
			 */
			Matrix4x4	_rotAxisMat;
			
			/*!
			 *	@brief		Cached inverse rotation axis matrix.
			 */
			Matrix4x4	_rotAxisIMat;

			/*!
			 *	@brief		Enumeration of dirty matrices.
			 *
			 *	Used to intelligently compute matrices as necessary.
			 */
			enum MatrixBit {
				ROT_MAT = 1,	// rotation matrix
				MAT = 2,		// full matrix
				IMAT = 4		// inverse matrix
			};

			/*!
			 *	@brief		An integer mask for determining which cached matrices are clean/dirty
			 *
			 *	Works with the MatrixBit enumeration to determine which matrices need to be
			 *	recomputed.
			 */
			int		_clean;

			/*! 
			 *	@brief		Cached rotation matrix.
			 */
			Matrix4x4	_rotMat;
			
			/*! 
			 *	@brief		Cached transformation matrix.
			 */
			Matrix4x4	_mat;
			
			/*! 
			 *	@brief		Cached inverse transformation matrix.
			 */
			Matrix4x4	_iMat;

			/*!
			 *	@brief		Update cached rotation axis matrices when component changes.
			 */
			void updateRotAxisMat();

			/*!
			 *	@brief		Set all matrices dirty.
			 */
			inline void setDirty() { _clean = 0; }
			
			/*!
			 *	@brief		Set the matrix corresponding to the given bit dirty.
			 *
			 *	@param		bit		The bit for the specific matrix to set dirty.  Should
			 *						be a MatrixBit enumeration value.
			 */
			inline void setDirty( int bit ) { _clean &= ~bit; }
			
			/*!
			 *	@brief		Set the matrix corresponding to the given bit dirty.
			 *
			 *	@param		bit		The bit for the specific matrix to set dirty. 
			 */
			inline void setDirty( MatrixBit bit ) { _clean &= (int)(~bit); }
			
			/*!
			 *	@brief		Reports if the indicated matrix is clean.
			 *
			 *	@param		bit		The bit for the specific matrix to set dirty.  
			 *	@returns	True if the matrix does NOT need recomputation, false otherwise.
			 */
			inline bool isClean( MatrixBit bit ) { return ( _clean & (int)bit ) != 0x0; }
			
			/*!
			 *	@brief		Sets the indicated matrix to be clean.
			 *
			 *	@param		bit		The bit for the specific matrix to set clean.  
			 */
			inline void setClean( MatrixBit bit ) { _clean |= (int)bit; }
			
			/*!
			 *	@brief		Sets the indicated matrix to be clean.
			 *
			 *	@param		bit		The bit for the specific matrix to set clean.  Should
			 *						be a MatrixBit enumeration value.
			 */
			inline void setClean( int bit ) { _clean |= bit; }
		};
	}	// namespace SceneGraph
}	// namespace Menge
#endif	// __XFORMMATRIX_H__
