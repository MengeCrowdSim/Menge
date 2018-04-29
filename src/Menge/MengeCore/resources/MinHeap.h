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
 @file       MinHeap.h
 @brief      Special MinHeap implementaiton which will work with the A* algorithm and, particulary,
             is threadsafe.
 */

#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#ifndef _WIN32
#include "sys/types.h"
#endif

namespace Menge {

/*!
 @brief    An implementation of a min heap for A* algorithm.

 The heap needs to be able to restructure itself because the values of nodes IN the heap can change
 due to the A* algorithm.

 Also tracks all of the A* data.
 */
class AStarMinHeap {
 public:
  /*!
   @brief    Constructor.

   @param    heap    A pointer to a block of memory to be used for the heap for N nav mesh nodes.
   @param    data    A pointer to a block of memory to be used for the A* data (f, g, & h) for N nav
                    mesh nodes.
   @param    state    A pointer to a block of memory to be used for the heap state (in heap &
                    finished) for N nav mesh nodes.
   @param    path    A pointer to a block of memory to be used for recording the path taken.
   @param    N        The number of nodes.
   */
  AStarMinHeap(unsigned int* heap, float* data, bool* state, unsigned int* path, size_t N);

  /*!
   @brief    Reports if the heap is empty.

   @returns  True if the heap is empty, false if it is not.
   */
  bool empty() const { return _nextFree == 0; }

  /*!
   @brief    Extract the minimum keyed value.

   @returns  The index of the nav mesh node with the minimum keyed value.
   */
  unsigned int pop();

  /*!
   @brief    Insert a new value into the heap.

   @param    x    The index of the node to insert.
   */
  void push(unsigned int x);

  /*!
   @brief    Set the g-value for the given node.

   @param    node    The nav mesh node index for which the g-value is to be set.
   @param    value    The g-value for the given node.
   */
  inline void g(unsigned int node, float value) { _g[node] = value; }

  /*!
   @brief    Retrieve the g-value for the given node.

   @param    node    The nav mesh node index whose g-value is to be retrieved.
   @returns  The g-value of the indexed node.
   */
  inline float g(unsigned int node) const { return _g[node]; }

  /*!
   @brief    Set the h-value for the given node.

   @param    node    The nav mesh node index for which the h-value is to be set.
   @param    value    The h-value for the given node.
   */
  inline void h(unsigned int node, float value) { _h[node] = value; }

  /*!
   @brief    Retrieve the h-value for the given node.

   @param    node    The nav mesh node index whose h-value is to be retrieved.
   @returns  The h-value of the indexed node.
   */
  inline float h(unsigned int node) const { return _h[node]; }

  /*!
   @brief    Set the f-value for the given node.

   @param    node    The nav mesh node index for which the f-value is to be set.
   @param    value    The f-value for the given node.
   */
  inline void f(unsigned int node, float value) {
    if (_inHeap[node])
      changeF(node, value);
    else
      _f[node] = value;
  }

  /*!
   @brief    Retrieve the f-value for the given node.

   @param    node    The nav mesh node index whose f-value is to be retrieved.
   @returns  The f-value of the indexed node.
   */
  inline float f(unsigned int node) const { return _f[node]; }

  /*!
   @brief    Change the f-value for the given node in the heap.

   @param    node    The node whose f-value is to change.
   @param    value    The new f-value.
   */
  void changeF(unsigned int node, float value);

  /*!
   @brief    Reports if the node has been visited.

   @param    node    The index of the nav mesh node to test.
   @returns  True if the node has been visited, false otherwise.
   */
  inline bool isVisited(unsigned int node) const { return _visited[node]; }

  /*!
   @brief    Reports if the node is currently in the heap.

   @param    node    The index of the nav mesh node to test.
   @returns  True if the node is in the heap, false otherwise.
   */
  inline bool isInHeap(unsigned int node) const { return _inHeap[node]; }

  /*!
   @brief    Sets the node from which this node was reached.

   @param    dst    The index of the nav mesh node reached.
   @param    src    The index of the nav mesh node from which dst was reached.
   */
  inline void setReachedFrom(unsigned int dst, unsigned int src) { _cameFrom[dst] = src; }

  /*!
   @brief    Report the node from which this node was reached.

   @param    dst    The index of the nav mesh node reached.
   @returns  The index of the nav mesh node from which dst was reached.
   */
  inline unsigned int getReachedFrom(unsigned int dst) const { return _cameFrom[dst]; }

 protected:
  /*!
   @brief    Resets the heap and all data.

   @param    N    The number of nodes in the nav mesh.
   */
  void initialize(size_t N);

  /*!
   @brief    The VALUE of the minimum keyed heap member
   */
  float _minKey;

  /*!
   @brief    The location of the minimum keyed heap member.
   */
  unsigned int _minIdx;

  /*!
   @brief    The location of the next free slot on the heap
   */
  unsigned int _nextFree;

  /*!
   @brief    An array of f-values for each node in the navigation mesh.

   This is a value used in the A* algorithm. The memory is supplied upon construction.
   */
  float* _f;

  /*!
   @brief    An array of g-values for each node in the navigation mesh.

   This is a value used in the A* algorithm. The memory is supplied upon construction.
   */
  float* _g;

  /*!
   @brief    An array of h-values for each node in the navigation mesh.

   This is a value used in the A* algorithm. The memory is supplied upon construction.
   */
  float* _h;

  /*!
   @brief    An array of booleans reporting if the given node is in the heap.
   
   The memory is supplied upon construction.
   */
  bool* _inHeap;

  /*!
   @brief    An array of booleans reporting if the given node has been visited.
   
   The memory is supplied upon construction.
   */
  bool* _visited;

  /*!
   @brief    An array of node indices of the nodes in the heap.
   The memory is supplied upon construction.
   */
  unsigned int* _heap;

  /*!
   @brief    An array of node indices which indicate how a node was

   reached.  The memory is supplied upon construction.
   */
  unsigned int* _cameFrom;
};
}  // namespace Menge

#endif  // __MIN_HEAP_H__
