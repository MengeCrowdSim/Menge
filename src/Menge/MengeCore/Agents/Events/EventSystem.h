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
 *	@file		EventSystem.h
 *	@brief		The definition of the core event system.
 */

#ifndef __EVENT_SYSTEM_H__
#define __EVENT_SYSTEM_H__

#include <vector>
#include "mengeCommon.h"

// forward declaration
class TiXmlElement;

namespace Menge {
	// forward declaration

	class Event;
	class EventResponse;
	class EventEffect;
	class EventTarget;
	Event * parseEvent( TiXmlElement * node, const std::string & specFldr );

	/*!
	 *	@brief		The main event engine -- causes event triggers to be evaluated
	 *				and effects to be applied.
	 */
	class EventSystem {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		EventSystem();

		/*!
		 *	@brief		Destructor
		 */
		~EventSystem();

		/*!
		 *	@brief		Adds an event to the system.  
		 *
		 *	The system becomes the owner of the event and will take responsibility
		 *	for deleting it.
		 *
		 *	@param		evt		The event to add to the system.
		 */
		void addEvent( Event * evt ) { _events.push_back( evt ); }

		/*!
		 *	@brief		Evaluates the registered events
		 */
		void evaluateEvents();

		/*!
		 *	@brief		Finalize the event system.
		 */
		void finalize();

		/*!
		 *	@brief		Parses events from an "Events" tag.
		 *
		 *	@param		node		The node containing the event system description.
		 *	@param		behaveFldr	The folder containing the behavior specification.
		 *							all event paths are defined relative to this folder.
		 *	@returns	True if parsing was successful, false otherwise.
		 */
		bool parseEvents( TiXmlElement * node, const std::string & behaveFldr );

		/*!
		 *	@brief		Causes an event exception to be thrown based on the
		 *				the system's tolerance for event configuration errors.
		 *
		 *	@param		msg			The event associated with the problem.
		 *	@throws		EventException if the system is *not* conservative; i.e.,
		 *				configuration errors will be noted, but simulation will
		 *				continue.
		 *	@throws		EventFatalException if the system *is* conservative.
		 */
		static void finalizeException( const std::string msg );

		/*!
		 *	@brief		Global variable to indicate how event configuration errors
		 *				should be handled.
		 *
		 *	If true, event configuration errors result in fatal exceptions, if false
		 *	they are merely warnings.
		 */
		static bool CONSERVATIVE_SETUP;

		friend class Event;
		friend class EventEffect;
		friend class EventTarget;
		friend class EventResponse;
		friend Event * parseEvent( TiXmlElement * node, const std::string & specFldr );

	protected:
		/*!
		 *	@brief		The events to process.
		 */
		std::vector< Event * > _events;

		/*!
		 *	@brief		A mapping from target names to targets.
		 */
		HASH_MAP< std::string, EventTarget * >	_targets;

		/*!
		 *	@brief		A mapping from effect names to effects.
		 */
		HASH_MAP< std::string, EventEffect * > _effects;
	};
}

#endif // __EVENT_SYSTEM_H__