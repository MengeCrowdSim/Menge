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

#include "MengeCore/BFSM/VelocityComponents/VelCompNavMesh.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Tasks/NavMeshLocalizerTask.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeCore/Runtime/os.h"
#include "MengeCore/resources/PathPlanner.h"
#include "MengeCore/resources/PortalPath.h"
#include "MengeCore/resources/Route.h"

#include <iomanip>
#include <sstream>

namespace Menge {

namespace BFSM {

/////////////////////////////////////////////////////////////////////
//                   Implementation of NavMeshVelComponent
/////////////////////////////////////////////////////////////////////

const std::string NavMeshVelComponent::NAME = "nav_mesh";

/////////////////////////////////////////////////////////////////////

NavMeshVelComponent::NavMeshVelComponent()
    : VelComponent(), _headingDevCos(-1.f), _navMesh(0x0), _localizer(0x0) {}

/////////////////////////////////////////////////////////////////////

void NavMeshVelComponent::onExit(Agents::BaseAgent* agent) { _localizer->clearPath(agent->_id); }

/////////////////////////////////////////////////////////////////////

void NavMeshVelComponent::setHeadingDeviation(float angle) { _headingDevCos = cos(angle); }

/////////////////////////////////////////////////////////////////////

void NavMeshVelComponent::setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                                          Agents::PrefVelocity& pVel) const {
  // If agent does not have a path
  PortalPath* path = _localizer->getPath(agent->_id);
  if (path == 0x0) {
    // Get the route
    Vector2 goalPoint = goal->getCentroid();
    unsigned int goalNode = _localizer->getNode(goalPoint);
    if (goalNode == NavMeshLocation::NO_NODE) {
      throw VelCompFatalException(
          "Can't compute a path to a goal outside of the "
          "navigation mesh.  Bad NavMeshVelComponent!");
    }
    unsigned int agtNode = _localizer->getNode(agent);
    PortalRoute* route =
        _localizer->getPlanner()->getRoute(agtNode, goalNode, agent->_radius * 2.f);
    // compute the path
    path = new PortalPath(agent->_pos, goal, route, agent->_radius);
    // assign it to the localizer
    _localizer->setPath(agent->_id, path);
  }
  pVel.setSpeed(agent->_prefSpeed);
  path->setPreferredDirection(agent, _headingDevCos, pVel);
}

/////////////////////////////////////////////////////////////////////

BFSM::Task* NavMeshVelComponent::getTask() {
  return new NavMeshLocalizerTask(_navMesh->getName(), true /*usePlanner*/);
}

/////////////////////////////////////////////////////////////////////

void NavMeshVelComponent::doUpdateGoal(const Agents::BaseAgent* agent, const Goal* goal) {
  assert(goal->moves() && "NavMeshVelComponent::doUpdateGoal called for unmoving goal");
  PortalPath* path = _localizer->getPath(agent->_id);
  assert(path != nullptr &&
         "Somehow updating a moving goal for an agent that doesn't have a path");
  assert(path->getGoal() == goal &&
         "Trying to update an (agent, goal) pair for which I have a conflicting goal");
  PortalPath* update_path = _localizer->updatePathForGoal(agent, path);
  if (update_path == nullptr) {
    logger << Logger::ERR_MSG << "Agent " << agent->_id
           << " is working toward a moving goal that can no longer be accessed from the "
              "navigation mesh.\n";
    throw VelCompFatalException("Moving goal can no longer be connected to the nav mesh");
  }
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of NavMeshVCFactory
/////////////////////////////////////////////////////////////////////

NavMeshVCFactory::NavMeshVCFactory() : VelCompFactory() {
  _fileNameID = _attrSet.addStringAttribute("file_name", true /*required*/);
  _headingID = _attrSet.addFloatAttribute("heading_threshold", false /*required*/, 180.f);
}

/////////////////////////////////////////////////////////////////////

bool NavMeshVCFactory::setFromXML(VelComponent* vc, TiXmlElement* node,
                                  const std::string& behaveFldr) const {
  NavMeshVelComponent* nmvc = dynamic_cast<NavMeshVelComponent*>(vc);
  assert(nmvc != 0x0 &&
         "Trying to set attributes of a navigation mesh velocity component on an "
         "incompatible object");

  if (!VelCompFactory::setFromXML(nmvc, node, behaveFldr)) return false;

  // get the absolute path to the file name

  std::string fName;
  std::string path = os::path::join(2, behaveFldr.c_str(), _attrSet.getString(_fileNameID).c_str());
  os::path::absPath(path, fName);
  // nav mesh
  NavMeshPtr nmPtr;
  try {
    nmPtr = loadNavMesh(fName);
  } catch (ResourceException) {
    logger << Logger::ERR_MSG;
    logger << "Couldn't instantiate the navigation mesh referenced on line ";
    logger << node->Row() << ".";
    return false;
  }
  nmvc->setNavMesh(nmPtr);
  // nav mesh localizer
  NavMeshLocalizerPtr nmlPtr;
  try {
    nmlPtr = loadNavMeshLocalizer(fName, true);
  } catch (ResourceException) {
    logger << Logger::ERR_MSG
           << "Couldn't instantiate the navigation mesh localizer "
              "required by the velocity component on line "
           << node->Row() << ".";
    return false;
  }
  nmvc->setNavMeshLocalizer(nmlPtr);
  nmvc->setHeadingDeviation(_attrSet.getFloat(_headingID) * DEG_TO_RAD);

  return true;
}

}  // namespace BFSM
}  // namespace Menge
