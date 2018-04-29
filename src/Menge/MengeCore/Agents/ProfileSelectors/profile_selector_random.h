#pragma once

#include <vector>

#include "MengeCore/Agents/ProfileSelectors/ProfileSelector.h"
#include "MengeCore/Agents/ProfileSelectors/ProfileSelectorFactory.h"
#include "MengeCore/data_set_selector.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {
namespace Agents {
// forward declaration
class BaseAgent;
class AgentInitializer;

/*!
 @brief   A profile selector that selects a profile from a set with uniform probability.

 To specify a const profile selector, use the following syntax:

 ```xml
 <ProfileSelector type="random">
   <Profile name="PROFILE_NAME0"/>
   <Profile name="PROFILE_NAME1"/>
   ...
   <Profile name="PROFILE_NAMEN"/>
 </ProfileSelector>
 ```
 
 Simply exchange the `PROFILE_NAME#` with a defined, named profile in the scene specification file.
 */
class MENGE_API ProfileSelectorRandom final : public ProfileSelector {
 public:
  ProfileSelectorRandom() = default;
  AgentInitializer* getProfile() const override;
  bool cacheProfiles(HASH_MAP<std::string, AgentInitializer*>& profiles) override;

 private:
  friend class ProfileSelectorRandomFactory;
  SetSelector<AgentInitializer> _profiles;
  // TODO: Correct parsing so that this data is available *prior* to parsing.
  std::vector<std::string> _profile_names;
};

/*!
 @brief   The factor for the ProfileSelectorRandom.
 */
class MENGE_API ProfileSelectorRandomFactory : public ProfileSelectorFactory {
 public:
  ProfileSelectorRandomFactory() = default;
  const char* name() const override { return "random"; }
  const char* description() const override {
    return "Profile selector which selects a profile from a set with uniform "
           "probability";
  }

 private:
  ProfileSelector* instance() const override { return new ProfileSelectorRandom{}; }
  bool setFromXML(ProfileSelector* pSel, TiXmlElement* node,
                  const std::string& specFldr) const override;
};

}  // namespace Agents
}  // namespace Menge
