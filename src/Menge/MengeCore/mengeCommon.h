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
 @file    common.h
 @brief   A set of common includes for the core Menge library.
 */

#ifndef __MENGE_COMMON_H__
#define __MENGE_COMMON_H__

#ifdef _MSC_VER
// Visual Studio does not properly support exception specifciation
//   i.e. void func() throw(x)
//  This prevents it from spewing warnings.
#pragma warning(disable : 4290)
#endif

// core configuration
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/Math/consts.h"
#include "MengeCore/Math/vector.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/Runtime/Logger.h"

#include "macros.h"

#include <cassert>
#include <iostream>

#endif  // __MENGE_COMMON_H__
