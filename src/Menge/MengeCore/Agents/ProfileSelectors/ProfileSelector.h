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
 *	@file		ProfileSelector.h
 *	@brief		The definition of the agent profile selector element.  
 *				This is the mechanism which associates an agent profile with a new agent.
 */
#ifndef __PROFILE_SELECTOR_H__
#define	__PROFILE_SELECTOR_H__

#include "mengeCommon.h"
#include "Element.h"

namespace Menge {

	namespace Agents {
		// forward declaration
		class BaseAgent;
		class AgentInitializer;

		/*!
		 *	@brief		Exception class for profile selector computation.
		 */
		class MENGE_API ProfileSelectorException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ProfileSelectorException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ProfileSelectorException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal profile selector exception.
		 */
		class MENGE_API ProfileSelectorFatalException : public ProfileSelectorException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ProfileSelectorFatalException() : MengeException(), ProfileSelectorException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ProfileSelectorFatalException( const std::string & s ): MengeException(s), ProfileSelectorException(), MengeFatalException() {}
		};


		/*!
		 *	@brief		The base class for agent profile assignment strategies.
		 *
		 *	This is an abstract class, primarily defining the mechanism for selecting
		 *	an agent profile to assign to a newly created agent.
		 */
		class MENGE_API ProfileSelector : public Element {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ProfileSelector();

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~ProfileSelector();

		public:
			/*!
			 *	@brief		Select an agent profile based on internal mechanisms.
			 *
			 *	@returns	A const pointer to an agent initializer.
			 */
			virtual AgentInitializer * getProfile() const = 0;

			/*!
			 *	@brief		Caches the agent initializers defined in the specification.
			 *
			 *	Although the ProfileSelector can cache pointers to the profiles for
			 *	internal convenience, it does not own the memory and should not delete
			 *	them.
			 *
			 *	@param		profiles	A mapping from profile name to initializer.
			 *	@returns	True if successful, false otherwise.
			 */
			virtual bool cacheProfiles( HASH_MAP< std::string, AgentInitializer * > profiles ) = 0;
		};

	}	// namespace Agents
}	 // namespace Menge
#endif // __PROFILE_SELECTOR_H__
