#include "MengeCore/Agents/Events/TargetAgentById.h"

#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/Core.h"
#include "MengeCore/Runtime/Logger.h"

namespace Menge {

/////////////////////////////////////////////////////////////////////
//          Implementation of TargetAgentById
/////////////////////////////////////////////////////////////////////

void TargetAgentById::update() {
  _elements.clear();
  Agents::BaseAgent* agent = SIMULATOR->getAgent(_agentId);
  if (agent) {
    _elements.push_back(agent);
  } else {
    // TODO: Add name as a field on the EventTarget calss.
    logger << Logger::WARN_MSG << "TargetAgentById event target attempted";
    logger << " to target agent with id " << _agentId << ". No agent existed with that ";
    logger << "identifier.";
  }
  // TODO: This is bad; I shouldn't be calling the parent class. The parent class should
  // determine if update is required and invoke a DoUpdate so the child can populate the
  // elements with the known agents.
  AgentEventTarget::update();
}

/////////////////////////////////////////////////////////////////////
//          Implementation of TargetAgentByIdFactory
/////////////////////////////////////////////////////////////////////

TargetAgentByIdFactory::TargetAgentByIdFactory() : AgentEventTargetFactory() {
  _idID = _attrSet.addSizeTAttribute("id", true /*required*/);
}

/////////////////////////////////////////////////////////////////////

bool TargetAgentByIdFactory::setFromXML(EventTarget* target, TiXmlElement* node,
                                        const std::string& specFldr) const {
  TargetAgentById* idTarget = dynamic_cast<TargetAgentById*>(target);
  assert(idTarget != 0x0 &&
         "Trying to set attributes of a target-by-id event target "
         "on an incompatible object");

  if (!AgentEventTargetFactory::setFromXML(target, node, specFldr)) return false;

  idTarget->_agentId = _attrSet.getSizeT(_idID);

  return true;
}

}  // namespace Menge
