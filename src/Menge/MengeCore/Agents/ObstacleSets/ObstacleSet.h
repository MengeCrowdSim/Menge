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
 @file    ObstacleSet.h
 @brief    The definition of the ObstacleSet element -- the numbers and positions of obstacles in a
          set.
 */
#ifndef __OBSTACLE_SET_H__
#define __OBSTACLE_SET_H__

#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/Element.h"

namespace Menge {

namespace Agents {
// forward declaration
class Obstacle;
class ObstacleSetFactory;

/*!
 @brief    Exception class for obstacle set instantiation
 */
class MENGE_API ObstacleSetException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  ObstacleSetException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ObstacleSetException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal obstacle set exception.
 */
class MENGE_API ObstacleSetFatalException : public ObstacleSetException,
                                            public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  ObstacleSetFatalException() : MengeException(), ObstacleSetException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  ObstacleSetFatalException(const std::string& s)
      : MengeException(s), ObstacleSetException(), MengeFatalException() {}
};

/*!
 @brief    The base class for obstacle set definitions

 This is an abstract class defining the functions of an obstacle set in Menge Obstacle sets must be
 able to provide a count of Obstacles in their definition and pass those obstacles on to the
 SpatialQuery.
 */
class MENGE_API ObstacleSet : public Element {
 public:
  /*!
   @brief    Default constructor
   */
  ObstacleSet();

  friend class ObstacleSetFactory;

 protected:
  /*!
   @brief    Protected destructor.
   
   The destroy method should be used to delete an instance of an obstacle set.
   */
  virtual ~ObstacleSet();

 public:
  /*!
   @brief    Reports the number of obstacles in the set.

   @returns  The number of obstacles in this set.
   */
  virtual size_t obstacleCount() = 0;

  /*!
   @brief    Returns the ith obstacle.

   @param    i    The index of the requested obstacle.
   @returns  A pointer to the corresponding Obstacle instance.
   @throws    ObstacleSetException if the index, i, is invalid.
   */
  virtual Obstacle* getObstacle(size_t i) = 0;

  /*!
   @brief    Sets the class of this obstacle set.

   @param    cl    the class id to which the obstacle set belongs.
   */
  virtual void setClass(size_t cl) { _class = cl; };

  /*!
   @brief    Gets the class of this obstacle set.

   @returns  The class id to which the obstacle set belongs.
   */
  virtual size_t getClass() { return _class; };

 protected:
  /*!
   @brief    The class for the obstacles in this set.

   This works with the BaseAgent::_obstacleSet member to determine which obstacles affect an agent.
   */
  size_t _class;
};

}  // namespace Agents
}  // namespace Menge
#endif  // __OBSTACLE_SET_H__
