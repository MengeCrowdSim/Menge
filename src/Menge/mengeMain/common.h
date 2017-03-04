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
 *	@file		common.h
 *	@brief		A set of common includes for the main application.
 */

#ifndef __COMMON_H__
#define	__COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#include "GL/gl.h"
#include <iostream>
#include <cassert>
#include "macros.h"
#include "math/vector.h"
#include "math/consts.h"
#include "math/Matrix.h"
using namespace Menge::Math; 

#endif	 // __COMMON_H__