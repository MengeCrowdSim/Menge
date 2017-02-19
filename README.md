# Menge

See [Menge home page](http://gamma.cs.unc.edu/Menge/) for details.

## Running simulations

Once you have built Menge (see below), you are ready to run one of the dozens of examples included
with Menge.  Change to the directory: `$MENGE_ROOT$\Exe`.  Once there, you have several options:

1. Play *all* the examples in sequence
..* Linux: execute `./allExamples`
..* Windows: execute `allExamples.bat`  
2. Execute a specific example.  For example: 
..* Linux: `./menge -p ../examples/core/4square.xml`
..* Windows: `menge -p ..\examples\core\4square.xml`
... This will execute the 4square example with all of the built-in default values.

You can see the available command-line parameters by typing:

`menge -h` in Windos or `./menge -h` in Linux.

While a simulation is running, a visualization window will be displayed.

* Space-bar pauses/continues the simulation (up to the maximum simulation duration).
* `Ctrl-q` exits the simulation
* Left-clicking on agent will show diagnostics information about that agent.
* Pressing `h` will display help information in the console for further available manipulation.

## Build instructions

### Depenencies

Menge visualization has several dependencies.  For windows, they are included in the project. For
Linux and OSX, they need to be installed.  The dependencies include:

  - SDL
  - SDL True Type
  - SDL Image
  
In Linux, these can all be acquired at the same time with the following command:

  `sudo apt-get install libsdl-ttf2.0-dev libsdl-image1.2-dev libsdl1.2-dev`

### Building Menge

#### Windows

#### Linux

  - Open a terminal
  - Change to the `$MENGE_ROOT/projects/g++` directory
  - type: `make`
  
It will attempt to build both Menge and its Plug-ins.  It will not attempt to build the
documentation (see below for instructions).  It does this by calling CMake and then
immediately make.

It uses whatever default C++ compiler, if one can be found.  It also defaults to building a
Release version.  There are several explicit variants with obvious interpretation:

  - `make release`
  - `make debug`
  - `make release-clang`
  - `make debug-clang`
  - `make clean`
  - `make clean-release`
  - `make clean-debug`

#### Mac OSX

Steps to follow in order to get `Menge` compiled:

```bash
- cd Menge/projects/g++
- mkdir `build` # optional, but handy
- cmake -D CMAKE_MACOSX_RPATH=1 .. 
- make
```

### Building Documentation

Menge comes with documentation and you can build the source documentation locally.  This is
particularly important when submitting changes (to confirm that new code is documented 
correctly -- see docs/README.md).

We use [http://www.stack.nl/~dimitri/doxygen/](doxygen) to capture the source code documentation
as well as provide overview of authoring scenarios and Menge's architectural philosophy. Make
sure you have doxygen installed.  

In all cases, after following the instructions below, the documentation will be located in (with
slashes appropriate to your OS):

	- Windows: `$MENGE_ROOT\doc\output\menge\html\index.html`
	- Linux/OSX: `$MENGE_ROOT/doc/output/menge/html/index.html`

#### Windows

Open $MENGE_ROOT\projects\VS2013\Menge\Menge13.sln.  Build the `MengeDocs` project.

   
#### Linux and Mac OSX

To build, simply go to a terminal window. Change to $MENGE_ROOT/projects/g++.  Execute the command:

   `make docs`
   