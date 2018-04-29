/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

#ifndef __CORE_PLUGIN_ENGINE_H__
#define __CORE_PLUGIN_ENGINE_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/BasePluginEngine.h"
#include "MengeCore/PluginEngine/Plugin.h"

namespace Menge {

// Forward declaration
class SimulatorDBEntry;
class SimulatorDB;

namespace BFSM {
class ActionFactory;
class ConditionFactory;
class TargetFactory;
class VelCompFactory;
class VelModFactory;
class TaskFactory;
class GoalFactory;
class GoalSelectorFactory;
}  // namespace BFSM

namespace Agents {
class ElevationFactory;
class SpatialQueryFactory;
class AgentGeneratorFactory;
class ProfileSelectorFactory;
class StateSelectorFactory;
class ObstacleSetFactory;
}  // namespace Agents

class EventTriggerFactory;
class EventEffectFactory;
class EventTargetFactory;

namespace PluginEngine {

/*!
 @brief    The definition of the plugin engine for Menge's core simulation functionality.
 */
class MENGE_API CorePluginEngine
    : public BasePluginEngine<CorePluginEngine, Plugin<CorePluginEngine> > {
 public:
  /*!
   @brief    Default constructor.

   @param    simDB    A simulator database for pedestrian model registration
   */
  CorePluginEngine(SimulatorDB* simDB);

  /*!
   @brief    Virtual destructor.
   */
  virtual ~CorePluginEngine();

  /*!
   @brief    Register a simulator database entry.

   @param    dbEntry    An instance of a model database entry to register.
   */
  void registerModelDBEntry(SimulatorDBEntry* dbEntry);

  /*!
   @brief    Register an ActionFactory.

   @param    factory    An instance of the ActionFactory to register
   */
  void registerActionFactory(BFSM::ActionFactory* factory);

  /*!
   @brief    Register a ConditionFactory.

   @param    factory    An instance of the ConditionFactory to register
   */
  void registerConditionFactory(BFSM::ConditionFactory* factory);

  /*!
   @brief    Register a TargetFactory.

   @param    factory    An instance of the TargetFactory to register
   */
  void registerTargetFactory(BFSM::TargetFactory* factory);

  /*!
   @brief    Register a VelCompFactory.

   @param    factory    An instance of the VelCompFactory to register
   */
  void registerVelCompFactory(BFSM::VelCompFactory* factory);

  /*!
   @brief    Register a VelModFactory.

   @param    factory    An instance of the VelModifier to register
   */
  void registerVelModFactory(BFSM::VelModFactory* factory);

  /*!
   @brief    Register a TaskFactory.

   @param    factory    An instance of the TaskFactory to register
   */
  void registerTaskFactory(BFSM::TaskFactory* factory);

  /*!
   @brief    Register a GoalFactory.

   @param    factory    An instance of the GoalFactory to register
   */
  void registerGoalFactory(BFSM::GoalFactory* factory);

  /*!
   @brief    Register a GoalSelectorFactory.

   @param    factory    An instance of the GoalSelectorFactory to register
   */
  void registerGoalSelectorFactory(BFSM::GoalSelectorFactory* factory);

  /*!
   @brief    Register an ElevationFactory.

   @param    factory    An instance of the ElevationFactory to register
   */
  void registerElevationFactory(Agents::ElevationFactory* factory);

  /*!
   @brief    Register an SpatialQueryFactory.

   @param    factory    An instance of the SpatialQueryFactory to register
   */
  void registerSpatialQueryFactory(Agents::SpatialQueryFactory* factory);

  /*!
   @brief    Register an AgentGeneratorFactory.

   @param    factory    An instance of the AgentGeneratorFactory to register
   */
  void registerAgentGeneratorFactory(Agents::AgentGeneratorFactory* factory);

  /*!
   @brief    Register an ObstacleSetFactory.

   @param    factory    An instance of the ObstacleSetFactory to register
   */
  void registerObstacleSetFactory(Agents::ObstacleSetFactory* factory);

  /*!
   @brief    Register an ProfileSelectorFactory.

   @param    factory    An instance of the ProfileSelectorFactory to register
   */
  void registerProfileSelectorFactory(Agents::ProfileSelectorFactory* factory);

  /*!
   @brief    Register an StateSelectorFactory.

   @param    factory    An instance of the StateSelectorFactory to register
   */
  void registerStateSelectorFactory(Agents::StateSelectorFactory* factory);

  /*!
   @brief    Register an EventEffectFactory.

   @param    factory    An instance of the EventEffectFactory to register
   */
  void registerEventEffectFactory(EventEffectFactory* factory);

  /*!
   @brief    Register an EventTriggerFactory.

   @param    factory    An instance of the EventTriggerFactory to register
   */
  void registerEventTriggerFactory(EventTriggerFactory* factory);

  /*!
   @brief    Register an EventTargetFactory.

   @param    factory    An instance of the EventTargetFactory to register
   */
  void registerEventTargetFactory(EventTargetFactory* factory);

 protected:
  /*!
   @brief    Provides the string that will be written to the logger as an info at the beginning of
            plugin loading.

   It should be specialized for each type of plugin engine.
   */
  virtual std::string getIntroMessage();

  /*!
   @brief    The simulator database for registration.
   */
  SimulatorDB* _simDB;
};

/////////////////////////////////////////////////////////////////////
//          Specialization of Plugin
/////////////////////////////////////////////////////////////////////

/*!
 @brief  Provides the plug-in method name for Menge *core* plug-ins.

 @returns  The name of the registration function.
 */
template <>
const char* Plugin<CorePluginEngine>::getRegisterName() const;

extern template class Plugin<CorePluginEngine>;
}  // namespace PluginEngine
}  // namespace Menge

#endif  // __CORE_PLUGIN_ENGINE_H__
