#include "MengeVis/Runtime/VCContext/VecFieldVCContext.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/VelocityComponents/VelCompVF.h"
#include "MengeCore/resources/VectorField.h"
#include "MengeVis/Runtime/VisElementException.h"

#include <iomanip>
#include <sstream>

namespace MengeVis {
namespace Runtime {
namespace VCContext {

using Menge::VectorFieldPtr;
using Menge::Agents::BaseAgent;
using Menge::Agents::PrefVelocity;
using Menge::BFSM::Goal;
using Menge::BFSM::VFVelComponent;
using Menge::Math::Vector2;

/////////////////////////////////////////////////////////////////////
//                   Implementation of VecFieldVCContext
/////////////////////////////////////////////////////////////////////

VecFieldVCContext::VecFieldVCContext()
    : VelCompContext(), _vc(0x0), _showLocal(true), _neighborhood(5) {}

/////////////////////////////////////////////////////////////////////

void VecFieldVCContext::setElement(const Menge::BFSM::VelComponent* vc) {
  _vc = dynamic_cast<const VFVelComponent*>(vc);
  if (_vc == 0x0) {
    throw VisElementException(
        "Trying to set a velocity field velocity component "
        "context with an invalid value: either null or "
        "wrong velocity component type.");
  }
}

/////////////////////////////////////////////////////////////////////

std::string VecFieldVCContext::getElementName() const { return VFVelComponent::NAME; }

/////////////////////////////////////////////////////////////////////

std::string VecFieldVCContext::getUIText(const std::string& indent) const {
  std::stringstream ss;
  ss << indent << "Vector field velocity component";
  ss << "\n" << indent << "    Toggle (Ctrl-L)ocal display";
  ss << "\n" << indent << "        Neighborhood: ";
  ss << _neighborhood << " cells (Ctrl-up/down to change)";
  return ss.str();
}

/////////////////////////////////////////////////////////////////////

SceneGraph::ContextResult VecFieldVCContext::handleKeyboard(SDL_Event& e) {
  SceneGraph::ContextResult result(false, false);

  SDL_Keymod mods = SDL_GetModState();
  bool hasCtrl = (mods & KMOD_CTRL) > 0;
  bool hasAlt = (mods & KMOD_ALT) > 0;
  bool hasShift = (mods & KMOD_SHIFT) > 0;
  bool noMods = !(hasCtrl || hasAlt || hasShift);

  if (e.type == SDL_KEYDOWN) {
    if (hasCtrl && !hasAlt && !hasShift) {
      if (e.key.keysym.sym == SDLK_UP) {
        ++_neighborhood;
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_DOWN) {
        --_neighborhood;
        if (_neighborhood <= 1) {
          _neighborhood = 1;
        }
        result.set(true, true);
      } else if (e.key.keysym.sym == SDLK_l) {
        _showLocal = !_showLocal;
        result.set(true, true);
      }
    }
  }

  return result;
}

/////////////////////////////////////////////////////////////////////

void VecFieldVCContext::draw3DGL(const BaseAgent* agt, const Goal* goal) {
  glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  // draw the grid
  int rowZero = 0;
  const VectorFieldPtr vf = _vc->getVectorField();
  int lastRow = vf->getRowCount();
  int colZero = 0;
  int lastCol = vf->getColCount();
  const float cellSize = vf->getCellSize();
  Vector2 maxCorner = vf->getMaximumPoint();
  Vector2 minCorner = vf->getMinimumPoint();
  float left = minCorner.x();
  float right = maxCorner.x();
  float bottom = minCorner.y();
  float top = maxCorner.y();

  if (_showLocal) {
    int r, c;
    vf->getCell(agt->_pos, r, c);

    rowZero = r - _neighborhood;
    if (rowZero < 0) rowZero = 0;
    int tmp = r + _neighborhood + 1;
    if (tmp < lastRow) lastRow = tmp;

    colZero = c - _neighborhood;
    if (colZero < 0) colZero = 0;
    tmp = c + _neighborhood + 1;
    if (tmp < lastCol) lastCol = tmp;

    left = minCorner.x() + colZero * cellSize;
    bottom = minCorner.y() + rowZero * cellSize;
    right = minCorner.x() + lastCol * cellSize;
    top = minCorner.y() + lastRow * cellSize;
  }

  glLineWidth(1.f);
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);
  // lines parallel w/ x-axis
  for (int r = rowZero; r <= lastRow; ++r) {
    float y = minCorner.y() + r * cellSize;
    glVertex3f(left, y, 0.f);
    glVertex3f(right, y, 0.f);
  }

  // lines parallel w/ y-axis
  for (int c = colZero; c <= lastCol; ++c) {
    float x = minCorner.x() + c * cellSize;
    glVertex3f(x, bottom, 0.f);
    glVertex3f(x, top, 0.f);
  }
  glEnd();

  // draw the vectors
  float y = bottom + 0.5f * cellSize;
  glColor3f(1.f, 0.5f, 0.f);
  glBegin(GL_LINES);
  const float UNIT_SCALE = cellSize * 0.45f;
  for (int r = rowZero; r < lastRow; ++r) {
    float x = left + 0.5f * cellSize;
    for (int c = colZero; c < lastCol; ++c) {
      Vector2 dir = vf->getFieldValue(r, c) * UNIT_SCALE;
      Vector2 end(dir.x() + x, dir.y() + y);
      glVertex3f(x, y, 0.f);
      glVertex3f(end.x(), end.y(), 0.f);
      x += cellSize;
    }
    y += cellSize;
  }
  glEnd();

  // TODO: draw the goal

  // draw the preferred velocity
  PrefVelocity pVel;
  _vc->setPrefVelocity(agt, goal, pVel);
  drawPrefVel(pVel, agt->_pos);

  glPopAttrib();
}
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis
