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
 @file    TeleportAction.h
 @brief    Defines a BFSM action that causes agents to teleport to a new location.
 */

#ifndef __TELEPORT_ACTION_H__
#define __TELEPORT_ACTION_H__

#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

// forward declaration

namespace Agents {
class BaseAgent;
}

namespace BFSM {
// forward declaration
class TeleportActFactory;

/*!
 @brief    Causes the agents to teleport to a new location.

 This action does *not* undo the teleport on leaving the state. It is hard-wired this way.
 */
class MENGE_API TeleportAction : public Action {
 public:
  /*!
   @brief    Constructor

   */
  TeleportAction();

  /*!
   @brief    Virtual destructor.
   */
  virtual ~TeleportAction();

  /*!
   @brief    Upon entering the state, this is called -- it is the main work of the action.

   @param    agent    The agent to act on.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  friend class TeleportActFactory;

 protected:
  /*!
   @brief    The generator for computing teleport destination locations.
   */
  Vec2DGenerator* _goals;
};

/*!
 @brief    Factory for the TeleportAction.
 */
class MENGE_API TeleportActFactory : public ActionFactory {
 public:
  /*!
   @brief    Constructor.
   */
  TeleportActFactory();

  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "teleport"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Causes the agent to teleport to a user-specified location based on a 2D "
           "generator";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const;

  /*!
   @brief    Given a pointer to an Action instance, sets the appropriate fields from the provided XML
            node.

   It is assumed that the value of the `type` attribute is this Action's type (i.e. 
   ActionFactory::thisFactory has already been called and returned true). If sub-classes of
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param    action      A pointer to the action whose attributes are to be set.
   @param    node        The XML node containing the action attributes.
   @param    behaveFldr  The path to the behavior file.  If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Action* action, TiXmlElement* node, const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "exit_reset" boolean attribute.
   */
  size_t _2DGoalID;
};

}  //  namespace BFSM
}  // namespace Menge
#endif  // __TELEPORT_ACTION_H__
