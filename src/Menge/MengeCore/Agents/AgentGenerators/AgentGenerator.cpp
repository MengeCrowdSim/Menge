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

#include "AgentGenerators/AgentGenerator.h"
#include "Math/RandGenerator.h"
#include "Math/consts.h"

namespace Menge {

	namespace Agents {

		/////////////////////////////////////////////////////////////////////
		//					Implementation of AgentGeneratorException
		/////////////////////////////////////////////////////////////////////

		AgentGeneratorException AGENT_GENERATOR_EXCEPTION;

		/////////////////////////////////////////////////////////////////////
		//					Implementation of AgentGenerator
		/////////////////////////////////////////////////////////////////////

		AgentGenerator::AgentGenerator(): Element(), _disp(0x0), _dir(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		AgentGenerator::~AgentGenerator() {
			if ( _disp ) {
				delete _disp;
				delete _dir;
			}
		}

		/////////////////////////////////////////////////////////////////////

		void AgentGenerator::setNoiseGenerator( FloatGenerator * gen ) {
			if ( _disp ) delete _disp;
			_disp = gen;
			if ( !_dir ) {
				// NOTE: This isn't perfect uniform probability
				//	The closed interval means that 0 degrees is SLIGHTLY
				//	more probably than any other direction (but the skew
				//	is considered negligible in practice).
				_dir = new UniformFloatGenerator( 0.f, TWOPI );
			}
		}
			
		/////////////////////////////////////////////////////////////////////

		Vector2 AgentGenerator::addNoise( const Vector2 & pos ) {
			if ( _disp ) {
				float disp = _disp->getValue();
				float dir = _dir->getValue();
				float x = cos( dir ) * disp;
				float y = sin( dir ) * disp;
				return pos + Vector2( x, y );
			} else {
				return pos;
			}
		}

	}	// namespace Agents
}	// namespace Menge
