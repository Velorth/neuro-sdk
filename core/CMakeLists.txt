cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# project name and supported languages
project(neurosdk CXX)

# detect operating system and host processor
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

add_library(neurosdk_object OBJECT ${SOURCE_FILES} ${HEADER_FILES})

add_library(neurosdk SHARED $<TARGET_OBJECTS:neurosdk_object>)
add_library(neurosdk_static STATIC $<TARGET_OBJECTS:neurosdk_object>)
