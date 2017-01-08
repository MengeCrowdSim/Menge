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

/*!
 *	@file		SimSystem.h
 *	@brief		The system which runs the simulation, coordinating
 *				the FSM and simulator.
 */

#ifndef __SIM_SYSTEM_H__
#define	__SIM_SYSTEM_H__

#include "MengeVis/SceneGraph/System.h"
#include "MengeCore/MengeException.h"

// forward declaration
namespace Menge {
	namespace Agents {
		class SimulatorInterface;
	}
}

namespace MengeVis {
	// forward declarations
	namespace SceneGraph {
		class GLScene;
	}
	namespace Runtime {
		class VisAgent;

		// Exceptions for the system
		/*!
		 *	@brief		Generic exception for the SimSystem.
		 */
		class MENGEVIS_API SimSystemException : public virtual Menge::MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SimSystemException() : Menge::MengeException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SimSystemException( const std::string & s ) : Menge::MengeException( s ) {}
		};

		/*!
		 *	@brief		The fatal SimSystem exception.
		 */
		class MENGEVIS_API SimSystemFatalException : public SimSystemException,
			public Menge::MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SimSystemFatalException() : Menge::MengeException(), SimSystemException(),
				Menge::MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SimSystemFatalException( const std::string & s ) : Menge::MengeException( s ),
				SimSystemException(),
				Menge::MengeFatalException() {}
		};


		/*!
		 *	@brief		Templated class for performing simulation.
		 *
		 *	The SimSystem is the main mechanism which performs the simulation loop.
		 *	It is responsible for updating the simulation step after step, with
		 *	appropriate calls to the BFSM and the simulator.
		 */
		class MENGEVIS_API SimSystem : public SceneGraph::System {
		public:

			/*!
			 *	@brief		Constructor
			 *
			 *	@param	sim		A pointer to the simulator.
			 */
			SimSystem( Menge::Agents::SimulatorInterface * sim );

			/*!
			 *	@brief		Destructor.
			 */
			~SimSystem();

			/*!
			 *	@brief		Update the simulation (and possibly visual elements) to the
			 *				given global time.
			 *
			 *	@param		time		The global time of the system.
			 *	@returns	True if the system has changed such that it requires a redraw.
			 */
			virtual bool updateScene( float time );

			/*!
			 *	@brief		Add visual representations of the simulation obstcles to the GLScene.
			 *
			 *	@param		scene		The scene which receives nodes for drawing obstacles.
			 */
			void addObstacleToScene( SceneGraph::GLScene * scene );

			/*!
			 *	@brief		Add visual representations of the simulation agents to the GLScene.
			 *
			 *	@param		scene		The scene which receives nodes for drawing agents.
			 */
			virtual void addAgentsToScene( SceneGraph::GLScene * scene );

			/*!
			 *	@brief		Add visual representations of obstacles and agents to the GLScene.
			 *
			 *	@param		scene		The scene which receives nodes for drawing agents.
			 */
			void populateScene( SceneGraph::GLScene * scene );

			/*!
			 *	@brief		Update the position of the *visual* agents from the simulation data.
			 *
			 *	@param		agtCount		The number of agents in the system.
			 */
			virtual void updateAgentPosition( int agtCount );

			/*!
			 *	@brief		Returns a pointer to the visualization agents
			 *
			 *	@returns	Returns the pointer to the pointers.
			 */
			inline VisAgent ** getVisAgents() { return _visAgents; }

			/*!
			 *	@brief		Reports the number of agents.
			 *
			 *	@returns	The number of VisAgents updated by the system.
			 */
			size_t getAgentCount() const;

		protected:

			/*!
			 *	@brief		Simulator to run and (possibly) visualize.
			 */
			Menge::Agents::SimulatorInterface * _sim;

			/*!
			 *	@brief		The visualization agents the system is responsible
			 *				for updating.
			 */
			VisAgent **	_visAgents;

			/*!
			 *	@brief		The global time of last system update.
			 */
			float	_lastUpdate;

			/*!
			 *	@brief		Indicates if the simulation is running.
			 */
			bool	_isRunning;
		};
	}	// namespace Runtime
}	// namespace MengeVis
#endif	// __SIM_SYSTEM_H__
