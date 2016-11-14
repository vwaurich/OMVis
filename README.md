# OMVIS - An open source tool for model and simulation visualization



## What is OMVIS?
OMVIS is a tool to visualize and animate simulation models. It is meant to visualize models 
from different simulation tools. What shall be visualized is described in a XML file and the 
result files provide the corresponding data. Besides the animation of result files, there is 
a possibility to animate multibody systems with a FMU interactively.

OMVIS is developed by Volker Waurich and Martin Flehmig at TU Dresden. If you have questions 
concerning OMVIS or trouble using it, feel free to contact us via *volker.waurich[at]tu-dresden.de*.

OMVIS is open-source and released under GNU GENERAL PUBLIC LICENSE (v3.0). See the [license file](../../../LICENSE) 
which comes with OMVIS or visit http://www.gnu.org/licenses/gpl.html for further information.


## Documentation
The principal documentation is generated from the source code by using 
[Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html "Doxygen's Homepage"). Besides there is:

Volker Waurich: [Multi-Body-System Visualisation and Interactive Real-Time Simulation of FMUs for OpenModelica based Simulation](http://www.modprod.liu.se/filarkiv/1.672874/OpenModelica2016-talk10-VolkerWaurich-Multi-Body-SystemVisualisation.pdf), OpenModelica Workshop 2016


## Installation

OMVIS is concurrently developed and tested on Windows and Linux systems with the aim to support 
a wide variety of systems. Thus, we hope, that there will be a smooth configuration and build 
process for your system, too.

If you have any trouble, please let us know via *volker.waurich[at]tu-dresden.de*.

### Software Dependencies
OMVIS, like most other software projects, builds up on a variety of other great software packages. In the 
following we give a list of all necessary dependencies and the version number, which is at least required.

    0. CMake (Version >= 3.0.0)
        - https://cmake.org/
        - Linux: via Package Manager or build from source
        - Windows: Download and install binary from https://cmake.org/download/

    1. C++ Compiler
        - support of C++ standard 2014 (-std=c++14)

    2. SDL (Version >= 2.0.0) (zlib)
        - https://www.libsdl.org
        - Linux: via Package Manager --> libsdl2-dev
        - Windows: Download and install binary from https://www.libsdl.org/download-2.0.php

    3. OpenSceneGraph (Version >= 3.4.0) (OSGPL/LGPL)
        - http://www.openscenegraph.org
        - Linux: via Package Manager --> libopenscenegraph-dev
        - Windows: 
            - Source http://www.openscenegraph.org/index.php/download-section/stable-releases, 
            - Dependencies http://www.openscenegraph.org/index.php/download-section/dependencies

    4. FMI Library (Version >= 2.0) (BSD)
        - http://www.jmodelica.org/FMILibrary
        - Linux: Download sources, unzip, read FMILIB_Readme.txt, configure, make, install
        - Windows: download binaries
        - Its important to remember where FMIlibary is installed to. The installation path has to 
          be provided to the CMake script of OMVIS.

    5. QT (Version >= 5.0) (LGPL)
        - Windows installer http://www.qt.io/

    6. RapdiXml Library (Boost Software License or MIT)
        - http://rapidxml.sourceforge.net/
        - Download the header only library, extract and give the CMake script of OMVIS a hint to the 
          path via the environment variable RAPIDXML_ROOT.

    7. read_matlab4.c and read_matlab4.h from OpenModelica (OSMC-PL, GPL v3.0)
        - https://github.com/OpenModelica/OMCompiler/tree/master/SimulationRuntime/c
        - 1. Case: OpenModelica is present/installed on the system. Specify the installation path 
                   via the environment variable OPENMODELICHOME and the CMake script of OMVIS will 
                   automatically find the files read_matlab.h and read_matlab.c, respectively.
        - 2. Case: Download the files read_matlab.h and read_matlab.c and pass their path to CMake
                   via -DMATLABREADER=/PATH/TO/read_matlab.[h|c].

    8. Boost C++ Libraries (Boost Software License)
        - http://www.boost.org/
        - Linux: via Package Manager
            --> libboost-filesystem-dev
            --> libboost-program-options-dev
            --> libboost-system-dev
            --> libboost-dev
        - Windows: build from source


### Configure and Build
In order to get a version of OMVIS, the repository is cloned via

    ~> git clone https://github.com/vwaurich/OMVis.git

Than, OMVIS can is configured for the particular system and the build process is invoked

    ~> cd OMVIS/
    ~> mkdir build && cd build
    ~> cmake -DFMILIB_HOME=/PATH/TO/FMILIB2/ -DRAPIDXML_ROOT=/PATH/TO/RAPIDXML/ ../
    ~> make OMVIS


OMVIS has successfully been build and tested on Windows 7 using msvc2015 Linux Mint 17 (Qiana) using GCC 6.2 and Clang 3.8.


### Configure and Build using MSVC2015

-compile OSG and SDL2 from source using the same Compiler(msvc2015)

-added to additional dependencies:
imm32.lib
winmm.lib
Ws2_32.lib
version.lib

fmilib_shared.lib
SDL2main.lib


## Usage

It is quite easy to use OMVIS to visualize a simulation present in a FMU or MAT file:

1. Step: Run OMVIS by starting the executable.
2. Step: Open a model file by using the "File Open" dialog.
3. Step: Initialize the simulation.
4. Step: (If possible) Specify the input mapping to keyboard or joystick.
5. Step: Visualize the simulation by pressing "Start".

Moreover, OMVIS supports command line arguments. The following command will start OMVIS and load the FMU 
BouncingBall.fmu located in the examples folder into OMVIS

      ~> ./OMVIS --mode =BouncingBall.fmu --path=../examples/


### Remote Visualization
In this case, the computation is done on a server while the visualization and steering of the simulation is handled on
the local machine (localhost).

This functionality is under development.

Remarks:
  - Visual XML file has to present in working directory


## Project Status / Outlook
Alpha

* OMVIS currently **only supports FMU 1.0**. Support for the new standard FMU 2.0 needs to be added.
* The **remote visualization** in conjunction with [ParallelFMU](https://github.com/marchartung/ParallelFMU) and 
  [NetworkOffloader](https://github.com/marchartung/NetworkOffloader) library needs some more effort to make it stable.
