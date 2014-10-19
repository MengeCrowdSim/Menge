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
 *	@file		VelComponent.h
 *	@brief		The definition of how preferred velocity is computed in a state.
 */
#ifndef __VEL_COMPONENT_H__
#define __VEL_COMPONENT_H__

#include "fsmCommon.h"
#include "Element.h"
#include "PrefVelocity.h"
#include "MengeException.h"

// STL
#include <vector>		// stl vector

namespace Menge {

	// forward declaration
	template < class Element >
	class ElementFactory;
		
	namespace BFSM {

		// FORWARD DECLARATIONS
		class VelCompContext;
		class Goal;

		/*!
		 *	@brief		Base exception class for preferred velocity computation.
		 */
		class MENGE_API VelCompException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelCompException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VelCompException( const std::string & s ): MengeException(s) {}

		};
		
		/*!
		 *	@brief		Exception thrown when the velocity component has an error which cannot be
		 *				recovered from.
		 */
		class VelCompFatalException : public VelCompException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelCompFatalException() : MengeException(), VelCompException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VelCompFatalException( const std::string & s ): MengeException(s), VelCompException(), MengeFatalException() {}
		};

		/*!
		 *	@brief		The base class for computing an agent's preferred velocity.
		 *
		 *	Velocity components each have weights.  The relative weights determine
		 *	how multiple velocity components combine inside a single state.  Their
		 *	share is its own weight divided by the summed weight of all velocity
		 *	components.
		 */
		class MENGE_API VelComponent : public Element {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelComponent() : Element() {}

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~VelComponent(){}

		public:
			/*!
			 *	@brief		Called when the agent enters the state which possesses this velocity component.
			 *
			 *	Gives the velocity component to intialize any agent-specific data.
			 *
			 *	@param		agent		The agent who entered the state.
			 */
			virtual void onEnter( Agents::BaseAgent * agent ) {}

			/*!
			 *	@brief		Called when the agent leaves the state which possesses this velocity component.
			 *
			 *	Gives the velocity component to restore any agent-specific data it might have changed.
			 *
			 *	@param		agent		The agent who left the state.
			 */
			virtual void onExit( Agents::BaseAgent * agent ) {}

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
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) = 0;

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();

			friend class ElementFactory< VelComponent >;

		};

		/*!
		 *	@brief		Parses a TinyXML element containing a velocity component specification
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new velocity component implementation (NULL if no valid
		 *				instance could be created).
		 */
		VelComponent * parseVelComponent( TiXmlElement * node, const std::string & behaveFldr );


	}	// namespace BFSM
}	// namespace Menge
#endif	// __VEL_COMPONENT_H__
