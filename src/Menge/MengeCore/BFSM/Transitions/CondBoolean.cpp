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

#include "CondBoolean.h"
#include "ConditionDatabase.h"
#include "Logger.h"
#include "BaseAgent.h"
#include "tinyxml.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of Bool2Condition
		///////////////////////////////////////////////////////////////////////////

		Bool2Condition::Bool2Condition(): Condition(), _op1(0x0), _op2(0x0) {
		}

		///////////////////////////////////////////////////////////////////////////

		Bool2Condition::Bool2Condition( const Bool2Condition & cond ) {
			_op1 = cond._op1->copy();
			_op2 = cond._op2->copy();
		}

		///////////////////////////////////////////////////////////////////////////

		Bool2Condition::~Bool2Condition() {
			if ( _op1 ) _op1->destroy();
			if ( _op2 ) _op2->destroy();
		}

		///////////////////////////////////////////////////////////////////////////

		void Bool2Condition::onEnter( Agents::BaseAgent * agent ) {
			_op1->onEnter( agent );
			_op2->onEnter( agent );
		}

		///////////////////////////////////////////////////////////////////////////

		void Bool2Condition::onLeave( Agents::BaseAgent * agent ) {
			_op1->onLeave( agent );
			_op2->onLeave( agent );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of Bool2CondFactory
		///////////////////////////////////////////////////////////////////////////

		bool Bool2CondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			Bool2Condition * bCond = dynamic_cast< Bool2Condition * >( condition );
			assert( bCond != 0x0 && "Trying to set the properties of a binary boolean condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( bCond, node, behaveFldr ) ) return false;

			// There should be two xml tags for conditions.
			int childCount = 0;
			for ( TiXmlElement * child = node->FirstChildElement(); child; child = child->NextSiblingElement() ) {
				if ( child->ValueStr() == "Condition" ) {
					Condition * condition = ConditionDB::getInstance( child, behaveFldr );
					if ( condition == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to parse the child condition of a binary boolean condition on line " << child->Row() << ": " << child->ValueStr() << ".";
						return false;
					}
					if ( childCount == 0 ) {
						bCond->_op1 = condition;
						++childCount;
					} else if ( childCount == 1 ) {
						bCond->_op2 = condition;
						++childCount;
					} else {
						logger << Logger::ERR_MSG << "Too many child conditions for a binary boolean condition on line " << child->Row() << ": " << child->ValueStr() << ".";
						return false;
					}
				} else {
					logger << Logger::ERR_MSG << "Unrecognized child tag of a binary boolean condition on line " << child->Row() << ": " << child->ValueStr() << ".";
					return false;
				}
			}
			if ( childCount != 2 ) {
				logger << Logger::ERR_MSG << "The binary boolean condition on line " << node->Row() << " requires TWO child conditions.";
				return false;
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of AndCondition
		///////////////////////////////////////////////////////////////////////////

		AndCondition::AndCondition() : Bool2Condition() {}

		///////////////////////////////////////////////////////////////////////////

		AndCondition::AndCondition( const AndCondition & cond ) : Bool2Condition(cond) {
		}

		///////////////////////////////////////////////////////////////////////////

		bool AndCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			// This relies on the compiler to only evaluate the second condition if the
			//	first is true.
			return _op1->conditionMet( agent, goal ) && _op2->conditionMet( agent, goal );
		}
		
		///////////////////////////////////////////////////////////////////////////

		Condition * AndCondition::copy() {
			return new AndCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of OrCondition
		///////////////////////////////////////////////////////////////////////////

		OrCondition::OrCondition() : Bool2Condition() {}

		///////////////////////////////////////////////////////////////////////////

		OrCondition::OrCondition( const OrCondition & cond ) : Bool2Condition(cond) {
		}

		///////////////////////////////////////////////////////////////////////////

		bool OrCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			// This relies on the compiler to only evaluate the second condition if the
			//	first is false.
			return _op1->conditionMet( agent, goal ) || _op2->conditionMet( agent, goal );
		}
		
		///////////////////////////////////////////////////////////////////////////

		Condition * OrCondition::copy() {
			return new OrCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of NotCondition
		///////////////////////////////////////////////////////////////////////////

		NotCondition::NotCondition(): Condition(), _op(0x0) {
		}

		///////////////////////////////////////////////////////////////////////////

		NotCondition::NotCondition( const NotCondition & cond ) {
			_op = cond._op->copy();
		}

		///////////////////////////////////////////////////////////////////////////

		NotCondition::~NotCondition() {
			if ( _op ) _op->destroy();
		}

		///////////////////////////////////////////////////////////////////////////

		void NotCondition::onEnter( Agents::BaseAgent * agent ) {
			_op->onEnter( agent );
		}

		///////////////////////////////////////////////////////////////////////////

		void NotCondition::onLeave( Agents::BaseAgent * agent ) {
			_op->onLeave( agent );
		}

		///////////////////////////////////////////////////////////////////////////

		bool NotCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			return ! _op->conditionMet( agent, goal );
		}

		///////////////////////////////////////////////////////////////////////////

		Condition * NotCondition::copy() {
			return new NotCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of NotCondFactory
		///////////////////////////////////////////////////////////////////////////

		bool NotCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			NotCondition * bCond = dynamic_cast< NotCondition * >( condition );
			assert( bCond != 0x0 && "Trying to set the properties of a NOT boolean condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( bCond, node, behaveFldr ) ) return false;

			// There should be two xml tags for conditions.
			TiXmlElement * child = node->FirstChildElement();
			if ( child == 0x0 ) {
				logger << Logger::ERR_MSG << "The NOT condition on line " << node->Row() << " requires one child condition -- none provided.";
				return false;
			} else if ( child->NextSiblingElement() != 0x0 ) {
				logger << Logger::ERR_MSG << "The NOT condition on line " << node->Row() << " has too many children elements - it should get a single condition element.";
				return false;
			} else {
				if ( child->ValueStr() == "Condition" ) {
					Condition * condition = ConditionDB::getInstance( child, behaveFldr );
					if ( condition == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to parse the child condition of the NOT condition on line " << child->Row() << ": " << child->ValueStr() << ".";
						return false;
					} else {
						bCond->_op = condition;
					}
				} else {
					logger << Logger::ERR_MSG << "The NOT condition on line " << node->Row() << " requires a single child condition.  Found a " << child->ValueStr() << " on line " << child->Row() << ".";
					return false;
				}
			}
			
			return true;
		}
	}	// namespace BFSM 
}	// namespace Menge