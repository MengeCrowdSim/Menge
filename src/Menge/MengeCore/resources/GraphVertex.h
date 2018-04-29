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
 @file    GraphVertex.h
 @brief    The definition of a graph vertex for performing graph searches and path planning.
 */

#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#include "MengeCore/mengeCommon.h"
#include "MengeCore/resources/GraphEdge.h"

namespace Menge {

using Math::Vector2;

/*!
 @brief    A graph vertex.
 */
class MENGE_API GraphVertex {
 public:
  /*!
   @brief    Constructor
   */
  GraphVertex();

  /*!
   @brief    Destructor
   */
  ~GraphVertex();

  /*!
   @brief    Assignment operator
   */
  GraphVertex& operator=(const GraphVertex& n);

  /*!
   @brief    Computes the distance between this vertex and the given vertex.

   @param    other    The other vertex.
   @returns  The Euclidian distance between this vertex and the other vertex.
   */
  float getDistance(const GraphVertex& other) { return abs(_pos - other._pos); }

  /*!
   @brief    Reports the distance to the ith neighbor.

   @param    i    The index of the desired neighbor.
   @returns  The distance to the ith neghbor.
   */
  float getDistance(size_t i) const;

  /*!
   @brief    Sets the node identifier.

   @param    id    The identifier to set this node to.
   */
  void setID(size_t id) { _id = id; }

  /*!
   @brief    Retrive the identifier for this node.
   */
  size_t getID() const { return _id; }

  /*!
   @brief    Sets the graph vertex's position.

   @param    p    The point value to set.
   */
  inline void setPosition(const Vector2& p) { _pos.set(p); }

  /*!
   @brief    Reports the position of the vertex.

   @returns  The position of the vertex.
   */
  Vector2 getPosition() const { return _pos; }

  /*!
   @brief    Sets the ith edge for the vertex.

   @param    edge    The edge to set
   @param    i       The index to set.
   @returns  True if the operation is valid, false otherwise.
   */
  bool setEdge(const GraphEdge& edge, size_t i);

  /*!
   @brief    Reports the number of vertices adjacent to this vertex.
   */
  size_t getNeighborCount() const { return _edgeCount; }

  /*!
   @brief    Returns a pointer to the ith neighbor.

   @param    i    The index of the neighboring vertex to retrieve. The index is defined in the range
                [0, N-1], where this vertex has N neighbors.
   @returns  A pointer to the ith neighboring vertex.
   */
  const GraphVertex* getNeighbor(size_t i) const;

  /*!
   @brief    Sets the degree -- the number of neighbors this vertex has.

   @param    degree    The number of neighbors this vertex has.
   */
  void setDegree(size_t degree);

  /*!
   @brief    Reports the number of edges on the node.

   @returns  The number of edges.
   */
  size_t getEdgeCount() const { return _edgeCount; }

  /*!
   @brief    Retrieves the ith edge connected to this node.

   @param    i    The local index of the desired edge.
   @returns  A pointer to the ith edge.
   */
  GraphEdge& getEdge(size_t i) { return _edges[i]; }

  /*!
   @brief    Retrieves a const pointer to the ith edge connected to this node.

   @param    i    The local index of the desired edge.
   @returns  A pointer to the ith edge.
   */
  const GraphEdge& getEdge(size_t i) const { return _edges[i]; }

 protected:
  /*!
   @brief    An array of edges connecting to other vertices
   */
  GraphEdge* _edges;

  /*!
   @brief    The number of edges connecting to this vertex.
   */
  size_t _edgeCount;

  /*!
   @brief    The position of this vertex.
   */
  Vector2 _pos;

  /*!
   @brief    The identifier for this vertex.
   */
  size_t _id;
};

}  // namespace Menge
#endif  // __GRAPH_VERTEX_H__
