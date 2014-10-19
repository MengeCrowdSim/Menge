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

/*
* Freestyle formation
* http://graphics.cs.uh.edu/wp-content/papers/2013/2011_CGA-crowd-formation-generation-preprint.pdf
*/ 

#include "FreeFormation.h"
#include "SimulatorInterface.h"
#include "VelocityModifiers/VelModifier.h"
#include "Core.h"

namespace Formations {
	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FreeFormation
	/////////////////////////////////////////////////////////////////////

	const std::string FreeFormation::LABEL("formation");

	/////////////////////////////////////////////////////////////////////

	FreeFormation::FreeFormation(const std::string & name): Resource(name){
		_speed = 0.0f;
		_direction = Vector2(1,0);
		_pos = Vector2(0,0);
		_agentRadius = 0;
	};

	/////////////////////////////////////////////////////////////////////

	FreeFormation::~FreeFormation(){
		//we're certainly not allowed to delete the agents! 
		//but we should clear the vectors we control.
		
		std::vector<FormationPoint *>::iterator fpIter = _formationPoints.begin();
		for ( ; fpIter != _formationPoints.end(); ++fpIter ) {
			delete *fpIter;
		}

		//in all actuality, we don't have to do anything the data is constructed in such a way to implicitly handle this

		// The problem with this is that this is TWO data structures that have been
		//	merged.  The *static* underlying formation definition, and the *dynamic* 
		//	data mapping agents to that formation.
	};

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::addAgent( const Agents::BaseAgent *agt ) {
		if ( _agents.find( agt->_id ) == _agents.end() ){
			_agents[ agt->_id ] = agt;
			_agentWeights[ agt->_id ] = 1.0f; // default weight - this never changes
		}
	};

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::removeAgent( const Agents::BaseAgent *agt ) {
		std::map<size_t, const Agents::BaseAgent *>::iterator itr = _agents.find( agt->_id );
		if ( itr != _agents.end()){
			_agents.erase( itr );
		}
	};

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::addFormationPoint(Vector2 point, bool borderPoint, float weight ) {
		//now add the point
		FormationPoint *pt = new FormationPoint();

		pt->_id = _formationPoints.size(); 
		pt->_pos = point;
		pt->_dist = abs( point );
		pt->_dir = pt->_dist > 1e-5 ? -( point / pt->_dist ) : Vector2(0.f, 0.f);
		pt->_border = borderPoint;
		pt->_weight = weight;

		_formationPoints.push_back(pt);
		if (pt->_border){
			//add this to the border list. DATA REDUNDANCY but we consider it a cache for mapping
			_borderPoints.push_back(pt);
		}
	}

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::addAgentPoint(const Agents::BaseAgent *agt) {
		//make a sentinel point
		FormationPoint *agtPoint = new FormationPoint();

		agtPoint->_id = agt->_id;
		agtPoint->_pos = agt->_pos;
		agtPoint->_dir = _pos - agt->_pos;
		agtPoint->_dist = abs( agtPoint->_dir );
		agtPoint->_border = false;
		agtPoint->_weight = 0.0f;
		
		//normalize direction
		if ( agtPoint->_dist > 1e-5f ) {
			agtPoint->_dir /= agtPoint->_dist;
		}
				 
		//we'll convert to formation coordinates later
		if (agtPoint->_dist > _agentRadius){
			_agentRadius = agtPoint->_dist;
		}

		//now store it
		_agentPoints[agt->_id] = agtPoint;
	}

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::normalizeFormation() {
		std::vector<FormationPoint *>::iterator fpIter = _formationPoints.begin();

		// Compute weighted center of the reference formation
		Vector2 weightedCenter(0.f, 0.f);
		float totalWeight = 0.f;
		for (; fpIter != _formationPoints.end(); ++fpIter){
			weightedCenter += (*fpIter)->_pos * (*fpIter)->_weight;
			totalWeight += (*fpIter)->_weight;
		}
		//average the weighted center
		weightedCenter /= totalWeight;
		
		// Translate to canonical formation space and determine formation size
		//	offset by weighted center and compute encompassing circle.
		float formationRadius = 0.f;
		for ( ; fpIter != _formationPoints.end(); ++fpIter ) {
			(*fpIter)->_pos -= weightedCenter;
			(*fpIter)->_dist = abs( (*fpIter)->_pos );
			if ( (*fpIter)->_dist > formationRadius ){
				formationRadius = (*fpIter)->_dist;
			}	
		}
		
		float invDist = 1.f / formationRadius;
		// Scale all distances
		for (; fpIter != _formationPoints.end(); ++fpIter){
			(*fpIter)->_dist *= invDist;
			(*fpIter)->_pos *= invDist;	
		}
	}

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::mapAgentsToFormation(const BFSM::FSM * fsm) {
		//we need intermediate vars
		std::vector<FormationPoint *>::const_iterator formationItr;
		const Agents::BaseAgent *agt;
		size_t exceptionCount = 0;	// delete me
		size_t agtCount = 0;
		std::map<size_t,const Agents::BaseAgent *>::iterator itr; //agents
		std::map<size_t, FormationPoint *>::iterator mapIter; //agent points
		float totalWeight = 0.0f;
		//reset vars
		_pos.set( 0.f, 0.f );
		_direction.set( 0.f, 0.f );
		_speed = 0.0f;
		_agentRadius = 0.0f;
		
		//clear the relationships
		// TODO: Anything that maps agents -> value should NOT clear at each time
		//		step.  The structure of these objects should only change when the
		//		agents in the formation changes.  Not just to update values.			
		_agent_formationPoint.clear();
		_formationPoint_agent.clear();
		mapIter = _agentPoints.begin();
		for ( ; mapIter != _agentPoints.end(); ++mapIter ){
			delete mapIter->second;
		}
		_agentPoints.clear();

		// Compute formation world position, direction, and speed
		float totalSpeed = 0.f;
		for ( itr = _agents.begin(); itr != _agents.end(); ++itr ) {	
			agt = itr->second;
				
			_pos += agt->_pos * _agentWeights[agt->_id];
			totalWeight += _agentWeights[agt->_id];
			//see if we have a cache
			if (_agentPrefDirs.find(agt->_id) == _agentPrefDirs.end()){
				_direction += agt->_velPref.getPreferredVel();
			} else {
				_direction += _agentPrefVels[agt->_id];
			}
			totalSpeed += agt->_velPref.getSpeed();
			++agtCount;
		}

		//now that we can localize and normalize the formation, let's do so.
		_pos /= totalWeight;
		float mag = abs( _direction );
		_speed = totalSpeed > 0.f ?  mag / totalSpeed : mag ;
		if ( mag > 1e-5 ) {
			_direction /= mag;
		}
		
		// Define "sentinel" points for the agents -- currently unnormalized. 
		for ( itr = _agents.begin(); itr != _agents.end(); ++itr ) {	
			agt = itr->second;
			//make a sentinel point
			addAgentPoint(agt);
		}

		// First select agents for the border points
		formationItr = _borderPoints.begin();
		for ( ; formationItr != _borderPoints.end(); ++formationItr ){
			mapPointToAgent( *formationItr );
		}

		// Finally, map formation points to the remaining agents
		itr = _agents.begin();
		for (;itr != _agents.end();++itr){	
			agt = itr->second;
			if ( _agent_formationPoint.find( agt->_id) == _agent_formationPoint.end() ){
				mapAgentToPoint(agt);
			}
		}
	};

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::mapAgentToPoint( const Agents::BaseAgent * agt ){
		// This uses a brute-force approach of teseting every formation
		//		point for the agent.  No spatial queries.

		// The corresponding sentinel point for this agent
		FormationPoint * agtPoint = _agentPoints[agt->_id];
		
		std::vector<FormationPoint *>::iterator fpIter = _formationPoints.begin();
		size_t minPt = -1; // min pt for mapping 
		float distance, minDistance;
		distance = minDistance = 1000000.0f;

		for ( ; fpIter != _formationPoints.end(); ++fpIter ){
			FormationPoint * formPt = *fpIter;
			if ( _formationPoint_agent.find( formPt->_id ) == _formationPoint_agent.end() ) {
				// so the keypoint is not mapped. get the distance
				distance = formationDistance( agtPoint, formPt );
				if (distance < minDistance) {
					minDistance = distance;
					minPt = formPt->_id;
				}
			}
		}

		if (minPt == -1) {
			// TODO: Although this claims to be "fatal", it doesn't cause the
			//			program to crash.  Make the exception appropriate.
			throw BFSM::VelModFatalException( "Not enough points in formation." );
		} else {
			_formationPoint_agent[ minPt ] = agtPoint->_id;
			_agent_formationPoint[ agtPoint->_id ] = minPt;
			_agentWeights[ agt->_id ] = _formationPoints[ minPt ]->_weight;
		}
	};

