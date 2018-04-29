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
 @file    ActionFactory.h
 @brief    The factory for parsing xml data and instantiating actions.
 */

#ifndef __ACTION_FACTORY_H__
#define __ACTION_FACTORY_H__

#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

#include "thirdParty/tinyxml.h"

#include <string>

namespace Menge {

namespace BFSM {
/*!
 @brief    A class for parsing the xml description of an action and instantiating particular 
          instances.
 */
class MENGE_API ActionFactory : public ElementFactory<Action> {
 public:
  /*!
   @brief    Constructor.
   */
  ActionFactory();

 protected:
  /*!
   @brief   Given a pointer to an Action instance, sets the appropriate fields from the provided 
            XML node.

   It is assumed that the value of the `type` attribute is this Action's type. (i.e. 
   ActionFactory::thisFactory has already been called and returned true.) If sub-classes of 
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param    action       A pointer to the action whose attributes are to be set.
   @param    node         The XML node containing the action attributes.
   @param    behaveFldr   The path to the behavior file.  If the action references resources in the
                          file system, it should be defined relative to the behavior file location.
                          This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Action* action, TiXmlElement* node, const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "exit_reset" boolean attribute.
   */
  size_t _exitResetID;
};
}  // namespace BFSM
}  // namespace Menge

#endif  // __ACTION_FACTORY_H__
