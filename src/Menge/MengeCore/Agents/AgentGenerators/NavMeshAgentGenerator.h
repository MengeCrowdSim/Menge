/*

License

Menge
Copyright © and trademark ™ 2012-16 University of North Carolina at Chapel Hill.
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

/*!
*	@file		NavMeshAgentGenerator.h
*	@brief		An agent generator which creates a set of agents based on an explicit
*				enumeration of agent positions and strategically places them on a 
*				navigation mesh.
*/

#ifndef __NAVMESH_AGENT_GENERATOR_H__
#define __NAVMESH_AGENT_GENERATOR_H__

#include "MengeCore/mengeCommon.h"
#include "MengeCore/Agents/AgentGenerators/AgentGenerator.h"
#include "MengeCore/Agents/AgentGenerators/AgentGeneratorFactory.h"
#include "MengeCore/resources/NavMesh.h"
#include "MengeCore/resources/NavMeshLocalizer.h"

#include <vector>

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		Definition of agent generator class which produces agents based on
		 *				explicit enumeration of agent positions in an XML file and defines
		 *				their placement on a navigation mesh.
		 *				
		 *	To specify an navigation mesh generator, use the following syntax:
		 *
		 *		<Generator type="nav_mesh_explicit" file_name="string" group_name="string">
		 *			<Agent p_x="float" p_y="float"/>
		 *			<Agent p_x="float" p_y="float"/>
		 *			...
		 *			<Agent p_x="float" p_y="float"/>
		 *		</Genrator>
		 *
		 *	The `nav_mesh_explicit` is related to the @ref ExplicitGenerator.  Each agent's initial
		 *  2D position is explicitly enumerated.  However, that position may project onto multiple
		 *  locations on the navigation mesh.  The NavMeshGenerator class allows for specifying
		 *  a partciular polygon group to project the point onto. It introduces two
		 *  new properties:
		 *		- `file_name`: the relative path to the navigation mesh specification.
		 *		- `group_name`: the name of a polygon group specified in the navigation mesh 
		 *			defined in `file_name`.
		 *
		 *  @sa NavMesh
		 */
		class MENGE_API NavMeshGenerator : public AgentGenerator {
		public:
			/*!
			 *	@brief		Constructor
			 */
			NavMeshGenerator();

			/*!
			 *	@brief		Reports the number of agents created.
			 *
			 *	@returns	The number of agents this generator creates.
			 */
			virtual size_t agentCount() { return _positions.size(); }

			/*!
			*	@brief		Get the position of the ith agent.
			*
			*	@param		i		The index of the requested agent.
			*	@param		agt		A pointer to the ith agent whose position is to be set.
			*	@throws		AgentGeneratorException if the index, i, is invalid.
			*/
			virtual void setAgentPosition(size_t i, BaseAgent * agt);

			/*!
			 *	@brief		Adds a position to the generator
			 *
			 *	@param		p		The position to add.
			 */
			void addPosition(const Vector2 & p);

			/*!
			*	@brief		Sets the navigation mesh pointer.
			*
			*	@param		nm		The managed pointer to the navigation mesh.
			*/
			void setNavMesh(const NavMeshPtr & nm) { _navMesh = nm; }

			/*!
			 *	@brief		Sets the generators navigation mesh polygon group name.
			 *
			 *	@param		name		The name of the navigation mesh polygon group.
			 */
			void setGroupname(const std::string & name) { _groupName = name; }

			/*!
			*	@brief		Sets the navigation mesh localizer pointer.
			*
			*	@param		nml		The managed pointer to the navigation mesh localizer.
			*/
			void setNavMeshLocalizer(const NavMeshLocalizerPtr & nml) { _localizer = nml; }

		protected:
			/*!
			 *	@brief		The agent positions parsed from the file.
			 */
			std::vector< Vector2 >	_positions;

			/*!
			 *	@brief		The navigation mesh used to query elevation and gradient
			 */
			NavMeshPtr	_navMesh;

			/*!
			 *	@brief		The localizer for identifying where the agents are in the
			 *				navigation mesh.
			 */
			NavMeshLocalizerPtr	_localizer;

			/*!
			 *	@brief		The group name to place the agent onto in the navigaiton mesh.
			 *
			 *	If an empty string, the highest polygon will be used.
			 */
			std::string _groupName;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for NavMeshGenerator
		 */
		class MENGE_API NavMeshGeneratorFactory : public AgentGeneratorFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NavMeshGeneratorFactory();

			/*!
			 *	@brief		The name of the generator type.
			 *
			 *	The generator's name must be unique among all registered agent generator components.
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the unique elevation name.
			 */
			virtual const char * name() const { return "nav_mesh_explicit"; }

			/*!
			 *	@brief		A description of the agent generator.
			 *
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the agent generator description.
			 */
			virtual const char * description() const {
				return "Agent generation is done via an explicit list of agent positions, given "
					"in the XML specification. Furthermore, agent position can be targeted at "
					"specific nav mesh groups";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's agent generator implementation.
			 *
			 *	All AgentGeneratorFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding generator type.  The various field values
			 *	of the instance will be set in a subsequent call to
			 *	AgentGeneratorFactory::setFromXML. The caller of this function takes ownership of
			 *	the memory.
			 *
			 *	@returns		A pointer to a newly instantiated EleAgentGenerator class.
			 */
			AgentGenerator * instance() const { return new NavMeshGenerator(); }

			/*!
			 *	@brief		Given a pointer to a Goal Selector instance, sets the appropriate
			 *				fields from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this Goal Selector's type.
			 *	(i.e. GoalSelectorFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of GoalSelectorFactory introduce *new* GoalSelector parameters, then
			 *	the sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		gen				A pointer to the goal selector whose attributes are to
			 *								be set.
			 *	@param		node			The XML node containing the goal attributes.
			 *	@param		behaveFldr		The path to the behavior file.  If the condition
			 *								references resources in the file system, it should be
			 *								defined relative to the behavior file location.  This
			 *								is the folder containing that path.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( AgentGenerator * gen, TiXmlElement * node,
									 const std::string & behaveFldr ) const;

			/*!
			 *	@brief		Parses an agent position from an \<Agent\> tag.
			 *
			 *	@param		node		The XML node containing the agent definition.
			 *	@returns	The 2D point defined in the \<Agent\> tag.
			 *	@throws		AgentGeneratorException is the agent tag doesn't provide the required
			 *				data.
			 */
			Vector2 parseAgent(TiXmlElement * node) const;

			/*!
			 *	@brief		The identifier for the navigation mesh "file_name" string attribute.
			 */
			size_t	_fileNameID;

			/*!
			 *	@brief		The identifier for the navigation mesh "group_name" string attribute.
			 */
			size_t	_polyGroupID;
		};
	}	// namespace Agents
}	 // namespace Menge
#endif	// __NAVMESH_AGENT_GENERATOR_H__
