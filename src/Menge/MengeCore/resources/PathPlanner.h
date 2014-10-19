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
 *  @file       PathPlanner.h
 *  @brief      Defines an entity for computing paths through a navigation mesh.
 */

#ifndef __PATH_PLANNER_H__
#define	__PATH_PLANNER_H__

#include "mengeCommon.h"
#include "NavMesh.h"
#include "ReadersWriterLock.h"
#include <map>
#include <list>

namespace Menge {

	/*!
	 *	@brief		Exception class for path planner.
	 */
	class MENGE_API PathPlannerException : public virtual MengeException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		PathPlannerException() : MengeException() {}		

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		PathPlannerException( const std::string & s ): MengeException(s) {}
	};

	/*!
	 *	@brief		The fatal path planner exception.
	 */
	class MENGE_API PathPlannerFatalException : public PathPlannerException, public MengeFatalException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		PathPlannerFatalException() : MengeException(), PathPlannerException(), MengeFatalException() {}

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		PathPlannerFatalException( const std::string & s ): MengeException(s), PathPlannerException(), MengeFatalException() {}
	};

	// FORWARD DECLARATIONS
	class PortalRoute;
	class PathPlanner;

	/*!
	 *	@brief		Definition of the identifier of a Route.
	 */
	typedef size_t RouteKey;

	/*!
	 *	@brief		A list of PortalRoute pointers.
	 */
	typedef std::list< PortalRoute * > PRouteList;

	/*!
	 *	@brief		An iterator to a PRouteList.
	 */
	typedef PRouteList::iterator PRouteListItr;

	/*!
	 *	@brief		A const iterator to a PRouteList.
	 */
	typedef PRouteList::const_iterator PRouteListCItr;

	/*!
	 *	@brief		A mapping from RouteKey to PRouteList.
	 */
	typedef HASH_MAP< RouteKey, PRouteList > PRouteMap;

	/*!
	 *	@brief		An iterator to a PRouteMap.
	 */
	typedef PRouteMap::iterator PRouteMapItr;

	/*!
	 *	@brief		A const iterator to a PRouteMap.
	 */
	typedef PRouteMap::const_iterator PRouteMapCItr;

	/*!
	 *	@brief		Class for computing paths through a navigation mesh.
	 */
	class PathPlanner {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		ptr			A resource pointer to the nav mesh the
		 *							planner uses.
		 */
		PathPlanner( NavMeshPtr ptr );

		/*!
		 *	@brief		Destrutor.
		 */
		~PathPlanner();

		/*!
		 *	@brief		Returns a route between the two specified nodes.
		 *
		 *	@param		startID		The index of the navigation mesh node at
		 *							which the route starts.
		 *	@param		endID		The index of the navigation mesh node at
		 *							which the route ends.
		 *	@param		minWidth	The minimum passable width required for the
		 *							route.
		 *	@returns	A pointer to a PortalRoute from startID to endID with 
		 *				the required clearance.
		 */
		PortalRoute * getRoute( unsigned int startID, unsigned int endID, float minWidth );	

	protected:
		/*!
		 *	@brief		Computes a route (and adds it to the cache) between start
		 *				and end with the minimum clearance given.
		 *
		 *	@param		startID		The index of the navigation mesh node at
		 *							which the route starts.
		 *	@param		endID		The index of the navigation mesh node at
		 *							which the route ends.
		 *	@param		minWidth	The minimum passable width required for the
		 *							route.
		 *	@returns	A pointer to a PortalRoute from startID to endID with 
		 *				the required clearance.
		 */
		PortalRoute * computeRoute( unsigned int startID, unsigned int endID, float minWidth );

		/*!
		 *	@brief		Compute's "h" for the A* algorithm.  H is the estimate of the
		 *				cost of a node to a goal point.  In this case, simply Euclidian
		 *				distance.
		 *
		 *	@param		node		The estimated cost from the given node to the goal point.
		 *	@param		goal		The goal point.
		 *	@returns	The h-value.
		 */
		float computeH( unsigned int node, const Vector2 & goal );

		/*!
		 *	@brief		Cache the given route going from start to goal
		 *
		 *	Caching the route saves the solution for an agent from startID to endID with the
		 *	provided minimum width.  This route may be identical to a route that was found for
		 *	a larger agent.  If the previous agent was sufficiently large, a recomputation was
		 *	triggered in case there was a better path.  However, it may be that this new path
		 *	is the same as the old path.  In this case, the two routes are merged and a pointer
		 *	to the merged route is returned.  If this route is uniquely superior, then pointer
		 *	provided as input will be returned.
		 *
		 *	@param		startID		The index of the start node.
		 *	@param		endID		The index of the end node.
		 *	@param		route		The route between them.
		 *	@returns	The equivalent route.
		 */
		PortalRoute * cacheRoute( unsigned int startID, unsigned int endID, PortalRoute * route );

		/*!
		 *	@brief		A mapping from RouteKeys (a size_t) to to a list of routes.
		 *				The list consists of routes between the points in the
		 *				route key in INCREASING maximum width.  (i.e. narrowest
		 *				route to widest route.)
		 */
		PRouteMap _routes;

		/*!
		 *	@brief		Lock for securing _routes;
		 */
		ReadersWriterLock _routeLock;

		/*!
		 *	@brief		The navigation mesh for planning on.
		 */
		NavMeshPtr	_navMesh;

		/*!
		 *	@brief		Initializes the heap memory
		 *
		 *	@param		nodeCount		The number of nodes for which to initialize memory.
		 */
		 void initHeapMemory( size_t nodeCount );

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
}	// namespace Menge

#endif	 // __PATH_PLANNER_H__
