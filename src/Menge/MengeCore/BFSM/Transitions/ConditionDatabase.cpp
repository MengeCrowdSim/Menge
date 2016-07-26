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

#include "ConditionDatabase.h"
#include "Transitions/CondAuto.h"
#include "Transitions/CondTimer.h"
#include "Transitions/CondSpace.h"
#include "Transitions/CondGoal.h"
#include "Transitions/CondBoolean.h"
#include "Transitions/CondProximity.h"
#include "Transitions/CondFollow.h"
#include "Transitions/CondVia.h"
#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace Menge {

	// Specialization
	template <>
	std::string ElementDB< BFSM::ConditionFactory, BFSM::Condition >::getElementName() { return "condition"; }

	template <>
	void ElementDB< BFSM::ConditionFactory, BFSM::Condition >::addBuiltins() {
		addFactory( new BFSM::AutoCondFactory() );
		addFactory( new BFSM::TimerCondFactory() );
		addFactory( new BFSM::CircleCondFactory() );
		addFactory( new BFSM::AABBCondFactory() );
		addFactory( new BFSM::OBBCondFactory() );
		addFactory( new BFSM::GoalCondFactory() );
		addFactory( new BFSM::AndCondFactory() );
		addFactory( new BFSM::OrCondFactory() );
		addFactory( new BFSM::NotCondFactory() );
		addFactory( new BFSM::ProximityCondFactory() );
		addFactory( new BFSM::FollowTargetCondFactory() );
		addFactory( new BFSM::ViaTargetCondFactory() );
	}

}	// namespace Menge

#endif	// DOXYGEN_SHOULD_SKIP_THIS
