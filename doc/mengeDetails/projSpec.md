Project Specification {#page_ProjectSpec}
=================

@section sec_Proj_overview Overview

The project specification file brings the high-level simulator parameters into one place. These are
the types of values that could/would be provided on the command-line. The project specification
aggregates them into a file to reduce typing.

The file takes the form (with the attributes alphabetized for convenience):

```xml
<?xml version="1.0"?>

<Project 
	behavior="[Relative Path]"
	dumpPath="images/test"
	duration="postive number"
	model="valid model name"
	output="[Relative Path]"
	random="non-negative number"
	scbVersion="valid scb version"
	scene="[Relative Path]"
	subSteps="non-negative number"
	timeStep="positive number"
	view="[Relative Path]"
/>
```

@section sec_Proj_details Description of Project Attributes

The XML `<Project>` tag attributes have the following interpretation:
  - `behavior`: the path (relative to the location of the project specification file) to a [behavior
  file](@ref page_BehaveSpec).
  - `dumpPath`: the path (relative to the location of the project specificaiton file) to a folder in
  which screen grabs should be written (aka "dumped"). It defaults to the current directory. If the
  path does not currently exist, %Menge will attempt to create it.
  - `duration`: A postive real number specifying the upper bound on how much time will be simulated.
  NOTE: if the simulation "finishes" (i.e., all agents are simultaneously in states marked "final"),
  the simulation will stop at an earlier time.
  - `model`: A string indicating the pedestrian model to use. To see the models available to %Menge
  execute the command: `menge.exe -l` (on Windows) or `./menge -l` (on Linux/OSX).
  - `output`: The name of the scb file (relative to the location of the project specification file)
  to write agent trajectory data to. If no path is given, no trajectory file will be written.
  - `random`: A non-negative number to serve as the seed to the random number generator. If 0, %Menge
  will seed the generators with a random value. If positive, it will seed the generators with that
  value, insuring consistent initialization across multiple executions.
  - `scbVersion`: A string indicating a supported trajectory (aka `scb`) file. To see the list of
  supported strings, execute: `menge.exe -h`/`./menge -h` (on Windows and Linux/OSX, respectively).
  Look for the values associated with the `--scbVersion` flag.
  - `scene`: the path (relative to the location of the project specification file) to a [scene 
  specification file](@ref page_SceneSpec).
  - `subSteps`: specification of the number of "minor" time steps to take between each major time
  step (see `timeStep` below). The difference between major and minor time steps is that 
  trajectories and visualizations are only updated on *major* timesteps. So, for example, to create
  trajectory files at a fixed frequency (e.g., 30 Hz) for models that require significantly different
  simulation time steps, use a larger value for `subSteps` to change the effective simulation time
  step from 30 Hz to 120 Hz (e.g., by passing in the value 3 -- 3 sub steps plus one major time 
  step).
  - `timeStep`: a postivie real number indicating the size of the *major* time step. 
  - `view`: the path (relative to the location of the project specification file) to a [view
  configuration](@ref page_ViewSpec).
  
A given project specification file can be indicated on the command line and any of the values given
in the project specification file can be overriden on the [command line](@ref page_CommandLine).

All of these values have default values -- if they are not given in a project file or otherwise on
the command line, the default value will be used. The default values won't necessarily lead to a 
valid simulation (i.e., failure to specify scene, behavior, or model will fail to create a 
simulation).
