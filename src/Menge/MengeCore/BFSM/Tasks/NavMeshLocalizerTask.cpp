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

#include "NavMeshLocalizerTask.h"
#include "FSM.h"
#include "NavMeshLocalizer.h"
#include "SimulatorInterface.h"
#include <exception>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshLocalizerTask
		/////////////////////////////////////////////////////////////////////

		NavMeshLocalizerTask::NavMeshLocalizerTask( const std::string & navMeshName, bool usePlanner ): Task() {
			_localizer = loadNavMeshLocalizer( navMeshName, usePlanner );
		}

		/////////////////////////////////////////////////////////////////////

		void NavMeshLocalizerTask::doWork( const FSM * fsm ) throw( TaskException ) {
			const Agents::SimulatorInterface * sim = fsm->getSimulator();
			int agtCount = (int)sim->getNumAgents();
			
			size_t exceptionCount = 0;
			#pragma omp parallel for reduction(+:exceptionCount)
			for ( int a = 0; a < agtCount; ++a ) {
				try {
					const Agents::BaseAgent * agt = sim->getAgent( a );
					_localizer->updateLocation( agt );
				} catch ( Menge::MengeException & e  ){
					logger << Logger::ERR_MSG << e.what() << "\n";
					++exceptionCount;
				} catch ( std::exception & e ) {
					logger << Logger::ERR_MSG << "Unanticipated system exception: " << e.what() << ".";
					++exceptionCount;
				}
			}
			if ( exceptionCount > 0 ) {
				throw TaskFatalException();
			}
		}
		/////////////////////////////////////////////////////////////////////

		std::string NavMeshLocalizerTask::toString() const {
			// TODO: include the name of the navigation mesh
			return "Navigation Mesh Localizer Task";
		}

		/////////////////////////////////////////////////////////////////////

		bool NavMeshLocalizerTask::isEquivalent( const Task * task ) const {
			const NavMeshLocalizerTask * other = dynamic_cast< const NavMeshLocalizerTask * >( task );
			if ( other == 0x0 ) {
				return false;
			} else {
				return _localizer == other->_localizer;
			}
		}
		

	}	// namespace BFSM 
}	// namespace Menge