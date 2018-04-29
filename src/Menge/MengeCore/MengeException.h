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
 @file    MengeException.h
 @brief    The base definition for exceptions in Menge.

 TODO: Refactor all exceptions to inherit from this class.
 */

#ifndef __MENGE_EXCEPTION_H__
#define __MENGE_EXCEPTION_H__

#include <exception>
#include <string>
#include "MengeCore/CoreConfig.h"

#pragma warning(disable : 4275)
namespace Menge {

/*!
 @brief    Base exception class for menge operations.
 */
class MENGE_API MengeException : public std::exception {
 public:
  /*!
   @brief    Default constructor.
   */
  MengeException() : std::exception() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  MengeException(const std::string& s) : std::exception(), _msg(s) {}

  /*!
   @brief    Destructor.
   */
  ~MengeException() throw() {}

  /*!
   @brief    Provides the exception message.

   @returns  A pointer to the exception message.
   */
  virtual const char* what() const throw() { return _msg.c_str(); }

  /*!
   @brief    The exception message.
   */
  std::string _msg;
};

#pragma warning(default : 4275)

/*!
 @brief    Base class for *fatal* exceptions.

 A fatal exception is one that requires the simulator to stop because it has achieved an inoperable
 state.
 */
class MENGE_API MengeFatalException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  MengeFatalException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  MengeFatalException(const std::string& s) : MengeException(s) {}
};

}  // namespace Menge

#endif  // __MENGE_EXCEPTION_H__
