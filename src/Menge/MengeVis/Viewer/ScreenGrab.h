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
 @file    ScreenGrab.h
 @brief   Functionality for capturing screen grabs to the file system.
 */

#ifndef __SCREENGRAB_H__
#define __SCREENGRAB_H__

namespace MengeVis {

/*!
 @brief   Function to create a png from the current screenbuffer

 @param   windowWidth     The current width of the Menge window.
 @param   windowHeight    The current height of the Menge window.
 @param   filename        Path to the file to write.

 @returns True if the output was successful
 */
bool snapshotPNG(int windowWidth, int windowHeight, const char* filename);
}  // namespace MengeVis
#endif  // __SCREENGRAB_H__