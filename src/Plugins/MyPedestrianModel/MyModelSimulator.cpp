/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "MyModelSimulator.h"
#include "MengeCore/Runtime/Utils.h"
#include "MengeCore/Math/consts.h"

namespace MyModel {

	using Menge::toFloat;
	using Menge::UtilException;
	using Menge::Agents::SimulatorBase;
	using Menge::Agents::XMLParamException;
	using Menge::Math::NormalFloatGenerator;
	using Menge::Math::UniformFloatGenerator;
	using Menge::TWOPI;
	
	////////////////////////////////////////////////////////////////
	//					Implementation of MyModel::Simulator
	////////////////////////////////////////////////////////////////

	NormalFloatGenerator Simulator::_speedDeviation( 0.f, 0.f, 0.f, 0.f );
	UniformFloatGenerator Simulator::_angleDeviation( 0.f, TWOPI );

	////////////////////////////////////////////////////////////////

	bool Simulator::setExpParam( const std::string & paramName, const std::string & value )
		throw( XMLParamException ) {
		std::printf("Setting exp param my model");
		try {
			if ( paramName == "stddev" ) {
				float stddev = toFloat( value );
				const float HALF_RANGE = 3.f * stddev;
				_speedDeviation.set( 0.f, stddev, -HALF_RANGE, HALF_RANGE );
			} else if ( ! SimulatorBase<Agent>::setExpParam( paramName, value ) ) {
				// Simulator base didn't recognize the parameter either
				return false;
			}
		} catch ( UtilException ) {
			throw XMLParamException( std::string( "MyModel parameter \"") + paramName +
											 std::string("\" value couldn't be converted to the "
											 "correct type.  Found the value: " ) + value );
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////

	void Simulator::initializeGrid(){
	    std::printf("Initializing grid\n");
		mainGrid = new SharedGrid(100,100);
		mainGrid->setupGridCells();
		densityField.grid = mainGrid;
	}

	void Simulator::doStep() {
        std::printf("Agent do step computation\n");
        assert( _spatialQuery != 0x0 && "Can't run without a spatial query instance defined" );
        _spatialQuery->updateAgents();
        int AGT_COUNT = static_cast< int >( _agents.size() );

        std::printf("Updating density field\n");
        densityField.update(_agents);
		#pragma omp parallel for
        for (int i = 0; i < AGT_COUNT; ++i) {

                    _agents[i].computeNewVelocity();

		}

		#pragma omp parallel for
				for (int i = 0; i < AGT_COUNT; ++i) {
					_agents[i].update( TIME_STEP );
				}

				_globalTime += TIME_STEP;

	}




}	//namespace MyModel
