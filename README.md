# OMVis - An open source tool for model and simulation visualization.



## What is OMVis?
OMVis is a tool to visualize and animate simulation models. It is meant to visualize models 
from different simulation tools. What shall be visualized is described in a XML-file and the 
result files provide the corresponding data. Besides the animation of result files, there is 
a possibility to animate multibody-systems with a FMU interactively.

OMVis is developed by Volker Waurich and Martin Flehmig at TU Dresden. If you have questions 
concerning OMVis or trouble using it, feel free to contact us via volker.waurich[at]tu-dresden.de.

OMVis is open-source and released under GNU GENERAL PUBLIC LICENSE (v3.0). See the LICENSE file 
which is comes with OMVis or visit http://www.gnu.org/licenses/gpl.html for further information.


## Installation

### Software Dependencies
OMVis, like most other software projects, builds up on a variety of software packages. In the 
following we give a list of all necessary dependencies and the version number, which is required.

    0. CMake (>= 3.0.0)
        - https://cmake.org/
        - Linux: via Package Manager or build from source
        - Windows: Install binary

    1. C++ Compiler
        - support of C++ standard 2011 (-std=c++11)

    2. SDL (>= 2.0.0) (zlib)
        - https://www.libsdl.org
        - Linux: via Package Manager --> libsdl2-dev
        - Windows: binaries from https://www.libsdl.org/download-2.0.php

    3. OpenSceneGraph (Version >= 3.4.0) (OSGPL/LGPL)
        - http://www.openscenegraph.org
        - Linux: via Package Manager --> libopenscenegraph-dev
        - Windows: 
            - Source http://www.openscenegraph.org/index.php/download-section/stable-releases, 
            - Dependencies http://www.openscenegraph.org/index.php/download-section/dependencies

    4. FMI Library (>= 2.0) (BSD)
        - http://www.jmodelica.org/FMILibrary
        - Linux: Download sources, unzip, read FMILIB_Readme.txt, configure, make, install
        - Windows: download binaries
        - Its important to remember where FMIlibary is installed to. The installation path has to 
          be provided to the CMake file of OMVis.

    5. QT (>= 5.0) (LGPL)
        - Windows installer http://www.qt.io/

    6. RapdiXml Library (Boost Software License or MIT)
        - http://rapidxml.sourceforge.net/
        - Download the header only library, extract and give (OMVis) Cmake a hint to the path via 
          environment variable RAPIDXML_ROOT.

    7. read_matlab4.c and read_matlab4.h from OpenModelica (OSMC-PL, GPL v3.0)
        - https://github.com/OpenModelica/OMCompiler/tree/master/SimulationRuntime/c
        - 1. Case: OpenModelica is present/installed on the system. Specify the installation path 
                   via OPENMODELICHOME environment variable and the CMake file will automatically 
                   find the read_matlab files.
        - 2. Case: Download the files read_matlab.c and read_matlab.h and pass their path to CMake
                   via -DMATLABREADER.

    8. Boost C++ Libraries (Boost Software License)
        - http://www.boost.org/
        - Linux: via Package Manager
            --> libboost-filesystem-dev
            --> libboost-program-options-dev
            --> libboost-system-dev
            --> libboost-dev
        - Windows: build from source



### Configure and Build
<!-- ``` -->
    ~> mkdir build
    ~> cd build
    ~> cmake -DFMILIB_HOME=/PATH/TO/FMILIB2/ -DRAPIDXML_ROOT=/PATH/TO/RAPIDXML/
    ~> make OMVis
<!-- ``` -->

### Environment Settings / Configure Options
It might be neccessary to provide one or all of the following environment settings to the CMake 
makefile of OMVis:

As usual, the C/C++ compiler and the compiler flags can be controlled via the environment variables 
<i>`CXX=<cxx>`</i>, and </i>`CXXFLAGS=<cxxflags>`</i>.

More to come.


### Configure and Build using MSVC2015

-compile OSG and SDL2 from source using the same Compiler(msvc2015)

-added to additional dependencies:
imm32.lib
winmm.lib
Ws2_32.lib
version.lib

fmilib_shared.lib
SDL2main.lib


### Develop OMVis using Eclipse (Linux)

1. Create Eclipse project from cmake:
-------------------------------------
In order to create a proper Eclipse project for OMVis call cmake with <i>-G="Eclipse CDT4 - Unix Makefiles"</i>. This has to 
be done out ouf source, i.e., create a build directory on top of the source folder:
<!-- ``` -->
    ~> mkdir OMVis_eclipse
    ~> cd OMVis_eclipse
    ~> cmake ../OMVis/ -G="Eclipse CDT4 - Unix Makefiles"
<!-- ``` -->

2. Configure Eclipse project:
-----------------------------

2.1 Get rid of "Symbol QMainWindow could not be resolved" or "Unresolved inclusion: <QAction>":
Right click on project -> C/C++ General -> Path and Symbols
Add the Qt5 includes here, e.g., /usr/include/qt5/, /usr/include/qt5/QtGui/ and /usr/include/qt5/QtCore/.
It might be necessary to add these paths to C as well as to C++. After that, you need to rebuild the index by Right 
click on project -> Index -> Rebuild.


## Usage

### Visualization
It is quite easy to use OMVis to visualize a simulation present in a FMU or MAT-file.

1. Step: Run OMVis by starting the executable.
2. Step: Open a model file by using the "File Open" dialog.
3. Step: Initialize the simulation.
4. Step: (If possible) Specify the input mapping to keyboard or joystick.
5. Step: Visualize the simulation by pressing "Start".


### Remote Visualization
In this case, the computation is done on a server while the visualization and 
steering of the simulation is handled on the local machine (localhost).

This functionality is under development.

Remarks:
  - Visual XML file has to present in working directory

## Development
Dear future developer, dear future me,

There are some points of interest that I want to write down in order to release some memory in my
head:

- While developing OMVis, we used the so called 'const correctnesses', i.e., if a variable is 
  declared const in a method declaration and definition, it is not altered. If a method is 
  declared const, it does not alter the internal status of the objects. Vice versa, if a function 
  or a variable in function declaration is not declared const, than the object or variable will be
  altered.