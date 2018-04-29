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
 @file    VisConfig.h
 @brief   Sets up the proper compiler directives for platform and dll export/import
 */

#ifndef __VIS_CONFIG_H__
#define __VIS_CONFIG_H__

#if defined(_MSC_VER)
// Exposing templated instances in a dll requires special handling
// There is no problem as long as all code is built on the same machine.
// Until this is finally corrected, we will simply suppress the warning.
//
// TODO: Correct the problem and remove the warning suppression.
#pragma warning(disable : 4251)
#endif

//// First determine compiling environment
#if defined(WIN32) || defined(_WIN32)
/*!
 @brief   Defines the build environment as windows.
 */
#define PLUGIN_WIN32 1
#else
/*!
 @brief   Defines the build environment as linux.
 */
#define PLUGIN_LINUX 1
#endif

// Now set up compiler-dependent export/import symbols

#if defined(_MSC_VER)
#if defined(MENGEVIS_STATICLIB)
#define MENGEVIS_API
#else
#if defined(MENGEVIS_EXPORTS)
// We are building the DLL, export the symbols tagged like this
#define MENGEVIS_API __declspec(dllexport)
#else
// If we are consuming the DLL, import the symbols tagged like this
#define MENGEVIS_API __declspec(dllimport)
#endif
#endif

#elif defined(__GNUC__)

#define MATHEXTERN
#if defined(MENGEVIS_STATICLIB)
#define MENGEVIS_API
#else
#if defined(MENGEVIS_EXPORTS)
#define MENGEVIS_API __attribute__((visibility("default")))
#else
// If you use -fvisibility=hidden in GCC, exception handling and RTTI
// would break if visibility wasn't set during export _and_ import
// because GCC would immediately forget all type infos encountered.
// See http://gcc.gnu.org/wiki/Visibility
#define MENGEVIS_API __attribute__((visibility("default")))
#endif
#endif

#else

#error Unknown compiler, please implement shared library macros

#endif  // Compiler-detection

#endif  // __VIS_CONFIG_H__
