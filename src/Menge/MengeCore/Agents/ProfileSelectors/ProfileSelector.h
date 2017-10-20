/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 *	@file		ProfileSelector.h
 *	@brief		The definition of the agent profile selector element.  
 *				This is the mechanism which associates an agent profile with a new agent.
 */
#ifndef __PROFILE_SELECTOR_H__
#define	__PROFILE_SELECTOR_H__

#include "MengeCore/mengeCommon.h"
#include "MengeCore/PluginEngine/Element.h"

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
		class MENGE_API ProfileSelectorFatalException : public ProfileSelectorException,
														public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ProfileSelectorFatalException() : MengeException(), ProfileSelectorException(),
											  MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			ProfileSelectorFatalException( const std::string & s ): MengeException(s),
																	ProfileSelectorException(),
																	MengeFatalException() {}
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
			virtual bool cacheProfiles( HASH_MAP< std::string, AgentInitializer * >& profiles ) = 0;
		};

	}	// namespace Agents
}	 // namespace Menge
#endif // __PROFILE_SELECTOR_H__
