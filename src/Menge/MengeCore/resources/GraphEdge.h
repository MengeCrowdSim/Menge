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
 @file    GraphEdge.h
 @brief    The definition of a graph edge for performing graph searches and path planning.
 */

#ifndef __GRAPH_EDGE_H__
#define __GRAPH_EDGE_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {

// forward declaration
class GraphVertex;

/*!
 @brief    A graph edge.
 */
class MENGE_API GraphEdge {
 public:
  /*!
   @brief    Constructor.
   */
  GraphEdge();

  /*!
   @brief    Destructor
   */
  ~GraphEdge();

  /*!
   @brief    Set's the neighboring vertex

   @param    v    The first connected vertex.
   */
  void setNeighbor(GraphVertex* v) { _nbr = v; }

  /*!
   @brief    Sets the distance between the vertices

   @param    d    The distance between the two vertices.
   */
  void setDistance(float d) { _distance = d; }

  /*!
   @brief    Reports the distance between the vertices connected by this edge.

   @returns  The length of this edge.
   */
  inline float getDistance() const { return _distance; }

  /*!
   @brief    Returns the first attached GraphVertex

   @returns  A pointer to the first graph vertex.
   */
  GraphVertex* getNeighbor() const { return _nbr; }

 protected:
  /*!
   @brief    The distance between the two vertices connected by this edge.
   */
  float _distance;

  /*!
   @brief    A pointer to the first graph vertex connected by this edge.
   */
  GraphVertex* _nbr;
};
}  // namespace Menge
#endif  // __GRAPH_EDGE_H__
