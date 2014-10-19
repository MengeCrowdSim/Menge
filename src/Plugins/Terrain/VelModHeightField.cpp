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

#include "VelModHeightField.h"
#include "BaseAgent.h"
#include "tinyxml.h"
#include "os.h"


namespace Terrain {

	////////////////////////////////////////////////////////////////
	//					Implementation of HeightFieldVelModifier
	////////////////////////////////////////////////////////////////

	HeightFieldModifier::HeightFieldModifier(): BFSM::VelModifier(), _field(0x0), _turnWeight(1.f), _upHillScale(1.f), _downHillScale(1.f) {
	}


	////////////////////////////////////////////////////////////////

	HeightFieldModifier::HeightFieldModifier( HeightFieldPtr hfPtr ): BFSM::VelModifier(), _field(hfPtr) {
	}

	////////////////////////////////////////////////////////////////

	BFSM::VelModifier* HeightFieldModifier::copy() const{
		return new HeightFieldModifier(_field);
	};

	////////////////////////////////////////////////////////////////

	void HeightFieldModifier::adaptPrefVelocity(const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ){
		Vector2 pref = pVel.getPreferred();
		// This code is derived from fsm Hacks

		// modify direction
		Vector3 norm = _field->getNormalAt( agent->_pos._x, agent->_pos._y );
		Vector2 grad( norm._x, norm._z );
		Vector2 newDir = pref + ( _turnWeight * grad );
		newDir.normalize();
		pVel.setSingle( newDir );
		
		// modify speed
		float len = abs(grad);
		float dp = (grad / len) * pref;
		//marginal speedup, but large slow down
		dp *= ( dp > 0.f ) ? _downHillScale : _upHillScale;
		float inSpeed = pVel.getSpeed();
		float modSpeed = inSpeed * (1.f + (len * dp));
		pVel.setSpeed( modSpeed );
	}


	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FormationModFactory
	/////////////////////////////////////////////////////////////////////

	HeightFieldModifierFactory::HeightFieldModifierFactory(): BFSM::VelModFactory() {
		_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		_turnID = _attrSet.addFloatAttribute( "dir_weight", false /*required*/, 1.2f );
		_uphillID = _attrSet.addFloatAttribute( "up_hill_scale", false /*required*/, 1.f );
		_downhillID = _attrSet.addFloatAttribute( "down_hill_scale", false /*required*/, 0.2f );
	}

	/////////////////////////////////////////////////////////////////////

	bool HeightFieldModifierFactory::setFromXML( BFSM::VelModifier * modifier, TiXmlElement * node, const std::string & behaveFldr ) const { 
		HeightFieldModifier * hfm = dynamic_cast< HeightFieldModifier * >( modifier );
		assert( hfm != 0x0 && "Trying to set attributes of a height field velocity modifier on an incompatible object" );

		if ( ! BFSM::VelModFactory::setFromXML( hfm, node, behaveFldr ) ) return false;

		// get the file name
		std::string fName;
		std::string path = os::path::join( 2, behaveFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
		os::path::absPath( path, fName );
		
		// height field
		HeightFieldPtr hfPtr;
		try {
			hfPtr = loadHeightField( fName );
		} catch ( ResourceException ) {
			logger << Logger::ERR_MSG << "Couldn't instantiate the height field elevation referenced on line " << node->Row() << ".";
			return false;
		}
		hfm->setHeightField( hfPtr );
		hfm->_turnWeight = _attrSet.getFloat( _turnID );
		hfm->_upHillScale = _attrSet.getFloat( _uphillID );
		hfm->_downHillScale = _attrSet.getFloat( _downhillID );

		return true;
	}

} //namespace Terrain

