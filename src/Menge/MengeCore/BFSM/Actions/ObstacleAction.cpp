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

#include "Actions/ObstacleAction.h"
#include "BaseAgent.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ObstacleAction
		/////////////////////////////////////////////////////////////////////

		ObstacleAction::ObstacleAction():Action(), _setOperand(0),_originalMap() {
		}

		/////////////////////////////////////////////////////////////////////

		ObstacleAction::~ObstacleAction() {
			// Is this delete safe?  This may require a destroy method if it is
			//	instantiated in MengeCore and used in external dll
			_originalMap.clear();
		}

		/////////////////////////////////////////////////////////////////////

		void ObstacleAction::onEnter( Agents::BaseAgent * agent ) {
			_lock.lock();
			if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_obstacleSet;
			agent->_obstacleSet = newValue( agent->_obstacleSet );
			_lock.release();
		}

		/////////////////////////////////////////////////////////////////////

		void ObstacleAction::leaveAction( Agents::BaseAgent * agent ) {
			_lock.lock();
			std::map< size_t, size_t >::iterator itr = _originalMap.begin();
			assert( itr != _originalMap.end() && "Trying to find an original value for an agent whose value was not cached" );
			agent->_obstacleSet = itr->second;
			_originalMap.erase( itr );
			_lock.release();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ObstacleActFactory
		/////////////////////////////////////////////////////////////////////

		ObstacleActFactory::ObstacleActFactory():ActionFactory() {
			_operandID = _attrSet.addSizeTAttribute( "operand", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool ObstacleActFactory::setFromXML( Action * action, TiXmlElement * node, const std::string & behaveFldr ) const {
			ObstacleAction * oAction = dynamic_cast< ObstacleAction * >( action );
			assert( oAction != 0x0 && "Trying to set obstacle set action properties on an incompatible object" );
			
			if ( ! ActionFactory::setFromXML( action, node, behaveFldr ) ) return false;
			oAction->_setOperand = _attrSet.getSizeT( _operandID );

			return true;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of RemoveObstacleSetAction
		/////////////////////////////////////////////////////////////////////

		size_t RemoveObstacleSetAction::newValue( size_t value ) {
			return value & (~_setOperand);
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of AddObstacleSetAction
		/////////////////////////////////////////////////////////////////////

		size_t AddObstacleSetAction::newValue( size_t value ) {
			return value | _setOperand;
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SetObstacleSetAction
		/////////////////////////////////////////////////////////////////////

		size_t SetObstacleSetAction::newValue( size_t value ) {
			return _setOperand;
		}
		
	}	// namespace BFSM
}	// namespace Menge