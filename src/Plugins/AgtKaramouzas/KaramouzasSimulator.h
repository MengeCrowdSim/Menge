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

#ifndef __KARAMOUZAS_SIMULATOR_H__
#define	__KARAMOUZAS_SIMULATOR_H__

/*!
 *  @file       KaramouzasSimulator.h
 *  @brief      Contains the Karamouzas::Simulator class
 *				Implements Karamouzas's 2009 pedestrian model
 *				"A Predictive Collision Avoidance Model for Pedestrian Simulation"
 */

#include "mengeCommon.h"
#include "SimulatorBase.h"
#include "KaramouzasAgent.h"

using namespace Menge;

/*!
 *	@namespace		Karamouzas
 *	@brief			Contains the specification of the pedestrian model
 *					from the Karamouzas et al., 2009 paper.
 */
namespace Karamouzas {
	/*!
	 *  @brief      Defines the simulator operating on a Karamouzas::Agent.
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
		virtual bool isExpTarget( const std::string & tagName ) { return tagName == "Karamouzas"; }

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
		 *	@brief		Agent orientation is a weighted average between preferred
		 *				direction and actual direction.  This is that weight
		 */
		static float	ORIENT_WEIGHT;

		/*!
		 *	@brief		The half angle that defines the field of view around
		 *				orientation
		 */
		static float	COS_FOV_ANGLE;

		/*!
		 *	@brief		The reaction time used to define the driving force
		 */
		static float	REACTION_TIME;

		/*!
		 *	@brief		The steepness that the wall forces fall off: K > 0 
		 */
		static float	WALL_STEEPNESS;

		/*!
		 *	@brief		The safe distance agents prefer to keep from the walls
		 */
		static float	WALL_DISTANCE;

		/*!
		 *	@brief		The number of agents considered in the colliding set
		 */
		static int		COLLIDING_COUNT;

		/*!
		 *	@brief		The critical distance at which agent response force climbs.
		 */
		static float	D_MIN;

		/*!
		 *	@brief		The distance at which a constant agent response force begins.
		 */
		static float	D_MID;

		/*!
		 *	@brief		The distance at which there is no longer an agent response force.
		 */
		static float	D_MAX;

		/*!
		 *	@brief		The level of response force in the constant region.
		 */
		static float	AGENT_FORCE;
	};
}	// namespace Karamouzas

#endif	 // __KARAMOUZAS_SIMULATOR_H__