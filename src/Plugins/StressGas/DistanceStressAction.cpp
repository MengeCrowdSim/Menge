#include "DistanceStressAction.h"

#include <algorithm>

#include "DistanceStressFunction.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeCore/Math/RandGenerator.h"

namespace StressGAS {

using Menge::logger;
using Menge::Logger;
using Menge::Agents::BaseAgent;
using Menge::BFSM::Action;
using Menge::Math::createGeometry;
using Menge::Math::FloatGenerator;
using Menge::Math::Geometry2D;

/////////////////////////////////////////////////////////////////////
//                   Implementation of DistanceStressAction
/////////////////////////////////////////////////////////////////////

StressFunction* DistanceStressAction::makeStressFunction(BaseAgent* agent, AgentStressor* stressor,
                                                         float coolDuration) {
  return new DistanceStressFunction(_geometry, _innerDist->getValue(), _outerDist->getValue(),
                                    _func, agent, stressor, _coolDuration->getValue());
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//                   Implementation of DistanceStressActionFactory
/////////////////////////////////////////////////////////////////////

DistanceStressActionFactory::DistanceStressActionFactory() {
  _outerId = _attrSet.addFloatDistAttribute("outer_", true, 10.f, 1.f);
  _innerId = _attrSet.addFloatDistAttribute("inner_", true, 10.f, 1.f);
  _funcId = _attrSet.addStringAttribute("fall_off", false, "linear");
}

/////////////////////////////////////////////////////////////////////
//
bool DistanceStressActionFactory::setFromXML(Action* action, TiXmlElement* node,
                                             const std::string& behaveFldr) const {
  DistanceStressAction* dAction = dynamic_cast<DistanceStressAction*>(action);
  assert(dAction != 0x0 &&
         "Trying to set distance stress action properties on an incompatible object");
  if (!BaseStressActionFactory::setFromXML(action, node, behaveFldr)) {
    return false;
  }

  // problems with reading the geometry should be logged by createGeoemetry
  Geometry2D* geometry = createGeometry(node);
  if (geometry == 0x0) return false;
  if (dAction->_geometry != 0x0) delete dAction->_geometry;
  dAction->_geometry = geometry;

  if (dAction->_innerDist != 0x0) delete dAction->_innerDist;
  dAction->_innerDist = _attrSet.getFloatGenerator(_innerId);

  if (dAction->_outerDist != 0x0) delete dAction->_outerDist;
  dAction->_outerDist = _attrSet.getFloatGenerator(_outerId);

  std::string func = _attrSet.getString(_funcId);
  std::transform(func.begin(), func.end(), func.begin(), ::tolower);

  // exit behavior
  if (func == "linear")
    dAction->_func = DistanceStressFunction::LINEAR;
  else if (func == "quadratic")
    dAction->_func = DistanceStressFunction::QUADRATIC;
  else {
    logger << Logger::WARN_MSG
           << "Parameter \"fall_off\" has an unrecognizable "
              "value: "
           << func
           << ".  Should be one of: linear or quadratic.  Default to "
              "linear.";
    dAction->_func = DistanceStressFunction::LINEAR;
  }

  return true;
}
}  // namespace StressGAS
