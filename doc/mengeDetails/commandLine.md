Command Line Interface {#page_CommandLine}
=================

@section sec_CLI_overview Overview

The command-line interface supports all of the simulator parameters specified in the 
[project specification](@ref page_ProjectSpec) as well as additional flags to access runtime
functionality:

  - `-h/--help`: Displays information on the command line interface to the console.
  - `-p/--project [filename]`: Specifies the project specification file from which simulator 
  parameters are initially drawn (but can still be overriden).
  - `--verbose`: Increases the number and types of messages written to the log.
  - `-l/--listModels`: Lists models that %Menge has access to upon execution, with a brief summary
  for each model.
  - `-L/--listModelsDetails`: Lists models that %Menge has access to upon execution, with a detailed
  description of each model.
  
@section sec_CLI_mapping Project Specificaiton-Command Line Flag Mapping

This section defines which command line flags override which project specification attributes. See
the [project specification file](@ref page_ProjectSpec) for more details.

| Project specification | Command-line flag |
| :-------------------: | :---------------- |
| `behavior`            | `-b/--behavior`   |
| `dumpPath`            | `-u/--dumpPath`   |
| `duration`            | `-d/--duration`   |
| `model`               | `-m/--model`      |
| `random`              | `-r/--random`     |
| `scbVersion`          | `--scbVersion`    |
| `scene`               | `-s/--scene`      |
| `subSteps`            | `--subSteps`      |
| `timeStep`            | `-t/--timeStep`   |
| `view`                | `--view`          |
