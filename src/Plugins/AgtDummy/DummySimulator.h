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
 *  @file       DummySimulator.h
 *  @brief      Contains the Dummy::Simulator class.
 */

#ifndef __DUMMY_SIMULATOR_H__
#define __DUMMY_SIMULATOR_H__

#include "mengeCommon.h"
#include "SimulatorBase.h"	
#include "DummyAgent.h"
#include "RandGenerator.h"

using namespace Menge;


/*!
 *	@namespace		Dummy
 *	@brief			The name space for the Dummy pedestrian model.
 *
 *	The dummy pedestrian model is an incredibly simple example model.
 *	Agents compute a new velocity by randomly perturbing their preferred
 *	velocity.  It is an example of the minimum implementation necessary
 *	to create a pedestrian model plug-in.
 */
namespace Dummy {
	/*!
	 *	@brief		The simulator for the Dummy pedestrian model.
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
		virtual bool isExpTarget( const std::string & tagName ) { return tagName == "Dummy"; }

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
		 *	@brief		The standard deviation of speed 
		 */
		static NormalFloatGenerator 	_speedDeviation;

		/*!
		 *	@brief		The uniform distribution of direction
		 */
		static UniformFloatGenerator	_angleDeviation;
	};
}	// namespace Dummy

#endif	 // __DUMMY_SIMULATOR_H__
