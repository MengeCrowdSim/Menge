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
 *	@file		AgentInitializer.h
 *	@brief		The infrastructure for initializing agent properties
 *				from the scene specification file.
 */

#ifndef __AGENT_INITIALIZER_H__
#define	 __AGENT_INITIALIZER_H__

#include "CoreConfig.h"
#include <string>
#include "VelocityModifiers/VelModifier.h"

// forward declaration
class TiXmlElement;

namespace Menge {

	// Forward declarations
	namespace Math {
		class FloatGenerator;
		class IntGenerator;
	}

	using namespace Math;

	namespace Agents {
		// forward declaration
		class BaseAgent;

		/*!
		 *	@brief		Class which determines the agent properties for each new agent.
		 *
		 *	This base agent intializer class facilitates setting all BaseAgent properties.
		 *	The property values are set using number generators (see RandGenerator.h).
		 */
		class MENGE_API AgentInitializer {
		public:
			/*!
			 *	@brief		Return type for parsing efforts.  Helps the various derived classes
			 *				coordinate their work.
			 */
			enum ParseResult {
				FAILURE = 0,	///< The parsing ended in failure.
				IGNORED,		///< The XML data was ignored by the function.
				ACCEPTED		///< The XML data was accepted by the function.
			};

			/*!
			 *	@brief		Constructor.
			 *
			 *	The values for each agent take a hard-coded default values.
			 */
			AgentInitializer();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		init		The AgentInitializer to copy all values
			 *							from.
			 */
			AgentInitializer( const AgentInitializer & init );

			/*!
			 *	@brief		Destructor.
			 */
			virtual ~AgentInitializer();

			/*!
			 *	@brief		Parses an AgentSet property tag, setting agent values as
			 *				appropriate.
			 *
			 *	This function can be called on *all* AgentSet property tags.  The function
			 *	is responsible for determining which tags have relevant information and which
			 *	are to be ignored.
			 *
			 *	If a tag is deemed to be relevant, but the content of the tag is incorrect,
			 *	such as a mal-formed property definition, then the parsing "fails".  However,
			 *	unexpected property attributes or specifications will be ignored.  
			 *	If the system is running in verbose mode, these unexpected attributes will be
			 *	indicated on the console.
			 *
			 *	Such a tag could look like this:

			 *			<Common r="0.1" class="2" ... />

			 *	or

			 *			<Common r="0.1" class="2" ... >
			 *				<Property type="radius" dist="u" min="0.18" max="0.2"/>
			 *				<Property type="prefSpeed" dist="n" mean="1.34" stddev="0.2"/>
			 *			</Common>
			 *
			 *	@param		node		The xml node containing agent properties for an AgentSet.
			 *	@param		sceneFldr	Since we now need access to resources at this stage, 
			 *                          we pass the base folder for resource loading
			 *	@returns	True if parsing was "successful", false otherwise.
			 */
			bool parseProperties( TiXmlElement * node, const std::string & sceneFldr );

			/*!
			 *	@brief		Sets the properties of the given agent based on the initializer's
			 *				values.
			 *
			 *	This needs to be overridden by sub-classes.  The first thing the sub-class
			 *	should do is dynamic_cast the argument to its expected type to make sure it
			 *	is the proper agent type.  If not, this should be considered failure.
			 *	Then it should set its unique properties and then call the super class's
			 *	setProperties function.
			 *
			 *	@param		agent		The agent whose properties are to be set.
			 *	@returns	True if the properties were set successfully, false otherwise.
			 */
			virtual bool setProperties( BaseAgent * agent );

			/*!
			 *	@brief		Sets all generators to default values.
			 *
			 *	Resets all number generators to default const values.  This assumes that all
			 *	required number generators already exist and will delete them appropriately.
			 *	*Do not* call this in the constructor.
			 */
			virtual void setDefaults();

			/*!
			 *	@brief		Creates a copy of this AgentInitializer instance.
			 *
			 *	@returns	A pointer to a new AgentInitializer with all of the same values
			 *				as this.  The caller is responsible for freeing up the 
			 *				new instance.
			 */
			virtual AgentInitializer * copy() const { return new AgentInitializer( *this ); }

