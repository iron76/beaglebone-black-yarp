cmake_minimum_required(VERSION 2.8)

project(gpio_athlete)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(gpio_athlete gpio_athlete.cpp)

install(TARGETS gpio_athlete DESTINATION bin/)


