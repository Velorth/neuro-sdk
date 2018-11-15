cmake_minimum_required(VERSION 3.4.1)

project(android-neurosdk CXX)

add_definitions(-DGSL_THROW_ON_CONTRACT_VIOLATION)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti -pedantic-errors")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../share/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/jni-support)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/gsl/include)
set(SDK_LINK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../share/${ANDROID_ABI})
link_directories(${SDK_LINK_DIR})

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SDKLIB_FILES ${SDK_LINK_DIR}/libneurosdk.so)

add_library(android-neurosdk SHARED ${SOURCE_FILES})
target_link_libraries(android-neurosdk log)
target_link_libraries(android-neurosdk neurosdk)

set(WRAPPER_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
file(MAKE_DIRECTORY ${WRAPPER_LIBDIR})
add_custom_command(TARGET android-neurosdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SDKLIB_FILES} ${WRAPPER_LIBDIR})