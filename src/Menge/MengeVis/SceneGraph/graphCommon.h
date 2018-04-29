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
 @file    graphCommon.h
 @brief   Various important pre-compiler directives for the scene graph.
 */

#ifndef __GRAPH_COMMON_H__
#define __GRAPH_COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif
#include "MengeCore/Math/Matrix.h"
#include "MengeCore/Math/consts.h"
#include "MengeCore/Math/vector.h"
//#include "mengeCommon.h"

#ifndef GL_BGR
/*!
 @brief   OpenGL constant for determing pixel order for 3-color textures.
 */
#define GL_BGR 0x80E0
#endif

#ifndef GL_BGRA
/*!
 @brief   OpenGL constant for determing pixel order for 4-color textures.
 */
#define GL_BGRA 0x80E1
#endif

#ifndef GL_CLAMP_TO_EDGE
/*!
 @brief   OpenGL constant for determing texture behavior.
 */
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#endif  // __GRAPH_COMMON_H__
