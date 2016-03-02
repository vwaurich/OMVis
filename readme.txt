OMVis -  An open source tool for model and simulation visualization.
====================================================================

OMVis is a tool to visualize and animate simulation models. It is meant to visualize models from different simulation tools.
What shall be visualized is described in a XML-file and the result files provide the corresponding data.
Besides the animation of result files, there is a possibility to animate multibody-systems with a FMU interactively.


=== BUILD AND COMPILE ===
== Dependencies
	1. SDL 2.0.4 (zlib)
		- https://www.libsdl.org/
		- via Package Manager: libsdl2-dev
		- windows binaries https://www.libsdl.org/download-2.0.php
	2. OpenSceneGraph (Version >= 3.4.0) (OSGPL/LGPL)
		- http://trac.openscenegraph.org
		- via Package Manager: libopenscenegraph-dev
		- windows: build from source http://www.openscenegraph.org/index.php/download-section/stable-releases, use dependencies http://www.openscenegraph.org/index.php/download-section/dependencies
	3. FMI Library (BSD)
		- http://www.jmodelica.org/FMILibrary
		- windows: download binaries
	4. QT 5.5 (LGPL)
		- windows installer http://www.qt.io/
	5. (rapdixml.hpp) (Boost Software License or MIT)
		- copy in OMVis/include/Util
		- http://rapidxml.sourceforge.net/ 
	6. OpenModelica - read_matlab4.cpp - read_matlab4.h (OSMC-PL, GPL v3.0) 
		- can be found in OMC repository \OpenModelica\OMCompiler\SimulationRuntime\c\util
		- https://github.com/OpenModelica
		- copy read_matlab4.cpp in OMVis/src/Util  and read_matlab4.h in OMVis/include/Util
	7. Boost C++ Libraries (Boost Software License)
		-windows: build from source http://www.boost.org/


== Build ==
mkdir build
cd build
cmake -DFMILIB_HOME=/home/mf/opt/HPCOM/fmilib2.0.1 ../
make


== Options ==
	- Custom C/C++ compiler sets can be used via environment variables CC=clang CXX=clang++
	
	
== BUILD AND COMPILE MSVC2015 ==
 
-compile OSG and SDL2 from source using the same Compiler(msvc2015)

-added to additional dependencies:
imm32.lib
winmm.lib
Ws2_32.lib
version.lib

fmilib_shared.lib
SDL2main.lib