			/*!
			 *	@brief		Determines if the agent properties parsing process will be verbose.
			 */
			static bool	VERBOSE;

		protected:
			/*!
			 *	@brief		Reports if this AgentInitializer cares about the given AgentSet
			 *				property XML tag.
			 *
			 *	This is the mechanism by which new sub-classes can extend the parameter
			 *	space.  Each pedestrian model which introduces new per-agent properties that
			 *	must override this function.  However, the overriden function must, in turn,
			 *	call the parent class if it doesn't consider the tag relevant, giving the
			 *	parent class a chance to determine if the tag is relevant.  This is the 
			 *	mechanism by which derived classes will also benefit from the `<Common>`
			 *	parameter set.
			 *
			 *	@param		tagName		The name of the tag to test.
			 *	@returns	True if the tag is relevant, false otherwise.
			 */
			virtual bool isRelevant( const ::std::string & tagName );

			/*!
			 *	@brief		Defines a constant value for an agent property as specified
			 *				by the attribute of an agent property tag.
			 *	
			 *	Derived classes should override this function, but possibly call the parent
			 *	class's implementation.  First, it should test to see if the paramName is 
			 *	expected by the derived class.  If so, the derived class can determine fail
			 *	or accept.  If it is not expected, it should call the parent class's implementation
			 *	and returns its value.
			 *
			 *	@param			paramName		A string containing the parameter name.
			 *	@param			value			A string containing the value for the parameter.
			 *	@returns		The result of the parse: failure, ignored, or accepted.
			 */
			virtual ParseResult setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value );
			
			/*!
			 *	@brief		Parses the Property tag that is the child of an AgentSet Parameter
			 *				tag.
			 *
			 *	As with the other parsing code, success is defined by finding an expected,
			 *	correct field *or* an unexpected tag (which is just ignored).  Failure occurs
			 *	when the tags are as expected, but the values are invalid.
			 *
			 *	@param		node		The xml node containing agent properties for an AgentSet.
			 *	@returns	True if parsing was "successful", false otherwise.
			 */
			bool parsePropertySpec( TiXmlElement * node );

			/*!
			 *	@brief		Process the given `<Property .../>` tag.  
			 *
			 *	As a pre-condition to this function, the XML node contains a `<Property.../>`
			 *	tag and has been confirmed to have, at least, a name attribute.  Nothing
			 *	else about the tag has been validated.  
			 *
			 *	If the property name is unexpected, it will be ignored.  If it is expected, 
			 *	this function will attempt to interpret the XML tag as a number distribution
			 *	for a valid agent attribute.  If it can do so, it is successful, if it can't,
			 *	it fails.
			 *
			 *	@param		propName		The extracted "name" attribute from the Property tag.
			 *	@param		node			The XML node for the Property tag.
			 *	@returns	The ParseResult indicating if the property was successful 
			 *				(IGNORED or ACCEPTED) or if it was a malformed tag (FAILURE).
			 */
			virtual ParseResult processProperty( ::std::string propName, TiXmlElement * node );

