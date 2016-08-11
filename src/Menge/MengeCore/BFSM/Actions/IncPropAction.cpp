

#include "Actions/IncPropAction.h"
#include "BaseAgent.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of IncPropAction
		/////////////////////////////////////////////////////////////////////

		IncPropAction::IncPropAction():Action(),_increment(0.0f) {
		}

		/////////////////////////////////////////////////////////////////////

		IncPropAction::~IncPropAction() {

		}

		/////////////////////////////////////////////////////////////////////

		void IncPropAction::onEnter( Agents::BaseAgent * agent ) {
			if(agent->_radius<20)//TODO replace with property spec from xml
				agent->_radius = agent->_radius+this->_increment;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of IncPropActFactory
		/////////////////////////////////////////////////////////////////////

		IncPropActFactory::IncPropActFactory():ActionFactory() {
			_IncrementID = _attrSet.addFloatAttribute("increment", true, 0.0f);
		}

		/////////////////////////////////////////////////////////////////////

		Action * IncPropActFactory::instance() const {
			return new IncPropAction();
		}

		/////////////////////////////////////////////////////////////////////

		bool IncPropActFactory::setFromXML( Action * action, TiXmlElement * node, const std::string & behaveFldr ) const {
			IncPropAction * IncAction = dynamic_cast< IncPropAction * >( action );
			assert( action != 0x0 && "Trying to set Inc action properties on an incompatible object" );

			if ( ! ActionFactory::setFromXML( action, node, behaveFldr ) ) return false;
			// Override undoing the growing regardless of what was set in the XML
			IncAction->_undoOnExit = false;
			IncAction->_increment = _attrSet.getFloat(_IncrementID);
			return true;
		}

	}	// namespace BFSM
}	// namespace Menge
