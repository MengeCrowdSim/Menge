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

#include "MengeVis/Viewer/ScreenGrab.h"

#include "MengeCore/Runtime/Logger.h"

#include "thirdParty/SDL/SDL.h"
#include "thirdParty/SDL/SDL_image.h"
#include "thirdParty/SDL/SDL_surface.h"
#ifdef _WIN32  // only supported under windows
#include "windows.h"
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <string.h>
#include "GL/gl.h"
#endif
#include <iostream>

namespace MengeVis {

using Menge::logger;
using Menge::Logger;

bool snapshotPNG(int width, int height, const char* path) {
  static int oldHeight = 0;
  static int oldWidth = 0;
  static GLubyte* image = 0x0;
  static GLubyte* rev_image = 0x0;
  const int size = width * height * 3;  // # of bytes in image.
  if (oldHeight != height || oldWidth != width) {
    oldHeight = height;
    oldWidth = width;
    if (image) {
      delete[] image;
      delete[] rev_image;
    }
    image = new GLubyte[size];
    rev_image = new GLubyte[size];
  }

  // Acquire the image
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  Uint32 rmask, gmask, bmask, amask;
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0;
  const int pitch = width * 3;

  // The rows of data in image are vertically flipped from what they need to be in the surface.
  GLubyte* tgt = rev_image + size - pitch;
  for (GLubyte* src = image; src < image + size; src += pitch) {
    memcpy(tgt, src, pitch);
    tgt -= pitch;
  }

  SDL_Surface* surf =
      SDL_CreateRGBSurfaceFrom(rev_image, width, height, 24, pitch, rmask, gmask, bmask, amask);
  IMG_SavePNG(surf, path);
  SDL_FreeSurface(surf);

  return true;
}

}  // namespace MengeVis
