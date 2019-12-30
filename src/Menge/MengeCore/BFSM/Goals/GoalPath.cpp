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

#include <memory>

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/Goals/GoalPath.h"

namespace Menge {
namespace BFSM {

using Math::Vector2;
using std::vector;

/////////////////////////////////////////////////////////////////////
//                   Implementation of PathGoal
/////////////////////////////////////////////////////////////////////

const std::string PathGoal::NAME = "path";

/////////////////////////////////////////////////////////////////////

void PathGoal::move(float time_step) {
  const float step_len = time_step * _speed;
  const float dist_sq = _p_WGo.distanceSq(_way_points[_current_waypoint]);
  if (dist_sq >= step_len * step_len) {
    // The distance I would travel is less than the distance to the waypoint. Simply advance.
    _p_WGo += _v_W * time_step;
  } else {
    // This step would take me *beyond* the way point. So, I need to advance to the point and then
    // turn.
    const float dist = std::sqrt(dist_sq);
    _p_WGo = _way_points[_current_waypoint];
    _current_waypoint += _path_direction;
    if (_current_waypoint >= static_cast<int>(_way_points.size()) || _current_waypoint < 0) {
      if (_closed) {
        _current_waypoint = 0;
      } else {
        // Reverse direction.
        _path_direction = -_path_direction;
        // _current_waypoint has been incremented once in the *old* path direction (see above). So,
        // we increment it *twice* in the new path direction; once to undo the previous, mistaken
        // increment and once to advanc in the new direction.
        _current_waypoint += 2 * _path_direction;
      }
    }
    set_velocity();
    // Because of constant speed, the amount of time left is proportionate to the amount of distance
    // remaining.
    const float remaining_time = time_step * (1.0f - dist / step_len);
    _p_WGo += _v_W * remaining_time;
  }
}

/////////////////////////////////////////////////////////////////////

void PathGoal::configure(float speed, bool closed, Geometry2D* geometry, vector<Vector2>&& path) {
  _speed = speed;
  _closed = closed;
  setGeometry(geometry);
  _way_points = std::move(path);
  _current_waypoint = 1;
  _p_WGo = _way_points[0];
  _path_direction = 1;
  set_velocity();
}

/////////////////////////////////////////////////////////////////////

void PathGoal::set_velocity() {
  Vector2 p_GoP_W = _way_points[_current_waypoint] - _p_WGo;
  const float len = p_GoP_W.Length();
  assert(len >= 1e-5 &&
         "PathGoal::set_velocity has been called with the goal position coincident "
         "with the target waypoint");
  _v_W = p_GoP_W * (_speed / len);
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of PathGoalFactory
/////////////////////////////////////////////////////////////////////

PathGoalFactory::PathGoalFactory() {
  _speedId = _attrSet.addFloatAttribute("speed", true /* required */);
  _closedId = _attrSet.addBoolAttribute("closed", true /* required */);
  _shapeId = _attrSet.addStringAttribute("shape", true /* required */);
}

/////////////////////////////////////////////////////////////////////

bool PathGoalFactory::setFromXML(Goal* goal, TiXmlElement* node,
                                 const std::string& behaveFldr) const {
  PathGoal* pGoal = dynamic_cast<PathGoal*>(goal);
  assert(pGoal != 0x0 && "Trying to set path goal attributes on an incompatible object.");

  if (!GoalFactory::setFromXML(pGoal, node, behaveFldr)) return false;

  const bool closed = _attrSet.getBool(_closedId);
  pGoal->_closed = closed;

  const float speed = _attrSet.getFloat(_speedId);
  if (speed < 0) {
    logger << Logger::ERR_MSG << "The 'speed' attribute of the 'path' goal on line " << node->Row()
           << " has been "
           << "given a negative value: " << speed << ".\n";
    return false;
  }
  pGoal->_speed = speed;

  // Now parse the path.
  vector<Vector2> path;
  TiXmlElement* child{};
  for (child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
    if (child->ValueStr() == "Point") {
      double x, y;
      bool success = child->Attribute("x", &x);
      success = child->Attribute("y", &y) && success;
      if (!success) {
        logger << Logger::ERR_MSG << "The 'path' goal on line " << node->Row()
               << " has malformed <Point> data on "
               << "line " << child->Row() << "; it should have two attributes: x=\"float\" and "
               << "y=\"float\".\n";
        return false;
      }
      path.emplace_back(static_cast<float>(x), static_cast<float>(y));
    } else {
      logger << Logger::WARN_MSG << "The 'path' goal on line " << node->Row()
             << " has an unexpected child tag: '" << child->ValueStr() << "'. That tag is being "
             << "ignored.\n";
    }
  }
  if (path.size() < 2) {
    logger << Logger::ERR_MSG << "The 'path' goal on line " << node->Row() << " only has "
           << path.size() << " points specified. There must be at least two.\n";
    return false;
  }

  const std::string shape_value = _attrSet.getString(_shapeId);
  Math::Geometry2D* geometry = createGeometry(node);
  // NOTE: Error messages have already been logged by createGeometry() in the event of not returning
  // a valid pointer.
  if (geometry == nullptr) return false;

  pGoal->configure(speed, closed, geometry, std::move(path));
  return true;
}
}  // namespace BFSM
}  // namespace Menge
