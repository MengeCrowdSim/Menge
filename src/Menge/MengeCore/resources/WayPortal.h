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
 *  @file       WayPortal.h
 *  @brief      Defines a way portal for a navigation mesh route/path.
 */

#ifndef __WAY_PORTAL_H__
#define __WAY_PORTAL_H__

#include "NavMeshEdge.h"

namespace Menge {

	// FORWARD DECLARATIONS
	class PortalRoute;
	class PortalPath;
	namespace Agents {
		class PrefVelocity;
	}

	/*!
	 *	@brief		The basic structure defining a way "point" along a path
	 *				through a navigation mesh.  It consists of the navigation mesh
	 *				edge to pass through and the node one should be in while heading
	 *				to the edge.
	 */
	class WayPortal {
	public:
		/*!
		 *	@brief		Constructor
		 *
		 *	@param		edge		The NavigationMeshEdge which serves as the
		 *							immediate goal for this WayPortal.
		 *	@param		nodeID		The nav mesh node that an agent should be in
		 *							while heading for this node.
		 *	@param		p0IsLeft	Indicates if the p0 value of the edge is on the
		 *							left, when faced from inside the node.
		 */	
		WayPortal( const NavMeshEdge * edge, unsigned int nodeID, bool p0IsLeft );

		/*!
		 *	@brief		Retrives the left physical vertex of the portal.
		 *
		 *	@returns	The position of the left physical vertex, as viewed 
		 *				from inside _nodeID.
		 */
		inline Vector2 getLeft() const { return _p0IsLeft ? _edge->getP0() : _edge->getP1(); }

		/*!
		 *	@brief		Retrives the position in the portal offset from the left physical vertex.
		 *
		 *	@param		offset		The distance offset from the left physical vertex.
		 *	@returns	The position offset from the left physical vertex, as viewed 
		 *				from inside _nodeID.
		 */
		inline Vector2 getLeft( float offset ) const { return _p0IsLeft ? _edge->getP0( offset ) : _edge->getP1( offset ); }

		/*!
		 *	@brief		Retrives the right physical vertex of the portal.
		 *
		 *	@returns	The position of the right physical vertex, as viewed 
		 *				from inside _nodeID.
		 */
		inline Vector2 getRight() const { return _p0IsLeft ? _edge->getP1() : _edge->getP0(); }

		/*!
		 *	@brief		Retrives the position in the portal offset from the right physical vertex.
		 *
		 *	@param		offset		The distance offset from the right physical vertex.
		 *	@returns	The position offset from the right physical vertex, as viewed 
		 *				from inside _nodeID.
		 */
		inline Vector2 getRight( float offset ) const { return _p0IsLeft ? _edge->getP1( offset ) : _edge->getP0( offset ); }

		/*!
		 *	@brief		Computes the intersection point of the given line with this portal.
		 *
		 *	This algorithm ASSUMES that the line DOES intersect the portal.  The behavior is undefined
		 *	if they don't intersect.
		 *
		 *	@param		point			A point on the line to test.
		 *	@param		dir				A direction vector defining the position of the line.
		 *	@returns	A point, on the portal, at which the line intersects.
		 */
		Vector2 intersectionPoint( const Vector2 & point, const Vector2 & dir ) const;

		/*!
		 *	@brief		Sets the directions on the preferred velocity based on the wayportal.
		 *
		 *	@param		pos			The position of the agent.
		 *	@param		radius		The radius of the agent.
		 *	@param		dir			Preferred direction of the agent.  Direction is NOT assumed to
		 *							be of unit-length.
		 *	@param		pVel		The preferred velocity 
		 */
		void setPreferredDirection( const Vector2 & pos, float radius, const Vector2 & dir, Agents::PrefVelocity & pVel ) const;

		friend class PortalRoute;
		friend class PortalPath;

	protected:
		/*!
		 *	@brief		The edge which serves as goal.
		 */
		const NavMeshEdge *	_edge;

		/*!
		 *	@brief		The index of the node in which the agent is traveling toward
		 *				the edge.  It should be true that the specified node is
		 *				attached/connected to the given edge.
		 */
		unsigned int	_nodeID;

		/*!
		 *	@brief		Reports if the edge's P0 is the left vertex.
		 *				Left is defined based on moving across the portal from
		 *				the node indicated by _nodeID;
		 */
		bool	_p0IsLeft;
	};

}	// namespace Menge
#endif	// __WAY_PORTAL_H__