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

#include "Goals/Goal.h"
#include "Goals/GoalDatabase.h"
#include "GoalSet.h"
#include <cmath>
#include "fsmCommon.h"
#include <cassert>
#include "Math/consts.h"
#include "graphCommon.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//					Implementation of Goal
		/////////////////////////////////////////////////////////////////////

		const size_t Goal::MAX_CAPACITY = -1;	// -1 is the biggest value for size_t

		/////////////////////////////////////////////////////////////////////

		bool Goal::hasCapacity() const { 
			_lock.lockRead();
			bool result = _population < _capacity; 
			_lock.releaseRead();
			return result;
		}

		/////////////////////////////////////////////////////////////////////

		void Goal::assign( const Agents::BaseAgent * agent ) {
			_lock.lockWrite();
			++_population;
			if ( _population > _capacity ) throw GoalException();
			if ( _population >= _capacity && _goalSet ) _goalSet->setGoalFull( this );
			_lock.releaseWrite();
		}

		/////////////////////////////////////////////////////////////////////

		void Goal::free() {
			_lock.lockWrite();
			if ( _population >= _capacity && _goalSet ) _goalSet->setGoalAvailable( this );
			--_population;		
			_lock.releaseWrite();
		}

		/////////////////////////////////////////////////////////////////////
		
		void Goal::drawGL() const {
			glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT);
			glDepthMask( GL_FALSE );
			glColor4f( 0.75f, 0.1f, 0.75f, 0.25f );
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			drawGLGeometry();

			glDisable( GL_BLEND );
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			drawGLGeometry();

			glPopAttrib();
		}

		/////////////////////////////////////////////////////////////////////
		//					Implementation of Helpers
		/////////////////////////////////////////////////////////////////////

		Goal * parseGoal( TiXmlElement * node, const std::string & behaveFldr ) {
			return GoalDB::getInstance( node, behaveFldr );
		}

	}	// namespace BFSM 
}	// namespace Menge