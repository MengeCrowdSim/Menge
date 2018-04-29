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
 @file    Resource.h
 @brief    The basic class for all on-disk resources.
 */

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/Runtime/SimpleLock.h"
#include "MengeCore/resources/ResourceManager.h"

#include <string>

namespace Menge {

/*!
 @brief    A base exception for resources to throw.
 */
class MENGE_API ResourceException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  ResourceException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ResourceException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal resource exception.
 */
class MENGE_API ResourceFatalException : public ResourceException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  ResourceFatalException() : MengeException(), ResourceException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ResourceFatalException(const std::string& s)
      : MengeException(s), ResourceException(), MengeFatalException() {}
};

/*!
 @brief    Basic class for managing on-disk resources.

 This is for data structures that are defined in files but may be used by multiple entities. This
 prevents unnecessary loading and duplication and allows for centralized memory management.
 */
class MENGE_API Resource {
 public:
  /*!
   @brief    Constructor.

   This does not initialize the data structure.  It merely registers the resource.

   @param    fileName    The path to the on-disk resource.
   */
  Resource(const std::string& fileName) : _fileName(fileName), _refCount(0) {}

  /*!
   @brief    This supplants the destructor.

   In order to preserve potential problems in windows when dlls do not share the same c-runtime
   library, the destructor is held to be private. To garbage collect a Resource, the destroy method
   should be called (which in turn, will call the destructor from its own memory space, averting
   run-time crashes).

   Once this has been called, the Resource no longer exists. Calling methods or accessing members
   will produce indetermine behavior (most likely errors).
   */
  void destroy();

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~Resource() {}

 public:
  /*!
   @brief    Return the file name for this resource.

   @returns  The file name associated with this resource.
   */
  const std::string& getName() const { return _fileName; }

  /*!
   @brief    Increment references to the managed data.

   Any object that carries a pointer to managed data should call this function when acquiring and
   storing a pointer to that data.

   @returns  The total number of references.
   */
  int incRef() {
    _lock.lock();
    int val = ++_refCount;
    _lock.release();
    return val;
  }

  /*!
   @brief    Decrement references to the managed data.

   Any object that carries a pointer to managed data should call this function upon destruction
   (thereby removing its reference from the managed data's reference count.

   @returns  The number of remaining references.
   */
  int decRef() {
    _lock.lock();
    int val = --_refCount;
    _lock.release();
    return val;
  }

  /*!
   @brief    Reports if the data is referenced.

   Any object that carries a pointer to managed data, after dereferencing itself upon destruction,
   should determine if it is no longer referenced and delete the object if this function returns
   true.  NOTE: this is not thread-safe.

   @returns  True if the underlying managed data is no longer referenced.
   */
  bool isUnreferenced() const {
    _lock.lock();
    bool val = _refCount <= 0;
    _lock.release();
    return val;
  }

  /*!
   @brief    Returns a unique resource label to be used to identify different resource *types* which
            use the same underlying file data.
   */
  virtual const std::string& getLabel() const = 0;

  friend class ResourceManager;

 protected:
  /*!
   @brief    The file which contains the resource's data.
   */
  const std::string _fileName;

  /*!
   @brief    The number of data wrappers using this managed data.
   */
  int _refCount;

  /*!
   @brief    Simple lock to handle reference counts safely.
   */
  SimpleLock _lock;
};

/*!
 @brief    Base Class providing a pointer interface to Resources
 */
template <class Rsrc>
class ResourcePtr {
 public:
  /*!
   @brief    Constructor.

   @param    rsrc    A pointer to the underlying data
   */
  ResourcePtr(Rsrc* rsrc = 0x0) : _data(rsrc) {
    if (_data) _data->incRef();
  }

  /*!
   @brief    Copy constructor.

   @param    rPtr    A resource pointer to the underlying data.
   */
  ResourcePtr(const ResourcePtr<Rsrc>& rPtr) : _data(rPtr._data) {
    if (_data) _data->incRef();
  }

  /*!
   @brief    Destructor
   */
  ~ResourcePtr() {
    if (_data) {
      _data->decRef();
      if (_data->isUnreferenced()) {
        ResourceManager::removeResource(_data);
      }
    }
  }

  /*!
   @brief    Assignment operator

   @param    ptr    A resource pointer to the underlying data.
   @returns  this object, having replaced
   */
  ResourcePtr<Rsrc>& operator=(const ResourcePtr<Rsrc>& ptr) {
    if (this != &ptr) {
      if (_data) {
        _data->decRef();
        if (_data->isUnreferenced()) ResourceManager::removeResource(_data);
      }
      _data = ptr._data;
      if (_data) _data->incRef();
    }
    return *this;
  }

  /*!
   @brief    The indirection operator.

   @returns    Returns a pointer to the underlying data
   */
  Rsrc* operator->() const { return _data; }

  /*!
   @brief    Reports if to Resource pointers (of the same type) refer to the same data.

   @param    ptr    A pointer of the same type.
   @returns  True if the underlying data is the same in each pointer
   */
  bool operator==(const ResourcePtr<Rsrc>& ptr) const { return _data == ptr._data; }

  /**
   @brief    Reports if the resource pointer contains data.

   @returns  True if there is data contained in the pointer.
   */
  bool hasData() const { return _data != 0x0; }

 protected:
  /*!
   @brief    The underlying resource data.
   */
  Rsrc* _data;
};
}  // namespace Menge

#endif  // __RESOURCE_H__
