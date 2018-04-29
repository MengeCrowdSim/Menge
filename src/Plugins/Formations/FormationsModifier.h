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
 @file		FormationsModifier.h
 @brief		definition of a VelocityModifier to enforce formation behavior
 */
#ifndef __FORMATIONS_MODIFIER_H__
#define __FORMATIONS_MODIFIER_H__

#include "FormationsConfig.h"
#include "FreeFormation.h"

#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/BFSM/VelocityModifiers/VelModifierFactory.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#if defined(_MSC_VER)
// Visual Studio spews warnings about some members.
// It wants them to be exposed in the dll IN CASE a client uses it.
// TODO: remove this warning supression, correct the problem
#pragma warning(disable : 4251)
#endif

// forward declaration
class TiXmlElement;

namespace Formations {

// forward declaration
class FormationModifierFactory;

/*!
 @brief		A velocity modifier which adapts agent preferred velocities so that they move in
          formations.

 To specify a formation velocity modifier, use the following syntax:
 
 ```xml
 <VelModifier type="formation" file_name="string"/>
 ```

 The value `file_name` contains the path to the formation definition file (see 
 @ref Formations::FreeFormation for details on the file). The path should be relative to the file
 that specifies the velocity modifier.
 */
class FORMATIONS_API FormationModifier : public Menge::BFSM::VelModifier {
 public:
  /*!
   @brief	Constructor
   */
  FormationModifier();

  /*!
   @brief	Constructor for given Formation

   @param		form		A managed pointer to formation data
   */
  FormationModifier(FormationPtr form);

 protected:
  /*!
   @brief		Destructor.
   */
  virtual ~FormationModifier();

 public:
  /*!
   @brief		Creates a copy of this velocity modifier.
   */
  Menge::BFSM::VelModifier* copy() const;

  /*!
   @brief		Adapt the input preferred velocity according to the formation.

   @param		agent		The agent whose preferred velocity is provided.
   @param		pVel		The preferred velocity to modify -- modified in place.
   */
  void adaptPrefVelocity(const Menge::Agents::BaseAgent* agent, Menge::Agents::PrefVelocity& pVel);

  /*!
   @brief		Provides the task the formation modifier uses to update formations.

   @returns	A pointer to the supporting task. The caller is responsible to delete it.
   */
  virtual Menge::BFSM::Task* getTask();

  /*!
   @brief		Set the formation data.

   @param		form		A managed resource pointer to the underlying formation data.
   */
  void setFormation(FormationPtr form);

  /*!
   @brief		Register an agent to be affected by this velocity modifier.
   */
  void registerAgent(const Menge::Agents::BaseAgent* agent);

  /*!
   @brief		Unregister an agent from being affected by this velocity modifier.
   */
  void unregisterAgent(const Menge::Agents::BaseAgent* agent);

  friend class FormationModifierFactory;

 protected:
  /*!
   @brief		The underlying formation data.
   */
  FormationPtr _formation;

  /*!
   @brief		Concurrency lock for _formation.
   */
  Menge::ReadersWriterLock _lock;
};

///////////////////////////////////////////////////////////////////////////////

/*!
 @brief		The factory class for the FormationModifier
 */
class FORMATIONS_API FormationModifierFactory : public Menge::BFSM::VelModFactory {
 public:
  /*!
   @brief		Constructor.
   */
  FormationModifierFactory();

  /*!
   @brief		The name of the modifier

   The modifier's name must be unique among all registered modifier.
   Each modifier factory must override this function.

   @returns	A string containing the unique modifier name.
   */
  virtual const char* name() const { return "formation"; }

  /*!
   @brief		A description of the modifier

   Each modifier factory must override this function.

   @returns	A string containing the modifier description.
   */
  virtual const char* description() const {
    return "Forces agents in this state or machine to move in formation";
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
  Menge::BFSM::VelModifier* instance() const { return new FormationModifier(); }

  /*!
   @brief		Given a pointer to an modifier instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this modifier's type (i.e.
   modifierFactory::thisFactory() has already been called and returned true). If sub-classes of
   ModifierFactory introduce *new* modifier parameters, then the sub-class should override this
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
};
}  // namespace Formations
#endif  // __FORMATIONS_MODIFIER_H__
