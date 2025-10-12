/*
 OSM Simulation plugin

 Sergio Lozano Mateos
 UNED

*/

#include "OSMSimulator.h"
#include "MengeCore/Runtime/Utils.h"

namespace OSM {

using Menge::DEG_TO_RAD;
using Menge::toFloat;
using Menge::toInt;
using Menge::UtilException;
using Menge::Agents::SimulatorBase;
using Menge::Agents::XMLParamException;

////////////////////////////////////////////////////////////////
//					Implementation of Karamouzas::Simulator
////////////////////////////////////////////////////////////////

// These values come directly from the Karamouzas paper
float Simulator::ORIENT_WEIGHT = 0.8f;
float Simulator::COS_FOV_ANGLE = cos(100.f * DEG_TO_RAD);  // 100 degrees (200 total)
float Simulator::REACTION_TIME = 0.4f;
float Simulator::WALL_STEEPNESS = 2.f;
float Simulator::WALL_DISTANCE = 2.f;  // 2 meters (from the author's email...seem's quite large.)
int Simulator::COLLIDING_COUNT = 5;    // K most dangerous neighbors considered
float Simulator::D_MIN = 1.f;
float Simulator::D_MID = 8.f;
float Simulator::D_MAX = 10.f;
float Simulator::AGENT_FORCE = 3.f;  // how many Newtons?

////////////////////////////////////////////////////////////////

bool Simulator::setExpParam(const std::string& paramName,
                            const std::string& value) throw(XMLParamException) {
  try {
    if (paramName == "orient_weight") {
      ORIENT_WEIGHT = toFloat(value);
    } else if (paramName == "fov") {
      COS_FOV_ANGLE = cos(0.5f * toFloat(value) *
                          DEG_TO_RAD);  // convert degrees to radians - operate on half angle
    } else if (paramName == "reaction_time") {
      REACTION_TIME = toFloat(value);
    } else if (paramName == "wall_steepness") {
      WALL_STEEPNESS = toFloat(value);
    } else if (paramName == "wall_distance") {
      WALL_DISTANCE = toFloat(value);
    } else if (paramName == "colliding_count") {
      COLLIDING_COUNT = toInt(value);
    } else if (paramName == "d_min") {
      D_MIN = toFloat(value);
    } else if (paramName == "d_mid") {
      D_MID = toFloat(value);
    } else if (paramName == "d_max") {
      D_MAX = toFloat(value);
    } else if (paramName == "agent_force") {
      AGENT_FORCE = toFloat(value);
    } else if (!SimulatorBase<Agent>::setExpParam(paramName, value)) {
      // Simulator base didn't recognize the parameter either
      return false;
    }
  } catch (UtilException) {
    throw XMLParamException(std::string("Karamouzas parameter \"") + paramName +
                            std::string("\" value couldn't be converted to the correct "
                                        "type.  Found the value: ") +
                            value);
  }
  return true;
}
}  // namespace OSM
