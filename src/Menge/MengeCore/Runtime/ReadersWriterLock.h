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
 @file  ReadersWriterLock.h
 @brief  The definition of a readers-writer lock
 */

#ifndef __READERS_WRITER_LOCK_H__
#define __READERS_WRITER_LOCK_H__

#include "MengeCore/CoreConfig.h"
#ifdef _OPENMP
#include <omp.h>
#endif

namespace Menge {

/*!
 @brief    The definition of a readers-writer lock.

 A readers-writer lock can be used to secure a resource for concurrent usage such that multiple
 readers can safely utilize the resource but writing tasks must have sole access.

 */
class MENGE_API ReadersWriterLock {
 public:
  /*!
   @brief    Constructor.
   */
  ReadersWriterLock();

  /*!
   @brief    Destructor.
   */
  virtual ~ReadersWriterLock();

  /*!
   @brief    Requests access to read a resource.
   
   When this function returns, the resource will be safe to read. The calling thread *must* call
   releaseRead otherwise deadlocks may occur.
   */
  void lockRead() const;

  /*!
   @brief    Releases the lock for reading.
   
   This must only be called by threads which had previously successfully called lockRead.
   */
  void releaseRead() const;

  /*!
   @brief    Requests access to write a resource.
   
   When this function returns, the resource will be safe to write. The calling thread *must* call
   releaseWrite otherwise deadlocks *will* occur.
   */
  void lockWrite() const;

  /*!
   @brief    Releases the lock for writing.
   
   This must only be called by threads which had previously successfully called lockWrite.
   */
  void releaseWrite() const;

#ifdef _OPENMP
 private:
  /*!
   @brief    The openmp lock used for synchronization.

   This is mutable so the corresponding functions can be called in a const context.
   */
  mutable omp_lock_t _lock;
#endif  // _OPENMP
};
}  // namespace Menge
#endif
