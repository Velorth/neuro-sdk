cmake_minimum_required(VERSION 3.4.1)

project(android-neurosdk CXX)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../core/include)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_definitions(-DGSL_THROW_ON_CONTRACT_VIOLATION)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti -pedantic-errors")

add_library(android-neurosdk SHARED ${SOURCE_FILES})