CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(OMVIS)

SET(CMAKE_VERBOSE_MAKEFILE ON)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
MESSAGE(STATUS "Using Cmake version ${CMAKE_VERSION} and ${CMAKE_MODULE_PATH}")


# ---------------------------
# Find All Dependencies
# ---------------------------

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)
# Detect and add SFML
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
SET(CMAKE_PREFIX_PATH "D://Software//Qt//5.5//msvc2012")

# Find the QtWidgets library
FIND_PACKAGE(Qt5Widgets)
IF(NOT(Qt5Widgets_FOUND))
  MESSAGE(FATAL_ERROR "Qt5Widgets library not found but required.")
ELSE(Qt5Widgets_FOUND)
  MESSAGE(STATUS "Qt5Widgets library found.")
ENDIF(NOT(Qt5Widgets_FOUND))

# Find the Qt5OpenGL library
FIND_PACKAGE(Qt5OpenGL)
IF(NOT(Qt5OpenGL_FOUND))
  MESSAGE(FATAL_ERROR "Qt5OpenGL library not found but required.")
ELSE(Qt5OpenGL_FOUND)
  MESSAGE(STATUS "Qt5OpenGL library found.")
ENDIF(NOT(Qt5OpenGL_FOUND))

# Find the QtGui library
FIND_PACKAGE(Qt5Gui)
IF(NOT(Qt5Gui_FOUND))
  MESSAGE(FATAL_ERROR "Qt5Gui library not found but required.")
ELSE(Qt5Gui_FOUND)
  MESSAGE(STATUS "Qt5Gui library found.")
ENDIF(NOT(Qt5Gui_FOUND))

# Find the Qt5Core library
FIND_PACKAGE(Qt5Core)
IF(NOT(Qt5Core_FOUND))
  MESSAGE(FATAL_ERROR "Qt5Core library not found but required.")
ELSE(Qt5Core_FOUND)
  MESSAGE(STATUS "Qt5Core library found.")
ENDIF(NOT(Qt5Core_FOUND))

# Find FMI library
FIND_PACKAGE(FMILib)
IF(NOT(FMILIB_FOUND))
  MESSAGE(FATAL_ERROR "FMI library not found but required. Set FMILIB_HOME to the location containing lib and include folder.")
ENDIF(NOT(FMILIB_FOUND))

# Find OpenSceneGraph
FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgViewer osgUtil osgGA osgQt)
IF(NOT(OPENSCENEGRAPH_FOUND))
  MESSAGE(FATAL_ERROR "OpenSceneGraph not found but required.")
ENDIF()

# Find SDL2
FIND_PACKAGE(SDL2 REQUIRED sdl2)
IF(NOT(SDL2_FOUND))
  MESSAGE(FATAL_ERROR "SDL2 not found but required.")
ELSEIF(SDL2_FOUND)
  MESSAGE(STATUS "SDL2 FOUND and ${SDL2_INCLUDE_DIR} as well as ${SDL2_LIBRARIES}")
ENDIF()

# Find Doxygen
FIND_PACKAGE(Doxygen)

# Find Boost
FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options filesystem system)
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost enabled")
ELSE(Boost_FOUND)
  MESSAGE (FATAL_ERROR "Boost required")
ENDIF(Boost_FOUND)

# Check for C++11 Support
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
IF(COMPILER_SUPPORTS_CXX11)
  ADD_COMPILE_OPTIONS(-std=c++11 -Wall)
ELSEIF(COMPILER_SUPPORTS_CXX0X)
  ADD_COMPILE_OPTIONS(-std=c++0x -Wall)
ELSE()
  MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

# Check for Compiler Warnings
CHECK_CXX_COMPILER_FLAG("-Wextra -Wunused" COMPILER_SUPPORTS_WEXTRA)
IF(COMPILER_SUPPORTS_WEXTRA)
  ADD_COMPILE_OPTIONS(-Wextra -Wunused)
ENDIF()

# Find rapidxml
#MESSAGE (STATUS "RapidXML ${RAPIDXML_ROOT}")
FIND_PACKAGE(RapidXML REQUIRED)


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
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  ENDIF()
ELSEIF(UNIX)
  #ADD_EXECUTABLE(Test Test.cpp) # Tell the project how to behave in this environment
ELSE()
  MESSAGE(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

MESSAGE(STATUS "Libraries extra: ${LIBRARIES_EXTRA} ...")


# ---------------------------
# Find Sources
# ---------------------------

FILE(GLOB_RECURSE SRCS "src/*.cpp")
#FILE(GLOB_RECURSE HEADERS "include/*/*.hpp")
ADD_DEFINITIONS(-DUSE_LOGGER)


# ---------------------------
# Build Documentation
# ---------------------------

IF(BUILD_DOCUMENTATION)
  IF(NOT(DOXYGEN_FOUND))
      MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()
  MESSAGE(STATUS "Doxygen is used to build the documentation;-)")
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
ENDIF()


# ---------------------------
# Setup Build
# ---------------------------

INCLUDE_DIRECTORIES("include" ${FMILIB_INCLUDE_DIR} ${OPENSCENEGRAPH_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} 
${Boost_INCLUDE_DIRS} ${RAPIDXML_DIR})
ADD_EXECUTABLE(OMVIS ${SRCS})
TARGET_LINK_LIBRARIES(OMVIS ${FMILIB_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${SDL2_LIBRARY} ${Boost_LIBRARIES} 
${LIBRARIES_EXTRA} Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Core)

MESSAGE(STATUS "SDL2 LIBS: ${SDL2_LIBRARY}")
MESSAGE(STATUS "OSG LIBS: ${OPENSCENEGRAPH_LIBRARIES}")