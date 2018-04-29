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
 @file    TextWriter.h
 @brief   Functionality for writing text on the OpenGL context.
 */

#ifndef __TEXT_WRITER_H__
#define __TEXT_WRITER_H__

#include "MengeVis/SceneGraph/image.h"
#include "MengeVis/VisConfig.h"

#include "thirdParty/SDL/SDL_ttf.h"

#include <map>
#include <string>

namespace MengeVis {

namespace SceneGraph {
/*!
 @brief   An STL map to map from font size to SDL font class.
 */
typedef std::map<int, TTF_Font*> FontMap;

/*!
 @brief   An iterator for the FontMap.
 */
typedef FontMap::iterator FontMapItr;

/*!
 @brief   A const iterator for the FontMap.
 */
typedef FontMap::const_iterator FontMapCItr;

/*!
 @brief   A singleton class responsible for writing text to an OpenGL context.

 It has numerous functions for drawing text. It typically works in screen space, where the upper,
 left-hand corner is (0, 0).
 */
class MENGEVIS_API TextWriter {
  /*!
   @brief   The name of the default font to use.
   */
  static std::string DEFAULT_FONT;

  /*!
   @brief   The default font size.
   */
  static const int DEFAULT_SIZE;

 public:
  /*!
   @brief   Sets the default font to use.

   @param   fontName    The path to the font to use.
   */
  static void setDefaultFont(const std::string& fontName);

  /*!
   @brief   Enumeration for controlling text alignment.
   */
  enum Alignment {
    NO_ALIGN = 0x0,       // 0
    VCENTER = 0x1,        // 1
    TOP = 0x2,            // 2
    BOTTOM = 0x4,         // 4
    LEFT = 0x8,           // 8
    LEFT_CENTER = 0x9,    // 9
    LEFT_TOP = 0xA,       // 10
    LEFT_BOTTOM = 0xC,    // 12
    RIGHT = 0x10,         // 16
    RIGHT_CENTER = 0x11,  // 17
    RIGHT_TOP = 0x12,     // 18
    RIGHT_BOTTOM = 0x14,  // 20
    HCENTER = 0x20,       // 32
    CENTERED = 0x21,      // 33
    CENTER_TOP = 0x22,    // 34
    CENTER_BOTTOM = 0x24  // 36
  };

  /*!
   @brief   Acquire pointer to a singleton text writer instance.
   */
  static TextWriter* Instance();

  /*!
   @brief   Resets the text writer's context-dependent data; called when the OpenGL context is 
            called.
   */
  void newGLContext();

  /*!
   @brief   Print the given text with an alignment relative to the given anchor point.

   @param   text          The string to draw.
   @param   alignment     The alignment, relative to the screen.
   @param   anchorX       The x-position of the anchor point (in screen coords).
   @param   anchorY       The y-position of the anchor point (in screen coords).
   @param   fontSize      The size of the text, in pixels.
   @param   currColor     Indicates if the current opengl color should be used (true) or the text
                          writer's default color should be used false).
   @param   transparent   Indicates if the text is super-imposed over the screen or given amatte.
                          True --> superimposed, false --> matted.
   */
  void printText(const std::string& text, Alignment alignment, float anchorX, float anchorY,
                 int fontSize, bool currColor = false, bool transparent = true);

  /*!
   @brief   Print the given text to the screen at the given coordinates.

   @param   text          The string to draw.
   @param   left          The position of the left extent of the text (in screen coords).
   @param   bottom        The position of the bottom extent of the text (in screen coords).
   @param   fontSize      The size of the text, in pixels.
   @param   currColor     Indicates if the current opengl color should be used (true) or the text
                          writer's default color should be used false).
   @param   transparent   Indicates if the text is super-imposed over the screen or given a matte.
                          True --> superimposed, false --> matted.
   */
  void printText(const std::string& text, float left, float bottom, int fontSize,
                 bool currColor = false, bool transparent = true);

  /*!
   @brief   Print the given text to the screen with the specified alignment.

   @param   text          The string to draw.
   @param   alignment     The alignment, relative to the screen.
   @param   fontSize      The size of the text, in pixels.
   @param   hpad          The amount of horizontal padding to apply in the horizontal alignment
                          direction.  For example, if left aligned, it goes on the left, if right
                          aligned, the padding is on the right.
   @param   vpad          The amount of vertical padding to apply in the vertical direction.
   @param   currColor     Indicates if the current opengl color should be used (true) or the text
                          writer's default color should be used false).
   @param   transparent   Indicates if the text is super-imposed over the screen or given a matte.
                          True --> superimposed, false --> matted.
   */
  void printAlignText(const std::string& text, Alignment alignment, int fontSize, float hpad,
                      float vpad, bool currColor = false, bool transparent = true);

