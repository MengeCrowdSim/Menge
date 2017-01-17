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
 *	@file		VisElementDatabase.h
 *	@brief		The base (templated) implementation of the database
 *				that tracks visualization plug-in elements.
 */

#ifndef __VIS_ELEMENT_DATABASE_H__
#define __VIS_ELEMENT_DATABASE_H__

#include "macros.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <string>
#include <list>

namespace MengeVis {

	namespace Runtime {
		/*!
		 *	@brief		The base functionality of an visualization element database.
		 *
		 *	It maps simulation elements to visualization artifacts. Visualization elements provide
		 *	a unique identifer that can be used to determine if there is a bespoke implementation of
		 *	visualization element.
		 */
		template < class VisElement, class SimElement >
		class VisElementDB {
		public:
			/*!
			 *	@brief		Reports the number of registered visualization element types.
			 *
			 *	@returns	The number of registered target factories.
			 */
			static inline size_t count() { return _visElements.size(); }

			/*!
			 *	@brief		Returns an instance of the corresponding visualization element.
			 *
			 *	@param		node		The element to to provide a visual element for.
			 *	@returns	A pointer to a compatible visualization element, targeted to the given
			 *				@p element.  Guaranteed to return a non-null pointer.
			 */
			static VisElement * getInstance( SimElement * element ) {
				// extract type name
				const std::string stringId = element->getStringId();
				typename HASH_MAP<std::string, VisElement * >::iterator itr =
					_visElements.find( stringId );
				VisElement * vElement = 0x0;
				if ( itr != _visElements.end() ) {
					vElement = itr->second;
				} else {
					Menge::logger << Menge::Logger::WARN_MSG << "Unable to find a ";
					Menge::logger << getElementName() << " vis element for the selected element: ";
					Menge::logger << stringId << ". Using default visualization element.";
					vElement = getDefaultElement();
				}
				vElement->setElement( element );
				return vElement;
			}

			/*!
			 *	@brief		Initializes the database.
			 */
			static void initialize() {
				if ( !_initialized ) {
					_initialized = true;
					addBuiltins();
				}
			}

			/*!
			 *	@brief		Returns the default visual element for this type visual element.
			 *
			 *	@returns	An instance of the default element.
			 */
			static VisElement * getDefaultElement();

			/*!
			 *	@brief		Adds the built-in factories to the database.
			 *
			 *	Sub-classes will specialize this function to add the built-in factory
			 *	types.
			 */
			static void addBuiltins();

			/*!
			 *	@brief		Adds a new visualization element for elements with the given name.
			 *
			 *	The database takes ownership of the data pointed to. To be added, the @element name
			 *	must be unique relative all previously defined visualization elements *in this
			 *	database*.  If the @visElement cannot be added, it will be deleted.
			 *
			 *	@param		elementName		The unique identifier of the element type.
			 *	@param		visElement		The visualization element for the named element type.
			 *	@returns	True if the visualization element is successfully added.
			 */
			static bool addVisElement( const std::string & elementName, VisElement * visElement ) {
				typename  HASH_MAP<std::string, VisElement * >::iterator itr =
					_visElements.find( elementName );
				if ( itr != _visElements.end() ) {
					Menge::logger << Menge::Logger::ERR_MSG << "Trying to add a " << getElementName();
					Menge::logger << " visualization element which conflicts with a previous vis ";
					Menge::logger << "element.\n\tBoth " << getElementName() << "s use the name: ";
					Menge::logger << elementName << ".\n";
					//visElement->destroy();
					delete visElement;
					return false;
				}
				_visElements[ elementName ] = visElement;
				return true;
			}

			/*!
			 *	@brief		Returns the name of the element type managed by this database.
			 *
			 *  @returns    the name of the element managed by this database
			 */
			static std::string getElementName();

			/*!
			 *	@brief		Removes all registered visualization elements from the database.
			 */
			static void clear() {
				typename  HASH_MAP<std::string, VisElement * >::iterator itr = _visElement.begin();
				for ( ; itr != _visElement.end(); ++itr ) {
					itr.second->destroy();
				}
				_visElement.clear();
			}

		protected:
			/*!
			 *	@brief		Tracks whether the database has been initialized, so that
			 *				multiple calls to initialize will not cause problems.
			 */
			static bool _initialized;

			/*!
			 *	@brief		The registered visualization elements. Mapping from simulation element
			 *				identifier to corresponding visualization element.
			 */
			static HASH_MAP<std::string, VisElement * > _visElements;

		};

		// The two functions, addBuiltins and getElementName are *not* defined in-line
		//	because of a quirk of visual studio's template system.  By defining the
		//	default functionality for the template OUTSIDE the class definition,
		//	these can be properly, explicitly specialized in separate C++ files.

		template < class VisElement, class SimElement >
		void VisElementDB< VisElement, SimElement >::addBuiltins() {}

		template < class VisElement, class SimElement >
		std::string VisElementDB< VisElement, SimElement >::getElementName(){ return "unnamed"; }

		template < class VisElement, class SimElement >
		bool VisElementDB< VisElement, SimElement >::_initialized = false;

		template < class VisElement, class SimElement >
		HASH_MAP<std::string, VisElement * > VisElementDB< VisElement, SimElement >::_visElements;

		template < class VisElement, class SimElement >
		VisElement * VisElementDB< VisElement, SimElement >::getDefaultElement() {
			throw VisElementFatalException();
		}
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __VIS_ELEMENT_DATABASE_H__
