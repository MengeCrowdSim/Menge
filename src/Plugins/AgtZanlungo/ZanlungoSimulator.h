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

#ifndef __ZANLUNGO_SIMULATOR_H__
#define	__ZANLUNGO_SIMULATOR_H__

/*!
 *  @file       ZanlungoSimulator.h
 *  @brief      Contains the Zanlungo::Simulator class
 *
 *				Implements Zanlungo's 2011 pedestrian model
 *				"Social Force Model with Explicit Collision Prediction"
 */

#include "mengeCommon.h"
#include "SimulatorBase.h"
#include "ZanlungoAgent.h"

using namespace Menge;

/*!
 *	@namespace		Zanlungo
 *	@brief			Contains the specification of the pedestrian model
 *					based on the Zanlungo et al., 2011 paper.
 */
namespace Zanlungo {
	/*!
	 *  @brief      Defines the simulator operating on a Zanlungo::Agent.
	 */
	class Simulator : public Agents::SimulatorBase< Agent > {
	public:
		/*!
		 *  @brief      Constructor.
		 */
		Simulator(): Agents::SimulatorBase< Agent >() {}

		/*!
		 *	@brief			Reports if there are non-common Experiment parameters that
		 *					this simulator requires in the XML file.
		 *	@returns		By default, the simulator base ONLY uses common parameters.
		 *					Always returns false.
		 */
		virtual bool hasExpTarget() { return true; }

		/*!
		 *	@brief			Reports if the given Experiment attribute tag name belongs to this
		 *					simulator.
		 *  @param		tagName		the name of the considered tag
		 *	@returns		By default, the simulator base ONLY uses common parameters.
		 *					Always returns false.
		 */
		virtual bool isExpTarget( const std::string & tagName ) { return tagName == "Zanlungo"; }

		/*!
		 *	@brief			Given an Experiment parameter name and value, sets the appropriate
		 *					simulator parameter.
		 *	@param			paramName		A string containing the parameter name for the experiment.
		 *	@param			value			A string containing the value for the parameter.
		 *	@returns		whether or not parameters were successfully set
		 */
		virtual bool setExpParam( const std::string & paramName, const std::string & value ) throw( Agents::XMLParamException );

	protected:
		friend class Agent;
		/*!
		 *	@brief		The magnitude of the inter-agent repulsion forces
		 *				In the paper, this is the parameter A in the agent repulsion force
		 */
		static float	AGENT_SCALE;

		/*!
		 *	@brief		The magnitude of the agent-obstacle repulsion forces
		 *				This variable is not in the paper (dealing with obstacles is not
		 *				discussed.)  However, this is the equivalent of AGENT_SCALE
		 *				but for obstacles.
		 */
		static float	OBST_SCALE;

		/*!
		 *	@brief		The reaction time used to define the driving force
		 */
		static float	REACTION_TIME;

		/*!
		 *	@brief		The the fall-off distance of repulsive forces
		 *				In the paper, this is the parameter B in the agent repulsion force.
		 */
		static float	FORCE_DISTANCE;
	};
}	// namespace Zanlungo

#endif	 // __ZANLUNGO_SIMULATOR_H__