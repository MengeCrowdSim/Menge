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
 *	@file		CondSpace.h
 *	@brief		The definition of space-based conditions.  I.e. those conditions which are
 *				triggered based on an agent entering or leaving a particular space.
 */

#ifndef __COND_SPACE_H__
#define __COND_SPACE_H__

#include "CoreConfig.h"
#include "Transitions/Condition.h"
#include "Transitions/ConditionFactory.h"
#include "Math/Geometry2D.h"
#include "fsmCommon.h"

namespace Menge {

	namespace BFSM {
		//forward declartions
		class SpaceCondFactory;

		/*!
		 *	@brief		The abstact base class for spatial conditions.  This includes all
		 *				conditions which define a geometric region and then determines
		 *				that the condition is met by either entering or exiting the region.
		 */
		class MENGE_API SpaceCondition : public Condition {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			SpaceCondition();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			SpaceCondition( const SpaceCondition & cond );

		protected:
			/*!
			 *	@brief		Constructor.
			 */
			~SpaceCondition();

		public:
			/*!
			 *	@brief		The AutoCondition is always met -- it is a tautology.
			 *
			 *	@param		agent		The agent to test the transition for.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@returns	True if the condition has been met, false otherwise.
			 */
			virtual bool conditionMet( Agents::BaseAgent * agent, const Goal * goal );

			friend class SpaceCondFactory;
		protected:
			/*!
			 *	@brief		Determines if a point is inside the test region.
			 *
			 *	Purely a virtual function.  Derived transitions must override this function.
			 *
			 *	@param		pt		The point to test w.r.t. the transition region.
			 *	@returns	True if the transition region contains the given point.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const = 0;

			/*!
			 *	@brief		Determines if the transition happens when the agent is 
			 *				outside (true) or inside (false).
			 */
			bool	_outsideActive;
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for creating the SpaceCondition
		 */
		class MENGE_API SpaceCondFactory : public ConditionFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			SpaceCondFactory();

		protected:
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
			virtual bool setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "inside" bool attribute.
			 */
			size_t	_insideID;
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A transition based on spatial relationship with a circle.
		 *
		 *	The agent will transition when it reaches the relationship (inside or outside)
		 *	to the defined circle.
		 */
		class MENGE_API CircleCondition : public SpaceCondition, public CircleShape {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			CircleCondition();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			CircleCondition( const CircleCondition & cond );

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy();

		protected:
			/*!
			 *	@brief		Determines if a point is inside the test region.
			 *
			 *	@param		pt		The point to test w.r.t. the transition region.
			 *	@returns	True if the transition region contains the given point.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const { return CircleShape::containsPoint( pt ); }
		};

		/*!
		 *	@brief		The factory for creating the SpaceCondition
		 */
		class MENGE_API CircleCondFactory : public SpaceCondFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			CircleCondFactory();

			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "circle"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The circle condition.  It becomes active when an agent achieves "\
					"a particular relationship (inside/outside) with a static circle in the " \
					"environment.";
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
			virtual Condition * instance() const { return new CircleCondition(); }

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
			virtual bool setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "center_x" float attribute.
			 */
			size_t	_centerXID;
		
			/*!
			 *	@brief		The identifier for the "center_y" float attribute.
			 */
			size_t	_centerYID;
		
			/*!
			 *	@brief		The identifier for the "radius" float attribute.
			 */
			size_t	_radiusID;
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A transition based on spatial relationship with an axis-aligned bounding box (AABB).
		 *
		 *	The agent will transition when it reaches the relationship (inside or outside)
		 *	to the defined axis-aligned bounding box (AABB).
		 */
		class MENGE_API AABBCondition : public SpaceCondition, public AABBShape {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			AABBCondition();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			AABBCondition( const AABBCondition & cond );

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy();
		protected:
			/*!
			 *	@brief		Determines if a point is inside the test region.
			 *
			 *	@param		pt		The point to test w.r.t. the transition region.
			 *	@returns	True if the transition region contains the given point.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const { return AABBShape::containsPoint( pt ); }
		};

		/*!
		 *	@brief		The factory for creating the AABBCondition
		 */
		class MENGE_API AABBCondFactory : public SpaceCondFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			AABBCondFactory();

			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "AABB"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The axis-aligned bounding box (AABB) condition.  It becomes active when an agent achieves "\
					"a particular relationship (inside/outside) with a static axis-aligned bounding box in the " \
					"environment.";
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
			virtual Condition * instance() const { return new AABBCondition(); }

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
			virtual bool setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "min_x" float attribute.
			 */
			size_t	_minXID;
		
			/*!
			 *	@brief		The identifier for the "min_y" float attribute.
			 */
			size_t	_minYID;
		
			/*!
			 *	@brief		The identifier for the "max_x" float attribute.
			 */
			size_t	_maxXID;
		
			/*!
			 *	@brief		The identifier for the "max_y" float attribute.
			 */
			size_t	_maxYID;
		};

		///////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A transition based on spatial relationship with an axis-aligned bounding box (AABB).
		 *
		 *	The agent will transition when it reaches the relationship (inside or outside)
		 *	to the defined axis-aligned bounding box (AABB).
		 */
		class MENGE_API OBBCondition : public SpaceCondition, public OBBShape {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			OBBCondition();

			/*!
			 *	@brief		Copy Constructor.
			 *
			 *	@param		cond		The condition to copy from.
			 */
			OBBCondition( const OBBCondition & cond );

			/*!
			 *	@brief		Create a copy of this condition.
			 *
			 *	It is the responsibility of the caller to delete the object.
			 *
			 *	@returns:	A "deep copy" of this condition - such that there is no shared
			 *				objects between this and its copy.
			 */
			virtual Condition * copy();
		protected:
			/*!
			 *	@brief		Determines if a point is inside the test region.
			 *
			 *	@param		pt		The point to test w.r.t. the transition region.
			 *	@returns	True if the transition region contains the given point.
			 */
			virtual bool containsPoint( const Vector2 & pt ) const { return OBBShape::containsPoint( pt ); }
		};

		/*!
		 *	@brief		The factory for creating the OBBCondition
		 */
		class MENGE_API OBBCondFactory : public SpaceCondFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			OBBCondFactory();

			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "OBB"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "The oriented bounding box (OBB) condition.  It becomes active when an agent achieves "\
					"a particular relationship (inside/outside) with a static oriented bounding box in the " \
					"environment.";
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
			virtual Condition * instance() const { return new OBBCondition(); }

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
			virtual bool setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "pivot_x" float attribute.
			 */
			size_t	_pivotXID;
		
			/*!
			 *	@brief		The identifier for the "pivot_y" float attribute.
			 */
			size_t	_pivotYID;
		
			/*!
			 *	@brief		The identifier for the "width" float attribute.
			 */
			size_t	_widthID;
		
			/*!
			 *	@brief		The identifier for the "height" float attribute.
			 */
			size_t	_heightID;
		
			/*!
			 *	@brief		The identifier for the "angle" float attribute.
			 */
			size_t	_angleID;
		};

	}	// namespace BFSM
}

#endif	// __COND_SPACE_H__