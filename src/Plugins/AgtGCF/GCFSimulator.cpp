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

#include "GCFSimulator.h"
#include "MengeCore/Runtime/Utils.h"

namespace GCF {

using Menge::toFloat;
using Menge::UtilException;
using Menge::Agents::SimulatorBase;
using Menge::Agents::XMLParamException;

////////////////////////////////////////////////////////////////
//					Implementation of GCF::Simulator
////////////////////////////////////////////////////////////////

// These values come directly from the GCF 2010 paper -- TODO: Determine that this is true
float Simulator::REACTION_TIME = 0.5f;
float Simulator::NU_AGENT = 0.3f;
float Simulator::MAX_AGENT_DIST = 2.f;
float Simulator::MAX_AGENT_FORCE = 3.f;
float Simulator::AGENT_INTERP_WIDTH = 0.12f;
bool Simulator::SPEED_COLOR = false;

////////////////////////////////////////////////////////////////

bool Simulator::setExpParam(const std::string& paramName,
                            const std::string& value) throw(XMLParamException) {
  try {
    if (paramName == "reaction_time") {
      REACTION_TIME = toFloat(value);
    } else if (paramName == "agent_force_strength") {
      NU_AGENT = toFloat(value);
    } else if (paramName == "max_agent_dist") {
      MAX_AGENT_DIST = toFloat(value);
    } else if (paramName == "max_agent_force") {
      MAX_AGENT_FORCE = toFloat(value);
    } else if (paramName == "agent_interp_width") {
      AGENT_INTERP_WIDTH = toFloat(value);
    } else if (paramName == "speed_color") {
      SPEED_COLOR = toInt(value) != 0;
    } else if (!Agents::SimulatorBase<Agent>::setExpParam(paramName, value)) {
      // Simulator base didn't recognize the parameter either
      return false;
    }
  } catch (UtilException) {
    throw Agents::XMLParamException(std::string("GCF parameter \"") + paramName +
                                    std::string("\" value couldn't be converted to the "
                                                "correct type.  Found the value: ") +
                                    value);
  }
  return true;
}

////////////////////////////////////////////////////////////////

void Simulator::finalize() {
  Agents::SimulatorBase<Agent>::finalize();
  const float thresh = 1.f / (3.f * MAX_AGENT_FORCE);
  if (AGENT_INTERP_WIDTH < thresh) {
    AGENT_INTERP_WIDTH = thresh * 1.5f;
    // TODO: Log this change
  }
}
}  // namespace GCF
