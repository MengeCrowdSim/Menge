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

#include "VelocityComponents/VelCompConst.h"
#include "BaseAgent.h"

#include <sstream>
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstVelComponent
		/////////////////////////////////////////////////////////////////////

		ConstVelComponent::ConstVelComponent(): VelComponent() {
			setVelocity( Vector2( 1.f, 0.f ) );
		}

		/////////////////////////////////////////////////////////////////////

		ConstVelComponent::ConstVelComponent( const Vector2 & vel ): VelComponent() {
				setVelocity( vel );
			}

		/////////////////////////////////////////////////////////////////////

		void ConstVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			pVel.setSingle( _dir );
			pVel.setSpeed( _speed );
			pVel.setTarget( _dir * ( _speed * 5.f ) + agent->_pos );
		}

		/////////////////////////////////////////////////////////////////////

		void ConstVelComponent::setVelocity( const Vector2 & velocity ) {
			_dir.set( norm( velocity ) );
			_speed = abs( velocity );
		}

		/////////////////////////////////////////////////////////////////////

		VelCompContext * ConstVelComponent::getContext() { 
			return new ConstVCContext( this ); 
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstVCContext
		/////////////////////////////////////////////////////////////////////

		ConstVCContext::ConstVCContext( ConstVelComponent * vc ):VelCompContext(),_vc(vc) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string ConstVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Const: " << _vc->getConstVelocity();
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void ConstVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			// draw preferred velocity
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos );
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstVCFactory
		/////////////////////////////////////////////////////////////////////

		ConstVCFactory::ConstVCFactory() : VelCompFactory() {
			_xID = _attrSet.addFloatAttribute( "x", true /*required*/ );
			_yID = _attrSet.addFloatAttribute( "y", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool ConstVCFactory::setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const {
			ConstVelComponent * cvc = dynamic_cast< ConstVelComponent * >( vc );
			assert( cvc != 0x0 && "Trying to set attributes of a const velocity component on an incompatible object" );

			if ( ! VelCompFactory::setFromXML( vc, node, behaveFldr ) ) return false;

			cvc->setVelocity( Vector2( _attrSet.getFloat( _xID ), _attrSet.getFloat( _yID ) ) );
			return true;
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstVelDirComponent
		/////////////////////////////////////////////////////////////////////

		ConstVelDirComponent::ConstVelDirComponent(): VelComponent() {
			setDirection( Vector2( 1.f, 0.f ) );
		}

		/////////////////////////////////////////////////////////////////////

		ConstVelDirComponent::ConstVelDirComponent( const Vector2 & vel ): VelComponent() {
			setDirection( vel );
		}
		
		/////////////////////////////////////////////////////////////////////

		void ConstVelDirComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			pVel.setSingle( _dir );
			pVel.setSpeed( agent->_prefSpeed );
			pVel.setTarget( _dir * ( agent->_prefSpeed * 5.f ) + agent->_pos );
		}

		/////////////////////////////////////////////////////////////////////

		void ConstVelDirComponent::setDirection( const Vector2 & dir ) {
			_dir.set( norm( dir ) );
		}
		
		/////////////////////////////////////////////////////////////////////

		VelCompContext * ConstVelDirComponent::getContext() {
			return new ConstDirVCContext( this );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstDirVCContext
		/////////////////////////////////////////////////////////////////////

		ConstDirVCContext::ConstDirVCContext( ConstVelDirComponent * vc ):VelCompContext(),_vc(vc) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string ConstDirVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Const direction: " << _vc->_dir;
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void ConstDirVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			// draw preferred velocity
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos );
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ConstDirVCFactory
		/////////////////////////////////////////////////////////////////////

		ConstDirVCFactory::ConstDirVCFactory() : VelCompFactory() {
			_xID = _attrSet.addFloatAttribute( "x", true /*required*/ );
			_yID = _attrSet.addFloatAttribute( "y", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool ConstDirVCFactory::setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const {
			ConstVelDirComponent * cvc = dynamic_cast< ConstVelDirComponent * >( vc );
			assert( cvc != 0x0 && "Trying to set attributes of a const direction velocity component on an incompatible object" );

			if ( ! VelCompFactory::setFromXML( cvc, node, behaveFldr ) ) return false;
			
			cvc->setDirection( Vector2( _attrSet.getFloat( _xID ), _attrSet.getFloat( _yID ) ) );
			return true;
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ZeroVelComponent
		/////////////////////////////////////////////////////////////////////

		ZeroVelComponent::ZeroVelComponent(): VelComponent() {
		}

		/////////////////////////////////////////////////////////////////////

		void ZeroVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			pVel.setSingle( Vector2(1.f,0.f) );
			pVel.setSpeed( 0.f );
			pVel.setTarget( agent->_pos );
		}

		/////////////////////////////////////////////////////////////////////

		VelCompContext * ZeroVelComponent::getContext() {
			return new ZeroVCContext();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ZeroVCContext
		/////////////////////////////////////////////////////////////////////

		ZeroVCContext::ZeroVCContext():VelCompContext() {
		}

		/////////////////////////////////////////////////////////////////////

		std::string ZeroVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Zero velocity";
			return ss.str();
		}

		
		
	}	// namespace BFSM
}	// namespace Menge