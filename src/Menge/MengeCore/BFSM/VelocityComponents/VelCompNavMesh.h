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
 *	@file		VelCompNavMesh.h
 *	@brief		Provides the definition of the navigation mesh velocity component.
 *				The preferred velocity is defined for each agent based on a
 *				polygonal mesh representation of the free space (a navigation mesh)
 *				and searches on that graph.
 */

#ifndef __VEL_COMP_NAV_MESH_H__
#define __VEL_COMP_NAV_MESH_H__

#include "CoreConfig.h"
#include "VelocityComponents/VelComponent.h"
#include "VelocityComponents/VelComponentFactory.h"
#include "VelocityComponents/VelCompContext.h"
#include "NavMesh.h"
#include "NavMeshLocalizer.h"

// forward declaration

namespace Menge {

	namespace BFSM {
		// forward declaration
		class NavMeshVCContext;

		/*!
		 *	@brief		A velocity component that returns a preferred velocity whose direction
		 *				and preferred speed are computed from a navigation mesh.
		 *
		 *	A navigation mesh is a representation of the traversalbe space.  The traversable space
		 *	is represented as a polygonal mesh.  Graph searches through the mesh are performed to
		 *  find paths through arbitrarily complex environments.
		 */
		class MENGE_API NavMeshVelComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			NavMeshVelComponent();

			/*!
			 *	@brief		Called when the agent exits the state that uses this velocity component.
			 *
			 *	@param		agent		The agent exiting the state.
			 */
			virtual void onExit( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Sets the navigation mesh pointer.
			 *
			 *	@param		nm		The managed pointer to the navigation mesh.
			 */
			void setNavMesh( const NavMeshPtr & nm ) { _navMesh = nm; }

			/*!
			 *	@brief		Returns a resource pointer to the underlying navigation mesh
			 *
			 *	@returns	The navigation mesh.
			 */
			NavMeshPtr getNavMesh() { return _navMesh; }

			/*!
			 *	@brief		Returns a resource pointer to the underlying navigation mesh localizer
			 *
			 *	@returns	The navigation mesh localizer.
			 */
			NavMeshLocalizerPtr getLocalizer() { return _localizer; }

			/*!
			 *	@brief		Sets the navigation mesh localizer pointer.
			 *
			 *	@param		nml		The managed pointer to the navigation mesh localizer.
			 */
			void setNavMeshLocalizer( const NavMeshLocalizerPtr & nml ) { _localizer = nml; }

			/*!
			 *	@brief		Sets the angular deviation which triggers replanning.
			 *
			 *	@param		angle		The angle of deviation in radians.
			 */
			void setHeadingDeviation( float angle );

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
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();

			/*!
			 *	@brief		Returns a pointer to the nav mesh localizer task.
			 *
			 *	@returns		A pointer to the nav mesh localizer task.  It is the responsibility
			 *					of the caller to free the memory of the provided task by
			 *					calling its destroy method.
			 */
			virtual Task * getTask();

			friend class NavMeshVCContext;

		protected:
			/*!
			 *	@brief		The cosine of the heading deviation angular threshold.  This detects
			 *				when the angle of approach deviates beyond a threshold and the agent
			 *				needs to replan.
			 */
			float	_headingDevCos;

			/*!
			 *	@brief		The navigation mesh.
			 */
			NavMeshPtr	_navMesh;

			/*!
			 *	@brief		The localizer for the navigation mesh.
			 */
			NavMeshLocalizerPtr _localizer;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the NavMeshVelComponent.
		 */
		class MENGE_API NavMeshVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vc			A pointer to the underlying fsm velocity component.
			 *							The context will *not* delete the velocity component.
			 */
			NavMeshVCContext( NavMeshVelComponent * vc );

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity component information.
			 *
			 *	@param		indent		An optional string representing indentation to be
			 *							applied to the text.  It is prefixed at the start
			 *							of each line.
			 *	@returns	The string for printing on the UI layer.
			 */
			virtual std::string getUIText( const std::string & indent="" ) const;

			/*!
			 *	@brief		Give the context the opportunity to respond to a keyboard
			 *				event.
			 *
			 *	@param		e		The SDL event with the keyboard event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual SceneGraph::ContextResult handleKeyboard( SDL_Event & e );

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	This should never be called in select mode.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, const Goal * goal );

		protected:
			/*!
			 *	@brief		Draws the navigation mesh to the 3D context based on current settings.
			 */
			void drawNavMesh() const;

			/*!
			 *	@brief		Draws the path for the selected agent.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 */
			void drawPath( const Agents::BaseAgent * agt, const Goal * goal ) const;

			/*!
			 *	@brief		Draws the ids on the polygon mesh centers.
			 */
			void drawNodeIDs() const;

			/*!
			 *	@brief		The underlying finite state machine velocity component.
			 */
			NavMeshVelComponent * _vc;

			/*!
			 *	@brief		Visualization flag - controls if polygon centers are drawn.
			 */
			bool	_drawCenters;

			/*!
			 *	@brief		Visualizaton flag - controls if polygon ids are drawn.
			 */
			bool	_drawNodeIDs;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the NavMeshVelComponent.
		 */
		class MENGE_API NavMeshVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NavMeshVCFactory();

			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "nav_mesh"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is derived from a path along a "\
					"polygonally decomposed representation of the traversable space (a navigaiton mesh).";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field values
			 *	of the instance will be set in a subsequent call to VelCompFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new NavMeshVelComponent(); }

			/*!
			 *	@brief		Given a pointer to an VelComponent instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this VelComponent's type.
			 *	(i.e. VelCompFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of VelCompFactory introduce *new* VelComponent parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		vc			A pointer to the velocity component whose attributes are to be set.
			 *	@param		node		The XML node containing the velocity component attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the velocity component references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "file_name" string attribute.
			 */
			size_t	_fileNameID;

			/*!
			 *	@brief		The identifier for the "heading_threshold" float attribute.
			 */
			size_t	_headingID;
		};
	}	// namespace BFSM
}	// namespace Menge
#endif	// __VEL_COMP_NAV_MESH_H__
