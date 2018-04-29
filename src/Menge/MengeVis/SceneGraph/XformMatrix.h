/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file    XformMatrix.h
 @brief   Defines the math of performing 3D transformation using a 4x4 homgeneous matrix.
 */

#ifndef __XFORMMATRIX_H__
#define __XFORMMATRIX_H__

#include "MengeVis/VisConfig.h"
// TODO: Include Logger when MengeCore satisfies the same include rules as vis
//#include "MengeCore/Runtime/Logger.h"
#include <iostream>
#include "MengeVis/SceneGraph/graphCommon.h"

namespace MengeVis {

namespace SceneGraph {

class Transform;

/*!
 @brief   The transformation matrix.

 The transform matrix is the concatenation of *five* transformation matrices:

     SCALE * ROT_AXIS^-1 * ROTATE * ROT_AXIS * TRANSLATE

 The scale, rotation, and translation matrices are self-explanatory. They account for the changes in
 size, rotation around the orign, and re-positioning of the node. The transform also defines a
 "rotation axis". The rotation values can be defined around an arbitrary axis, rather than the
 node's local space.  One can think of the rotation axis as a pre-rotation.
 */
class MENGEVIS_API XformMatrix {
 public:
  /*!
   @brief   Default constructor - identity matrix.
   */
  XformMatrix();

  /*!
   @brief   Set the translation of this node.

   @param   vec   The 3D position of the transform.
   */
  void setTranslation(const Menge::Math::Vector3& vec) {
    _trans = vec;
    setDirty(MAT | IMAT);
  }

  /*!
   @brief   Offset the translation of this node.

   @param   vec   The change to position of this node.
   */
  void addTranslation(const Menge::Math::Vector3& vec) {
    _trans += vec;
    setDirty(MAT | IMAT);
  }

  /*!
   @brief   Set the scale of this node.

   @param   vec   The desired scale of this node.
   */
  void setScale(const Menge::Math::Vector3& vec) {
    _scale = vec;
    setDirty(MAT | IMAT);
  }

  /*!
   @brief   Set the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in degrees.
   */
  void setRotationDeg(const Menge::Math::Vector3& vec) {
    _rot = vec * Menge::DEG_TO_RAD;
    setDirty();
  }

  /*!
   @brief   Set the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in radians.
   */
  void setRotationRad(const Menge::Math::Vector3& vec) {
    _rot = vec;
    setDirty();
  }

  /*!
   @brief   Offsets the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation. In this case, each axis is summed independently. Generally, because
   these are Euler angles, this will will not lead to linear interpolation of orientation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in degrees.
   */
  void addRotationDeg(const Menge::Math::Vector3& vec) {
    _rot.SumScale(Menge::DEG_TO_RAD, vec);
    setDirty();
  }

  /*!
   @brief   Offsets the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation. In this case, each axis is summed independently. Generally, because
   these are Euler angles, this will will not lead to linear interpolation of orientation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in radians.
   */
  void addRotationRad(const Menge::Math::Vector3& vec) {
    _rot += vec;
    setDirty();
  }

  /*!
   @brief   Set the orientation of this node's rotation axis.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The orientation of the rotation axis around the x-, y-, and z-axes, respectively
                  in degrees.
   */
  void setRotAxisDeg(const Menge::Math::Vector3& vec);

  /*!
   @brief   Set the orientation of this node's rotation axis.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The orientation of the rotation axis around the x-, y-, and z-axes, respectively
                  in radians.
   */
  void setRotAxisRad(const Menge::Math::Vector3& vec);

  /*!
   @brief   Comptues the rotation p.
   */
  // TODO(curds01): Figure out why this is commented out.
  // void rotatePivotMatrix( Matrix4x4 & mat );

  /*!
   @brief   Computes the translation matrix.

   @param   mat   Writes the translation matrix in the provided matrix.
   */
  void translationMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the translation inverse matrix.

   @param   mat   Writes the inverse translation matrix in the provided matrix.
   */
  void translationInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the scale matrix.

   @param   mat   Writes the scale matrix in the provided matrix.
   */
  void scaleMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the inverse scale matrix.

