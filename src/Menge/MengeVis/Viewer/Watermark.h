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
 @file    Watermark.h
 @brief   Data for controlling a watermark.
 */

#ifndef __WATER_MARK_H__
#define __WATER_MARK_H__

#include <string>

// forward declaration
class TiXmlElement;

namespace MengeVis {

class Image;

namespace Viewer {
/*!
 @brief   The potential alignment of the watermark.
 */
enum WatermarkAlign {
  NO_ALIGN,      ///< Undefined alignment.
  CENTERED,      ///< Center the image in the screen.
  BOTTOM_LEFT,   ///< Place the image in the bottom-left corner of the screen.
  BOTTOM_RIGHT,  ///< Place the image in the bottom-right corner of the screen.
  TOP_LEFT,      ///< Place the image in the top-left corner of the screen.
  TOP_RIGHT      ///< Place the image in the top-right corner of the screen.
};

/*!
 @brief   A class for handling the watermark functionality.

 A watermark is a semi-transparent image drawn over the top of the scene.
 */
class Watermark {
 public:
  /*!
   @brief   Constructor.
   */
  Watermark();

  /*!
   @brief   Destructor.
   */
  ~Watermark();

  /*!
   @brief   Returns the file name for the water mark.
   */
  const std::string& getFilename() const { return _fileName; }

  /*!
   @brief   Draws the watermark to the screen.

   @param   w   The width of the screen (in pixels).
   @param   h   The height of the screen (in pixels).
   */
  void drawGL(float w, float h) const;

  /*!
   @brief   Reports the alignment of the watermark.
   */
  WatermarkAlign get_alignment() const { return _alignment; }

  /*!
   @brief   Reports the opacity of the watermark.
   */
  float get_opacity() const { return _opacity; }

  /*!
   @brief   Reports the scale of the watermark.
   */
  float get_scale() const { return _scale; }

  friend Watermark* parseWatermark(TiXmlElement* node, const std::string& viewFldr);

 protected:
  /*!
   @brief   File name of the watermark.
   */
  std::string _fileName;

  /*!
   @brief   A pointer to the image the watermark uses.
   */
  Image* _img;

  /*!
   @brief   The alignment of the watermark.
   */
  WatermarkAlign _alignment;

  /*!
   @brief   The opacity of the watermark.
   
   Should be a value in the range [0, 1], where 0 is completely transparent and 1 is completely
   opaque.
   */
  float _opacity;

  /*!
   @brief   The scale of the watermark.

   If the alignment is FULL_SCREEN, this can cause tiling.
   */
  float _scale;
};

/*!
 @brief   Parses a water mark xml specification and returns the corresponding instance.

 @param   node        The XML node containing the watermark definition.
 @param   viewFldr    The path to the view configuration file.
 @returns A pointer to a water mark, if the definition was valid (NULL otherwise).
 */
Watermark* parseWatermark(TiXmlElement* node, const std::string& viewFldr);
}  // namespace Viewer
}  // namespace MengeVis
#endif  // __WATER_MARK_H__