PROJECT(tests)
SET(PROJECT_VERSION 1.0)
cmake_minimum_required(VERSION 2.6)
include(FindPkgConfig)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb -DSANDBOX")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ggdb -DSANDBOX")

add_subdirectory(pc)
add_subdirectory(stubs)
add_subdirectory(robot)

