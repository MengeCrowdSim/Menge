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

  - SDL 2
  - SDL 2 True Type
  - SDL 2 Image
  
In Linux, these can all be acquired at the same time with the following command:

  `sudo apt-get install libsdl2-ttf-dev libsdl2-image-dev libsdl2-dev`

### Building Menge

#### Windows

In windows, the main Menge application and libraries are built separately from the plugins. This is
represented by two different Visual Studio solutions.

##### Menge application and libraries

Simply open one of the two pre-configured Visual Studio projects (one for VS 2013 and one for VS
2015). They would be located in `$MengeRoot\projects\vs2013\Menge` and
`$MengeRoot\projects\vs2015\Menge`, respectively. Each has multiple projects:

  - menge: the main executable
  - MengeCore: the main MengeCore.dll providing all of the core simulation technology
  - MengeDocs: a project to build the documentation (via doxygen)
  - MengeVis: the visualization library used by the main executable
  - tinyxml: the xml-parser used by Menge to consume the configuration files.
  
Simply build the `menge` project and all upstream dependencies will be built.

If switching between 32-bit and 64-bit builds, it is advisable to rebuild the whole solution. The
current build configuration can lead to installation of incompatible binaries otherwise.

##### Menge plug-ins

Similarly, there is a Plugins solution for building all of the plugins distributed with Menge. 
Again, open the corresponding solution file and simply build any or all of the plug-ins you want.
They will install in the appropriate folder so the executable will find them.


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
   
## Attribution

- Menge uses [www.sourceforge.net/projects/tinyxml](tinyxml) to parse XML.
- Menge makes use of an implementation of normal random distrubtions implemented by John Burkardt.
  See `src/MengeCore/Math/SimRandom.cpp` for details.
- Menge uses [http://tclap.sourceforge.net/](tclap) to handle command-line parsing.
- The GCF model (`src/Plugins/GCFAgent`) uses an algorithm for computing the distance of closest
  approach (DCA) between two ellipses from http://www.math.kent.edu/~zheng/ellipsoid.c.

If you use Menge as part of an academic publication, we request that you cite Menge as follows:

```
  @article{CDA1,
	author = {Sean Curtis and Andrew Best and Dinesh Manocha},
	title = {Menge: A Modular Framework for Simulating Crowd Movement},
	journal = {Collective Dynamics},
	volume = {1},
	number = {0},
	year = {2016},
	keywords = {crowd simulation; pedestrians; open source; framework; software system},
	issn = {2366-8539},	pages = {1--40}	doi = {10.17815/CD.2016.1},
	url = {https://collective-dynamics.eu/index.php/cod/article/view/A1}
  }
```
