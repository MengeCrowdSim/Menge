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
 @file    GLCamera.h
 @brief   Class to handle camera transformations in OpenGL
 */
#ifndef __GLCAMERA_H__
#define __GLCAMERA_H__

#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {

/*!
 @brief   The class for controlling the view camera: it's position, orientation field of view,
          projection matrix, etc.
 */
class MENGEVIS_API GLCamera {
 public:
  /*!
   @brief   Determines the projection matrix of the 3D camera
   */
  enum ProjectionType { ORTHO, PERSP };

  /*!
   @brief   Default constructor.
   */
  GLCamera();

  /*!
   @brief   Constructor for setting camera position.

   @param   p   The position of the camera.
   */
  GLCamera(const Menge::Math::Vector3& p);

  /*!
   @brief   Constructor for setting camera position and target.

   @param   p   The position of the camera.
   @param   t   The position of the camera's view target.
   */
  GLCamera(const Menge::Math::Vector3& p, const Menge::Math::Vector3& t);

  /*!
   @brief   Constructor for setting camera position, target, and up vector.

   @param   p   The position of the camera.
   @param   t   The position of the camera's view target.
   @param   u   The direction of the camera's up direction.
   */
  GLCamera(const Menge::Math::Vector3& p, const Menge::Math::Vector3& t,
           const Menge::Math::Vector3& u);

  /*!
   @brief   Sets the camera to use an orthographic projection.

   @param   scaleFactor   Defines the horizontal span of the orthographic view as a function of the
                          distance to target. This "zooms" the orthographic camera closer.
   */
  void setOrtho(float scaleFactor = 1.f);

  /*!
   @brief   Sets the camera to use a perspective projection.
   */
  void setPersp();

  /*!
   @brief   Reports the projection mode of the camera.

   @returns The enumeration indicating the projection type of the camera.
   */
  ProjectionType getProjection() const { return _type; }

  /*!
   @brief   Reports the name of this camera.

   @returns The camera name.
   */
  const std::string& get_name() const { return _name; }

  /*!
   @brief   Sets the camera's name.

   @param   name    The camera's new name.
   */
  void set_name(const std::string& name) { _name = name; }

  /*!
   @brief   Called the first time a camera is put into use.

   Failing to call this may lead to unexpected camera settings until the camera is manipulated.
   */
  void setActive();

  /*!
   @brief   Return the position of the camera.

   @returns  The position fo the camera in 3D space.
   */
  Menge::Math::Vector3 getPosition() const { return _position; }

  /*!
   @brief   Set the camera position explicitly from a Vector3.

   @param   p   The desired position of the camera.
   */
  void setPosition(const Menge::Math::Vector3& p);

  /*!
   @brief   Set the camera position explicitly from three values.

   @param   x   The position of the camera along the x-axis.
   @param   y   The position of the camera along the y-axis.
   @param   z   The position of the camera along the z-axis.
   */
  void setPosition(float x, float y, float z);

  /*!
   @brief   Return the position of the camera's view target.

   @returns The position of the camera's view target.
   */
  Menge::Math::Vector3 getTarget() const { return _target; }

  /*!
   @brief   Set the position of the camera's view target explicitly from a Vector3.

   @param   p   The desired position of the camera's view target.
   */
  void setTarget(const Menge::Math::Vector3& p);

  /*!
   @brief   Set the position of the camera's view target explicitly from three values.

   @param   x   The position of the camera's view target along the x-axis.
   @param   y   The position of the camera's view target along the y-axis.
   @param   z   The position of the camera's view target along the z-axis.
   */
  void setTarget(float x, float y, float z);

  /*!
   @brief   Return the orientation of the camera's up direction.

   @returns The orientation of the camera's up direction.
   */
  Menge::Math::Vector3 getUp() const { return _up; }

  /*!
   @brief   Set the orientation of the camera's up vector from a Vector3.

   @param   u   The desired direction of the camera's up direction. It is assumed to be of normal
                length.
   */
  void setUp(const Menge::Math::Vector3& u);

  /*!
   @brief   Set the orientation of the camera's up vector from three values.

   @param   x   The x-component of the camera's up vector.
   @param   y   The y-component of the camera's up vector.
   @param   z   The z-component of the camera's up vector.
   */
  void setUp(float x, float y, float z);

  /*!
   @brief   Sets the field of view of the camera -- only applicable when using the perspective
            projection.

   @param   fov   The horizontal field of view (in degrees).
   */
  inline void setFOV(float fov) { _fov = fov; }

