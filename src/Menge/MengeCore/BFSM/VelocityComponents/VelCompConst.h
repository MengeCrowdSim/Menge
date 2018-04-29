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
 @file    VelCompConst.h
 @brief    Provides the definitions of some simple velocity components based on constant velocities.
 */

#ifndef __VEL_COMP_CONST_H__
#define __VEL_COMP_CONST_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {

/////////////////////////////////////////////////////////////////////

/*!
 @brief    A velocity component that always returns a constant velocity.
 */
class MENGE_API ConstVelComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  ConstVelComponent();

  /*!
   @brief    Constructor.

   @param    vel      The preferred velocity to return.
   */
  ConstVelComponent(const Vector2& vel);

  /*!
   @brief    Sets the constant velocity.

   @param    velocity    The velocity for this velocity component.
   */
  void setVelocity(const Vector2& velocity);

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
   @brief    Returns the constant velocity.

   @returns  The velocity computed by this constant velocity component.
   */
  Vector2 getConstVelocity() const { return _dir * _speed; }

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;

  friend class ConstVCFactory;

 protected:
  /*!
   @brief    The direction of the constant preferred velocity.
   */
  Vector2 _dir;

  /*!
   @brief    The speed of the constant preferred velocity.
   */
  float _speed;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ConstVelComponent.
 */
class MENGE_API ConstVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    Constructor.
   */
  ConstVCFactory();

  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered velocity components. Each
   velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return ConstVelComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which is always a constant value "
           "(direction and magnitude).";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity component.

   All VelCompFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding velocity component type. The various field values of the instance will be set in a
   subsequent call to VelCompFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated VelComponent class.
   */
  VelComponent* instance() const { return new ConstVelComponent(); }

  /*!
   @brief    Given a pointer to an VelComponent instance, sets the appropriate fields from the
   provided XML node.

   It is assumed that the value of the `type` attribute is this VelComponent's type (i.e.
   VelCompFactory::thisFactory has already been called and returned true). If sub-classes of
   VelCompFactory introduce *new* VelComponent parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    vc          A pointer to the velocity component whose attributes are to be set.
   @param    node        The XML node containing the velocity component attributes.
   @param    behaveFldr  The path to the behavior file.  If the velocity component references
                        resources in the file system, it should be defined relative to the behavior
                        file location. This is the folder containing that path.
   @returns  True if successful.
   */
  virtual bool setFromXML(VelComponent* vc, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "x" float attribute.
   */
  size_t _xID;

  /*!
   @brief    The identifier for the "y" float attribute.
   */
  size_t _yID;
};

/*!
 @brief    A velocity component that always returns a constant direction but leaves the preferred
          speed unchanged.
 */
class MENGE_API ConstVelDirComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  ConstVelDirComponent();

  /*!
   @brief    Constructor.

   @param    vel      The preferred velocity direction to return (vel is *not* assumed to be
                    unit-length).
   */
  ConstVelDirComponent(const Vector2& vel);

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
   @brief    Sets the direction of the velocity component.

   @param    dir      The direction the velocity should provide. The input need not be normalized, as
                    that will be done by this function.
   */
  void setDirection(const Vector2& dir);

  /*!
   @brief    Reports the direction of this velocity component.

   @returns  The direction.
   */
  const Vector2& getDirection() const { return _dir; }

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;

  friend class ConstDirVCContext;

 protected:
  /*!
   @brief    The direction of the constant preferred velocity.
   */
  Vector2 _dir;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ConstVelDirComponent.
 */
class MENGE_API ConstDirVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    Constructor.
   */
  ConstDirVCFactory();

  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered velocity components. Each
   velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return ConstVelDirComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which is always in a fixed direction but "
           "uses the agent's preferred speed.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity component.

   All VelCompFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding velocity component type. The various field values of the instance will be set in a
   subsequent call to VelCompFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated VelComponent class.
   */
  VelComponent* instance() const { return new ConstVelDirComponent(); }

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
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(VelComponent* vc, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "x" float attribute.
   */
  size_t _xID;

  /*!
   @brief    The identifier for the "y" float attribute.
   */
  size_t _yID;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    A velocity component that always returns the zero velocity.
 */
class MENGE_API ZeroVelComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  ZeroVelComponent();

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
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the ZeroVelComponent.
 */
class MENGE_API ZeroVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered velocity components. Each
   velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return ZeroVelComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which is always the zero velocity.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity component.

   All VelCompFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding velocity component type. The various field values of the instance will be set in a
   subsequent call to VelCompFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated VelComponent class.
   */
  VelComponent* instance() const { return new ZeroVelComponent(); }
};

//////////////////////////////////////////////////////////////////////////////

}  // namespace BFSM
}  // namespace Menge

#endif  // __VEL_COMP_CONST_H__
