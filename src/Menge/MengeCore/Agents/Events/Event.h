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
 *	@file		Event.h
 *	@brief		The definition of the Menge Event.
 */

#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <vector>

// forward declaration
class TiXmlElement;

namespace Menge {
	// forward declaration
	class EventTrigger;
	class EventResponse;
	class Event;
	class EventSystem;
	
	///////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Parses the xml specification of an event and returns a parsed
	 *				event.
	 *
	 *	The event instance will still need to be finalized.
	 *
	 *	@param		node		The tiny xml node containing the event specification.
	 *	@param		specFldr	The path to the specification file.  All external
	 *							resource references in the spec file are defined relative
	 *							to this folder.
	 *	@returns	A new instance of an Event.  Null if there is a parsing error.
	 *				The caller assumes responsibility to delete the instance.
	 */
	Event * parseEvent( TiXmlElement * node, const std::string & specFldr );

	///////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The definition of a Menge event.  
	 *
	 *	An event is defined by two components: a trigger, and a list of event *responses*.  
	 *	The trigger defines the circumstances which cause the event to "fire".  
	 *	The event can have multiple respones registered.  When the event fires,
	 *	each event response is triggered to modify the simulation.
	 */
	class Event {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		name		The name of the event.
		 */
		Event( const std::string & name );

		/*!
		 *	@brief		Destructor.
		 */
		~Event();

		/*!
		 *	@brief		Allows the event to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventFatalException if the event system has conservative
		 *				configuration and there is a problem in the event specification.
		 */
		void finalize();

		/*!
		 *	@brief		Evaluates the event (and triggers handlers as necessary).
		 */
		void evaluate();

		friend Event * parseEvent( TiXmlElement * node, const std::string & specFldr );
		friend class EventSystem;
	protected:
		/*!
		 *	@brief		The name of the event.
		 */
		std::string	_name;

		/*!
		 *	@brief		The trigger for the event.
		 */
		EventTrigger * _trigger;

		/*!
		 *	@brief		The effects registered to this event.
		 */
		std::vector< EventResponse * > _responses;
	};
}	// namespace Menge

#endif // __EVENT_H__