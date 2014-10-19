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
 *	@file		StateDescrip.h
 *	@brief		The state (and state goal) specification in the behavior configuration file.
 */

#ifndef __STATE_DESCRIP_H__
#define __STATE_DESCRIP_H__

#include "fsmCommon.h"
#include <list>
#include <string>


// forward declarations
class TiXmlElement;

namespace Menge {

	namespace BFSM {
		// forward declarations
		class Action;
		class GoalSelector;
		class VelComponent;
		class VelModifier;
		

		////////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The description of a state  (see State).
		 */
		class StateDescrip {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		name		The name of the state.
			 *	@param		isFinal		Determines if this state is a final state (true) or not (false).
			 */
			StateDescrip( const std::string & name, bool isFinal );

			/*!
			 *	@brief		Destructor
			 */
			~StateDescrip();

			/*!
			 *	@brief		Friend operator for printing the state description to an output stream.
			 *
			 *	@param		out			The output stream.
			 *	@param		state		A state description to write to the stream.
			 *	@returns	The provided output stream.
			 */
			friend Logger & operator<<( Logger & out, const StateDescrip & state );
			
			/*!
			 *	@brief		The name of the state.
			 */
			std::string	_name;

			/*!
			 *	@brief		Dictates whether this state is considered a "final" state (true) or not (false).
			 */
			bool	_isFinal;

			/*!
			 *	@brief		The description of the goal selector used for this state.
			 */
			GoalSelector * _goalSelector;

			/*!
			 *	@brief		The velocity component for this state.
			 */
			VelComponent *	_velComponent;

			/*!
			 *	@brief		The set of actions for this state.
			 */
			std::list< Action * >	 _actions;

			/*!
			 *	@brief		The set of actions for this state.
			 */
			std::vector< VelModifier * >	 _velModifiers;  //TODO: list or vector?
		};

		/*!
		 *	@brief		Parses a TinyXML element containing a state description
		 *
		 *	@param		node			The TinyXML element
		  *	@param		behaveFldr	The folder in which the behavior is defined -- all resources
		 *							are defined relative to this folder.
		 *	@param		states			List of state descriptions.  The new state description
		 *								is appended to the end of this liest.
		 *	@returns	A boolean reporting succes (true) or failure (false).
		 */
		bool parseState( TiXmlElement * node, const std::string & behaveFldr, std::list< StateDescrip * > & states );

	}	// namespace BFSM
}	// namespace Menge

#endif	 // __STATE_DESCRIP_H__