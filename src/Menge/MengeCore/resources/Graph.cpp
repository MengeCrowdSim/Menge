/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation
for educational, research, and non-profit purposes, without fee, and without a
written agreement is hereby granted, provided that the above copyright notice,
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North
Carolina at Chapel Hill. The software program and documentation are supplied "as is,"
without any accompanying services from the University of North Carolina at Chapel
Hill or the authors. The University of North Carolina at Chapel Hill and the
authors do not warrant that the operation of the program will be uninterrupted
or error-free. The end-user understands that the program was developed for research
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeCore/resources/Graph.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/Core.h"
#include "MengeCore/resources/GraphEdge.h"
#include "MengeCore/resources/MinHeap.h"
#include "MengeCore/resources/RoadMapPath.h"

#include <fstream>
#include <iostream>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace Menge {

/////////////////////////////////////////////////////////////////////
//          Implementation of Graph
/////////////////////////////////////////////////////////////////////

const std::string Graph::LABEL("graph");

/////////////////////////////////////////////////////////////////////

Graph::Graph(const std::string& fileName)
    : Resource(fileName),
      _vCount(0),
      _vertices(0x0),
      DATA_SIZE(0),
      STATE_SIZE(0),
      _HEAP(0x0),
      _DATA(0x0),
      _STATE(0x0) {}

//////////////////////////////////////////////////////////////////////////////////////

Graph::~Graph() { clear(); }

//////////////////////////////////////////////////////////////////////////////////////