			/*!
			 *	@brief		Helper function for setting a property from an xml attribute.
			 *
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined on the main property tag: e.g., 
			 *
			 *			<Common property="value.../>
			 *			
			 *	@param		gen			A reference to a pointer to a valid FloatGenerator.
			 *							The new float generator will be put into this variable.
			 *	@param		valueStr	The string representing the attribute value to convert
			 *							to a const float generator.
			 *	@param		scale		Optional argument for changing the units of the input value.
			 *							(e.g., specification commonly describes angles as degrees, 
			 *							but internal representation is in radians).
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult constFloatGenerator( FloatGenerator * & gen, const ::std::string & valueStr, float scale=1.f );

			/*!
			 *	@brief		Helper function for setting a property from an xml attribute.
			 *			
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined on the main property tag: e.g., 
			 *
			 *			<Common property="value.../>
			 *			
			 *	@param		numValue	The new float will be put into this variable.
			 *	@param		valueStr	The string representing the attribute value to convert
			 *							to a const float generator.
			 *	@param		scale		Optional argument for changing the units of the input value.
			 *							(e.g., specification commonly describes angles as degrees, 
			 *							but internal representation is in radians).
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult constFloat( float & numValue, const ::std::string & valueStr, float scale=1.f );

			/*!
			 *	@brief		Helper function for setting a property from an xml attribute.
			 *			
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined on the main property tag: e.g., 
			 *
			 *			<Common property="value.../>
			 *			
			 *	@param		gen			A reference to a pointer to a valid IntGenerator.
			 *							The new int generator will be put into this variable.
			 *	@param		valueStr	The string representing the attribute value to convert
			 *							to a const float generator.
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult constIntGenerator( IntGenerator * & gen, const ::std::string & valueStr );

			/*!
			 *	@brief		Helper function for setting a property from an xml attribute.
			 *			
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined on the main property tag: e.g., 
			 *
			 *			<Common property="value.../>
			 *			
			 *	@param		numValue	The new size_t will be put into this variable.
			 *	@param		valueStr	The string representing the attribute value to convert
			 *							to a const float generator.
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult constSizet( size_t & numValue, const ::std::string & valueStr );

			/*!
			 *	@brief		Helper function for setting a property from an xml Property node.
			 *
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined explicitly as a distribution on a Property tag, e.g.,
			 *	
			 *			<Property name="property" dist="x".../>
			 *
			 *			
			 *	@param		gen			A reference to a pointer to a valid FloatGenerator.
			 *							The new float generator will be put into this variable.
			 *	@param		node		The xml node containing attributes for the distribution
			 *							specification.
			 *	@param		scale		Optional argument for changing the units of the input value.
			 *							(e.g., specification commonly describes angles as degrees, 
			 *							but internal representation is in radians).
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult getFloatGenerator( FloatGenerator * & gen, TiXmlElement * node, float scale=1.f );

			/*!
			 *	@brief		Helper function for setting a property from an xml Property node.
			 *			
			 *	This is used to parse and create a value generator in the case where the
			 *	attribute is defined on the main property tag: e.g., 
			 *
			 *			<Common property="value.../>
			 *			
			 *	@param		gen			A reference to a pointer to a valid IntGenerator.
			 *							The new int generator will be put into this variable.
			 *	@param		node		The xml node containing attributes for the distribution
			 *							specification.
			 *	@returns	A parse result indicating either ACCEPTED or FAILURE.  IGNORED should
			 *				never be a result, because it should only be called on expected
			 *				parameters, so it must either succed or fail.
			 */
			ParseResult getIntGenerator( IntGenerator * & gen, TiXmlElement * node );

			/*!
			 *	@brief		The maximum speed the agent can take.
			 */
			FloatGenerator * _maxSpeed;

			/*!
			 *	@brief		The maximum acceleration the agent can experience (interpreted isotropically).
			 */
			FloatGenerator * _maxAccel;

			/*!
			 *	@brief		The preferred speed of the agent
			 */
			FloatGenerator * _prefSpeed;	

			/*!
			 *	@brief		The number of nearby agents used to plan dynamic respones.
			 */
			IntGenerator * _maxNeighbors;

			/*!
			 *	@brief		The maximum distance at which another agent will be 
			 *				considered for a response.
			 */
			FloatGenerator * _neighborDist;

			/*!
			 *	@brief		The agent's radius.  See Agents::BaseAgent::_radius for details.
			 */
			FloatGenerator * _radius;

			/*!
			 *	@brief		The agent's maximum angular velocity (in radians/sec) -- 
			 *				used for controlling the changes in agent orientation.
			 */
			FloatGenerator * _maxAngVel;

			/*!
			 *	@brief		A mask indicating which obstacles affect the agent.
			 *				See Agents::BaseAgent::_obstacleSet for details.
			 */
			size_t _obstacleSet;

			/*!
			 *	@brief		The priority of each agent.  See Agents::BaseAgent::_priority for details.
			 */
			float _priority;

			/*!
			 *	@brief		The population class for this agent.  See Agents::BaseAgent::_class for details.
			 */
			size_t _class;

			/*!
			 *  @brief		Velocity Modifiers to be applied to this class
			 */
			std::vector<BFSM::VelModifier *> _velModifiers;

		};
	}	// namespace Agents
}	// namespace Menge
#endif	 // __AGENT_INITIALIZER_H__
