CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(OMVis)

SET(CMAKE_VERBOSE_MAKEFILE ON)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#MESSAGE(STATUS "Using Cmake version ${CMAKE_VERSION} and ${CMAKE_MODULE_PATH}")

# Enable Logger
ADD_DEFINITIONS(-DUSE_LOGGER)


# ---------------------------
# Find All Dependencies
# ---------------------------
# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
SET(CMAKE_AUTOMOC ON)
# Detect and add SFML
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

# Find the QtWidgets library
FIND_PACKAGE(Qt5Widgets)
IF(Qt5Widgets_FOUND)
  MESSAGE(STATUS "Qt5Widgets library version ${Qt5Widgets_VERSION_STRING} found.")
ELSE(Qt5Widgets_FOUND)
  MESSAGE(FATAL_ERROR "Qt5Widgets library not found but required.")
ENDIF(Qt5Widgets_FOUND)

# Find the Qt5OpenGL library
FIND_PACKAGE(Qt5OpenGL)
IF(Qt5OpenGL_FOUND)
  MESSAGE(STATUS "Qt5OpenGL library version ${Qt5OpenGL_VERSION_STRING} found.")
ELSE(Qt5OpenGL_FOUND)
  MESSAGE(FATAL_ERROR "Qt5OpenGL library not found but required.")
ENDIF(Qt5OpenGL_FOUND)

# Find the QtGui library
FIND_PACKAGE(Qt5Gui)
IF(Qt5Gui_FOUND)
  MESSAGE(STATUS "Qt5Gui library version ${Qt5Gui_VERSION_STRING} found.")
ELSE(Qt5Gui_FOUND)
  MESSAGE(FATAL_ERROR "Qt5Gui library not found but required.")
ENDIF(Qt5Gui_FOUND)

# Find the Qt5Core library
FIND_PACKAGE(Qt5Core)
IF(Qt5Core_FOUND)
  MESSAGE(STATUS "Qt5Core library version ${Qt5Core_VERSION_STRING} found.")
ELSE(Qt5Core_FOUND)
  MESSAGE(FATAL_ERROR "Qt5Core library not found but required.")
ENDIF(Qt5Core_FOUND)

# Find FMI library
FIND_PACKAGE(FMILib)
IF(FMILIB_FOUND)
  MESSAGE(STATUS "FMI library found.")
ELSE(FMILIB_FOUND)
  MESSAGE(FATAL_ERROR "FMI library not found but required. Set FMILIB_HOME to the location containing lib and include folder of FMI library.")
ENDIF(FMILIB_FOUND)

# Find OpenSceneGraph
FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgViewer osgUtil osgGA osgQt)
IF(OPENSCENEGRAPH_FOUND)
  MESSAGE(STATUS "OpenSceneGraph found123.")
ELSE(OPENSCENEGRAPH_FOUND)
  MESSAGE(FATAL_ERROR "OpenSceneGraph not found but required.")
ENDIF(OPENSCENEGRAPH_FOUND)

# Find SDL2
FIND_PACKAGE(SDL2 REQUIRED sdl2)
IF(SDL2_FOUND)
  MESSAGE(STATUS "SDL2 library found.")
ELSEIF(SDL2_FOUND)
  MESSAGE(FATAL_ERROR "SDL2 not found but required. Set SDL2DIR to the location containing lib and include folder of SDL2 library.")
ENDIF(SDL2_FOUND)

# Find Boost
FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options)
IF(Boost_FOUND)
  MESSAGE(STATUS "Boost Program Options library found.")
ELSE(Boost_FOUND)
  MESSAGE (FATAL_ERROR "Boost Program Options library not found.")
ENDIF(Boost_FOUND)

# Find rapidxml
FIND_PACKAGE(RapidXML REQUIRED)


# Find read_matlab4.h and read_matlab4.c from OpenModelica
# If the environment variable OPENMODELICAHOME is set and points to the OpenModelica installation, 
# we will find the files. Otherwise, the user can specify the path via argument MATLABREADER to CMake.
IF(DEFINED ENV{OPENMODELICAHOME})
  SET(OMC_MATLAB_READER_INC $ENV{OPENMODELICAHOME}/include/omc/c/util/)
  SET(OMC_MATLAB_READER_C $ENV{OPENMODELICAHOME}/include/omc/c/util/read_matlab4.c)
  SET(MATLABREADER_FOUND YES)
