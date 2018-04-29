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
 @file    ResourceManager.h
 @brief    The basic class for all on-disk resources.
 */

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "MengeCore/mengeCommon.h"

#include <map>
#include <string>

namespace Menge {

// Forward declaration
class Resource;

/*!
 @brief    Type declaration for a resource map - mapping file names to resource pointers.
 */
typedef std::map<std::string, Resource*> ResourceMap;

/*!
 @brief    Class to handle management of on-disk resources.
 */
class MENGE_API ResourceManager {
 public:
  /*!
   @brief    Retrieve a resource from the manager.

   @param    fileName  The name of the file associated with the resource.
   @param    reader    Pointer to a function for parsing the given file and producing a resource
                      instance.
   @param    suffix    The string to append to the file name. This allows different *types* of
                      resources basedon on the same file resource to be distinguished. It is the
                      burden of the programmer to make sure each type provides a unique suffix,
                      otherwise problems will arise between suffix collisions.
   @returns    A pointer to the reference, if it is loaded, NULL otherwise. The caller is responsible
              for knowing what type of resource it should be and test it using a dynamic-cast.
   */
  static Resource* getResource(const std::string& fileName, Resource* (*reader)(const std::string&),
                               const std::string& suffix);

  /*!
   @brief    Passes through the resources and removes all unreferenced resources.
   */
  static void cleanup();

  /*!
   @brief    Remove the given resource

   @param    rsrc    A pointer to the resource to remove.
   @returns  True if the pointer is successfully removed, false otherwise. It can fail if the
            resource doesn't belong to the manager or if the resources reference count is not zero.
   */
  static bool removeResource(Resource* rsrc);

 protected:
  /*!
   @brief    Mapping from resource name to resource.
   */
  static ResourceMap _resources;

 private:
  /*!
   @brief    The string used to concatenate filenames with resource type suffixes.
   */
  static const std::string CAT_SYMBOL;
};
}  // namespace Menge
#endif  // __RESOURCE_MANAGER_H__
