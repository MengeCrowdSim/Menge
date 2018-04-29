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
#ifndef __VIS_ELEMENT_EXCEPTION_H__
#define __VIS_ELEMENT_EXCEPTION_H__

#include "MengeCore/MengeException.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {
namespace Runtime {
/*!
 @brief   Exception base class for vis elements.
 */
class MENGEVIS_API VisElementException : public virtual Menge::MengeException {
 public:
  /*!
   @brief   Default constructor.
   */
  VisElementException() : MengeException() {}

  /*!
   @brief   Constructor with message.

   @param   s   The exception-specific message.
   */
  VisElementException(const std::string& s) : Menge::MengeException(s) {}
};

/*!
 @brief   The fatal vis element exception.
 */
class MENGEVIS_API VisElementFatalException : public VisElementException,
                                              public Menge::MengeFatalException {
 public:
  /*!
   @brief   Default constructor.
   */
  VisElementFatalException() : VisElementException(), Menge::MengeFatalException() {}

  /*!
   @brief   Constructor with message.

   @param   s   The exception-specific message.
   */
  VisElementFatalException(const std::string& s)
      : VisElementException(s), Menge::MengeFatalException() {}
};

}  // namespace Runtime
}  // namespace MengeVis

#endif  // __VIS_ELEMENT_EXCEPTION_H__
