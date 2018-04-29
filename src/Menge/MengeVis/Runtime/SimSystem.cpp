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

#include "MengeVis/Runtime/SimSystem.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"
#include "MengeVis/Runtime/VisAgent/VisAgentDatabase.h"
#include "MengeVis/Runtime/VisObstacle.h"
#include "MengeVis/SceneGraph/GLScene.h"

#include <set>

namespace MengeVis {
namespace Runtime {

using Menge::logger;
using Menge::Logger;
using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Agents::SCBWriter;
using Menge::Agents::SimulatorInterface;
using Menge::Math::Vector2;
using Menge::Math::Vector3;
using SceneGraph::GLScene;
using SceneGraph::SystemStopException;

////////////////////////////////////////////////////////////////////////////
//      Implementation of SimSystem
////////////////////////////////////////////////////////////////////////////

SimSystem::SimSystem(SimulatorInterface* sim)
    : SceneGraph::System(), _sim(sim), _lastUpdate(0.f), _isRunning(true) {}

////////////////////////////////////////////////////////////////////////////

SimSystem::~SimSystem() {
  if (_sim) delete _sim;
}

////////////////////////////////////////////////////////////////////////////

bool SimSystem::updateScene(float time) {
  if (_sim->step()) {
    updateAgentPosition(static_cast<int>(_sim->getNumAgents()));
    return true;
  }
  throw SystemStopException();
}

////////////////////////////////////////////////////////////////////////////

void SimSystem::addObstacleToScene(GLScene* scene) {
  // TODO: If the bsptree (ObstacleKDTree.h) chops up the obstacles, this isn't doing the
  //    right thing.  Currently, the bsptree chops them
  //  THIS IS A HACK to address the issues of the ObstacleKDTree
  //    The right thing to do is modify things so that they are not chopped up.
  std::set<const Obstacle*> handled;
  const std::vector<Obstacle*>& obstacles = _sim->getSpatialQuery()->getObstacles();
  for (size_t o = 0; o < obstacles.size(); ++o) {
    const Obstacle* obst = obstacles[o];
    if (handled.find(obst) == handled.end()) {
      Vector2 p0a = obst->getP0();
      Vector2 p1a = obst->getP1();
      const Obstacle* next = obst->_nextObstacle;
      while (next && next->_unitDir * obst->_unitDir >= 0.99999f) {
        handled.insert(next);
        p1a.set(next->getP1());
        next = next->_nextObstacle;
      }
      Vector3 p0(p0a.x(), p0a.y(), _sim->getElevation(p0a));
      Vector3 p1(p1a.x(), p1a.y(), _sim->getElevation(p1a));
      VisObstacle* vo = new VisObstacle(p0, p1);

      scene->addNode(vo);
      handled.insert(obst);
    }
  }
}

////////////////////////////////////////////////////////////////////////////

void SimSystem::addAgentsToScene(GLScene* scene) {
  _visAgents = new VisAgent*[_sim->getNumAgents()];
  for (size_t a = 0; a < _sim->getNumAgents(); ++a) {
    BaseAgent* agt = _sim->getAgent(a);
    VisAgent* baseNode = VisAgentDB::getInstance(agt);
    VisAgent* agtNode = baseNode->moveToClone();
    float h = _sim->getElevation(agt);
    agtNode->setPosition(agt->_pos.x(), agt->_pos.y(), h);
    scene->addNode(agtNode);
    _visAgents[a] = agtNode;
  }
}

////////////////////////////////////////////////////////////////////////////

void SimSystem::populateScene(GLScene* scene) {
  assert(_sim != 0x0 && "Can't add SimSystem to scene when no simulator is connected");

  addAgentsToScene(scene);
  addObstacleToScene(scene);
}

////////////////////////////////////////////////////////////////////////////

void SimSystem::updateAgentPosition(int agtCount) {
#pragma omp parallel for
  for (int a = 0; a < agtCount; ++a) {
    const BaseAgent* agt = _visAgents[a]->getAgent();
    float h = _sim->getElevation(agt);
    _visAgents[a]->setPosition(agt->_pos.x(), agt->_pos.y(), h);
  }
}

////////////////////////////////////////////////////////////////////////////

size_t SimSystem::getAgentCount() const { return _sim->getNumAgents(); }

////////////////////////////////////////////////////////////////////////////
}  // namespace Runtime
}  // namespace MengeVis
