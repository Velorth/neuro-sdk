cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)

# project name and supported languages
project(neurosdk VERSION 1.4.0 LANGUAGES CXX)

if (ANDROID)
  set (TARGET_PLATFORM ${CMAKE_ANDROID_ARCH_ABI})
  set (PLATFORM_SUFFIX "")
  set (TARGET_OS android)
else (ANDROID)
if (UNIX)

  if (APPLE)
    
  else (APPLE)

  endif (APPLE)
else (UNIX)
  if (WIN32)  
    if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        set (TARGET_PLATFORM "x86")     
    else ()
        set (TARGET_PLATFORM "x64")     
    endif()
        set (PLATFORM_SUFFIX -${TARGET_PLATFORM})
        set (TARGET_OS windows)
  endif (WIN32)
endif (UNIX)
endif (ANDROID)

SET (NEUROSDK $ENV{NEUROSDK}/${TARGET_OS}/${TARGET_PLATFORM})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################
##DEFINES SECTION##
###################
add_definitions(-DLIBNEUROSDK_LIBRARY)
if (WIN32)    
    add_definitions(-DEXPORT_SYMBOLS)
endif (WIN32)
#####################


########################
##BUILD CONFIG SECTION##
########################
if (WIN32)
	SET (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING
			"Debug;Release" FORCE)
	SET (RELEASE_BUILD_NAME "Release")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /MD")
else () 
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fexceptions -fno-rtti -pedantic-errors")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif (WIN32)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()
########################


#########################
##INCLUDE PATHS SECTION##
#########################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/c-wrap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/network)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/dsp/include)
if (ANDROID)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/jni-support)
endif()
#############################


