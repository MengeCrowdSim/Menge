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
 @file    VelModifierScale.h
 @brief    Provides the definition of a simple velocity modifier that simply scale's the preferred
          velocity's speed.
 */

#ifndef __VEL_MOD_SCALE_H__
#define __VEL_MOD_SCALE_H__

#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/CoreConfig.h"
#if 0
#include "MengeCore/BFSM/VelModifiers/VelModifierContext.h"
#endif
#include "MengeCore/BFSM/VelocityModifiers/VelModifierFactory.h"

namespace Menge {

namespace BFSM {

/*!
 @brief    A velocity modifier that scales the input velocity's speed.

 To specify a scale velocity modifier, use the following syntax:

 ```xml
 <VelModifier type="scale" scale="float"/>
 ```

 The value `scale` contains the scalar factor the input preferred velocity's speed will be multiplied by.
 */
class MENGE_API ScaleVelModifier : public VelModifier {
 public:
  /*!
   @brief    Default constructor.
   */
  ScaleVelModifier();

  /*!
   @brief    Constructor.

   @param    scale      The scale factor to apply to the preferred velocity's speed.
   */
  ScaleVelModifier(const float scale);

  /*!
   @brief    Sets the scale factor.

   @param    scale      The the scale factor to apply to the preferred velocity's speed.
   */
  void setScale(const float scale);

  /*!
   @brief    Scales the input preferred velocity's speed by the pre-defined scale factor.

   @param    agent    The agent for which a preferred velocity is modified.
   @param    pVel    The instance of Agents::PrefVelocity to set.
   */
  void adaptPrefVelocity(const Agents::BaseAgent* agent, Agents::PrefVelocity& pVel);

  /*!
   @brief    Returns scale factor.

   @returns  The scale factor stored in this modifier.
   */
  float getScale() const { return _scale; }

  /*!
   @brief    Creates a copy of this velocity modifier.

   @returns  A unique, deep copy of this velocity modifier.
   */
  VelModifier* copy() const;
#if 0
  /*!
   @brief    Provides a display context for interacting with this velocity modifier.

   @returns  A pointer to a context for this vel modifier.
   */
  virtual VelModContext * getContext();    //TODO: Implement the context
#endif
  friend class ScaleVMFactory;

 protected:
  /*!
   @brief    The scale factor to apply.
   */
  float _scale;
};
#if 0
//////////////////////////////////////////////////////////////////////////////

/*!
  @brief    The context for the ScaleVelModifier.
  */
//TODO: provide a context here
class MENGE_API ScaleVMContext : public VelModContext {
public:
  /*!
   @brief    Constructor.

   @param    vm      A pointer to the underlying fsm velocity modifier. The context will *not* delete
                    the velocity modifier.
    */
  ScaleVMContext( ScaleVelModifier * vm );

  /*!
   @brief    Provides a string to be printed in the display as a UI element detailing velocity
            modifier information.

   @param    indent    An optional string representing indentation to be applied to the text. It is
                      prefixed at the start of each line.
   @returns  The string for printing on the UI layer.
   */
  virtual std::string getUIText( const std::string & indent="" ) const;

  /*!
   @brief    Draw context elements into the 3D world.

   This should never be called in select mode.

   @param    agt      The particular agent for which the FSM is being visualized.
   */
  virtual void draw3DGL( const Agents::BaseAgent * agt);

protected:
  /*!
   @brief    The underlying finite state machine velocity modifier.
   */
  ScaleVelModifier * _vm;
};
#endif
/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ScaleVelModifier.
 */
class MENGE_API ScaleVMFactory : public VelModFactory {
 public:
  /*!
   @brief    Constructor.
   */
  ScaleVMFactory();

  /*!
   @brief    The name of the velocity modifier.

   The velocity modifiers's name must be unique among all registered velocity modifiers. Each
   velocity modifier factory must override this function.

   @returns  A string containing the unique velocity modifier name.
   */
  virtual const char* name() const { return "scale"; }

  /*!
   @brief    A description of the velocity modifier.

   Each velocity modifier factory must override this function.

   @returns  A string containing the velocity modifier description.
   */
  virtual const char* description() const {
    return "Scales the input preferred velocity by the given scale factor.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity modifier.

   All VelModFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding velocity modifier type. The various field values of the instance will be set in a
   subsequent call to VelModFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated VelModifier class.
   */
  VelModifier* instance() const { return new ScaleVelModifier(); }

  /*!
   @brief    Given a pointer to an VelModifier instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this VelModifier's type (i.e.
   VelModFactory::thisFactory() has already been called and returned true). If sub-classes of
   VelModFactory introduce *new* VelComponent parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    vm          A pointer to the velocity modifier whose attributes are to be set.
   @param    node        The XML node containing the velocity modifier attributes.
   @param    behaveFldr  The path to the behavior file.  If the velocity modifier references
                        resources in the file system, it should be defined relative to the behavior
                        file location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(VelModifier* vm, TiXmlElement* node, const std::string& behaveFldr) const;

  // TODO: This should support a numerical distribution
  /*!
   @brief    The identifier for the "scale" float attribute.
   */
  size_t _scaleID;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __VEL_MOD_SCALE_H__
