#include "MengeCore/PluginEngine/CorePluginEngine.h"
#include "MengeCore/Runtime/SimulatorDB.h"
#include "gtest/gtest.h"

// Tests created heavily based on Michael Feathers purpose for testing systems you are not used to
// through characterization tests: https://michaelfeathers.silvrback.com/characterization-testing

using namespace Menge;
using Menge::PluginEngine::CorePluginEngine;

// Creates instance of plugins engine with empty SimulatorDB object to verify default models
TEST(CharacterizingCorePluginEngine, shouldUseOrcaAndPedVOAsDefaultModels) {
  SimulatorDB simDB;
  CorePluginEngine plugins(&simDB);
  EXPECT_EQ(simDB.modelCount(), 2);
  EXPECT_EQ(simDB.name(0), "orca");
  EXPECT_EQ(simDB.name(1), "pedvo");
}
