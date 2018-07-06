cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

set(SRC_DIR_CORE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../core)
set(SRC_DIR_GSL ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/gsl)
set(SRC_DIR_NET ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/network)
set(SRC_DIR_DSP ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../utils/dsp)
set(SRC_DIR_ANDROID ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES_CORE_CHANNELS ${SRC_DIR_CORE}/channels/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CORE_DEVICE ${SRC_DIR_CORE}/device/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CORE_SCANNER ${SRC_DIR_CORE}/device_scanner/*.cpp)
file(GLOB SOURCE_FILES_CORE_COMMON ${SRC_DIR_CORE}/*.cpp)
file(GLOB SOURCE_FILES_CORE_BLE_COMMON ${SRC_DIR_CORE}/ble/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CORE_BLE_EMUL ${SRC_DIR_CORE}/ble/emulator/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CORE_BLE_ANDROID ${SRC_DIR_CORE}/ble/android/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_ANDROID ${SRC_DIR_ANDROID}/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_NET ${SRC_DIR_NET}/*.cpp)
include_directories(${SRC_DIR_CORE}/include)
include_directories(${SRC_DIR_GSL}/include)
include_directories(${SRC_DIR_NET})
include_directories(${SRC_DIR_ANDROID}/include)
include_directories(${SRC_DIR_DSP}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti")
add_definitions(-DGSL_THROW_ON_CONTRACT_VIOLATION)
add_library(neurosdk SHARED  ${SOURCE_FILES_CORE_COMMON}
                                ${SOURCE_FILES_CORE_CHANNELS}
                                ${SOURCE_FILES_CORE_DEVICE}
                                ${SOURCE_FILES_CORE_SCANNER}
                                ${SOURCE_FILES_CORE_BLE_COMMON}
                                ${SOURCE_FILES_CORE_BLE_EMUL}
                                ${SOURCE_FILES_CORE_BLE_ANDROID}
                                ${SOURCE_FILES_ANDROID}
                                ${SOURCE_FILES_NET})
target_link_libraries(neurosdk log android)