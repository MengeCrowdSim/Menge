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

#include "RectGridGenerator.h"
#include "tinyxml.h"
#include "Logger.h"
#include "Math/consts.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of RectGridGenerator
		////////////////////////////////////////////////////////////////////////////

		RectGridGenerator::RectGridGenerator() : AgentGenerator(), _anchor(0.f,0.f), _offset(0.f,0.f), _xCount(0), _yCount(0), _cosRot(1.f), _sinRot(0.f) {
		}

		////////////////////////////////////////////////////////////////////////////

		Vector2 RectGridGenerator::agentPos( size_t i ) {
			if ( i >= _xCount * _yCount ) {
				throw AgentGeneratorFatalException("RectGridGenerator trying to access an agent outside of the specified population");
			}
			size_t v = i / _xCount;
			size_t u = i % _xCount;
			// local
			Vector2 p = addNoise( Vector2( u * _offset._x, v * _offset._y ) );
			// rotated
			Vector2 r = Vector2( _cosRot * p._x - _sinRot * p._y,
								_cosRot * p._y + _sinRot * p._x );
			// world
			return _anchor + r;
		}

		////////////////////////////////////////////////////////////////////////////

		void RectGridGenerator::setRotationDeg( float angle ) {
			float rad = angle * DEG_TO_RAD;
			_cosRot = cos( rad );
			_sinRot = sin( rad );
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of RectGridGeneratorFactory
		////////////////////////////////////////////////////////////////////////////

		RectGridGeneratorFactory::RectGridGeneratorFactory() : AgentGeneratorFactory() {
			//                                         name       required default
			_anchorXID = _attrSet.addFloatAttribute( "anchor_x", true,    0.f );
			_anchorYID = _attrSet.addFloatAttribute( "anchor_y", true,    0.f );
			_offsetXID = _attrSet.addFloatAttribute( "offset_x", true,    0.f );
			_offsetYID = _attrSet.addFloatAttribute( "offset_y", true,    0.f );
			_xCountID = _attrSet.addSizeTAttribute( "count_x", true,    0 );
			_yCountID = _attrSet.addSizeTAttribute( "count_y", true,    0 );
			_rotID = _attrSet.addFloatAttribute( "rotation", false,    0.f );

		}

		////////////////////////////////////////////////////////////////////////////

		bool RectGridGeneratorFactory::setFromXML( AgentGenerator * gen, TiXmlElement * node, const std::string & specFldr ) const {
			RectGridGenerator * lGen = dynamic_cast< RectGridGenerator * >( gen );
			assert( lGen != 0x0 && "Trying to set attributes of a rectangular grid agent generator component on an incompatible object" );

			if ( ! AgentGeneratorFactory::setFromXML( lGen, node, specFldr ) ) return false;

			lGen->setAnchor( Vector2( _attrSet.getFloat( _anchorXID ),
									  _attrSet.getFloat( _anchorYID ) ) );
			lGen->setOffset( Vector2( _attrSet.getFloat( _offsetXID ),
									  _attrSet.getFloat( _offsetYID ) ) );
			lGen->setAgentCounts( _attrSet.getSizeT( _xCountID ),
								  _attrSet.getSizeT( _yCountID ) );
			lGen->setRotationDeg( _attrSet.getFloat( _rotID ) );

			return true;
		}

	}	// namespace Agents
}	 // namespace Menge