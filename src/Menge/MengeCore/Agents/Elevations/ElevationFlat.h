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
 @file       ElevationFlat.h
 @brief      The definition of an elevation class for flat ground; elevation is always zero and
             gradient is always <0,0>.
 */

#ifndef __ELEVATION_FLAT_H__
#define __ELEVATION_FLAT_H__

// Menge Base
#include "MengeCore/Agents/Elevations/Elevation.h"
#include "MengeCore/Agents/Elevations/ElevationFactory.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {

/*!
 @brief    Defintion of elevation class representing flat ground.
 */
class MENGE_API FlatElevation : public Elevation {
 public:
  /*!  @brief    Reports the elevation of the simulation domain at the given point.

   The domain may have more than one valid elevation for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    point    A point on the x-z cartesian plane.
   @returns  The elevation at the given point.
   */
  float getElevation(const Vector2& point) const;

  /*!
   @brief    Reports the elevation of the simulation domain for the given agent

   @param    agent    A pointer to the agent for which elevation should be reported.
   @returns  The elevation (position on the y-axis) based on current agent state/position.
   */
  float getElevation(const BaseAgent* agent) const;

  /*!  @brief    Reports the gradient of the simulation domain at the given point.

   The domain may have more than one valid gradient for the point. It is the responsibility of the
   elevation entity resolve this.

   @param    point    A point on the x-z cartesian plane.
   @returns  The gradient at the given point.
   */
  virtual Vector2 getGradient(const Vector2& point) const;

  /*!
   @brief    Reports the gradient of the simulation domain for the given agent

   @param    agent    A pointer to the agent for which gradient should be reported.
   @returns  The gradient of the domain based on current agent state/position.
   */
  Vector2 getGradient(const BaseAgent* agent) const;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the FlatElevation.
 */
class MENGE_API FlatElevationFactory : public ElevationFactory {
 public:
  /*!
   @brief    The name of the elevation.

   The elevation's name must be unique among all registered elevation components. Each elevation
   factory must override this function.

   @returns  A string containing the unique elevation name.
   */
  virtual const char* name() const { return "flat"; }

  /*!
   @brief    A description of the elevation.

   Each elevation factory must override this function.

   @returns  A string containing the elevation description.
   */
  virtual const char* description() const {
    return "Provides the elevation data for a flat plane, located at zero elevation.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's elevation implementation.

   All ElevationFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding elevation type.  The various field values of the instance will be set in a
   subsequent call to ElevationFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Elevation class.
   */
  Elevation* instance() const { return new FlatElevation(); }
};

}  // namespace Agents
}  // namespace Menge
#endif  // __ELEVATION_FLAT_H__
