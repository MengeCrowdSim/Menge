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
 *	@file		VelModifier.h
 *	@brief		The definition of how preferred velocity is modified by a filter
 */
#ifndef __VEL_MODIFIER_H__
#define __VEL_MODIFIER_H__

#include "fsmCommon.h"
#include "Element.h"
#include "PrefVelocity.h"
#include "MengeException.h"

// STL
#include <vector>	

namespace Menge {

	// forward declaration
	template < class Element >
	class ElementFactory;
		
	namespace BFSM {

		// FORWARD DECLARATIONS
		class VelModContext;

		/*!
		 *	@brief		Base exception class for preferred velocity modification.
		 */
		class MENGE_API VelModException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelModException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VelModException( const std::string & s ): MengeException(s) {}

		};
		
		/*!
		 *	@brief		Exception thrown when the velocity modifier has an error which cannot be
		 *				recovered from.
		 */
		class VelModFatalException : public VelModException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelModFatalException() : MengeException(), VelModException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			VelModFatalException( const std::string & s ): MengeException(s), VelModException(), MengeFatalException() {}
		};

		/*!
		 *	@brief		The base class for modifying preferred velocities
		 *
		 *	Each velocity modifier is allowed to change the input preferred velocity without limit based on arbitrary criteria.
		 *	They have an order, which defines which order in which they are executed. Modifiers are chained so that the input
		 *	of each modifier is the output of the previous, making order very important.
		 */
		class MENGE_API VelModifier : public Element {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VelModifier() : Element() {}

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~VelModifier(){}

		public:
			
			/*!
			 *	@brief       Modifies the input preferred velocity in place.
			 *
			 *  The main function of the velocity modifier.  Applies its own algorithms
			 *	to the input preferred velocity, transforming it into a new transform, in place.
			 *	All VelModifier sub-classes must implement this function.
			 *
			 *	@param		agent		The agent for which a preferred velocity is adapted.
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void adaptPrefVelocity( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ) = 0;

			/*!
			 *	@brief       Registers an agent for use with the VelModifier. 
			 *
			 *	When a velocity modifier is assigned to the agent, the agent is "registered" to it.
			 *	If the velocity modifier needs to maintain per-agent data, this is the time to
			 *	instantiate that data.  Default implementation does nothing.
			 *
			 *	@param		agent		The agent to be registered
			 */
			virtual void registerAgent(const Agents::BaseAgent * agent) {}
			
			/*!
			 *	@brief       Unregisters a registed agent. 
			 *
			 *	If an velocity modifier is removed from an agent, this allows the velocity
			 *	modifier to destroy any agent-specific data.  Default implementation does nothing.
			 *
			 *	@param		agent		The agent to be unregistered
			 */
			virtual void unregisterAgent(const Agents::BaseAgent * agent) {}
			
			/*!
			 *	@brief       Called when an agent enters the state with this velocity modifier.
			 *
			 *	This function calls registerAgent.  Entering a state which has one or more
			 *	velocity modifiers assigned to it, causes the velocity modifiers to be temporarily
			 *	assigned to the agent.  
			 *
			 *	@param		agent		The agent entering the state
			 */
			void onEnter(const Agents::BaseAgent * agent);
			
			/*!
			 *	@brief       Called when an agent exits the state with this velocity modifier.
			 *
			 *	The dual of onEnter.  This function calls unregisterAgent.  Agents departing a 
			 *	state with one or more velocity modifiers will be unregistered.
			 *
			 *	@param		agent		The agent entering the state
			 */
			void onLeave(const Agents::BaseAgent * agent);

			/*!
			 *	@brief		Creates a copy of this velocity modifier.
			 *
			 *	@returns	A unique, deep copy of this velocity modifier.	
			 */
			virtual VelModifier* copy() const = 0;
			
			/*!
			 *	@brief		Provides a display context for interacting with this velocity modifier.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this velocity modifier
			 */
			virtual VelModContext * getContext();

			friend class ElementFactory< VelModifier >;
		};

		/*!
		 *	@brief		Parses a TinyXML element containing a velocity modifier specification.
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new velocity modifier implementation (NULL if no valid
		 *				instance could be created).
		 */
		VelModifier * parseVelModifier( TiXmlElement * node, const std::string & behaveFldr );

	}	// namespace BFSM
}	// namespace Menge

#endif	// __VEL_MODIFIER_H__
