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

/*!
 *	@file		Graph.h
 *	@brief		The definition of a graph for performing graph searches
 *				and path planning.
 */

#ifndef __GRAPH_H__
#define	__GRAPH_H__

#include "mengeCommon.h"
#include "Resource.h"
#include "GraphVertex.h"

namespace Menge {

	// Forward declarations
	class GraphEdge;
	class RoadMapPath;
	namespace BFSM {
		class Goal;
	}

	namespace Agents {
		class BaseAgent;
	}

	/*!
	 *	@brief		A roadmap graph and the infrastructure for performing graph
	 *				searches.  NOTE: This implementation assumes that the graph 
	 *				doesn't change.
	 */
	class MENGE_API Graph : public Resource {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		fileName		The name of the file which contains the vector field definition.
		 */
		Graph( const std::string & fileName );

	protected:
		/*!
		 *	@brief		Destructor.
		 */
		~Graph();

	public:
		/*!
		 *	@brief		Returns a unique resource label to be used to identify
		 *				different resource *types* which use the same underlying
		 *				file data.
		 */
		virtual const std::string & getLabel() const { return LABEL; }

		/*!
		 *	@brief		Clears the graph -- such that it has no vertices and no edges.
		 */
		void clear();

		/*!
		 *	@brief		Parses a graph definition and returns a pointer to it.
		 *
		 *	This function works in conjunction with the ResourceManager.  That is why it
		 *	returns a pointer, not to a Graph, but to a Resource.  The ResourceManager
		 *	uses it to load and instantiate Graph instances.
		 *
		 *	@param		fileName		The path to the file containing the VectorField
		 *								definition.
		 *	@returns	A pointer to the new Graph (if the file is valid), NULL if
		 *				invalid.
		 */
		static Resource * load( const std::string & fileName );
		
		/*!
		 *	@brief		Compute path
		 *
		 *	@param		agent		The agent for whom to compute the path.
		 *	@param		goal		The agent's goal.
		 *	@returns	A pointer to a RoadMapPath.  If there is an error,
		 *				the poitner will be NULL.
		 */
		RoadMapPath * getPath( const Agents::BaseAgent * agent, const BFSM::Goal * goal );

		/*!
		 *	@brief		Return the number of vertices in the graph.
		 *
		 *	@returns	The number of vertices in the graph.
		 */
		inline size_t getVertexCount() const { return _vCount; }

		/*!
		 *	@brief		Returns a const pointer to the ith vertex.
		 *
		 *	The validitiy of the index is only checked in debug mode with
		 *	an assertion.
		 *
		 *	@param		i		The index of the desired pointer.
		 *	@returns	A const pointer to the ith graph vertex.
		 */
		const GraphVertex * getVertex( size_t i ) const;

		/*!
		 *	@brief		The unique label for this data type to be used with 
		 *				resource management.
		 */
		static const std::string LABEL;

	protected:

		/*!
		 *	@brief		Find the closest visible graph vertex to the given
		 *				point.
		 *
		 *	@param		point		The point to connect to the graph.
		 *	@param		radius		The radius of the agent testing.
		 *	@returns	The index of the closest node.
		 */
		size_t getClosestVertex( const Vector2 & point, float radius );

		/*!
		 *	@brief		Computes the shortest path from start to end vertices.
		 *
		 *	This function instantiates a new path, but the caller is responsible
		 *	for deleting it.
		 *
		 *	@param		startID		The index of the start vertex.
		 *	@param		endID		The index of the end vertex.
		 *	@returns	A pointer to a new RoadMapPath.
		 */
		RoadMapPath * getPath( size_t startID, size_t endID );

		/*!
		 *	@brief		Compute's "h" for the A* algorithm.  H is the estimate of the
		 *				cost of a node to a goal point.  In this case, simply Euclidian
		 *				distance.
		 *
		 *	@param		v			The vertex to estimate the cost.
		 *	@param		goal		The goal point.
		 *	@returns	The h-value.
		 */
		inline float computeH( size_t v, const Vector2 & goal ) { return abs( _vertices[v].getPosition() - goal ); }

		/*!
		 *	@brief		The number of vertices.
		 */
		size_t _vCount;

		/*!
		 *	@brief		An array containing all vertices.
		 */
		GraphVertex *	_vertices;

		/*!
		 *	@brief		Initializes the heap memory based on current
		 *				graph state.
		 */
		 void initHeapMemory();

		/*!
		 *	@brief		The size of a block of data used for COST in
		 *				the A* algorithm  (3N, N = number of nodes)
		 */
		size_t DATA_SIZE;

		/*!
		 *	@brief		The size of a block of data used for STATE in
		 *				the A* algorithm  (2N, N = number of nodes)
		 */
		size_t STATE_SIZE;

		/*!
		 *	@brief		The full set of data to serve as the heap
		 *				There are N entries in a single heap
		 *				and one heap per thread.
		 */
		unsigned int * _HEAP;	

		/*!
		 *	@brief		The full set of data for reconstructing the path.
		 *				For any given entry i, the value at i is the index of
		 *				the node from which i was reached. There is one block
		 *				per thread.
		 */
		unsigned int * _PATH;

		/*!
		 *	@brief		The block of data for tracking the f, g, and h data for nodes.
		 *				(This is where DATA_SIZE = 3N comes from.)  One block for
		 *				each active thread.
		 *				The first N values in a block are the f values, the next N are the g values,
		 *				and the last set of N values are the h values.
		 */
		float * _DATA;	

		/*!
		 *	@brief		Block of data for reportin node state (if its in a heap or if its
		 *				no longer used for calculation.  First N booleans are "in heap", second
		 *				N are "finished".  One block of 2N booleans per thread.
		 */
		bool *	_STATE;
	};

	/*!
	 *	@brief		The definition of the managed pointer for Graph data
	 */
	typedef ResourcePtr< Graph > GraphPtr;

	/*!
	 *	@brief		        Loads the graph of the given name
	 *
	 *	@param	 fileName   The name of the file containing the graph definition.
	 *	@returns	        The GraphPtr containing the data.
	 *	@throws		        A  ResourceException if the data is unable to be instantiated.
	 */
	GraphPtr loadGraph( const std::string & fileName ) throw ( ResourceException );
}	// namespace Menge

#endif	// __GRAPH_H__