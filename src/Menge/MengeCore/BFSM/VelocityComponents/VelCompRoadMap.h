/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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
 *	@file		VelCompRoadMap.h
 *	@brief		Provides the definition of the road map velocity component.
 *				The preferred velocity is defined for each agent based on a
 *				discrete graph reprsentation of the free space (a roadmap)
 *				and searches on that graph.
 */

#ifndef __VEL_COMP_ROAD_MAP_H__
#define __VEL_COMP_ROAD_MAP_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/resources/Graph.h"
#include "MengeCore/resources/RoadMapPath.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#include <map>

namespace Menge {

	// forward declaration
	template < class R >
	class ResourcePtr;

	/*!
	 *  @brief forward declaration of graph resource pointer.
	 *         see graph.h for more details
	 */
	typedef ResourcePtr< Graph > GraphPtr;

	namespace BFSM {
		// forward declaration
		class RoadMapVCContext;

		/*!
		 *	@brief		A velocity component that returns a preferred velocity whose direction
		 *				and preferred speed are computed from a velocity field.
		 *
		 *	If the velocity vector is of unit length, the preferred speed will be unchanged.
		 *	Otherwise, the preferred speed is scaled by the length of the velocity vector.
		 */
		class MENGE_API RoadMapVelComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			RoadMapVelComponent();

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		graph		The graph representing the roadmap.
			 *							The graph will be destroyed when the velocity component
			 *							is destroyed.
			 */
			RoadMapVelComponent( const GraphPtr & graph );

			/*!
			 *	@brief		Destructor.
			 */
			~RoadMapVelComponent();

			/*!
			 *	@brief		Sets the road map for this velocity component.
			 *
			 *	@param		graph		The graph of the roadmap.
			 */
			void setRoadMap( const GraphPtr & graph ) { _roadmap = graph; }

			/*!
			 *	@brief		Returns a resource pointer to the underlying raod map.
			 *
			 *	@returns	The road map.
			 */
			GraphPtr getRoadMap() { return _roadmap; }

			/*!
			 *	@brief		Returns a reference to the path data for the velocity component.
			 *
			 *	@returns	A const reference to the underlying pathdata.
			 */
			const PathMap & getPathMap() const { return _paths; }

			/*!
			 *	@brief		Called when the agent leaves the state which possesses this velocity
			 *				component.
			 *
			 *	Gives the velocity component to restore any agent-specific data it might have
			 *	changed.
			 *
			 *	@param		agent		The agent who left the state.
			 */
			virtual void onExit( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Computes and sets the agent's preferred velocity.
			 *
			 *	The velocity component directly sets preferred velocity values in the
			 *	the provided preferred velocity instance.  See Agents::PrefVelocity for details.
			 *	Rather than setting the agent's preferred velocity value directly, a reference
			 *	to a preferred velocity instance is passed in to make the use more general.
			 *	This allows the computation of the preferred velocity for the agent, without
			 *	necessarily making changes to it.
			 *
			 *	@param		agent		The agent for which a preferred velocity is computed.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal,
										  Agents::PrefVelocity & pVel ) const;

			/*!
			 *	@brief		Used by the plugin system to know what artifacts to associate with
			 *				agents of this type.  Every sub-class of must return a globally
			 *				unique value if it should be associated with unique artifacts.
			 */
			virtual std::string getStringId() const { return NAME; }

			/*! The unique identifier used to register this type with run-time components. */
			static const std::string NAME;

		protected:
			/*!
			 *	@brief		The roadmap.
			 */
			GraphPtr	_roadmap;

			/*!
			 *	@brief		The paths for all agents in this state.  Performing otherwise const
			 *				queries, may cause this to update.
			 */
			// TODO: Perform these updates in a non-const context so that this does *not* have to
			// be mutable.
			mutable PathMap	_paths;

			/*!
			 *	@brief		Lock to protect _paths;
			 */
			ReadersWriterLock	_lock;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the RoadMapVelComponent.
		 */
		class MENGE_API RoadMapVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			RoadMapVCFactory();

			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return RoadMapVelComponent::NAME.c_str(); }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is derived from a path along a "
					"graph discretization of the free space (a.k.a. a road map).";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field
			 *	valuesof the instance will be set in a subsequent call to
			 *	VelCompFactory::setFromXML. The caller of this function takes ownership of the
			 *	memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new RoadMapVelComponent(); }
			
			/*!
			 *	@brief		Given a pointer to an VelComponent instance, sets the appropriate
			 *				fields from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this VelComponent's type.
			 *	(i.e. VelCompFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of VelCompFactory introduce *new* VelComponent parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		vc			A pointer to the velocity component whose attributes are to
			 *							be set.
			 *	@param		node		The XML node containing the velocity component attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the velocity component
			 *							references resources in the file system, it should be
			 *							defined relative to the behavior file location.  This is
			 *							the folder containing that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( VelComponent * vc, TiXmlElement * node,
									 const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "file_name" string attribute.
			 */
			size_t	_fileNameID;
		};
	}	// namespace BFSM
}	// namespace Menge
#endif	// __VEL_COMP_ROAD_MAP_H__
