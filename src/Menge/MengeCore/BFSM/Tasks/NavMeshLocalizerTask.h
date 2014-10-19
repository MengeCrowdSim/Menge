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

#ifndef __NAV_MESH_LOCALIZER_TASK_H__
#define	__NAV_MESH_LOCALIZER_TASK_H__

/*!
 *	@file	NavMeshLocalizerTask.h
 *	@brief	A task based on the NavMeshLocalizer so that it updates 
 *			its tracked agent positions at every FSM time step.
 */

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Resource.h"
#include "Task.h"

namespace Menge {

	// forward declaration
	class NavMeshLocalizer;

	/*!
	 *  @brief forward declaration. See NavMeshLocalizer for more details
	 */
	typedef ResourcePtr< NavMeshLocalizer > NavMeshLocalizerPtr;

	namespace BFSM {
		/*!
		 *	@brief	Sub-class of NavMeshLocalizer that can be run as an FSM task
		 */
		class MENGE_API NavMeshLocalizerTask : public Task {
		public:
			/*!
			 *	@brief		Constructor from a localizer.
			 *
			 *	@param		navMeshName		The name of the navigation mesh which the
			 *								task depends on.
			 *	@param		usePlanner		Indicates if the localizer should use a planner
			 *								(true) or not (false).
			 */
			NavMeshLocalizerTask( const std::string & navMeshName, bool usePlanner );

			/*!
			 *	@brief		The work performed by the task.
			 *
			 *	@param		fsm		The finite state machine for the task to operate on.
			 *	@throws		A TaskException if there was some non-fatal error
			 *				in execution.  It should be logged.
			 *	@throws		A TaskFatalException if there is a fatal error that
			 *				should arrest execution of the simulation.
			 */
			virtual void doWork( const FSM * fsm ) throw( TaskException );

			/*!
			 *	@brief		String representation of the task
			 *
			 *	@returns	A string containing task information.
			 */
			virtual std::string toString() const;

			/*!
			 *	@brief		Reports if this task is "equivalent" to the given task.
			 *				This makes it possible for a task to be redundantly added
			 *				to the fsm without fear of duplication as the equivalent
			 *				duplicates will be culled.
			 *
			 *	@param		task		The task to test against this one.
			 *	@returns	A boolean reporting if the two tasks are equivalent (true)
			 *				or unique (false).
			 */
			virtual bool isEquivalent( const Task * task ) const;

		protected:
			/*!
			 *	@brief		The localizer used by this task.
			 */
			NavMeshLocalizerPtr _localizer;
		};
	}	 // namespace BFSM 
}	// namespace Menge
#endif	// __NAV_MESH_LOCALIZER_TASK_H__
