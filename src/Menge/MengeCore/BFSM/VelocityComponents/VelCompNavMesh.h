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
 @file    VelCompNavMesh.h
 @brief    Provides the definition of the navigation mesh velocity component.

 The preferred velocity is defined for each agent based on a polygonal mesh representation of the
 free space (a navigation mesh) and searches on that graph.
 */

#ifndef __VEL_COMP_NAV_MESH_H__
#define __VEL_COMP_NAV_MESH_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/resources/NavMesh.h"
#include "MengeCore/resources/NavMeshLocalizer.h"

// forward declaration

namespace Menge {

namespace BFSM {
// forward declaration
class NavMeshVCContext;

/*!
 @brief    A velocity component that returns a preferred velocity whose direction and preferred speed
          are computed from a navigation mesh.

 A navigation mesh is a representation of the traversalbe space. The traversable space is
 represented as a polygonal mesh.  Graph searches through the mesh are performed to find paths
 through arbitrarily complex environments.
 */
class MENGE_API NavMeshVelComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  NavMeshVelComponent();

  /*!
   @brief    Called when the agent exits the state that uses this velocity component.

   @param    agent    The agent exiting the state.
   */
  virtual void onExit(Agents::BaseAgent* agent);

  /*!
   @brief    Sets the navigation mesh pointer.

   @param    nm    The managed pointer to the navigation mesh.
   */
  void setNavMesh(const NavMeshPtr& nm) { _navMesh = nm; }

  /*!
   @brief    Returns a resource pointer to the underlying navigation mesh.

   @returns  The navigation mesh.
   */
  const NavMeshPtr getNavMesh() const { return _navMesh; }

  /*!
   @brief    Returns a resource pointer to the underlying navigation mesh localizer

   @returns  The navigation mesh localizer.
   */
  const NavMeshLocalizerPtr getLocalizer() const { return _localizer; }

  /*!
   @brief    Sets the navigation mesh localizer pointer.

   @param    nml    The managed pointer to the navigation mesh localizer.
   */
  void setNavMeshLocalizer(const NavMeshLocalizerPtr& nml) { _localizer = nml; }

  /*!
   @brief    Sets the angular deviation which triggers replanning.

   @param    angle    The angle of deviation in radians.
   */
  void setHeadingDeviation(float angle);

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

  /*!
   @brief    Returns a pointer to the nav mesh localizer task.

   @returns    A pointer to the nav mesh localizer task.  It is the responsibility of the caller to
              free the memory of the provided task by calling its destroy method.
   */
  virtual Task* getTask();

 protected:
  void doUpdateGoal(const Agents::BaseAgent* agent, const Goal* goal) override;

  /*!
   @brief    The cosine of the heading deviation angular threshold.
   
   This detects when the angle of approach deviates beyond a threshold and the agent needs to
   replan.
   */
  float _headingDevCos;

  /*!
   @brief    The navigation mesh.
   */
  NavMeshPtr _navMesh;

  /*!
   @brief    The localizer for the navigation mesh.
   */
  NavMeshLocalizerPtr _localizer;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the NavMeshVelComponent.
 */
class MENGE_API NavMeshVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    Constructor.
   */
  NavMeshVCFactory();

  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered velocity components. Each
   velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return NavMeshVelComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which is derived from a path along a "
           "polygonally decomposed representation of the traversable space (a "
           "navigaiton mesh).";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity component.

   All VelCompFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding velocity component type. The various field values of the instance will be set
   in a subsequent call to VelCompFactory::setFromXML(). The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated VelComponent class.
   */
  VelComponent* instance() const { return new NavMeshVelComponent(); }

  /*!
   @brief    Given a pointer to an VelComponent instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this VelComponent's type (i.e.
   VelCompFactory::thisFactory has already been called and returned true). If sub-classes of
   VelCompFactory introduce *new* VelComponent parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    vc          A pointer to the velocity component whose attributes are to be set.
   @param    node        The XML node containing the velocity component attributes.
   @param    behaveFldr  The path to the behavior file. If the velocity component references
                        resources in the file system, it should be defined relative to the behavior
                        file location. This is the folder containing that path.
   @returns  Reports true if successful.
   */
  virtual bool setFromXML(VelComponent* vc, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "file_name" string attribute.
   */
  size_t _fileNameID;

  /*!
   @brief    The identifier for the "heading_threshold" float attribute.
   */
  size_t _headingID;
};
}  // namespace BFSM
}  // namespace Menge
#endif  // __VEL_COMP_NAV_MESH_H__
