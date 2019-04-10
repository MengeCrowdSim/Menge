/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill.
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

#include "MengeCore/Agents/ObstacleSets/ExplicitObstacleSet.h"

#include "thirdParty/tinyxml.h"

#include <vector>

namespace Menge {

namespace Agents {

////////////////////////////////////////////////////////////////////////////
//      Implementation of ExplicitObstacleSet
////////////////////////////////////////////////////////////////////////////

ExplicitObstacleSet::ExplicitObstacleSet() : ListObstacleSet() {}

////////////////////////////////////////////////////////////////////////////
//      Implementation of ExplicitObstacleSetFactory
////////////////////////////////////////////////////////////////////////////

bool ExplicitObstacleSetFactory::setFromXML(ObstacleSet* gen, TiXmlElement* node,
                                            const std::string& specFldr) const {
  ExplicitObstacleSet* eSet = dynamic_cast<ExplicitObstacleSet*>(gen);
  assert(eSet != 0x0 &&
         "Trying to set attributes of an explicit obstacle set on an "
         "incompatible object");

  if (!ObstacleSetFactory::setFromXML(eSet, node, specFldr)) return false;

  bool isClosed = false;
  for (TiXmlElement* child = node->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (child->ValueStr() == "Obstacle") {
      try {
        ObstacleVertexList obs = parseObstacle(child);
        eSet->addObstacle(obs);
      } catch (ObstacleSetException) {
        return false;
      }
    } else {
      logger << Logger::WARN_MSG
             << "Found an unexpected child tag in an "
                "ObstacleSet on line "
             << node->Row() << ".  Ignoring the tag: ";
      logger << child->ValueStr() << ".";
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////

ObstacleVertexList ExplicitObstacleSetFactory::parseObstacle(TiXmlElement* node) const {
  int iVal;
  ObstacleVertexList vList;
  vList.closed = false;
  if (node->Attribute("closed", &iVal)) {
    vList.closed = (iVal != 0);
  }

  if (!vList.closed) {
    logger << Logger::ERR_MSG
           << "This version of Menge does not support open "
              "obstacles.  Obstacle on line "
           << node->Row() << " is declared to be open.\n";
    throw ObstacleSetFatalException("Unsupported open obstacles");
  }

  double dVal;
  bool valid = true;

  for (TiXmlElement* vert = node->FirstChildElement(); vert; vert = vert->NextSiblingElement()) {
    if (vert->ValueStr() == "Vertex") {
      float p_x = 0;
      float p_y = 0;
      if (vert->Attribute("p_x", &dVal)) {
        p_x = (float)dVal;
      } else {
        valid = false;
      }
      if (vert->Attribute("p_y", &dVal)) {
        p_y = (float)dVal;
      } else {
        valid = false;
      }

      if (!valid) {
        logger << Logger::ERR_MSG << "Obstacle vertex on line " << vert->Row();
        logger << " is missing the full x- and y-position specification.";
        throw ObstacleSetFatalException(
            "Obstacle vertex missing full "
            "specification");
      }
      vList.vertices.push_back(Vector2(p_x, p_y));
    } else {
      logger << Logger::WARN_MSG
             << "Encountered unexpected tag inside an obstacle "
                "definition on line "
             << vert->Row() << ": " << vert->ValueStr()
             << ".  "
                "It will be ignored.";
    }

    if (!valid) {
      logger << Logger::ERR_MSG << "Incomplete obstacle definition on line ";
      logger << node->Row() << ".";
      throw ObstacleSetFatalException("Incomplete obstacle definition");
    }
  }

  return vList;
};
}  // namespace Agents
}  // namespace Menge
