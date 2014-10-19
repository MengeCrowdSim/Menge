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

/*!
 *  @file     FSMEnumeration.h
 *  @brief    Enumerated types for the State class.
 */

#ifndef __STATE_ENUM_H__
#define __STATE_ENUM_H__

#include "Logger.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		The set of properties on which a PropertyAction can be used.
		 */
		enum PropertyOperand {
			NO_PROPERTY,	///< The "NULL" property - indicating no valid property.
			MAX_SPEED,		///< The agent's maximum speed.
			MAX_ACCEL,		///< The agent's maximum, isotropic acceleration.
			PREF_SPEED,		///< The agent's preferred speed.
			MAX_ANGLE_VEL,	///< The agent's maximum angular velocity
			NEIGHBOR_DIST,	///< The agent's neighbor distance
			PRIORITY,		///< The agent's priority
			RADIUS			///< The agent's radius
		};

		/*!
		 *	@brief		An enumeration for defining inital velocity for agents
		 */
		enum InitVelEnum {
			ZERO_INIT_VEL,		///< The agents will start with the zero velocity
			PREF_INIT_VEL		///< The agents will start with their preferred velocity
		};

	}   // namespace BFSM

	/*!
	 *	@brief		Friend function for printing string versions of the PropertyOperand enum
	 *
	 *	@param		out		The stream to write the string to.
	 *	@param		op		The ProperyOperand to write.
	 *	@returns	A reference to the output stream.
	 */
	Logger & operator<<( Logger & out, const BFSM::PropertyOperand op );

}	// namespace Menge

#endif   // __STATE_ENUM_H__
