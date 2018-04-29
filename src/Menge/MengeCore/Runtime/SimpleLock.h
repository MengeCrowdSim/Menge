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
 @file  SimpleLock.h
 @brief  The definition of a simple thrading lock
 */

#ifndef __SIMPLE_LOCK_H__
#define __SIMPLE_LOCK_H__

#include "MengeCore/CoreConfig.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace Menge {

/*!
 @brief    The definition of a simple mutex-style lock.

 This lock provides a simple mutex to guarantee thread-safe concurrency. Use of this lock guarantees
 that only one thread at a time can acquire the lock.

 */
class MENGE_API SimpleLock {
 public:
  /*!
   @brief    Constructor.
   */
  SimpleLock();

  /*!
   @brief    Destructor.
   */
  virtual ~SimpleLock();

  /*!
   @brief    Requests access to the mutex.
   
   When this function returns, the lock will be uniquely acquired by the calling thread. The calling
   thread *must* call release otherwise deadlocks may occur.
   */
  void lock() const;

  /*!
   @brief    Releases the acquired lock.
   
   This must only be called by the thread which had previously successfully called lock.
   */
  void release() const;

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
#endif  // __SIMPLE_LOCK_H__
