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
 *	@file	SimpleLock.h
 *	@brief	The definition of a simple thrading lock
 */

#ifndef __SIMPLE_LOCK_H__
#define __SIMPLE_LOCK_H__

#include "CoreConfig.h"
#ifdef _OPENMP
#include <omp.h>
#endif

namespace Menge {

	/*! 
	 *	@brief		The definition of a simple mutex-style lock.
	 *
	 *	This lock provides a simple mutex to guarantee thread-safe concurrency.
	 *	Use of this lock guarantees that only one thread at a time can acquire
	 *	the lock.
	 *
	 */
	class MENGE_API SimpleLock {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		SimpleLock();

		/*!
		 *	@brief		Destructor.
		 */
		virtual ~SimpleLock();

		/*!
		 *	@brief		Requests access to the mutex.  When this function
		 *				returns, the lock will be uniquely acquired by the calling thread.
		 *				The calling thread *must* call release otherwise
		 *				deadlocks may occur.
		 */
		void lock() const;

		/*!
		 *	@brief		Releases the acquired lock.  This must only be called
		 *				by the thread which had previously successfully called lock.
		 */
		void release() const;

	#ifdef _OPENMP
	private:
		/*!
		 *	@brief		The openmp lock used for synchronization.
		 *	
		 *	This is mutable so the corresponding functions can be called in a const
		 *	context.
		 */
		mutable omp_lock_t _lock;
	#endif	// _OPENMP

	};
}	// namespace Menge
#endif	// __SIMPLE_LOCK_H__
