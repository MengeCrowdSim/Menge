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
 @file    Transform.h
 @brief   Scene graph node which carries rigid transformations.
 */
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "MengeVis/SceneGraph/GLNode.h"
#include "MengeVis/SceneGraph/XformMatrix.h"
#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {
/*!
 @brief   Scene graph node which applies transforms to nodes.

 The Transform uses the XformMatrix to compute the transformation math. See XformMatrix for the
 details of the math.
 */
class MENGEVIS_API Transform : public GLDagNode {
 public:
  /*!
   @brief   Constructor.

   @param   parent    A pointer to the optional parent node in the graph.
   */
  Transform(GLDagNode* parent = 0x0);

  /*!
   @brief   Set the translation of this node.

   @param   vec   The 3D position of the transform.
   */
  inline void setTranslation(const Menge::Math::Vector3& vec) { _xform.setTranslation(vec); }

  /*!
   @brief   Offset the translation of this node.

   @param   vec   The change to position of this node.
   */
  inline void addTranslation(const Menge::Math::Vector3& vec) { _xform.addTranslation(vec); }

  /*!
   @brief   Set the scale of this node.

   @param   vec   The desired scale of this node.
   */
  inline void setScale(const Menge::Math::Vector3& vec) { _xform.setScale(vec); }

  /*!
   @brief   Set the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in degrees.
   */
  inline void setRotationDeg(const Menge::Math::Vector3& vec) { _xform.setRotationDeg(vec); }

  /*!
   @brief   Set the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in radians.
   */
  inline void setRotationRad(const Menge::Math::Vector3& vec) { _xform.setRotationRad(vec); }

  /*!
   @brief   Offsets the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation. In this case, each axis is summed independently. Generally, because
   these are Euler angles, this will will not lead to linear interpolation of orientation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in degrees.
   */
  inline void addRotationDeg(const Menge::Math::Vector3& vec) { _xform.addRotationDeg(vec); }

  /*!
   @brief   Offsets the orientation of this node.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation. In this case, each axis is summed independently. Generally, because
   these are Euler angles, this will will not lead to linear interpolation of orientation.

   @param   vec   The rotations around the x-, y-, and z-axes, respectively in radians.
   */
  inline void addRotationRad(const Menge::Math::Vector3& vec) { _xform.addRotationRad(vec); }

  /*!
   @brief   Set the orientation of this node's rotation axis.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The orientation of the rotation axis around the x-, y-, and z-axes, respectively
                  in degrees.
   */
  inline void setRotAxisDeg(const Menge::Math::Vector3& vec) { _xform.setRotAxisDeg(vec); }

  /*!
   @brief   Set the orientation of this node's rotation axis.

   Rotation is performed using Euler angles and the implict evaluation order of x-, then y-, and
   finally z-axis rotation.

   @param   vec   The orientation of the rotation axis around the x-, y-, and z-axes, respectively
   in radians.
   */
  inline void setRotAxisRad(const Menge::Math::Vector3& vec) { _xform.setRotAxisRad(vec); }

  /*!
   @brief   Reports the transform's translation value.

   @returns The translation of this matrix.
   */
  const Menge::Math::Vector3& translation() const { return _xform._trans; }

  /*!
   @brief   Reports the transform's orientation value.

   @returns The orientation of this matrix.
   */
  const Menge::Math::Vector3& rotation() const { return _xform._rot; }

  /*!
   @brief   Reports the transform's scale value.

   @returns The orientation of this matrix.
   */
  const Menge::Math::Vector3& scale() const { return _xform._scale; }

  /*!
   @brief   Sets the transformation to be dirty.

   The transform node tries to perform lazy math. Only re-computing matrices due to changes and
   required computation. Setting it to dirty will force matrix computation next time the matrix is 
   needed.
   */
  void setDirty() { _xform.setDirty(); }

  /*!
   @brief   Returns the transform matrix created by this transform node.

   @param   mat   This matrix contains the matrix when done.
   */
  inline void getMatrix(Menge::Math::Matrix4x4& mat) { _xform.getMatrix(mat); }

  /*!
   @brief   Writes the transform's inverse matrix created by this transform node.

   @param   mat   This matrix contains the inverse matrix when done.
   */
  inline void getInverseMatrix(Menge::Math::Matrix4x4& mat) { _xform.getInverseMatrix(mat); }

  /*!
   @brief   Returns the world matrix of this node; the matrix that transforms points from this
            node's object space to world space.

   @param   mat   This matrix contains the world matrix when done.
   */
  void getWorldMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Returns the world inverse matrix of this node; the matrix that transforms points from
            world space to this node's object space.

   @param   mat   This matrix contains the world inverse matrix when done.
   */
  void getWorldInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Returns this node's parent's world matrix.

   @param   mat   This matrix contains the parent matrix when done.
   */
  void getParentMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Returns this node's parent's world inverse matrix.

   @param   mat   This matrix contains the parent's inverse matrix when done.
   */
  void getParentInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Causes this node's child nodes to draw themselves to the scene, transformed by this
            node's transformation matrix.

   @param   select    Determines if the draw call is being performed for the purpose of selection
                      (true) or for visualization (false).
   */
  virtual void drawGL(bool select = false);

 protected:
  /*!
   @brief   The underlyilng transformation matrix associated with this transform node.
   */
  XformMatrix _xform;
};
}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __TRANSFORM_H__