  /*!
   @brief   Prints the text anchored to the position.
   
   The relationship to the anchor depends on the anchor weights.  It interpolates them from bottom
   left top top right.

   @param   text          The string to draw.
   @param   anchorX       The x-position of the anchor point (in screen coords).
   @param   anchorY       The y-position of the anchor point (in screen coords).
   @param   xWeight       The horizontal weight in the range [0, 1]. 0--> left, 1 --> right
                          alignment.
   @param   yWeight       The vertical weight in the range [0, 1]. 0--> bottom, 1 --> top alignment.
   @param   fontSize      The size of the text, in pixels.
   @param   currColor     Indicates if the current opengl color should be used (true) or the text
                          writer's default color should be used false).
   @param   transparent   Indicates if the text is super-imposed over the screen or given a matte.
                          True --> superimposed, false --> matted.
   */
  void printAnchorText(const std::string& text, float anchorX, float anchorY, float xWeight,
                       float yWeight, int fontSize, bool currColor = false,
                       bool transparent = true);

  /*!
   @brief   Reports the size, in pixels, the given text will consume on the screen, given current
            font and indicated font size.

   @param   text          The string to draw.
   @param   fontSize      The size of the font (in points).
   @param   textWidth     The width of the text will be stored in this value.
   @param   textHeight    The height of the text will be stored in this value.
   */
  void textSize(const std::string& text, int fontSize, float& textWidth, float& textHeight);

  /*!
   @brief   Inform the text writer that the viewport has resized.

   This must be called by the viewer when the viewport resizes. This information is how the
   TextWriter correctly interprets alignment.

   @param   w   The viewport width (in pixels).
   @param   h   The viewport height (in pixels).
   */
  void resize(int w, int h);

  /*!
   @brief   Reports TextWriter's belief of the viewport width.

   @returns The effective viewport width the TextWrtier is using.
   */
  inline int getViewWidth() const { return _width; }

  /*!
   @brief   Reports TextWriter's belief of the viewport width.

   @returns The effective viewport width the TextWrtier is using.
   */
  inline int getViewHeight() const { return _height; }

  /*!
   @brief   Changes the font family

   The first font is loaded at the default size. If it is unable to load the font, the old family is
   left in place

   @param   fontName    The name of the font family to use.
   @returns True if the font was successfully loaded, false otherwise. If false, it uses the
            default font.
   */
  bool setFont(const std::string& fontName);

  /*!
   @brief   Sets the default font color.

   @param   r       The red component of the color.
   @param   g       The green component of the color.
   @param   b       The blue component of the color.
   @param   alpha   The alpha component of the color. Default value is 1.0.
   */
  void setDefaultColor(float r, float g, float b, float alpha = 1.f);

  /*!
   @brief   A utility function to map from screen to image coordinates.

   The screen coordiantes have the origin in the top, left corner. Image coordinates have the
   origin at the *bottom*, left corner.

   @param   y   The vertical position in screen coordinates.
   @returns The vertical position in corresponding image coordinates.
   */
  inline int flipY(int y) { return _height - y; }

  // TODO: Support multiple families of fonts

 protected:
  /*!
   @brief   Returns a pointer to a font for the given size.

   @param   size    The desired size of the font.
   @returns A pointer to a font instance at the desired size. If size is negative, NULL is returned.
   */
  TTF_Font* getFont(int size);

  /*!
   @brief   Close out the current set of fonts.
   */
  void closeFonts();

  /*!
   @brief   Protected constructor.  No instantiation allowed.
   */
  TextWriter();

  /*!
   @brief   Protected destructor
   */
  ~TextWriter();

  /*!
   @brief   The singleton pointer.
   */
  static TextWriter* _instance;

  /*!
   @brief   The width of the viewport the TextWriter considers when computing text positions.
   */
  int _width;

  /*!
   @brief   The height of the viewport the TextWriter considers when computing text positions.
   */
  int _height;

  /*!
   @brief   Generate the display list for the quad upon which text is printed.
   */
  void makeTextDList();

  /*!
   @brief   Generate the OpenGL texture object for the text.
   */
  void makeGLTexture();

  /*!
   @brief   Set the surface upon which the text has been rendered to a texture.

   @param   surf    The surface upon which the text has been rendered.
   */
  void sdlSurfaceToGLTex(SDL_Surface* surf);

  /*!
   @brief   The work necessary to print a single line of text.

   The text is displayed relative to the anchor point of an entire *block* of text, (left, bottom),
   withthe text extending above and to the right of the anchor point.

   @param   font      The font to render the text.
   @param   text      The text to draw.
   @param   left      The horizontal value of the anchor point.
   @param   bottom    The vertical value of the anchor point.
   @param   lineNum   The line number of the given text (zero-indexed). Used to offset this line
                      relative to the anchor point.
   */
  void printLine(TTF_Font* font, const char* text, float left, float bottom, int lineNum);

  /*!
   @brief   Map of all font sizes for this font face.
   */
  FontMap _fonts;

  /*!
   @brief   Name of font.
   */
  std::string _fontName;

  /*!
   @brief   Texture object for texturing the rendered text.
   */
  GLuint _textID;

  /*!
   @brief   Call list for the text surface.
   */
  GLuint _textQuadID;

  /*!
   @brief   The default font color.
   */
  float _defColor[4];
};
}  // namespace SceneGraph
}  // namespace MengeVis
#endif  //__TEXT_WRITER_H__
