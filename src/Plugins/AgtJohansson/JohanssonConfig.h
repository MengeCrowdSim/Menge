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
 @file		JohanssonConfig.h
 @brief		Configures the shared library aspect of the includes.
 */

#ifndef __JOHANSSON_CONFIG_H__
#define __JOHANSSON_CONFIG_H__

// Now set up compiler-dependent export/import symbols

#if defined(_MSC_VER)

#if defined(JOHANSSON_STATICLIB)
#define JOHANSSON_API
#else
#if defined(JOHANSSON_EXPORT)
// We are building the DLL, export the symbols tagged like this
#define JOHANSSON_API __declspec(dllexport)
#else
// If we are consuming the DLL, import the symbols tagged like this
#define JOHANSSON_API __declspec(dllimport)
#endif
#endif

#elif defined(__GNUC__)

#if defined(JOHANSSON_STATICLIB)
#define JOHANSSON_API
#else
#if defined(JOHANSSON_EXPORT)
#define JOHANSSON_API __attribute__((visibility("default")))
#else
// If you use -fvisibility=hidden in GCC, exception handling and RTTI
// would break if visibility wasn't set during export _and_ import
// because GCC would immediately forget all type infos encountered.
// See http://gcc.gnu.org/wiki/Visibility
#define JOHANSSON_API __attribute__((visibility("default")))
#endif
#endif

#else

#error Unknown compiler, please implement shared library macros

#endif  // Compiler-detection

#endif  // __JOHANSSON_CONFIG_H__
