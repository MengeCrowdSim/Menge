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

#include "KaramouzasSimulator.h"
#include "Utils.h"

namespace Karamouzas {
	////////////////////////////////////////////////////////////////
	//					Implementation of Karamouzas::Simulator
	////////////////////////////////////////////////////////////////

	// These values come directly from the Karamouzas paper
	float	Simulator::ORIENT_WEIGHT = 0.8f;
	float	Simulator::COS_FOV_ANGLE = cos( 100.f * DEG_TO_RAD );	// 100 degrees (200 total)
	float	Simulator::REACTION_TIME = 0.4f;
	float	Simulator::WALL_STEEPNESS = 2.f;
	float	Simulator::WALL_DISTANCE = 2.f;		// 2 meters (from the author's email...seem's quite large.)
	int		Simulator::COLLIDING_COUNT = 5;		// K most dangerous neighbors considered
	float	Simulator::D_MIN = 1.f;
	float	Simulator::D_MID = 8.f;
	float	Simulator::D_MAX = 10.f;
	float	Simulator::AGENT_FORCE = 3.f;		// how many Newtons?  

	////////////////////////////////////////////////////////////////

	bool Simulator::setExpParam( const std::string & paramName, const std::string & value ) throw( Agents::XMLParamException ) {
		try {
			if ( paramName == "orient_weight" ) {
				ORIENT_WEIGHT = toFloat( value );
			} else if ( paramName == "fov" ) {
				COS_FOV_ANGLE = cos( 0.5f * toFloat( value ) * DEG_TO_RAD );	// convert degrees to radians - operate on half angle
			} else if ( paramName == "reaction_time" ) {
				REACTION_TIME = toFloat( value );
			} else if ( paramName == "wall_steepness" ) {
				WALL_STEEPNESS = toFloat( value );
			} else if ( paramName == "wall_distance" ) {
				WALL_DISTANCE = toFloat( value );
			} else if ( paramName == "colliding_count" ) {
				COLLIDING_COUNT = toInt( value );
			} else if ( paramName == "d_min" ) {
				D_MIN = toFloat( value );
			} else if ( paramName == "d_mid" ) {
				D_MID = toFloat( value );
			} else if ( paramName == "d_max" ) {
				D_MAX = toFloat( value );
			} else if ( paramName == "agent_force" ) {
				AGENT_FORCE = toFloat( value );
			} else if ( ! Agents::SimulatorBase< Agent >::setExpParam( paramName, value ) ) {
				// Simulator base didn't recognize the parameter either
				return false;
			}
		} catch ( UtilException ) {
			throw Agents::XMLParamException( std::string( "Karamouzas parameter \"") + paramName + std::string("\" value couldn't be converted to the correct type.  Found the value: " ) + value );
		}
		return true;
	}
}	//namespace Karamouzas 
