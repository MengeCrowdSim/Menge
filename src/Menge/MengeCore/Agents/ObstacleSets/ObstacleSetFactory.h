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
 @file    ObstacleSetFactory.h
 @brief    The factory for parsing xml data and instantiating ObstacleSet implementations.
 */

#ifndef __OBSTACLE_SET_FACTORY_H__
#define __OBSTACLE_SET_FACTORY_H__

#include "MengeCore/Agents/ObstacleSets/ObstacleSet.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

namespace Menge {

namespace Agents {

/*!
 @brief    A class for parsing the xml description of an agent generator and instantiating particular
          instances.
 */
class MENGE_API ObstacleSetFactory : public ElementFactory<ObstacleSet> {
 public:
  /*
   @brief    default constructor
   */
  ObstacleSetFactory();

 protected:
  /*!
   @brief    Given a pointer to an ObstacleSet instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this ObstacleSet's type (i.e.
   ObstacleSetFactory::thisFactory has already been called and returned true). If sub-classes of
   ObstacleSetFactory introduce *new* ObstacleSet parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    gen        A pointer to the ObstacleSet whose attributes are to be set.
   @param    node      The XML node containing the ObstacleSet attributes.
   @param    specFldr  The path to the specification file. If the ObstacleSet references resources in
                      the file system, it should be defined relative to the specification file
                      location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(ObstacleSet* gen, TiXmlElement* node, const std::string& specFldr) const;

  /*!
   @brief    The identifier for the "class" size_t parameter.
   */
  size_t _classID;
};
}  // namespace Agents
}  // namespace Menge

#endif  //__OBSTACLE_SET_FACTORY_H__
