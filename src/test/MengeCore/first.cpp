#include <iostream>
#include <MengeVis/Runtime/SimSystem.h>
#include <path_utils.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../Menge/MengeCore/Runtime/SimulatorDB.h"
#include "../../Menge/MengeCore/PluginEngine/CorePluginEngine.h"
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
//    bool addNode( GLNode * node, GLDagNode * parent = 0x0 );
    MOCK_METHOD2(addNode, bool(MengeVis::SceneGraph::GLNode * node, MengeVis::SceneGraph::GLDagNode * parent));


};

TEST(CharacterizingCorePluginEngine, defaultSimulationEngines) {
    SimulatorDB simDB;
    CorePluginEngine plugins(&simDB);
    EXPECT_EQ(simDB.modelCount(), 2);
    EXPECT_EQ(simDB.name(0), "orca");
    EXPECT_EQ(simDB.name(1), "pedvo");
}

TEST(CharacterizingSimulatorInterface, creatingAnInstance) {

    SimulatorDB simDB;
    CorePluginEngine plugins(&simDB);

    size_t agentCount;
    // Time step (in seconds)
    float TIME_STEP = 0.2f;
// The number of uniform simulation steps to take between logical time steps
    size_t SUB_STEPS = 0;
// Maximum duration of simulation (in seconds)
    float SIM_DURATION = 800.f;
// Controls whether the simulation is verbose or not
    bool VERBOSE = false;


    const std::string &behaveFile = "/home/mauricio/dev/Menge/examples/core/4square/4squareB.xml";
    const std::string &sceneFile = "/home/mauricio/dev/Menge/examples/core/4square/4squareS.xml";
    const std::string &outFile = "";
    const std::string &scbVersion = "1";
    SimulatorInterface *sim = simDB.getDBEntry("orca")->getSimulator(agentCount, TIME_STEP, SUB_STEPS,
                                                                     SIM_DURATION, behaveFile, sceneFile, outFile,
                                                                     scbVersion, VERBOSE);

    EXPECT_FALSE(sim == 0x00);


    MengeVis::Runtime::SimSystem * system = new MengeVis::Runtime::SimSystem( sim );

    EXPECT_FALSE(system == 0x00);

    MockScene scene;

    system->populateScene( &scene );

    EXPECT_CALL(scene, addNode(_, _)).Times(AtLeast(1));

}


