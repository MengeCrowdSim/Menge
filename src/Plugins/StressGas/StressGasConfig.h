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

#ifndef __STRESSGAS_CONFIG_H__
#define __STRESSGAS_CONFIG_H__

/*!
@namespace		StressGAS
@brief			The name space for the GAS-based pedestrian stress model.

Generalized Adaptive Syndrome (GAS) is a model that describes how people's behavior changes as
stress increases. This plugins provides functionality for introducing stress onto agents and
observing corresponding changes in their behaviors. The primary element in the plug-in is the
BaseStressAction and its derivatives which control when an agent begins to accumulate stress, how it
accumualtes, how stress stops accumulating, and what happens afterwards.
*/
namespace StressGAS {}
// Now set up compiler-dependent export/import symbols

#if defined(_MSC_VER)

#if defined(STATICLIB)
#define EXPORT_API
#else
#if defined(EXPORT)
// We are building the DLL, export the symbols tagged like this
#define EXPORT_API __declspec(dllexport)
#else
// If we are consuming the DLL, import the symbols tagged like this
#define EXPORT_API __declspec(dllimport)
#endif
#endif

#elif defined(__GNUC__)

#if defined(STATICLIB)
#define EXPORT_API
#else
#if defined(EXPORT)
#define EXPORT_API __attribute__((visibility("default")))
#else
// If you use -fvisibility=hidden in GCC, exception handling and RTTI
// would break if visibility wasn't set during export _and_ import
// because GCC would immediately forget all type infos encountered.
// See http://gcc.gnu.org/wiki/Visibility
#define EXPORT_API __attribute__((visibility("default")))
#endif
#endif

#else

#error Unknown compiler, please implement shared library macros

#endif  // Compiler-detection

#endif  // __ProxyAgents_CONFIG_H__
