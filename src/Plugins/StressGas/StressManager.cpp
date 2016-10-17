/*!
 *	@file		StressManager.h
 *	@brief		Manages stress values for agents
 */

#include "FSM.h"
#include "BaseAgent.h"
#include "PedVO/PedVOAgent.h"
#include <map>
#include <set>
#include <vector>
#include "StressManager.h"
#include "Core.h"

namespace StressGAS {

	////////////////////////////////////////////////////////////////
	//             Implementation of StressGasManager
	////////////////////////////////////////////////////////////////
	
	StressManager::StressManager() : _stressFunctions(), _lock() {
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
		_lock.lockRead();
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.begin();
		std::set<const Agents::BaseAgent *> deleteSet;
		for ( ; itr != _stressFunctions.end(); ++itr ) {
			itr->second->processStress();
			if ( itr->second->isFinished() ) {
				deleteSet.insert( itr->first );
			}
		}
		std::set<const Agents::BaseAgent *>::iterator aItr = deleteSet.begin();
		for ( ; aItr != deleteSet.end(); ++aItr ){
			_stressFunctions.erase( *aItr );
		}
		_lock.releaseRead();
	};

	////////////////////////////////////////////////////////////////

	StressFunction * StressManager::getStressFunction( const Agents::BaseAgent * agent ) {
		_lock.lockRead();
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		StressFunction * func = 0x0;
		if ( itr != _stressFunctions.end() ) {
			func = itr->second;
		}
		_lock.releaseRead();
		return func;
	}

	////////////////////////////////////////////////////////////////

	void StressManager::setStressFunction( const Agents::BaseAgent * agent, StressFunction * func ) {
		_lock.lockWrite();
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		if ( itr != _stressFunctions.end() ) {
			delete itr->second;
		}
		_stressFunctions[ agent ] = func;
		_lock.releaseWrite();
	}

	////////////////////////////////////////////////////////////////

	StressFunction * StressManager::popStressFunction( const Agents::BaseAgent * agent ) {
		_lock.lockWrite();
		HASH_MAP< const Agents::BaseAgent *, StressFunction *>::iterator itr =
			_stressFunctions.find( agent );
		StressFunction * func = 0x0;
		if ( itr != _stressFunctions.end() ) {
			func = itr->second;
			_stressFunctions.erase( itr );
		}
		_lock.releaseWrite();
		return func;
	}

	////////////////////////////////////////////////////////////////

};	// namespace StressGAS
