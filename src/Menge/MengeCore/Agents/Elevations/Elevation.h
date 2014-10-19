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
 *	@file		Elevation.h
 *	@brief		The definition of the elevation element.  This approximately allows
 *				for agents to be simulated on non-planer domains.
 */
#ifndef __ELEVATION_H__
#define	__ELEVATION_H__

#include "mengeCommon.h"
#include "Element.h"

namespace Menge {

	namespace Agents {
		// forward declaration
		class BaseAgent;

		/*!
		 *	@brief		Exception class for elevation computation.
		 */
		class MENGE_API ElevationException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ElevationException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ElevationException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal elevation exception.
		 */
		class MENGE_API ElevationFatalException : public ElevationException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ElevationFatalException() : MengeException(), ElevationException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ElevationFatalException( const std::string & s ): MengeException(s), ElevationException(), MengeFatalException() {}
		};


		/*!
		 *	@brief		The base class for determining an agent's elevation.
		 *
		 *	This is an abstract class, primarily defining the Elevation abstraction.
		 *	Essentially, the Elevation object must be able to report elevation for
		 *	an agent (or an arbitrary position) and the "gradient" at that point.
		 *
		 *	The gradient is not *truly* the gradient.  In fact, it is merely the 
		 *	projection of the elevation object's normal on the planning plane.  So,
		 *	if the elevation is parallel to the planning plane, the gradient would be
		 *	the vector \<0, 0\>.  If the elevation surface is perpendicular to the
		 *	planning plane (a highly unlikely contingent) it would be the vector \<x, y\>
		 *	such that the magnitude of that vector is 1.
		 */
		class MENGE_API Elevation : public Element {
		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~Elevation(){}

		public:
			/*!	
			 *	@brief		Reports the elevation of the simulation domain at the given point.
			 *
			 *	The domain may have more than one valid elevation for the point.
			 *	It is the responsibility of the elevation entity resolve this.
			 *
			 *	@param		point		A point on the x-z cartesian plane.
			 *	@returns	The elevation at the given point.
			 */
			virtual float getElevation( const Vector2 & point ) const = 0;

			/*!
			 *	@brief		Reports the elevation of the simulation domain for the given agent
			 *
			 *	The domain may have more than one valid elevation for the point.
			 *	It is the responsibility of the elevation entity resolve this.
			 *
			 *	@param		agent		A pointer to the agent for which elevation should be reported.
			 *	@returns	The elevation (position on the y-axis) based on current agent state/position.
			 */
			virtual float getElevation( const BaseAgent * agent ) const = 0;

			/*!	@brief		Reports the gradient of the simulation domain at the given point.
			 *
			 *	The domain may have more than one valid gradient for the point.
			 *	It is the responsibility of the elevation entity resolve this.
			 *
			 *	@param		point		A point on the x-z cartesian plane.
			 *	@returns	The gradient at the given point.
			 */
			virtual Vector2 getGradient( const Vector2 & point ) const = 0;

			/*!
			 *	@brief		Reports the gradient of the simulation domain for the given agent
			 *
			 *	The domain may have more than one valid gradient for the point.
			 *	It is the responsibility of the elevation entity resolve this.
			 *
			 *	@param		agent		A pointer to the agent for which gradient should be reported.
			 *	@returns	The gradient of the domain based on current agent state/position.
			 */
			virtual Vector2 getGradient( const BaseAgent * agent ) const = 0;
		};

	} // namespace Agents
}	// 

#endif // __ELEVATION_H__
