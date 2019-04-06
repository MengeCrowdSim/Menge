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

#include "MengeVis/Viewer/ViewConfig.h"
#include "MengeCore/Runtime/os.h"
#include "MengeVis/SceneGraph/GLLight.h"
#include "MengeVis/Viewer/Watermark.h"

#include "tinyxml/tinyxml.h"

#include <fstream>
#include <iostream>
#include <sstream>

using Menge::Logger;

namespace MengeVis {

using Menge::logger;
using SceneGraph::GLLight;

namespace Viewer {

////////////////////////////////////////////////////////////////////////////
//			Implementation of ViewConfig helpers
////////////////////////////////////////////////////////////////////////////

/*!
 *	@brief		Streaming output operator to display camera specification.
 *
 *	@param		out			The output stream to which to write the view
 *configuration.
 *	@param		camParam	The camera parameters to convert to a string.
 *	@returns	The output stream.
 */
Logger& operator<<(Logger& out, const CameraParam& camParam) {
  out << "Camera - Pos: ( " << camParam._posX << ", " << camParam._posY << ", ";
  out << camParam._posZ << " )";
  out << ", Tgt: ( " << camParam._tgtX << ", " << camParam._tgtY << ", ";
  out << camParam._tgtZ << " )";
  out << ", Planes: [ " << camParam._nearPlane << ", " << camParam._farPlane << " ]";
  out << ", fov: " << camParam._fov;
  return out;
}

////////////////////////////////////////////////////////////////////////////

/*!
 *	@brief		Streaming output operator to display light specification.
 *
 *	@param		out				The output stream to which to write the view
 *configuration.
 *	@param		lightParam		The light parameters to convert to a string.
 *	@returns	The output stream.
 */
Logger& operator<<(Logger& out, const LightParam& lightParam) {
  out << "Light - Pos: ( " << lightParam._x << ", " << lightParam._y << ", ";
  out << lightParam._z << " )";
  out << ", Color: ( " << lightParam._r << ", " << lightParam._g << ", ";
  out << lightParam._b << " )";
  return out;
}

////////////////////////////////////////////////////////////////////////////
//			Implementation of ViewConfig
////////////////////////////////////////////////////////////////////////////

ViewConfig::ViewConfig() : _viewFldr("."), _waterMark(0x0) { setDefaults(); }

////////////////////////////////////////////////////////////////////////////

ViewConfig::~ViewConfig() {
  if (_waterMark) delete _waterMark;
}

////////////////////////////////////////////////////////////////////////////

bool ViewConfig::readXML(const std::string& fileName) {
  TiXmlDocument xml(fileName);
  bool loadOkay = xml.LoadFile();

  if (!loadOkay) {  // load xml file
    logger << Logger::ERR_MSG << "Could not load view configuration xml (";
    logger << fileName << ") due to xml syntax errors.\n";
    logger << "\t" << xml.ErrorDesc();
    return false;
  }

  TiXmlElement* rootNode = xml.RootElement();
  if (!rootNode) {
    logger << Logger::ERR_MSG << "View configuration (" << fileName;
    logger << ") does not contain a root element.";
    return false;
  }

  if (rootNode->ValueStr() != "View") {
    logger << Logger::ERR_MSG << "View configuration (" << fileName;
    logger << ")'s root element is not \"View\".";
    return false;
  }

  std::string absPath;
  Menge::os::path::absPath(fileName, absPath);
  std::string junk;
  Menge::os::path::split(absPath, _viewFldr, junk);
  logger.line();
  logger << Logger::INFO_MSG << "View root: " << _viewFldr << "\n";

  bool valid = true;

  // View parameters
  double d;
  int i;

  _z_up = false;
  if (rootNode->Attribute("z_up", &i)) {
    _z_up = i != 0;
    ;
  }
  if (!_z_up) {
    logger << Logger::WARN_MSG << "\tView parameters expressed in left-handed, y-up frame.";
  }

  if (!rootNode->Attribute("width", &i)) {
    logger << Logger::ERR_MSG << "\tView element on line " << rootNode->Row();
    logger << " must specify width parameter.";
    valid = false;
  } else {
    _width = i;
  }

  if (!rootNode->Attribute("height", &i)) {
    logger << Logger::ERR_MSG << "\tView element on line " << rootNode->Row();
    logger << " must specify height parameter.";
    valid = false;
  } else {
    _height = i;
  }

  const char* name = rootNode->Attribute("bgImg");
  if (name != 0x0) {
    std::string tmp = Menge::os::path::join(2, _viewFldr.c_str(), name);
    Menge::os::path::absPath(tmp, _bgImg);
  }

  bool parsed_font = false;
  _camSpecs.clear();
  _lightSpecs.clear();
  TiXmlElement* child;
  for (child = rootNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
    if (child->ValueStr() == "Camera") {
      CameraParam cam;
      // name
      const char* name = child->Attribute("name");
      if (name != nullptr) {
        cam._name = name;
      } else {
        cam._name = "Camera " + std::to_string(_camSpecs.size() + 1);
      }
      // position
      if (!child->Attribute("xpos", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"xpos\" parameter.";
        valid = false;
      } else {
        cam._posX = (float)d;
      }
      if (!child->Attribute("ypos", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"ypos\" parameter.";
        valid = false;
      } else {
        cam._posY = (float)d;
      }
      if (!child->Attribute("zpos", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"zpos\" parameter.";
        valid = false;
      } else {
        cam._posZ = (float)d;
      }
      // target position
      if (!child->Attribute("xtgt", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"xtgt\" parameter.";
        valid = false;
      } else {
        cam._tgtX = (float)d;
      }
      if (!child->Attribute("ytgt", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"ytgt\" parameter.";
        valid = false;
      } else {
        cam._tgtY = (float)d;
      }
      if (!child->Attribute("ztgt", &d)) {
        logger << Logger::ERR_MSG << "\tCamera element on line " << child->Row();
        logger << " must specify \"ztgt\" parameter.";
        valid = false;
      } else {
        cam._tgtZ = (float)d;
      }
      // clipping planes
      if (child->Attribute("near", &d)) {
        cam._nearPlane = (float)d;
      }
      if (child->Attribute("far", &d)) {
        cam._farPlane = (float)d;
      }
      // projection
      float fov = 45.f;
      if (child->Attribute("fov", &d)) {
        fov = (float)d;
      }
      if (fov == 0.f) {
        cam._projType = SceneGraph::GLCamera::ORTHO;
        cam._fov = fov;
      } else {
        cam._projType = SceneGraph::GLCamera::PERSP;
        cam._fov = fov;
      }
      // orthographic zoom
      if (child->Attribute("orthoScale", &d)) {
        cam._orthoScale = (float)d;
      }
      if (valid) {
        if (!_z_up) {
          cam.xformToZUp();
        }
        _camSpecs.push_back(cam);
      }

    } else if (child->ValueStr() == "Light") {
      LightParam light;
      // position
      if (!child->Attribute("x", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"x\" parameter.";
        valid = false;
      } else {
        light._x = (float)d;
      }
      if (!child->Attribute("y", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"y\" parameter.";
        valid = false;
      } else {
        light._y = (float)d;
      }
      if (!child->Attribute("z", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"z\" parameter.";
        valid = false;
      } else {
        light._z = (float)d;
      }
      // type
      const char* type = child->Attribute("type");
      if (type != 0x0) {
        std::string typeStr(type);
        if (typeStr == "point") {
          light._w = 1.f;
        } else if (typeStr == "directional") {
          light._w = 0.f;
        } else {
          logger << Logger::ERR_MSG << "\tLight element on line ";
          logger << child->Row();
          logger << " must specify \"type\" parameter as \"point\" ";
          logger << "or \"directional\".";
          valid = false;
        }
      } else {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"type\" parameter as \"point\" or ";
        logger << "\"directional\".";
        valid = false;
      }
      // space
      type = child->Attribute("space");
      if (type != 0x0) {
        std::string typeStr(type);
        if (typeStr == "world") {
          light._space = SceneGraph::GLLight::WORLD;
        } else if (typeStr == "camera") {
          light._space = SceneGraph::GLLight::CAMERA;
        } else {
          logger << Logger::ERR_MSG << "\tLight element on line ";
          logger << child->Row() << " must specify \"space\" parameter as ";
          logger << "\"world\" or \"camera\".";
          valid = false;
        }
      } else {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"space\" parameter as \"world\" or \"camera\".";
        valid = false;
      }
      // color
      if (!child->Attribute("diffR", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"diffR\" parameter.";
        valid = false;
      } else {
        light._r = (float)d;
      }
      if (!child->Attribute("diffG", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"diffG\" parameter.";
        valid = false;
      } else {
        light._g = (float)d;
      }
      if (!child->Attribute("diffB", &d)) {
        logger << Logger::ERR_MSG << "\tLight element on line " << child->Row();
        logger << " must specify \"diffB\" parameter.";
        valid = false;
      } else {
        light._b = (float)d;
      }
      if (child->Attribute("diffA", &d)) {
        light._a = (float)d;
      }
      if (valid) {
        if (!_z_up) {
          light.xformToZUp();
        }
        _lightSpecs.push_back(light);
      }
    } else if (child->ValueStr() == "Font") {
      parsed_font = true;
      // font name
      const char* name = child->Attribute("name");
      if (name != 0x0) {
        _fontName = name;
      }
      double d;

      if (child->Attribute("r", &d)) {
        _fontColor[0] = (float)d;
      }

      if (child->Attribute("g", &d)) {
        _fontColor[1] = (float)d;
      }

      if (child->Attribute("b", &d)) {
        _fontColor[2] = (float)d;
      }

      if (child->Attribute("a", &d)) {
        _fontColor[3] = (float)d;
      }
    } else if (child->ValueStr() == "Watermark") {
      _waterMark = parseWatermark(child, _viewFldr);
    }
  }
  if (valid && _camSpecs.size() == 0) {
    logger << Logger::WARN_MSG << "No cameras specified; using default!";
    _camSpecs.push_back(CameraParam());
  }
  if (valid && !_z_up) {
    logger << Logger::WARN_MSG << "Converted to z-up. "
           << "Replace your view configuration with the following xml:\n\n";
    logger << toXML(parsed_font);
  }
  return valid;
}

////////////////////////////////////////////////////////////////////////////

std::string ViewConfig::toXML(bool include_font) const {
  std::stringstream ss;
  ss << "<?xml version=\"1.0\"?>\n\n";
  ss << "<View width=\"" << _width << "\" height=\"" << _height << "\" z_up=\"1\"";
  if (!_bgImg.empty()) {
    ss << " bgImg=\"" << _bgImg << "\"";
  }
  ss << " >\n";
  for (const auto& cam : _camSpecs) {
    ss << "\t<Camera "
       << "xpos=\"" << cam._posX << "\" ypos=\"" << cam._posY << "\" zpos=\"" << cam._posZ
       << "\" xtgt=\"" << cam._tgtX << "\" ytgt=\"" << cam._tgtY << "\" ztgt=\"" << cam._tgtZ
       << "\" far=\"" << cam._farPlane << "\" near=\"" << cam._nearPlane << "\" fov=\"" << cam._fov
       << "\" orthoScale=\"" << cam._orthoScale << "\"/>\n";
  }
  for (const auto& light : _lightSpecs) {
    ss << "\t<Light "
       << "x=\"" << light._x << "\" y=\"" << light._y << "\" z=\"" << light._z << "\" "
       << "type=\"" << (light._w == 0.f ? "directional" : "point") << "\" "
       << "space=\"" << (light._space == GLLight::WORLD ? "world" : "camera") << "\" "
       << "diffR=\"" << light._r << "\" diffG=\"" << light._g << "\" diffB=\"" << light._b
       << "\"/>\n";
  }
  if (include_font) {
    ss << "\t<Font "
       << "name=\"" << _fontName << "\" r=\"" << _fontColor[0] << "\" g=\"" << _fontColor[1]
       << "\" b=\"" << _fontColor[2] << "\" a=\"" << _fontColor[3] << "\"/>\n";
  }
  if (_waterMark != nullptr) {
    ss << "\t<Watermark file_name=\"" << _waterMark->getFilename() << "\" "
       << "alignment=\"";
    switch (_waterMark->get_alignment()) {
      case NO_ALIGN:
        ss << "bad alignment";
        break;
      case CENTERED:
        ss << "centered";
        break;
      case BOTTOM_LEFT:
        ss << "bottom_left";
        break;
      case BOTTOM_RIGHT:
        ss << "bottom_right";
        break;
      case TOP_LEFT:
        ss << "top_left";
        break;
      case TOP_RIGHT:
        ss << "top_right";
        break;
    }
    ss << "\" scale=\"" << _waterMark->get_scale() << "\" "
       << "opacity=\"" << _waterMark->get_opacity() << "\"/>\n";
  }
  ss << "</View>\n";
  return ss.str();
}

////////////////////////////////////////////////////////////////////////////

void ViewConfig::setDefaults() {
  _width = 640;
  _height = 480;
  _camSpecs.clear();
  _camSpecs.push_back(CameraParam());
  _lightSpecs.clear();
  _bgImg = "";
  _fontName = "arial.ttf";
  _fontColor[0] = _fontColor[1] = _fontColor[2] = 1.f;
  _fontColor[3] = 0.75f;
}

////////////////////////////////////////////////////////////////////////////

void ViewConfig::setCamera(SceneGraph::GLCamera& camera, size_t i) const {
  // TODO: set multiple cameras
  const CameraParam& cfg = _camSpecs[i];
  camera.setPosition(cfg._posX, cfg._posY, cfg._posZ);
  camera.setTarget(cfg._tgtX, cfg._tgtY, cfg._tgtZ);
  camera.setFarPlane(cfg._farPlane);
  camera.setNearPlane(cfg._nearPlane);
  camera.set_name(cfg._name);
  if (cfg._projType == SceneGraph::GLCamera::ORTHO) {
    camera.setOrtho(cfg._orthoScale);
  } else {
    camera.setFOV(cfg._fov);
    camera.setPersp();
  }
}

////////////////////////////////////////////////////////////////////////////

void ViewConfig::setCameras(std::vector<SceneGraph::GLCamera>& cameras) const {
  cameras.clear();
  for (size_t i = 0; i < _camSpecs.size(); ++i) {
    SceneGraph::GLCamera cam;
    setCamera(cam, i);
    cameras.push_back(cam);
  }
}

////////////////////////////////////////////////////////////////////////////

void ViewConfig::setLight(SceneGraph::GLLight& light, size_t i) const {
  // TODO: set multiple cameras
  const LightParam& cfg = _lightSpecs[i];
  light.setColor(cfg._r, cfg._g, cfg._b, cfg._a);
  light.setPosition(cfg._x, cfg._y, cfg._z, cfg._w);
  light.setSpace(cfg._space);
}

////////////////////////////////////////////////////////////////////////////

void ViewConfig::setLights(std::vector<SceneGraph::GLLight>& lights) const {
  lights.clear();
  for (size_t i = 0; i < _lightSpecs.size(); ++i) {
    SceneGraph::GLLight light;
    setLight(light, i);
    lights.push_back(light);
  }
}

////////////////////////////////////////////////////////////////////////////

Logger& operator<<(Logger& out, const ViewConfig& cfg) {
  out << "View configuration:";
  out << "\n\twidth:              " << cfg._width;
  out << "\n\theight:             " << cfg._height;
  for (size_t i = 0; i < cfg._camSpecs.size(); ++i) {
    out << "\n\t" << i << " " << cfg._camSpecs[i];
  }
  for (size_t i = 0; i < cfg._lightSpecs.size(); ++i) {
    out << "\n\t" << i << " " << cfg._lightSpecs[i];
  }
  out << "\n\tBackground image:   " << cfg._bgImg;
  if (cfg._waterMark) {
    out << "\n\tWatermark image: " << cfg._waterMark->getFilename();
  }
  return out;
}

}  // namespace Viewer
}  // namespace MengeVis
