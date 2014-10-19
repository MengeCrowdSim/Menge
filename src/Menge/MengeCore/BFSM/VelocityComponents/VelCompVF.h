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
 *	@file		VelCompVF.h
 *	@brief		Provides the definition of the vector field velocity component.
 *				The preferred velocity is defined for each agent based on where
 *				the agent is in relation a uniformly discretized 2D grid.
 */

#ifndef __VEL_COMP_VF_H__
#define __VEL_COMP_VF_H__

#include "CoreConfig.h"
#include "VelocityComponents/VelComponent.h"
#include "VelocityComponents/VelComponentFactory.h"
#include "VelocityComponents/VelCompContext.h"
#include "VectorField.h"

namespace Menge {

	// forward declaration
	template < class R >
	class ResourcePtr;

	/*!
	 *  @brief forward declaration of VectorField resource pointer.
	 *         see VectorField.h for more details
	 */
	typedef ResourcePtr< VectorField > VectorFieldPtr;

	namespace BFSM {
		/*!
		 *	@brief		A velocity component that returns a preferred velocity whose direction
		 *				and preferred speed are computed from a velocity field.
		 *
		 *	If the velocity vector is of unit length, the preferred speed will be unchanged.
		 *	Otherwise, the preferred speed is scaled by the length of the velocity vector.
		 */
		class MENGE_API VFVelComponent : public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VFVelComponent();

			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vf			The VectorField to use for velocity computation.
			 *	@param		useNearest	Determines if the velocity is computed by selecting the
			 *							neareset vector (true) or the bilinear interpolation of
			 *							four near-by cells (false).
			 */
			VFVelComponent( VectorFieldPtr & vf, bool useNearest );

			/*!
			 *	@brief		Determines whether the vector field velocity component computes
			 *				velocity from the nearest cell center or from a bilinear interpolation on
			 *				the four near-by cells.
			 *
			 *	@param		useNearest		Uses only nearest cell if true, 4-cell neighborhood if false.
			 */
			void setUseNearest( bool useNearest ) { _nearest = useNearest; }

			/*!
			 *	@brief		Sets the velocity field pointer.
			 *
			 *	@param		vf		The managed pointer to the velocity field.
			 */
			void setVectorField( const VectorFieldPtr & vf ) { _vf = vf; }

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

			friend class VecFieldVCContext;

		protected:
			/*!
			 *	@brief		The vector field resource which provides preferred velocity vectors.
			 */
			VectorFieldPtr	_vf;

			/*!
			 *	@brief		Determines if the vector selection is based on nearest (true)
			 *				or bilinear interpolation (false)
			 */
			bool	_nearest;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the VFVelComponent.
		 */
		class MENGE_API VecFieldVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vc			A pointer to the underlying fsm velocity component.
			 *							The context will *not* delete the velocity component.
			 */
			VecFieldVCContext( VFVelComponent * vc );

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
			 *	@brief		The underlying finite state machine velocity component.
			 */
			VFVelComponent * _vc;

			/*!
			 *	@brief		Displays only the local area around the agent
			 */
			bool	_showLocal;

			/*!
			 *	@brief		Size of the local neighborhood to visualize
			 */
			int	_neighborhood;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the VFVelComponent.
		 */
		class MENGE_API VFVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			VFVCFactory();

			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "vel_field"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which is derived from a velocity field defined"\
					" on a uniform, 2D discretization of the planning space.";
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
			VelComponent * instance() const { return new VFVelComponent(); }
			
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
			 *	@brief		The identifier for the "use_nearest" bool attribute.
			 */
			size_t	_useNearestID;
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __VEL_COMP_VF_H__