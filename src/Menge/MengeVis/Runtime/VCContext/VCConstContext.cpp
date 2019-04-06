#include "MengeVis/Runtime/VCContext/VCConstContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/VelocityComponents/VelCompConst.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <sstream>

namespace MengeVis {
namespace Runtime {
namespace VCContext {

using Menge::Agents::BaseAgent;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::ConstVelComponent;
using Menge::BFSM::ConstVelDirComponent;
using Menge::BFSM::Goal;
using Menge::BFSM::ZeroVelComponent;

/////////////////////////////////////////////////////////////////////
//                   Implementation of ConstVCContext
/////////////////////////////////////////////////////////////////////

ConstVCContext::ConstVCContext() : VelCompContext(), _vc(0x0) {}

/////////////////////////////////////////////////////////////////////

void ConstVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const ConstVelComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a const velocity component context "
        "with an invalid value: either null or wrong "
        "velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string ConstVCContext::getElementName() const { return ConstVelComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string ConstVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Const: " << _vc->getConstVelocity();
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

void ConstVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  // draw preferred velocity
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos);
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of ConstDirVCContext
/////////////////////////////////////////////////////////////////////

ConstDirVCContext::ConstDirVCContext() : VelCompContext(), _vc(0x0) {}

/////////////////////////////////////////////////////////////////////

void ConstDirVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const ConstVelDirComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a const direciton velocity component"
        " context with an invalid value: either null or "
        "wrong velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string ConstDirVCContext::getElementName() const { return ConstVelDirComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string ConstDirVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Const direction: " << _vc->getDirection();
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

void ConstDirVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  // draw preferred velocity
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos);
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of ZeroVCContext
/////////////////////////////////////////////////////////////////////

ZeroVCContext::ZeroVCContext() : VelCompContext() {}

/////////////////////////////////////////////////////////////////////

std::string ZeroVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Zero velocity";
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

std::string ZeroVCContext::getElementName() const { return ZeroVelComponent::NAME; }
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis
