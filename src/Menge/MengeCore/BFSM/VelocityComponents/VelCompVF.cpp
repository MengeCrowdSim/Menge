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

#include "VelocityComponents/VelCompVF.h"
#include "BaseAgent.h"
#include "os.h"
#include "Logger.h"
#include "Goals/Goal.h"

#include <sstream>
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of VFVelComponent
		/////////////////////////////////////////////////////////////////////

		VFVelComponent::VFVelComponent(): VelComponent(), _vf(0x0), _nearest(true) {
		}

		/////////////////////////////////////////////////////////////////////

		VFVelComponent::VFVelComponent( VectorFieldPtr & vf, bool useNearest ): VelComponent(), _vf(vf), _nearest(useNearest) {
		}

		/////////////////////////////////////////////////////////////////////

		void VFVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			Vector2 dir;
			if ( _nearest ) {
				dir.set( _vf->getFieldValue( agent->_pos ) );
			} else {
				dir.set( _vf->getFieldValueInterp( agent->_pos ) );
			}

			float scale = abs( dir );
			float speed = agent->_prefSpeed * scale;
			
			if ( scale > EPS ) {
				pVel.setSingle( dir / scale );
				pVel.setSpeed( speed );			
			} else {
				pVel.setSingle( Vector2( 1.f, 0.f ) );
				pVel.setSpeed( 0.f );
			}
			pVel.setTarget( pVel.getPreferredVel() * 5.f + agent->_pos );
		}

		/////////////////////////////////////////////////////////////////////
		
		VelCompContext * VFVelComponent::getContext() {
			return new VecFieldVCContext( this );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of VecFieldVCContext
		/////////////////////////////////////////////////////////////////////

		VecFieldVCContext::VecFieldVCContext( VFVelComponent * vc ):VelCompContext(),_vc(vc),_showLocal(true), _neighborhood(5) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string VecFieldVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Vector field velocity component";
			ss << "\n" << indent << "    Toggle (Ctrl-L)ocal display";
			ss << "\n" << indent << "        Neighborhood: " << _neighborhood << " cells (Ctrl-up/down to change)";
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		SceneGraph::ContextResult VecFieldVCContext::handleKeyboard( SDL_Event & e ) { 
			SceneGraph::ContextResult result( false, false ); 

			SDLMod mods = e.key.keysym.mod;
			bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
			bool hasAlt = ( mods & KMOD_ALT ) > 0;
			bool hasShift = ( mods & KMOD_SHIFT ) > 0;
			bool noMods = !(hasCtrl || hasAlt || hasShift );

			if ( e.type == SDL_KEYDOWN ) {
				if ( hasCtrl && !hasAlt && !hasShift ) {
					if ( e.key.keysym.sym == SDLK_UP ) {
						++_neighborhood;
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_DOWN ) {
						--_neighborhood;
						if ( _neighborhood <= 1 ) {
							_neighborhood = 1;
						}
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_l ) {
						_showLocal = !_showLocal;
						result.set( true, true );
					}
				}
			}

			return result;
		} 

		/////////////////////////////////////////////////////////////////////

		void VecFieldVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			glPushAttrib( GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glDepthMask( GL_FALSE );
			// draw the grid
			int rowZero = 0;
			int lastRow = _vc->_vf->getRowCount();
			int colZero = 0;
			int lastCol = _vc->_vf->getColCount();
			const float cellSize = _vc->_vf->getCellSize();
			Vector2 maxCorner = _vc->_vf->getMaximumPoint();
			Vector2 minCorner = _vc->_vf->getMinimumPoint();
			float left = minCorner.x();
			float right = maxCorner.x();
			float bottom = minCorner.y();
			float top = maxCorner.y();

			if ( _showLocal ) {
				int r, c;
				_vc->_vf->getCell( agt->_pos, r, c );

				rowZero = r - _neighborhood;
				if ( rowZero < 0 ) rowZero = 0;
				int tmp = r + _neighborhood + 1;
				if ( tmp < lastRow ) lastRow = tmp;
				
				colZero = c - _neighborhood;
				if ( colZero < 0 ) colZero = 0;
				tmp = c + _neighborhood + 1;
				if ( tmp < lastCol ) lastCol = tmp;

				left = minCorner.x() + colZero * cellSize;
				bottom = minCorner.y() + rowZero * cellSize;
				right = minCorner.x() + lastCol * cellSize;
				top = minCorner.y() + lastRow * cellSize;
			}

			glLineWidth( 1.f );
			glColor3f( 0.5f, 0.5f, 0.5f );
			glBegin( GL_LINES );
				// lines parallel w/ x-axis
				for ( int r = rowZero; r <= lastRow; ++r ) {
					float y = minCorner.y() + r * cellSize;
					glVertex3f( left, 0.f, y );
					glVertex3f( right, 0.f, y );
				}
				
				// lines parallel w/ y-axis
				for ( int c = colZero; c <= lastCol; ++c ) {
					float x = minCorner.x() + c * cellSize;
					glVertex3f( x, 0.f, bottom );
					glVertex3f( x, 0.f, top );
				}
			glEnd();

			// draw the vectors
			float y = bottom + 0.5f * cellSize;
			glColor3f( 1.f, 0.5f, 0.f );
			glBegin( GL_LINES );
			const float UNIT_SCALE = cellSize * 0.45f;
			for ( int r = rowZero; r < lastRow; ++ r ) {
				float x = left + 0.5f * cellSize;	
				for ( int c = colZero; c < lastCol; ++c ) {
					Vector2 dir = _vc->_vf->getFieldValue( r, c ) * UNIT_SCALE;
					Vector2 end( dir.x() + x, dir.y() + y );
					glVertex3f( x, 0.f, y );
					glVertex3f( end.x(), 0.f, end.y() );
					x += cellSize;
				}
				y += cellSize;
			}
			glEnd();

			// TODO: draw the goal
			
			// draw the preferred velocity
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos );

			glPopAttrib();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of VFVCFactory
		/////////////////////////////////////////////////////////////////////

		VFVCFactory::VFVCFactory() : VelCompFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
			_useNearestID = _attrSet.addBoolAttribute( "use_nearest", false /*required*/, true /*default*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool VFVCFactory::setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const {
			VFVelComponent * vfvc = dynamic_cast< VFVelComponent * >( vc );
			assert( vfvc != 0x0 && "Trying to set attributes of a velocity field velocity component on an incompatible object" );
			
			if ( ! VelCompFactory::setFromXML( vfvc, node, behaveFldr ) ) return false;

			// get the file name
			std::string fName;
			std::string path = os::path::join( 2, behaveFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			VectorFieldPtr vfPtr;
			try {
				vfPtr = loadVectorField( fName );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the vector field referenced on line " << node->Row() << ".";
				return false;
			}
			vfvc->setVectorField( vfPtr );
			vfvc->setUseNearest( _attrSet.getBool( _useNearestID ) );

			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge