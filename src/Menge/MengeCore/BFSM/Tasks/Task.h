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

#ifndef __TASK_H__
#define __TASK_H__

/*!
 *	@file	Task.h
 *	@brief	Defines the interface for behavior FSM tasks.
 *			actions that need to be taken at each time step.
 */
#include "fsmCommon.h"
#include "Element.h"
#include "MengeException.h"

#include <string>

// forward declaration
class TiXmlElement;

namespace Menge {

	namespace BFSM {

		// FORWARD DECLARATIONS
		class FSM;

		/*!
		 *	@brief		Exception thrown when a task fails at doing its work.
		 *				These exceptions can be logged but should not arrest 
		 *				execution of the simulation.
		 */
		class TaskException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			TaskException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			TaskException( const std::string & s ): MengeException(s) {}

		};
		
		/*!
		 *	@brief		Exception thrown when the task has an error which cannot be
		 *				recovered from.
		 */
		class TaskFatalException : public TaskException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			TaskFatalException() : MengeException(), TaskException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			TaskFatalException( const std::string & s ): MengeException(s), TaskException(), MengeFatalException() {}
		};

		/*!
		 *	@brief	Interface for basic FSM task.
		 *
		 *			Tasks must implement three functions: doWork, toString, and
		 *			isEquivalent.  Tasks' doWork functions are evaluated at the
		 *			beginning of each FSM evaluation per time step.
		 */
		class MENGE_API Task : public Element {
		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~Task(){}

		public:
			/*!
			 *	@brief		The work performed by the task.
			 *
			 *	@param		fsm		The behavior finite state machine on which the task
			 *						is performed.
			 *
			 *	@throws		A TaskException if there was some non-fatal error
			 *				in execution.  It should be logged.
			 *	@throws		A TaskFatalException if there is a fatal error that
			 *				should arrest execution of the simulation.
			 */
			virtual void doWork( const FSM * fsm ) throw( TaskException ) = 0;

			/*!
			 *	@brief		String representation of the task
			 *
			 *	TODO: Determine when this is used.
			 *
			 *	@returns	A string containing task information.
			 */
			virtual std::string toString() const = 0;

			/*!
			 *	@brief		Reports if this task is "equivalent" to the given task.
			 *
			 *				This makes it possible for a task to be redundantly added
			 *				to the fsm without fear of duplication as the equivalent
			 *				duplicates will be culled.
			 *
			 *	@param		task		The task to test against this one.
			 *	@returns	A boolean reporting if the two tasks are equivalent (true)
			 *				or unique (false).
			 */
			virtual bool isEquivalent( const Task * task ) const = 0;
		};	

		/*!
		 *	@brief		Parses a TinyXML element containing a task specification
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new task instance.
		 */
		Task * parseTask( TiXmlElement * node, const std::string & behaveFldr );
	}	// namespace BFSM
}	// namespace Menge
#endif	// __TASK_H__