  /*!
   @brief   Returns the current horizontal field of view (in degrees).
   */
  inline float getFOV() const { return _fov; }

  /*!
   @brief   Sets the far plane distance.

   @param   fp    The desired far plane distance.
   */
  inline void setFarPlane(float fp) { _farPlane = fp; }

  /*!
   @brief   Reports the camera's current far plane distance.

   @returns The camera's current far plane distance.
   */
  inline float getFarPlane() const { return _farPlane; }

  /*!
   @brief   Sets the near plane distance.

   @param   np    The desired near plane distance.
   */
  inline void setNearPlane(float np) { _nearPlane = np; }

  /*!
   @brief   Reports the camera's current near plane distance.

   @returns The camera's current near plane distance.
   */
  inline float getNearPlane() const { return _nearPlane; }

  /*!
   @brief   Reports the orthographic scale factor for this camera.

   The orthographic scale factor is the value which determines how "zoomed in" an orthographic
   camera is. It is the ratio of the between camera and target and the width of the viewable region
   in orthographic.

   @returns The orthographic scale
   */
  float getOrthoScaleFactor() const;

  /*!
   @brief   Sets the camera's viewport to a view with the given width and height dimensions.

   @param   w         The width of the viewport (in pixels).
   @param   h         The height of the viewport (in pixels).
   @param   left      The position of the left-most edge of the viewport in pixels.  Defaults to zero.
   @param   bottom    The position of the bottom-most edge of the viewport in pixels. Defaults to
                      zero.
   */
  void setViewport(int w, int h, int left = 0, int bottom = 0);

  /// Create OpenGL commands

  /*!
   @brief   Emits the appropriate OpenGL commands for setting up a projection matrix.

   @param   w   The viewport width (in pixels).
   @param   h   The viewport height (in pixels).
   */
  void setProjMat(int w, int h) const;

  /*!
   @brief   Emits the appropriate OpenGL commands for setting up a selection context.

   @param   selectPoint   A pointer to an array of two floats interpreted as the x- and y-positions
                          of the selection point, in screen space.
   */
  void setSelectMat(int* selectPoint) const;

  /*!
   @brief   Emits the appropriate OpenGL commands for setting up the view for this camera.
   */
  void setGLView() const;

  /*!
   @brief   Sets the camera to look along the world's x-axis.

   The camera's distance from the origin is the same as its previous displacement from the origin
   along the x-axis.

   @param   downPositive    If true, the camera looks along the positive x-axis direction. If false,
                            it looks in the negative direction.
   */
  void viewXAxis(bool downPositive = true);

  /*!
   @brief   Sets the camera to look along the world's y-axis.

   The camera's distance from the origin is the same as its previous displacement from the origin
   along the y-axis.

   @param   downPositive    If true, the camera looks along the positive y-axis direction. If false,
                            it looks in the negative direction.
   */
  void viewYAxis(bool downPositive = true);

  /*!
   @brief   Sets the camera to look along the world's z-axis.

   The camera's distance from the origin is the same as its previous displacement from the origin
   along the z-axis.

   @param   downPositive    If true, the camera looks along the positive z-axis direction. If false,
                            it looks in the negative direction.
   */
  void viewZAxis(bool downPositive = true);

  /*!
   @brief   Causes the camera to "pan" the given angle.

   The camera "pans" by rotating around its own center along the world's vertical axis (i.e.,
   turning left and right).

   @param   angle   The amount of pan (in radians).  Positive values cause the camera to turn left,
                    negative values turn the camera right.
   */
  void pan(float angle);

  /*!
   @brief   Causes the camera to "tilt" the given angle.

   The camera "tilts" by rotating around its own center along the its own horizontal axis (i.e.,
   looking up and down).

   @param   angle   The amount of pan (in radians). Postive values rotates the camera down, negative
                    rotates up.
   */
  void tilt(float angle);

  /*!
   @brief   Causes the camera to "truck" the given amount.

   "Trucking" is moving the camera side to side on its own horizontal axis (i.e., "strafe" left and
   right).

   @param   amount    The amount to move. Postive values move the camera to its right and negative
   values move left.
   */
  void truck(float amount);

  /*!
   @brief   Causes the camera to "crane" the given amount.

   A camera "cranes" by moving along its own vertical axis. This causes the camera to rise and lower
   in its view. The actual movement in the WORLD coordinate system depends on the camera's
   orientation.

   @param   amount    The amount to move. Postive values move the camera up and negative values move
                      down.
   */
  void crane(float amount);

