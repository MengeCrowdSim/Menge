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

#### Windows

To build, simply go to a command-line window, move to the root of the Menge source and type:

   `doxygen doc\MengeFul.cfg`
   
#### Linux and Mac OSX

To build, simply go to a terminal window, move to the root of the Menge source and type:

   `make docs`
   