###################
##HEADERS SECTION##
###################
file(GLOB HEADER_FILES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB HEADER_FILES_BLE ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/*.h)
file(GLOB HEADER_FILES_BLE_EMUL ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/emulator/*.h)
file(GLOB_RECURSE HEADER_FILES_CHANNELS ${CMAKE_CURRENT_SOURCE_DIR}/include/channels/*.h)
file(GLOB_RECURSE HEADER_FILES_DEVICE ${CMAKE_CURRENT_SOURCE_DIR}/include/device/*.h)
file(GLOB HEADER_FILES_DEVSCAN ${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/*.h)
file(GLOB HEADER_FILES_SIGNAL ${CMAKE_CURRENT_SOURCE_DIR}/include/signal/*.h)
file(GLOB HEADER_FILES_UTILS_NET ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/*.h)
file(GLOB_RECURSE HEADER_FILES_UTILS_DSP ${CMAKE_CURRENT_SOURCE_DIR}/../utils/dsp/include/*.h)
file(GLOB HEADER_FILES_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/include/c-wrap/*.h)

if (UNIX)
  if (APPLE)
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/ios/*.h)
    file(GLOB HEADER_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.h)
  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/linux/*.h)
    file(GLOB HEADER_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.h)
    else(DEBIAN_FOUND)
        if (ANDROID)
            file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/android/*.h)
            file(GLOB HEADER_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.h)
            file(GLOB HEADER_FILES_UTILS_JNI ${CMAKE_CURRENT_SOURCE_DIR}/../utils/jni-support/*.h)
        else (ANDROID)
    	    message(SEND_ERROR "Operating system is not supported")
    	endif (ANDROID)
    endif (DEBIAN_FOUND)    
  endif (APPLE)
endif (UNIX)
if (WIN32) 	
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/win/*.h)
    file(GLOB HEADER_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/windows/*.h)
endif (WIN32)
##################


###################
##SOURCES SECTION##
###################
file(GLOB SOURCE_FILES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_FILES_BLE ${CMAKE_CURRENT_SOURCE_DIR}/ble/*.cpp)
file(GLOB SOURCE_FILES_BLE_EMUL ${CMAKE_CURRENT_SOURCE_DIR}/ble/emulator/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CHANNELS ${CMAKE_CURRENT_SOURCE_DIR}/channels/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_DEVICE ${CMAKE_CURRENT_SOURCE_DIR}/device/*.cpp)
file(GLOB SOURCE_FILES_DEVSCAN ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/*.cpp)
file(GLOB SOURCE_FILES_UTILS_NET ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/*.cpp)
file(GLOB SOURCE_FILES_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/c-wrap/*.cpp)

if (UNIX)
  if (APPLE)
	file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/ios/*.mm)
    file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/mm/*.mm)
	file(GLOB SOURCE_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.cpp)
  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    	file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/linux/*.cpp)
    	file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/cpp/*.cpp)
    	file(GLOB SOURCE_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.cpp)
    else(DEBIAN_FOUND)
		    if (ANDROID) 
    		file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/android/*.cpp)
    		file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/cpp/*.cpp)
    		file(GLOB SOURCE_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/posix/*.cpp)
    		file(GLOB SOURCE_FILES_UTILS_JNI ${CMAKE_CURRENT_SOURCE_DIR}/../utils/jni-support/*.cpp)
        else (ANDROID)
    	    message(SEND_ERROR "Operating system is not supported")
		endif (ANDROID)
    endif (DEBIAN_FOUND)    
  endif (APPLE)
endif (UNIX)
if (WIN32) 	
    file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/win/*.cpp)
    file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/cpp/*.cpp)
    file(GLOB SOURCE_FILES_UTILS_NET_OS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/network/windows/*.cpp)
endif (WIN32)
##################

add_library(neurosdk${PLATFORM_SUFFIX} SHARED ${SOURCE_FILES_COMMON} 
							${SOURCE_FILES_BLE}  
							${SOURCE_FILES_BLE_OS}
							${SOURCE_FILES_BLE_EMUL} 
							${SOURCE_FILES_CHANNELS}  
							${SOURCE_FILES_DEVICE}
							${SOURCE_FILES_DEVSCAN}
							${SOURCE_FILES_DEVSCAN_OS} 
							${SOURCE_FILES_UTILS_NET}  
							${SOURCE_FILES_UTILS_NET_OS}
                            ${SOURCE_FILES_CWRAP}
							${SOURCE_FILES_UTILS_JNI}
							${HEADER_FILES_COMMON} 
							${HEADER_FILES_BLE} 
							${HEADER_FILES_BLE_OS} 
							${HEADER_FILES_BLE_EMUL}
							${HEADER_FILES_CHANNELS} 
							${HEADER_FILES_DEVICE} 
							${HEADER_FILES_DEVSCAN} 
							${HEADER_FILES_DEVSCAN_OS}
							${HEADER_FILES_SIGNAL} 
							${HEADER_FILES_UTILS_NET}
							${HEADER_FILES_UTILS_NET_OS}
							${HEADER_FILES_UTILS_DSP}
                            ${HEADER_FILES_CWRAP}
							${HEADER_FILES_UTILS_JNI})

##########################
##EXPORT HEADERS SECTION##
##########################
file(GLOB SHARED_HEADERS_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/include/common_types.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/event_listener.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/event_notifier.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/saturation_cast.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/loop.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/logger.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/task_queue.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/lib_export.h)
file(GLOB SHARED_HEADERS_CHANNELS 	${CMAKE_CURRENT_SOURCE_DIR}/include/channels/device_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/bipolar_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/filtered_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/spectrum_channel.h)
file(GLOB SHARED_HEADERS_CHANNELS_INFO 	${CMAKE_CURRENT_SOURCE_DIR}/include/channels/info/channel_info.h
										${CMAKE_CURRENT_SOURCE_DIR}/include/channels/info/mems_data.h
										${CMAKE_CURRENT_SOURCE_DIR}/include/channels/info/quaternion.h
										${CMAKE_CURRENT_SOURCE_DIR}/include/channels/info/electrode_state.h)
file(GLOB SHARED_HEADERS_DEVICE ${CMAKE_CURRENT_SOURCE_DIR}/include/device/device.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/device_parameters.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/ma_device_params.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/mems_device_params.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/param_values.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/signal_device_params.h)
file(GLOB SHARED_HEADERS_DEVSCAN 	${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/device_scanner.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/scanner_factory.h)
file(GLOB SHARED_HEADERS_FILTERS ${CMAKE_CURRENT_SOURCE_DIR}/../utils/dsp/include/filter/*.h)
file(GLOB SHARED_HEADERS_ALG ${CMAKE_CURRENT_SOURCE_DIR}/../utils/dsp/include/algorithm.h)
file(GLOB_RECURSE SHARED_HEADERS_SIGNAL ${CMAKE_CURRENT_SOURCE_DIR}/include/signal/*.h)
file(GLOB SHARED_HEADERS_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/include/c-wrap/*.h)
#########################

############################
##LINK AND INSTALL SECTION##
############################
if (UNIX)
  if (APPLE)

  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    	target_link_libraries(neurosdk${PLATFORM_SUFFIX} bluetooth gattlib)
    else(DEBIAN_FOUND)    	
		if (ANDROID)
    		target_link_libraries(neurosdk${PLATFORM_SUFFIX} android)
        else (ANDROID)
    	    message(SEND_ERROR "Operating system is not supported")
		endif (ANDROID)
    endif (DEBIAN_FOUND)    
  endif (APPLE)
endif (UNIX)
if (WIN32) 	
    target_link_libraries(neurosdk${PLATFORM_SUFFIX} WindowsApp)
endif (WIN32)

install(TARGETS neurosdk${PLATFORM_SUFFIX} DESTINATION ${NEUROSDK})
install(FILES ${SHARED_HEADERS_COMMON} DESTINATION ${NEUROSDK}/include)
install(FILES ${SHARED_HEADERS_CHANNELS} DESTINATION ${NEUROSDK}/include/channels)
install(FILES ${SHARED_HEADERS_CHANNELS_INFO} DESTINATION ${NEUROSDK}/include/channels/info)
install(FILES ${SHARED_HEADERS_DEVICE} DESTINATION ${NEUROSDK}/include/device)
install(FILES ${SHARED_HEADERS_DEVSCAN} DESTINATION ${NEUROSDK}/include/device_scanner)
install(FILES ${SHARED_HEADERS_FILTERS} DESTINATION ${NEUROSDK}/include/filter)
install(FILES ${SHARED_HEADERS_ALG} DESTINATION ${NEUROSDK}/include)
install(FILES ${SHARED_HEADERS_SIGNAL} DESTINATION ${NEUROSDK}/include/signal)
install(FILES ${SHARED_HEADERS_CWRAP} DESTINATION ${NEUROSDK}/include/c-wrap)
if (WIN32)  
install(FILES $<TARGET_PDB_FILE:neurosdk${PLATFORM_SUFFIX}> DESTINATION ${NEUROSDK} OPTIONAL)
endif (WIN32)
##################
