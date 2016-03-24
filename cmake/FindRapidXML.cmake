# This script locates the RapidXML library
# ------------------------------------
#
# usage:
# find_package(RAPIDXML ...)
#
# searches in RAPIDXML_ROOT and usual locations
#
# Sets RAPIDXML_DIR

# find the tinyxml directory find the SFML include directory
# message(STATUS "RapidXML ${RAPIDXML_ROOT}")
FIND_PATH (RAPIDXML_DIR rapidxml.hpp
          PATH_SUFFIXES include
          PATHS
          ${RAPIDXML_ROOT}
          $ENV{RapidXML_ROOT}
          ~/Library/Frameworks
          /Library/Frameworks
          /usr/local/
          /usr/
          /sw # Fink
          /opt/local/ # DarwinPorts
          /opt/csw/ # Blastwave
          /opt/)

IF (NOT RAPIDXML_DIR)
    IF (RapidXML_FIND_REQUIRED) #prefix is filename, case matters
        MESSAGE (FATAL_ERROR "Could not find RapidXML!")
    ELSEIF (NOT RapidXML_FIND_QUIETLY)
        MESSAGE ("Could not find RapidXML!")
    ENDIF (RapidXML_FIND_REQUIRED)
ENDIF (NOT RAPIDXML_DIR)

IF (NOT RapidXML_FIND_QUIETLY)
    MESSAGE("Found RapidXML: ${RAPIDXML_DIR}")
ENDIF (NOT RapidXML_FIND_QUIETLY)