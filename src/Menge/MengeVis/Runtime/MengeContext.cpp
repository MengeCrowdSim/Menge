#include "MengeVis/Runtime/MengeContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/State.h"
#include "MengeVis/Runtime/AgentContext/AgentContextDatabase.h"
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"
#include "MengeVis/Runtime/StateContext.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"

#include "MengeVis/SceneGraph/Select.h"

namespace MengeVis {
namespace Runtime {

using Menge::Agents::BaseAgent;
using Menge::Agents::SimulatorInterface;
using Menge::BFSM::State;

////////////////////////////////////////////////////////////////////////////
//      Implementation of MengeContext
////////////////////////////////////////////////////////////////////////////

MengeContext::MengeContext(SimulatorInterface* sim)
    : _sim(sim),
      _state(DEFAULT_ST),
      _agentContext(0x0),
      _stateContext(),
      _selected(0x0),
      _visAgents(0x0),
      _agtCount(0x0),
      _digitCount(0),
      _drawVC(true),
      _drawTrans(true) {
  _digits[MAX_TYPE_DIGITS] = '\0';
}

////////////////////////////////////////////////////////////////////////////

MengeContext::~MengeContext() {
  // TODO: Determine what this owns and should clean up.
}

////////////////////////////////////////////////////////////////////////////

SceneGraph::ContextResult MengeContext::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result(false, false);
  SDL_Keymod mods = SDL_GetModState();
  bool hasCtrl = (mods & KMOD_CTRL) > 0;
  bool hasAlt = (mods & KMOD_ALT) > 0;
  bool hasShift = (mods & KMOD_SHIFT) > 0;
  bool noMods = !(hasCtrl || hasAlt || hasShift);

  if (e.type == SDL_KEYDOWN) {
    switch (_state) {
      case DEFAULT_ST: {
        if (hasCtrl && e.key.keysym.sym == SDLK_v) {
          _drawVC = !_drawVC;
          result.set(true, true);
        } else if (hasAlt && e.key.keysym.sym == SDLK_t) {
          _drawTrans = !_drawTrans;
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_RETURN) {
          beginIDTyping();
          result.set(true, true);
        }
      } break;
      case TYPE_AGENT_ID_ST: {
        result.set(true, false);
        if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) ||
            (e.key.keysym.sym >= SDLK_KP_0 && e.key.keysym.sym <= SDLK_KP_9)) {
          int offset;
          if (e.key.keysym.sym <= SDLK_9)
            offset = e.key.keysym.sym - SDLK_0;
          else
            offset = e.key.keysym.sym - SDLK_KP_0;
          char digit = '0' + offset;
          addIDDigit(digit);
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_RETURN) {
          finishIDTyping();
          result.set(true, true);
        } else if (e.key.keysym.sym == SDLK_SPACE) {
          cancelIDTyping();
          result.set(true, true);
        }
      } break;
    }
  }

  if (_selected != 0x0) {
    if (!result.isHandled()) result = _stateContext.handleKeyboard(e);
    if (!result.isHandled()) result = _agentContext->handleKeyboard(e);
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::drawGL(int vWidth, int vHeight) {
  if (_selected != 0x0) {
    getOpenGLView();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    _agentContext->drawGL(vWidth, vHeight);
    const BaseAgent* agt = _selected->getAgent();
    // TODO: Determine if this should be const
    State* state = _sim->getBFSM()->getCurrentState(agt);
    _stateContext.setState(state);
    _stateContext.draw3DGL(agt, _drawVC, _drawTrans);
    drawUIGL(vWidth, vHeight);
    glPopAttrib();
  }
}

////////////////////////////////////////////////////////////////////////////

bool MengeContext::selectGL(const SceneGraph::GLScene* scene, const SceneGraph::GLCamera& camera,
                            int vWidth, int vHeight, int* selectPoint) {
  bool val = SceneGraph::SelectContext::selectGL(scene, camera, vWidth, vHeight, selectPoint);
  if (val) {
    val = updateSelected();
  } else if (_selected) {
    _selected = 0x0;
    val = true;
  }
  return val;
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::activate() { updateSelected(); }

////////////////////////////////////////////////////////////////////////////

bool MengeContext::updateSelected() {
  bool changed = false;
  VisAgent* s = dynamic_cast<VisAgent*>(SceneGraph::Selectable::getSelectedObject());
  changed = s != _selected;
  _selected = s;
  if (changed && s != 0x0) {
    _agentContext = AgentContextDB::getInstance(s);
  }
  return changed;
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::drawUIGL(int vWidth, int vHeight, bool select) {
  if (!select && _selected != 0x0) {
    std::stringstream ss;
    ss << _agentContext->contextName();
    ss << _agentContext->agentText(_selected->getAgent());
    writeToScreen(ss.str(), SceneGraph::TextWriter::LEFT_TOP, 15, 10.f, 10.f);
    writeToScreen(_stateContext.getUIText("   "), SceneGraph::TextWriter::RIGHT_TOP, 15, 10.f,
                  10.f);
    drawIDTyping();
  }
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::beginIDTyping() {
  if (_visAgents != 0x0) {
    _state = TYPE_AGENT_ID_ST;
    _digitCount = 0;
    _digits[0] = '\0';
  }
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::finishIDTyping() {
  _state = DEFAULT_ST;
  size_t id = (size_t)atoi(&_digits[0]);  // I know it must be non-negative
  if (id < _agtCount) {
    VisAgent* s = _visAgents[id];
    if (s != _selected) {
      SceneGraph::Selectable::setSelectedObject(s);
      _selected = s;
    }
  }
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::cancelIDTyping() { _state = DEFAULT_ST; }

////////////////////////////////////////////////////////////////////////////

void MengeContext::drawIDTyping() {
  if (_state == TYPE_AGENT_ID_ST) {
    std::stringstream ss;
    ss << "Type an id to select: " << (&_digits[0]);
    ss << "\n   (Enter to finish, Space to cancel)";
    writeToScreen(ss.str(), SceneGraph::TextWriter::CENTERED, 15);
  }
}

////////////////////////////////////////////////////////////////////////////

void MengeContext::addIDDigit(const char digit) {
  if (_digitCount < MAX_TYPE_DIGITS) {
    // Room to add another digit.
    _digits[_digitCount] = digit;
    _digits[_digitCount + 1] = '\0';
    ++_digitCount;
  }
}

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

}  // namespace Runtime
}  // namespace MengeVis