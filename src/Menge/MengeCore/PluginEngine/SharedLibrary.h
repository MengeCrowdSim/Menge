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

///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
/*!
 *	@file		SharedLibrary.h
 *	@brief		Functions for loading and unloading shared libraries.
 */
#ifndef __PLUGIN_SHARED_LIBRARY_H__
#define __PLUGIN_SHARED_LIBRARY_H__

#include "CoreConfig.h"
#include "Logger.h"

#include <string>
#include <stdexcept>
#include <iostream>

#if defined( PLUGIN_WIN32 )

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#elif defined( PLUGIN_LINUX )

#include <dlfcn.h>

#else

#error Please implement the shared library functions for your system

#endif

namespace Menge {

	#if defined( PLUGIN_WIN32 )

	/*!
	 *	@brief		Class with static functions for loading/unloading shared libraries
	 */
	class SharedLibrary {
	public:
		/*!
		 *	@brief		Handle by which DLLs are referenced.
		 */
		typedef HMODULE HandleType;

		/*!
		 *	@brief		Loads the DLL from the given path.
		 *
		 *	@param		Path of the dll to be loaded.
		 *	@returns	The Handle for the DLL
		 *	@throws		std::runtime_error if there is an error in loading the dll
		 */
		MENGE_API static HandleType Load( const std::string &path ) {
			HMODULE moduleHandle = ::LoadLibraryA( path.c_str() );
			if( moduleHandle == NULL ) {
				throw std::runtime_error("Could not load DLL");
			}

			return moduleHandle;
		}

		/*!
		 *	@brief		Unloads the DLL for the given handle
		 *
		 *	@param		The Handle for the DLL
		 *	@throws		std::runtime_error if there is an error in unloading the dll
		 */
		MENGE_API static void Unload( HandleType sharedLibraryHandle ) {
			BOOL result = ::FreeLibrary( sharedLibraryHandle );
			if ( result == FALSE ) {
				throw std::runtime_error ("Could not unload DLL" );
			}
		}

		/*!
		 *	@brief		Templated class for returning a pointer to a function with the
		 *				given signature from the indicated dll.
		 *
		 *	@param		handle		The dll's handle.
		 *	@param		funcName	The name of the function
		 *	@returns	The desired function pointer.
		 *	@throws		std::runtime_error if there is an error in getting the pointer.
		 */
		template< typename TSignature >
		static TSignature * GetFunctionPointer( HandleType handle, const std::string & funcName ) {
			FARPROC functionAddress = ::GetProcAddress( handle, funcName.c_str() );
			if(functionAddress == NULL) {
				throw std::runtime_error("Could not find exported function");
			}

			return reinterpret_cast<TSignature *>(functionAddress);
		}

	};

	#endif

	  // ----------------------------------------------------------------------- //

	#if defined( PLUGIN_LINUX )

	/*!
	 *	@brief		Class with static functions for loading/unloading shared libraries
	 */
	class SharedLibrary {
	public:
		/*!
		 *	@brief		Handle by which shared libraries are referenced.
		 */
		typedef void * HandleType;

		/*!
		 *	@brief		Loads the shared library from the given path.
		 *
		 *	@param		path		Path of the shared library to be loaded.
		 *	@returns	The Handle for the shared library
		 *	@throws		std::runtime_error if there is an error in loading the shared library
		 */
		MENGE_API static HandleType Load( const std::string &path ) {
			void * sharedObject = ::dlopen(path.c_str(), RTLD_NOW);
			if( sharedObject == NULL) {
				logger << Logger::ERR_MSG << dlerror() << "\n";
				throw std::runtime_error( std::string("Could not load '") + path + "'" );
			}

			return sharedObject;
		}

		/*!
		 *	@brief		Unloads the shared library for the given handle
		 *
		 *	@param		sharedLibraryHandle		The Handle for the shared library
		 *	@throws		std::runtime_error if there is an error in unloading the shared library
		 */
		MENGE_API static void Unload(HandleType sharedLibraryHandle) {
			int result = ::dlclose( sharedLibraryHandle );
			if(result != 0) {
				throw std::runtime_error( "Could not unload shared object" );
			}
		}

		/*!
		 *	@brief		Templated class for returning a pointer to a function with the
		 *				given signature from the indicated dll.
		 *
		 *	@param		handle		The dll's handle.
		 *	@param		funcName	The name of the function
		 *	@returns	The desired function pointer.
		 *	@throws		std::runtime_error if there is an error in getting the pointer.
		 */
		template< typename TSignature >
		static TSignature *GetFunctionPointer( HandleType handle, const std::string &funcName ) {
			::dlerror(); // clear error value

			void *functionAddress = ::dlsym( handle, funcName.c_str() );

			const char *error = ::dlerror(); // check for error
			if ( error != NULL ) {
				throw std::runtime_error( "Could not find exported function" );
			}

			return reinterpret_cast<TSignature *>(functionAddress);
		}
	};

	#endif
}	// namespace Menge

#endif // __PLUGIN_SHARED_LIBRARY_H__
