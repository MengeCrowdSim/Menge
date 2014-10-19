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

#include "ReadersWriterLock.h"
#include <iostream>

namespace Menge {

/////////////////////////////////////////////////////////////////////
//					Implementation of ReadersWriterLock
/////////////////////////////////////////////////////////////////////

// NOTE: This implemenation is naively simple.
//	OpenMP does not allow for sophisticated readers-writer locks.  It contains
//	no semaphores.  So, this implementation simply serializes ALL reads and
//	writes.  Not optimal, but safe.
//
// TODO: Update this implementation when the threading paradigm is changed to
//	support more sophisticated constructs.

	#ifdef _OPENMP

	ReadersWriterLock::ReadersWriterLock() {
		omp_init_lock( &_lock );
	}

	///////////////////////////////////////////////////////////////////////

	ReadersWriterLock::~ReadersWriterLock() {
		omp_destroy_lock( &_lock );
	}

	///////////////////////////////////////////////////////////////////////

	// NOTE: The code below is left in place, commented out, to facilitate debugging
	//	of future concurrency issues.
	void ReadersWriterLock::lockRead() const {
		//#pragma omp critical( READER_LOCK ) 
		//{
		//std::cout << "READ LOCK( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//}
		omp_set_lock( &_lock );
		//#pragma omp critical( READER_LOCK ) 
		//	{
		//	std::cout << "\t READ LOCKED( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//	}
	}

	///////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::releaseRead() const {
		//#pragma omp critical( READER_LOCK ) 
		//{
		//std::cout << "READ RELEASE( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//}
		omp_unset_lock( &_lock );
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::lockWrite() const {
		//#pragma omp critical( READER_LOCK ) 
		//{
		//std::cout << "WRITE LOCK( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//}
		omp_set_lock( &_lock );
		//#pragma omp critical( READER_LOCK ) 
		//	{
		//	std::cout << "\t WRITE LOCKED( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//	}
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::releaseWrite() const {
		//#pragma omp critical( READER_LOCK ) 
		//{
		//std::cout << "WRITE RELEASE( " << ((size_t)this) << ") - " << omp_get_thread_num() << "\n";
		//}
		omp_unset_lock( &_lock );
	}

	#else	// _OPENMP

	ReadersWriterLock::ReadersWriterLock() {
	}

	/////////////////////////////////////////////////////////////////////

	ReadersWriterLock::~ReadersWriterLock() {
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::lockRead() const {
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::releaseRead() const {
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::lockWrite() const {
	}

	/////////////////////////////////////////////////////////////////////

	void ReadersWriterLock::releaseWrite() const {
	}

#endif	// _OPENMP
}	// namespace Menge