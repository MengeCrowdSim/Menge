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

#include "TargetProb.h"
#include "tinyxml.h"

namespace Menge {

	namespace BFSM {

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of ProbTarget
		///////////////////////////////////////////////////////////////////////////

		ProbTarget::ProbTarget():TransitionTarget(), _randNum(0.f,1.f),_totalWeight(0.f),_targetNames(), _targets()  {
		}

		///////////////////////////////////////////////////////////////////////////

		ProbTarget::ProbTarget( const ProbTarget & tgt ):TransitionTarget(tgt), _randNum(tgt._randNum),_totalWeight(tgt._totalWeight), _targetNames(tgt._targetNames), _targets(tgt._targets) {
		}

		///////////////////////////////////////////////////////////////////////////

		State * ProbTarget::nextState( Agents::BaseAgent * agent ) {
			const size_t TGT_COUNT = _targets.size();
			assert( TGT_COUNT > 0 && "Trying to transition to an empty set of states" );

			const float TGT_WEIGHT = _totalWeight * _randNum.getValue();
			
			State * state = 0x0;
			float accumWeight = 0.f;
			std::map< State *, float >::const_iterator itr = _targets.begin();
			while ( accumWeight <= TGT_WEIGHT && itr != _targets.end() ) {
				state = itr->first;
				accumWeight += itr->second;
				++itr;
			}
			return state;
		}

		///////////////////////////////////////////////////////////////////////////

		bool ProbTarget::connectStates( std::map< std::string, State * > & stateMap ) {
			_totalWeight = 0.f;
			std::list< std::pair< float, std::string > >::iterator itr = _targetNames.begin();
			for ( ; itr != _targetNames.end(); ++itr ) {
				const float weight = (*itr).first;
				std::string name = (*itr).second;
				if ( stateMap.find( name ) == stateMap.end() ) {
					logger << Logger::ERR_MSG << "Probability Target with invalid state name: " << name << ".";
					return false;
				}
				_totalWeight += weight;
				_targets[ stateMap[ name ] ] = weight;
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		TransitionTarget * ProbTarget::copy() {
			return new ProbTarget( *this );
		}

		///////////////////////////////////////////////////////////////////////////
		//                   Implementation of ProbTargetFactory
		///////////////////////////////////////////////////////////////////////////

		bool ProbTargetFactory::setFromXML( TransitionTarget * target, TiXmlElement * node, const std::string & behaveFldr ) const {
			ProbTarget * tgt = dynamic_cast< ProbTarget * >( target );
			assert( tgt != 0x0 && "Trying to set the properties of a probabalistic transition target on an incompatible object" );

			if ( ! TargetFactory::setFromXML( tgt, node, behaveFldr ) ) return false;
			
			// ProbTargetFactory does *not* use the attribute set directly
			//	All of its parameters are child tags which it parses by hand.
			// Scan the child tags for State tags
			for ( TiXmlElement * child = node->FirstChildElement(); child; child = child->NextSiblingElement() ) {
				if ( child->ValueStr() == "State" ) {
					// read the weight
					double weight;
					if ( ! child->Attribute( "weight", &weight ) ) {
						logger << Logger::WARN_MSG << "The State tag on line " << child->Row() << " is missing the \"weight\" property.  It is assumed to be 1.0.";
						weight = 1.f;
					}

					// read the name
					const char * nameCStr = child->Attribute( "name" );
					if ( nameCStr == 0x0 ) {
						logger << Logger::ERR_MSG << "The State tag on line " << child->Row() << " hasn't specified the \"name\" property.";
						return false;
					}
					tgt->_targetNames.push_back( std::pair< float, std::string >( (float)weight, std::string( nameCStr ) ) );
				} else {
					logger << Logger::ERR_MSG << "Found an incompatible xml tag (" << child->ValueStr() << ") as a child of a probabalistic transition target tag on line " << child->Row() << ".";
					return false;
				}
			}
			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge