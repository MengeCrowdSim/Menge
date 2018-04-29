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
 @file    Elevation.h
 @brief    The definition of the elevation element. This approximately allows for agents to be
          simulated on non-planer domains.
 */
#ifndef __ELEVATION_H__
#define __ELEVATION_H__

#include "MengeCore/Math/Vector2.h"
#include "MengeCore/PluginEngine/Element.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {
// forward declaration
class BaseAgent;

/*!
 @brief    Exception class for elevation computation.
 */
class MENGE_API ElevationException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  ElevationException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ElevationException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal elevation exception.
 */
class MENGE_API ElevationFatalException : public ElevationException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  ElevationFatalException() : MengeException(), ElevationException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ElevationFatalException(const std::string& s)
      : MengeException(s), ElevationException(), MengeFatalException() {}
};

/*!
 @brief    The base class for determining an agent's elevation.

 This is an abstract class, primarily defining the Elevation abstraction. Essentially, the
 Elevation object must be able to report elevation for an agent (or an arbitrary position) and the
 "gradient" at that point.

 The gradient is not *truly* the gradient.  In fact, it is merely the projection of the elevation
 object's normal on the planning plane.  So, if the elevation is parallel to the planning plane,
 the gradient would be the vector \<0, 0\>.  If the elevation surface is perpendicular to the
 planning plane (a highly unlikely contingent) it would be the vector \<x, y\> such that the
 magnitude of that vector is 1.
 */
class MENGE_API Elevation : public Element {
 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~Elevation() {}

 public:
  /*!
   @brief    Reports the elevation of the simulation domain at the given point.

   The domain may have more than one valid elevation for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    point    A point on the x-z cartesian plane.
   @returns  The elevation at the given point.
   */
  virtual float getElevation(const Math::Vector2& point) const = 0;

  /*!
   @brief    Reports the elevation of the simulation domain for the given agent

   The domain may have more than one valid elevation for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    agent    A pointer to the agent for which elevation should be reported.
   @returns  The elevation (position on the y-axis) based on current agent state/position.
   */
  virtual float getElevation(const BaseAgent* agent) const = 0;

  /*!  @brief    Reports the gradient of the simulation domain at the given point.

   The domain may have more than one valid gradient for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    point    A point on the x-z cartesian plane.
   @returns  The gradient at the given point.
   */
  virtual Math::Vector2 getGradient(const Math::Vector2& point) const = 0;

  /*!
   @brief    Reports the gradient of the simulation domain for the given agent

   The domain may have more than one valid gradient for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    agent    A pointer to the agent for which gradient should be reported.
   @returns  The gradient of the domain based on current agent state/position.
   */
  virtual Math::Vector2 getGradient(const BaseAgent* agent) const = 0;
};

}  // namespace Agents
}  // namespace Menge

#endif  // __ELEVATION_H__
