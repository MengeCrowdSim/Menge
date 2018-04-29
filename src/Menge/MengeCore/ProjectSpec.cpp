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

#include "MengeCore/ProjectSpec.h"

#include "thirdParty/tinyxml.h"

#include "MengeCore/Runtime/SimulatorDB.h"
#include "MengeCore/Runtime/os.h"

namespace Menge {
////////////////////////////////////////////////////////////////
//      Implementation of ProjectSpec
////////////////////////////////////////////////////////////////

ProjectSpec::ProjectSpec()
    : _projPath("."),
      _sceneXML(""),
      _behaviorXML(""),
      _modelName(""),
      _scbFileName(""),
      _scbVersion("2.1"),
      _verbose(false),
      _viewConfig(""),
      _duration(400.f),
      _timeStep(-1.f),
      _seed(0),
      _imgDumpPath("."),
      _subSteps(0) {}

////////////////////////////////////////////////////////////////

ProjectSpec::~ProjectSpec() {}

////////////////////////////////////////////////////////////////

bool ProjectSpec::fullySpecified() const {
  bool valid = true;
  if (_behaviorXML == "") {
    std::cerr << "!!!  To run a simulation, a behavior specification must be provided.";
    valid = false;
  }

  if (_sceneXML == "") {
    std::cerr << "!!!  To run a simulation, a scene specification must be provided.";
    valid = false;
  }

  if (_modelName == "") {
    std::cerr << "!!!  To run a simulation, a named pedestrian model must be specified.\n";
    valid = false;
  }

  return valid;
}

////////////////////////////////////////////////////////////////

void ProjectSpec::setOutputName(const std::string& fileName) {
  os::path::absPath(fileName, _scbFileName);
  if (_scbFileName != "") {
    if (_scbFileName.substr(_scbFileName.size() - 4, 4) != ".scb") _scbFileName += ".scb";
  }
}

////////////////////////////////////////////////////////////////

bool ProjectSpec::loadFromXML(const std::string& xmlName) {
  logger << Logger::INFO_MSG << "Parsing project XML: " << xmlName;
  TiXmlDocument xml(xmlName);
  bool valid = xml.LoadFile();

  if (!valid) {  // load xml file
    std::cerr << "Could not load project specification xml " << xmlName << ".\n";
    return false;
  }

  TiXmlElement* rootNode = xml.RootElement();
  if (!rootNode) {
    std::cerr << "Root element does not exist\n.";
    return false;
  }

  if (rootNode->ValueStr() != "Project") {
    std::cerr << "Root element value is not 'Project'.\n";
    return false;
  }

  std::string absPath;
  os::path::absPath(xmlName, absPath);
  std::string junk;
  os::path::split(absPath, _projPath, junk);
  logger.line();
  logger << Logger::INFO_MSG << "Project root: " << _projPath << "\n";

  // Project parameters
  {
    const char* name = rootNode->Attribute("scene");
    if (name != 0x0) {
      std::string tmp = os::path::join(2, _projPath.c_str(), name);
      os::path::absPath(tmp, _sceneXML);
    }
  }

  {
    const char* name = rootNode->Attribute("behavior");
    if (name != 0x0) {
      std::string tmp = os::path::join(2, _projPath.c_str(), name);
      os::path::absPath(tmp, _behaviorXML);
    }
  }

  {
    const char* name = rootNode->Attribute("model");
    if (name != 0x0) {
      _modelName = std::string(name);
    }
  }

  {
    const char* name = rootNode->Attribute("output");
    if (name != 0x0) {
      setOutputName(os::path::join(2, _projPath.c_str(), name));
    }
  }

  {
    const char* name = rootNode->Attribute("scbVersion");
    if (name != 0x0) {
      // TODO: Validate this version
      _scbVersion = std::string(name);
    }
  }

  {
    const char* name = rootNode->Attribute("dumpPath");
    if (name != 0x0) {
      std::string tmp = os::path::join(2, _projPath.c_str(), name);
      os::path::absPath(tmp, _imgDumpPath);
    }
  }

  {
    const char* name = rootNode->Attribute("view");
    if (name != 0x0) {
      std::string tmp = os::path::join(2, _projPath.c_str(), name);
      os::path::absPath(tmp, _viewConfig);
    }
  }

  double d;
  int i;

  if (rootNode->Attribute("duration", &d)) {
    _duration = (float)d;
  }

  if (rootNode->Attribute("timeStep", &d)) {
    _timeStep = (float)d;
  }

  if (rootNode->Attribute("random", &i)) {
    _seed = i;
  }

  if (rootNode->Attribute("subSteps", &i)) {
    _subSteps = (size_t)i;
  }

  std::cout << "Returning true\n";
  return true;
}

////////////////////////////////////////////////////////////////

Logger& operator<<(Logger& out, const ProjectSpec& spec) {
  out << "<?xml version=\"1.0\"?>\n\n<Project\n";
  out << "\tscene=\"" << spec._sceneXML << "\"\n";
  out << "\tbehavior=\"" << spec._behaviorXML << "\"\n";
  out << "\tmodel=\"" << spec._modelName << "\"\n";
  out << "\toutput=\"" << spec._scbFileName << "\"\n";
  out << "\tscbVersion=\"" << spec._scbVersion << "\"\n";
  out << "\tview=\"" << spec._viewConfig << "\"\n";
  out << "\tduration=\"" << spec._duration << "\"\n";
  out << "\ttimeStep=\"" << spec._timeStep << "\"\n";
  out << "\trandom=\"" << spec._seed << "\"\n";
  out << "\tdumpPath=\"" << spec._imgDumpPath << "\"\n";
  out << "\tsubSteps=\"" << spec._subSteps << "\"\n";
  out << "/>";
  return out;
}

}  // namespace Menge
