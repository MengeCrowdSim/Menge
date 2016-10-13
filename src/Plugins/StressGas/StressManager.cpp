/*!
 *	@file		StressManager.h
 *	@brief		Manages stress values for agents
 */

#include "FSM.h"
#include "BaseAgent.h"
#include "PedVO/PedVOAgent.h"
#include <map>
#include <vector>
#include "StressManager.h"
#include "Core.h"

namespace StressGAS {

	////////////////////////////////////////////////////////////////
	//             Implementation of StressGasManager
	////////////////////////////////////////////////////////////////
	
	StressManager::StressManager() : _stressFunctions() {
	
	};

	////////////////////////////////////////////////////////////////
	
	StressManager::~StressManager(){	
		// delete remaining stress functions
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.begin();
		for ( ; itr != _stressFunctions.end(); ++itr ) {
			delete itr->second;
		}
	};

	////////////////////////////////////////////////////////////////
	
	void StressManager::updateStress() {
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.begin();
		for ( ; itr != _stressFunctions.end(); ++itr ) {
			itr->second->processStress();
		}
	};

	////////////////////////////////////////////////////////////////

	StressFunction * StressManager::getStressFunction( const Agents::BaseAgent * agent ) {
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		if ( itr != _stressFunctions.end() ) {
			return itr->second;
		}
		return 0x0;
	}

	////////////////////////////////////////////////////////////////

	void StressManager::setStressFunction( const Agents::BaseAgent * agent, StressFunction * func ) {
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		if ( itr != _stressFunctions.end() ) {
			delete itr->second;
		}
		_stressFunctions[ agent ] = func;
	}

	////////////////////////////////////////////////////////////////

	StressFunction * StressManager::popStressFunction( const Agents::BaseAgent * agent ) {
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		if ( itr != _stressFunctions.end() ) {
			StressFunction * func = itr->second;
			_stressFunctions.erase( itr );
			return func;
		}
		return 0x0;
	}

	////////////////////////////////////////////////////////////////

};	// namespace StressGAS
