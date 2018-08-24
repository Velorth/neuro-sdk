cmake_minimum_required(VERSION 3.4.1)

project(android-neurosdk CXX)

add_definitions(-DGSL_THROW_ON_CONTRACT_VIOLATION)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti -pedantic-errors")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../share/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/jni-support)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../share/${ANDROID_ABI})

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(android-neurosdk SHARED ${SOURCE_FILES})
target_link_libraries(android-neurosdk log)
target_link_libraries(android-neurosdk debug neurosdkd)
target_link_libraries(android-neurosdk optimized neurosdk)