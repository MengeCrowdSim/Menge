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
 *	@file		ElementFactory.h
 *	@brief		The base (templated) implementation of a factory for
 *				a plug-in element.
 */

#ifndef __ELEMENT_FACTORY_H__
#define __ELEMENT_FACTORY_H__

#include "AttributeSet.h"

// forward declaration
class TiXmlElement;

namespace Menge {

	/*!
	 *	@brief		The templated base class for a plugin element
	 */
	template < class Element >
	class ElementFactory {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		ElementFactory() {}

		/*!
		 *	@brief		This supplants the destructor.
		 *
		 *	In order to preserve potential problems in windows when
		 *	dlls do not share the same c-runtime library, the destructor
		 *	is held to be private.  To garbage collect and ConditionFactory,
		 *	the destory method should be called (which in turn, will call
		 *	the destructor from its own memory space, averting run-time
		 *  crashes.
		 */
		void destroy() {
			delete this;
		}

	protected:
		/*!
		 *	@brief		Destructor.
		 */
		virtual ~ElementFactory(){}

	public:

		/*!
		 *	@brief		The name of the action.
		 *
		 *	The action's name must be unique among all registered actions.
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the unique action name.
		 */
		virtual const char * name() const = 0;

		/*!
		 *	@brief		A description of the action.
		 *
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the action description.
		 */
		virtual const char * description() const = 0;

		/*!
		 *	@brief		Reports if the xml node defines an element for this factory.
		 *
		 *	This works by comparing the name attribute (if it exists) of the xml
		 *	node with this element factory's name.
		 *
		 *	@param		typeName	the name of the xml node in question
		 *	@returns	Returns true if the xml (appears) to specify this factory's
		 *				element.  
		 */
		bool thisFactory( const std::string & typeName ) const { return typeName == name(); }

		/*!
		 *	@brief		Parses the xml data.
		 *
		 *	This should be overridden by sub-classes.
		 *
		 *	@param		node		The xml node containing the data for the condition.
		 *	@param		behaveFldr	The path to the behavior file.  If the action references
		 *							resources in the file system, it should be defined relative
		 *							to the behavior file location.  This is the folder containing
		 *							that path. It can be ignored if no file system resources are used.
		 *	@returns	A pointer to a new condition instance for this data.
		 *				If there is an error in the xml, NULL is returned.
		 */
		Element * createInstance( TiXmlElement * node, const std::string & behaveFldr ) const {
			Element * element = instance();
			if ( ! setFromXML( element, node, behaveFldr ) ) {
				element->destroy();
				element = 0x0;
			}
			return element;
		}

	protected:
		/*!
		 *	@brief		Create an instance of this class's condition.
		 *
		 *	All ConditionFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding condition type.  The various field values
		 *	of the instance will be set in a subsequent call to ConditionFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated Action class.
		 */
		virtual Element * instance() const = 0;

		/*!
		 *	@brief		Given a pointer to an element instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute will be the element's type.
		 *	(i.e. ElementFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of ElementFactory introduce *new*  parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		element			A pointer to the element whose attributes are to be set.
		 *	@param		node			The XML node containing the element attributes.
		 *	@param		specFldr		The path to the defining file.  If the element references
		 *								resources in the file system, it should be defined relative
		 *								to this file location.  This is the folder containing
		 *								that path. For elements defined in  behavior files, this will
		 *                              be the path to that file; the same is true for those defined in
		 *                              scene files
		 *	@returns	A boolean reporting success (true) or failure (false).
		 *	@throws		An AttributeException if there is a problem in extracting values from the
		 *				attribute set.
		 */
		virtual bool setFromXML( Element * element, TiXmlElement * node, const std::string & specFldr ) const {
			_attrSet.clear();
			return _attrSet.extract( node );
		}

		/*!
		 *	@brief		The attribute set for this factory.
		 */
		mutable AttributeSet	_attrSet;
	};
}	// namespace Menge

#endif	// __ELEMENT_FACTORY_H__