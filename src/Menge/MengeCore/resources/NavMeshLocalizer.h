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
 *  @file       NavMeshLocalizer.h
 *  @brief      Data structure responsible for knowing where on the navigation mesh
 *				each agent is located.
 */

#ifndef __NAV_MESH_LOCALIZER_H__
#define	__NAV_MESH_LOCALIZER_H__

#include "NavMesh.h"
#include "NavMeshNode.h"
#include "mengeCommon.h"
#include "Resource.h"
#include "ReadersWriterLock.h"
#include <map>
#include <set>

namespace Menge {

	namespace Agents {
		class BaseAgent;
	}

	// Forward declaration
	class PortalPath;
	class PathPlanner;

	/*!
	 *	@brief		Class for indicating how the location of the agent is defined.
	 *				Either by a portal path or a node. 
	 */
	class NavMeshLocation {
	public:
		/*!
		 *	@brief		Default constructor
		 */
		NavMeshLocation():_nodeID(NO_NODE),_hasPath(false){}

		/*!
		 *	@brief		Constructor
		 *				Initializes the location to being a node id.
		 *
		 *	@param		nodeID		The identifier of a nav mesh node.
		 */
		NavMeshLocation( unsigned int nodeID ):_nodeID(nodeID), _hasPath(false){}

		/*!
		 *	@brief		Constructor
		 *				Initializes the location to being a path.
		 *				The NavMeshLocation takes responsibility for deleting the path.
		 *
		 *	@param		path		A pointer to a path.  This class takes responsibility
		 *							for freeing the memory.
		 */
		NavMeshLocation( PortalPath * path ):_path(path), _hasPath(true) {}

		/*!
		 *	@brief		Sets the current position to being a node.
		 *
		 *	@param		nodeID		The identifier of the navigation mesh node
		 *							for the location. If the location was previously
		 *							a path, the path is deleted.
		 */
		void setNode( unsigned int nodeID );

		/*!
		 *	@brief		Clears the path (if any), maintaining the node location.
		 */
		void clearPath();

		/*!
		 *	@brief		Reports the node the agent is currently in.
		 *
		 *	@returns	The index of the node associated with this location.
		 *				If the location is not on a node, PortalPath::NO_NODE is returned.
		 */
		unsigned int getNode() const;

		/*!
		 *	@brief		Sets the current position to being the given path.
		 *
		 *	@param		path		The path for defining the current location.
		 *							The locator takes responsibility for deleting the
		 *							path.
		 */
		void setPath( PortalPath * path );

		/*!
		 *	@brief		Reports if the location is a path.
		 *
		 *	@returns	True if the location is a path, false if a node.
		 */
		inline bool isPath() const { return _hasPath; }

		/*!
		 *	@brief		Reports if the location is a node.
		 *
		 *	@returns	True if the location is a node, false if a path.
		 */
		inline bool isNode() const { return !_hasPath; }

		/*!
		 *	@brief		Location information.  If the agent has a path it is a 
		 *				pointer to that path.  Otherwise, it is an index into
		 *				the navigation mesh.
		 */
		union {
			size_t _nodeID;
			PortalPath * _path;
		};

		/*!
		 *	@brief		Determines interpretation of the location union.  If true,
		 *				_path contains a pointer.  Otherwise, _nodeID is an index
		 *				in the navigation mesh.
		 */
		bool	_hasPath;

		/*!
		 *	@brief		Signal for indicating that the position is NOT on the
		 *				navigation mesh.
		 */
		const static unsigned int NO_NODE;
	};

	/////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		A collection of agent ids.  
	 *				It represents the population of each nav mesh node.
	 */
	typedef std::set< size_t > OccupantSet;

	/*!
	 *	@brief		Iterator for an OccupantSet.
	 */
	typedef OccupantSet::iterator OccupantSetItr;

	/*!
	 *	@brief		Const iterator for an OccupantSet.
	 */
	typedef OccupantSet::const_iterator OccupantSetCItr;

	/*!
	 *	@brief		Class responsible for tracking agent relatinoships to the 
	 *				navigation mesh: its current location and its path.
	 */
	class MENGE_API NavMeshLocalizer : public Resource {
	public:
		/*!
		 *	@brief		Constructor
		 *
		 *	@param		name		The name of the underlying navigation mesh.
		 */
		NavMeshLocalizer( const std::string & name );

