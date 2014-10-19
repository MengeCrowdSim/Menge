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

#include "Goals/GoalPoint.h"
#include "PrefVelocity.h"
#include "graphCommon.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of PointGoal
		/////////////////////////////////////////////////////////////////////

		PointGoal::PointGoal():Goal(),_point(0.f,0.f) {}

		/////////////////////////////////////////////////////////////////////

		void PointGoal::setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const {
			Vector2 dir( norm( _point - q ) );
			directions.setSingle( dir );
			directions.setTarget( _point );
		}

		/////////////////////////////////////////////////////////////////////

		void PointGoal::drawGL() const{
			glBegin( GL_POINTS );
			glVertex3f( _point.x(), 0.f, _point.y() );
			glEnd();
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of PointGoalFactory
		/////////////////////////////////////////////////////////////////////

		PointGoalFactory::PointGoalFactory() : GoalFactory() {
			_xID = _attrSet.addFloatAttribute( "x", true /*required*/ );
			_yID = _attrSet.addFloatAttribute( "y", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool PointGoalFactory::setFromXML( Goal * goal, TiXmlElement * node, const std::string & behaveFldr ) const {
			
			PointGoal * pGoal = dynamic_cast< PointGoal * >( goal );
			assert( pGoal != 0x0 && "Trying to set point goal attributes on an incompatible object." );

			if ( ! GoalFactory::setFromXML( pGoal, node, behaveFldr ) ) return false;

			pGoal->setPosition( _attrSet.getFloat( _xID ), _attrSet.getFloat( _yID ) );
			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge 