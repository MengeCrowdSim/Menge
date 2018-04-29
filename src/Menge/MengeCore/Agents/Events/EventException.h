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
 @file    EventException.h
 @brief    The definition of event-related exceptions.
 */

#ifndef __EVENT_EXCEPTION_H__
#define __EVENT_EXCEPTION_H__

#include <string>
#include "MengeCore/MengeException.h"

namespace Menge {
/*!
 @brief    The base event exception.
 */
class MENGE_API EventException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  EventException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  EventException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal event exception.
 */
class MENGE_API EventFatalException : public EventException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  EventFatalException() : MengeException(), EventException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  EventFatalException(const std::string& s)
      : MengeException(s), EventException(), MengeFatalException() {}
};

}  // namespace Menge

#endif  // __EVENT_EXCEPTION_H__
