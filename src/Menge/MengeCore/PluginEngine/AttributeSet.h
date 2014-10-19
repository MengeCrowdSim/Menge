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
 *	@file		AttributeSet.h
 *	@brief		Defines the set of element attributes.
 */

#ifndef __ATTRIBUTE_SET_H__
#define __ATTRIBUTE_SET_H__

#include <vector>
#include "CoreConfig.h"
#include "Math/Vector2.h"

// forward declaration
class TiXmlElement;

namespace Menge {

	class Attribute;
	namespace Math {
		class FloatGenerator;
		class IntGenerator;
		class Vec2DGenerator;
	}
	using namespace Math;
	/*!
	 *	@brief		A set of attributes.
	 *
	 *	This is a utility class for facilitating parsing XML specifications.
	 *	It is used for elements who can be completely defined strictly via
	 *	XML attributes in a single tag.  It allows handling these types of
	 *	specifications without explicitly dealing with the XML.
	 *
	 *	Making use of this utility is a two-step process.  
	 *		1 Define the expected attributes.
	 *		2 Use the extracted values to initialize the agent.
	 *
	 *	Every element factory has a built-in attribute set (called _attrSet).  
	 *	In the factory's constructor, simply make calls into the attribute set
	 *	providing the details of each desired attribute.  The attribute set
	 *	returns a unique identifier for each new attribute.  Store these
	 *	attributes, you will use them to extract the parsed values later.
	 *
	 *	In the setFromXML function, first, call the parent class's implementation
	 *	of the function.  If there is a parsing error, an exception will be automatically
	 *	thrown.  You don't need to catch it.  The framework will catch the exception and
	 *	respond accordingly.  Assuming there is no exception, simply use the
	 *	stored identifiers to extract values from the attribute set and set it to
	 *	the instance.
	 */
	class MENGE_API AttributeSet {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		AttributeSet();

		/*!
		 *	@brief		Destructor.
		 */
		~AttributeSet();

		/*!
		 *	@brief		Adds a boolean attribute to the set.
		 *
		 *	@param		name		The attribute name.
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addBoolAttribute( const std::string & name, bool required, bool defValue=false );

		/*!
		 *	@brief		Adds an integer attribute to the set.
		 *
		 *	@param		name		The attribute name.
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addIntAttribute( const std::string & name, bool required, int defValue=0 );

		/*!
		 *	@brief		Adds a size_t attribute to the set.
		 *
		 *	@param		name		The attribute name.
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addSizeTAttribute( const std::string & name, bool required, size_t defValue=0 );

		/*!
		 *	@brief		Adds a float attribute to the set.
		 *
		 *	@param		name		The attribute name.
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addFloatAttribute( const std::string & name, bool required, float defValue=0 );

		/*!
		 *	@brief		Adds a float distribution attribute to the set.
		 *
		 *	@param		prefix		The prefix for the distribution (provide the empty string to use defaults.)
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	
		 *	@param		scale		The scale value for this distribution.
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addFloatDistAttribute( const std::string & prefix, bool required, float defValue, float scale );

		/*!
		 *	@brief		Adds a Vector2 distribution attribute to the set.
		 *
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	
		 *	@param		scale		The optional scale value for this distribution.
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addVec2DDistAttribute( bool required, const Vector2 & defValue, float scale=1.f );

		/*!
		 *	@brief		Adds an integer distribution attribute to the set.
		 *
		 *	@param		prefix		The prefix for the distribution (provide the empty string to use defaults.)
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addIntDistAttribute( const std::string & prefix, bool required, int defValue=0 );

		/*!
		 *	@brief		Adds an integer attribute to the set.
		 *
		 *	@param		name		The attribute name.
		 *	@param		required	Whether or not the attribute is required (true)
		 *							or not (false).
		 *	@param		defValue	The default value for the attribute if none is provided.
		 *							This value will serve no purpose if required is passed true.	 
		 *	@returns	The unique identifier for this attribute.
		 *	@throws		An AttributeDefinitionException if the attribute name is not unique.
		 */
		size_t addStringAttribute( const std::string & name, bool required, const std::string & defValue="" );

		/*!
		 *	@brief		Prepares the set to parse a new instance.
		 */
		void clear();

		/*!
		 *	@brief		Parses the node for the given attributes.
		 *
		 *	@param		node		The XML node containing the attributes.
		 *	@returns	True on sucessful extraction, false otherwise.
		 */
		bool extract( TiXmlElement * node );

		/*!
		 *	@brief		Retrieve the int value from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	The int value.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		int getInt( size_t propID );

		/*!
		 *	@brief		Retrieve the size_t value from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	The size_t value.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		size_t getSizeT( size_t propID );

		/*!
		 *	@brief		Retrieve the int value from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	The boolean value.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		bool getBool( size_t propID );

		/*!
		 *	@brief		Retrieve the float value from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	The float value.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		float getFloat( size_t propID );

		/*!
		 *	@brief		Retrieve the string value from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	The int value.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		std::string	getString( size_t propID );

		/*!
		 *	@brief		Retrieve the float generator from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	A pointer to the float generator.  The caller now is responsible
		 *				for the memory to which this pointer refers.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		FloatGenerator * getFloatGenerator( size_t propID );

		/*!
		 *	@brief		Retrieve the Vector2 generator from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	A pointer to the Vector2 generator.  The caller now is responsible
		 *				for the memory to which this pointer refers.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		Vec2DGenerator * getVec2DGenerator( size_t propID );

		/*!
		 *	@brief		Retrieve the int generator from the given property ID.
		 *
		 *	If the attribute associated with the given property ID is of the
		 *	wrong type, this will throw an exception.
		 *
		 *	@param		propID		The property ID -- this is given as the
		 *							result of AttributeSet::addAttribute.
		 *	@returns	A pointer to the int generator.  The caller now is responsible
		 *				for the memory to which this pointer refers.
		 *	@throws		An AttributeDefinitionException if the types are wrong.
		 */
		IntGenerator * getIntGenerator( size_t propID );

	protected:
		/*!
		 *	@brief		reports if the new attribute name clashes with a previous attribute name.
		 */
		bool isUniqueName( const std::string & name );

		/*!
		 *	@brief		The attributes in the set.
		 */
		std::vector< Attribute * >	_attrs;
	};
}	// namespace Menge
#endif // __ATTRIBUTE_SET_H__
