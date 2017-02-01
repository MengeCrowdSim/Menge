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
 *	@file		AircraftAction.h
 *	@brief		Definition of actions used in Aircraft
 *				loading and unloading.
 */
#ifndef __Aircraft_ACTION_H__
#define __Aircraft_ACTION_H__

#include "AircraftConfig.h"

#include "MengeCore/BFSM/FSMEnumeration.h"
#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"
#include "MengeCore/Runtime/SimpleLock.h"
#include <map>

//forward declaration
class TiXmlElement;

/*!
 *	@namespace	Aircraft
 *	@brief		The namespace containing Menge elements required for simulating
 *				aircraft loading and unloading.
 */
namespace Aircraft {
	// forward declaration
	class SetPropertyXActFactory;
	
	/*!
	 *	@brief		An action that sets agent properties based on the
	 *				agent's position along the x-axis.
	 */
	class EXPORT_API PropertyXAction : public Menge::BFSM::Action {
	public:
		/*!
		 *	@brief		Constructor
		 */
		PropertyXAction();

		/*!
		 *	@brief		Virtual destructor.
		 */
		~PropertyXAction();

		/*! 
		 *	@brief		Upon entering the state, this is called -- it is the main work
		 *				of the action.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void onEnter( Menge::Agents::BaseAgent * agent );

		friend class PropertyXActFactory;

	protected:

		/*!
		 *	@brief		The work to do upon state exit.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void leaveAction( Menge::Agents::BaseAgent * agent );

	protected:
		/*!
		 *	@brief		The x-position at which the value is set to
		 *				_originValue;
		 */
		float	_xOrigin;

		/*!
		 *	@brief		The value for the property at _xOrigin;
		 */
		float	_originValue;

		/*!
		 *	@brief		The rate of change to the property for each unit
		 *				displacement along the x-axis.  (Essentially, this
		 *				is the slope of the line.)
		 */
		float	_scale;

		/*!
		 *	@brief		The property to operate on.
		 */
		Menge::BFSM::PropertyOperand _property;

		/*!
		 *	@brief		A mapping from agent id to the agent's property
		 *				value before the action was applied.
		 */
		std::map< size_t, float >	_originalMap;

		/*!
		 *	@brief		Lock to protect _originalMap.
		 */
		Menge::SimpleLock	_lock;
	};

	/*!
	 *	@brief		Factory for instantiating PropertyXAction instances.
	 */
	class EXPORT_API PropertyXActFactory : public Menge::BFSM::ActionFactory {
	public:
		/*!
		 *	@brief		The name of the action.
		 *
		 *	The action's name must be unique among all registered actions.
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the unique action name.
		 */
		virtual const char * name() const { return "set_xproperty"; }

		/*!
		 *	@brief		A description of the action.
		 *
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the action description.
		 */
		virtual const char * description() const {
			return "Sets an agent property's value from a simple linear equation dependent on the "
				"agent's x-position.";
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
		Menge::BFSM::Action * instance() const { return new PropertyXAction(); }
		
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
		 *							to the behavior file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( Menge::BFSM::Action * action, TiXmlElement * node,
								 const std::string & behaveFldr ) const;
	};
}	// namespace Aircraft

#endif	// __Aircraft_ACTION_H__
