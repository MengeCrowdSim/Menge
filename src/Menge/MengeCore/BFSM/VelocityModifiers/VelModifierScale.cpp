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

#include "VelocityModifiers/VelModifierScale.h"
#include "BaseAgent.h"
#include <sstream>
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//             Implementation of ScaleVelModifier
		/////////////////////////////////////////////////////////////////////

		ScaleVelModifier::ScaleVelModifier(): VelModifier(), _scale(1.f) {
		}

		/////////////////////////////////////////////////////////////////////

		ScaleVelModifier::ScaleVelModifier(const float scale): VelModifier(), _scale(scale) {
		}

		/////////////////////////////////////////////////////////////////////

		void ScaleVelModifier::setScale(const float scale) {
			_scale = scale;
		}

		/////////////////////////////////////////////////////////////////////

		void ScaleVelModifier::adaptPrefVelocity(const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel) {
			pVel.setSpeed(pVel.getSpeed() * _scale);
		}

		/////////////////////////////////////////////////////////////////////

		VelModContext * ScaleVelModifier::getContext() {
			return new ScaleVMContext(this);
		}

		/////////////////////////////////////////////////////////////////////
		//				Implementation of ScaleVMContext
		/////////////////////////////////////////////////////////////////////
		
		ScaleVMContext::ScaleVMContext(ScaleVelModifier *vm):VelModContext(), _vm(vm) {
		};

		/////////////////////////////////////////////////////////////////////

		std::string ScaleVMContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Scale Applied: " << _vm->getScale();
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void ScaleVMContext::draw3DGL( const Agents::BaseAgent * agt) {
			// draw preferred velocity
		}

		/////////////////////////////////////////////////////////////////////

		VelModifier * ScaleVelModifier::copy() const { 
			return new ScaleVelModifier(_scale);
		};

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ScaleVMFactory
		/////////////////////////////////////////////////////////////////////

		ScaleVMFactory::ScaleVMFactory() : VelModFactory() {
			// TODO: This should support a numerical distribution
			_scaleID = _attrSet.addFloatAttribute( "scale", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool ScaleVMFactory::setFromXML( VelModifier * vm, TiXmlElement * node, const std::string & behaveFldr ) const {
			ScaleVelModifier * cvm = dynamic_cast< ScaleVelModifier * >( vm );
			assert( cvm != 0x0 && "Trying to set attributes of a Scale Velocity Modifier on an incompatible object" );

			if ( ! VelModFactory::setFromXML( vm, node, behaveFldr ) ) return false;

			cvm->setScale( _attrSet.getFloat( _scaleID ) );
			return true;
		}
		
	}	// namespace BFSM
}	 // namespace Menge