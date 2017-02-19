# Menge

See [Menge home page](http://gamma.cs.unc.edu/Menge/) for details.

## Build instructions

### Building Menge

#### Windows

#### Linux

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

Open $MENGE_ROOT\projects\VS2013\Menge\Menge13.sln.  Build the `MengeDocs` project.  The output will be in:

   
#### Linux and Mac OSX

To build, simply go to a terminal window. Change to $MENGE_ROOT/projects/g++.  Execute the command:

   `make docs`
   