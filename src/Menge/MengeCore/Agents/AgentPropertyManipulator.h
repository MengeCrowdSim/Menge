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
 *	@file		AgentPropertyManipulator.h
 *	@brief		Classes for manipulating agent properties in an
 *				"undoable" manner (albeit, to a limited degree).
 */

#ifndef __AGENT_PROPERTY_MANIPULATOR_H__
#define __AGENT_PROPERTY_MANIPULATOR_H__

#include "Element.h"
#include "mengeCommon.h"
#include "FSMEnumeration.h"
#include <map>
#include "SimpleLock.h"


namespace Menge {
	// Forward declarations
	namespace Agents {
		class BaseAgent;
	}


	/*!
	 *	@brief		Helper function for parsing property actions
	 *
	 *	It converts the xml string indicating the property to an 
	 *	enumerated value
	 *
	 *	@param		opName		The name of the operand as specified in the
	 *							xml specification.
	 *	@returns	The corresponding enumeration.  If there is no corresponding
	 *				enumeration, NO_PROPERTY is returned.
	 */
	MENGE_API BFSM::PropertyOperand parsePropertyName( const std::string & opName );

	/////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Base class for manipulating agent properties in an
	 *				undoable manner.  
	 *	The undo is limited, it only maintains knowledge of one manipulation.  
	 *	If multiple manipulations are applied, only the last can be undone.
	 */
	class MENGE_API AgentPropertyManipulator : public Element {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		AgentPropertyManipulator();

	protected:
		/*!
		 *	@brief		Virtual destructor.
		 */
		virtual ~AgentPropertyManipulator();

	public:
		/*!
		 *	@brief		Applies the manipulation to the given agent, storing
		 *				the previous state for restoration.
		 *
		 *	@param		agent		The agent to manipulate.
		 */
		void manipulate( Agents::BaseAgent * agent );

		/*!
		 *	@brief		Restores the agent's previous value.
		 *
		 *	If there is no previous value for the provided agent, nothing
		 *	happens.
		 *
		 *	@param		agent		The agent whose property value should be restored.
		 */
		void restore( Agents::BaseAgent * agent );

		/*!
		 *	@brief		Sets the generator for the manipulator.
		 *
		 *	@param		gen		The generator to assign to this manipulator.
		 *						Any previous generator will be destroyed and
		 *						it is assumed that the generator is *not* shared
		 *						with any other entity.
		 */
		void setGenerator( FloatGenerator * gen );

		/*!
		 *	@brief		Sets the property operand.
		 *
		 *	@param		prop		The property to operate on.
		 */
		void setProperty( BFSM::PropertyOperand prop ) { _property = prop; }

	protected:
		/*!
		 *	@brief		Computes the new property value given the original property value.
		 *
		 *	@param		value		The original value of the property.
		 *	@param		agentID		The identifier for the agent.
		 *	@returns	The new value.
		 */
		virtual float newValue( float value, size_t agentID ) = 0;

		/*!
		 *	@brief		The generator for determining the operand value.
		 */
		FloatGenerator * _operandGen;

		/*!
		 *	@brief		The property to operate on.
		 */
		BFSM::PropertyOperand _property;

		/*!
		 *	@brief		A mapping from agent id to the agent's property
		 *				value before the action was applied.
		 */
		std::map< size_t, float >	_originalMap;

		/*! 
		 *	@brief		Lock for guaranteeing thread-safety.
		 */
		SimpleLock	_lock;
	};

	/////////////////////////////////////////////////////////////////////

	/*! 
	 *	@brief		Modifies a particular agent property based by explicitly
	 *				setting the property value from a distribution.
	 */
	class MENGE_API SetPropertyManipulator : public AgentPropertyManipulator {
	protected:
		/*!
		 *	@brief		Computes the new property value given the original property value.
		 *
		 *	@param		value		The original value of the property.
		 *	@param		agentID		The identifier for the agent.
		 *	@returns	The new value.
		 */
		virtual float newValue( float value, size_t agentID );
	};

	/////////////////////////////////////////////////////////////////////

	/*! 
	 *	@brief		Modifies a particular agent property based by adding
	 *				the value from a distribution to the original agent 
	 *				parameter value.
	 */
	class MENGE_API OffsetPropertyManipulator : public AgentPropertyManipulator {
	protected:
		/*!
		 *	@brief		Computes the new property value given the original property value.
		 *
		 *	@param		value		The original value of the property.
		 *	@param		agentID		The identifier for the agent.
		 *	@returns	The new value.
		 */
		virtual float newValue( float value, size_t agentID );
	};

	/////////////////////////////////////////////////////////////////////

	/*! 
	 *	@brief		Modifies a particular agent property based by multiplying
	 *				the value from a distribution to the original agent 
	 *				parameter value.
	 */
	class MENGE_API ScalePropertyManipulator : public AgentPropertyManipulator {
	protected:
		/*!
		 *	@brief		Computes the new property value given the original property value.
		 *
		 *	@param		value		The original value of the property.
		 *	@param		agentID		The identifier for the agent.
		 *	@returns	The new value.
		 */
		virtual float newValue( float value, size_t agentID );
	};
	
}	// namespace Menge
#endif	// __AGENT_PROPERTY_MANIPULATOR_H__
