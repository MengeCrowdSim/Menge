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
 *	@file	Transform.h
 *	@brief	Scene graph node which carries rigid transformations.
 */
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "CoreConfig.h"
#include "GLNode.h"
#include "XformMatrix.h"

namespace Menge {

	namespace SceneGraph {
		/*!
		 *	@brief		Scene graph node which applies transforms to nodes.
		 *
		 *	The Transform uses the XformMatrix to compute the transformation math.
		 *	See XformMatrix for the details of the math.
		 */
		class MENGE_API Transform : public GLDagNode {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		parent		A pointer to the optional parent node in the
			 *							graph.
			 */
			Transform( GLDagNode * parent=0x0 );
			
			/*!
			 *	@brief		Set the translation of this node.
			 *
			 *	@param		vec		The 3D position of the transform.
			 */
			inline void setTranslation( const Vector3 & vec ) { _xform.setTranslation( vec ) ; }
			
			/*!
			 *	@brief		Offset the translation of this node.
			 *
			 *	@param		vec		The change to position of this node.
			 */
			inline void addTranslation( const Vector3 & vec ) { _xform.addTranslation( vec ); }
			
			/*!
			 *	@brief		Set the scale of this node.
			 *
			 *	@param		vec		The desired scale of this node.
			 */
			inline void setScale( const Vector3 & vec ) { _xform.setScale( vec ); }

			/*!
			 *	@brief		Set the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in degrees.
			 */
			inline void setRotationDeg( const Vector3 & vec ) { _xform.setRotationDeg( vec ); }
			
			/*!
			 *	@brief		Set the orientation of this node.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The rotations around the x-, y-, and z-axes, respectively
			 *						in radians.
			 */
			inline void setRotationRad( const Vector3 & vec ) { _xform.setRotationRad( vec ); }
			
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
			inline void addRotationDeg( const Vector3 & vec ) { _xform.addRotationDeg( vec ); }

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
			inline void addRotationRad( const Vector3 & vec ) { _xform.addRotationRad( vec ); }

			/*!
			 *	@brief		Set the orientation of this node's rotation axis.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The orientation of the rotation axis around the x-, y-, 
			 *						and z-axes, respectively in degrees.
			 */
			inline void setRotAxisDeg( const Vector3 & vec ) { _xform.setRotAxisDeg( vec ); }
			
			/*!
			 *	@brief		Set the orientation of this node's rotation axis.
			 *
			 *	Rotation is performed using Euler angles and the implict evaluation order
			 *	of x-, then y-, and finally z-axis rotation. 
			 *
			 *	@param		vec		The orientation of the rotation axis around the x-, y-, 
			 *						and z-axes, respectively in radians.
			 */
			inline void setRotAxisRad( const Vector3 & vec ) { _xform.setRotAxisRad( vec ); }

			/*!
			 *	@brief		Reports the transform's translation value.
			 *
			 *	@returns	The translation of this matrix.
			 */
			const Vector3 & translation() const { return _xform._trans; }
			
			/*!
			 *	@brief		Reports the transform's orientation value.
			 *
			 *	@returns	The orientation of this matrix.
			 */
			const Vector3 & rotation() const { return _xform._rot; }
			
			/*!
			 *	@brief		Reports the transform's scale value.
			 *
			 *	@returns	The orientation of this matrix.
			 */
			const Vector3 & scale() const { return _xform._scale; }

			/*!
			 *	@brief		Sets the transformation to be dirty.
			 *
			 *	The transform node tries to perform lazy math.  Only re-computing
			 *	matrices due to changes and required computation.  Setting it to dirty
			 *	will force matrix computation next time the matrix is needed.
			 */
			void setDirty() { _xform.setDirty(); }

			/*!
			 *	@brief		Returns the transform matrix created by this transform node.
			 *
			 *	@param		mat		This matrix contains the matrix when done.
			 */
			inline void getMatrix( Matrix4x4 & mat ) { _xform.getMatrix( mat ); }
			
			/*!
			 *	@brief		Writes the transform's inverse matrix created by this transform node.
			 *
			 *	@param		mat		This matrix contains the inverse matrix when done.
			 */
			inline void getInverseMatrix( Matrix4x4 & mat ) { _xform.getInverseMatrix( mat ); }
			
			/*!
			 *	@brief		Returns the world matrix of this node; the matrix that transforms points
			 *				from this node's object space to world space.
			 *
			 *	@param		mat		This matrix contains the world matrix when done.
			 */
			void getWorldMatrix( Matrix4x4 & mat );
			
			/*!
			 *	@brief		Returns the world inverse matrix of this node; the matrix that transforms points
			 *				from world space to this node's object space.
			 *
			 *	@param		mat		This matrix contains the world inverse matrix when done.
			 */
			void getWorldInverseMatrix( Matrix4x4 & mat );

			/*!
			 *	@brief		Returns this node's parent's world matrix.
			 *
			 *	@param		mat		This matrix contains the parent matrix when done.
			 */
			void getParentMatrix( Matrix4x4 & mat );

			/*!
			 *	@brief		Returns this node's parent's world inverse matrix.
			 *
			 *	@param		mat		This matrix contains the parent's inverse matrix when done.
			 */
			void getParentInverseMatrix( Matrix4x4 & mat );

			/*!
			 *	@brief		Causes this node's child nodes to draw themselves to the scene, transformed
			 *				by this node's transformation matrix.
			 *
			 *	@param		select		Determines if the draw call is being performed
			 *							for the purpose of selection (true) or for visualization
			 *							(false).
			 */
			virtual void drawGL( bool select=false );

		protected:
			/*!
			 *	@brief		The underlyilng transformation matrix associated with this transform node.
			 */
			XformMatrix	_xform;
		};
	}	// namespace SceneGraph
}	// namespace Menge
#endif	// __TRANSFORM_H__
