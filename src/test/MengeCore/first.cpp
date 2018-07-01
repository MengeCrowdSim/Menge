#include <iostream>
#include <MengeVis/Runtime/SimSystem.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MengeCore/Runtime/SimulatorDB.h"
#include "MengeCore/PluginEngine/CorePluginEngine.h"
#include "MengeVis/SceneGraph/GLScene.h"


using namespace Menge;
using Menge::PluginEngine::CorePluginEngine;
using Menge::Agents::SimulatorInterface;

using MengeVis::Runtime::SimSystem;
using MengeVis::SceneGraph::GLDagNode;

using MengeVis::Runtime::SimSystem;


using ::testing::AtLeast;
using ::testing::_;

class MockScene : public MengeVis::SceneGraph::GLScene {
public:
    MOCK_METHOD2(addNode, bool(MengeVis::SceneGraph::GLNode * node, MengeVis::SceneGraph::GLDagNode * parent));


};

TEST(CharacterizingCorePluginEngine, defaultSimulationEngines) {
    SimulatorDB simDB;
    CorePluginEngine plugins(&simDB);
    EXPECT_EQ(simDB.modelCount(), 2);
    EXPECT_EQ(simDB.name(0), "orca");
    EXPECT_EQ(simDB.name(1), "pedvo");
}
