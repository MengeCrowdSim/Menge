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
 @file    StateSelector.h
 @brief    The definition of the agent initial state selector element.

 This is the mechanism which determines which state in the FSM the agent starts in.
 */
#ifndef __STATE_SELECTOR_H__
#define __STATE_SELECTOR_H__

#include "MengeCore/PluginEngine/Element.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {
// forward declaration
class BaseAgent;

/*!
 @brief    Exception class for state selector computation.
 */
class MENGE_API StateSelectorException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  StateSelectorException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  StateSelectorException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal state selector exception.
 */
class MENGE_API StateSelectorFatalException : public StateSelectorException,
                                              public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  StateSelectorFatalException()
      : MengeException(), StateSelectorException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  StateSelectorFatalException(const std::string& s)
      : MengeException(s), StateSelectorException(), MengeFatalException() {}
};

/*!
 @brief    The base class for selecting an agent's intial state.

 This is an abstract class, primarily defining the mechanism for selecting an agent's initial state.
 */
class MENGE_API StateSelector : public Element {
 public:
  /*!
   @brief    Constructor
   */
  StateSelector();

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~StateSelector();

 public:
  /*!
   @brief    Provides the name of a state.

   Ths must be overriden by child classes.

   @returns:    The name of the state.
   */
  virtual std::string getState() = 0;
};

}  // namespace Agents
}  // namespace Menge
#endif  // __STATE_SELECTOR_H__
