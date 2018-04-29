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
 @file    PropertyAction.h
 @brief    Defines a set of BFSM actions that change agent parameters.
 */

#ifndef __PROPERTY_ACTION_H__
#define __PROPERTY_ACTION_H__

#include "MengeCore/Agents/AgentPropertyManipulator.h"
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

////////////////////////////////////////////////////////////////////

/*!
 @brief    The base class for modifying agent properties.

 This is an abstract class and must be sub-classed. To create different types of property actions,
 simply specialize this templated class with a different type of AgentPropertyManipulator.
 */
template <class Manipulator>
class MENGE_API PropertyAction : public Action {
 public:
  /*!
   @brief    Constructor
   */
  PropertyAction() : Action(), _manip() {}

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~PropertyAction() {}

 public:
  /*!
   @brief    Upon entering the state, this is called -- it is the main work of the action.

   @param    agent    The agent to act on.
   */
  virtual void onEnter(Agents::BaseAgent* agent) { _manip.manipulate(agent); }

  /*!
   @brief    Returns a pointer to the manipulator.
   */
  Manipulator* getManipulator() { return &_manip; }

 protected:
  /*!
   @brief    The work to do upon state exit.

   @param    agent    The agent to act on.
   */
  virtual void resetAction(Agents::BaseAgent* agent) { _manip.restore(agent); }

  /*!
   @brief    The manipulator responsible for changing agent properties.
   */
  Manipulator _manip;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the PropertyAction.

 This is still an abstract class because it doesn't define the name or description. Nor does it
 define instance.
 */
template <class Manipulator>
class MENGE_API PropertyActFactory : public ActionFactory {
 public:
  /*!
   @brief    Constructor.
   */
  PropertyActFactory() {
    _propertyID = _attrSet.addStringAttribute("property", true /*required*/);
    _generatorID = _attrSet.addFloatDistAttribute("", true /*required*/, 0.f, 1.f);
  }

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
  virtual bool setFromXML(Action* action, TiXmlElement* node, const std::string& behaveFldr) const {
    PropertyAction<Manipulator>* pAction = dynamic_cast<PropertyAction<Manipulator>*>(action);
    assert(pAction != 0x0 && "Trying to set property action properties on an incompatible object");

    if (!ActionFactory::setFromXML(action, node, behaveFldr)) return false;
    Menge::AgentPropertyManipulator* manip = pAction->getManipulator();

    PropertyOperand prop = Menge::parsePropertyName(_attrSet.getString(_propertyID));
    manip->setProperty(prop);
    if (prop == NO_PROPERTY) {
      logger << Logger::ERR_MSG << "The property action defined on line ";
      logger << node->Row();
      logger << " specified an invalid value for the \"property\" attribute";
      return false;
    }
    manip->setGenerator(_attrSet.getFloatGenerator(_generatorID));

    return true;
  }

  /*!
   @brief    The identifier for the "property" string attribute.
   */
  size_t _propertyID;

  /*!
   @brief    The identifier for the float distribution attribute.
   */
  size_t _generatorID;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the SetPropertyAction.
 */
class MENGE_API SetPropertyActFactory : public PropertyActFactory<Menge::SetPropertyManipulator> {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "set_property"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Causes the specified property to be *replaced* by the user-defined value";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new PropertyAction<Menge::SetPropertyManipulator>(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the OffsetPropertyAction.
 */
class MENGE_API OffsetPropertyActFactory
    : public PropertyActFactory<Menge::OffsetPropertyManipulator> {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "offset_property"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Adds the user-defined value into the agent's specified property";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new PropertyAction<Menge::OffsetPropertyManipulator>(); }
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ScalePropertyAction.
 */
class MENGE_API ScalePropertyActFactory
    : public PropertyActFactory<Menge::ScalePropertyManipulator> {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "scale_property"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "Scales the user-defined value into the agent's specified property";
  };

 protected:
  /*!
   @brief    Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML. The caller of this function takes ownership of the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  Action* instance() const { return new PropertyAction<Menge::ScalePropertyManipulator>(); }
};

}  //  namespace BFSM
}  // namespace Menge

#endif  // __PROPERTY_ACTION_H__
