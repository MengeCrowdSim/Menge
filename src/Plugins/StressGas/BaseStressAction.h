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
#ifndef __BASE_STRESS_ACTION_H__
#define __BASE_STRESS_ACTION_H__

#include "StressFunction.h"
#include "StressGasConfig.h"

#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"

// forward declaration
class TiXmlElement;
namespace Menge {
namespace Math {
class FloatGenerator;
}
}  // namespace Menge

namespace StressGAS {
// forward declaration
class BaseStressActionFactory;

/*!
 @enum	ExitBehavior

 @brief	The various actions the action can take when the agent leaves the BFSM state.
 */
enum ExitBehavior {
  RESET,      ///< Traditional action; original values reset and stress stops.
  COOL_DOWN,  ///< Accumulation stops, cool down starts.  Stressor over at end of cool down.
  PAUSE,      ///< Stress levels maintained, no new accumulation.
  CONTINUE    ///< Stress continues even after exit
};

/*!
 @brief		The base class for all stress actions.

 It provides the core functionality for defining the stress vector (the offset to agent parameters
 which represent a change to 100% stressed). Handles the initial registration with the stress
 manager.

 The stress action's basic behavior is to initialize stress accumulation upon entering the action's
 corresponding state and control how the stress accumulation changes when the agent exits the state.

 The nature of the stress accumulation is dicatated by creating a subclass of this method and
 overriding the makeStressFunction. A StressFunction is responsible for mapping agent state to a
 stress level - a value in the range [0, 1]. This is interpreted as ranging between no stress and
 being "fully" stressed. It is invoked at each time step. The formula for computing instantaneous
 stress can use arbitrary logic.

 Upon exiting the state, the action can specify what happens to the stress accumulation when the
 agent exits the state. There are for options:

   - "reset" - this reverts the agent to its unstressed state. This is analgous to the basic Action
      behavior "exit_reset". However, the stress action *ignores* the "exit_reset" parameter and
      uses the exit behavior field instead.
   - "pause" - the current stress level is preserved but will not change.
   - "cool down" - the stress level will decrease at a xml-specified rate.
   - "continue" - continue accumluating stress into subsequent states.

 Regardless of what the specified exit behavior is, if the agent enters a state with a stress
 action, that will replace the current stress accumulator; however, the agent's stress level will be
 preserved across the change.

 XML example:

 ```xml
 <Action type="..."
    exit_behavior={"reset"|"continue"|"pause"|"cool down"}
    cool_duration_dist="c|n|u" ...
    neighbor_dist_dist="c|n|u" ...
    max_neighbors_dist="c|n|u" ...
    radius_dist="c|n|u" ...
    pref_speed_dist="c|n|u" ...
    time_horizon_dist="c|n|u" ...
 />
 ```
 
    - `exit_behavior` defines the state-exiting behavior and is required.
    - `cool_duration_dist` dictates the amount of time it would take the agent to cool down from a
      100% stress level.  It has a default value and, therefore, is optional.
    - `neighbor_dist_dst`, `max_neighbors_dist`, `radius_dist`, `pref_speed_dist`,
      `time_horizon_dist` define the changes to the agent parameters as stress increases. The
      semantics are defined in @see StressFunction. All have constant distribution default values.
      However, arbitrary distributions can be provided to vary the stress behaviors for agents who
      enter the state.
 
 All of the values (except for `exit_behavior`) are numerical distribution parameters. If omitted,
 they apply a constant distribution of a default value across all agents. Otherwise, other
 distributions can be indicated as defined by Menge::Math::FloatGenerator.
 @see Menge::Math::FloatGenerator

 Individual sub-clases can include additional parameters.
 */
class EXPORT_API BaseStressAction : public Menge::BFSM::Action {
 public:
  /*! The default time to cool down from 100% stress. */
  const static float DEFAULT_COOL_DURATION;

