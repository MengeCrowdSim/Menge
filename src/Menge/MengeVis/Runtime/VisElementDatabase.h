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

/*!
 @file    VisElementDatabase.h
 @brief   The base (templated) implementation of the database that tracks visualization plug-in
          elements.
 */

#ifndef __VIS_ELEMENT_DATABASE_H__
#define __VIS_ELEMENT_DATABASE_H__

#include "MengeCore/Runtime/Logger.h"
#include "MengeVis/Runtime/VisElementException.h"
#include "macros.h"

#include <list>
#include <set>
#include <string>

namespace MengeVis {

namespace Runtime {
/*!
 @brief   The base functionality of an visualization element database.

 It maps simulation elements to visualization artifacts. Visualization elements provide a unique
 identifer that can be used to determine if there is a bespoke implementation of visualization
 element.
 */
template <class VisElement, class SimElement>
class VisElementDB {
 public:
  /*!
   @brief   Reports the number of registered visualization element types.

   @returns The number of registered target factories.
   */
  static inline size_t count() { return _visElements.size(); }

  /*!
   @brief   Returns an instance of the corresponding visualization element.

   @param   element   The element to to provide a visual element for.
   @returns A pointer to a compatible visualization element, targeted to the given @p element.
            Guaranteed to return a non-null pointer.
   */
  // TODO: Why is this not const?
  static VisElement* getInstance(const SimElement* element) {
    // extract type name
    const std::string stringId = element->getStringId();
    typename HASH_MAP<std::string, VisElement*>::iterator itr = _visElements.find(stringId);
    VisElement* vElement = 0x0;
    if (itr != _visElements.end()) {
      vElement = itr->second;
    } else {
      if (_element_types_queried.count(stringId) == 0) {
        Menge::logger << Menge::Logger::WARN_MSG << "Unable to find a '";
        Menge::logger << getElementName() << "' vis element for the selected element: '";
        Menge::logger << stringId << "'. Using default visualization element.";
      }
      vElement = getDefaultElement();
    }
    _element_types_queried.insert(stringId);
    vElement->setElement(element);
    return vElement;
  }

  /*!
   @brief   Initializes the database.
   */
  static void initialize() {
    if (!_initialized) {
      _initialized = true;
      addBuiltins();
    }
  }

  /*!
   @brief   Returns the default visual element for this type visual element.

   @returns An instance of the default element.
   */
  static VisElement* getDefaultElement();

  /*!
   @brief   Adds the built-in factories to the database.

   Sub-classes will specialize this function to add the built-in factory types.
   */
  static void addBuiltins();

  /*!
   @brief   Adds a new visualization element for elements with the given name.

   The database takes ownership of the data pointed to. To be added, the `element` name must be
   unique relative all previously defined visualization elements *in this database*. If the
   `visElement` cannot be added, it will be deleted.

   @param   visElement    The visualization element for the named element type.
   @returns True if the visualization element is successfully added.
   */
  static bool addVisElement(VisElement* visElement) {
    std::string elementName = visElement->getElementName();
    typename HASH_MAP<std::string, VisElement*>::iterator itr = _visElements.find(elementName);
    if (itr != _visElements.end()) {
      Menge::logger << Menge::Logger::ERR_MSG << "Trying to add a " << getElementName();
      Menge::logger << " visualization element which conflicts with a previous vis ";
      Menge::logger << "element.\n\tBoth " << getElementName() << "s use the name: ";
      Menge::logger << elementName << ".\n";
      // visElement->destroy();
      delete visElement;
      return false;
    }
    _visElements[elementName] = visElement;
    return true;
  }

  /*!
   @brief   Returns the name of the element type managed by this database.

   @returns The name of the element managed by this database.
   */
  static std::string getElementName();

  /*!
   @brief   Removes all registered visualization elements from the database.
   */
  static void clear() {
    typename HASH_MAP<std::string, VisElement*>::iterator itr = _visElements.begin();
    for (; itr != _visElements.end(); ++itr) {
      itr.second->destroy();
    }
    _visElements.clear();
  }

 protected:
  /*!
   @brief   Tracks whether the database has been initialized, so that multiple calls to initialize
            will not cause problems.
   */
  static bool _initialized;

  /*!
   @brief   The registered visualization elements. Mapping from simulation element identifier to
            corresponding visualization element.
   */
  static HASH_MAP<std::string, VisElement*> _visElements;

  /*!
   @brief  When getInstance is called, the string id gets registered with this databse. It means if
           a particular type is unsupported, only a single warning message is dispatched.  */
  static std::set<std::string> _element_types_queried;
};

// The two functions, addBuiltins and getElementName are *not* defined in-line
//  because of a quirk of visual studio's template system.  By defining the
//  default functionality for the template OUTSIDE the class definition,
//  these can be properly, explicitly specialized in separate C++ files.

template <class VisElement, class SimElement>
void VisElementDB<VisElement, SimElement>::addBuiltins() {}

template <class VisElement, class SimElement>
std::string VisElementDB<VisElement, SimElement>::getElementName() {
  return "unnamed";
}

template <class VisElement, class SimElement>
bool VisElementDB<VisElement, SimElement>::_initialized = false;

template <class VisElement, class SimElement>
HASH_MAP<std::string, VisElement*> VisElementDB<VisElement, SimElement>::_visElements;

template <class VisElement, class SimElement>
std::set<std::string> VisElementDB<VisElement, SimElement>::_element_types_queried;

template <class VisElement, class SimElement>
VisElement* VisElementDB<VisElement, SimElement>::getDefaultElement() {
  throw VisElementFatalException();
}
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __VIS_ELEMENT_DATABASE_H__