ELSEIF(MATLABREADER)
  IF(EXISTS ${MATLABREADER}/read_matlab4.h)
    MESSAGE(STATUS "Found read_matlab4.h.")
    SET(OMC_MATLAB_READER_INC ${MATLABREADER})
    IF(EXISTS ${MATLABREADER}/read_matlab4.c)
      MESSAGE(STATUS "Found read_matlab4.c.")
      SET(OMC_MATLAB_READER_C ${MATLABREADER}/read_matlab4.c)
      SET(MATLABREADER_FOUND YES)
    ELSE()
      SET(MATLABREADER_FOUND NO)
    ENDIF()
  ENDIF()
ELSE(DEFINED ENV{OPENMODELICAHOME})
  MESSAGE(WARNING "OpenModelica not found. You can specify the environment variable OPENMODELICA_HOME pointing to your OpenModelica installation \
                   or pass the path to read_matlab4.h and read_matlab4.c via MATLABREADER to cmake file.")
  SET(MATLABREADER_FOUND NO)
ENDIF(DEFINED ENV{OPENMODELICAHOME})

MESSAGE(STATUS "BUMS ${MATLABREADER_FOUND} .")

# ---------------------------
# Compiler Flags
# ---------------------------
# Check for C++11 Support
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX11)
  ADD_COMPILE_OPTIONS(-std=c++11 -Wall)
ELSEIF(COMPILER_SUPPORTS_CXX0X)
  ADD_COMPILE_OPTIONS(-std=c++0x -Wall)
ELSE(COMPILER_SUPPORTS_CXX11)
  MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF(COMPILER_SUPPORTS_CXX11)

# Compiler Flags Used In Debug Mode
IF(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_COMPILE_OPTIONS(-O0)
  #-fno-elide-constructors -pedantic-errors -ansi -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Winit-self)

  # Check for Compiler Warnings
  CHECK_CXX_COMPILER_FLAG("-Wextra -Wunused" COMPILER_SUPPORTS_WEXTRA)
  IF(COMPILER_SUPPORTS_WEXTRA)
    ADD_COMPILE_OPTIONS(-Wextra -Wunused)
  ENDIF(COMPILER_SUPPORTS_WEXTRA)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)


# ---------------------------
# Windows
# ---------------------------
SET(LIBRARIES_EXTRA "")
IF(WIN32) # Check if we are on Windows
  IF(MSVC) # Check if we are using the Visual Studio compiler
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /SUBSYSTEM:WINDOWS") # Tell the project how to behave in this environment
    SET(LIBRARIES_EXTRA "imm32.lib;winmm.lib;Ws2_32.lib;version.lib;${FMILIB_SHARED};${SDL2_MAIN}")
  ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Tell the project how to behave in this environment
  ELSE()
    MESSAGE(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  ENDIF()
ELSEIF(UNIX)
  #ADD_EXECUTABLE(Test Test.cpp) # Tell the project how to behave in this environment
ELSE()
  MESSAGE(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()
#MESSAGE(STATUS "Libraries extra: ${LIBRARIES_EXTRA} ...")


# ---------------------------
# Build Documentation
# ---------------------------
IF(BUILD_DOCUMENTATION)
  # Find Doxygen
  FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen found. Documentation can be build.")
  ELSE(DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF(DOXYGEN_FOUND)

  SET(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
  SET(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
  SET(doxyfile_html_in ${CMAKE_CURRENT_SOURCE_DIR}/doc)
  SET(DOC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include")
  CONFIGURE_FILE(${doxyfile_in} ${doxyfile} @ONLY IMMEDIATE)
  ADD_CUSTOM_TARGET(doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
ENDIF(BUILD_DOCUMENTATION)


# ---------------------------
# Setup Build
# ---------------------------

# Find Sources
FILE(GLOB_RECURSE SRCS "src/*.cpp")

INCLUDE_DIRECTORIES("include" ${FMILIB_INCLUDE_DIR} ${OPENSCENEGRAPH_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${RAPIDXML_DIR} ${OMC_MATLAB_READER_INC})
ADD_EXECUTABLE(OMVis ${SRCS} ${OMC_MATLAB_READER_C})
TARGET_LINK_LIBRARIES(OMVis ${FMILIB_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${SDL2_LIBRARY} ${Boost_LIBRARIES} ${LIBRARIES_EXTRA} Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Core)

#MESSAGE(STATUS "SDL2 LIBS: ${SDL2_LIBRARY}")
#MESSAGE(STATUS "OSG LIBS: ${OPENSCENEGRAPH_LIBRARIES}")