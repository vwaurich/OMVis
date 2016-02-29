OMVis -  An open source tool for model and simulation visualization.
====================================================================

==== REMARKS ===
At the moment, not all source and header files are in the project.


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
		-included , http://rapidxml.sourceforge.net/
	6. OpenModelica - read_matlab4.cpp - read_matlab4.h (OSMC-PL, GPL v3.0)
		-included, https://openmodelica.org/
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


