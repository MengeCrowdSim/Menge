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
 *	@file		PropertyAction.h
 *	@brief		Defines a set of BFSM actions that change agent
 *				parameters.
 */

#ifndef __PROPERTY_ACTION_H__
#define __PROPERTY_ACTION_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Agents/AgentPropertyManipulator.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"

namespace Menge {

	// forward declaration

	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {
		
		////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The base class for modifying agent properties.
		 *
		 *	This is an abstract class and must be sub-classed.  To create different
		 *	types of property actions, simply specialize this templated class with
		 *	a different type of AgentPropertyManipulator.
		 */
		template <class Manipulator>
		class MENGE_API PropertyAction : public Action {
		public:
			/*!
			 *	@brief		Constructor
			 */
			PropertyAction():Action(), _manip() {}

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~PropertyAction(){}

		public:
			/*! 
			 *	@brief		Upon entering the state, this is called -- it is the main work
			 *				of the action.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void onEnter( Agents::BaseAgent * agent ) { _manip.manipulate( agent ); }

			/*!
			 *	@brief		Returns a pointer to the manipulator.
			 */
			Manipulator * getManipulator() { return &_manip; }

		protected:

			/*!
			 *	@brief		The work to do upon state exit.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void resetAction( Agents::BaseAgent * agent ) { _manip.restore( agent ); }

			/*!
			 *	@brief		The manipulator responsible for changing agent properties.
			 */
			Manipulator	_manip;

		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Factory for the PropertyAction.
		 *
		 *	This is still an abstract class because it doesn't define the
		 *	name or description.  Nor does it define instance.
		 */
		template <class Manipulator>
		class MENGE_API PropertyActFactory : public ActionFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			PropertyActFactory() {
				_propertyID = _attrSet.addStringAttribute( "property", true /*required*/ );
				_generatorID = _attrSet.addFloatDistAttribute( "", true /*required*/, 0.f, 1.f );
			}

		protected:
			/*!
			 *	@brief		Given a pointer to an Action instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this Action's type.
			 *	(i.e. ActionFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of ActionFactory introduce *new* Action parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		action		A pointer to the action whose attributes are to be set.
			 *	@param		node		The XML node containing the action attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the action references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder
			 *							containing that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( Action * action, TiXmlElement * node,
									 const std::string & behaveFldr ) const {
				PropertyAction< Manipulator > * pAction = 
					dynamic_cast< PropertyAction< Manipulator > * >( action );
				assert( pAction != 0x0 &&
						"Trying to set property action properties on an incompatible object" );
				
				if ( ! ActionFactory::setFromXML( action, node, behaveFldr ) ) return false;
				Menge::AgentPropertyManipulator * manip = pAction->getManipulator();

				PropertyOperand prop = 
					Menge::parsePropertyName( _attrSet.getString( _propertyID ) );
				manip->setProperty( prop );
				if ( prop == NO_PROPERTY ) {
					logger << Logger::ERR_MSG << "The property action defined on line ";
					logger << node->Row();
					logger << " specified an invalid value for the \"property\" attribute";
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
		 *	@brief		Factory for the SetPropertyAction.
		 */
		class MENGE_API SetPropertyActFactory : 
			public PropertyActFactory< Menge::SetPropertyManipulator > {
		public:
			
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "set_property"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "Causes the specified property to be *replaced* by the user-defined value";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's action.
			 *
			 *	All ActionFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding action type.  The various field values
			 *	of the instance will be set in a subsequent call to ActionFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated Action class.
			 */
			Action * instance() const {
				return new PropertyAction< Menge::SetPropertyManipulator >();
			}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Factory for the OffsetPropertyAction.
		 */
		class MENGE_API OffsetPropertyActFactory :
			public PropertyActFactory< Menge::OffsetPropertyManipulator > {
		public:
			
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "offset_property"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "Adds the user-defined value into the agent's specified property";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's action.
			 *
			 *	All ActionFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding action type.  The various field values
			 *	of the instance will be set in a subsequent call to ActionFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated Action class.
			 */
			Action * instance() const {
				return new PropertyAction< Menge::OffsetPropertyManipulator >();
			}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Factory for the ScalePropertyAction.
		 */
		class MENGE_API ScalePropertyActFactory :
			public PropertyActFactory< Menge::ScalePropertyManipulator > {
		public:
			
			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "scale_property"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "Scales the user-defined value into the agent's specified property";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's action.
			 *
			 *	All ActionFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding action type.  The various field values
			 *	of the instance will be set in a subsequent call to ActionFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated Action class.
			 */
			Action * instance() const {
				return new PropertyAction< Menge::ScalePropertyManipulator >();
			}
		};
		
	}	//  namespace BFSM

}	// namespace Menge

#endif // __PROPERTY_ACTION_H__
