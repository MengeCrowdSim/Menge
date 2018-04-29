#include "MengeCore/menge_c_api.h"

#include <algorithm>

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"
#include "MengeCore/Core.h"
#include "MengeCore/PluginEngine/CorePluginEngine.h"
#include "MengeCore/Runtime/SimulatorDB.h"

/////////////////////////////////////////////////////////////////////
//          Local Variables
/////////////////////////////////////////////////////////////////////

Menge::Agents::SimulatorInterface* _simulator = 0x0;

/////////////////////////////////////////////////////////////////////
//          API implementation
/////////////////////////////////////////////////////////////////////

extern "C" {

using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Math::Vector2;
using std::find;

bool InitSimulator(const char* behaveFile, const char* sceneFile, const char* model,
                   const char* pluginPath) {
  const bool VERBOSE = false;
  if (_simulator != 0x0) delete _simulator;
  Menge::SimulatorDB simDB;
  // TODO: Plugin engine is *not* public.  I can't get plugins.
  Menge::PluginEngine::CorePluginEngine engine(&simDB);
  if (pluginPath != 0x0) {
    engine.loadPlugins(pluginPath);
  }
  Menge::SimulatorDBEntry* simDBEntry = simDB.getDBEntry(std::string(model));
  if (simDBEntry == 0x0) return false;

  size_t agentCount;
  float timeStep = 0.1f;        // Default to 10Hz
  int subSteps = 0;             // take no sub steps
  float duration = 1e6;         // effectively no simulation duration.
  std::string outFile = "";     // Don't write an scb file.
  std::string scbVersion = "";  // No scb version
  bool verbose = false;
  _simulator = simDBEntry->getSimulator(agentCount, timeStep, subSteps, duration, behaveFile,
                                        sceneFile, outFile, scbVersion, verbose);
  return _simulator != 0x0;
}

/////////////////////////////////////////////////////////////////////

void SetTimeStep(float timeStep) {
  assert(_simulator != 0x0);
  _simulator->setTimeStep(timeStep);
}

/////////////////////////////////////////////////////////////////////

bool DoStep() {
  assert(_simulator != 0x0);
  return _simulator->step();
}

/////////////////////////////////////////////////////////////////////

const char* GetStateName(size_t state_id) {
  assert(_simulator != nullptr);
  Menge::BFSM::FSM* bfsm = _simulator->getBFSM();
  if (state_id < bfsm->getNodeCount()) {
    Menge::BFSM::State* state = bfsm->getNode(state_id);
    return state->getName().c_str();
  }
  return nullptr;
}

/////////////////////////////////////////////////////////////////////

size_t StateCount() {
  assert(_simulator != nullptr);
  Menge::BFSM::FSM* bfsm = _simulator->getBFSM();
  return bfsm->getNodeCount();
}

/////////////////////////////////////////////////////////////////////

size_t AgentCount() {
  assert(_simulator != 0x0);
  return _simulator->getNumAgents();
}

/////////////////////////////////////////////////////////////////////

bool GetAgentPosition(size_t i, float* x, float* y, float* z) {
  assert(_simulator != 0x0);
  BaseAgent* agt = _simulator->getAgent(i);
  if (agt != 0x0) {
    *x = agt->_pos._x;
    *y = _simulator->getElevation(agt);
    *z = agt->_pos._y;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

bool GetAgentVelocity(size_t i, float* x, float* y, float* z) {
  assert(_simulator != 0x0);
  BaseAgent* agt = _simulator->getAgent(i);
  if (agt != 0x0) {
    *x = agt->_vel._x;
    *y = 0;  // get elevation
    *z = agt->_vel._y;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

bool GetAgentPrefVelocity(size_t i, float* x, float* y) {
  assert(_simulator != nullptr);
  Menge::Agents::BaseAgent* agt = _simulator->getAgent(i);
  if (agt != nullptr) {
    const auto& vel_pref = agt->_velPref.getPreferredVel();
    *x = vel_pref._x;
    *y = vel_pref._y;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

bool GetAgentState(size_t i, size_t* state_id) {
  assert(_simulator != nullptr);
  Menge::Agents::BaseAgent* agt = _simulator->getAgent(i);
  if (agt != nullptr) {
    const auto* bfsm = _simulator->getBFSM();
    *state_id = bfsm->getAgentStateID(agt->_id);
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

bool GetAgentOrient(size_t i, float* x, float* y) {
  assert(_simulator != 0x0);
  BaseAgent* agt = _simulator->getAgent(i);
  if (agt != 0x0) {
    *x = agt->_orient._x;
    *y = agt->_orient._y;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////

int GetAgentClass(size_t i) {
  assert(_simulator != 0x0);
  BaseAgent* agt = _simulator->getAgent(i);
  if (agt != 0x0) {
    return static_cast<int>(agt->_class);
  }
  return -1;
}

/////////////////////////////////////////////////////////////////////

float GetAgentRadius(size_t i) {
  assert(_simulator != 0x0);
  BaseAgent* agt = _simulator->getAgent(i);
  if (agt != 0x0) {
    return agt->_radius;
  }
  return -1;
}

/////////////////////////////////////////////////////////////////////

std::vector<std::string> triggers;
bool triggersValid = false;

/////////////////////////////////////////////////////////////////////

int ExternalTriggerCount() {
  if (!triggersValid) triggers = Menge::EVENT_SYSTEM->listExternalTriggers();
  return static_cast<int>(triggers.size());
}

/////////////////////////////////////////////////////////////////////

const char* ExternalTriggerName(int i) {
  if (i < ExternalTriggerCount()) {
    return triggers[i].c_str();
  }
  return nullptr;
}

/////////////////////////////////////////////////////////////////////

void FireExternalTrigger(const char* triggerName) {
  Menge::EVENT_SYSTEM->activateExternalTrigger(triggerName);
}

/////////////////////////////////////////////////////////////////////

size_t ObstacleCount() {
  assert(_simulator != 0x0);
  return _simulator->getSpatialQuery()->getObstacles().size();
}

/////////////////////////////////////////////////////////////////////

size_t GetNextObstacle(size_t i) {
  assert(_simulator != 0x0);
  const std::vector<Obstacle*>& obstacles = _simulator->getSpatialQuery()->getObstacles();
  assert(i < obstacles.size());
  const Obstacle* queryObstacle = obstacles[i];
  const Obstacle* nextObstacle = queryObstacle->next();
  std::vector<Obstacle*>::const_iterator itr =
      find(obstacles.begin(), obstacles.end(), nextObstacle);
  assert(itr != obstacles.end());
  return itr - obstacles.begin();
}

/////////////////////////////////////////////////////////////////////

bool GetObstacleEndPoints(size_t i, float* x0, float* y0, float* z0, float* x1, float* y1,
                          float* z1) {
  assert(_simulator != 0x0);
  const std::vector<Obstacle*>& obstacles = _simulator->getSpatialQuery()->getObstacles();
  assert(i < obstacles.size());
  const Obstacle* queryObstacle = obstacles[i];
  const Vector2& p0 = queryObstacle->getP0();
  *x0 = p0._x;
  *y0 = 0.0;  // TODO: Use elevation to set this more intelligently.
  *z0 = p0._y;
  const Vector2& p1 = queryObstacle->getP1();
  *x1 = p1._x;
  *y1 = 0.0;  // TODO: Use elevation to set this more intelligently.
  *z1 = p1._y;
  return true;
}

/////////////////////////////////////////////////////////////////////

bool GetObstacleP0(size_t i, float* x0, float* y0, float* z0) {
  assert(_simulator != 0x0);
  const std::vector<Obstacle*>& obstacles = _simulator->getSpatialQuery()->getObstacles();
  assert(i < obstacles.size());
  const Obstacle* queryObstacle = obstacles[i];
  const Vector2& p0 = queryObstacle->getP0();
  *x0 = p0._x;
  *y0 = 0.0;  // TODO: Use elevation to set this more intelligently.
  *z0 = p0._y;
  return true;
}

/////////////////////////////////////////////////////////////////////

bool GetObstacleP1(size_t i, float* x1, float* y1, float* z1) {
  assert(_simulator != 0x0);
  const std::vector<Obstacle*>& obstacles = _simulator->getSpatialQuery()->getObstacles();
  assert(i < obstacles.size());
  const Obstacle* queryObstacle = obstacles[i];
  const Vector2& p1 = queryObstacle->getP1();
  *x1 = p1._x;
  *y1 = 0.0;  // TODO: Use elevation to set this more intelligently.
  *z1 = p1._y;
  return true;
}
}  // extern"C"