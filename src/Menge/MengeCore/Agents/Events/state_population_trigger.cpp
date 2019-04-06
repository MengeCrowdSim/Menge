#include "MengeCore/Agents/Events/state_population_trigger.h"

#include <sstream>

#include "MengeCore/Agents/Events/EventException.h"
#include "MengeCore/BFSM/State.h"

namespace Menge {

/////////////////////////////////////////////////////////////////////
//           Implementation of StatePopulationTrigger
/////////////////////////////////////////////////////////////////////

StatePopulationTrigger::StatePopulationTrigger() : StateEvtTrigger(), _lastPop(0) {}

/////////////////////////////////////////////////////////////////////

void StatePopulationTrigger::finalize() {
  StateEvtTrigger::finalize();
  _lastPop = _state->getPopulation();
}

/////////////////////////////////////////////////////////////////////

bool StatePopulationTrigger::testCondition() {
  const size_t prev_population = _lastPop;
  _lastPop = _state->getPopulation();
  switch (_mode) {
    case ON_INCREASE:
      return _lastPop > prev_population;
    case ON_DECREASE:
      return _lastPop < prev_population;
    case RISE_TO:
      return prev_population < _threshold && _lastPop >= _threshold;
    case DROP_TO:
      return prev_population > _threshold && _lastPop <= _threshold;
    case WHILE_HIGHER:
      return _lastPop > _threshold;
    case WHILE_LOWER:
      return _lastPop < _threshold;
    default:
      throw EventException("StatePopulationTrigger has a invalid behavior mode");
  }
}

/////////////////////////////////////////////////////////////////////

bool StatePopulationTrigger::set_mode(const std::string& mode_name) {
  if (mode_name == "on_increase") {
    _mode = ON_INCREASE;
    return false;
  } else if (mode_name == "on_decrease") {
    _mode = ON_DECREASE;
    return false;
  } else if (mode_name == "rise_to") {
    _mode = RISE_TO;
    return true;
  } else if (mode_name == "drop_to") {
    _mode = DROP_TO;
    return true;
  } else if (mode_name == "while_higher") {
    _mode = WHILE_HIGHER;
    return true;
  } else if (mode_name == "while_lower") {
    _mode = WHILE_LOWER;
    return true;
  } else {
    std::stringstream ss;
    ss << "Unrecognized behavior value for StatePopulationTrigger: '" << mode_name << "'";
    throw EventException(ss.str());
  }
}

/////////////////////////////////////////////////////////////////////
//           Implementation of StatePopulationTriggerFactory
/////////////////////////////////////////////////////////////////////

StatePopulationTriggerFactory::StatePopulationTriggerFactory() : StateEvtTriggerFactory() {
  _behaviorID = _attrSet.addStringAttribute("behavior", true, "");
  _thresholdID = _attrSet.addIntAttribute("threshold", false, -1);
}

/////////////////////////////////////////////////////////////////////

bool StatePopulationTriggerFactory::setFromXML(EventTrigger* trigger, TiXmlElement* node,
                                               const std::string& specFldr) const {
  StatePopulationTrigger* sTrigger = dynamic_cast<StatePopulationTrigger*>(trigger);
  assert(sTrigger != 0x0 &&
         "Trying to set attributes of a state event trigger on an incompatible object");

  if (!StateEvtTriggerFactory::setFromXML(sTrigger, node, specFldr)) return false;

  bool need_threshold = sTrigger->set_mode(_attrSet.getString(_behaviorID));
  if (need_threshold) {
    int value = _attrSet.getInt(_thresholdID);
    if (value < 0) {
      throw EventException(
          "Missing or invalid threshold value for 'state_population' event trigger");
    }
    sTrigger->set_threshold(value);
  }

  return true;
}

}  // namespace Menge
