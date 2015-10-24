# Copyright: (C) 2015 CoDyCo project
# Author: Francesco Nori
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Make sure that YARP_DIR is set in your environment to wherever you 
# compiled YARP.

cmake_minimum_required(VERSION 2.8.9)

# Start a project.
PROJECT(run_athletebot)

# find YARP
FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

# find BBIO
FIND_LIBRARY(BBIO_LIBRARY libBBBio.a)

SET(ENABLE_athletebot TRUE CACHE BOOLEAN "we want to use athletebot")

# Search for source code.
file( GLOB ATH_SOURCES src/*.cpp )
file( GLOB ATH_HEADERS include/*.h )

SET(folder_source ${ATH_SOURCES} adder.cpp add_athletebot.cpp yarpdev.cpp)
SET(folder_header ${ATH_HEADERS} adder.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

# Set up our main executable.
IF (folder_source)
  ADD_EXECUTABLE(run_athletebot ${folder_source} ${folder_header})
  TARGET_LINK_LIBRARIES(run_athletebot ${YARP_LIBRARIES} ${BBIO_LIBRARY})
  INSTALL_TARGETS(/bin run_athletebot)
ELSE (folder_source)
  MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)
