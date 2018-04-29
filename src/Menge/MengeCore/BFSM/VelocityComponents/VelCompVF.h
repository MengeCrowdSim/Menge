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
 @file    VelCompVF.h
 @brief    Provides the definition of the vector field velocity component.

 The preferred velocity is defined for each agent based on where the agent is in relation a
 uniformly discretized 2D grid.
 */

#ifndef __VEL_COMP_VF_H__
#define __VEL_COMP_VF_H__

#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/resources/VectorField.h"

namespace Menge {

// forward declaration
template <class R>
class ResourcePtr;

/*!
 @brief forward declaration of VectorField resource pointer. See VectorField.h for more details.
 */
typedef ResourcePtr<VectorField> VectorFieldPtr;

namespace BFSM {
/*!
 @brief    A velocity component that returns a preferred velocity whose direction and preferred speed
          are computed from a velocity field.

 If the velocity vector is of unit length, the preferred speed will be unchanged. Otherwise, the
 preferred speed is scaled by the length of the velocity vector.
 */
class MENGE_API VFVelComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  VFVelComponent();

  /*!
   @brief    Constructor.

   @param    vf          The VectorField to use for velocity computation.
   @param    useNearest  Determines if the velocity is computed by selecting the neareset vector
                        (true) or the bilinear interpolation of four near-by cells (false).
   */
  VFVelComponent(VectorFieldPtr& vf, bool useNearest);

  /*!
   @brief    Determines whether the vector field velocity component computes velocity from the
            nearest cell center or from a bilinear interpolation on the four near-by cells.

   @param    useNearest    Uses only nearest cell if true, 4-cell neighborhood if false.
   */
  void setUseNearest(bool useNearest) { _nearest = useNearest; }

  /*!
   @brief    Sets the velocity field pointer.

   @param    vf    The managed pointer to the velocity field.
   */
  void setVectorField(const VectorFieldPtr& vf) { _vf = vf; }

  /*!
   @brief    Provides access to the underlying vector field.

   @returns  A pointer to the vector field.
   */
  const VectorFieldPtr getVectorField() const { return _vf; }

  /*!
   @brief    Computes and sets the agent's preferred velocity.

   The velocity component directly sets preferred velocity values in the the provided preferred
   velocity instance. See Agents::PrefVelocity for details. Rather than setting the agent's
   preferred velocity value directly, a reference to a preferred velocity instance is passed in to
   make the use more general. This allows the computation of the preferred velocity for the agent,
   without necessarily making changes to it.

   @param    agent    The agent for which a preferred velocity is computed.
   @param    goal    The agent's goal (although this may be ignored).
   @param    pVel    The instance of Agents::PrefVelocity to set.
   */
  virtual void setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                               Agents::PrefVelocity& pVel) const;

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;

 protected:
  /*!
   @brief    The vector field resource which provides preferred velocity vectors.
   */
  VectorFieldPtr _vf;

  /*!
   @brief    Determines if the vector selection is based on nearest (true) or bilinear interpolation
            (false).
   */
  bool _nearest;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the VFVelComponent.
 */
class MENGE_API VFVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    Constructor.
   */
  VFVCFactory();

  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered
   velocity components.  Each velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return VFVelComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which is derived from a velocity field "
           "defined on a uniform, 2D discretization of the planning space.";
  };

 protected:
  /*!
  @brief    Create an instance of this class's velocity component.

  All VelCompFactory sub-classes must override this by creating (on the heap)
  a new instance of its corresponding velocity component type.  The various field
  valuesof the instance will be set in a subsequent call to
  VelCompFactory::setFromXML. The caller of this function takes ownership of the
  memory.

  @returns    A pointer to a newly instantiated VelComponent class.
  */
  VelComponent* instance() const { return new VFVelComponent(); }

  /*!
   @brief    Given a pointer to an VelComponent instance, sets the appropriate
   fields from the provided XML node.

   It is assumed that the value of the `type` attribute is this VelComponent's type.
   (i.e. VelCompFactory::thisFactory has already been called and returned true.)
   If sub-classes of VelCompFactory introduce *new* VelComponent parameters, then the
   sub-class should override this method but explicitly call the parent class's
   version.

   @param    vc          A pointer to the velocity component whose attributes are to be set.
   @param    node        The XML node containing the velocity component attributes.
   @param    behaveFldr  The path to the behavior file.  If the velocity component references
                         resources in the file system, it should be defined relative to the behavior
                         file location.  This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(VelComponent* vc, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "file_name" string attribute.
   */
  size_t _fileNameID;

  /*!
   @brief    The identifier for the "use_nearest" bool attribute.
   */
  size_t _useNearestID;
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __VEL_COMP_VF_H__
