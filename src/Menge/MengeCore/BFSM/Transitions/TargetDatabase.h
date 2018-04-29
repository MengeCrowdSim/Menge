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
 @file    TargetDatabase.h
 @brief    Central database for querying available behavior transition targets.

 For transition targets to be used in the finite state machine, they must register themselves into
 the TargetDatabase.  This is done via the PluginEngine.
 */

#ifndef __TARGET_DATABASE_H__
#define __TARGET_DATABASE_H__

#include "MengeCore/BFSM/Transitions/Target.h"
#include "MengeCore/BFSM/Transitions/TargetFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The database of registered transition targets.
 */
typedef ElementDB<TargetFactory, TransitionTarget> TargetDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 @brief    Explicit specialization of addBuiltins for the TransitionTarget Database
 */
template <>
void ElementDB<BFSM::TargetFactory, BFSM::TransitionTarget>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the TransitionTarget Database
 */
template <>
std::string ElementDB<BFSM::TargetFactory, BFSM::TransitionTarget>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge
#endif  // __TARGET_DATABASE_H__
