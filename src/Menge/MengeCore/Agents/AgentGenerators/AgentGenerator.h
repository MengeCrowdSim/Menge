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
 *	@file		AgentGenerator.h
 *	@brief		The definition of the agent generator element.  
 *				Defines the intial numbers and positions of agents in the simulation.
 */
#ifndef __AGENT_GENERATOR_H__
#define	__AGENT_GENERATOR_H__

#include "mengeCommon.h"
#include "Element.h"

namespace Menge {

	namespace Math {
		class FloatGenerator;
	}

	namespace Agents {
		// forward declaration
		class BaseAgent;

		/*!
		 *	@brief		Exception class for agent generator computation.
		 */
		class MENGE_API AgentGeneratorException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			AgentGeneratorException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			AgentGeneratorException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal agent generator exception.
		 */
		class MENGE_API AgentGeneratorFatalException : public AgentGeneratorException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			AgentGeneratorFatalException() : MengeException(), AgentGeneratorException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			AgentGeneratorFatalException( const std::string & s ): MengeException(s), AgentGeneratorException(), MengeFatalException() {}
		};

		/*!
		 *	@brief		The base class inital agent generation.
		 *
		 *	This is an abstract class, primarily defining the agent generation abstraction.
		 *	Essentially, the AgentGenerator produces a set of agent positions.  Its
		 *	properties should be sufficient to produce a count of agents with 
		 *	defined positions.
		 */
		class MENGE_API AgentGenerator : public Element {
		public:
			/*!
			 *	@brief		Constructor
			 */
			AgentGenerator();

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~AgentGenerator();

		public:
			/*!
			 *	@brief		Reports the number of agents created.
			 *
			 *	@returns	The number of agents this generator creates.
			 */
			virtual size_t agentCount() = 0;

			/*!
			 *	@brief		Get the position of the ith agent.
			 *
			 *	@param		i		The index of the requested agent.
			 *	@returns	The 2D position of the agent based on the generator's parameters.
			 *	@throws		AgentGeneratorException if the index, i, is invalid.
			 */
			virtual Vector2 agentPos( size_t i ) = 0;
			
			/*!
			 *	@brief		Sets the Generators noise generation.
			 *
			 *	The AgentGenerator takse position of the float generator provided and
			 *	will delete it up on its destruction.
			 *
			 *	@param		gen		The generator.
			 */
			void setNoiseGenerator( Math::FloatGenerator * gen );

			/*!
			 *	@brief		Perturbs the given point according to the given noise generator.
			 *
			 *	@param		pos		The position to perturb.
			 *	@returns	The perturbed point.
			 */
			Vector2 addNoise( const Vector2 & pos );

		protected:
			/*!
			 *	@brief		The generator for displacement amount (defaults to zero).
			 */
			Math::FloatGenerator	* _disp;

			/*!
			 *	@brief		The generator for angular displacement.
			 */
			Math::FloatGenerator	* _dir;
		};

	} // namespace Agents
}	// namespace Menge
#endif // __AGENT_GENERATOR_H__
