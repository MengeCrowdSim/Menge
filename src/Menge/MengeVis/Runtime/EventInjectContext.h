#pragma once
/*!
 @file    EventInjectContext.h
 @brief   The definition of a context that captures ExternalEventTriggers in the system and maps
          them to user actions.
 */

#include "MengeCore/mengeCommon.h"
#include "MengeVis/SceneGraph/Context.h"

// forward declarations
namespace Menge {
class ExternalEvtTrigger;
}
namespace MengeVis {
class Image;
}

// Uncomment to include mouse wheel events. Although they are currently not *visualized*.
//#define USE_MOUSE_WHEEL

namespace MengeVis {
namespace Runtime {
/*!
 @brief   A context that connects user actions to Menge::ExternalEventTrigger.

 The EventInjectionContext serves as a layer between UI and the simulator. It keys on
 ExternalEventTrigger instances with particular names. Each of the specific names is triggered by a
 specific user action. The following table shows the actions and the name of the trigger that gets
 fired:

 User Action               | Trigger Name
 --------------------------|---------------------
 Press left arrow          | left_arrow
 Press right arrow         | right_arrow
 Press up arrow            | up_arrow
 Press down arrow          | down_arrow
 Click left mouse button   | left_mouse
 Click right mouse button  | right_mouse
 Click middle mouse button | middle_mouse
 @cond
   Roll mouse wheel up       | mouse_wheel_up
   Roll mouse wheel down     | mouse_wheel_down
 @endcond

 Note: no mouse data is provided (i.e., mouse position, state of modifiers, etc.). The simple user
 action is sufficient to fire the trigger.

 The EventInjectionContext also contains a child context. Events that trigger simulation events are
 _also_ propagated down to the child context.

 Finally, if the EvenEjectionContext finds appropriate external triggers, it will display the valid
 event actions in the display (the display of which is toggle-able by the user).
 */
class MENGEVIS_API EventInjectionContext : public SceneGraph::Context {
 public:
  /*!
   @brief   Constructor.

   @param   ctx   The optional pass-through context.
   */
  EventInjectionContext(SceneGraph::Context* ctx = nullptr);

  /*!
   @brief   Destructor.
   */
  ~EventInjectionContext();

  /*!
   @brief   The draw function for the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   */
  void drawGL(int vWidth, int vHeight) override;

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
  bool selectGL(const SceneGraph::GLScene* scene, const SceneGraph::GLCamera& camera, int vWidth,
                int vHeight, int* selectPoint) override;

  /*!
   @brief   Give the context the opportunity to respond to a mouse event.

   @param   e   The SDL event with the mouse event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  SceneGraph::ContextResult handleMouse(SDL_Event& e) override;

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  SceneGraph::ContextResult handleKeyboard(SDL_Event& e) override;

  /*!
   @brief   Allow the context to update any time-dependent state it might have to the given global
            time.
   */
  void update() override;

  /*!
   @brief   Callback for when the OpenGL context is changed.
   */
  void newGLContext() override;

  /*!
   @brief   Called when the context is activated.
   */
  void activate() override;

  /*!
   @brief   Called when the context is deactivated.
   */
  void deactivate() override;

 protected:
  /*!
   @brief   Draw UI elements into the context.

   @param   vWidth    The width of the viewport (in pixels).
   @param   vHeight   The height of the viewport (in pixels).
   @param   select    Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  void drawUIGL(int vWidth, int vHeight, bool select = false) override;

 private:
  // This queries the menge simulator for external triggers, configuring the context.
  void identifyTriggers();

  // The optional child context.
  SceneGraph::Context* _childContext;

  // The display image.
  Image* _image;

  // Identifiers for the registered event triggers.
  enum Event {
    RIGHT_ARROW = 0,
    LEFT_ARROW,
    UP_ARROW,
    DOWN_ARROW,
    LEFT_MOUSE,
    MIDDLE_MOUSE,
    RIGHT_MOUSE,
#ifdef USE_MOUSE_WHEEL
    WHEEL_UP,
    WHEEL_DOWN,
#endif
    TOTAL_EVENTS,
  };

  // Map from SDL enum values to the trigger name that it triggers.
  //  KeyCodes are signed 32-bit ints
  //  Mouse codes are unsigned 32-bit ints
  //  Mouse wheel requires test of the sign of the wheel motion.
  HASH_MAP<unsigned int, std::string> _triggers;

  // Reports if *any* events are connected.
  bool _anyConnected{false};

  // TODO: VS2013 doesn't support initializaiton lists on arrays. Restore the initialization
  //  list after killing VS2013.

#if 1
  // A boolean indicating which events are connected -- indexed by enumeration.
  bool _isConnected[TOTAL_EVENTS];

  // The dimensions of each of the event actuator regions in the image listed as:
  // min x, min y, max x, max y.
  // These values are a hard-coded property of the underlying image file. If the image
  // changes, these must change. They are expressed as *fractions* of the image with
  // the lower-right hand corner serving as the origin.
  float _imageDimensions[TOTAL_EVENTS][4];

#else
  // A boolean indicating which events are connected -- indexed by enumeration.
  bool _isConnected[TOTAL_EVENTS] = {
      false,  // RIGHT_ARROW
      false,  // LEFT_ARROW
      false,  // UP_ARROW
      false,  // DOWN_ARROW
      false,  // LEFT_MOUSE
      false,  // MIDDLE_MOUSE
      false,  // RIGHT_MOUSE
#ifdef USE_MOUSE_WHEEL
      false,  // WHEEL_UP
      false,  // WHEEL_DOWN
#endif
  };

  // The dimensions of each of the event actuator regions in the image listed as:
  // min x, min y, max x, max y.
  // These values are a hard-coded property of the underlying image file. If the image
  // changes, these must change. They are expressed as *fractions* of the image with
  // the lower-right hand corner serving as the origin.
  float _imageDimensions[TOTAL_EVENTS][4]{
      {0.783938815f, 0.062240664f, 0.986615679f, 0.502074689f},  // RIGHT_ARROW
      {0.378585086f, 0.062240664f, 0.58126195f, 0.502074689f},   // LEFT_ARROW
      {0.58126195f, 0.502074689f, 0.783938815f, 0.941908714f},   // UP_ARROW
      {0.58126195f, 0.062240664f, 0.783938815f, 0.502074689f},   // DOWN_ARROW
      {0.015296367f, 0.572614108f, 0.105162524f, 0.970954357f},  // LEFT_MOUSE
      {0.105162524f, 0.572614108f, 0.210325048f, 0.970954357f},  // MIDDLE_MOUSE
      {0.210325048f, 0.572614108f, 0.307839388f, 0.970954357f},  // RIGHT_MOUSE
  };
#endif
};
}  // namespace Runtime
}  // namespace MengeVis
