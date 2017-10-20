#include "MengeCore/Agents/ProfileSelectors/profile_selector_random.h"

#include "tinyxml/tinyxml.h"

namespace Menge {
	namespace Agents {
		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ProfileSelectorRandom
		////////////////////////////////////////////////////////////////////////////

		AgentInitializer* ProfileSelectorRandom::getProfile() const {
			return _profiles.getRandom();
		}

		////////////////////////////////////////////////////////////////////////////

		bool ProfileSelectorRandom::cacheProfiles(
			HASH_MAP<std::string, AgentInitializer*>& profiles) {
			assert(_profiles.size() == 0 && "Calling cacheProfiles on a random profile selector"
				"that already has cached profiles.");
			bool valid = true;
			for (const auto& name : _profile_names) {
				auto itr = profiles.find(name);
				if (itr != profiles.end()) {
					_profiles.addData(itr->second);
				}
				else {
					logger << Logger::ERR_MSG << "Random profile selector unalbe to find profile "
						<< "of the name: " << name << "\n";
					valid = false;
				}
			}
			return valid;
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ProfileSelectorRandomFactory
		////////////////////////////////////////////////////////////////////////////

		bool ProfileSelectorRandomFactory::setFromXML(ProfileSelector* pSel, TiXmlElement* node,
			const std::string & specFldr) const {
			ProfileSelectorRandom* sel = dynamic_cast<ProfileSelectorRandom*>(pSel);
			assert(sel != 0x0 && "Trying to set attributes of a random profile selector "
				"element on an incompatible object");
			if (!ProfileSelectorFactory::setFromXML(sel, node, specFldr)) return false;

			// Parse each of the names.
			for (TiXmlElement* child = node->FirstChildElement("Profile");
				 child;
				 child = child->NextSiblingElement()) {
				const char* nameCStr = child->Attribute("name");
				if (nameCStr == 0x0) {
					logger << Logger::ERR_MSG << "The AgentProfile referred to on line " 
						<< child->Row() << " is missing the required \"name\" attribute.";
					return false;
				}
				sel->_profile_names.emplace_back(nameCStr);
			}

			return true;
		}
	}  // namespace Agents
}  // namespace Menge
