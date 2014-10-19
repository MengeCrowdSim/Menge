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
 *	@file		StateSelector.h
 *	@brief		The definition of the agent initial state selector element.  
 *				This is the mechanism which determines which state in the FSM the agent starts in.
 */
#ifndef __STATE_SELECTOR_H__
#define	__STATE_SELECTOR_H__

#include "mengeCommon.h"
#include "Element.h"

namespace Menge {

	namespace Agents {
		// forward declaration
		class BaseAgent;

		/*!
		 *	@brief		Exception class for state selector computation.
		 */
		class MENGE_API StateSelectorException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			StateSelectorException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			StateSelectorException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal state selector exception.
		 */
		class MENGE_API StateSelectorFatalException : public StateSelectorException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			StateSelectorFatalException() : MengeException(), StateSelectorException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			StateSelectorFatalException( const std::string & s ): MengeException(s), StateSelectorException(), MengeFatalException() {}
		};

		/*!
		 *	@brief		The base class for selecting an agent's intial state.
		 *
		 *	This is an abstract class, primarily defining the mechanism for selecting
		 *	an agent's initial state.
		 */
		class MENGE_API StateSelector : public Element {
		public:
			/*!
			 *	@brief		Constructor
			 */
			StateSelector();

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~StateSelector();

		public:
			/*!
			 *	@brief		Provides the name of a state.
			 *
			 *	Ths must be overriden by child classes.
			 *
			 *	@returns:		The name of the state.
			 */
			virtual std::string getState() = 0;
		};

	} // namespace Agents
}	// namespace Menge
#endif // __PROFILE_SELECTOR_H__
