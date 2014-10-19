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
 *	@file		ObstacleSet.h
 *	@brief		The definition of the ObstacleSet element
 *				Defines the numbers and positions of obstacles in a set
 */
#ifndef __OBSTACLE_SET_H__
#define	__OBSTACLE_SET_H__

#include "Element.h"
#include "MengeException.h"

namespace Menge {

	namespace Agents {
		// forward declaration
		class Obstacle;
		class ObstacleSetFactory;
		
		/*!
		 *	@brief		Exception class for obstacle set instantiation
		 */
		class MENGE_API ObstacleSetException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ObstacleSetException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ObstacleSetException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal obstacle set exception.
		 */
		class MENGE_API ObstacleSetFatalException : public ObstacleSetException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ObstacleSetFatalException() : MengeException(), ObstacleSetException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ObstacleSetFatalException( const std::string & s ): MengeException(s), ObstacleSetException(), MengeFatalException() {}
		};


		/*!
		 *	@brief		The base class for obstacle set definitions
		 *
		 *    This is an abstract class defining the functions of an obstacle set in Menge
		 *    Obstacle sets must be able to provide a count of Obstacles in their definition
		 *    and pass those obstacles on to the SpatialQuery
		 */
		class MENGE_API ObstacleSet : public Element {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			ObstacleSet();

			friend class ObstacleSetFactory;

		protected:
			/*!
			 *	@brief		Protected destructor.  The destroy method should be used
			 *				to delete an instance of an obstacle set.
			 */
			virtual ~ObstacleSet();

		public:
			
			/*!
			 *	@brief		Reports the number of obstacles in the set.
			 *
			 *	@returns	The number of obstacles in this set.
			 */
			virtual size_t obstacleCount() = 0;

			/*!
			 *	@brief		Returns the ith obstacle.
			 *
			 *	@param		i		The index of the requested obstacle.
			 *	@returns	A pointer to the corresponding Obstacle instance.
			 *	@throws		ObstacleSetException if the index, i, is invalid.
			 */
			virtual Obstacle * getObstacle( size_t i ) = 0;
			
			/*!
			 *	@brief		Sets the class of this obstacle set.
			 *
			 *	@param		cl		the class id to which the obstacle set belongs.
			 */
			virtual void setClass( size_t cl) { _class = cl; };

			/*!
			 *	@brief		Gets the class of this obstacle set.
			 *
			 *	@returns	The class id to which the obstacle set belongs.
			 */
			virtual size_t getClass() { return _class; };

		protected:
			
			/*!
			 *	@brief		The class for the obstacles in this set.
			 *
			 *				This works with the BaseAgent::_obstacleSet member
			 *				to determine which obstacles affect an agent.
			 */
			size_t _class;
		};

	} // namespace Agents
}	// namespace Menge
#endif // __OBSTACLE_SET_H__
