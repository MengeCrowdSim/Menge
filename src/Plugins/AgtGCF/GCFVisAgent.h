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
 @file		GCFVisAgent.h
 @brief		The custom visualization for elliptical agents.
 */
#ifndef __GCF_VIS_AGENT_H__
#define __GCF_VIS_AGENT_H__

#include "GCFAgent.h"
#include "MengeVis/Runtime/VisAgent/VisAgent.h"

namespace GCF {

/*!
 @brief		Class responsible for drawing gcf agents -- as changing ellipses
 */
class GCFVisAgent : public MengeVis::Runtime::VisAgent {
 public:
  /*!
   @brief		The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  std::string getElementName() const override { return Agent::NAME; }

  /*!
   @brief		Creates a clone of this vis agent, moving the contained agent to the clone.
   
   The caller is responsible for the new VisAgent instance.

   @returns	  A copy of this visualization agent (with a pointer to the same underlying simulation
              agent).
   */
  VisAgent* moveToClone() override;

  /*!
   @brief		Draw the agent into the 3D world.
   */
  void drawAgent(float r, float g, float b) const override;

 protected:
  /*!
   @brief		Confirm that the agent provided is compatible with this VisAgent instance.

   Sub-classes that care about the *type* of agent that is passed in should override this and
   perform the appropriate test, returning true if the BaseAgent instance is compatible.
   */
  bool doValidateAgent(const Menge::Agents::BaseAgent* agent) override;
};
}  // namespace GCF

#endif  // __GCF_VIS_AGENT_H__
