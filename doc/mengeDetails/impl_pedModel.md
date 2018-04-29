Implementing new Pedestrian Models                {#page_PedModel}
======================

@section sec_overivew Overview

The %Menge architectural philosophy limits the type of pedestrian models which can be easily implemented as plug-ins.  %Menge works with a particular class of pedestrian models, those that can be thought of as a function which map a per-agent preferred velocity and local state (nearby obstacles and agents) into a feasible velocity.  While this abstraction doesn't include all viable pedestrian simulators, it is a very common and convenient abstraction.  This type of pedestrian model is also referred to as a local collision avoidance or local navigation algorithm.

In order to introduce a new pedestrian model into the %Menge system, there are certain classes which must be implemented and some which are optional, they include:

- Simulator class (required)
- Pedestrian class (required)
- Simulator database entry class (required)
- An agent initializer (only required if there are novel per-agent parameters)
- An agent visualization context (optional)

The Simulator and Pedestrian classes are responsible for the run-time computation which maps the preferred velocity to a feasible velocity.  The Simulator class holds the *global* simulation parameters.  The Pedestrian class holds the per-agent simulation parameters and performs the actual computation.

The Simulator database entry class is how the plug-in registers the new pedestrian model with %Menge (see @ref page_extension).  %Menge uses it to instantiate the model-specific features at run time.  The agent initializer is only required if there are per-agent parameter values (beyond the BaseAgent properties enumerated in the `<Common>` tag.  Finally, you can extend the BaseAgentContext to provide an interactive visualization of the agent's velocity computation.  If you do not explicitly define one, the new pedestrian model will use the BaseAgentContext which allows for interactive querying of basic agent properties.

@section sec_pedModel The Pedestrian Model

This section discusses the required elements (including the quasi-optional agent initializer).  These must be implemented in order for the model to be usable by %Menge.  We recommend that you place all of your pedestrian model elements in a common namespace.

@subsection subsec_simulator Pedestrian Simulator

The Simulator class is responsible for evolving the agent simulator state.  It handles nearest neighbor queries, calls the Pedestrians function for computing a feasible velocity, performs the time integration, and maintains the global simulation parameters.  Of those duties almost all of them happen automatically and it is unnecessary for you to take any action or even concern yourself with them in implementing a new model.  The only task you must concern yourself with is handling your particular model's global parameters.

Your Simulator class should sub-class the templated PedSim::SimulatorBase class.  The base class is templated on agent type, so your simulator declaration would look something like this (assuming that your Agent and Simulator are both defined in the namespace `NewModel`):

  namespace NewModel {
    class Simulator : public PedSim::SimulatorBase< Agent > {
    public:
      /*! Functions */
      friend class Agent;
    };
  }  // namespace NewModel

If your agent model has no global parameters, then you should simply implement the function PedSim::SimulatorBase::hasExpTarget like this:

  virtual bool hasExpTarget() { return false; }

This informs the XML parser that your model does *not* expect to see any specific global parameter definitions.  If, however, your model does have global parameters, then it is necessary to inform the parser and provide the functionality for handling it.  

First, the `hasExpTarget` function must return `true`.  Second, the following function must be implemented, reporting the name of the property set via the function PedSim::SimulatorBase::isExpTarget:

  bool isExpTarget( const std::string & tagName ) { return tagName == "TagName"; }

Replace `TagName` with a uniquely identifying string that is not `Common` (as that is reserved for the common properties of all pedestrian models).  Finally, your Simulator class must handle parsing the properties by implementing the function PedSim::SimulatorBase::setExpParam:

  bool setExpParam( const std::string & paramName, const std::string & value ) throw( PedSim::XMLParamException )

Each attribute of the tag `TagName` will be passed into this function (as a parameter-value pair of strings).  The `setExpParam` function should test the parameter name, making sure it is expected, and confirm that the value is consistent with expectations.  If the parameter is expected and the string value is acceptable, it should return true, otherwise return false.

It is recommended that the global parameters be stored as static members of the simulator.  This is a reasonable choice because %Menge will only instantiate one Simulator at a time.  As global members of the simulator, each agent will have easy access to the global parameters without necessarily increasing the agent's memory footprint.

@subsection subsec_model Pedestrian Model

The Pedestrian class computes the new feasible velocity using the pedestrian model's formulation.  It should sub-class the PedSim::BaseAgent class.  It is only necessary to create a default constructor, destructor, and implement the PedSim::BaseAgent::computeNewVelocity function.

It is very important to note that the computeNewVelocity function should set the PedSim::BaseAgent::_velNew member and should *not* set the PedSim::BaseAgent::_vel property directly.  The _vel property will be set after applying the acceleration constraints.

@subsection subsec_parse Parsing Properties from the XML

If your pedestrian model requires per-agent properties which are not part of the `<Common>` set, then you will need to implement an intializer class, derived from PedSim::AgentInitializer.  The initializer helps parse the model's per-agent properties (as a child tag of the `AgentSet` tag) and uses it to initialize the properties of the agents.  Essentially, when reading an `AgentSet` tag, the agent initializer reads the particular properties defined in the `AgentSet` and then, for each agent in the set, assigns it property values based on the definitions.

The agent initializer must implement several functions:
- Default constructor and destructor
- An initialization function which sets all of the agent properties to default values (PedSim::AgentInitializer::setDefaults)
- A function to set the properties of an agent (PedSim::AgentInitializer::setProperties)
- Three functions to aid in the parsing of XML (PedSim::AgentInitializer::isRelevant, PedSim::AgentInitializer::setFromXMLAttribute, and PedSim::AgentInitializer::processProperty)

The natural reflex in implementing an agent initializer would be to create numerical primitives to represent agent properties.  For example, if your model includes a per-agent property called `mass`, then you would probably add a member to the agent initializer called `_mass` and simply set it from the value found in the xml.  However, per-agent properties can be set with value distributions (TODO: include reference to discussion of value distributions).  It achieves this by using PedMath::FloatGenerator and PedMath::IntGenerator classes as appropriate.  This means, the agent initializer should store the `_mass` value as as PedMath::FloatGenerator.  By default, it would be a PedMath::ConstFloatGenerator with the default value.  This can be replaced by either a different const, or by a different float generator based on the XML specification.

See PedOrca or PedHelbing for an example in how to implement these functions.

@section sec_register Registering the Pedestrian Model

Pedestrian models are registered with %Menge via the templated class SimulatorDBEntry.  It is templated on both the simulator and the agent classes.  The simulator database entry performs several tasks:
- It provides the command-line parameter which should be used to invoke this model (SimulatorDBEntry::commandLineName).  This name must be unique from all other models available to %Menge -- if it is not unique, an error message wil be generated. 
- It provides a short description of the model (SimulatorDBEntry::briefDescription) -- displayed in response to the -l command-line argument -- and a detailed description of the model (SimulatorDBEntry::longDescription) -- displayed in response to the -L command-line argument.
- It provides a name to be displayed in the title bar of the interactive viewer (SimulatorDBEntry::viewerName).
- It instantiates a custom agent context (see @ref sec_pedVis) if one exists, or a basic agent context by default (SimulatorDBEntry::getAgentContext).
- It instantiates an agent initializer (custom if one exists, otherwise a base agent initializer for parsing the `Common` per-agent properties (SimulatorDBEntry::getAgentInitalizer).

The simulator database also handles instantiate the simulator, but this happens automatically in the SimulatorDBEntry class and you do not need to make any explicit effort in this regard.

These functions are simply implemented and largely consist of returning constant strings or implementing one of the other, supporting classes.

@section sec_pedVis Visualizing the Pedestrian Model's Computation

In interactive mode (i.e., with the OpenGL viewer) you can interactively visualize aspects of the agent's state (by hitting the 'A' key).  The BaseAgentContext is responsible for displaying the properties which belong to PedSim::BaseAgent.  You can use a sub-class of the BaseAgentContext to include parameters which uniquely belong to you novel pedestrian model.  You can even use it to show the computation of the new velocity per-agent.  To do so, subclass the templated BaseAgentContext with your agent class as the templated parameter.  There are several functions which you should implemente:

- Constructor which takes an array of VisAgent instances and the number of agents.  These should simply be passed to the parent class's constructor.
- BaseAgentContext::contextName - this defines the name displayed in the OpenGL context when the agent context is displayed.
- BaseAgentContext::handleKeyboard - implement this and catch the appropriate keyboard strokes if there is display functionality that you want to change based on user keyboard interaction.
- BaseAgentContext::handleMouse - implement this and catch the mouse events if there is display functionality that you want to change based on user mouse interaction.  Generally, this is not used -- in this version, keyboard interaction serves as the primary UI mechanism.
- BaseAgentContext::update - this is called each time the simulator advances a time step.  It is possible that your context may be visualizing some aspect of the simulator which may be valid in one time step and invalid in the next time step.  This is the function that will allow you to validate your display state and correct it as necessary.
- BaseAgentContext::draw3DGL - this function is called to draw visual elements into the 3D world (as opposed to the 2D overlay in which text is drawn).  Any property that requires visualization should be embedded in this function.
- BaseAgentContext::agentText - when the context is active, text is printed in the corners of the screen, providing information about the selected agent.  Your custom agent context can extend the information is displayed by adding to the base class's string.

Ultimately, for more details, see the provided examples.

@section sec_setup Setting up the Project

Pedestrian plug-ins are most easily built if they are included in the current project infrastructure.  For a pedestrian model called "BetaModel", the recommended setup is to create a folder in the PedPlugins folder called "PedBeta".  All files should be added to this folder.

@subsection subsec_windowsProject Windows

These instructions make some assumptions:
- You will not be using pre-compiled headers
If you know what you are doing, you can change the instructions below to accomodate changed assumptions.

Do the following:

- Create a new dll project in the PedPlugins visual studio project called PedBeta.
- Delete the default files created (Readme.txt, stdafx.h, stdafx.cpp)
- Clear the file "PedJunk.cpp" (so that it is empty)
- Set the following project settings:
  - General Settings
    - Output directory (both release and debug)
      - "$(SolutionDir)lib"
    - Compiler Settings
      - General
        - Add'l Include Directories (release and debug)
          - "$(SolutionDir)";"$(SolutionDir)tinyxml";"$(SolutionDir)pedModels";"$(SolutionDir)math";"$(SolutionDir)simple";"$(SolutionDir)fsm";"$(SolutionDir)SceneGraph";"$(SolutionDir)include";"$(SolutionDir)SimRuntime";"$(SolutionDir)PluginEngine"
        - Precompiled Headers
          - Turn off pre-compiled headers (release and debug)
    - Linker
      - General
        - Add'l Library Directories (release and debug)
          - "$(SolutionDir)lib"
      - Input
        - Additional dependencies (debug and release)
          - SceneGraph.lib OpenGL32.lib SDL_ttf.lib GLU32.lib SDL.lib PluginEngine.lib
    - Build Events
      - Post-Build Events
        - Command Line (release and debug )

            if not exist $(SolutionDir)Exe\plugins  md $(SolutionDir)Exe\plugins
            xcopy /y $(TargetPath) $(SolutionDir)Exe\plugins

        - Description
          - `Copy DLL to exectuable folder`


NOTE: For windows dlls, which C runtime libraries (CRT) you link against will have a significant impact.  You should compile your plug-in against the "Multi-threaded DLL" CRT (or "Multi-threaded Debug DLL" for debug builds).

@subsection subsec_windowsLinux Linux

Adding the pedestrian model plug-in into the build system is simple.

-# Edit PedPlugins/CMakeLists.txt by adding the line `ADD_SUBDIRECTORY(PedBeta)`.
-# Copy a CMakeLists.txt file from one of the example pedestrian plug-ins into `PedPlugins/PedBeta`.
-# Edit the following:
  - project( ???Model) --> project(BetaModel)
  - change the file list of all .cpp files in your folder
  - replace the library name in the `add_library` and `target_link_libraries` directives to betaPed.

Now, the plug-in will be automatically built and placed in the executable folder.