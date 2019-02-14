/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 *  @file       MyModelSimulator.h
 *  @brief      Contains the MyModel::Simulator class.
 */

#ifndef __MYMODEL_SIMULATOR_H__
#define __MYMODEL_SIMULATOR_H__

#include "MyModelAgent.h"
#include "MengeCore/mengeCommon.h"
#include "MengeCore/Agents/SimulatorBase.h"	
#include "MengeCore/Math/RandGenerator.h"
#include "DensityField.h"
#include <iostream>
#include <math.h>
using namespace std;
/*!
 *	@namespace		MyModel
 *	@brief			The name space for the MyModel pedestrian model.
 *
 *	The dummy pedestrian model is an incredibly simple example model.
 *	Agents compute a new velocity by randomly perturbing their preferred
 *	velocity.  It is an example of the minimum implementation necessary
 *	to create a pedestrian model plug-in.
 */
namespace MyModel {
	/*!
	 *	@brief		The simulator for the MyModel pedestrian model.
	 */
	class Simulator : public Menge::Agents::SimulatorBase< Agent > {
	public:
		/*!
		 *  @brief      Constructor.
		 */
		Simulator(): Menge::Agents::SimulatorBase< Agent >() {initializeGrid();}

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
		virtual bool isExpTarget( const std::string & tagName ) { return tagName == "MyModel"; }

		/*!
		 *	@brief			Given an Experiment parameter name and value, sets the appropriate
		 *					simulator parameter.
		 *	@param			paramName		A string containing the parameter name for the
		 *									experiment.
		 *	@param			value			A string containing the value for the parameter.
		 *	@returns		whether or not parameters were successfully set
		 */
		virtual bool setExpParam( const std::string & paramName, const std::string & value )
			throw ( Menge::Agents::XMLParamException );

		virtual void doStep();

		SharedGrid* mainGrid;
		DensityField densityField;

		void initializeGrid();

	protected:
		friend class Agent;

		/*!
		 *	@brief		The standard deviation of speed 
		 */
		static Menge::Math::NormalFloatGenerator 	_speedDeviation;

		/*!
		 *	@brief		The uniform distribution of direction
		 */
		static Menge::Math::UniformFloatGenerator	_angleDeviation;
	};
}	// namespace MyModel

#endif	 // __MYMODEL_SIMULATOR_H__
