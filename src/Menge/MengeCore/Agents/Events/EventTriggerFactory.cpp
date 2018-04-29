#include "MengeCore/Agents/Events/EventTriggerFactory.h"

namespace Menge {

/////////////////////////////////////////////////////////////////////
//          Implementation of EventTriggerFactory
/////////////////////////////////////////////////////////////////////

EventTriggerFactory::EventTriggerFactory() {
  _nameID = _attrSet.addStringAttribute("name", true, "");
}

/////////////////////////////////////////////////////////////////////

bool EventTriggerFactory::setFromXML(EventTrigger* trigger, TiXmlElement* node,
                                     const std::string& specFldr) const {
  if (!ElementFactory<EventTrigger>::setFromXML(trigger, node, specFldr)) return false;
  trigger->_name = _attrSet.getString(_nameID);

  return true;
}

}  // namespace Menge
