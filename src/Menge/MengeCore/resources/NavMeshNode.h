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
 *  @file       NavMeshNode.h
 *  @brief      Defines the "node" of the adjacency graph in a navigation mesh.
 *				The node corresponds to a polygon in the mesh.
 */

#ifndef __NAV_MESH_NODE_H__
#define __NAV_MESH_NODE_H__

#include <fstream>
#include "fsmCommon.h"
#include "NavMeshPoly.h"

namespace Menge {

	// Forward declarations
	class NavMesh;
	class NavMeshEdge;
	class PathPlanner;
	class NavMeshObstacle;

	/*!
	 *	@brief		The navigation mesh adjacency graph node.  It corresponds to
	 *				a convex polygon in the navigation mesh.
	 */
	class NavMeshNode {
	public:
		/*!
		 *	@brief		Constructor
		 */
		NavMeshNode();

		/*!
		 *	@brief		Destructor
		 */
		~NavMeshNode();

		/*!
		 *	@brief		Assignment operator
		 */
		NavMeshNode & operator=( const NavMeshNode & n );

		/*!
		 *	@brief		Sets the node identifier.
		 *
		 *	@param		id		The identifier to set this node to.
		 */
		void setID( unsigned int id ) { _id = id; }

		/*!
		 *	@brief		Retrive the identifier for this node.
		 */
		unsigned int getID() const { return _id; }

		/*!
		 *	@brief		Sets the node's center value.
		 *
		 *	@param		c		The point value to set.
		 */
		inline void setCenter( const Vector2 & c ) { _center.set( c ); }

		/*!
		 *	@brief		Reports the center (centroid) of the node's polygon.
		 *
		 *	@returns	The position of the polygon's centroid.
		 */
		Vector2 getCenter() const { return _center; }

		/*!
		 *	@brief		Reports the center (centroid) of the node's polygon.
		 *
		 *	@returns	The position of the polygon's centroid.
		 */
		Vector3 getCenter3D() const { return Vector3( _center.x(), _poly.getElevation( _center ), _center.y() ); }

		/*!
		 *	@brief		Returns the number of vertices in the node's polygon.
		 *
		 *	@returns	The number of vertices in the polygon.
		 */
		size_t getVertexCount() const { return _poly._vertCount; }

		/*!
		 *	@brief		Sets this node's polygon to the given vertex array.
		 *				The polygon can then evaluate its indices with respect to
		 *				this array of vertex values.
		 *
		 *	@param		vertices	An array of Vector2's defining the
		 *							polygon vertices.
		 */
		void setVertices( const Vector2 * vertices ) { _poly._vertices = vertices; }

		/*!
		 *	@brief		Gets the index of the ith vertex in this node's polygon
		 *
		 *	@param		i		The local index of the vertex in the node's polygon.
		 *	@returns	The global index in the navigation mesh of the ith vertex
		 *				in this node's polygon.
		 */
		unsigned int getVertexID( size_t i ) const { return _poly._vertIDs[ i ]; }

		/*!
		 *	@brief		Returns the number of obstacles connected to this node.
		 *
		 *	@returns	The number of obtacles in on this node.
		 */
		size_t getObstacleCount() const { return _obstCount; }

		/*!
		 *	@brief		Returns a const pointer to the ith obstacle in the node.
		 *
		 *	@param		i		The local index of the desired obstacle.
		 *	@returns	A const pointer to the ith obstacle.
		 */
		const NavMeshObstacle * getObstacle( size_t i ) const { return _obstacles[ i ]; }

		/*!
		 *	@brief		Returns a const pointer to the ith obstacle in the node.
		 *
		 *	@param		i		The local index of the desired obstacle.
		 *	@returns	A pointer to the ith obstacle.
		 */
		NavMeshObstacle * getObstacle( size_t i ) { return _obstacles[ i ]; }

		/*!
		 *	@brief		Reports the number of nodes adjacent to this node.
		 */
		size_t getNeighborCount() const { return _edgeCount; }

		/*!
		 *	@brief		Returns a pointer to the ith neighbor.
		 *
		 *	@param		i		The index of the neighboring node to retrieve.
		 *						The index is defined in the range [0, N-1], where
		 *						this node has N neighbors.
		 *	@returns	A pointer to the ith neighboring node.
		 */
		const NavMeshNode * getNeighbor( size_t i ) const;

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
		NavMeshEdge * getEdge( size_t i ) { return _edges[ i ]; }

		/*!
		 *	@brief		Retrieves a const pointer to the ith edge connected to this node.
		 *
		 *	@param		i		The local index of the desired edge.
		 *	@returns	A pointer to the ith edge.
		 */
		const NavMeshEdge * getEdge( size_t i ) const { return _edges[ i ]; }

		/*!
		 *	@brief		Returns the pointer to the edge connecting this node
		 *				with the node whose identifier is given.
		 *	
		 *	@param		nodeID		The id of the desired adjacent node.
		 *	@returns	The pointer to the edge connecting the two nodes.
		 *				If the nodes are not connected, NULL is returned.
		 */
		NavMeshEdge * getConnection( unsigned nodeID );

		/*! 
		 *	@brief		Reports if the given point is inside the polygon
		 *
		 *	@param		point		The point to test.
		 *	@returns	A boolean reporting if the point lies inside the polygon 
		 *				(true) or outside (false ).
		 *				// TODO: What about the boundary?
		 */
		bool containsPoint( const Vector2 & point ) const { return _poly.containsPoint( point ); }

		/*!
		 *	@brief		Sets the node properties from a node definition
		 *				in the given ascii file stream.
		 *	
		 *	@param		f		The input file stream.
		 *	@returns	A boolean indicating successful parsing (true) or failure
		 *				(false).
		 */
		bool loadFromAscii( std::ifstream & f );

		/*!
		 *	@brief		Computes the height based on this node's polygon
		 */
		inline float getElevation( const Vector2 & p ) const { return _poly.getElevation( p ); }

		/*!
		 *	@brief		Computes the gradient based on this node's polygon
		 */
		inline Vector2 getGradient() const { return _poly.getGradient(); }

		friend class NavMesh;
		friend class NavMeshEdge;
		friend class PathPlanner;

	protected:
		/*!
		 *	@brief		An array of edges connecting to other nodes
		 */
		NavMeshEdge **	_edges;

		/*!
		 *	@brief		The number of edges connecting to this node.
		 */
		size_t	_edgeCount;

		/*!
		 *	@brief		An array of obstacles connected to this node.
		 */
		NavMeshObstacle ** _obstacles;

		/*!
		 *	@brief		The number of obstacles connecting to this node.
		 */
		size_t _obstCount;

		/*!
		 *	@brief		The "position" of the node - used to compute distance 
		 *				between node and goal (A* heuristic)
		 */
		Vector2		_center;

		/*!
		 *	@brief		The polygon associated with this node
		 */
		NavMeshPoly		_poly;

		/*!
		 *	@brief		The identifier of this node
		 */
		unsigned int	_id;
	};
}	// namespace Menge

#endif	// __NAV_MESH_NODE_H__
