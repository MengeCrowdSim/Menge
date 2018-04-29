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
 @brief   A profile selector that selects a profile from a set with weighted probability.

 To specify a const profile selector, use the following syntax:

 ```xml
 <ProfileSelector type="weighted">
   <Profile name="PROFILE_NAME0" weight="[float]"/>
   <Profile name="PROFILE_NAME1" weight="[float]"/>
   ...
  <Profile name="PROFILE_NAMEN" weight="[float]"/>
 </ProfileSelector>
 ```

 Simply exchange the `PROFILE_NAME#` with a defined, named profile in the scene specification file.
 The weight's can be any *postive* real value. The probability that a particular profile gets
 selected is proportional to its relative weight.
 */
class MENGE_API ProfileSelectorWeighted final : public ProfileSelector {
 public:
  ProfileSelectorWeighted() = default;
  AgentInitializer* getProfile() const override;
  bool cacheProfiles(HASH_MAP<std::string, AgentInitializer*>& profiles) override;

 private:
  friend class ProfileSelectorWeightedFactory;
  SetSelector<AgentInitializer> _profiles;
  // TODO: Correct parsing so that this data is available *prior* to parsing.
  struct WeightedNames {
    WeightedNames(const std::string& name_in, float weight_in) : name(name_in), weight(weight_in) {}
    std::string name;
    float weight{};
  };
  std::vector<WeightedNames> _profile_specs;
};

/*!
 @brief    The factor for the ProfileSelectorWeighted.
 */
class MENGE_API ProfileSelectorWeightedFactory : public ProfileSelectorFactory {
 public:
  ProfileSelectorWeightedFactory() = default;
  const char* name() const override { return "weighted"; }
  const char* description() const override {
    return "Profile selector which selects a profile from a set with weighted "
           "probability";
  }

 private:
  ProfileSelector* instance() const override { return new ProfileSelectorWeighted{}; }
  bool setFromXML(ProfileSelector* pSel, TiXmlElement* node,
                  const std::string& specFldr) const override;
};

}  // namespace Agents
}  // namespace Menge