  /*!
   @brief   Causes the camera to "dolly" in and out.

   Dollying a camera moves it forward and backward in the camera's facing direction (i.e., moving
   in the direction of the target).  The target moves with the camera.

   @param   amount    The amount to move. Positive values move forward and negative values move
                      backward.
   */
  void dolly(float amount);

  /*!
   @brief   Causes the camera to "dolly" along a plane.

   In this case, the elevation (in world space) does not change. The camera moves parallel with the
   ground plane based on the projeciton of the camera's facing direction on that plane.

   @param   amount    The amount to move. Positive values move forward and negative values move
                      backward.
   */
  void dollyPlane(float amount);

  /*!
   @brief   Causes the camera to "zoom" toward its target.

   This is not a zoom in the sense that the lens focal length changes. This merely moves the camera
   along its facing direction while holding the target stationary.

   @param   amount    The amount to zoom. Positive values move forward and negative values move
                      backward.
   */
  void zoom(float amount);

  /*!
   @brief   Causes the camera to rotate an axis, parallel with the world vertical axis centered on
            the camera's target.

   @param   angle   The angle of rotation (in radians). Positive values move the camera to its
                    right, negative to its left.
   */
  void orbitVerticalAxis(float angle);

  /*!
   @brief   Camera rotates, at distance, around target on axis parallel to camera's horizontal axis.

   @param   angle   The angle of rotation (in radians). Positive values move the camera in its
                    upward direction, negative to its downward.
   */
  void orbitHorizontalAxis(float angle);

  /// Camera state

  /*!
   @brief   Returns distance between camera and target

   @returns The distance between camera and its target.
   */
  float targetDistance() const;

  /*!
   @brief   Returns the unit-length vector indicating direction FROM camera to TARGET.

   @returns Direction to target in world space.
   */
  Menge::Math::Vector3 targetDir() const;

  /*!
   @brief   Returns the ACTUAL up dir -- as opposed to the desired up.

   Essentially, the cross product of the camera's horizontal direction with its facing direction.

   @returns Camera's current, local up vector in world space.
   */
  Menge::Math::Vector3 getActualUpDir() const;

  /*!
   @brief   Returns the normalized vector representing the camera's horizontal axis pointing to
            right.

   @returns The direction of the camera's right in world space.
   */
  Menge::Math::Vector3 getRightDir() const;

  /*!
   @brief   Outputs the state of the camera to the console.

   The format can be cut-and-pasted into a view configuration file.
   */
  void outputState() const;

 private:
  /*!
   @brief   Determines the camera projeciton type: orthographic or perspective.
   */
  ProjectionType _type;

  /*!
   @brief   The name of the camera.
   */
  std::string _name;

  /*!
   @brief   Position of camera -- used for both perspective and orthographic.
   */
  Menge::Math::Vector3 _position;

  /*!
   @brief   Position of the camera's target -- used primarily for perspective.
   */
  Menge::Math::Vector3 _target;

  /*!
   @brief   The desired up direction for the camera. 
   
   Only relevant for the perspective camera.
   */
  Menge::Math::Vector3 _up;

  /*!
   @brief   The distance from the camera to the camera's far clipping plane.
   */
  float _farPlane;

  /*!
   @brief   The distance from the camera to the camera's near clipping plane.
   */
  float _nearPlane;

  /*!
   @brief   The horizontal angle of field of view.
   */
  float _fov;

  /*!
   @brief   The viewport's width.
   
   (It is assumed that the viewport's origin is at (0,0).  Mutable because it changes during screen
   resizes.
   */
  mutable int _viewWidth;

  /*!
   @brief   The viewport's height.
   
   (It is assumed that the viewport's origin is at (0,0).  Mutable because it changes during screen
   resizes.
   */
  mutable int _viewHeight;

  /*!
   @brief   Deterimines if the projection matrix needs to be recomputed.
   */
  mutable bool _dirtyProj;

  /*!
   @brief   The width of the orthographic display.  
   
   Combined with the scale factor determines the scope of the orthographic view.

   @see  setOrtho().
   */
  float _orthoHWidth;

  /*!
   @brief   Common camera projection functionality shared by both perspective and orthographic
            projections.
   */
  void _setProjMat() const;
};

}  // namespace SceneGraph
}  // namespace MengeVis

#endif  // __GLCAMERA_H__
