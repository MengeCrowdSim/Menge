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
 *	@file		AgentPropertyEffect.h
 *	@brief		The definition of an event effect that modifies agent
 *				properties.
 */

#ifndef __AGENT_PROPERT_EFFECT_H__
#define __AGENT_PROPERT_EFFECT_H__

#include "EventEffectFactory.h"
#include "AgentEventEffect.h"
#include "AgentPropertyManipulator.h"

namespace Menge {

	// forward declaration
	namespace Agents {
		class BaseAgent;
	}

	/*!
	 *	@brief		The base class event effect changes agent properties.
	 *
	 *	This is an abstract class and must be sub-classed.  To create different
	 *	types of event effects, simply specialize this templated class with
	 *	a different type of AgentPropertyManipulator.
	 */
	template <class Manipulator>
	class MENGE_API AgentPropertyEffect : public AgentEventEffect {
	public:
		/*!
		 *	@brief		Constructor
		 */
		AgentPropertyEffect(): AgentEventEffect(), _manip() {}

		/*!
		 *	@brief		Returns a pointer to the manipulator.
		 */
		Manipulator * getManipulator() { return &_manip; }

	protected:

		/*!
		 *	@brief		The actual work of the effect.
		 *
		 *	Sub-classes should implement this.  It is the action to be taken for each
		 *	agent.
		 *
		 *	@param		agent		The agent to operate on.
		 */
		virtual void agentEffect( Agents::BaseAgent * agent ) { _manip.manipulate( agent ); }

		/*!
		 *	@brief		The manipulator responsible for changing agent properties.
		 */
		Manipulator	_manip;
	};

	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The factory for agent property event effects.
	 */
	template < class Manipulator >
	class MENGE_API AgentPropertyEffectFactory : public EventEffectFactory {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		AgentPropertyEffectFactory() : EventEffectFactory() {
			_propertyID = _attrSet.addStringAttribute( "property", true /*required*/ );
			_generatorID = _attrSet.addFloatDistAttribute( "", true /*required*/, 0.f, 1.f );
		}

	protected:
		/*!
		 *	@brief		Given a pointer to an AgentPropertyEffect instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this AgentPropertyEffect's type.
		 *	(i.e. EventEffectFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of EventEffectFactory introduce *new* EventEffect parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		effect		A pointer to the effect whose attributes are to be set.
		 *	@param		node		The XML node containing the event effect attributes.
		 *	@param		behaveFldr	The path to the behavior file.  If the action references
		 *							resources in the file system, it should be defined relative
		 *							to the behavior file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( EventEffect * effect, TiXmlElement * node, const std::string & behaveFldr ) const {
			AgentPropertyEffect< Manipulator > * aEffect = dynamic_cast< AgentPropertyEffect< Manipulator > * >( effect );
			assert( aEffect != 0x0 && "Trying to set agent event effect properties on an incompatible object" );
			
			// This parses the target
			if ( ! EventEffectFactory::setFromXML( aEffect, node, behaveFldr ) ) return false;

			// Configure manipulator
			AgentPropertyManipulator * manip = aEffect->getManipulator();

			BFSM::PropertyOperand prop = parsePropertyName( _attrSet.getString( _propertyID ) );
			manip->setProperty( prop );
			if ( prop == BFSM::NO_PROPERTY ) {
				logger << Logger::ERR_MSG << "The property event effect defined on line " << node->Row() << " specified an invalid value for the \"property\" attribute";
				return false;
			}
			manip->setGenerator( _attrSet.getFloatGenerator( _generatorID ) );

			return true;
		}

		/*!
		 *	@brief		The identifier for the "property" string attribute.
		 */
		size_t	_propertyID;

		/*!
		 *	@brief		The identifier for the float distribution attribute.
		 */
		size_t	_generatorID;
	};

	/////////////////////////////////////////////////////////////////////
	
	/*!
	 *	@brief		Factory for the SetPropertyEffect.
	 */
	class MENGE_API SetAgentPropertyEffectFactory : public AgentPropertyEffectFactory< SetPropertyManipulator > {
	public:
		
		/*!
		 *	@brief		The name of the effect.
		 *
		 *	The effect's name must be unique among all registered effect.
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the unique effect name.
		 */
		virtual const char * name() const { return "set_agent_property"; }

		/*!
		 *	@brief		A description of the effect.
		 *
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the effect description.
		 */
		virtual const char * description() const {
			return "Causes the specified property to be *replaced* by the user-defined value";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's effect.
		 *
		 *	All EventEffectFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding event effect type.  The various field values
		 *	of the instance will be set in a subsequent call to EventEffectFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated EventEffect class.
		 */
		EventEffect * instance() const { return new AgentPropertyEffect< SetPropertyManipulator >(); }	
	};

	/////////////////////////////////////////////////////////////////////
	
	/*!
	 *	@brief		Factory for the OffsetPropertyEffect.
	 */
	class MENGE_API OffsetAgentPropertyEffectFactory : public AgentPropertyEffectFactory< OffsetPropertyManipulator > {
	public:
		
		/*!
		 *	@brief		The name of the effect.
		 *
		 *	The effect's name must be unique among all registered effect.
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the unique effect name.
		 */
		virtual const char * name() const { return "offset_agent_property"; }

		/*!
		 *	@brief		A description of the effect.
		 *
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the effect description.
		 */
		virtual const char * description() const {
			return "Causes the specified property to be offset by the user-defined value";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's effect.
		 *
		 *	All EventEffectFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding event effect type.  The various field values
		 *	of the instance will be set in a subsequent call to EventEffectFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated EventEffect class.
		 */
		EventEffect * instance() const { return new AgentPropertyEffect< OffsetPropertyManipulator >(); }	
	};

	/////////////////////////////////////////////////////////////////////
	
	/*!
	 *	@brief		Factory for the ScalePropertyEffect.
	 */
	class MENGE_API ScaleAgentPropertyEffectFactory : public AgentPropertyEffectFactory< ScalePropertyManipulator > {
	public:
		
		/*!
		 *	@brief		The name of the effect.
		 *
		 *	The effect's name must be unique among all registered effect.
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the unique effect name.
		 */
		virtual const char * name() const { return "scale_agent_property"; }

		/*!
		 *	@brief		A description of the effect.
		 *
		 *	Each effect factory must override this function.
		 *
		 *	@returns	A string containing the effect description.
		 */
		virtual const char * description() const {
			return "Causes the specified property to be scaled by the user-defined value";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's effect.
		 *
		 *	All EventEffectFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding event effect type.  The various field values
		 *	of the instance will be set in a subsequent call to EventEffectFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated EventEffect class.
		 */
		EventEffect * instance() const { return new AgentPropertyEffect< ScalePropertyManipulator >(); }	
	};

}	// namespace Menge

#endif	// __AGENT_PROPERT_EFFECT_H__