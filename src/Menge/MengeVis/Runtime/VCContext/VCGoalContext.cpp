#include "MengeVis/Runtime/VCContext/VCGoalContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/VelocityComponents/VelCompGoal.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <sstream>

namespace MengeVis {
namespace Runtime {
namespace VCContext {

using Menge::Agents::BaseAgent;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::Goal;
using Menge::BFSM::GoalVelComponent;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//                   Implementation of GoalVCContext
/////////////////////////////////////////////////////////////////////

GoalVCContext::GoalVCContext() : VelCompContext(), _vc(0x0) {}

/////////////////////////////////////////////////////////////////////

void GoalVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const GoalVelComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a goal velocity component context "
        "with an invalid value: either null or wrong "
        "velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string GoalVCContext::getElementName() const { return GoalVelComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string GoalVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Goal velocity component";
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

void GoalVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  // draw goal point
  // TODO: Get the elevation of the goalPoint
  Vector2 goalPoint = goal->getTargetPoint(agt->_pos, agt->_radius);
  drawGoal(goalPoint, agt);

  // draw preferred velocity
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos);
}
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis
