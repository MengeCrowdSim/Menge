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
 *	@file		GraphVertex.h
 *	@brief		The definition of a graph vertex for performing graph searches
 *				and path planning.
 */

#ifndef __GRAPH_VERTEX_H__
#define	__GRAPH_VERTEX_H__

#include "mengeCommon.h"
#include "GraphEdge.h"

namespace Menge {

	/*!
	 *	@brief		A graph vertex.
	 */
	class GraphVertex {
	public:
		/*!
		 *	@brief		Constructor
		 */
		GraphVertex();

		/*!
		 *	@brief		Destructor
		 */
		~GraphVertex();

		/*!
		 *	@brief		Assignment operator
		 */
		GraphVertex & operator=( const GraphVertex & n );

		/*!
		 *	@brief		Computes the distance between this vertex and the
		 *				given vertex.
		 *
		 *	@param		other		The other vertex.
		 *	@returns	The Euclidian distance between this vertex and the
		 *				other vertex.
		 */
		float getDistance( const GraphVertex & other ) { return abs( _pos - other._pos ); }

		/*!
		 *	@brief		Reports the distance to the ith neighbor.
		 *
		 *	@param		i		The index of the desired neighbor.
		 *	@returns	The distance to the ith neghbor.
		 */
		float getDistance( size_t i ) const;

		/*!
		 *	@brief		Sets the node identifier.
		 *
		 *	@param		id		The identifier to set this node to.
		 */
		void setID( size_t id ) { _id = id; }

		/*!
		 *	@brief		Retrive the identifier for this node.
		 */
		size_t getID() const { return _id; }

		/*!
		 *	@brief		Sets the graph vertex's position.
		 *
		 *	@param		p		The point value to set.
		 */
		inline void setPosition( const Vector2 & p ) { _pos.set( p ); }

		/*!
		 *	@brief		Reports the position of the vertex.
		 *
		 *	@returns	The position of the vertex.
		 */
		Vector2 getPosition() const { return _pos; }

		/*!
		 *	@brief		Sets the ith edge for the vertex.
		 *
		 *	@param		edge		The edge to set
		 *	@param		i			The index to set.
		 *	@returns	True if the operation is valid, false otherwise.
		 */
		bool setEdge( const GraphEdge & edge, size_t i );

		/*!
		 *	@brief		Reports the number of vertices adjacent to this vertex.
		 */
		size_t getNeighborCount() const { return _edgeCount; }

		/*!
		 *	@brief		Returns a pointer to the ith neighbor.
		 *
		 *	@param		i		The index of the neighboring vertex to retrieve.
		 *						The index is defined in the range [0, N-1], where
		 *						this vertex has N neighbors.
		 *	@returns	A pointer to the ith neighboring vertex.
		 */
		const GraphVertex * getNeighbor( size_t i ) const;

		/*!
		 *	@brief		Sets the degree -- the number of neighbors this vertex
		 *				has
		 *
		 *	@param		degree		The number of neighbors this vertex has.
		 */
		void setDegree( size_t degree );

		/*!
		 *	@brief		Reports the number of edges on the node.
		 *
		 *	@returns	The number of edges.
		 */
		size_t getEdgeCount() const { return _edgeCount; }

		/*!
		 *	@brief		Retrieves the ith edge connected to this node.
		 *
		 *	@param		i		The local index of the desired edge.
		 *	@returns	A pointer to the ith edge.
		 */
		GraphEdge & getEdge( size_t i ) { return _edges[ i ]; }

		/*!
		 *	@brief		Retrieves a const pointer to the ith edge connected to this node.
		 *
		 *	@param		i		The local index of the desired edge.
		 *	@returns	A pointer to the ith edge.
		 */
		const GraphEdge & getEdge( size_t i ) const { return _edges[ i ]; }

	protected:

		/*!
		 *	@brief		An array of edges connecting to other vertices
		 */
		GraphEdge *		_edges;

		/*!
		 *	@brief		The number of edges connecting to this vertex.
		 */
		size_t	_edgeCount;

		/*!
		 *	@brief		The position of this vertex.
		 */
		Vector2	_pos;

		/*!
		 *	@brief		The identifier for this vertex.
		 */
		size_t	_id;
	};

}	 // namespace Menge
#endif		// __GRAPH_VERTEX_H__

	