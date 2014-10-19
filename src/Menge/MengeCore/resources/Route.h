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
 *  @file       Route.h
 *  @brief      The definition of a basic route through a navigation mesh
 */

#ifndef __ROUTE_H__
#define	__ROUTE_H__

#include "WayPortal.h"
#include <vector>

namespace Menge {

	// FORWARD DECLARATIONS
	class PathPlanner;
	class NavMeshEdge;

	/*!
	 *	@brief		The definition of a route through a navigation mesh from a 
	 *				start to an end node.
	 */
	class PortalRoute {
	public:
		/*!
		 *	@brief		Constructor
		 *
		 *	@param		start		The index of the start node of the route.
		 *	@param		end			The index of the end node of the rotue.
		 */
		PortalRoute( unsigned int start, unsigned int end );

		/*!
		 *	@brief		Destructor.
		 */
		~PortalRoute();

		/*!
		 *	@brief		Returns the identifier for the first node on the route.
		 *
		 *	@returns	The first node's index.
		 */
		inline unsigned int getStartNode() const { return _startNode; }

		/*!
		 *	@brief		Returns the identifier for the destination node on the route.
		 *
		 *	@returns	The destination node's index.
		 */
		inline unsigned int getEndNode() const { return _endNode; }

		/*!
		 *	@brief		Reports the number of portals on the route.
		 *
		 *	@returns	The number of portals on the route.
		 */
		inline size_t getPortalCount() const { return _portals.size(); }

		/*!
		 *	@brief		Reports the node for the ith portal.
		 *
		 *	@param		i		The index of the portal.
		 *	@returns	The index of the node the agent should be in when moving
		 *				toward the ith portal.
		 */
		inline unsigned int getPortalNode( size_t i ) const { return _portals[ i ]._nodeID; }

		/*!
		 *	@brief		Gets the ith portal.
		 *
		 *	@param		i		The path-local index of the portal.
		 *	@returns	A pointer to the ith portal.
		 */
		WayPortal * getPortal( size_t i ) { return &_portals[i]; }

		/*!
		 *	@brief		Gets a const version of the ith portal.
		 *
		 *	@param		i		The path-local index of the portal.
		 *	@returns	A pointer to the ith portal.
		 */
		const WayPortal * getPortal( size_t i ) const { return &_portals[i]; }

		/*!
		 *	@brief		Appends a way portal to the path.
		 *
		 *	@param		edge		The edge forming the portal
		 *	@param		node		The node in which the agent should be traveling
		 *							toward the edge
		 */
		void appendWayPortal( const NavMeshEdge * edge, unsigned int node );

		/*!
		 *	@brief		Reports if the two routes have the same envelope.
		 *
		 *	Two routes are equivalent if they have the same sequence of nav mesh nodes.
		 *	It is assumed that they have the same start and end nodes.
		 *
		 *	@param		route		The route to compare with this.
		 *	@returns	True if the two routes are equivalent.  False otherwise.
		 */
		bool isEquivalent( const PortalRoute * route );

		/*!
		 *	@brief		Report the length of the path.
		 */
		float getLength() const { return _length; }

		friend class PathPlanner;

	protected:

		/*!
		 *	@brief		The start node.
		 */
		unsigned int	_startNode;

		/*!
		 *	@brief		The end node.
		 */
		unsigned int	_endNode;

		/*!
		 *	@brief		The maximum width an agent can be to take this route
		 */
		float _maxWidth;

		/*!
		 *	@brief		The smallest agent width for which this has proven to
		 *				be the optimal path.
		 */
		float _bestSmallest;

		/*!
		 *	@brief		The length of the route
		 */
		float _length;

		/*!
		 *	@brief		The list of portals to pass through along the route
		 */
		std::vector< WayPortal >	_portals;
	};
}	// namespace Menge

#endif	// __ROUTE_H__

