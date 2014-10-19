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
 *	@file		ElementDatabase.h
 *	@brief		The base (templated) implementation of the database
 *				that tracks plug-in elements.
 */

#ifndef __ELEMENT_DATABASE_H__
#define __ELEMENT_DATABASE_H__

#include <string>
#include <list>
#include "tinyxml.h"
#include "Logger.h"

namespace Menge {

	/*!
	 *	@brief		The base functionality of an element database.
	 *
	 *	Queries for parsing element XML and instantiating the appropriate elements
	 *	for the finite state machine pass through the appropriate element database.
	 *
	 *	For plug-in-compatible elements, they should have their own database and
	 *	factory.  For simplicity, they should sub-class a specialization of this template.
	 */
	template < class Factory, class Element >
	class ElementDB {
	public:
		/*!
		 *	@brief		Reports the number of registered target factories.
		 *
		 *	@returns	The number of registered target factories.
		 */
		static inline size_t count() { return _factories.size(); }
		
		/*!
		 *	@brief		Returns an instance of the TransitionTarget defined in the XML node.
		 *
		 *	The caller is responsible for deleting the TransitionTarget instance.
		 *
		 *	@param		node		The tinyxml node holding the TransitionTarget definition.
		 *	@param		behaveFldr	The path to the behavior file.  If the transition references
		 *							resources in the file system, it should be defined relative
		 *							to the behavior file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A pointer to a TransitionTarget instance for the given Condtion XML data.
		 *				If no valid TargetFactory matches the data, NULL is returned.
		 */
		static Element * getInstance( TiXmlElement * node, const std::string & behaveFldr ) {
			// extract type name
			const char * typeCStr = node->Attribute( "type" );
			if ( ! typeCStr ) {
				logger << Logger::ERR_MSG << "A " << getElementName() << " tag has been provided with no \"type\" attribute on line " << node->Row() << "\n";
				return 0x0;
			}
			std::string typeName( typeCStr );
			typename std::list< Factory * >::iterator itr = _factories.begin();
			for ( ; itr != _factories.end(); ++itr ) {
				const Factory * factory = *itr;
				if ( factory->thisFactory( typeName ) ) {
					// try to parse it
					Element * element = factory->createInstance( node, behaveFldr );
					if ( ! element ) {
						logger << Logger::ERR_MSG << "The " << getElementName() << " of type \"" << typeName << "\" defined on line " << node->Row() << " could not be instantiated.\n";
						return 0x0;
					}
					return element;
				}
			}

			logger << Logger::ERR_MSG << "Found an undefined " << getElementName() << " type (" << typeName << ") on line " << node->Row() << "\n";
			return 0x0;
		}

		/*!
		 *	@brief		Initializes the database.
		 */
		static void initialize() {
			if ( ! _initialized ) {
				_initialized = true;
				addBuiltins();
			}
		}
		
		/*!
		 *	@brief		Adds the built-in factories to the database.
		 *
		 *	Sub-classes will specialize this function to add the built-in factory
		 *	types.
		 */
		static void addBuiltins();
		
		/*!
		 *	@brief		Adds a new Target factory to the database.
		 *	
		 *	The database takes ownership of the data pointed to.
		 *	To be added, the new factory must have a unique name to all
		 *	previous actions.  If the factory *cannot* be added, it will
		 *	be deleted.
		 *
		 *	@param		factory		A pointer to the factory to add.
		 *	@returns	True if the factory is successfully added, false othrewise.
		 */
		static bool addFactory( Factory * factory ) {
			std::string testName( factory->name() );
			typename std::list< Factory * >::iterator itr = _factories.begin();
			for (; itr != _factories.end(); ++itr ) {
				std::string prevName( (*itr)->name() );
				if ( testName == prevName ) {
					logger << Logger::ERR_MSG << "Trying to add a " << getElementName() << " factory which conflicts with a previous condition factories.\n";
					logger << "\tBoth " << getElementName() << "s use the name: " << testName << ".\n";
					logger << "\tDescription of the first " << getElementName() << " type: " << (*itr)->description() << ".\n";
					logger << "\tDescription of the new " << getElementName() << " type: " << factory->description() << ".";
					factory->destroy();
					return false;
				}
			}
			_factories.push_back( factory );
			return true;
		}

		/*!
		 *	@brief		Returns the name of the element managed by this database.
		 *
		 *  @returns    the name of the element managed by this database
		 */
		static std::string getElementName();
		
		/*!
		 *	@brief		Removes all registered factories from the database.
		 */
		static void clear() {
			typename std::list< Factory * >::iterator itr = _factories.begin();
			for ( ; itr != _factories.end(); ++itr ) {
				(*itr)->destroy();
			}
			_factories.clear();
		}
	protected:
		/*!
		 *	@brief		Tracks whether the database has been initialized, so that
		 *				multiple calls to initialize will not cause problems.
		 */
		static bool _initialized;
		
		/*!
		 *	@brief		The registered factories.
		 */
		static std::list< Factory * > _factories;
		
	};

	// The two functions, addBuiltins and getElementName are *not* defined in-line
	//	because of a quirk of visual studio's template system.  By defining the
	//	default functionality for the template OUTSIDE the class definition,
	//	these can be properly, explicitly specialized in separate C++ files.

	template < class Factory, class Element >
	void ElementDB< Factory, Element >::addBuiltins() {}

	template < class Factory, class Element >
	std::string ElementDB< Factory, Element >::getElementName(){ return "unnamed"; }

	template < class Factory, class Element >
	bool ElementDB< Factory, Element >::_initialized = false;

	template < class Factory, class Element >
	std::list< Factory * > ElementDB< Factory, Element >::_factories;
}	// namespace Menge

#endif	// __ELEMENT_DATABASE_H__
