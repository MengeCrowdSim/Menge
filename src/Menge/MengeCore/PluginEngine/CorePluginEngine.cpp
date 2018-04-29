#include "MengeCore/PluginEngine/CorePluginEngine.h"

#include "MengeCore/Agents/AgentGenerators/AgentGeneratorDatabase.h"
#include "MengeCore/Agents/Elevations/ElevationDatabase.h"
#include "MengeCore/Agents/Events/EventEffectDB.h"
#include "MengeCore/Agents/Events/EventTargetDB.h"
#include "MengeCore/Agents/Events/EventTriggerDB.h"
#include "MengeCore/Agents/ObstacleSets/ObstacleSetDatabase.h"
#include "MengeCore/Agents/ProfileSelectors/ProfileSelectorDatabase.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQueryDatabase.h"
#include "MengeCore/Agents/StateSelectors/StateSelectorDatabase.h"
#include "MengeCore/BFSM/Actions/ActionDatabase.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorDatabase.h"
#include "MengeCore/BFSM/Goals/GoalDatabase.h"
#include "MengeCore/BFSM/Tasks/TaskDatabase.h"
#include "MengeCore/BFSM/Transitions/ConditionDatabase.h"
#include "MengeCore/BFSM/Transitions/TargetDatabase.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentDatabase.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifierDatabase.h"
#include "MengeCore/Orca/ORCADBEntry.h"
#include "MengeCore/PedVO/PedVODBEntry.h"
#include "MengeCore/Runtime/SimulatorDB.h"

namespace Menge {
namespace PluginEngine {

/////////////////////////////////////////////////////////////////////
//          Implementation of CorePluginEngine
/////////////////////////////////////////////////////////////////////

CorePluginEngine::CorePluginEngine(SimulatorDB* simDB)
    : BasePluginEngine<CorePluginEngine, Plugin<CorePluginEngine> >(), _simDB(simDB) {
  registerModelDBEntry(new ORCA::DBEntry());
  registerModelDBEntry(new PedVO::DBEntry());
  BFSM::ActionDB::initialize();
  BFSM::ConditionDB::initialize();
  BFSM::TargetDB::initialize();
  BFSM::VelCompDB::initialize();
  BFSM::VelModDB::initialize();
  BFSM::TaskDB::initialize();
  BFSM::GoalDB::initialize();
  BFSM::GoalSelectorDB::initialize();
  Agents::ElevationDB::initialize();
  Agents::SpatialQueryDB::initialize();
  Agents::AgentGeneratorDB::initialize();
  Agents::ObstacleSetDB::initialize();
  Agents::ProfileSelectorDB::initialize();
  Agents::StateSelectorDB::initialize();
  EventEffectDB::initialize();
  EventTriggerDB::initialize();
  EventTargetDB::initialize();
}

/////////////////////////////////////////////////////////////////////

CorePluginEngine::~CorePluginEngine() {}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerModelDBEntry(SimulatorDBEntry* dbEntry) {
  _simDB->registerEntry(dbEntry);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerActionFactory(BFSM::ActionFactory* factory) {
  BFSM::ActionDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerConditionFactory(BFSM::ConditionFactory* factory) {
  BFSM::ConditionDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerTargetFactory(BFSM::TargetFactory* factory) {
  BFSM::TargetDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerVelCompFactory(BFSM::VelCompFactory* factory) {
  BFSM::VelCompDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerVelModFactory(BFSM::VelModFactory* factory) {
  BFSM::VelModDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerTaskFactory(BFSM::TaskFactory* factory) {
  BFSM::TaskDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerGoalFactory(BFSM::GoalFactory* factory) {
  BFSM::GoalDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerGoalSelectorFactory(BFSM::GoalSelectorFactory* factory) {
  BFSM::GoalSelectorDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerElevationFactory(Agents::ElevationFactory* factory) {
  Agents::ElevationDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerSpatialQueryFactory(Agents::SpatialQueryFactory* factory) {
  Agents::SpatialQueryDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerAgentGeneratorFactory(Agents::AgentGeneratorFactory* factory) {
  Agents::AgentGeneratorDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerObstacleSetFactory(Agents::ObstacleSetFactory* factory) {
  Agents::ObstacleSetDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerProfileSelectorFactory(Agents::ProfileSelectorFactory* factory) {
  Agents::ProfileSelectorDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerStateSelectorFactory(Agents::StateSelectorFactory* factory) {
  Agents::StateSelectorDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerEventEffectFactory(EventEffectFactory* factory) {
  EventEffectDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerEventTriggerFactory(EventTriggerFactory* factory) {
  EventTriggerDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

void CorePluginEngine::registerEventTargetFactory(EventTargetFactory* factory) {
  EventTargetDB::addFactory(factory);
}

/////////////////////////////////////////////////////////////////////

std::string CorePluginEngine::getIntroMessage() {
  return "Loading Menge core-simulation plugins...";
}

/////////////////////////////////////////////////////////////////////
//          Implementation of CorePluginEngine
/////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
const char* Plugin<CorePluginEngine>::getRegisterName() const {
  return "registerCorePlugin";
}
}
#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge