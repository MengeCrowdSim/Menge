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
 @file    image.h
 @brief   The interface for loading and using images in the scene graph.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "MengeVis/SceneGraph/ManagedData.h"
#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/VisConfig.h"

#include <map>
#include <string>

#include "thirdParty/SDL/SDL_image.h"

// TODO: This resource probably belongs in Core

namespace MengeVis {

/*!
 @brief   Manged image data (see ManagedData).
 */
class MENGEVIS_API ImageData : public ManagedData {
 public:
  /*!
   @brief   Mapping from image resources to its corresponding data.
   */
  static std::map<std::string, ImageData*> RESOURCES;

  /*!
   @brief   Determines if the ImageData::newGLContext() has been registered.
   */
  static bool IS_REGISTERED;

  /*!
   @brief   The function to call when an OpenGL context has changed.
   */
  static void newGLContext();

  /*!
   @brief   Initializes the image system.

   Must be called before images can be used. Essentially, it calls the SDL Image initialization.
   */
  static void start();

  /*!
   @brief   Called when done using images.
   */
  static void release();

  /*!
   @brief   Constructor.
   */
  ImageData();

  /*!
   @brief   Destructor.
   */
  ~ImageData();

  /*!
   @brief   Returns the width of the image (in pixels).

   @returns   The width of the image (in pixels).
   */
  int getWidth() const { return _img->w; }

  /*!
   @brief   Returns the height of the image (in pixels).

   @returns   The height of the image (in pixels).
   */
  int getHeight() const { return _img->h; }

  /*!
   @brief   Reports the bits per pixel.

   @returns   Either 24 or 32, depending on whether the image has an alpha channel (32) or not (24).
   */
  int getBpp() const;

  /*!
   @brief   Initializes the image for drawing in an OpenGL context.
   */
  void initGL() const;

  /*!
   @brief   Draws the image onto a quad, centered on the origin of the x-y plane.
   */
  void drawGL() const;

  /*!
   @brief   Binds the image to use as a texture in OpenGL.
   */
  void bind() const;

  /*!
   @brief   Loads the image from a file (specified by name).

   @param   fileName    The path to a valid image file.
   @returns A pointer to the underlying ImageData. If there is an error, NULL is returned.
   */
  friend ImageData* readImageData(const std::string& fileName);

  /*!
   @brief   Returns the red channel of the image at the given pixel.

   @param   x   The x-coordinate of the pixel.
   @param   y   The y-coordinate of the pixel.
   @returns The red channel of the image at the given pixel.
   */
  unsigned char getPixelAt(int x, int y) const;

 private:
  /*!
   @brief   Indicates whether the image uses per-pixel alpha.
   */
  bool _useAlpha;

  /*!
   @brief   The OpenGL texture id for the image.
   */
  mutable GLuint _texture;

  /*!
   @brief   The SDL surface which contains the image data.
   */
  SDL_Surface* _img;
};

/*!
 @brief   Wrapper for the managed ImageData.
 */
class MENGEVIS_API Image : public ManagedDataWrapper<ImageData> {
 public:
  /*!
   @brief   Construtor.

   @param   Data  An instance of ImageData.
   */
  Image(ImageData* data);

  /*!
   @brief   Destructor.
   */
  ~Image();

  /*!
   @brief   Binds the image to use as a texture in OpenGL.
   */
  void bind() const { _data->bind(); }

  /*!
   @brief   Returns the image data.

   @returns   A const pointer to the underlying image data.
   */
  const ImageData* data() const { return _data; }

  /*!
   @brief   Returns the width of the image (in pixels).

   @returns   The width of the image (in pixels).
   */
  int getWidth() const { return _data->getWidth(); }

  /*!
   @brief   Returns the height of the image (in pixels).

   @returns The height of the image (in pixels).
   */
  int getHeight() const { return _data->getHeight(); }
};

/*!
 @brief   Given a filename, returns a pointer to an Image with that data.

 @param   fileName    The unique path to the image.
 @returns A pointer to an Image instance. If there is a problem loading the image, NULL is returned.
          The caller is responsible for deleting the pointer.
 */
MENGEVIS_API Image* loadImage(const std::string& fileName);
}  // namespace MengeVis

#endif  // __IMAGE_H__
