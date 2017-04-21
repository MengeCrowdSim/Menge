#include "MengeVis/Runtime/EventInjectContext.h"

#include "MengeCore/Core.h"
#include "MengeCore/Agents/Events/EventSystem.h"
#include "MengeCore/Agents/Events/EventTriggerExternal.h"

namespace MengeVis {
	namespace Runtime {

		using SceneGraph::Context;
		using SceneGraph::ContextResult;
		using SceneGraph::GLCamera;

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of EventInjectContext
		////////////////////////////////////////////////////////////////////////////

		EventInjectionContext::EventInjectionContext(Context * ctx) : _childContext(ctx) {
			identifyTriggers();
		}

		////////////////////////////////////////////////////////////////////////////

		void EventInjectionContext::drawGL(int vWidth, int vHeight) {
			if (_childContext) _childContext->drawGL(vWidth, vHeight);
		}

		////////////////////////////////////////////////////////////////////////////

		bool EventInjectionContext::selectGL(const SceneGraph::GLScene * scene,
			const GLCamera & camera, int vWidth, int vHeight, int * selectPoint) {
			if (_childContext) 
				return _childContext->selectGL(scene, camera, vWidth, vHeight, selectPoint);
			return false;
		}

		////////////////////////////////////////////////////////////////////////////

		ContextResult EventInjectionContext::handleMouse( SDL_Event & e ) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				auto itr = _triggers.find(e.button.button);
				if (itr != _triggers.end()) {
					Menge::EVENT_SYSTEM->activateExternalTrigger(itr->second);
				}
			} else if (e.type == SDL_MOUSEWHEEL) {
				auto itr = _triggers.find(e.wheel.y > 0 ? WHEEL_UP : WHEEL_DOWN);
				if (itr != _triggers.end()) {
					Menge::EVENT_SYSTEM->activateExternalTrigger(itr->second);
				}
			}
			if (_childContext) return _childContext->handleMouse(e);
			return ContextResult(false, false);
		}

		////////////////////////////////////////////////////////////////////////////

		ContextResult EventInjectionContext::handleKeyboard( SDL_Event & e) {
			// TODO: Process the mouse for *this* context - it never does anything with the 
			//	context result.
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
		}

		////////////////////////////////////////////////////////////////////////////

		void EventInjectionContext::draw3DGL(bool select) {
			// NOTE: I don't have to call the drawUIGL of the child context; it will take care
			// of itself by invoking its drawGL (as I've done above).
		}

		////////////////////////////////////////////////////////////////////////////

		void EventInjectionContext::identifyTriggers() {
			_triggers.clear();
			const auto& triggers = Menge::EVENT_SYSTEM->listExternalTriggers();
			for (const std::string& triggerName : triggers) {
				if (triggerName == "left_arrow") {
					_triggers[SDLK_LEFT] = triggerName;
				} else if (triggerName == "right_arrow") {
					_triggers[SDLK_RIGHT] = triggerName;
				} else if (triggerName == "up_arrow") {
					_triggers[SDLK_UP] = triggerName;
				} else if (triggerName == "down_arrow") {
					_triggers[SDLK_DOWN] = triggerName;
				} else if (triggerName == "spacebar") {
					_triggers[SDLK_SPACE] = triggerName;
				} else if (triggerName == "left_mouse") {
					_triggers[SDL_BUTTON_LEFT] = triggerName;
				} else if (triggerName == "right_mouse") {
					_triggers[SDL_BUTTON_RIGHT] = triggerName;
				} else if (triggerName == "middle_mouse") {
					_triggers[SDL_BUTTON_MIDDLE] = triggerName;
				} else if (triggerName == "mouse_wheel_up") {
					_triggers[WHEEL_UP] = triggerName;
				} else if (triggerName == "mouse_wheel_down") {
					_triggers[WHEEL_DOWN] = triggerName;
				}
			}
		}

	}	// namespace Runtime
}	// namespace MengeVis
