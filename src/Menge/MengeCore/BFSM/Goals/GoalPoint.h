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
 *	@file		GoalPoint.h
 *	@brief		The definition of a BFSM point goal. 
 */

#ifndef __GOAL_POINT_H__
#define __GOAL_POINT_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Goals/GoalFactory.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		A simple point goal.  The goal is trivially this point.
		 */
		class MENGE_API PointGoal : public Goal {
		public:
			/*! Default constructor */
			PointGoal();

			/*!
			 *	@brief		Full constructor.
			 *				
			 *	@param		p		The position of the goal.
			 */
			PointGoal(const Vector2 & p);

			/*!
			 *	@brief		Full constructor - component-wise.
			 *
			 *	@param		x		The x-position of the goal.
			 *	@param		y		The x-position of the goal.
			 */
			PointGoal( float x, float y );

			/*!
			 *	@brief		Used by the plugin system to know what artifacts to associate with
			 *				agents of this type.  Every sub-class of must return a globally
			 *				unique value if it should be associated with unique artifacts.
			 */
			virtual std::string getStringId() const { return NAME; }

			/*! The unique identifier used to register this type with run-time components. */
			static const std::string NAME;
		};

		/*!
		 *	@brief		Factory for the PointGoal.
		 */
		class MENGE_API PointGoalFactory : public GoalFactory { 
		public:

			/*!
			 *	@brief		The name of the goal type.
			 *
			 *	The goal's name must be unique among all registered goals.
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the unique goal name.
			 */
			virtual const char * name() const { return PointGoal::NAME.c_str(); }

			/*!
			 *	@brief		A description of the goal.
			 *
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the goal description.
			 */
			virtual const char * description() const {
				return "An agent goal consisting of a single point in two-dimensional space";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal.
			 *
			 *	@returns		A pointer to a newly instantiated Goal class.
			 */
			Goal * instance() const { return new PointGoal(); }	

			/*!
			 *	@brief		Given a pointer to a Goal instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	@param		goal		A pointer to the goal whose attributes are to be set.
			 *	@param		node		The XML node containing the action attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the action references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder
			 *							containing that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( Goal * goal, TiXmlElement * node, 
									 const std::string & behaveFldr ) const;
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_POINT_H__
