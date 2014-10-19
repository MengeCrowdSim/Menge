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

#include "GoalSelectors/GoalSelectorShared.h"
#include "tinyxml.h"
#include <cassert>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SharedGoalSelector
		/////////////////////////////////////////////////////////////////////

		SharedGoalSelector::SharedGoalSelector():GoalSelector(), _stateName(""), _lineNo(0) {}

		/////////////////////////////////////////////////////////////////////
		
		Goal * SharedGoalSelector::getGoal( const Agents::BaseAgent * agent ) const {
			logger << Logger::ERR_MSG << "SharedGoalSelector was left in place to create an agent goal!";
			throw GoalSelectorException();
			return 0x0;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SharedGoalSelectorFactory
		/////////////////////////////////////////////////////////////////////

		bool SharedGoalSelectorFactory::setFromXML( GoalSelector * selector, TiXmlElement * node, const std::string & behaveFldr ) const {
			SharedGoalSelector * sgs = dynamic_cast< SharedGoalSelector * >( selector );
			assert( sgs != 0x0 && "Trying to set attributes of a shared goal selector on an incompatible object" );
			// NOTE: This does NOT call the parent's setFromXML because it is a special-purpose stub
			//		It only sub-classes the GoalSelectorFactory so it can sit in the same factory database.

			// get the file name
			const char * sNameCStr = node->Attribute( "state_name" );
			if ( sNameCStr ) {
				sgs->_stateName = sNameCStr;
				sgs->_lineNo = node->Row();
			} else {
				logger << Logger::ERR_MSG << "Shared goal selector defined on line " << node->Row() << " is missing the \"state_name\" parameter.";
				return false;
			}

			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge