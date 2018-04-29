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
 @file		VelModHeightField.h
 @brief		definition of a VelocityModifier to push agents along the gradient of a heightField.
 */
#ifndef __VELMOD_HEIGHT_FIELD_H__
#define __VELMOD_HEIGHT_FIELD_H__

// Resources
#include "HeightField.h"

// Menge Base
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifierFactory.h"

// forward declarations
class TiXmlElement;

namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace Terrain {

// forward declare Factory
class HeightFieldModifierFactory;

/*!
 @brief		Modify the preferred velocity based on a height field.

 Agents preferred velocities can be sped up, slowed, and turned based on their attempt to traverse a
 height field. The gradient of the height field is used to determine the new preferred velocity.

 Direction is determined by a re-normalized, weighted sum of the gradient with the input preferred
 direction.

 The resultant speed is a scaled factor of the input speed with the steepness of the gradient.
 Uphill and downhill speed changes are controlled separately -- gradients which lie perpindicular
 to the input preferred direction will cause a direction change, but no speed change.

 The output preferred velocity becomes a single velocity (instead of an arc). If the input preferred
 velocity spanned an arc, it will be collapsed to a single direction.

 To specify a formation velocity modifier, use the following syntax:

 ```xml
 <VelModifier type="height_field"
              file_name="string"
              down_hill_scale="float"
              dir_weight="float"
              up_hill_scale="float"
 />
 ```

 The parameters have the following meanings:
    - The value `file_name` contains the path to the height field definition file (see 
      @ref Terrain::HeightField for details on the file). The path should be relative to the file
      that specifies the velocity modifier.
    - The value `down_hill_scale` is a scale factor applied to down hill motion. It limits the amount
      the speed increases when going downhill (e.g., a value of zero prevents the speed from
      changing at all. A value of 1.0 will allow the speed to double).
    - The value `dir_weight` is a scale factor which determines how much the travel direction is
      affected by the height field. Smaller values will cause smaller changes to direction than
      larger values. (A value of zero will leave the agent moving along their original direction,
      but still susceptible to speed changes.)
    - The value `up_hill_scale` is a scale factor applied to up hill motion. It limits the amount
      that the speed is retarded when moving up hill. If the value is 1, then only a vertical
      surface will *completely* eliminate motion. However, larger values wall cause the agent to
      slow to a stop on smaller slopes. Smaller values will cause the agent to slow less while
      moving up hill.
 */
class EXPORT_API HeightFieldModifier : public Menge::BFSM::VelModifier {
 public:
  /*!
   @brief		Default constructor
   */
  HeightFieldModifier();

  /*!
   @brief		Constructor

   @param		hfPtr		A pointer to a height-field resource.
   */
  HeightFieldModifier(HeightFieldPtr hfPtr);

  /*!
   @brief		Sets the height field for this velocity modifier object to use.

   @param		hfPtr		A managed pointer to the height field.
   */
  void setHeightField(HeightFieldPtr hfPtr) { _field = hfPtr; }

  /*!
   @brief		Copy method for this velocity modifier
   */
  Menge::BFSM::VelModifier* copy() const;

  /*!
   @brief		adapt preferred velocity by pushing it away from the heightfield gradient

   @param		agent		The agent for which to modify preferred vel
   @param		pVel		The agent's current preferred velocity (having potentially been modified).
   */
  void adaptPrefVelocity(const Menge::Agents::BaseAgent* agent, Menge::Agents::PrefVelocity& pVel);

  /*
   Identify the factory helper.
   */
  friend class HeightFieldModifierFactory;

 protected:
  /*!
   @brief		The height field used to query elevation and gradient
   */
  HeightFieldPtr _field;

  /*!
   @brief		The weighting factor for how the gradient effects direction.
   */
  float _turnWeight;

  /*!
   @brief		The uphill scale factor.
   */
  float _upHillScale;

  /*!
   @brief		The downhill scale factor.
   */
  float _downHillScale;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief		Factory for the HeightFieldModifier.
 */
class EXPORT_API HeightFieldModifierFactory : public Menge::BFSM::VelModFactory {
 public:
  /*!
   @brief		Constructor.
   */
  HeightFieldModifierFactory();

  /*!
   @brief		The name of the modifier

   The modifier's name must be unique among all registered modifier.
   Each modifier factory must override this function.

   @returns	A string containing the unique modifier name.
   */
  virtual const char* name() const { return "height_field"; }

  /*!
   @brief		A description of the modifier

   Each modifier factory must override this function.

   @returns	A string containing the modifier description.
   */
  virtual const char* description() const {
    return "Pushes the agent's preferred velocity away from the gradient of a height "
           "field";
  };

 protected:
  /*!
   @brief		Create an instance of this class's modifier.

   All modifierFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding modifier type. The various field values of the instance will be set in a
   subsequent call to modifierFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns		A pointer to a newly instantiated modifier class.
   */
  Menge::BFSM::VelModifier* instance() const { return new HeightFieldModifier(); }

  /*!
   @brief		Given a pointer to an modifier instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this modifier's type (i.e.
   modifierFactory::thisFactory() has already been called and returned true). If sub-classes of
   modifierFactory introduce *new* modifier parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param		modifier	  A pointer to the modifier whose attributes are to be set.
   @param		node		    The XML node containing the modifier attributes.
   @param		behaveFldr	The path to the behavior file. If the modifier references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns	A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Menge::BFSM::VelModifier* modifier, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief		The identifier for the "file_name" string attribute.
   */
  size_t _fileNameID;

  /*!
   @brief		The identifier for the "dir_weight" float attribute.
   */
  size_t _turnID;

  /*!
   @brief		The identifier for the "up_hill_scale" float attribute.
   */
  size_t _uphillID;

  /*!
   @brief		The identifier for the "down_hill_scale" float attribute.
   */
  size_t _downhillID;
};
}  // namespace Terrain
#endif  // __VELMOD_HEIGHT_FIELD_H__
