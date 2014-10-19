/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#ifndef __SIM_XML_LOADER__
#define __SIM_XML_LOADER__

/*!
 *  @file       SimXMLLoader.h
 *  @brief      Contains functionality for parsing simulation specifcation
 *				from an XML file.
 */

#include "mengeCommon.h"
#include <string>
#include "tinyxml/tinyxml.h"

namespace Menge {

	namespace Agents {

		// Forward declartion
		class XMLSimulatorBase;
		class AgentInitializer;

		/*!
		 *	@brief		Class for parsing the SCENE XML specification and initialize a simulator
		 *				It only knows the most fundamental common aspects of the file, relying on the
		 *				Simulator to know of its own specific domain knowledge.
		 */
		class MENGE_API SimXMLLoader {
		public:
			/*!
			 *	@brief		Constructor
			 *
			 *	@param		sim		The simulator to populate based on definition in XML.
			 */
			SimXMLLoader( XMLSimulatorBase * sim );
			
			/*!
			 *	@brief		Parse the xml and load the given simulator.
			 *
			 *	@param		xmlName		The path to the xml file simulation specification.
			 *	@param		agentInit	The AgentInitializer necessary to parse AgentSet properties
			 *	@param		verbose		Determines if parsing outputs progress information (true)
			 *							or parses silently (false).
			 *	@returns	True if the simulation is successfully loaded, false otherwise.
			 */
			bool loadFromXML( const std::string & xmlName, AgentInitializer * agentInit, bool verbose=false );

		protected:
			/*!
			 *	@brief		The folder in which the scene configuration file appears
			 */
			std::string		_sceneFldr;

			/*!
			 *	@brief		The simulator to populate based on the contents of
			 *				the xml.
			 */
			XMLSimulatorBase	* _sim;

			/*!
			 *	@brief		Parses the definition of an AgentGroup.
			 *
			 *	@param		node		A pointer to the XML node containing the definition.
			 *	@param		agentInit	The AgentInitializer necessary to parse AgentProfile properties
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool parseAgentGroup( TiXmlElement * node, AgentInitializer * agentInit );

			
			/*!
			 *	@brief		Parses the definition of an obstacleset.
			 *
			 *	@param		node		A pointer to the XML node containing the obstacleset.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool parseObstacleSet( TiXmlElement * node );

			/*!
			 *	@brief		Parses the definition of an agent profile.
			 *
			 *	@param		node		A pointer to the XML node containing the definition.
			 *	@param		agentInit	The AgentInitializer necessary to parse AgentProfile properties
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool parseAgentProfile( TiXmlElement * node, AgentInitializer * agentInit );

			/*!
			 *	@brief		The number of agents loaded.
			 */
			unsigned int	_agtCount;

			/*!
			 *	@brief		Mapping from agent profile name to agent initializer.
			 */
			HASH_MAP< std::string, AgentInitializer * >	_profiles;
		};
	}	// namespace Agents
}	 // namespace Menge
#endif	// __SIM_XML_LOADER__
