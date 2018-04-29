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

/*!
 @file    ObstacleAction.h
 @brief    Defines a set of BFSM actions that change agent obstacle set value.
 */

#ifndef __OBSTACLE_ACTION_H__
#define __OBSTACLE_ACTION_H__

#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"
#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Runtime/SimpleLock.h"

#include <map>

// forward declaration

namespace Menge {

namespace Agents {
class BaseAgent;
}

namespace BFSM {
// forward declaration
class ObstacleActFactory;

/*!
 @brief    The base class for modifying agent obstacle sets.

 This is an abstract class and must be sub-classed.
 */
class MENGE_API ObstacleAction : public Action {
 public:
  /*!
   @brief    Constructor
   */
  ObstacleAction();

  /*!
   @brief    Virtual destructor.
   */
  virtual ~ObstacleAction();

  /*!
   @brief    Upon entering the state, this is called -- it is the main work of the action.

   @param    agent    The agent to act on.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  friend class ObstacleActFactory;

 protected:
  /*!
   @brief    The work to do upon state exit.

   @param    agent    The agent to act on.
   */
  virtual void resetAction(Agents::BaseAgent* agent);

  /*!
   @brief    Computes the new property value given the original property value.

   @param    value    The original obstacle set value.
   @returns  The new value.
   */
  virtual size_t newValue(size_t value) = 0;

  /*!
   @brief    The set operand to apply to the agents obstacle set.
   */
  size_t _setOperand;

  /*!
   @brief    A mapping from agent id to the agent's obstacle set value before the action was applied.
   */
  std::map<size_t, size_t> _originalMap;

  /*!
   @brief    Lock for guaranteeing thread-safety.
   */
  SimpleLock _lock;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ObstacleAction.

 This is still an abstract class because it doesn't define the name or description. Nor does it
 define instance.
 */
class MENGE_API ObstacleActFactory : public ActionFactory {
 public:
  /*!
   @brief    Constructor.
   */
  ObstacleActFactory();

 protected:
  /*!
   @brief    Given a pointer to an Action instance, sets the appropriate fields from the provided XML
            node.

   It is assumed that the value of the `type` attribute is this Action's type (i.e.
   ActionFactory::thisFactory has already been called and returned true). If sub-classes of
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param    action      A pointer to the action whose attributes are to be set.
   @param    node        The XML node containing the action attributes.
   @param    behaveFldr  The path to the behavior file. If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Action* action, TiXmlElement* node, const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "operand" size_t attribute.
   */
  size_t _operandID;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief  Modifies the agent's obstacle set by REMOVING the given obstacle set value.
 */
class MENGE_API RemoveObstacleSetAction : public ObstacleAction {
 public:
  /*!
   @brief    Computes the new property value given the original property value.

   @param    value    The original obstacle set value.
   @returns  The new value.
   */
  virtual size_t newValue(size_t value);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the RemoveObstacleSetAction.
 */
class MENGE_API RemoveObstacleActFactory : public ObstacleActFactory {
 public:
  /*!
   @brief    Constructor.
   */
  RemoveObstacleActFactory() : ObstacleActFactory() {}

  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "remove_obstacle"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Removes the specified obstacle set from the agents consideration";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new RemoveObstacleSetAction(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief  Modifies the agent's obstacle set by ADDING the given obstacle set value.
 */
class MENGE_API AddObstacleSetAction : public ObstacleAction {
 public:
  /*!
   @brief    Computes the new property value given the original property value.

   @param    value    The original obstacle set value.
   @returns  The new value.
   */
  virtual size_t newValue(size_t value);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ObstacleActFactory.
 */
class MENGE_API AddObstacleyActFactory : public ObstacleActFactory {
 public:
  /*!
   @brief    Constructor.
   */
  AddObstacleyActFactory() : ObstacleActFactory() {}

  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "add_obstacle"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Adds the specified obstacle set from the agents consideration";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new AddObstacleSetAction(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Modifies the agent's obstacle set by SETTING the given obstacle set value (i.e. overriding
          the old value with the new).
 */
class MENGE_API SetObstacleSetAction : public ObstacleAction {
 public:
  /*!
   @brief    Computes the new property value given the original property value.

   @param    value    The original obstacle set value.
   @returns  The new value.
   */
  virtual size_t newValue(size_t value);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the SetObstacleSetAction.
 */
class MENGE_API SetObstacleActFactory : public ObstacleActFactory {
 public:
  /*!
   @brief    Constructor.
   */
  SetObstacleActFactory() : ObstacleActFactory() {}

  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "set_obstacle"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Sets the specified obstacle set from the agents consideration";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new SetObstacleSetAction(); }
};

}  //  namespace BFSM

}  // namespace Menge
#endif  // __OBSTACLE_ACTION_H__
