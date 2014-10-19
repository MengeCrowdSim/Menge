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

#include "CondSpace.h"
#include "BaseAgent.h"
#include "Logger.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of SpaceCondition
		///////////////////////////////////////////////////////////////////////////

		SpaceCondition::SpaceCondition(): Condition(),_outsideActive(false) {
		}

		///////////////////////////////////////////////////////////////////////////

		SpaceCondition::SpaceCondition( const SpaceCondition & cond ):Condition(cond), _outsideActive(cond._outsideActive) {
		}

		///////////////////////////////////////////////////////////////////////////

		SpaceCondition::~SpaceCondition() {
		}

		///////////////////////////////////////////////////////////////////////////

		bool SpaceCondition::conditionMet( Agents::BaseAgent * agent, const Goal * goal ) {
			bool inside = containsPoint( agent->_pos );
			return inside ^ _outsideActive;	// the xor flips the shape's result as necessary
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of SpaceCondFactory
		/////////////////////////////////////////////////////////////////////

		SpaceCondFactory::SpaceCondFactory() : ConditionFactory() {
			_insideID = _attrSet.addBoolAttribute( "inside", true /*required*/ );
		}

		///////////////////////////////////////////////////////////////////////////

		bool SpaceCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			SpaceCondition * sCond = dynamic_cast< SpaceCondition * >( condition );
			assert( sCond != 0x0 && "Trying to set the properties of a space condition on an incompatible object" );

			if ( !ConditionFactory::setFromXML( condition, node, behaveFldr ) ) return false;

			sCond->_outsideActive = !_attrSet.getBool( _insideID );
			return true;
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of CircleCondition
		/////////////////////////////////////////////////////////////////////

		CircleCondition::CircleCondition():SpaceCondition(),CircleShape() {
		}

		/////////////////////////////////////////////////////////////////////

		CircleCondition::CircleCondition( const CircleCondition & cond ):SpaceCondition(cond),CircleShape() {
			_center = cond._center;
			_radSqd = cond._radSqd;
		}

		/////////////////////////////////////////////////////////////////////

		Condition * CircleCondition::copy() {
			return new CircleCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of CircleCondFactory
		/////////////////////////////////////////////////////////////////////

		CircleCondFactory::CircleCondFactory() : SpaceCondFactory() {
			_centerXID = _attrSet.addFloatAttribute( "center_x", true /*required*/ );
			_centerYID = _attrSet.addFloatAttribute( "center_y", true /*required*/ );
			_radiusID = _attrSet.addFloatAttribute( "radius", true /*required*/ );
		}

		///////////////////////////////////////////////////////////////////////////

		bool CircleCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			CircleCondition * cCond = dynamic_cast< CircleCondition * >( condition );
			assert( cCond != 0x0 && "Trying to set the properties of a circle condition on an incompatible object" );

			if ( !SpaceCondFactory::setFromXML( cCond, node, behaveFldr ) ) return false;

			cCond->set( Vector2( _attrSet.getFloat( _centerXID ), _attrSet.getFloat( _centerYID ) ), 
						_attrSet.getFloat( _radiusID ) );
			return true;		
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBCondition
		/////////////////////////////////////////////////////////////////////

		AABBCondition::AABBCondition():SpaceCondition(),AABBShape() {
		}

		/////////////////////////////////////////////////////////////////////

		AABBCondition::AABBCondition( const AABBCondition & cond ):SpaceCondition(cond),AABBShape(cond._minPt,cond._maxPt) {
		}

		/////////////////////////////////////////////////////////////////////

		Condition * AABBCondition::copy() {
			return new AABBCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of AABBCondFactory
		/////////////////////////////////////////////////////////////////////

		AABBCondFactory::AABBCondFactory() : SpaceCondFactory() {
			_minXID = _attrSet.addFloatAttribute( "min_x", true /*required*/ );
			_minYID = _attrSet.addFloatAttribute( "min_y", true /*required*/ );
			_maxXID = _attrSet.addFloatAttribute( "max_x", true /*required*/ );
			_maxYID = _attrSet.addFloatAttribute( "max_y", true /*required*/ );
		}

		///////////////////////////////////////////////////////////////////////////

		bool AABBCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			AABBCondition * cond = dynamic_cast< AABBCondition * >( condition );
			assert( cond != 0x0 && "Trying to set the properties of a AABB condition on an incompatible object" );

			if ( !SpaceCondFactory::setFromXML( cond, node, behaveFldr ) ) return false;

			cond->set( Vector2( _attrSet.getFloat( _minXID ), _attrSet.getFloat( _minYID ) ),
					   Vector2( _attrSet.getFloat( _maxXID ), _attrSet.getFloat( _maxYID ) ) 
					   );
			return true;
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBCondition
		/////////////////////////////////////////////////////////////////////

		OBBCondition::OBBCondition():SpaceCondition(),OBBShape() {
		}

		/////////////////////////////////////////////////////////////////////

		OBBCondition::OBBCondition( const OBBCondition & cond ):SpaceCondition(cond),OBBShape() {
			_pivot = cond._pivot;
			_size = cond._size;
			_halfSize = cond._halfSize;
			_cosTheta = cond._cosTheta;
			_sinTheta = cond._sinTheta;
		}

		/////////////////////////////////////////////////////////////////////

		Condition * OBBCondition::copy() {
			return new OBBCondition( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBCondFactory
		/////////////////////////////////////////////////////////////////////

		OBBCondFactory::OBBCondFactory() : SpaceCondFactory() {
			_pivotXID = _attrSet.addFloatAttribute( "pivot_x", true /*required*/ );
			_pivotYID = _attrSet.addFloatAttribute( "pivot_y", true /*required*/ );
			_widthID = _attrSet.addFloatAttribute( "width", true /*required*/ );
			_heightID = _attrSet.addFloatAttribute( "height", true /*required*/ );
			_angleID = _attrSet.addFloatAttribute( "angle", true /*required*/  );
		}

		///////////////////////////////////////////////////////////////////////////

		bool OBBCondFactory::setFromXML( Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
			OBBCondition * cond = dynamic_cast< OBBCondition * >( condition );
			assert( cond != 0x0 && "Trying to set the properties of a OBB condition on an incompatible object" );

			if ( !SpaceCondFactory::setFromXML( cond, node, behaveFldr ) ) return false;
			
			cond->set( Vector2( _attrSet.getFloat( _pivotXID ), _attrSet.getFloat( _pivotYID ) ),
					   _attrSet.getFloat( _widthID ), _attrSet.getFloat( _heightID ),
					   _attrSet.getFloat( _angleID ) * DEG_TO_RAD
					   );
			return true;
		}

	}	// namespace BFSM 
}	// namespace Menge