void Graph::clear() {
  if (_vCount) {
    _vCount = 0;
    delete[] _vertices;
    _vertices = 0x0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

Resource* Graph::load(const std::string& fileName) {
  std::ifstream f;
  f.open(fileName.c_str(), std::ios::in);

  if (!f.is_open()) {
    logger << Logger::ERR_MSG << "Error opening the roadmap file: " << fileName << "\n";
    return 0x0;
  }

  Graph* graph = new Graph(fileName);

  // load vertices
  if (!(f >> graph->_vCount)) {
    logger << Logger::ERR_MSG;
    logger << "Error parsing roadmap: file didn't start with an int (vertex count).\n";
    graph->destroy();
    return 0x0;
  }
  graph->_vertices = new GraphVertex[graph->_vCount];
  size_t* vertNbr = new size_t[graph->_vCount];
  int degree;
  float x, y;
  for (size_t i = 0; i < graph->_vCount; ++i) {
    if (!(f >> degree >> x >> y)) {
      logger << Logger::ERR_MSG << "Error parsing roadmap: format error for vertex ";
      logger << (i + 1) << ".\n";
      graph->destroy();
      delete[] vertNbr;
      return 0x0;
    }
    graph->_vertices[i].setID(i);
    graph->_vertices[i].setPosition(Vector2(x, y));
    graph->_vertices[i].setDegree(degree);
    vertNbr[i] = 0;
  }

  // load edges
  size_t eCount;
  if (!(f >> eCount)) {
    logger << Logger::ERR_MSG;
    logger << "Error parsing roadmap: didn't find edge count when expected.\n";
    delete[] vertNbr;
    graph->destroy();
    return 0x0;
  }

  bool validEdges = true;

  for (size_t e = 0; e < eCount; ++e) {
    GraphEdge edge;
    int from, to;
    if (!(f >> from >> to)) {
      logger << Logger::ERR_MSG << "Error parsing roadmap: format error for edge ";
      logger << (e + 1) << ".\n";
      delete[] vertNbr;
      graph->destroy();
      return 0x0;
    }
    edge.setDistance(graph->_vertices[from].getDistance(graph->_vertices[to]));
    edge.setNeighbor(&graph->_vertices[to]);
    if (!graph->_vertices[from].setEdge(edge, vertNbr[from])) {
      validEdges = false;
      logger << Logger::ERR_MSG << "Vertex " << from << " declared to have ";
      logger << graph->_vertices[from].getEdgeCount();
      logger << " edges.  Attempting to add the " << (vertNbr[from] + 1);
      logger << "th edge.\n";
    }
    ++vertNbr[from];
    edge.setNeighbor(&graph->_vertices[from]);
    if (!graph->_vertices[to].setEdge(edge, vertNbr[to])) {
      validEdges = false;
      logger << Logger::ERR_MSG << "Vertex " << to << " declared to have ";
      logger << graph->_vertices[to].getEdgeCount();
      logger << " edges.  Attempting to add the " << (vertNbr[to] + 1);
      logger << "th edge.\n";
    }
    ++vertNbr[to];
  }

  bool valid = validEdges;

  if (!validEdges) {
    logger << Logger::ERR_MSG << "Edges were improperly configured -- no valid roadmap.\n";
  }

  for (size_t v = 0; v < graph->_vCount; ++v) {
    if (vertNbr[v] != graph->_vertices[v].getEdgeCount()) {
      logger << Logger::ERR_MSG << "Vertex " << v << " declared to have ";
      logger << graph->_vertices[v].getEdgeCount() << " edges.  Only ";
      logger << vertNbr[v] << " assigned.\n";
      valid = false;
    }
  }
  if (!valid) {
    delete[] vertNbr;
    graph->destroy();
    return 0x0;
  }

  delete[] vertNbr;
  graph->initHeapMemory();
  return graph;
}

//////////////////////////////////////////////////////////////////////////////////////

RoadMapPath* Graph::getPath(const Agents::BaseAgent* agent, const BFSM::Goal* goal) {
  // Find the closest visible node to agent position
  size_t startID = getClosestVertex(agent->_pos, agent->_radius, Clearance::Partial);
  // Find the closest visible node to goal position
  Vector2 goalPos = goal->getCentroid();
  // TODO(curds01): Investigate finding a path to the goal *area* rather than centroid; more
  // difficult, but provides the possibility to get more efficient paths. Not just efficient, but
  // may also eliminate false negatives. See https://github.com/MengeCrowdSim/Menge/issues/121.
  size_t endID = getClosestVertex(goalPos, agent->_radius, Clearance::Full);
  if (startID == -1 || endID == -1) {
    return nullptr;
  }
  // Compute the path based on those nodes
  RoadMapPath* path = getPath(startID, endID);
  if (path) {
    path->setGoalPos(goal);
  }
  return path;
}

//////////////////////////////////////////////////////////////////////////////////////

RoadMapPath* Graph::updatePathForGoal(const Agents::BaseAgent* agent, RoadMapPath* path) {
  assert(path->getGoal()->moves() &&
         "Graph::updatePathForGoal() should only be called on mobile goals");
  const BFSM::Goal& goal = *path->getGoal();
  // Confirm that the goal position is still visible to the last node in the path. 
  const Vector2& goal_pos = goal.getCentroid();
  const size_t num_way_point = path->getWayPointCount();
  const Vector2& last_way_point = path->getWayPoint(num_way_point - 1);
  if (!Menge::SPATIAL_QUERY->queryVisibility(last_way_point, goal_pos, agent->_radius)) {
    return getPath(agent, &goal);
  }
  return path;
}

//////////////////////////////////////////////////////////////////////////////////////

const GraphVertex* Graph::getVertex(size_t i) const {
  assert(i < _vCount && "Indexing invalid graph vertex");
  return &_vertices[i];
}

//////////////////////////////////////////////////////////////////////////////////////

size_t Graph::getClosestVertex(const Vector2& point, float radius, Clearance clearance) {
  assert(_vCount > 0 && "Trying to operate on an empty roadmap");
  // TODO(curds01): Make this faster via a spatial hash - in other words, test them in the order
  // of closest to farthest.

  float bestDistSq = INFTY;
  size_t bestID = -1;
  for (size_t i = 0; i < _vCount; ++i) {
    float testDistSq = absSq(_vertices[i].getPosition() - point);
    if (testDistSq < bestDistSq) {
      if ((clearance == Clearance::Full &&
           Menge::SPATIAL_QUERY->queryVisibility(point, _vertices[i].getPosition(), radius)) ||
          Menge::SPATIAL_QUERY->linkIsTraversible(point, _vertices[i].getPosition(), radius)) {
        bestDistSq = testDistSq;
        bestID = i;
      }
    }
  }

  return bestID;
}

//////////////////////////////////////////////////////////////////////////////////////

RoadMapPath* Graph::getPath(size_t startID, size_t endID) {
  const size_t N = _vCount;
#ifdef _OPENMP
  // Assuming that threadNum \in [0, omp_get_max_threads() )
  const unsigned int threadNum = omp_get_thread_num();
  AStarMinHeap heap(_HEAP + threadNum * N, _DATA + threadNum * DATA_SIZE,
                    _STATE + threadNum * STATE_SIZE, _PATH + threadNum * N, N);
#else
  AStarMinHeap heap(_HEAP, _DATA, _STATE, _PATH, N);
#endif

  const Vector2 goalPos(_vertices[endID].getPosition());

  heap.g((unsigned int)startID, 0);
  heap.h((unsigned int)startID, computeH(startID, goalPos));
  heap.f((unsigned int)startID, heap.h((unsigned int)startID));
  heap.push((unsigned int)startID);

  bool found = false;
  while (!heap.empty()) {
    unsigned int x = heap.pop();

    if (x == endID) {
      found = true;
      break;
    }

    GraphVertex& vert = _vertices[x];

    const size_t E_COUNT = vert.getEdgeCount();
    for (size_t n = 0; n < E_COUNT; ++n) {
      const GraphVertex* nbr = vert.getNeighbor(n);
      size_t y = nbr->getID();
      if (heap.isVisited((unsigned int)y)) continue;
      float distance = vert.getDistance(n);
      float tempG = heap.g(x) + distance;

      bool inHeap = heap.isInHeap((unsigned int)y);
      if (!inHeap) {
        heap.h((unsigned int)y, computeH(y, goalPos));
      }
      if (tempG < heap.g((unsigned int)y)) {
        heap.setReachedFrom((unsigned int)y, (unsigned int)x);
        heap.g((unsigned int)y, tempG);
        heap.f((unsigned int)y, tempG + heap.h((unsigned int)y));
      }
      if (!inHeap) {
        heap.push((unsigned int)y);
      }
    }
  }
  if (!found) {
    logger << Logger::ERR_MSG << "Was unable to find a path from " << startID;
    logger << " to " << endID << "\n";
    return 0x0;
  }

  // Count the number of nodes in the path
  size_t wayCount = 1;  // for the startID
  size_t next = endID;
  while (next != startID) {
    ++wayCount;
    next = heap.getReachedFrom((unsigned int)next);
  }

  RoadMapPath* path = new RoadMapPath(wayCount);
  next = endID;
  for (size_t i = wayCount; i > 0; --i) {
    path->setWayPoint(i - 1, _vertices[next].getPosition());
    next = heap.getReachedFrom((unsigned int)next);
  }

  return path;
}

/////////////////////////////////////////////////////////////////////

void Graph::initHeapMemory() {
  int threadCount = 1;
#ifdef _OPENMP
  logger << Logger::INFO_MSG << "OMP ENABLED!\n";
  threadCount = omp_get_max_threads();
#endif
  logger << Logger::INFO_MSG << "Caching roadmap A* data for " << threadCount;
  logger << " threads\n";
  if (_DATA) {
    delete[] _DATA;
    _DATA = 0x0;
    delete[] _STATE;
    _STATE = 0x0;
    delete[] _HEAP;
    _HEAP = 0x0;
    delete[] _PATH;
    _PATH = 0x0;
  }

  DATA_SIZE = 3 * _vCount;   // the number of floats per thread in _data
  STATE_SIZE = 2 * _vCount;  // the number of bools, per thread, in _state;
  if (_vCount) {
    _DATA = new float[threadCount * DATA_SIZE];
    _STATE = new bool[threadCount * STATE_SIZE];
    _HEAP = new unsigned int[threadCount * _vCount];
    _PATH = new unsigned int[threadCount * _vCount];
  }
}

//////////////////////////////////////////////////////////////////////////////////////

GraphPtr loadGraph(const std::string& fileName) throw(ResourceException) {
  Resource* rsrc = ResourceManager::getResource(fileName, &Graph::load, Graph::LABEL);
  if (rsrc == 0x0) {
    logger << Logger::ERR_MSG << "No resource available\n";
    throw ResourceException();
  }
  Graph* graph = dynamic_cast<Graph*>(rsrc);
  if (graph == 0x0) {
    logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a Graph\n";
    throw ResourceException();
  }
  return GraphPtr(graph);
}

//////////////////////////////////////////////////////////////////////////////////////

}  // namespace Menge
