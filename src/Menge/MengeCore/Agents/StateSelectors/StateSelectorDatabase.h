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
 @file    StateSelectorDatabase.h
 @brief    Central database for querying available profile selector implementations.

 For profile selectors to be used in simulation, they must register themselves into the
 ProfileSelectorDatabase.  This is done via the PluginEngine.
 */

#ifndef __STATE_SELECTOR_DATABASE_H__
#define __STATE_SELECTOR_DATABASE_H__

#include "MengeCore/Agents/StateSelectors/StateSelector.h"
#include "MengeCore/Agents/StateSelectors/StateSelectorFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace Agents {
/*!
 @brief    The database of registered state selector implementations.
 */
typedef ElementDB<StateSelectorFactory, StateSelector> StateSelectorDB;

}  // namespace Agents

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<Agents::StateSelectorFactory, Agents::StateSelector>::addBuiltins();
template <>
std::string ElementDB<Agents::StateSelectorFactory, Agents::StateSelector>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge
#endif  // __STATE_SELECTOR_DATABASE_H__