	/////////////////////////////////////////////////////////////////////

	void FreeFormation::mapPointToAgent( FormationPoint * pt ){
		// This does a brute-force, linear search through the agents to find
		//	the "nearest" candidate.

		//iterate over the agents and find the best agent for this point
		std::map< size_t, const Agents::BaseAgent * >::const_iterator itr;
		FormationPoint * agtPoint;
		float distance, minDistance;
		distance = minDistance = 1000000.0;
		size_t minAgtID = -1;
		
		for ( itr = _agents.begin(); itr != _agents.end(); ++itr ){
			const Agents::BaseAgent * agt = itr->second;
			//make sure the agent isn't already mapped to a border
			if (_agent_formationPoint.find( agt->_id ) == _agent_formationPoint.end() ) {
				//check distance
				agtPoint = _agentPoints[ agt->_id ];

				//check distance
				distance = formationDistance( pt, agtPoint );
				if (distance < minDistance) {
					minDistance = distance;
					minAgtID = itr->second->_id;
				}
			}
		}
		
		// ignore if minAgtID == -1:  This means there were insufficient agents
		//		for the formation.  This is not a problem.
		if ( minAgtID > -1 ) {
			_formationPoint_agent[ pt->_id ] = minAgtID;
			_agent_formationPoint[ minAgtID ] = pt->_id;
			_agentWeights[ minAgtID ] = _formationPoints[ pt->_id ]->_weight;
		}
	};

