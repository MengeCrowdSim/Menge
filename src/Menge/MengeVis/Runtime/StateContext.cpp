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

#include "MengeVis/Runtime/StateContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/State.h"
#include "MengeVis/Runtime/GoalRenderer/GoalRendererDatabase.h"
#include "MengeVis/Runtime/VCContext/VelCompContextDatabase.h"

#include <limits>
#include <sstream>

#ifdef _WIN32
#undef max
#endif

namespace MengeVis {

namespace Runtime {

using Menge::Agents::BaseAgent;
using Menge::BFSM::Goal;
using Menge::BFSM::State;
using MengeVis::Runtime::GoalRendererDB;
using MengeVis::Runtime::VCContextDB;

/////////////////////////////////////////////////////////////////////
//                   Implementation of StateContext
/////////////////////////////////////////////////////////////////////

size_t StateContext::NO_ACTIVE_ID = std::numeric_limits<size_t>::max();

/////////////////////////////////////////////////////////////////////

StateContext::StateContext() : _state(0x0) {}

/////////////////////////////////////////////////////////////////////

StateContext::~StateContext() {}

/////////////////////////////////////////////////////////////////////

void StateContext::setState(State* state) {
  if (_state == 0x0 || _state->getID() != state->getID()) {
    _state = state;
    _vcContext = VCContextDB::getInstance(_state->getVelComponent());
    _activeTransition = _state->getTransitions().size() == 1 ? 0 : NO_ACTIVE_ID;
  }
}

/////////////////////////////////////////////////////////////////////

SceneGraph::ContextResult StateContext::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result(false, false);

  result = _vcContext->handleKeyboard(e);
  if (!result.isHandled() && _activeTransition != NO_ACTIVE_ID) {
    SDL_Keymod mods = SDL_GetModState();
    bool hasCtrl = (mods & KMOD_CTRL) > 0;
    bool hasAlt = (mods & KMOD_ALT) > 0;
    bool hasShift = (mods & KMOD_SHIFT) > 0;
    bool noMods = !(hasCtrl || hasAlt || hasShift);
    // TODO: Once I have transition contexts
    // if ( e.type == SDL_KEYDOWN ) {
    //  if ( !hasCtrl && !hasAlt && hasShift ) {
    //    if ( e.key.keysym.sym == SDLK_UP ) {
    //      ++_activeTransition;
    //      if ( _activeTransition >= _transContexts.size() ) {
    //        _activeTransition = NO_ACTIVE_ID;
    //      }
    //      result.set( true, true );
    //    }
    //  }
    //}
  }
  return result;
}

/////////////////////////////////////////////////////////////////////

std::string StateContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "State: " << _state->getName();
  std::string childIndent = indent + "    ";

  ss << "\n" << childIndent << "Display (Ctrl-V)elocity Component";
  ss << "\n" << _vcContext->getUIText(childIndent + "    ");

#if 1
  ss << "\n" << childIndent << "Transitions not yet supported";
#else
  if (_activeTransition == NO_ACTIVE_ID) {
    ss << "\n"
       << childIndent << _state->transitions_.size() << " transitions (Alt + up arrow to select)";
  } else {
    ss << "\n" << childIndent << "Display (Alt-T)ransition";
    ss << "\n" << childIndent << "    Transition w/o context selected";
  }
#endif
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

void StateContext::draw3DGL(const BaseAgent* agt, bool drawVC, bool drawTrans) {
  const Goal* goal = _state->getGoal(agt->_id);
  // TODO: this should not be a const cast
  _goalRenderer = GoalRendererDB::getInstance(goal);
  _goalRenderer->drawGL();
  if (drawVC) {
    _vcContext->draw3DGL(agt, goal);
  }
  if (drawTrans && _activeTransition != NO_ACTIVE_ID) {
  }
}
}  // namespace Runtime
}  // namespace MengeVis
