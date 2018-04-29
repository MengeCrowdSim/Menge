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

#ifndef __NAV_MESH_LOCALIZER_TASK_H__
#define __NAV_MESH_LOCALIZER_TASK_H__

/*!
 @file  NavMeshLocalizerTask.h
 @brief  A task based on the NavMeshLocalizer so that it updates its tracked agent positions at every
        FSM time step.
 */

#include "MengeCore/BFSM/Tasks/Task.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/resources/Resource.h"

namespace Menge {

// forward declaration
class NavMeshLocalizer;

/*!
 @brief forward declaration. See NavMeshLocalizer for more details
 */
typedef ResourcePtr<NavMeshLocalizer> NavMeshLocalizerPtr;

namespace BFSM {
/*!
 @brief  Sub-class of NavMeshLocalizer that can be run as an FSM task
 */
class MENGE_API NavMeshLocalizerTask : public Task {
 public:
  /*!
   @brief    Constructor from a localizer.

   @param    navMeshName    The name of the navigation mesh which the task depends on.
   @param    usePlanner    Indicates if the localizer should use a planner (true) or not (false).
   */
  NavMeshLocalizerTask(const std::string& navMeshName, bool usePlanner);

  /*!
   @brief    The work performed by the task.

   @param    fsm    The finite state machine for the task to operate on.
   @throws    A TaskException if there was some non-fatal error in execution. It should be logged.
   @throws    A TaskFatalException if there is a fatal error that should arrest execution of the
              simulation.
   */
  virtual void doWork(const FSM* fsm) throw(TaskException);

  /*!
   @brief    String representation of the task

   @returns  A string containing task information.
   */
  virtual std::string toString() const;

  /*!
   @brief    Reports if this task is "equivalent" to the given task.

   This makes it possible for a task to be redundantly added to the fsm without fear of duplication
   as the equivalent duplicates will be culled.

   @param    task    The task to test against this one.
   @returns  A boolean reporting if the two tasks are equivalent (true) or unique (false).
   */
  virtual bool isEquivalent(const Task* task) const;

 protected:
  /*!
   @brief    The localizer used by this task.
   */
  NavMeshLocalizerPtr _localizer;
};
}  // namespace BFSM
}  // namespace Menge
#endif  // __NAV_MESH_LOCALIZER_TASK_H__