	/////////////////////////////////////////////////////////////////////

	bool FreeFormation::getGoalForAgent( const Agents::BaseAgent * agt, Agents::PrefVelocity &pVel, Vector2 &target) {
		// The goal point is the agent's corresponding sential point (with the point moving the formations
		//	direction and speed.)

		//cache input pref dir
		_agentPrefDirs[ agt->_id ] = pVel.getPreferred();
		_agentPrefVels[ agt->_id ] = pVel.getPreferredVel();

		// assuming this is only called on agents in the formation
		assert(_agents.find(agt->_id) != _agents.end() && "Trying to get a formation goal for an agent that is not in the formation" );
		
		//the first frame an agent enters a formation does not guaruntee it has been mapped.
		if (_agent_formationPoint.find(agt->_id) != _agent_formationPoint.end()) {
		    target = _formationPoints[ _agent_formationPoint[ agt->_id ] ]->_pos + _pos;
		    target = target + _direction * _speed;
		    return true;
		}

		return false;
	};

	/////////////////////////////////////////////////////////////////////

	float FreeFormation::formationDistance(FormationPoint *pt1, FormationPoint *pt2){
		Vector2 relDir( pt1->_dir - pt2->_dir );
		return sqrtf( absSq( relDir ) + sqr( pt1->_dist - pt2->_dist ) );
	}

	/////////////////////////////////////////////////////////////////////

	Resource * FreeFormation::load( const std::string & fileName ) {
		// TODO: Change this to support comments.
		std::ifstream f;
		f.open( fileName.c_str(), std::ios::in );

		if ( !f.is_open() ) {
			logger << Logger::ERR_MSG << "Error opening formation file: " << fileName << ".";
			return 0x0;
		}

		// load vertices
		unsigned int borderCount;
		if ( ! ( f >> borderCount ) ) {
			logger << Logger::ERR_MSG << "Error in parsing formation: file didn't start with border vertex count.";
			return 0x0; 
		}

		FreeFormation * form = new FreeFormation( fileName );

		float x, y, weight;
		for ( unsigned int v = 0; v < borderCount; ++v ) {
			if ( ! ( f >> x >> y >> weight ) ) {
				logger << Logger::ERR_MSG << "Error in parsing formation: format error for vertex " << ( v + 1 ) << ".";
				form->destroy();
				return 0x0;
			}
			form->addFormationPoint( Vector2( x, y ), true, weight );
		}

		//while we have points left, they are internal points
		if ( ( f >> x >> y >> weight ) ) {
			while ( !f.eof() ) {
				form->addFormationPoint( Vector2( x, y ), false, weight );
				if ( f >> x ){
					if (!(f >> y >> weight)){
						logger << Logger::ERR_MSG << "Error in parsing formation: format error for point.";
						form->destroy();
						return 0x0;
					}
				}
			}
		}

		//normalize the formation
		form->normalizeFormation();

		return form;
	}

	/////////////////////////////////////////////////////////////////////

	FormationPtr loadFormation( const std::string & fileName ) throw ( ResourceException ) {
	    Resource * rsrc = ResourceManager::getResource( fileName, &FreeFormation::load, FreeFormation::LABEL );
		if ( rsrc == 0x0 ) {
			logger << Logger::ERR_MSG << "No resource available.";
			throw ResourceException();
		}
		FreeFormation * form = dynamic_cast< FreeFormation * >( rsrc );
		if ( form == 0x0 ) {
			logger << Logger::ERR_MSG << "Resource with name " << fileName << " is not a formation.";
			throw ResourceException();
		}
	
		return FormationPtr( form );
	}
}	// namespace Formations