	protected:
		/*!
		 *	@brief		Destructor
		 */
		~NavMeshLocalizer();

	public:


		/*!
		 *	@brief		Returns a unique resource label to be used to identify
		 *				different resource *types* which use the same underlying
		 *				file data.
		 */
		virtual const std::string & getLabel() const { return LABEL; }

		/*!
		 *	@brief		Reports the node the agent is currently in.
		 *
		 *	@param		agent		The agent whose position relative to the nav 
		 *							mesh is returned.
		 *	@returns	The index of the node associated with this location.
		 *				If the location is not on a node, NavMeshLocation::NO_NODE is returned.
		 */
		unsigned int getNode( const Agents::BaseAgent * agent ) const;

		/*!
		 *	@brief		Reports the node with the highest elevation for the given point.
		 *
		 *	@param		p		The point to test against the mesh.
		 *	@returns	The index of the node associated with this location.
		 *				If the location is not on a node, NavMeshLocation::NO_NODE is returned.
		 */
		unsigned int getNode( const Vector2 & p ) const;

		/*!
		 *	@brief		Returns the NavMeshNode of the given id.
		 *
		 *	@param		i			The index of the desired mesh node.
		 *	@returns	A copy the specified mesh node.
		 */
		const NavMeshNode getNode( unsigned int i ) { return _navMesh->getNode( i ); }

		/*!
		 *	@brief		Retrieves the path for the given agent.
		 *
		 *	@param		id			The identifier for the agent.
		 *	@returns	A pointer to the PortalPath for the agent.  If the agent is not currently
		 *				following a path, NULL is returned.
		 */
		PortalPath * getPath( size_t id );

		/*!
		 *	@brief		Sets the path for the given agent.
		 *
		 *	@param		agentID		The index of the agent for whom the path is set.
		 *	@param		path		The path for the agent.
		 */
		void setPath( size_t agentID, PortalPath * path );

		/*!
		 *	@brief		Clears the path for the given agent.
		 *
		 *	@param		agentID		The index of the agent whose path is to be cleared.
		 */
		void clearPath( size_t agentID );

		/*!
		 *	@brief		Sets the location of the agent to be anode
		 *
		 *	@param		agentID		The index of the agent to update.
		 *	@param		nodeID		The index of the node.
		 */
		void setNode( size_t agentID, unsigned int nodeID );

		/*!
		 *	@brief		Sets the tracking status of the localizer to all agents.
		 *
		 *				By default, the localizer only tracks agents on paths.  
		 *				Calling this function will cause all agents to be tracked,
		 *				even if they are not currently following navigation mesh
		 *				paths.  However, it can't be set back.
		 */
		void setTrackAll() { _trackAll = true; }

		/*!
		 *	@brief		Updates the location of the agent relative to the nav mesh.
		 *
		 *	@param		agent		The agent whose location is updated.
		 *	@param		force		Force causes the update regardless of whether 
		 *							the localizer is set to track all or not.
		 *	@returns	The index of the node in which the agent is found.  If it is not
		 *				on the navigation mesh, NavMeshLocation::NO_NODE is returned.
		 */
		unsigned int updateLocation( const Agents::BaseAgent * agent, bool force=false ) const;

		/*!
		 *	@brief		Set the path planner for the localizer.
		 *
		 *	@param		planner		The path planner.
		 */
		void setPlanner( PathPlanner * planner ) { _planner = planner; }

		/*!
		 *	@brief		Get the planner for the localizer.
		 *
		 *	@returns	A pointer to the planner
		 */
		PathPlanner * getPlanner() { return _planner; }

		/*!
		 *	@brief		Returns the occupant set for the given node
		 *
		 *	@param		nodeID		The index of the desired node.
		 *	@returns	A pointer to the OccupantSet of the given node.
		 */
		const OccupantSet * getNodeOccupants( unsigned int nodeID ) const { return &_nodeOccupants[ nodeID ]; }

		/*!
		 *	@brief		Returns a const pointer to the underlying navigation mesh
		 *
		 *	@returns	The pointer to the navigation mesh.
		 */
		const NavMeshPtr getNavMesh() const { return _navMesh; }