   @param   mat   Writes the inverse scale matrix in the provided matrix.
   */
  void scaleInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the rotation matrix.

   @param   mat   Writes the rotation matrix in the provided matrix.
   */
  void rotationMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the inverse rotation matrix.

   @param   mat   Writes the inverse rotation matrix in the provided matrix.
   */
  void rotationInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the transformation matrix.

   @param   mat   Writes the matrix in the provided matrix.
   */
  void getMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Computes the inverse transformation matrix.

   @param   mat   Writes the inverse matrix in the provided matrix.
   */
  void getInverseMatrix(Menge::Math::Matrix4x4& mat);

  friend class Transform;

  /*!
   @brief   Print the matrix to the output stream.

   @param   out         The output stream.
   @param   xformMat    The transformation matrix to print to the output stream.
   */
  // TODO(curds01): Figure out why this is commented out.
  // friend Menge::Logger & operator << ( Menge::Logger & out, const XformMatrix & xformMat );

 protected:
  /*!
   @brief   The translation component of the transformation.
   */
  Menge::Math::Vector3 _trans;

  /*!
   @brief   The scale component of the transformation.
   */
  Menge::Math::Vector3 _scale;

  /*!
   @brief   The rotation component of the transformation; Euler angles in radians.
   */
  Menge::Math::Vector3 _rot;

  /*!
   @brief   The rotation axis component of the transformation. Pre-rotation Euler angles in radians.
   */
  Menge::Math::Vector3 _rotAxis;

  /*!
   @brief   Cached rotation axis matrix.
   */
  Menge::Math::Matrix4x4 _rotAxisMat;

  /*!
   @brief   Cached inverse rotation axis matrix.
   */
  Menge::Math::Matrix4x4 _rotAxisIMat;

  /*!
   @brief   Enumeration of dirty matrices.

   Used to intelligently compute matrices as necessary.
   */
  enum MatrixBit {
    ROT_MAT = 1,  ///< rotation matrix
    MAT = 2,      ///< full matrix
    IMAT = 4      ///< inverse matrix
  };

  /*!
   @brief   An integer mask for determining which cached matrices are clean/dirty

   Works with the MatrixBit enumeration to determine which matrices need to be recomputed.
   */
  int _clean;

  /*!
   @brief   Cached rotation matrix.
   */
  Menge::Math::Matrix4x4 _rotMat;

  /*!
   @brief   Cached transformation matrix.
   */
  Menge::Math::Matrix4x4 _mat;

  /*!
   @brief   Cached inverse transformation matrix.
   */
  Menge::Math::Matrix4x4 _iMat;

  /*!
   @brief   Update cached rotation axis matrices when component changes.
   */
  void updateRotAxisMat();

  /*!
   @brief   Set all matrices dirty.
   */
  inline void setDirty() { _clean = 0; }

  /*!
   @brief   Set the matrix corresponding to the given bit dirty.

   @param   bit   The bit for the specific matrix to set dirty. Should be a MatrixBit enumeration
                  value.
   */
  inline void setDirty(int bit) { _clean &= ~bit; }

  /*!
   @brief   Set the matrix corresponding to the given bit dirty.

   @param   bit   The bit for the specific matrix to set dirty.
   */
  inline void setDirty(MatrixBit bit) { _clean &= (int)(~bit); }

  /*!
   @brief   Reports if the indicated matrix is clean.

   @param   bit   The bit for the specific matrix to set dirty.
   @returns True if the matrix does NOT need recomputation, false otherwise.
   */
  inline bool isClean(MatrixBit bit) { return (_clean & (int)bit) != 0x0; }

  /*!
   @brief   Sets the indicated matrix to be clean.

   @param   bit   The bit for the specific matrix to set clean.
   */
  inline void setClean(MatrixBit bit) { _clean |= (int)bit; }

  /*!
   @brief   Sets the indicated matrix to be clean.

   @param   bit   The bit for the specific matrix to set clean. Should be a MatrixBit enumeration 
                  value.
   */
  inline void setClean(int bit) { _clean |= bit; }
};
}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __XFORMMATRIX_H__
