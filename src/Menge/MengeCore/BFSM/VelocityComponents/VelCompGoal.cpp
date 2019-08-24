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

#include "MengeCore/BFSM/VelocityComponents/VelCompGoal.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Core.h"

#include <iomanip>
#include <sstream>

namespace Menge {

namespace BFSM {

/////////////////////////////////////////////////////////////////////
//                   Implementation of GoalVelComponent
/////////////////////////////////////////////////////////////////////

const std::string GoalVelComponent::NAME = "goal";

/////////////////////////////////////////////////////////////////////

GoalVelComponent::GoalVelComponent() : VelComponent() {}

/////////////////////////////////////////////////////////////////////

void GoalVelComponent::setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                                       Agents::PrefVelocity& pVel) const {
  // TODO(curds01): Now that goals can *move*; it might be better to pick a velocity based on an
  // estimated *future* position of the goal rather than it's instantaneous position. At the very
  // least it should be a flag that I can set. Then I'll do a better job of following mobile
  // goals.

  // directions
  goal->setDirections(agent->_pos, agent->_radius, pVel);

  // speed
  Vector2 goalPoint = pVel.getTarget();
  Vector2 disp = goalPoint - agent->_pos;
  const float distSq = absSq(disp);
  float speed = agent->_prefSpeed;

  if (distSq <= 0.0001f) {
    // I've basically arrived -- speed should be zero.
    speed = 0.f;
  } else {
    const float speedSq = speed * speed;
    const float TS_SQD = SIM_TIME_STEP * SIM_TIME_STEP;
    if (distSq / speedSq < TS_SQD) {
      // The distance is less than I would travel in a single time step.
      speed = sqrtf(distSq) / SIM_TIME_STEP;
    }
  }
  pVel.setSpeed(speed);
}

/////////////////////////////////////////////////////////////////////
}  // namespace BFSM
}  // namespace Menge
