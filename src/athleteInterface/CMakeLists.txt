# Copyright: (C) 2015 CoDyCo project
# Author: Francesco Nori
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Make sure that YARP_DIR is set in your environment to wherever you 
# compiled YARP.

cmake_minimum_required(VERSION 2.8.9)

# Start a project.
PROJECT(run_athletebot)

# Define the debug options
option(ENABLE_DEBUG boolean)
if (ENABLE_DEBUG)
add_definitions(-D_ENABLE_DEBUG_)
endif()

# Define the control options
option(DISABLE_IO boolean)
if (DISABLE_IO)
add_definitions(-D_DISABLE_IO_)
endif()

# find YARP
FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

# find BBIO
FIND_LIBRARY(BBIO_LIBRARY libBBBio.a)
FIND_PATH(BBIO_INCLUDE BBBiolib.h DOC "Location of the include for BBBiolib.h")
# find BBIO
FIND_LIBRARY(GPIO_LIBRARY libgpiommap.a)
FIND_PATH(GPIO_INCLUDE gpio.h DOC "Location of the include for gpio.h")

SET(ENABLE_athletebot TRUE CACHE BOOLEAN "we want to use athletebot")

# Search for source code.
file( GLOB ATH_SOURCES src/*.cpp )
file( GLOB ATH_HEADERS include/*.h )

SET(folder_source ${ATH_SOURCES} )
SET(folder_header ${ATH_HEADERS} )
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${BBIO_INCLUDE} ${GPIO_INCLUDE})

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# Set up our main executable.
IF (folder_source)
  ADD_EXECUTABLE(run_athletebot ${folder_source} ${folder_header})
  TARGET_LINK_LIBRARIES(run_athletebot ${YARP_LIBRARIES} ${BBIO_LIBRARY} ${GPIO_LIBRARY})
  INSTALL(TARGETS run_athletebot DESTINATION bin/ PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE SETGID)
  install(CODE "execute_process(COMMAND /bin/chown root ${CMAKE_INSTALL_PREFIX}/bin/run_athletebot)")
  install(CODE "execute_process(COMMAND /bin/chmod 4755 ${CMAKE_INSTALL_PREFIX}/bin/run_athletebot)")
ELSE (folder_source)
  MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)
