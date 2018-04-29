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

#include "MengeCore/Agents/Events/EventTriggerExternal.h"

#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/Core.h"

#include <cassert>

namespace Menge {

/////////////////////////////////////////////////////////////////////
//          Implementation of ExternalEvtTriggerFactory
/////////////////////////////////////////////////////////////////////

bool ExternalEvtTriggerFactory::setFromXML(EventTrigger* trigger, TiXmlElement* node,
                                           const std::string& specFldr) const {
  ExternalEvtTrigger* xTrigger = dynamic_cast<ExternalEvtTrigger*>(trigger);
  assert(xTrigger != 0x0 &&
         "Trying to set attributes of an external event trigger on an incompatible object");

  if (!EventTriggerFactory::setFromXML(xTrigger, node, specFldr)) return false;

  EVENT_SYSTEM->addExternalEventTrigger(xTrigger);

  return true;
}
}  // namespace Menge
