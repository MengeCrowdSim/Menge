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

#include "Transitions/Target.h"
#include "Logger.h"
#include <iostream>
#include <string>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SingleTarget
		/////////////////////////////////////////////////////////////////////

		TransitionTarget::TransitionTarget() : Element() {
		}

		/////////////////////////////////////////////////////////////////////

		TransitionTarget::TransitionTarget( const TransitionTarget & tgt ){}

		/////////////////////////////////////////////////////////////////////

		TransitionTarget::~TransitionTarget() {
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SingleTarget
		/////////////////////////////////////////////////////////////////////

		SingleTarget::SingleTarget():TransitionTarget(),_nextName(""), _next(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		SingleTarget::SingleTarget( const SingleTarget & tgt ):TransitionTarget(tgt),_nextName(tgt._nextName), _next(tgt._next) {
		}

		/////////////////////////////////////////////////////////////////////

		SingleTarget::SingleTarget( const std::string & toName ):_nextName(toName), _next(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		bool SingleTarget::connectStates( std::map< std::string, State * > & stateMap ) {
			if ( stateMap.find( _nextName ) == stateMap.end() ) {
				logger << Logger::ERR_MSG << "SingleTarget with invalid to node name: " << _nextName << "\n";
				return false;
			}
			_next = stateMap[ _nextName ];
			return true;
		}

		/////////////////////////////////////////////////////////////////////

		TransitionTarget * SingleTarget::copy() {
			return new SingleTarget( *this );
		}

	}	// namespace BFSM
}	// namespace Menge