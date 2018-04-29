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
 @file    ViewConfig.h
 @brief   Specification for parsing the view configuration file.
 */

#ifndef __VIEW_CONFIG_H__
#define __VIEW_CONFIG_H__

#include <algorithm>
#include <string>
#include <vector>
#include "MengeCore/Runtime/Logger.h"
#include "MengeVis/SceneGraph/GLCamera.h"
#include "MengeVis/SceneGraph/GLLight.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace Viewer {

// forward declarations
class Watermark;

/*!
 @brief   A set of parameters specifying a single camera.
 */
class CameraParam {
 public:
  /*!
   @brief   Default constructor.
   */
  CameraParam() {
    _posX = _posY = _tgtX = _tgtY = _tgtZ = 0.0f;
    _posZ = 10.f;
    _farPlane = 200.f;
    _nearPlane = 0.01f;
    _orthoScale = 1.f;
    _fov = 0.f;
    _projType = SceneGraph::GLCamera::ORTHO;
  }

  /*!
   @brief Assuming the camera is configured in a left-handed, y-up frame, transforms the camera into
          a right-handed, z-up frame.
   */
  void xformToZUp() {
    // Swap z- and y-axes.
    std::swap(_posY, _posZ);
    std::swap(_tgtY, _tgtZ);
    _tgtY = -_tgtY;
    _posY = -_posY;
  }

  /*!
   @brief   The name of the camera.
   */
  std::string _name;

  /*!
   @brief   Camera x-position in world space.
   */
  float _posX;

  /*!
   @brief   Camera y-position in world space.
   */
  float _posY;

  /*!
   @brief   Camera z-position in world space.
   */
  float _posZ;

  /*!
   @brief   Camera's target x-position in world space.
   */
  float _tgtX;

  /*!
   @brief   Camera's target y-position in world space.
   */
  float _tgtY;

  /*!
   @brief   Camera's target z-position in world space.
   */
  float _tgtZ;

  /*!
   @brief   Distance to camera's far plane.
   */
  float _farPlane;

  /*!
   @brief   Distance to camera's near plane.
   */
  float _nearPlane;

  /*!
   @brief   The "scale" factor applid to the camera in orthographic view.
   */
  float _orthoScale;

  /*!
   @brief   The camera's horizontal field of view (in degrees).
   */
  float _fov;

  /*!
   @brief   The camera's projection type (perspective or orthographic).

   @see SceneGraph::GLCamera.
   */
  SceneGraph::GLCamera::ProjectionType _projType;
};

////////////////////////////////////////////////////////////////////////////

/*!
 @brief   A set of parameters specifying a single light
 */
class LightParam {
 public:
  /*!
   @brief   Default constructor.
   */
  LightParam() {
    _r = _g = _b = 1.f;
    _x = _y = _z = 1.f;
    _w = 0.f;  // 0 --> directional, 1 --> point
  }

  /*!
   @brief   Assuming the light is configured in a left-handed, y-up frame, transforms it into a 
            right-handed, z-up frame.
   */
  void xformToZUp() {
    // Swap z- and y-axes.
    // TODO: Determine if I have to account for point lights differently from directional lights.
    if (_space == SceneGraph::GLLight::WORLD) std::swap(_y, _z);
  }

  /*!
   @brief   The red channel of the light's diffuse color.
   */
  float _r;

  /*!
   @brief   The green channel of the light's diffuse color.
   */
  float _g;

  /*!
   @brief   The blue channel of the light's diffuse color.
   */
  float _b;

  /*!
   @brief   The alpha channel of the lights' diffuse color.
   */
  float _a;

  /*!
   @brief   The x-value of the light position
   */
  float _x;

  /*!
   @brief   The y-value of the light position
   */
  float _y;

  /*!
   @brief   The z-value of the light position
   */
  float _z;

  /*!
   @brief   The w-value of the light position.
   
   Determines if the light is a point or directional light.
   @todo Document which values map to point, and which to directional.
   */
  float _w;

  /*!
   @brief   The space in which the light lives.
   */
  SceneGraph::GLLight::LightSpace _space;
};

////////////////////////////////////////////////////////////////////////////

/*!
 @brief   The specification of an OpenGL GLViewer for a scene

 @see GLViewer
 */
class MENGEVIS_API ViewConfig {
 public:
  /*!
   @brief   Default constructor.
   */
  ViewConfig();

  /*!
   @brief   Destructor.
   */
  ~ViewConfig();

  /*!
   @brief   Parses the XML configuration file.

   @param   fileName    The name of the view configuration file to parse.
   @returns A boolean reporting success (true) or failure (false).
   */
  bool readXML(const std::string& fileName);

  /*!
   @brief   Writes the current configuration to an XML string.
   */
  std::string toXML(bool include_font) const;

  /*!
   @brief   Sets the view configuration to a set of default values.
   */
  void setDefaults();

  /*!
   @brief   Set the camera properties based on the configuration

   @param   camera    The camera to set.
   @param   i         The index of the camera.
   */
  void setCamera(SceneGraph::GLCamera& camera, size_t i = 0) const;

  /*!
   @brief   Sets the vector of cameras based on the camera specifications

   @param   cameras   A vector to populate with cameras. Any pre-existing cameras will be deleted.
   */
  void setCameras(std::vector<SceneGraph::GLCamera>& cameras) const;

  /*!
   @brief   Set the light properties based on the configuration

   @param   light   The light to set.
   @param   i       The index of the light specification to apply.
   */
  void setLight(SceneGraph::GLLight& light, size_t i = 0) const;

  /*!
   @brief   Sets the vector of lights based on the light specifications

   @param   lights    A vector to populate with lights. Any pre-existing lights will be deleted.
   */
  void setLights(std::vector<SceneGraph::GLLight>& lights) const;

  /*!
   @brief   If true, indicates that the view specification is in a z-up specification.
   */
  bool _z_up;

  /*!
   @brief   The folder the view configuration file is located in.
   */
  std::string _viewFldr;

  /*!
   @brief   Width of viewport (in pixels).
   */
  int _width;

  /*!
   @brief   Height of viewport (in pixels).
   */
  int _height;

  /*!
   @brief   The name of the background image to use.
   */
  std::string _bgImg;

  /*!
   @brief   The optional watermark.
   */
  Watermark* _waterMark;

  /*!
   @brief   Font name.
   */
  std::string _fontName;

  /*!
   @brief   Default font color.
   */
  float _fontColor[4];

  /*!
   @brief   The set of cameras for the configuration
   */
  std::vector<CameraParam> _camSpecs;

  /*!
   @brief   The set of cameras for the configuration
   */
  std::vector<LightParam> _lightSpecs;
};

/*!
 @brief   Streaming output operator to display configuration specification.

 @param   out   The output stream to which to write the view configuration.
 @param   cfg   The configuration to convert to a string.
 @returns The output stream.
 */
MENGEVIS_API Menge::Logger& operator<<(Menge::Logger& out, const ViewConfig& cfg);

}  // namespace Viewer

}  // namespace MengeVis

#endif  // __VIEW_CONFIG_H__
