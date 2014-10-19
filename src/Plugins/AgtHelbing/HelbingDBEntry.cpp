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

#include "HelbingDBEntry.h"
#include "HelbingSimulator.h"
#include "SimulatorDB.h"
#include "HelbingAgentContext.h"
#include "HelbingInitializer.h"
#include "SimSystem.h"

namespace Helbing {
	/////////////////////////////////////////////////////////////////////////////
	//                     Implementation of Helbing::DBEntry
	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::briefDescription() const {
		return "Simulator based on Helbing et al. 2000 pedestrian model";
	}
	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::longDescription() const {
		return "Simulator based on Helbing et al. 2000 pedestrian model\n"
			   "\tThe model is based on Dirk Helbing's 2000 paper.\n"
			   "\tOriginally designed for evacuation scenarios, it includes\n"
			   "\tthe contact and friction forces between agents.";
	}

	/////////////////////////////////////////////////////////////////////////////

	::std::string DBEntry::viewerName() const {
		return "Helbing 2000";
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	Agents::SimulatorInterface * DBEntry::getNewSimulator() {
		return new Simulator();
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	BaseAgentContext * DBEntry::contextFromSystem( SimSystem * system ) {
		return new AgentContext( system->getVisAgents(), (unsigned int)system->getAgentCount() ); 
	}

	/////////////////////////////////////////////////////////////////////////////
	 
	Agents::AgentInitializer * DBEntry::getAgentInitalizer() const {
		return new AgentInitializer();
	}

	/////////////////////////////////////////////////////////////////////////////
 
}	// namespace Helbing 
