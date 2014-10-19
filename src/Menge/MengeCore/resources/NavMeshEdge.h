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
 *  @file       NavMeshEdge.h
 *  @brief      Defines the "edge" of the adjacency graph in a navigation mesh.
 *
 *				The connectivity edge is uniquely associated with a "portal",
 *				the shared edge between two adjacent polygons.
 */

#ifndef __NAV_MESH_EDGE_H__
#define __NAV_MESH_EDGE_H__

#include <fstream>
#include "mengeCommon.h"

	namespace Menge {

	// Forward declarations
	class NavMesh;
	class NavMeshNode;
	namespace Agents {
		class PrefVelocity;
	}

	/*!
	 *	@brief		The navigation mesh adjacency graph edge.  
	 *
	 *	It is comprised of
	 *	two parts: the portal geometry and the logical graph connectivity.
	 *	A NavMeshEdge exists because two navigation mesh polygons share
	 *	a common edge.  The portal geometry is the edge.  The graph
	 *	connectivity indicates that the two mesh polygons (nodes) are
	 *	connected in the logical graph.
	 */
	class NavMeshEdge {
	public:
		/*!
		 *	@brief		Constructor
		 */
		NavMeshEdge();

		/*!
		 *	@brief		Destructor
		 */
		~NavMeshEdge();

		/*!
		 *	@brief		Returns the first edge point.
		 *
		 *	@returns	The 2D value of the first edge end point
		 */
		inline Vector2 getP0() const { return _point; }

		/*!
		 *	@brief		Returns a point inset from the first end point.
		 *
		 *	@param		dist		The inset distance.
		 *	@returns	The 2D value of the point inset dist from the first edge end point
		 */
		inline Vector2 getP0( float dist ) const { return _point + _dir * dist; }

		/*!
		 *	@brief		Returns the second edge point.
		 *
		 *	@returns	The 2D value of the second edge end point
		 */
		inline Vector2 getP1() const { return _point + _dir * _width; }

		/*!
		 *	@brief		Returns a point inset from the second end point.
		 *
		 *	@param		dist		The inset distance.
		 *	@returns	The 2D value of the point inset dist from the second edge end point
		 */
		inline Vector2 getP1( float dist ) const { return _point + _dir * (_width - dist); }

		/*!
		 *	@brief		Returns the direction of the edge.
		 *
		 *	@returns	The edge's direction.
		 */
		inline Vector2 getDirection() const { return _dir; }

		/*!
		 *	@brief		Returns the first attached NavMeshNode
		 *
		 *	@returns	A pointer to the first navigation mesh node.
		 */
		NavMeshNode * getFirstNode() const { return _node0; }

		/*!
		 *	@brief		Returns a pointer to the node on the opposite end
		 *				of the edge from the given node (by id).
		 *
		 *	If the given node is not actually adjacent to the edge, then the
		 *	first adjacent node is returned.
		 *
		 *	@param		id		The identifier of the operand node.
		 *	@returns	A pointer to the node on the other side of the edge.
		 */
		NavMeshNode * getOtherByID( unsigned int id ) const;

		/*!
		 *	@brief		Returns a pointer to the node on the opposite end
		 *				of the edge from the given node (by pointer).
		 *
		 *	If the given node is not actually adjacent to the edge, then the
		 *	first adjacent node is returned.
		 *
		 *	@param		node		A pointer to the operand node.
		 *	@returns	A pointer to the node on the other side of the edge.
		 */
		NavMeshNode * getOtherByPtr( const NavMeshNode * node );

		/*!
		 *	@brief		Returns a const pointer to the node on the opposite end
		 *				of the edge from the given node (by pointer).
		 *
		 *	If the given node is not actually adjacent to the edge, then the
		 *	first adjacent node is returned.
		 *
		 *	@param		node		A pointer to the operand node.
		 *	@returns	A const pointer to the node on the other side of the edge.
		 */
		const NavMeshNode * getOtherByPtr( const NavMeshNode * node ) const;



		//////////////////////////////////////////////////////////////////////////
		//					Getters/setters
		//////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Sets the edge's point value.
		 *
		 *	@param		p		The point value to set.
		 */
		inline void setPoint( const Vector2 & p ) { _point.set( p ); }

		/*!
		 *	@brief		Sets the edge's direction value.
		 *
		 *	@param		d		The direction value to set.  It is assumed to be
		 *						normalized and is not tested.
		 */
		inline void setDirection( const Vector2 & d ) { _dir.set( d ); }

		/*!
		 *	@brief		Sets the edge's width value.
		 *
		 *	@param		w		The width value to set.  
		 */
		inline void setWidth( float w ) { _width = w; }

		/*!
		 *	@brief		Reports the width of the edge.
		 *
		 *	@returns	The width of the edge.
		 */
		inline float getWidth() const { return _width; }

		/*!
		 *	@brief		Sets the connected node pointers.
		 *
		 *	@param		n0		The pointer to the first connected node.
		 *	@param		n1		The pointer to the second connected node.
		 */
		inline void setNodes( NavMeshNode * n0, NavMeshNode * n1 ) { _node0 = n0; _node1 = n1; }

		/////////////////////////////////////////////////////////////////////////////
		//						Geometric queries
		/////////////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Selects a point along the edge
		 *
		 *	@param		t		The parameter from _point in the edge direction.
		 *	@returns	The point which is: _point + t * _dir;
		 */
		inline Vector2 getPoint( float t ) const { return _point + t * _dir; }

		/*!
		 *	@brief		Reports if the point q = _point + param * _dir is
		 *				clear for an agent with the given radius positioned at pos.
		 *				"Clear" means a straight-line path will not cause it to intersect
		 *				the ends of the edge.
		 *
		 *	@param		pos			The position of the agent.
		 *	@param		radius		The radius of the agent.
		 *	@param		param		The parameter, along the edge's direction, of the 
		 *							desired goal point.
		 *	@returns	A boolean reporting if the straight line path is clear (true) or
		 *				occlued (false).
		 */
		bool pointClear( const Vector2 & pos, float radius, float param ) const;

		/*!
		 *	@brief		Computes a target point with respect to this edge.  The target point
		 *				is a point along the line of the edge to which the agent with the
		 *				given radius can head straight towards and be guaranteed NOT to
		 *				intersect with the end points of the edge.  It also leads the agent
		 *				to get closer to crossing the edge.
		 *
		 *	@param		pos			The position from which to compute nearest point.
		 *	@param		radius		The radius of the agent -- required clearance.
		 *	@returns	A point which represents the goal point the agent should walk
		 *				towards to clear the obstacles and pass through the edge.
		 */
		Vector2 targetPoint( const Vector2 & pos, float radius ) const;

		/*!
		 *	@brief		Computes the collision-free velocity towards the portal based on the
		 *				agent radius and the preferred direction.  If the preferred direction
		 *				is collision free, it is returned.  Otherwise, the "best" collision-free
		 *				approximation of that direction is returned.
		 *
		 *	@param		pos			The position of the agent.
		 *	@param		radius		The radius of the agent.
		 *	@param		dir			Preferred direction of the agent.  Direction is NOT assumed to
		 *							be of unit-length.
		 *	@returns	The collision-free direction that best approximates the preferred
		 *				direction (may be the preferred direction).
		 */
		Vector2	getClearDirection( const Vector2 & pos, float radius, const Vector2 & dir ) const;

		/*!
		 *	@brief		Sets the directions of a preferred velocity that passes through the
		 *				edge biased towards the indicated direction.
		 *
		 *	If approaching the portal obliquely, the preferred direction may not provide
		 *	sufficient clearance.  In this case, the direction is changed so the agent makes
		 *	progress toward the portal without colliding with the portal limits.
		 *
		 *	At the same time, sets the limits of the preferred velocity based on the
		 *	actual clearance.
		 *
		 *	@param		pos			The position of the agent.
		 *	@param		radius		The radius of the agent.
		 *	@param		dir			Preferred direction of the agent.  Direction is NOT assumed to
		 *							be of unit-length.
		 *	@param		pVel		The preferred velocity whose directions are set.
		 */
		void setClearDirections( const Vector2 & pos, float radius, const Vector2 & dir, Agents::PrefVelocity & pVel ) const;

		/*!
		 *	@brief		Reports the squared distance to the edge from the given point.
		 *
		 *	@param		pt			The point to compute from.
		 *	@returns	The squared distance between p and the nearest point on the edge.
		 */
		float getSqDist( const Vector2 & pt ) const;

		/*!
		 *	@brief		Reports the squared distance to the edge from the given point
		 *				and gives the nearest point.
		 *
		 *	@param		pt			The point to compute from.
		 *	@param		nearPt		The nearest point to pt on the edge.  The values of
		 *							this vector will be changed by the function
		 *	@returns	The squared distance between p and the nearest point on the edge.
		 */
		float getSqDist( const Vector2 & pt, Vector2 & nearPt ) const;

		/*!
		 *	@brief		Reports the distance to the edge from the given point.
		 *
		 *	@param		pt		The point to compute from.
		 *	@returns	The squared distance between p and the nearest point on the edge.
		 */
		float getDist( const Vector2 & pt ) const { return sqr( getSqDist( pt ) ); }

		/*!
		 *	@brief		Computes the width-dependent distance between the
		 *				two nodes connected by this edge.  
		 *				If the edge width is narrower than the given minimum width
		 *				the distance is "infinite" (indicated by -1).  Otherwise
		 *				it is the distance between node centers.
		 *
		 *	@param		minWidth		The minimum required width.
		 *	@returns	The passable distance between the two nodes.
		 */
		float getNodeDistance( float minWidth );

		/*!
		 *	@brief		Return the Euclidian distance between the two
		 *				nodes this edge connects.
		 *
		 *	@returns	The distance between the two connected node's centers.
		 */
		inline float getNodeDistance() const { return _distance; }

		/*!
		 *	@brief		Sets the edge properties from an edge definition
		 *				in the given ascii file stream.
		 *	
		 *	@param		f			The input file stream.
		 *	@param		vertices	The array of vertices into which the definition
		 *							indexes.
		 *	@returns	A boolean indicating successful parsing (true) or failure
		 *				(false).
		 */
		bool loadFromAscii( std::ifstream & f, Vector2 * vertices );

		/*!
		 *	@brief		Reports if _point in this edge is on the left for
		 *				the node with the given id.  
		 *
		 *	@param		id		The id of the node to test.  (The id is assumed
		 *						to be connected.  It is only checked in debug mode.)
		 *	@returns	A boolean reporting if the point in the edge is on the left
		 *				when facing the edge from the node with the given id.
		 */
		bool pointOnLeft( unsigned int id ) const;

		/*!
		 *	@brief		Reports if _point in this edge is on the left 
		 *				when looked at from within the given node.  
		 *
		 *	@param		node		The pointer to the node to test.  (The id is assumed
		 *						to be connected.  It is only checked in debug mode.)
		 *	@returns	A boolean reporting if the point in the edge is on the left
		 *				when facing the edge from the node with the given id.
		 */
		bool pointOnLeft( const NavMeshNode * node ) const;

		friend class NavMesh;

	protected:
		// Geometry of the edge's portal
		/*!
		 *	@brief		The point that defines the portal geometry.  The portal
		 *				is defined as p(t) = _point + t * _dir, t in the range [0, _width]
		 */
		Vector2	_point;

		/*!
		 *	@brief		The unit-length direction of the portal.  See _point
		 *				for how the portal is defined w.r.t. this field member.
		 */
		Vector2 _dir;

		/*!
		 *	@brief		The width of the portal.
		 */
		float	_width;

		// Logical connectivity data
		/*!
		 *	@brief		The "distance" between the two nodes connected by this edge.
		 *				Used to estimate the cost of a path (the length of the path).
		 */
		float	_distance;

		// TODO: Does lower-indexed/upper-indexed really matter?
		/*!
		 *	@brief		A pointer to the first nav mesh node connected by
		 *				this edge.  When standing in this node, the edge's _point
		 *				is on the left.
		 */
		NavMeshNode *	_node0;

		/*!
		 *	@brief		A pointer to the second nav mesh node connected by
		 *				this edge.  When standing in this node, the edge's _point
		 *				is on the right.
		 */
		NavMeshNode *	_node1;
	};
}	// namespace Menge

#endif	 // __NAV_MESH_EDGE_H__
