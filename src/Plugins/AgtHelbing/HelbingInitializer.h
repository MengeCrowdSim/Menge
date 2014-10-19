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
 *	@file		HelbingInitializer.h
 *	@brief		The AgentInitializer for the Helbing simulator.
 */
#ifndef __HELBING_INITIALIZER_H__
#define	__HELBING_INITIALIZER_H__

#include "AgentInitializer.h"

using namespace Menge;

namespace Helbing {
	/*!
	 *	@brief		Class which determines the agent properties for each new Helbing agent.
	 */
	class AgentInitializer : public Agents::AgentInitializer {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	The values for each agent take a hard-coded default values.
		 */
		AgentInitializer();

		/*!
		 *	@brief		Copy Constructor.
		 *
		 *	@param		init		The initializer to copy values from
		 */
		AgentInitializer( const AgentInitializer & init );

		/*!
		 *	@brief		Destructor.
		 */
		virtual ~AgentInitializer();

		/*!
		 *	@brief		Sets the properties of the given agent based on the initializer's
		 *				values.
		 *
		 *	This needs to be overridden by sub-classes.  The first thing the sub-class
		 *	should do is dynamic_cast the argument to its expected type to make sure it
		 *	is the proper agent type.  If not, this should be considered failure.
		 *	Then it should set its unique properties an+d then call the super class's
		 *	setProperties function.
		 *
		 *	@param		agent		The agent whose properties are to be set.
		 *	@returns	True if the properties were set successfully, false otherwise.
		 */
		virtual bool setProperties( Agents::BaseAgent * agent );

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
		 *	mechanism by which derived classes will also benefit from the `<Helbing>`
		 *	parameter set.
		 *
		 *	@param		tagName		The tag to test for relevancy.
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
		virtual Agents::AgentInitializer::ParseResult setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value );

		/*!
		 *	@brief		Process the given <Property .../> tag.  
		 *
		 *	As a pre-condition to this function, the XML node contains a <Property.../>
		 *	tag and has been confirmed to have, at least, a name attribute.  Nothing
		 *	else about the tag has been validated.  
		 *
		 *	If the property name is unexpected, it will be ignored.  If it is expected, 
		 *	this function will attempt to interpret the XML tag as a number distribution
		 *	for a valid agent attribute.  If it can do so, it is successful, if it can't,
		 *	it fails.
		 *
		 *	@param		propName		The extractd "name" property from the Property tag.
		 *	@param		node			The XML node for the Property tag.
		 *	@returns	True if parsing was "successful", false otherwise.
		 */
		virtual ParseResult processProperty( ::std::string propName, TiXmlElement * node );

		/*!
		 *	@brief		The mass of the agent.
		 */
		FloatGenerator * _mass;			
	};
}	// namespace Helbing


#endif	// __HELBING_INITIALIZER_H__