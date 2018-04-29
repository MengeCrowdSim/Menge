#include "MengeVis/Runtime/EventInjectContext.h"

#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/Agents/Events/EventTriggerExternal.h"
#include "MengeCore/Core.h"
#include "MengeVis/SceneGraph/image.h"

#ifdef _MSC_VER
#include "windows.h"
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif

namespace MengeVis {
namespace Runtime {

using SceneGraph::Context;
using SceneGraph::ContextResult;
using SceneGraph::GLCamera;

////////////////////////////////////////////////////////////////////////////
//      Implementation of EventInjectContext
////////////////////////////////////////////////////////////////////////////

EventInjectionContext::EventInjectionContext(Context* ctx) : _childContext(ctx), _image(nullptr) {
  // TODO: I'm initializing these values here because VS2013 doesn't support
  // initialization at declaration of array data members.
  for (int i = 0; i < TOTAL_EVENTS; ++i) {
    _isConnected[i] = false;
  }
  float dims[TOTAL_EVENTS][4]{
      {0.783938815f, 0.062240664f, 0.986615679f, 0.502074689f},  // RIGHT_ARROW
      {0.378585086f, 0.062240664f, 0.58126195f, 0.502074689f},   // LEFT_ARROW
      {0.58126195f, 0.502074689f, 0.783938815f, 0.941908714f},   // UP_ARROW
      {0.58126195f, 0.062240664f, 0.783938815f, 0.502074689f},   // DOWN_ARROW
      {0.015296367f, 0.572614108f, 0.105162524f, 0.970954357f},  // LEFT_MOUSE
      {0.105162524f, 0.572614108f, 0.210325048f, 0.970954357f},  // MIDDLE_MOUSE
      {0.210325048f, 0.572614108f, 0.307839388f, 0.970954357f},  // RIGHT_MOUSE
  };
  for (int r = 0; r < TOTAL_EVENTS; ++r) {
    for (int c = 0; c < 4; ++c) {
      _imageDimensions[r][c] = dims[r][c];
    }
  }

  _image = loadImage("images/user_action_interface.png");
  identifyTriggers();
}

EventInjectionContext::~EventInjectionContext() {
  if (_image) delete _image;
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::drawGL(int vWidth, int vHeight) {
  if (_childContext) _childContext->drawGL(vWidth, vHeight);
  if (_anyConnected) drawUIGL(vWidth, vHeight);
}

////////////////////////////////////////////////////////////////////////////

bool EventInjectionContext::selectGL(const SceneGraph::GLScene* scene, const GLCamera& camera,
                                     int vWidth, int vHeight, int* selectPoint) {
  if (_childContext) return _childContext->selectGL(scene, camera, vWidth, vHeight, selectPoint);
  return false;
}

////////////////////////////////////////////////////////////////////////////

ContextResult EventInjectionContext::handleMouse(SDL_Event& e) {
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    auto itr = _triggers.find(e.button.button);
    if (itr != _triggers.end()) {
      Menge::EVENT_SYSTEM->activateExternalTrigger(itr->second);
    }
  }
#ifdef USE_MOUSE_WHEEL
  else if (e.type == SDL_MOUSEWHEEL) {
    auto itr = _triggers.find(e.wheel.y > 0 ? WHEEL_UP : WHEEL_DOWN);
    if (itr != _triggers.end()) {
      Menge::EVENT_SYSTEM->activateExternalTrigger(itr->second);
    }
  }
#endif
  if (_childContext) return _childContext->handleMouse(e);
  return ContextResult(false, false);
}

////////////////////////////////////////////////////////////////////////////

ContextResult EventInjectionContext::handleKeyboard(SDL_Event& e) {
  // TODO: Process the mouse for *this* context - it never does anything with the
  //  context result.
  if (e.key.type == SDL_KEYDOWN) {
    auto itr = _triggers.find(e.key.keysym.sym);
    if (itr != _triggers.end()) {
      Menge::EVENT_SYSTEM->activateExternalTrigger(itr->second);
    }
  }
  if (_childContext) return _childContext->handleKeyboard(e);
  return ContextResult(false, false);
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::update() {
  if (_childContext) return _childContext->update();
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::newGLContext() {
  if (_childContext) return _childContext->newGLContext();
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::activate() {
  if (_childContext) return _childContext->activate();
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::deactivate() {
  if (_childContext) return _childContext->deactivate();
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::drawUIGL(int vWidth, int vHeight, bool select) {
  // NOTE: I don't have to call the drawUIGL of the child context; it will take care
  // of itself by invoking its drawGL (as I've done above).
  const float iW = static_cast<float>(_image->getWidth());
  const float iH = static_cast<float>(_image->getHeight());
  const float aspectRatio = iW / iH;

  const float targetWidth = 100.f;
  const float targetHeight = targetWidth / aspectRatio;

  // set up rendering
  glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, vWidth, 0.0, vHeight, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glScalef(targetWidth, targetHeight, 1.f);

  _image->bind();
  const float opacity = 1.f;
  glColor4f(1.f, 1.f, 1.f, opacity);
  if (opacity < 1.f) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  const float u = 1.f;
  const float v = 1.f;

  glColor4f(0.5f, 0.5f, 0.5f, opacity);
  glBegin(GL_QUADS);
  glTexCoord2f(0.f, 1.f - 0.f);
  glVertex3f(0.f, 0.f, 0.f);

  glTexCoord2f(1.f, 1.f - 0.f);
  glVertex3f(1.f, 0.f, 0.f);

  glTexCoord2f(1.f, 1.f - 1.f);
  glVertex3f(1.f, 1.f, 0.f);

  glTexCoord2f(0.f, 1.f - 1.f);
  glVertex3f(0.f, 1.f, 0.f);

  for (int i = 0; i < TOTAL_EVENTS; ++i) {
    if (_isConnected[i])
      glColor4f(1.f, 1.f, 1.f, opacity);
    else
      glColor4f(0.5f, 0.5f, 0.5f, opacity);

    float min_x = _imageDimensions[i][0];
    float min_y = _imageDimensions[i][1];
    float max_x = _imageDimensions[i][2];
    float max_y = _imageDimensions[i][3];
    glTexCoord2f(min_x, 1.f - min_y);
    glVertex3f(min_x, min_y, 0.f);

    glTexCoord2f(max_x, 1.f - min_y);
    glVertex3f(max_x, min_y, 0.f);

    glTexCoord2f(max_x, 1.f - max_y);
    glVertex3f(max_x, max_y, 0.f);

    glTexCoord2f(min_x, 1.f - max_y);
    glVertex3f(min_x, max_y, 0.f);
  }
  glEnd();

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopAttrib();
}

////////////////////////////////////////////////////////////////////////////

void EventInjectionContext::identifyTriggers() {
  _triggers.clear();
  const auto& triggers = Menge::EVENT_SYSTEM->listExternalTriggers();
  auto connect = [this](int index) {
    this->_isConnected[index] = true;
    this->_anyConnected = true;
  };
  for (const std::string& triggerName : triggers) {
    if (triggerName == "left_arrow") {
      _triggers[SDLK_LEFT] = triggerName;
      connect(LEFT_ARROW);
    } else if (triggerName == "right_arrow") {
      _triggers[SDLK_RIGHT] = triggerName;
      connect(RIGHT_ARROW);
    } else if (triggerName == "up_arrow") {
      _triggers[SDLK_UP] = triggerName;
      connect(UP_ARROW);
    } else if (triggerName == "down_arrow") {
      _triggers[SDLK_DOWN] = triggerName;
      connect(DOWN_ARROW);
    } else if (triggerName == "left_mouse") {
      _triggers[SDL_BUTTON_LEFT] = triggerName;
      connect(LEFT_MOUSE);
    } else if (triggerName == "right_mouse") {
      _triggers[SDL_BUTTON_RIGHT] = triggerName;
      connect(RIGHT_MOUSE);
    } else if (triggerName == "middle_mouse") {
      _triggers[SDL_BUTTON_MIDDLE] = triggerName;
      connect(MIDDLE_MOUSE);
    }
#ifdef USE_MOUSE_WHEEL
    else if (triggerName == "mouse_wheel_up") {
      _triggers[WHEEL_UP] = triggerName;
      _isConnected[WHEEL_UP] = true;
    } else if (triggerName == "mouse_wheel_down") {
      _triggers[WHEEL_DOWN] = triggerName;
      _isConnected[WHEEL_DOWN] = true;
    }
#endif
  }
}

}  // namespace Runtime
}  // namespace MengeVis
