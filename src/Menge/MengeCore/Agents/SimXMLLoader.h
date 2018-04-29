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

#ifndef __SIM_XML_LOADER__
#define __SIM_XML_LOADER__

/*!
 @file       SimXMLLoader.h
 @brief      Contains functionality for parsing simulation specifcation from an XML file.
 */

#include "MengeCore/mengeCommon.h"

#include "tinyxml/tinyxml.h"

#include <string>

namespace Menge {

namespace Agents {

// Forward declartion
class XMLSimulatorBase;
class AgentInitializer;

/*!
 @brief    Class for parsing the SCENE XML specification and initialize a simulator.

 It only knows the most fundamental common aspects of the file, relying on the Simulator to know of
 its own specific domain knowledge.
 */
class MENGE_API SimXMLLoader {
 public:
  /*!
   @brief    Constructor

   @param    sim    The simulator to populate based on definition in XML.
   */
  SimXMLLoader(XMLSimulatorBase* sim);

  /*!
   @brief    Parse the xml and load the given simulator.

   @param    xmlName      The path to the xml file simulation specification.
   @param    agentInit    The AgentInitializer necessary to parse AgentSet properties
   @param    verbose      Determines if parsing outputs progress information (true) or silently
                        (false).
   @returns  True if the simulation is successfully loaded, false otherwise.
   */
  bool loadFromXML(const std::string& xmlName, AgentInitializer* agentInit, bool verbose = false);

 protected:
  /*!
   @brief    The folder in which the scene configuration file appears
   */
  std::string _sceneFldr;

  /*!
   @brief    The simulator to populate based on the contents of the xml.
   */
  XMLSimulatorBase* _sim;

  /*!
   @brief    Parses the definition of an AgentGroup.

   @param    node        A pointer to the XML node containing the definition.
   @param    agentInit    The AgentInitializer necessary to parse AgentProfile properties.
   @returns  A boolean reporting success (true) or failure (false).
   */
  bool parseAgentGroup(TiXmlElement* node, AgentInitializer* agentInit);

  /*!
   @brief    Parses the definition of an obstacleset.

   @param    node    A pointer to the XML node containing the obstacleset.
   @returns  A boolean reporting success (true) or failure (false).
   */
  bool parseObstacleSet(TiXmlElement* node);

  /*!
   @brief    Parses the definition of an agent profile.

   @param    node        A pointer to the XML node containing the definition.
   @param    agentInit    The AgentInitializer necessary to parse AgentProfile properties.
   @returns  A boolean reporting success (true) or failure (false).
   */
  bool parseAgentProfile(TiXmlElement* node, AgentInitializer* agentInit);

  /*!
   @brief    The number of agents loaded.
   */
  unsigned int _agtCount;

  /*!
   @brief    Mapping from agent profile name to agent initializer.
   */
  HASH_MAP<std::string, AgentInitializer*> _profiles;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __SIM_XML_LOADER__
