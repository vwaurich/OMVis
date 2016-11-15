CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(OMVIS)

SET(CMAKE_VERBOSE_MAKEFILE ON)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#MESSAGE(STATUS "Using Cmake version ${CMAKE_VERSION} and ${CMAKE_MODULE_PATH}")

# Enable Logger
ADD_DEFINITIONS(-DUSE_LOGGER)


# ---------------------------
# Find All Dependencies
# ---------------------------
# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR OFF)
# Instruct CMake to run moc automatically when needed
# SET(CMAKE_AUTOMOC ON)
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
  MESSAGE(FATAL_ERROR "FMI library not found but required. Set FMILIB_HOME to the location containing lib and include 
                       folder of FMI library.")
ENDIF(FMILIB_FOUND)

# Find OpenSceneGraph
FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgViewer osgUtil osgGA osgQt)
IF(OPENSCENEGRAPH_FOUND)
  MESSAGE(STATUS "OpenSceneGraph found.")
ELSE(OPENSCENEGRAPH_FOUND)
  MESSAGE(FATAL_ERROR "OpenSceneGraph not found but required.")
ENDIF(OPENSCENEGRAPH_FOUND)

# Find SDL2
FIND_PACKAGE(SDL2 REQUIRED sdl2)
IF(SDL2_FOUND)
  MESSAGE(STATUS "SDL2 library found.")
ELSEIF(SDL2_FOUND)
  MESSAGE(FATAL_ERROR "SDL2 not found but required. Set SDL2DIR to the location containing lib and include folder of \
                       SDL2 library.")
ENDIF(SDL2_FOUND)
FIND_PACKAGE(SDL2_net)
IF(NOT(SDL2_NET_FOUND))
  MESSAGE(FATAL_ERROR "SDL2_net required")
ENDIF(NOT(SDL2_NET_FOUND))

# Find Boost
FIND_PACKAGE(Boost REQUIRED COMPONENTS filesystem program_options system)
IF(Boost_FOUND)
  MESSAGE(STATUS "Boost libraries found.")
ELSE(Boost_FOUND)
  MESSAGE (FATAL_ERROR "Boost libraries not found.")
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
  MESSAGE(STATUS "Found read_matlab4.h via OPENMODELICAHOME")
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
  MESSAGE(WARNING "OpenModelica not found. You can specify the environment variable OPENMODELICA_HOME pointing to your \
                   OpenModelica installation or pass the path to read_matlab4.h and read_matlab4.c via MATLABREADER to \
                   this CMakefile.")
  SET(MATLABREADER_FOUND NO)
ENDIF(DEFINED ENV{OPENMODELICAHOME})


# ---------------------------
# Compiler Flags
# ---------------------------
# Check for C++11 Support
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
IF(COMPILER_SUPPORTS_CXX14)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
ELSE(COMPILER_SUPPORTS_CXX14)
  MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
ENDIF(COMPILER_SUPPORTS_CXX14)

# Compiler Flags Used In Debug Mode
IF(CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_COMPILE_OPTIONS(-O0 -Weffc++)
  #-fno-elide-constructors -pedantic-errors -ansi -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized 
  #-Wmissing-declarations -Winit-self)

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
# Custom make targes
# ---------------------------

# Clean all
ADD_CUSTOM_TARGET(cleanAll
   COMMAND ${CMAKE_MAKE_PROGRAM} clean
   COMMAND ${CMAKE_COMMAND} -E chdir ./examples/ make -f ${PROJECT_BINARY_DIR}/examples/Makefile clean
)

# Clean in examples folder
ADD_CUSTOM_TARGET(cleanExamples
   COMMAND ${CMAKE_COMMAND} -E chdir ./examples/ make -f ${PROJECT_BINARY_DIR}/examples/Makefile clean
)

# Make examples: Copy examples files and execute Makefile that runs omc *mos
ADD_CUSTOM_TARGET(buildExamples
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/examples
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/examples ${PROJECT_BINARY_DIR}/examples
    COMMAND ${CMAKE_COMMAND} -E chdir ./examples/ make -f ${PROJECT_BINARY_DIR}/examples/Makefile build
    COMMENT "Build examples"
)



# ---------------------------
# Setup Build
# ---------------------------

# Dependencies
ADD_SUBDIRECTORY(thirdparty/gtest-1.7.0/)
ADD_SUBDIRECTORY(thirdparty/NetworkOffloader/)

# Find Sources
FILE(GLOB_RECURSE SRCS "src/*/*.cpp")

# Moc necessary files. Automoc does not work for us, since the header files containing the QObject macro and the 
# source files which include this files are in separated (sub)folders.
QT5_WRAP_CPP(MOC_OMVISVIEWER include/View/OMVISViewer.hpp)
QT5_WRAP_CPP(MOC_SETTINGSDIALOGS include/View/Dialogs/BackgroundColorSettingDialog.hpp)
QT5_WRAP_CPP(MOC_SETTINGSDIALOGS include/View/Dialogs/PerspectiveSettingDialog.hpp)
QT5_WRAP_CPP(MOC_SETTINGSDIALOGS include/View/Dialogs/OpenRemoteConnectionDialog.hpp)
QT5_WRAP_CPP(MOC_SETTINGSDIALOGS include/View/Dialogs/OpenFileDialog.hpp)
QT5_WRAP_CPP(MOC_SETTINGSDIALOGS include/View/Dialogs/OkCancelHelpButtonBox.hpp)


FILE(GLOB_RECURSE SRCS_TESTS "src/Model/*.cpp" "src/Control/*.cpp" "src/Initialization/*.cpp" "src/Util/*.cpp")

ADD_EXECUTABLE(OMVIS ${SRCS} ${MOC_SETTINGSDIALOGS} ${MOC_OMVISVIEWER} ${OMC_MATLAB_READER_C} "src/Main.cpp")
ADD_EXECUTABLE(OMVISTests EXCLUDE_FROM_ALL ${SRCS_TESTS} ${OMC_MATLAB_READER_C} "test/Main.cpp")

SET(INCLUDEDIRS "include")

# Handle this includes as system libraries. This suppresses compiler warnings.
INCLUDE_DIRECTORIES(SYSTEM ${FMILIB_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gtest-1.7.0/include"
                           "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/NetworkOffloader/include"
                           ${OPENSCENEGRAPH_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${RAPIDXML_DIR}
                           ${OMC_MATLAB_READER_INC}
                           ${SDL2_INCLUDE_DIR} ${SDL2_NET_INCLUDE_DIRS})

TARGET_INCLUDE_DIRECTORIES(OMVIS PRIVATE ${INCLUDEDIRS})
TARGET_INCLUDE_DIRECTORIES(OMVISTests PRIVATE ${INCLUDEDIRS} "test/include")

SET(LINKLIBRARIES ${FMILIB_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_NET_LIBRARIES} 
                  ${Boost_LIBRARIES} ${LIBRARIES_EXTRA} Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Core)
TARGET_LINK_LIBRARIES(OMVIS ${LINKLIBRARIES} "netoff")
TARGET_LINK_LIBRARIES(OMVISTests ${LINKLIBRARIES} "gtest" "netoff")

ADD_CUSTOM_COMMAND(TARGET OMVIS PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/OMVISLogo.osg ${PROJECT_BINARY_DIR})

