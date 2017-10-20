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
 *	@file		ConstProfileSelector.h
 *	@brief		The definition of a profile selector that assigns all agents
 *				the same profile.
 */

#ifndef __CONST_PROFILE_SELECTOR_H__
#define __CONST_PROFILE_SELECTOR_H__

#include "MengeCore/mengeCommon.h"
#include "MengeCore/Agents/ProfileSelectors/ProfileSelector.h"
#include "MengeCore/Agents/ProfileSelectors/ProfileSelectorFactory.h"

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		A profile selector that stores a single agent initializer
		 *				and assigns this profile to all agents.
		 *
		 *	To specify a const profile selector use the following syntax:
		 *
		 *		<ProfileSelector type="const" name="PROFILE_NAME" />
		 *
		 *	Simply exchange the `PROFILE_NAME` for a named profile in the scene
		 *	specification file.
		 */
		class MENGE_API ConstProfileSelector : public ProfileSelector {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ConstProfileSelector();

			/*!
			 *	@brief		Select an agent profile based on internal mechanisms.
			 *
			 *	@returns	A const pointer to an agent initializer.
			 */
			virtual AgentInitializer * getProfile() const { return _init; }

			/*!
			 *	@brief		Caches the agent initializers defined in the specification.
			 *
			 *	Although the ProfileSelector can cache pointers to the profiles for
			 *	internal convenience, it does not own the memory and should not delete
			 *	them.
			 *
			 *	@param		profiles		A mapping from profile name to initializer.
			 *	@returns	True if successful, false otherwise.
			 */
			virtual bool cacheProfiles( HASH_MAP< std::string, AgentInitializer * >& profiles );

			/*!
			 *	@brief		Sets the name of the profile.
			 *
			 *	@param		name		The name of the selector's profile.
			 */
			void setName( const std::string & name ) { _profileName = name; }

		protected:
			/*!
			 *	@brief		The name of the profile to use.
			 */
			std::string _profileName;

			/*!
			 *	@brief		The initializer belonging to the named profile
			 */
			AgentInitializer * _init;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The factory for the ConstProfileSelector.
		 */
		class MENGE_API ConstProfileSelectorFactory : public ProfileSelectorFactory {
		public:
			/*!
			 *	@brief		Constructor
			 */
			ConstProfileSelectorFactory();

			/*!
			 *	@brief		The name of the profile selector type.
			 *
			 *	The profile selector's name must be unique among all registered profile selector
			 *	elements. Each profile selector factory must override this function.
			 *
			 *	@returns	A string containing the unique profile selector name.
			 */
			virtual const char * name() const { return "const"; }

			/*!
			 *	@brief		A description of the profile selector.
			 *
			 *	Each profile selector factory must override this function.
			 *
			 *	@returns	A string containing the profile selector description.
			 */
			virtual const char * description() const {
				return "Profile selector which assigns the same profile to all agents.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's profile selector implementation.
			 *
			 *	All ProfileSelectorFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding selector type.  The various field values
			 *	of the instance will be set in a subsequent call to
			 *	ProfileSelectorFactory::setFromXML. The caller of this function takes ownership of
			 *	the memory.
			 *
			 *	@returns		A pointer to a newly instantiated ProfileSelector class.
			 */
			ProfileSelector * instance() const { return new ConstProfileSelector(); }

			/*!
			 *	@brief		Given a pointer to a pProfileSelector instance, sets the appropriate
			 *				fields from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this ProfileSelector's
			 *	type. (i.e. ProfileSelectorFactory::thisFactory has already been called and
			 *	returned true.) If sub-classes of ProfileSelectorFactory introduce *new*
			 *	ProfileSelector parameters, then the sub-class should override this method but
			 *	explicitly call the parent class's version.
			 *
			 *	@param		pSel			A pointer to the profile selector whose attributes are
			 *								to be set.
			 *	@param		node			The XML node containing the goal attributes.
			 *	@param		specFldr		The path to the specification file.  If the condition
			 *								references resources in the file system, it should be
			 *								defined relative to the behavior file location.  This
			 *								is the folder containing that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( ProfileSelector * pSel, TiXmlElement * node,
									 const std::string & specFldr ) const;

			/*!
			 *	@brief		The identifier for the "name" string parameter.
			 */
			size_t _nameID;
		};

	}	// namespace Agents
}	// namespace Menge
#endif	// __CONST_PROFILE_SELECTOR_H__
