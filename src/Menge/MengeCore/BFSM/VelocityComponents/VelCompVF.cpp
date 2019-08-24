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

#include "MengeCore/BFSM/VelocityComponents/VelCompVF.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeCore/Runtime/os.h"

#include <iomanip>
#include <sstream>

namespace Menge {

namespace BFSM {

/////////////////////////////////////////////////////////////////////
//                   Implementation of VFVelComponent
/////////////////////////////////////////////////////////////////////

const std::string VFVelComponent::NAME = "vel_field";

/////////////////////////////////////////////////////////////////////

VFVelComponent::VFVelComponent() : VelComponent(), _vf(0x0), _nearest(true) {}

/////////////////////////////////////////////////////////////////////

VFVelComponent::VFVelComponent(VectorFieldPtr& vf, bool useNearest)
    : VelComponent(), _vf(vf), _nearest(useNearest) {}

/////////////////////////////////////////////////////////////////////

void VFVelComponent::setPrefVelocity(const Agents::BaseAgent* agent, const Goal*,
                                     Agents::PrefVelocity& pVel) const {
  Vector2 dir;
  if (_nearest) {
    dir.set(_vf->getFieldValue(agent->_pos));
  } else {
    dir.set(_vf->getFieldValueInterp(agent->_pos));
  }

  float scale = abs(dir);
  float speed = agent->_prefSpeed * scale;

  if (scale > EPS) {
    pVel.setSingle(dir / scale);
    pVel.setSpeed(speed);
  } else {
    pVel.setSingle(Vector2(1.f, 0.f));
    pVel.setSpeed(0.f);
  }
  pVel.setTarget(pVel.getPreferredVel() * 5.f + agent->_pos);
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of VFVCFactory
/////////////////////////////////////////////////////////////////////

VFVCFactory::VFVCFactory() : VelCompFactory() {
  _fileNameID = _attrSet.addStringAttribute("file_name", true /*required*/);
  _useNearestID = _attrSet.addBoolAttribute("use_nearest", false /*required*/, true /*default*/);
}

/////////////////////////////////////////////////////////////////////

bool VFVCFactory::setFromXML(VelComponent* vc, TiXmlElement* node,
                             const std::string& behaveFldr) const {
  VFVelComponent* vfvc = dynamic_cast<VFVelComponent*>(vc);
  assert(vfvc != 0x0 &&
         "Trying to set attributes of a velocity field velocity "
         "component on an incompatible object");

  if (!VelCompFactory::setFromXML(vfvc, node, behaveFldr)) return false;

  // get the file name
  std::string fName;
  std::string path = os::path::join(2, behaveFldr.c_str(), _attrSet.getString(_fileNameID).c_str());
  os::path::absPath(path, fName);
  VectorFieldPtr vfPtr;
  try {
    vfPtr = loadVectorField(fName);
  } catch (ResourceException) {
    logger << Logger::ERR_MSG
           << "Couldn't instantiate the vector field referenced "
              "on line "
           << node->Row() << ".";
    return false;
  }
  vfvc->setVectorField(vfPtr);
  vfvc->setUseNearest(_attrSet.getBool(_useNearestID));

  return true;
}

}  // namespace BFSM
}  // namespace Menge
