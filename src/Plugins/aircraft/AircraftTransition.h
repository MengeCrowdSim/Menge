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
 *	@file		AircraftTransition.h
 *	@brief		Definition of actions used in aircraft
 *				loading and unloading.
 */
#ifndef __AIRLINE_TRANSITION_H__
#define __AIRLINE_TRANSITION_H__

#include "AircraftConfig.h"
#include "Transitions/Condition.h"
#include "Transitions/ConditionFactory.h"
#include "FSMEnumeration.h"
#include "Math/Geometry2D.h"
#include <map>

using namespace Menge;

//forward declaration
class TiXmlElement;

namespace Aircraft {
	// forward declaration
	class ClearAABBCondFactory;

	/*!
	 *	@brief		An action that sets agent properties based on the
	 *				agent's position along the x-axis.
	 */
	class EXPORT_API ClearAABBCondition : public BFSM::Condition {
	public:
		/*!
		 *	@brief		Constructor
		 */
		ClearAABBCondition(): BFSM::Condition(), _relative(false), _agentClass(-1), _baseBox(){} // -1 is maximum agent class value

		/*!
		 *	@brief		Copy constructor
		 *
		 *	@param		cond		The condition to copy from.
		 */
		ClearAABBCondition( const ClearAABBCondition & cond );

	protected:
		/*!
		 *	@brief		Virtual destructor.
		 */
		virtual ~ClearAABBCondition();

	public:
		/*!
		 *	@brief		The AutoCondition is always met -- it is a tautology.
		 *
		 *	@param		agent		The agent to test the transition for.
		 *	@param		goal		The agent's goal (although this may be ignored).
		 *	@returns	True if the condition has been met, false otherwise.
		 */
		virtual bool conditionMet( Agents::BaseAgent * agent, const BFSM::Goal * goal );

		/*!
		 *	@brief		Create a copy of this condition.
		 *
		 *	It is the responsibility of the caller to delete the object.
		 *
		 *	@returns:	A "deep copy" of this condition - such that there is no shared
		 *				objects between this and its copy.
		 */
		virtual BFSM::Condition * copy();

		friend class ClearAABBCondFactory;
	
		/*!
		 *	@brief		Sets the dimensions of the bounding box.
		 *
		 *	@param		xMin		The minimum point on the AABB along the x-axis.
		 *	@param		xMax		The maximum point on the AABB along the x-axis.
		 *	@param		yMin		The minimum point on the AABB along the y-axis.
		 *	@param		yMax		The maximum point on the AABB along the y-axis.
		 *	@param		relative	Specifies if the clear box is relative (true) to the
		 *							agents *current* position, or absolute in the world.
		 *	@param		agentClass	The class of the agents which are tested.  If -1
		 *							all agents are considered.
		 */
		void setParams(float xMin, float xMax, float yMin, float yMax, bool relative, int agentClass);

	protected:
		/*!
		 *	@brief		Indicates whether the box is defined relative to the agent's position (true) or not (false).
		 */
		bool _relative; 

		/*!
		 *	@brief		The agent class this transition operates on.
		 *				If -1, all agent classes are tested.
		 */
		size_t _agentClass;

		/*!
		 *	@brief		The definition of the underlying AABB.
		 */
		AABBShape _baseBox;
	};

	/*!
	 *	@brief		The factory for creating the ClearAABBCondition
	 */
	class EXPORT_API ClearAABBCondFactory : public BFSM::ConditionFactory {
	public:
		/*!
		 *	@brief		The name of the action.
		 *
		 *	The action's name must be unique among all registered actions.
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the unique action name.
		 */
		const char * name() const { return "clear_AABB"; }

		/*!
		 *	@brief		A description of the action.
		 *
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the action description.
		 */
		const char * description() const {
			return "The clear axis-aligned bounding box (AABB) condition.  It becomes active when no agents are in a box "\
				"defined in either an absolute position, or relative to the agent.";
		}
		
	protected:
		/*!
		 *	@brief		Create an instance of this class's condition.
		 *
		 *	All ConditionFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding condition type.  The various field values
		 *	of the instance will be set in a subsequent call to ConditionFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated Action class.
		 */
		ClearAABBCondition * instance() const { return new ClearAABBCondition(); }

		/*!
		 *	@brief		Given a pointer to an Condition instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this Tarnsitions's type.
		 *	(i.e. ConditionFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of ConditionFactory introduce *new* Condition parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		condition		A pointer to the condition whose attributes are to be set.
		 *	@param		node			The XML node containing the condition attributes.
		 *	@param		behaveFldr		The path to the behavior file.  If the condition references
		 *								resources in the file system, it should be defined relative
		 *								to the behavior file location.  This is the folder containing
		 *								that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( BFSM::Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;
	};

}	// namespace Aircraft

#endif	// __AIRCRAFT_ACTION_H__
