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

#ifndef __TASK_H__
#define __TASK_H__

/*!
 @file  Task.h
 @brief  Defines the interface for behavior FSM tasks --  actions that need to be taken at each time
        step.
 */
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/Element.h"

#include <string>

// forward declaration
class TiXmlElement;

namespace Menge {

namespace BFSM {

// FORWARD DECLARATIONS
class FSM;

/*!
 @brief    Exception thrown when a task fails at doing its work.

 These exceptions can be logged but should not arrest execution of the simulation.
 */
class TaskException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  TaskException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  TaskException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    Exception thrown when the task has an error which cannot be recovered from.
 */
class TaskFatalException : public TaskException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  TaskFatalException() : MengeException(), TaskException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  TaskFatalException(const std::string& s)
      : MengeException(s), TaskException(), MengeFatalException() {}
};

/*!
 @brief  Interface for basic FSM task.

 Tasks must implement three functions: doWork, toString, and isEquivalent. Tasks' doWork functions
 are evaluated at the beginning of each FSM evaluation per time step.
 */
class MENGE_API Task : public Element {
 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~Task() {}

 public:
  /*!
   @brief    The work performed by the task.

   @param    fsm    The behavior finite state machine on which the task is performed.

   @throws    TaskException if there was some non-fatal error in execution. It should be logged.
   @throws    TaskFatalException if there is a fatal error that should arrest execution of the
              simulation.
   */
  virtual void doWork(const FSM* fsm) throw(TaskException) = 0;

  /*!
   @brief    String representation of the task

   @returns  A string containing task information.
   */
  // TODO: Determine when this is used.
  virtual std::string toString() const = 0;

  /*!
   @brief    Reports if this task is "equivalent" to the given task.

   This makes it possible for a task to be redundantly added to the fsm without fear of duplication
   as the equivalent duplicates will be culled.

   @param      task    The task to test against this one.
   @returns    True if the two tasks are equivalent.
   */
  virtual bool isEquivalent(const Task* task) const = 0;
};

/*!
 @brief    Parses a TinyXML element containing a task specification

 @param    node          The TinyXML element
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                        relative to this folder.
 @returns  A pointer to the new task instance.
 */
Task* parseTask(TiXmlElement* node, const std::string& behaveFldr);
}  // namespace BFSM
}  // namespace Menge
#endif  // __TASK_H__
