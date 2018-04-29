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
 @file    ElementDatabase.h
 @brief    The base (templated) implementation of the database that tracks plug-in elements.
 */

#ifndef __ELEMENT_DATABASE_H__
#define __ELEMENT_DATABASE_H__

#include "MengeCore/Runtime/Logger.h"

#include "thirdParty/tinyxml.h"

#include <list>
#include <string>

namespace Menge {

/*!
 @brief    The base functionality of an element database.

 Queries for parsing element XML and instantiating the appropriate elements for the finite state
 machine pass through the appropriate element database.

 For plug-in-compatible elements, they should have their own database and factory. For simplicity,
 they should sub-class a specialization of this template.
 */
template <class Factory, class Element>
class ElementDB {
 public:
  /*!
   @brief    Reports the number of registered target factories.

   @returns  The number of registered target factories.
   */
  static inline size_t count() { return _factories.size(); }

  /*!
   @brief    Returns an instance of the TransitionTarget defined in the XML node.

   The caller is responsible for deleting the TransitionTarget instance.

   @param    node        The tinyxml node holding the TransitionTarget definition.
   @param    behaveFldr  The path to the behavior file. If the transition references resources in
                        the file system, it should be defined relative to the behavior file
                        location. This is the folder containing that path.
   @returns  A pointer to a element instance for the given element XML data. If no valid factory
            matches the data, NULL is returned.
   */
  static Element* getInstance(TiXmlElement* node, const std::string& behaveFldr) {
    // extract type name
    const char* typeCStr = node->Attribute("type");
    if (!typeCStr) {
      logger << Logger::ERR_MSG << "A " << getElementName();
      logger << " tag has been provided with no \"type\" attribute on line ";
      logger << node->Row() << "\n";
      return 0x0;
    }
    std::string typeName(typeCStr);
    typename std::list<Factory*>::iterator itr = _factories.begin();
    for (; itr != _factories.end(); ++itr) {
      const Factory* factory = *itr;
      if (factory->thisFactory(typeName)) {
        // try to parse it
        Element* element = factory->createInstance(node, behaveFldr);
        if (!element) {
          logger << Logger::ERR_MSG << "The " << getElementName() << " of type \"";
          logger << typeName << "\" defined on line " << node->Row();
          logger << " could not be instantiated.\n";
          return 0x0;
        }
        return element;
      }
    }

    logger << Logger::ERR_MSG << "Found an undefined " << getElementName() << " type (";
    logger << typeName << ") on line " << node->Row() << "\n";
    return 0x0;
  }

  /*!
   @brief    Initializes the database.
   */
  static void initialize() {
    if (!_initialized) {
      _initialized = true;
      addBuiltins();
    }
  }

  /*!
   @brief    Adds the built-in factories to the database.

   Sub-classes will specialize this function to add the built-in factory types.
   */
  static void addBuiltins();

  /*!
   @brief    Adds a new Target factory to the database.

   The database takes ownership of the data pointed to. To be added, the new factory must have a
   unique name to all previous actions.  If the factory *cannot* be added, it will be deleted.

   @param    factory    A pointer to the factory to add.
   @returns  True if the factory is successfully added.
   */
  static bool addFactory(Factory* factory) {
    std::string testName(factory->name());
    typename std::list<Factory*>::iterator itr = _factories.begin();
    for (; itr != _factories.end(); ++itr) {
      std::string prevName((*itr)->name());
      if (testName == prevName) {
        logger << Logger::ERR_MSG << "Trying to add a " << getElementName();
        logger << " factory which conflicts with a previous condition factories.\n";
        logger << "\tBoth " << getElementName() << "s use the name: ";
        logger << testName << ".\n";
        logger << "\tDescription of the first " << getElementName();
        logger << " type: " << (*itr)->description() << ".\n";
        logger << "\tDescription of the new " << getElementName();
        logger << " type: " << factory->description() << ".";
        factory->destroy();
        return false;
      }
    }
    _factories.push_back(factory);
    return true;
  }

  /*!
   @brief    Returns the name of the element managed by this database.

   @returns    the name of the element managed by this database
   */
  static std::string getElementName();

  /*!
   @brief    Removes all registered factories from the database.
   */
  static void clear() {
    typename std::list<Factory*>::iterator itr = _factories.begin();
    for (; itr != _factories.end(); ++itr) {
      (*itr)->destroy();
    }
    _factories.clear();
  }

 protected:
  /*!
   @brief    Tracks whether the database has been initialized, so that multiple calls to initialize
            will not cause problems.
   */
  static bool _initialized;

  /*!
   @brief    The registered factories.
   */
  static std::list<Factory*> _factories;
};

// The two functions, addBuiltins and getElementName are *not* defined in-line
//  because of a quirk of visual studio's template system.  By defining the
//  default functionality for the template OUTSIDE the class definition,
//  these can be properly, explicitly specialized in separate C++ files.

template <class Factory, class Element>
void ElementDB<Factory, Element>::addBuiltins() {}

template <class Factory, class Element>
std::string ElementDB<Factory, Element>::getElementName() {
  return "unnamed";
}

template <class Factory, class Element>
bool ElementDB<Factory, Element>::_initialized = false;

template <class Factory, class Element>
std::list<Factory*> ElementDB<Factory, Element>::_factories;
}  // namespace Menge

#endif  // __ELEMENT_DATABASE_H__
