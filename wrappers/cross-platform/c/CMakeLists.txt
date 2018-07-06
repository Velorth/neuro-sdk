cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(c-neurosdk C CXX)

###################
##DEFINES SECTION##
###################
add_definitions(-DLIBNEUROSDK_LIBRARY)
add_definitions(-DGSL_THROW_ON_CONTRACT_VIOLATION)
if (WIN32)    
    add_definitions(-DEXPORT_SYMBOLS)
endif (WIN32)
#####################


########################
##BUILD CONFIG SECTION##
########################
if (WIN32) 
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /MD")
else () 
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti")
endif (WIN32)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;")

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()
########################


#########################
##INCLUDE SECTION##
#########################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if (WIN32)
    include_directories($ENV{NEUROSDK}/include)
    link_directories($ENV{NEUROSDK})
endif (WIN32)
#########################

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(c-neurosdk SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(c-neurosdk debug neurosdkd)
target_link_libraries(c-neurosdk optimized neurosdk)