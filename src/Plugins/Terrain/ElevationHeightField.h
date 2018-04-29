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
 @file       ElevationHeightField.h
 @brief      Defines elevation based on a height field.
 
 If an agent (or a point) cannot be located on the field, its elevation is ????
 */

#ifndef __ELEVATION_HEIGHT_FIELD_H__
#define __ELEVATION_HEIGHT_FIELD_H__

// Resources
#include "HeightField.h"

// Menge Base
#include "MengeCore/Agents/Elevations/Elevation.h"
#include "MengeCore/Agents/Elevations/ElevationFactory.h"

// forward declaration
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

/*!
 @namespace Terrain
 @brief	  The namespace containing the height field resource and Menge elements which use the height
          field.
 */
namespace Terrain {

/*!
 @brief		Elevation definition based on a height field.
 */
class EXPORT_API HeightFieldElevation : public Menge::Agents::Elevation {
 public:
  /*!
   @brief		Constructor
   */
  HeightFieldElevation();

  /*!	@brief		Reports the elevation of the simulation domain at the given point.

   The domain may have more than one valid elevation for the point.
   It is the responsibility of the elevation entity resolve this.

   @param		point		A point on the x-z cartesian plane.
   @returns	The elevation at the given point.
   */
  virtual float getElevation(const Menge::Math::Vector2& point) const;

  /*!
   @brief		Reports the elevation of the simulation domain for the given agent

   @param		agent		A pointer to the agent for which elevation should be reported.
   @returns	The elevation (position on the y-axis) based on current agent state/position.
   */
  virtual float getElevation(const Menge::Agents::BaseAgent* agent) const;

  /*!	@brief		Reports the gradient of the simulation domain at the given point.

   The domain may have more than one valid gradient for the point.
   It is the responsibility of the elevation entity resolve this.

   @param		point		A point on the x-z cartesian plane.
   @returns	The gradient at the given point.
   */
  virtual Menge::Math::Vector2 getGradient(const Menge::Math::Vector2& point) const;

  /*!
   @brief		Reports the gradient of the simulation domain for the given agent

   @param		agent		A pointer to the agent for which gradient should be reported.
   @returns	The gradient of the domain based on current agent state/position.
   */
  virtual Menge::Math::Vector2 getGradient(const Menge::Agents::BaseAgent* agent) const;

  /*!
   @brief		Sets the height field for this elevation object to use.

   @param		hfPtr		A managed pointer to the height field.
   */
  void setHeightField(HeightFieldPtr hfPtr) { _field = hfPtr; }

 protected:
  /*!
   @brief		The height field used to query elevation and gradient
   */
  HeightFieldPtr _field;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief		Factory for the HeightFieldElevation.
 */
class EXPORT_API HeightFieldElevationFactory : public Menge::Agents::ElevationFactory {
 public:
  /*!
   @brief		Constructor.
   */
  HeightFieldElevationFactory();

  /*!
   @brief		The name of the elevation.

   The elevation's name must be unique among all registered elevation components.
   Each elevation factory must override this function.

   @returns	A string containing the unique elevation name.
   */
  virtual const char* name() const { return "height_field"; }

  /*!
   @brief		A description of the elevation.

   Each elevation factory must override this function.

   @returns	A string containing the elevation description.
   */
  virtual const char* description() const {
    return "Provides elevation based on location on a height field.";
  };

 protected:
  /*!
   @brief		Create an instance of this class's elevation implementation.

   All ElevationFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding elevation type. The various field values of the instance will be set in a
   subsequent call to ElevationFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns		A pointer to a newly instantiated Elevation class.
   */
  Menge::Agents::Elevation* instance() const { return new HeightFieldElevation(); }

  /*!
   @brief		Given a pointer to an Elevation instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Elevation's type (i.e.
   ElevationFactory::thisFactory has already been called and returned true). If sub-classes of
   ElevationFactory introduce *new* Elevation parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param		e			      A pointer to the elevation whose attributes are to be set.
   @param		node		    The XML node containing the elevation attributes.
   @param		specFldr	  The path to the specification file. If the Elevation references resources in
                        the file system, it should be defined relative to the specification file
                        location. This is the folder containing that path.
   @returns	True if successful.
   */
  virtual bool setFromXML(Menge::Agents::Elevation* e, TiXmlElement* node,
                          const std::string& specFldr) const;

  /*!
   @brief		The identifier for the "file_name" string attribute.
   */
  size_t _fileNameID;
};
}  // namespace Terrain
#endif  // __ELEVATION_HEIGHT_FIELD_H__
