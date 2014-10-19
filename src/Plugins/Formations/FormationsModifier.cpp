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

#ifndef __FORMATIONS_MODIFIER_CPP__
#define __FORMATIONS_MODIFIER_CPP__

#include "FormationsModifier.h"
#include "FormationsTask.h"

namespace Formations {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FormationModifier
	/////////////////////////////////////////////////////////////////////

	FormationModifier::FormationModifier() {
	}

	/////////////////////////////////////////////////////////////////////

	FormationModifier::~FormationModifier(){
	};

	/////////////////////////////////////////////////////////////////////

	FormationModifier::FormationModifier( FormationPtr form ){
		_formation = form;
	}

	/////////////////////////////////////////////////////////////////////

	BFSM::VelModifier* FormationModifier::copy() const{
		return new FormationModifier( _formation );
	};

	/////////////////////////////////////////////////////////////////////

	void FormationModifier::adaptPrefVelocity( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ){
		//adapt the agent's velocity according to the formation
		Vector2 target = Vector2( 0.f, 0.f );
		_lock.lockRead();
		bool modify = _formation->getGoalForAgent( agent, pVel, target );
		_lock.releaseRead();
		if ( modify ) {
			pVel.setTarget(target);
			pVel.setSpeed(agent->_prefSpeed);
			Vector2 dir = target - agent->_pos;
			// TODO: The units don't match
			//	||dir|| is meters, speed is m/s  -- how does this work?
			//	Also, I'm normalizing dir possibly THREE times - that's a lot of
			//		square roots.
			if ( abs( dir ) < pVel.getSpeed() ){
				pVel.setSpeed( abs( dir ) );
			}
			dir.normalize();
			pVel.setSingle( dir );
		}
	}

	/////////////////////////////////////////////////////////////////////

	void FormationModifier::setFormation(FormationPtr form){
		_formation = form;
	};

	/////////////////////////////////////////////////////////////////////

	BFSM::Task * FormationModifier::getTask(){
		return new FormationsTask( _formation );
	};

	/////////////////////////////////////////////////////////////////////

	void FormationModifier::registerAgent(const Agents::BaseAgent * agent){
		_lock.lockWrite();
	    _formation->addAgent(agent);
		_lock.releaseWrite();
	};

	/////////////////////////////////////////////////////////////////////

	void FormationModifier::unregisterAgent(const Agents::BaseAgent * agent){
		_lock.lockWrite();
		_formation->removeAgent(agent);
		_lock.releaseWrite();
	};

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FormationModFactory
	/////////////////////////////////////////////////////////////////////

	FormationModifierFactory::FormationModifierFactory():BFSM::VelModFactory() {
		//no properties yet
		_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
	}

	/////////////////////////////////////////////////////////////////////

	bool FormationModifierFactory::setFromXML( BFSM::VelModifier * modifier, TiXmlElement * node, const std::string & behaveFldr ) const { 
		FormationModifier * formationMod = dynamic_cast<FormationModifier *>(modifier);
        assert( formationMod != 0x0 && "Trying to set property modifier properties on an incompatible object" );

		if ( ! BFSM::VelModFactory::setFromXML( modifier, node, behaveFldr ) ) return false;
		
		// get the absolute path to the file name

		std::string fName;
		std::string path = os::path::join( 2, behaveFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
		os::path::absPath( path, fName );
		// nav mesh
		FormationPtr formPtr;
		try {
			formationMod->setFormation(loadFormation( fName ));
		} catch ( ResourceException ) {
			logger << Logger::ERR_MSG << "Couldn't instantiate the formation referenced on line " << node->Row() << ".";
			return false;
		}

		return true;
	}

	/////////////////////////////////////////////////////////////////////
};
#endif
