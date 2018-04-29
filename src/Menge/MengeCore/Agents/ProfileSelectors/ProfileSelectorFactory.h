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
 @file    ProfileSelectorFactory.h
 @brief    The factory for parsing xml data and instantiating profile selector implementations.
 */

#ifndef __PROFILE_SELECTOR_FACTORY_H__
#define __PROFILE_SELECTOR_FACTORY_H__

#include "MengeCore/Agents/ProfileSelectors/ProfileSelector.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

namespace Menge {

namespace Agents {
/*!
 @brief    A class for parsing the xml description of an agent profile selector elements and
          instantiating particular instances.
 */
class MENGE_API ProfileSelectorFactory : public ElementFactory<ProfileSelector> {};
}  // namespace Agents
}  // namespace Menge
#endif  // __PROFILE_SELECTOR_FACTORY_H__
