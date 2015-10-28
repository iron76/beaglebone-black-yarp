# Copyright: (C) 2015 CoDyCo
# Authors: Francesco Nori
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.8.9)

project(beagle_bone_black_yarp)

execute_process(COMMAND git submodule update --init)

# execute_process(COMMAND git submodule foreach git pull origin master) 

add_subdirectory(src/libraries/BBBIOlib)

add_subdirectory(src/libraries/BeagleBone-GPIO)

add_subdirectory(src/modules/athleteInterface)


