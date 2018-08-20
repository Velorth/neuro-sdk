cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

if (WIN32)
    if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		set (TARGET_PLATFORM "x86")
	else ()
		set (TARGET_PLATFORM "x64")
	endif()
endif (WIN32)

# project name and supported languages
project(neurosdk CXX)

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
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fno-rtti -pedantic-errors")
endif (WIN32)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;")

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()
########################


#########################
##INCLUDE PATHS SECTION##
#########################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/gsl/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/network)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/dsp/include)
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
file(GLOB HEADER_FILES_UTILS_GSL ${CMAKE_CURRENT_SOURCE_DIR}/../utils/gsl/include/gsl/*)

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

add_library(neurosdk SHARED ${SOURCE_FILES_COMMON} 
							${SOURCE_FILES_BLE}  
							${SOURCE_FILES_BLE_OS}
							${SOURCE_FILES_BLE_EMUL} 
							${SOURCE_FILES_CHANNELS}  
							${SOURCE_FILES_DEVICE}
							${SOURCE_FILES_DEVSCAN}
							${SOURCE_FILES_DEVSCAN_OS} 
							${SOURCE_FILES_UTILS_NET}  
							${SOURCE_FILES_UTILS_NET_OS}
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
							${HEADER_FILES_UTILS_GSL})

##########################
##EXPORT HEADERS SECTION##
##########################
file(GLOB SHARED_HEADERS_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/include/common_types.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/event_listener.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/event_notifier.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/loop.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/logger.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/task_queue.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/lib_export.h)
file(GLOB SHARED_HEADERS_CHANNELS 	${CMAKE_CURRENT_SOURCE_DIR}/include/channels/base_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/battery_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/connection_stats_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/electrode_state_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/mems_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/orientation_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/resistance_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/respiration_channel.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/channels/signal_channel.h
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
file(GLOB_RECURSE SHARED_HEADERS_SIGNAL ${CMAKE_CURRENT_SOURCE_DIR}/include/signal/*.h)
#########################


############################
##LINK AND INSTALL SECTION##
############################
if (UNIX)
  if (APPLE)
	message(FATAL_ERROR "NOT IMPLEMENTED")
  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    	target_link_libraries(neurosdk bluetooth gattlib)
    else(DEBIAN_FOUND)    	
		if (ANDROID) 
    		target_link_libraries(neurosdk log android)
        else (ANDROID)
    	    message(SEND_ERROR "Operating system is not supported")
		endif (ANDROID)
    endif (DEBIAN_FOUND)    
  endif (APPLE)
endif (UNIX)
if (WIN32) 	
    target_link_libraries(neurosdk ws2_32 setupapi bluetoothapis)
    install(TARGETS neurosdk DESTINATION $ENV{NEUROSDK}/${TARGET_PLATFORM})
    install(FILES ${SHARED_HEADERS_COMMON} DESTINATION $ENV{NEUROSDK}/include)
    install(FILES ${SHARED_HEADERS_CHANNELS} DESTINATION $ENV{NEUROSDK}/include/channels)
    install(FILES ${SHARED_HEADERS_CHANNELS_INFO} DESTINATION $ENV{NEUROSDK}/include/channels/info)
    install(FILES ${SHARED_HEADERS_DEVICE} DESTINATION $ENV{NEUROSDK}/include/device)
    install(FILES ${SHARED_HEADERS_DEVSCAN} DESTINATION $ENV{NEUROSDK}/include/device_scanner)
    install(FILES ${SHARED_HEADERS_FILTERS} DESTINATION $ENV{NEUROSDK}/include/filter)
    install(FILES ${SHARED_HEADERS_SIGNAL} DESTINATION $ENV{NEUROSDK}/include/signal)
endif (WIN32)
##################
