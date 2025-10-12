/*
 OSM Simulation plugin

 Sergio Lozano Mateos
 UNED

*/

#include "OSMInitializer.h"
#include "OSMAgent.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/Runtime/Logger.h"

namespace OSM {

using Menge::Logger;
using Menge::logger;
using Menge::Agents::BaseAgent;
using Menge::Math::ConstFloatGenerator;

////////////////////////////////////////////////////////////////
//			Implementation of Karamouzas::AgentInitializer
////////////////////////////////////////////////////////////////

// Default values
const float PER_SPACE = 1.f;     ///< The default personal space for the agent.
const float ANTICIPATION = 3.f;  ///< The anticipation time of the agent.

////////////////////////////////////////////////////////////////

AgentInitializer::AgentInitializer() : Menge::Agents::AgentInitializer() {
  _perSpace = new ConstFloatGenerator(PER_SPACE);
  _anticipation = new ConstFloatGenerator(ANTICIPATION);
}

////////////////////////////////////////////////////////////////

AgentInitializer::AgentInitializer(const AgentInitializer& init)
    : Menge::Agents::AgentInitializer(init) {
  _perSpace = init._perSpace->copy();
  _anticipation = init._anticipation->copy();
}

////////////////////////////////////////////////////////////////

AgentInitializer::~AgentInitializer() {
  delete _perSpace;
  delete _anticipation;
}

////////////////////////////////////////////////////////////////

bool AgentInitializer::setProperties(BaseAgent* agent) {
  Agent* a = dynamic_cast<Agent*>(agent);
  if (a == 0x0) return false;
  a->_perSpace = _perSpace->getValue();
  a->_anticipation = _anticipation->getValue();

  return Menge::Agents::AgentInitializer::setProperties(agent);
}

////////////////////////////////////////////////////////////////

bool AgentInitializer::isRelevant(const ::std::string& tagName) {
  return (tagName == "OSM") || Menge::Agents::AgentInitializer::isRelevant(tagName);
}

////////////////////////////////////////////////////////////////

Menge::Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute(
    const ::std::string& paramName, const ::std::string& value) {
  ParseResult result = IGNORED;
  if (paramName == "personal_space") {
    result = constFloatGenerator(_perSpace, value);
  } else if (paramName == "anticipation") {
    result = constFloatGenerator(_anticipation, value);
  }

  if (result == FAILURE) {
    logger << Logger::WARN_MSG << "Attribute \"" << paramName;
    logger << "\" had an incorrectly formed value: \"" << value;
    logger << "\".  Using default value.";
    result = ACCEPTED;
  } else if (result == IGNORED) {
    return Menge::Agents::AgentInitializer::setFromXMLAttribute(paramName, value);
  }
  return result;
}

////////////////////////////////////////////////////////////////

AgentInitializer::ParseResult AgentInitializer::processProperty(::std::string propName,
                                                                TiXmlElement* node) {
  ParseResult result = IGNORED;
  if (propName == "personal_space") {
    result = getFloatGenerator(_perSpace, node);
  } else if (propName == "anticipation") {
    result = getFloatGenerator(_anticipation, node);
  }

  if (result == FAILURE) {
    logger << Logger::ERR_MSG << "Error extracting value distribution from Property ";
    logger << propName << ".";
    return result;
  } else if (result == IGNORED) {
    return Menge::Agents::AgentInitializer::processProperty(propName, node);
  }
  return result;
}

////////////////////////////////////////////////////////////////

void AgentInitializer::setDefaults() {
  if (_perSpace) delete _perSpace;
  _perSpace = new ConstFloatGenerator(PER_SPACE);
  if (_anticipation) delete _anticipation;
  _anticipation = new ConstFloatGenerator(ANTICIPATION);

  Menge::Agents::AgentInitializer::setDefaults();
}

////////////////////////////////////////////////////////////////

}  // namespace Karamouzas
