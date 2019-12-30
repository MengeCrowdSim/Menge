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

#include "MengeCore/BFSM/VelocityComponents/VelCompRoadMap.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Runtime/os.h"
#include "MengeCore/resources/Graph.h"

#include <iomanip>
#include <sstream>

namespace Menge {

namespace BFSM {

/////////////////////////////////////////////////////////////////////
//                   Implementation of RoadMapVelComponent
/////////////////////////////////////////////////////////////////////

const std::string RoadMapVelComponent::NAME = "road_map";

/////////////////////////////////////////////////////////////////////

RoadMapVelComponent::RoadMapVelComponent() : VelComponent(), _roadmap(0x0) {}

/////////////////////////////////////////////////////////////////////

RoadMapVelComponent::RoadMapVelComponent(const GraphPtr& graph) : VelComponent(), _roadmap(graph) {}

/////////////////////////////////////////////////////////////////////

RoadMapVelComponent::~RoadMapVelComponent() {
  PathMap::iterator itr = _paths.begin();
  for (; itr != _paths.end(); ++itr) {
    delete itr->second;
  }
  _paths.clear();
}

/////////////////////////////////////////////////////////////////////

void RoadMapVelComponent::onExit(Agents::BaseAgent* agent) {
  // This test is necessary.  Because state advancement can jump MULTIPLE states in a
  //  singletime step, it is possible to enter and exit a state with a roadmap velocity
  //  without ever actually calling "setPrefVelocity" on that velocity component.
  //
  //  Roadmap initializes the path in setPrefVelocity - so, things don't get properly
  //  initialized.
  _lock.lockWrite();
  PathMap::iterator itr = _paths.find(agent->_id);
  if (itr != _paths.end()) {
    delete itr->second;
    _paths.erase(agent->_id);
  }
  _lock.releaseWrite();
}

/////////////////////////////////////////////////////////////////////

void RoadMapVelComponent::setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                                          Agents::PrefVelocity& pVel) const {
  _lock.lockRead();
  PathMap::const_iterator itr = _paths.find(agent->_id);
  RoadMapPath* path = 0x0;
  if (itr == _paths.end()) {
    _lock.releaseRead();
    // compute the path and add it to the map
    //  Create path for the agent
    path = _roadmap->getPath(agent, goal);
    if (path == nullptr) {
      throw VelCompFatalException("Agent " + std::to_string(agent->_id) +
                                  " was unable to find a path to its goal");
    }
    _lock.lockWrite();
    _paths[agent->_id] = path;
    _lock.releaseWrite();
  } else {
    path = itr->second;
    _lock.releaseRead();
  }
  pVel.setSpeed(agent->_prefSpeed);
  if (!path->setPrefDirection(agent, pVel)) {
    // Path got lost; replan and retry. If that couldn't produce a trajectory it
    // becomes an irrecoverable error.
    delete path;
    path = _roadmap->getPath(agent, goal);
    if (path == nullptr) {
      throw VelCompFatalException("Agent " + std::to_string(agent->_id) +
                                  " lost roadmap path and was unable to create a new path");
    }
    // While this operation doesn't change the structure of the map (agent->_id is already a
    // key), we lock it to prevent any *other* write operation from interfering.
    _lock.lockWrite();
    _paths[agent->_id] = path;
    _lock.releaseWrite();
    if (!path->setPrefDirection(agent, pVel)) {
      throw VelCompFatalException("Agent " + std::to_string(agent->_id) +
                                  " following a roadmap path could *not* set preferred velocity");
    }
  }
}

/////////////////////////////////////////////////////////////////////

void RoadMapVelComponent::doUpdateGoal(const Agents::BaseAgent* agent, const Goal* goal) {
  assert(goal->moves() && "RoadMapVelComponent::doUpdateGoal called for unmoving goal");
  // The only way to get to this point is to have successfully updated agent preferred velocities,
  // so there *must* already exist a path for this agent.
  _lock.lockRead();
  RoadMapPath* path = _paths.at(agent->_id);
  _lock.releaseRead();

  assert(path->getGoal() == goal &&
          "Trying to update an agent, goal pair for which I have a conflicting goal");

  RoadMapPath* update_path = _roadmap->updatePathForGoal(agent, path);
  if (update_path == nullptr) {
    logger << Logger::ERR_MSG << "Agent " << agent->_id
            << " is working toward a moving goal that can no longer be accessed from the "
              "roadmap.\n";
    throw VelCompFatalException("Moving goal can no longer be connected to the road map");
  }
  if (path != update_path) {
    delete path;
    _lock.lockWrite();
    _paths[agent->_id] = update_path;
    _lock.releaseWrite();
  }
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of RoadMapVCFactory
/////////////////////////////////////////////////////////////////////

RoadMapVCFactory::RoadMapVCFactory() : VelCompFactory() {
  _fileNameID = _attrSet.addStringAttribute("file_name", true /*required*/);
}

/////////////////////////////////////////////////////////////////////

bool RoadMapVCFactory::setFromXML(VelComponent* vc, TiXmlElement* node,
                                  const std::string& behaveFldr) const {
  RoadMapVelComponent* rmvc = dynamic_cast<RoadMapVelComponent*>(vc);
  assert(rmvc != 0x0 &&
         "Trying to set attributes of a road map velocity component on an incompatible "
         "object");

  if (!VelCompFactory::setFromXML(rmvc, node, behaveFldr)) return false;

  // get the file name
  std::string fName;
  std::string path = os::path::join(2, behaveFldr.c_str(), _attrSet.getString(_fileNameID).c_str());
  os::path::absPath(path, fName);
  GraphPtr gPtr;
  try {
    gPtr = loadGraph(fName);
  } catch (ResourceException) {
    logger << Logger::ERR_MSG;
    logger << "Couldn't instantiate the road map referenced on line ";
    logger << node->Row() << ".";
    return false;
  }
  rmvc->setRoadMap(gPtr);

  return true;
}

}  // namespace BFSM
}  // namespace Menge