		/*!
		 *	@brief		Returns a pointer to the underlying navigation mesh
		 *
		 *	@returns	The pointer to the navigation mesh.
		 */
		NavMeshPtr getNavMesh() { return _navMesh; }

		////////////////////////////////////////////////////////////////
		//					Construction functions
		////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Parses a navigation mesh localizer definition and returns a pointer to it.
		 *
		 *	This function works in conjunction with the ResourceManager.  That is why it
		 *	returns a pointer, not to a NavMeshLocalizer, but to a Resource.  The 
		 *	ResourceManager uses it to load and instantiate NavMeshLocalizer instances.
		 *
		 *	@param		fileName		The path to the file containing the NavMesh
		 *								definition.
		 *	@returns	A pointer to the new NavMeshLocalizer (if the file is valid), NULL if
		 *				invalid.
		 */
		static Resource * load( const std::string & fileName );

		/*!
		 *	@brief		The unique label for this data type to be used with 
		 *				resource management.
		 */
		static const std::string LABEL;

	friend class PortalPath;

	protected:
		/*!
		 *	@brief		The underlying navigation mesh to perform operations on.
		 */
		NavMeshPtr _navMesh;

		/*!
		 *	@brief		Determines if just the position of agents on paths is tracked
		 *				(true) or the position of all agents (false).
		 */
		bool _trackAll;

		/*!
		 *	@brief		Optional planner.  This is only non-NULL if there is a PathPlanner created either
		 *				by a NavMeshGoalGenerator or NavMeshVelComponent.
		 */
		PathPlanner * _planner;

		/*!
		 *	@brief		A mapping from agent id to the agent's location w.r.t. the
		 *				navigation mesh.
		 */
		// NOTE: In order to call the [] operator in a const function, this needs to
		//		be mutable.  Even if the code guarantees no changes.
		mutable HASH_MAP< size_t, NavMeshLocation > _locations;

		/*!
		 *	@brief		Lock for location map (_locations).
		 */
		ReadersWriterLock	_locLock;
		
		/*!
		 *	@brief		A mapping from node id to agent ids, specifying the population
		 *				of each agent.
		 */
		OccupantSet * _nodeOccupants;

		/*!
		 *	@brief		Determines which node an agent is in without previous knowledge
		 *
		 *	@param		p				Given the initial position, returns the node
		 *								this point lies on (with the highest elevation).
		 *	@param		tgtElev			The target elevation for the agent.  The default
		 *								value leads to the highest node being selected.
		 *								But giving an alternative value will change it to
		 *								select a node nearest the current elevation.
		 *	@returns	The index of the node on which the point lies closest in elevation.  
		 *				to the tgtElev value.  If the point does not lie on any mesh node, 
		 *				NavMeshLocation::NO_NODE is returned.
		 */
		unsigned int findNodeBlind( const Vector2 & p, float tgtElev=1e5f ) const;

		/*!
		 *	@brief		Determines if the point is in a neighboring node to the given node
		 *
		 *	@param		node			The navigation mesh node whose neighbors
		 *								need to be tested.
		 *	@param		p				The point to test against the neighboring nodes.
		 *	@returns	The index of the neighboring node which contains p.  If p
		 *				does not lie on any neighboring mesh node, NavMeshLocation::NO_NODE 
		 *				is returned.
		 */
		unsigned int testNeighbors( const NavMeshNode & node, const Vector2 & p ) const;
	};


	/*!
	 *	@brief		The definition of the managed pointer for NavMeshLocalizer data
	 */
	typedef ResourcePtr< NavMeshLocalizer > NavMeshLocalizerPtr;

	/*!
	 *	@brief		Loads the navigation mesh of the given name
	 *
	 *	@param		fileName		The name of the file containing the navigation 
	 *								mesh definition.
	 *	@param		usePlanner		Indicates if a planner is required (true) or not (false).		
	 *	@returns	The NavMeshLocalizerPtr containing the data.
	 *	@throws		A ResourceException if the data is unable to be instantiated.
	 */
	NavMeshLocalizerPtr loadNavMeshLocalizer( const std::string & fileName, bool usePlanner ) throw ( ResourceException );

}	// namespace Menge

#endif	 // __NAV_MESH_LOCALIZER_H__
