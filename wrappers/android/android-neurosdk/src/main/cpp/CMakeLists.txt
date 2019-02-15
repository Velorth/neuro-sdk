cmake_minimum_required(VERSION 3.4.1)

project(android-neurosdk VERSION 1.4.0 LANGUAGES CXX)

SET (TARGET_PLATFORM ${CMAKE_ANDROID_ARCH_ABI})
SET (PLATFORM_SUFFIX "")
SET (TARGET_OS android)

SET (NEUROSDK $ENV{NEUROSDK}/${TARGET_OS}/${TARGET_PLATFORM})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fexceptions -fno-rtti -pedantic-errors")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(${NEUROSDK}/include)
include_directories(${NEUROSDK}/include/c-wrap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/jni-support)
link_directories(${NEUROSDK})

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SDKLIB_FILES ${SDK_LINK_DIR}/libneurosdk.so)

add_library(android-neurosdk SHARED ${SOURCE_FILES})
target_link_libraries(android-neurosdk log neurosdk)

set(WRAPPER_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
file(MAKE_DIRECTORY ${WRAPPER_LIBDIR})
add_custom_command(TARGET android-neurosdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SDKLIB_FILES} ${WRAPPER_LIBDIR})