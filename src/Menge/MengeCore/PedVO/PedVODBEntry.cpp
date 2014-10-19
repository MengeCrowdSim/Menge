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

#include "PedVODBEntry.h"
#include "SimulatorDB.h"
#include "PedVOAgentContext.h"
#include "PedVODBEntry.h"
#include "PedVOInitializer.h"
#include "PedVOSimulator.h"
#include "SimSystem.h"
#include "BaseAgentContext.h"

namespace PedVO {

	/////////////////////////////////////////////////////////////////////////////
	//                     Implementation of PedVO::DBEntry
	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::briefDescription() const {
		return "Simulator based on Pedestrian Velocity Obstacle model";
	}
	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::longDescription() const {
		return "Simulator based on Pedestrian Velocity Obstacle model\n"
			   "\tUnderlying collision avoidance is based on van den Berg's 2009\n"
			   "\tORCA paper.  The model has been extended to include density-\n"
			   "\tdependent behavior and right of way";
	}

	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::viewerName() const {
		return "PedVO";
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	Menge::Agents::SimulatorInterface * DBEntry::getNewSimulator() {
		return new Simulator();
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	Menge::BaseAgentContext * DBEntry::contextFromSystem( Menge::SimSystem * simSystem ) {
		return new PedVOAgentContext( simSystem->getVisAgents(), (unsigned int)simSystem->getAgentCount() ); 
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	Menge::Agents::AgentInitializer * DBEntry::getAgentInitalizer() const {
		return new AgentInitializer();
	}

	/////////////////////////////////////////////////////////////////////////////
}	// namespace PedVO