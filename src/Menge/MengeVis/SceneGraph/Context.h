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
 @file    Context.h
 @brief Defines a context for handling user interaction (mouse and keyboard input).
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "MengeCore/Math/vector.h"
#include "MengeVis/SceneGraph/GLCamera.h"
#include "MengeVis/SceneGraph/TextWriter.h"  // should this be in viewer?
#include "MengeVis/VisConfig.h"

// forward declaration
union SDL_Event;

/*!
 @brief   Forward declaration of the SDL event type.
 */
typedef SDL_Event SDL_Event;

namespace MengeVis {

/*!
 @namespace   MengeVis::SceneGraph
 @brief       The namespace that contains the basic components of a 3D scene for visualization.
 */
namespace SceneGraph {
// forward declarations
class Context;
class GLScene;

// TODO: decouple this from SDL events
/*!
 @brief   Defines the result of the context's consideration of user input.
 */
class MENGEVIS_API ContextResult {
 public:
  /*!
   @brief   Constructor

   @param   handled       Set to true if the event has been handled and does not need to be
                          considered by any other handler.
   @param   needsRedraw   Set to true if the processing of the event requires a redraw.
   */
  ContextResult(bool handled, bool needsRedraw) : _handled(handled), _redraw(needsRedraw) {}

  /*!
   @brief   Simple assignment operator overloaded.

   @param   res   Const reference to the ContextResult instance to copy from.
   @returns  A reference to this context result.
   */
  ContextResult& operator=(const ContextResult& res) {
    _handled = res._handled;
    _redraw = res._redraw;
    return *this;
  }

  /*!
   @brief   Sets the "handled" state to the given boolean state.

   @param   state   True means the result should consider the event handled, otherwise false.
   */
  inline void setHandled(bool state) { _handled = state; }

  /*!
   @brief   Reports if the result considers the event handled.

   @returns True if the result considers the event handled, false otherwise.
   */
  inline bool isHandled() const { return _handled; }

  /*!
   @brief   Sets the "needs redraw" state to the given boolean state.

   @param   state   True means the result should believe the event requires a redraw, otherwise
                    false.
   */
  inline void setNeedsRedraw(bool state) { _redraw = state; }

  /*!
   @brief   Reports if the result believes the event handling requires a redraw.

   @returns True if the result requires a redraw, false otherwise.
   */
  inline bool needsRedraw() const { return _redraw; }

  /*!
   @brief   Sets the handled and needs redraw state simultaneously.

   @param   handled   True if the result should consider the event handled, false otherwise.
   @param   redraw    True means the result should believe the event requires a redraw, otherwise
                      false.
   */
  inline void set(bool handled, bool redraw) {
    _handled = handled;
    _redraw = redraw;
  }

  /*!
   @brief   Combines the provided context result with this one; flags are combined with a boolean
            OR.

   @param   res   The context results to combine with this.
   */
  inline void combine(const ContextResult& res) {
    _handled = _handled || res._handled;
    _redraw = _redraw || res._redraw;
  }

  friend class Context;

 protected:
  /*!
   @brief   Reports if the event has been handled (and no one else needs to worry about it).
   */
  bool _handled;

  /*!
   @brief   Reports if the event requires a redraw on the scene.
   */
  bool _redraw;
};

/*!
 @brief   The base context class for defining a how events are handled.

 It also is responsible drawing UI elements in both screen space and in viewer world space. While
 the main viewer handles basic view manipulation, all other interactions should be handled with a
 context.
 */
class MENGEVIS_API Context {
 public:
  /*!
   @brief   Constructor.
   */
  Context() {}

  /*!
   @brief   Virtual destructor.
   */
  virtual ~Context() {}

  /*!
   @brief   The draw function for the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   */
  virtual void drawGL(int vWidth, int vHeight) {}

  /*!
   @brief   Performs selection based on a click on screen space. 
   
   Uses the OpenGL selection mechanism.

   @param   scene         The scene to select in.
   @param   camera        The camera.
   @param   vWidth        The width of the viewport.
   @param   vHeight       The height of the viewport.
   @param   selectPoint   The point (in screen space) at which object selection should take place.
   @returns A boolean indicating whether a redraw needs to take place.
   */
  virtual bool selectGL(const GLScene* scene, const GLCamera& camera, int vWidth, int vHeight,
                        int* selectPoint) {
    return false;
  }

  /*!
   @brief   Give the context the opportunity to respond to a mouse event.

   @param   e   The SDL event with the mouse event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual ContextResult handleMouse(SDL_Event& e) { return ContextResult(false, false); }

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual ContextResult handleKeyboard(SDL_Event& e) { return ContextResult(false, false); }

  /*!
   @brief   Allow the context to update any time-dependent state it might have to the given global
            time.
   */
  virtual void update() {}

