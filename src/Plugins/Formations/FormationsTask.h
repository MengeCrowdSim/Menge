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
 *	@file		FormationsTask.h
 *	@brief		Task used to maintain a single formation.
 */

#ifndef __FORMATIONS_TASK_H__
#define	__FORMATIONS_TASK_H__

#include "Tasks/Task.h"
#include "fsmCommon.h"
#include <string>
#include "Tasks/TaskFactory.h"
#include "tinyxml.h"
#include "FreeFormation.h"
#include "FormationsModifier.h"

using namespace Menge;

namespace Formations {
    /*!
	 *	@brief	Task responsible for updating agent data for maintaining a formation.
	 */
	class FormationsTask : public BFSM::Task {
	public:
		/*!
		 *	@brief		Constructor 
		 *
		 *  @param      form		The formation to be maintained in the task.
		 *
		 */
		FormationsTask( Formations::FormationPtr & form );

		/*!
		 *	@brief		The work performed by the task.
		 *
		 *	@param		fsm		The finite state machine for the task to operate on.
		 *	@throws		A TaskException if there was some non-fatal error
		 *				in execution.  It should be logged.
		 *	@throws		A TaskFatalException if there is a fatal error that
		 *				should arrest execution of the simulation.
		 */
		virtual void doWork( const BFSM::FSM * fsm ) throw( BFSM::TaskException );

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
		virtual bool isEquivalent( const BFSM::Task * task ) const;

		/*!
		 *	@brief   Get the formation represented in this task
		 *
		 *	@returns	The FormationPtr object containing the formation resource
		 */
		const Formations::FormationPtr getFormation() const { return _formation;} 

	protected:
		/*! 
		 *	@brief		The underlying formation data.
		 */
		Formations::FormationPtr _formation; //the formation

	};



}
#endif // __DENSITY_GRID_TASK_H__
