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

#include "GCFInitializer.h"
#include "GCFAgent.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/Runtime/Logger.h"

namespace GCF {

using Menge::Logger;
using Menge::logger;
using Menge::Agents::BaseAgent;
using Menge::Math::ConstFloatGenerator;

////////////////////////////////////////////////////////////////
//			Implementation of GCF::AgentInitializer
////////////////////////////////////////////////////////////////

// Default values
const float A_MIN = 0.18f;   ///< The agent's default minimum minor "facing" size
const float A_RATE = 0.53f;  ///< The agent's default rate of growth on the "facing" axis
const float B_MAX = 0.25f;   ///< The agent's default perpendicular axis radius.
const float B_GROWTH =
    0.05f;  ///< The agent's default growth rate for the perpendicular axis radius.

////////////////////////////////////////////////////////////////

AgentInitializer::AgentInitializer() : Menge::Agents::AgentInitializer() {
  _aMin = new ConstFloatGenerator(0.18f);
  _aRate = new ConstFloatGenerator(0.53f);
  _bMax = new ConstFloatGenerator(0.25f);
  _bGrowth = new ConstFloatGenerator(0.05f);
}

////////////////////////////////////////////////////////////////

AgentInitializer::AgentInitializer(const AgentInitializer& init)
    : Menge::Agents::AgentInitializer(init) {
  _aMin = init._aMin->copy();
  _aRate = init._aRate->copy();
  _bMax = init._bMax->copy();
  _bGrowth = init._bGrowth->copy();
}

////////////////////////////////////////////////////////////////

AgentInitializer::~AgentInitializer() {
  delete _aMin;
  delete _aRate;
  delete _bMax;
  delete _bGrowth;
}

////////////////////////////////////////////////////////////////

bool AgentInitializer::setProperties(Agents::BaseAgent* agent) {
  Agent* a = dynamic_cast<Agent*>(agent);
  if (a == 0x0) return false;
  a->_aMin = _aMin->getValue();
  a->_aRate = _aRate->getValue();
  a->_bMax = _bMax->getValue();
  a->_bGrowth = _bGrowth->getValue();
  bool val = Agents::AgentInitializer::setProperties(agent);
  a->_ellipse.setCenter(a->_pos);
  a->postUpdate();
  return val;
}

////////////////////////////////////////////////////////////////

bool AgentInitializer::isRelevant(const ::std::string& tagName) {
  return (tagName == "GCF") || Agents::AgentInitializer::isRelevant(tagName);
}

////////////////////////////////////////////////////////////////

Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute(
    const ::std::string& paramName, const ::std::string& value) {
  ParseResult result = IGNORED;
  if (paramName == "stand_depth") {
    result = constFloatGenerator(_aMin, value);
  } else if (paramName == "move_scale") {
    result = constFloatGenerator(_aRate, value);
  } else if (paramName == "slow_width") {
    result = constFloatGenerator(_bMax, value);
  } else if (paramName == "sway_change") {
    result = constFloatGenerator(_bGrowth, value);
  }

  if (result == FAILURE) {
    logger << Logger::WARN_MSG << "Attribute \"" << paramName << "\" had an incorrectly ";
    logger << "formed value: \"" << value << "\".  Using default value.";
    result = ACCEPTED;
  } else if (result == IGNORED) {
    return Agents::AgentInitializer::setFromXMLAttribute(paramName, value);
  }
  return result;
}

////////////////////////////////////////////////////////////////

AgentInitializer::ParseResult AgentInitializer::processProperty(::std::string propName,
                                                                TiXmlElement* node) {
  ParseResult result = IGNORED;
  if (propName == "facing_min") {
    result = getFloatGenerator(_aMin, node);
  } else if (propName == "facing_rate") {
    result = getFloatGenerator(_aRate, node);
  } else if (propName == "perp_max") {
    result = getFloatGenerator(_bMax, node);
  } else if (propName == "perp_growth") {
    result = getFloatGenerator(_bGrowth, node);
  }

  if (result == FAILURE) {
    logger << Logger::ERR_MSG << "Error extracting value distribution from Property ";
    logger << propName << ".";
    return result;
  } else if (result == IGNORED) {
    return Agents::AgentInitializer::processProperty(propName, node);
  }
  return result;
}

////////////////////////////////////////////////////////////////

void AgentInitializer::setDefaults() {
  if (_aMin) delete _aMin;
  _aMin = new ConstFloatGenerator(A_MIN);
  if (_aRate) delete _aRate;
  _aRate = new ConstFloatGenerator(A_RATE);
  if (_bMax) delete _bMax;
  _bMax = new ConstFloatGenerator(B_MAX);
  if (_bGrowth) delete _bGrowth;
  _bGrowth = new ConstFloatGenerator(B_GROWTH);
  Agents::AgentInitializer::setDefaults();
}

////////////////////////////////////////////////////////////////

}  // namespace GCF
