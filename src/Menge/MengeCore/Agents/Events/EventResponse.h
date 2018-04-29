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
 @file    EventResponse.h
 @brief    A response to an event trigger.
 */

#ifndef __EVENT_RESPONSE_H__
#define __EVENT_RESPONSE_H__

#include <string>

namespace Menge {

// forward declarations
class EventEffect;
class EventTarget;

/*!
 @brief    A response to an event trigger.  It combines an effect with a target.
 */
class EventResponse {
 public:
  /*!
   @brief    Constructor.
   */
  EventResponse();

  /*!
   @brief    Constructor.

   @param    effect    The name of the effect for this response.
   @param    target    The name of the target for this response.
   */
  EventResponse(const std::string& effect, const std::string& target);

  /*!
   @brief    Finalizes the response.
   */
  void finalize();

  /*!
   @brief    Applies the response to the simulation.
   */
  void apply();

 protected:
  /*!
   @brief    The name of the effect.
   */
  std::string _effectName;

  /*!
   @brief    The effect in this response.
   */
  EventEffect* _effect;

  /*!
   @brief    The name of the target.
   */
  std::string _targetName;

  /*!
   @brief    The target for this response.
   */
  EventTarget* _target;
};

}  // namespace Menge

#endif  // __EVENT_RESPONSE_H__
