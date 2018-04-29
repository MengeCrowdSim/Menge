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
 @file    ListObstacleSet.h
 @brief    An abstract class for any obstacle set which will need an _obstacles list. 
 
 Provides transformations from vertex vectors
 */

#ifndef __LIST_OBSTACLE_SET_H__
#define __LIST_OBSTACLE_SET_H__

#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/ObstacleSets/ObstacleSet.h"
#include "MengeCore/Agents/ObstacleSets/ObstacleSetFactory.h"
#include "MengeCore/Agents/ObstacleSets/ObstacleVertexList.h"
#include "MengeCore/mengeCommon.h"

#include <vector>

namespace Menge {

namespace Agents {

/*!
 @brief    Definition of obstacle set class which produces obstacles based on explicit definition in
          an XML file.
 */
class MENGE_API ListObstacleSet : public ObstacleSet {
 public:
  /*!
   @brief    Constructor
   */
  ListObstacleSet();

 protected:
  /*!
   @brief    Destructor
   */
  ~ListObstacleSet();

 public:
  /*!
   @brief    Reports the number of obstacles in the set

   @returns  The number of obstacles in this set
   */
  virtual size_t obstacleCount() { return _obstacles.size(); }

  /*!
   @brief    Get the ith obstacle.

   @param    i    The index of the requested obstacle.
   @returns  The Obstacle object.
   @throws    ObstacleSetException if the index, `i`, is invalid.
   */
  virtual Obstacle* getObstacle(size_t i);

  /*!
   @brief    Adds an obstacle  to the generator.

   @param    o    The Obstacle Object to be added
   @returns  true if the obstacle was added successfully
   @throws    ObstacleSetException if the obstacle is malformed
   */
  bool addObstacle(Obstacle* o);

  /*!
   @brief    Adds an obstacle  to the generator from a list of vertices.

   @param    o     ObstacleVertexList struct representing the incoming obstacle.
   @returns  true if the obstacle was added successfully.
   @throws    ObstacleSetException if the obstacle is malformed.
   */
  bool addObstacle(ObstacleVertexList o);

  friend class ExplicitObstacleSetFactory;

 protected:
  /*!
   @brief    The obstacles in an internal list
   */
  std::vector<Obstacle*> _obstacles;
};

}  // namespace Agents
}  // namespace Menge
#endif  // __EXPLICIT_OBSTACLE_SET_H__
