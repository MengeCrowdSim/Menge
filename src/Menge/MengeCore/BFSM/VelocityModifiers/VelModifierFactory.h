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
 @file    VelModifierFactory.h
 @brief    The factory for parsing xml data and instantiating velocity Modifierss.
 */

#ifndef __VEL_MODIFIER_FACTORY_H__
#define __VEL_MODIFIER_FACTORY_H__

#include <string>
#include "MengeCore/CoreConfig.h"

#include "MengeCore/BFSM/VelocityModifiers/VelModifier.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    A class for parsing the xml description of a velocity MODIFIER and instantiating
          particular instances.
 */
class MENGE_API VelModFactory : public ElementFactory<VelModifier> {};
}  // namespace BFSM
}  // namespace Menge

#endif  // __VEL_MODIFIER_FACTORY_H__
