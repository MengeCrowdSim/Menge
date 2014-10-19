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

/*!
 *	@file		ExplicitAgentGenerator.h
 *	@brief		An agent generator which creates a set of agents based on an explicit
 *				enumeration of agent positions.
 */

#ifndef __EXPLICIT_AGENT_GENERATOR_H__
#define __EXPLICIT_AGENT_GENERATOR_H__

#include "mengeCommon.h"
#include "AgentGenerators/AgentGenerator.h"
#include "AgentGenerators/AgentGeneratorFactory.h"
#include <vector>

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		Definition of agent generator class which produces agents based on
		 *				explicit enumeration of agent positions in an XML file.
		 */
		class MENGE_API ExplicitGenerator : public AgentGenerator {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ExplicitGenerator();

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
			 *	@returns	The 2D position of the agent based on the generator's parameters.
			 *	@throws		AgentGeneratorException if the index, i, is invalid.
			 */
			virtual Vector2 agentPos( size_t i );
			
			/*!
			 *	@brief		Adds a position to the generator
			 *
			 *	@param		p		The position to add.
			 */
			void addPosition( const Vector2 & p );
			
		protected:
			/*!
			 *	@brief		The agent positions parsed from the file.
			 */
			std::vector< Vector2 >	_positions;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for ExplicitGenerator
		 */
		class MENGE_API ExplicitGeneratorFactory : public AgentGeneratorFactory {
		public:
			/*!
			 *	@brief		The name of the generator type.
			 *
			 *	The generator's name must be unique among all registered agent generator components.  
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the unique elevation name.
			 */
			virtual const char * name() const { return "explicit"; }

			/*!
			 *	@brief		A description of the agent generator.
			 *
			 *	Each agent generator factory must override this function.
			 *
			 *	@returns	A string containing the agent generator description.
			 */
			virtual const char * description() const {
				return "Agent generation is done via an explicit list of agent positions, given in the XML specification.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's agent generator implementation.
			 *
			 *	All AgentGeneratorFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding generator type.  The various field values
			 *	of the instance will be set in a subsequent call to AgentGeneratorFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated EleAgentGenerator class.
			 */
			AgentGenerator * instance() const { return new ExplicitGenerator(); }

			/*!
			 *	@brief		Given a pointer to an AgentGenerator instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this AgentGenerator's type.
			 *	(i.e. AgentGenerator::thisFactory has already been called and returned true.)
			 *	If sub-classes of AgentGeneratorFactory introduce *new* AgentGenerator parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		gen			A pointer to the agent generator whose attributes are to be set.
			 *	@param		node		The XML node containing the agent generator attributes.
			 *	@param		specFldr	The path to the specification file.  If the AgentGenerator references
			 *							resources in the file system, it should be defined relative
			 *							to the specification file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( AgentGenerator * gen, TiXmlElement * node, const std::string & specFldr ) const;

			/*!
			 *	@brief		Parses an agent position from an \<Agent\> tag.
			 *
			 *	@param		node		The XML node containing the agent definition.
			 *	@returns	The 2D point defined in the \<Agent\> tag.
			 *	@throws		AgentGeneratorException is the agent tag doesn't provide the required data.
			 */
			Vector2 parseAgent( TiXmlElement * node ) const;
		};
	}	// namespace Agents
}	 // namespace Menge
#endif	// __EXPLICIT_AGENT_GENERATOR_H__