  /*!
   @brief		Constructor
   */
  BaseStressAction()
      : Menge::BFSM::Action(),
        _exitBehavior(RESET),
        _deltaNeighborDist(0x0),
        _deltaMaxNeighbors(0x0),
        _deltaRadius(0x0),
        _deltaTimeHorizon(0x0),
        _deltaPrefSpeed(0x0),
        _coolDuration(0x0) {}

  /*!
   @brief		Virtual destructor.
   */
  ~BaseStressAction();

  /*!
   @brief		Returns the stress function for this stress action.

   Stress actions vary in the stress function they are associated with. Each sub-class must define
   this method to provide the right type of StressFunction.

   @param	agent			    The agent to compute stress for.
   @param	stressor		  The stressor for the StressFunction to use.
   @param	coolDuration	The amount of simulation time required to cool down from full stress to no
                        stress (in simulation seconds).
   @returns	An instance of the appropriate stress function.
   */
  virtual StressFunction* makeStressFunction(Menge::Agents::BaseAgent* agent,
                                             AgentStressor* stressor, float coolDuration) = 0;

  /*!
   @brief		Upon entering the state, this is called -- it is the main work of the action.

   @param		agent		The agent to act on.
   */
  void onEnter(Menge::Agents::BaseAgent* agent);

  /*!
   @brief		Returns the task for managing stress accumulation.

   @returns   The StressTask instance
   */
  virtual Menge::BFSM::Task* getTask();

  friend class BaseStressActionFactory;

 protected:
  /*!
   @brief		The work to do upon state exit.

   @param		agent		The agent to act on.
   */
  void leaveAction(Menge::Agents::BaseAgent* agent);

  /*! @brief	The exit behavior the action takes. */
  ExitBehavior _exitBehavior;

  /*! @brief	The value for the change in neighbor distance. */
  Menge::Math::FloatGenerator* _deltaNeighborDist;

  /*! @brief	The value for the change in maximum neighbors. */
  Menge::Math::FloatGenerator* _deltaMaxNeighbors;

  /*! @brief	The value for the change in radius. */
  Menge::Math::FloatGenerator* _deltaRadius;

  /*! @brief	The value for the change in time horizon. */
  Menge::Math::FloatGenerator* _deltaTimeHorizon;

  /*! @brief	The value for the change in preference speed. */
  Menge::Math::FloatGenerator* _deltaPrefSpeed;

  /*! @brief  The value for the cool down duration. */
  Menge::Math::FloatGenerator* _coolDuration;

 protected:
};

/*!
 @brief		Factory for instantiating BaseStressAction instances.
 */
class EXPORT_API BaseStressActionFactory : public Menge::BFSM::ActionFactory {
 public:
  /*!
   @brief	Default constructor.
   */
  BaseStressActionFactory();

 protected:
  /*!
   @brief		Given a pointer to an Action instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Action's type (i.e.
   ActionFactory::thisFactory has already been called and returned true). If sub-classes of
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param		action		  A pointer to the action whose attributes are to be set.
   @param		node		    The XML node containing the action attributes.
   @param		behaveFldr	The path to the behavior file. If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns	A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Menge::BFSM::Action* action, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /** @brief	Identifier for the neighbor distance change float attribute. */
  size_t _neighborDistId;

  /** @brief	Identifier for the maximum neighbor change int attribute. */
  size_t _maxNeighborsId;

  /** @brief	Identifier for the radius change float attribute. */
  size_t _radiusId;

  /** @brief	Identifier for the preference speed change float attribute. */
  size_t _prefSpeedId;

  /** @brief	Identifier for the time horizon change float attribute; only affects
   ORCA-related agents. */
  size_t _timeHorizonId;

  /** @brief  Identifier for the exit behavior of the stress action */
  size_t _exitBehaviorId;

  /** @brief  Identifier for the cool down duration of the stress action */
  size_t _coolDurationId;
};
}  // namespace StressGAS

#endif  // __BASE_STRESS_ACTION_H__