  /*!
   @brief   Callback for when the OpenGL context is changed.
   */
  virtual void newGLContext() {}

  /*!
   @brief   Called when the context is activated.
   */
  virtual void activate() {}

  /*!
   @brief   Called when the context is deactivated.
   */
  virtual void deactivate() {}

 protected:
  /*!
   @brief   UI setup - handles the configuration of the opengl context for orthographic rendering.

   @param   vWidth    The width of the view (in pixels).
   @param   vHeight   The height of the view (in pixels).
   */
  void uiSetup(int vWidth, int vHeight);

  /*!
   @brief   Cleans up the OpenGL state after having drawn the UI elements of the context.
   */
  void uiShutdown();

  /*!
   @brief   Draw UI elements into the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   @param   select    Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  virtual void drawUIGL(int vWidth, int vHeight, bool select = false){};

  /*!
   @brief   Draw context elements into the 3D world.

   @param   select    Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  virtual void draw3DGL(bool select = false){};

  /*!
   @brief   Updates the various view matrices: modelview, projection, and view.
   */
  void getOpenGLView();

  /*!
   @brief   Writes the given text at the given location

   @param   txt         The text to write to the screen.
   @param   pos         The position, in world coordinates, to place the text.
   @param   currColor   Indicates if the current opengl color should be used (true) or the text
                        writer's default color should be used (false).
   */
  void writeText(const std::string& txt, const Menge::Math::Vector2& pos,
                 bool currColor = false) const;

  /*!
   @brief   Writes the given text at the given 3D location

   @param   txt         The text to write to the screen.
   @param   pos         The position, in world coordinates, to place the text.
   @param   currColor   Indicates if the current opengl color should be used (true) or the text
                        writer's default color should be used (false).
   */
  void writeText(const std::string& txt, const Menge::Math::Vector3& pos,
                 bool currColor = false) const;

  /*!
   @brief   Writes the given text at the given location, but anchored based on the radial direction
            given.

   @param   txt         The text to write to the screen.
   @param   pos         The position, in world coordinates, to place the text.
   @param   dir         A vector representing the direction of the radius at whose end the text
                        should be . Direction defined in world coordinates.
   @param   currColor   Indicates if the current opengl color should be used (true) or the text
                        writer's default color should be used (false).
   */
  void writeTextRadially(const std::string& txt, const Menge::Math::Vector2& pos,
                         const Menge::Math::Vector2& dir, bool currColor = false) const;

  /*!
   @brief   Writes the given text aligned to the given location.

   @param   txt         The text to write to the screen.
   @param   pos         The position, in world coordinates, to place the text.
   @param   align       TextWriter alignment specification.
   @param   currColor   Indicates if the current opengl color should be used (true) or the text
                        writer's default color should be used (false).
   */
  void writeAlignedText(const std::string& txt, const Menge::Math::Vector2& pos,
                        TextWriter::Alignment align, bool currColor = false) const;

  /*!
   @brief   Writes the given text to the screen based on the given alignment.

   The text will be written to one of 9 locations:
      - Top left corner, top center, top right corner
      - left center, screen center, right center
      - bottom left corner, bottom center, bottom right corner

   @param   txt         The text to write to the screen.
   @param   align       TextWriter alignment specification.
   @param   fontSize    The size of the text to write.
   @param   hPad        The number of pixels to pad the text horizontally.
   @param   vPad        The number of pixels to pad the text vertically.
   @param   currColor   Indicates if the current opengl color should be used (true) or the text
                        writer's default color should be used (false).
   @param   trans       Determines if the text is printed transparently (true) or not (false).
   */
  void writeToScreen(const std::string& txt, TextWriter::Alignment align, int fontSize,
                     float hPad = 0.f, float vPad = 0.f, bool currColor = false,
                     bool trans = true) const;

  /*!
   @brief   The current model view matrix.
   */
  double _modViewMat[16];

  /*!
   @brief   The current projection matrix.
   */
  double _projMat[16];

  /*!
   @brief   The current viewport parameters.
   */
  int _viewMat[4];
};

/*!
 @brief   A context that performs object selection by directly clicking on selectable entities in
          the OpenGL context.
 */
class MENGEVIS_API SelectContext : public Context {
 public:
  /*!
   @brief   Performs selection based on a click on screen space.

   Uses the OpenGL selection mechanism.

   @param   scene         The scene to select in.
   @param   camera        The camera.
   @param   vWidth        The width of the viewport.
   @param   vHeight       The height of the viewport.
   @param   selectPoint   The point (in screen space) at which object selection should take place.
   @returns A boolean indicating whether a redraw needs to take place.
   */
  virtual bool selectGL(const GLScene* scene, const GLCamera& camera, int vWidth, int vHeight,
                        int* selectPoint);
};
}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __CONTEXT_H__
