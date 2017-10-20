#include "MengeCore/Agents/ProfileSelectors/profile_selector_weighted.h"

#include "tinyxml/tinyxml.h"

namespace Menge {
	namespace Agents {
		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ProfileSelectorWeighted
		////////////////////////////////////////////////////////////////////////////

		AgentInitializer* ProfileSelectorWeighted::getProfile() const {
			return _profiles.getWeighted();
		}

		////////////////////////////////////////////////////////////////////////////

		bool ProfileSelectorWeighted::cacheProfiles(
			HASH_MAP<std::string, AgentInitializer*>& profiles) {
			assert(_profiles.size() == 0 && "Calling cacheProfiles on a random profile selector"
				"that already has cached profiles.");
			bool valid = true;
			for (const auto& spec : _profile_specs) {
				auto itr = profiles.find(spec.name);
				if (itr != profiles.end()) {
					_profiles.addData(itr->second, spec.weight);
				}
				else {
					logger << Logger::ERR_MSG << "Weighted profile selector unalbe to find "
						<< "profile of the name: " << spec.name << "\n";
					valid = false;
				}
			}
			return valid;
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ProfileSelectorWeightedFactory
		////////////////////////////////////////////////////////////////////////////

		bool ProfileSelectorWeightedFactory::setFromXML(ProfileSelector* pSel, TiXmlElement* node,
			const std::string & specFldr) const {
			ProfileSelectorWeighted* sel = dynamic_cast<ProfileSelectorWeighted*>(pSel);
			assert(sel != 0x0 && "Trying to set attributes of a weighted profile selector "
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

				double weight;
				if (!child->Attribute("weight", &weight)) {
					logger << Logger::ERR_MSG << "The AgentProfile referred to on line "
						<< child->Row() << " is missing the required \"weight\" attribute.";
					return false;
				}
				sel->_profile_specs.emplace_back(nameCStr, static_cast<float>(weight));
			}

			return true;
		}
	}  // namespace Agents
}  // namespace Menge
