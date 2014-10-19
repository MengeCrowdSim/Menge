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

#include "HexLatticeGenerator.h"
#include "tinyxml.h"
#include "Logger.h"
#include "Math/consts.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of Helper functions
		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Computes the effective density of an agent to achieve the target
		 *				density.
		 *
		 *	@param		density		The target density.
		 *	@returns	The radius of the disk which, if maximally packed, achieves the
		 *				target density.
		 */
		float effectiveRadius( float density ) {
			return 1.f / sqrt( 2.f * density * sqrt( 3.f ) );
		}

		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Computes the rank distance based on a target radius.
		 *				rank distance is the distance between two rows in the lattice.
		 *
		 *	@param		radius		The target radius of the disk.
		 *	@returns	The distance between rows.
		 */
		float rankDistance( float radius ) {
			return radius * sqrt( 3.f );
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of HexLatticeGenerator
		////////////////////////////////////////////////////////////////////////////

		HexLatticeGenerator::HexLatticeGenerator() : AgentGenerator(), _anchor(0.f,0.f), _rowDir(ROW_X),
				_cosRot(1.f), _sinRot(0.f), _totalPop(0), _rowDist(0.f), _nbrDist(0.f), _rowPop(0),
				_rowCount(0)
		{
		}

		////////////////////////////////////////////////////////////////////////////

		Vector2 HexLatticeGenerator::agentPos( size_t i ) {
			if ( i >= _totalPop ) {
				throw AgentGeneratorFatalException("HexLatticeGenerator trying to access an agent outside of the specified population");
			}

			// Compute local position
			const float R = _nbrDist * 0.5f;
			
			float x, y;
			if ( _rowDir == ROW_X ) {
				const size_t BAND_POP = _rowPop * 2 - 1;
				size_t band = i / BAND_POP;  // number of full preceeding bands
				i -= band * BAND_POP;
				if ( i >= _rowPop ) {
					// minor row
					i -= _rowPop;
					x = R + i * _nbrDist;
					y = band * 2.f * _rowDist + _rowDist;
				} else {
					// major row
					x = i * _nbrDist;
					y = band * 2.f * _rowDist;
				}
			} else {
				const size_t column = i / _rowPop; // number of full columns preceding i
				i -= _rowPop * column;
				x = column * _rowDist;
				y = i * _nbrDist;
				if ( column % 2 ) {
					y += R;
				}
			}
			Vector2 p = addNoise( Vector2( x, y ) );
			// rotated
			Vector2 r = Vector2( _cosRot * p._x - _sinRot * p._y,
								_cosRot * p._y + _sinRot * p._x );
			// world
			return _anchor + r;
		}

		////////////////////////////////////////////////////////////////////////////

		void HexLatticeGenerator::setRotationDeg( float angle ) {
			float rad = angle * DEG_TO_RAD;
			_cosRot = cos( rad );
			_sinRot = sin( rad );
		}

		////////////////////////////////////////////////////////////////////////////

		void HexLatticeGenerator::set( const Vector2 & anchor, AnchorAlignEnum align, LatticeRowEnum dir,
									float width, float density, size_t tgtPopulation, float angle ) 
		{
			_rowDir = dir;
			setRotationDeg( angle );
			float r = effectiveRadius( density );
			_rowDist = rankDistance( r );
			_nbrDist = 2.f * r;
			// Compute the layout of the agents for quick compute
			float actualWidth;
			switch ( dir ) {
				case ROW_X:
					_rowPop = (size_t)( width / _nbrDist );
					if ( _rowPop == 0 ) ++_rowPop;
					_rowCount = ( tgtPopulation / ( 2 * _rowPop - 1 ) + 1 ) * 2;
					_totalPop = ( 2 * _rowPop - 1 ) * ( _rowCount / 2 );
					if ( _totalPop - ( _rowPop - 1 ) > tgtPopulation ) {
						_totalPop -= ( _rowPop - 1 );
						--_rowCount;
					}
					actualWidth = ( _rowPop - 1 ) * _nbrDist;
					break;
				case ROW_Y:
					_rowCount = (int)( width / _rowDist );
					if ( _rowCount == 0 ) ++_rowCount;
					_rowPop = tgtPopulation / _rowCount + 1;
					_totalPop = _rowCount * _rowPop;
					actualWidth = ( _rowCount - 1 ) * _rowDist;
					break;
			}
			// Recompute _anchor based on _anchorAlign - essentially, change the anchor point
			//	such that the first agent is always placed at the origin.
			// This is the rotated displacement of the center/right corner to the origin
			if ( align == CENTER ) {
				_anchor = anchor - Vector2( _cosRot * actualWidth, _sinRot * actualWidth ) * 0.5f;
			} else if ( align == RIGHT_CORNER ) {
				_anchor = anchor - Vector2( _cosRot * actualWidth, _sinRot * actualWidth );
			} else {
				// Don't need any modification for _align == LEFT
				_anchor = anchor;
			}
			
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of HexLatticeGeneratorFactory
		////////////////////////////////////////////////////////////////////////////

		HexLatticeGeneratorFactory::HexLatticeGeneratorFactory() : AgentGeneratorFactory() {
			_anchorXID = _attrSet.addFloatAttribute( "anchor_x", true,    0.f );
			_anchorYID = _attrSet.addFloatAttribute( "anchor_y", true,    0.f );
			_alignID = _attrSet.addStringAttribute( "alignment", true, "center" );
			_rowDirID = _attrSet.addStringAttribute( "row_direction", true, "x" );
			_densityID = _attrSet.addFloatAttribute( "density", true,    0.f );
			_widthID = _attrSet.addFloatAttribute( "width", true,    0.f );
			_popID = _attrSet.addSizeTAttribute( "population", true,    0 );
			_rotID = _attrSet.addFloatAttribute( "rotation", false,    0.f );
		}

		////////////////////////////////////////////////////////////////////////////

		bool HexLatticeGeneratorFactory::setFromXML( AgentGenerator * gen, TiXmlElement * node, const std::string & specFldr ) const {
			HexLatticeGenerator * lGen = dynamic_cast< HexLatticeGenerator * >( gen );
			assert( lGen != 0x0 && "Trying to set attributes of a hexagonal lattice agent generator component on an incompatible object" );

			if ( ! AgentGeneratorFactory::setFromXML( lGen, node, specFldr ) ) return false;

			HexLatticeGenerator::AnchorAlignEnum align;
			std::string alignS = _attrSet.getString( _alignID );
			if ( alignS == "center" ) {
				align = HexLatticeGenerator::CENTER;
			} else if ( alignS == "left" ) {
				align = HexLatticeGenerator::LEFT_CORNER;
			} else if ( alignS == "right" ) {
				align = HexLatticeGenerator::RIGHT_CORNER;
			} else {
				logger << Logger::ERR_MSG << "Incorrectly specified hex lattice alignment value on line " << node->Row() << ": " << alignS << ".  Should be center, left, or right.";
				return false;
			}

			HexLatticeGenerator::LatticeRowEnum dir;
			std::string rowDir = _attrSet.getString( _rowDirID );
			if ( rowDir == "x" ) {
				dir = HexLatticeGenerator::ROW_X;
			} else if ( rowDir == "y" ) {
				dir = HexLatticeGenerator::ROW_Y;
			} else {
				logger << Logger::ERR_MSG << "Incorrectly specified hex lattice row direction value on line " << node->Row() << ": " << rowDir << ".  Should be x or y.";
				return false;
			}
			lGen->set( Vector2( _attrSet.getFloat( _anchorXID ), _attrSet.getFloat( _anchorYID ) ),
						align,
						dir,
						_attrSet.getFloat( _widthID ),
						_attrSet.getFloat( _densityID ),
						_attrSet.getSizeT( _popID ), 
						_attrSet.getFloat( _rotID )
						);

			return true;
		}

	}	// namespace Agents
}	// namespace Menge
