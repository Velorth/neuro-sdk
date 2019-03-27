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
  if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -fexceptions -fno-rtti -pedantic-errors")
   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") 
  else(APPLE)
	 SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fexceptions -fno-rtti -pedantic-errors")
	 SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") 
  endif(APPLE) 
endif (WIN32)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()
########################


#########################
##INCLUDE PATHS SECTION##
#########################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/c-wrap/include)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extensions)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extensions/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extensions/include/c-wrap)
ENDIF()
if (ANDROID)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../utils/jni-support)
endif()
#############################


###################
##HEADERS SECTION##
###################
file(GLOB HEADER_FILES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB HEADER_FILES_BLE ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/*.h)
file(GLOB_RECURSE HEADER_FILES_CHANNELS ${CMAKE_CURRENT_SOURCE_DIR}/include/channels/*.h)
file(GLOB_RECURSE HEADER_FILES_DEVICE ${CMAKE_CURRENT_SOURCE_DIR}/include/device/*.h)
file(GLOB HEADER_FILES_DEVSCAN ${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/*.h)
file(GLOB HEADER_FILES_SIGNAL ${CMAKE_CURRENT_SOURCE_DIR}/include/signal/*.h)
file(GLOB HEADER_FILES_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/c-wrap/include/*.h)


if (UNIX)
  if (APPLE)
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/ios/*.h)
    file(GLOB HEADER_FILES_BLE_OS_NEW ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/mac/*.h)
    set(HEADER_FILES_BLE_OS ${HEADER_FILES_BLE_OS} ${HEADER_FILES_BLE_OS_NEW})
  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/linux/*.h)
    else(DEBIAN_FOUND)
        if (ANDROID)
            file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/android/*.h)
            file(GLOB HEADER_FILES_UTILS_JNI ${CMAKE_CURRENT_SOURCE_DIR}/../utils/jni-support/*.h)
        else (ANDROID)
    	    message(SEND_ERROR "Operating system is not supported")
    	endif (ANDROID)
    endif (DEBIAN_FOUND)    
  endif (APPLE)
endif (UNIX)
if (WIN32) 	
    file(GLOB HEADER_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/win/*.h)
endif (WIN32)
##################


###################
##SOURCES SECTION##
###################
file(GLOB SOURCE_FILES_COMMON ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_FILES_BLE ${CMAKE_CURRENT_SOURCE_DIR}/ble/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_CHANNELS ${CMAKE_CURRENT_SOURCE_DIR}/channels/*.cpp)
file(GLOB_RECURSE SOURCE_FILES_DEVICE ${CMAKE_CURRENT_SOURCE_DIR}/device/*.cpp)
file(GLOB SOURCE_FILES_DEVSCAN ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/*.cpp)
file(GLOB SOURCE_FILES_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/c-wrap/*.cpp)

if (UNIX)
  if (APPLE)
	file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/ios/*.mm)
  file(GLOB SOURCE_FILES_BLE_OS_NEW ${CMAKE_CURRENT_SOURCE_DIR}/ble/mac/*.mm)
  set(SOURCE_FILES_BLE_OS ${SOURCE_FILES_BLE_OS} ${SOURCE_FILES_BLE_OS_NEW})
  file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/mm/*.mm)
  else (APPLE)
    ## Check for Debian GNU/Linux
    find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
      )
    if (DEBIAN_FOUND)
    	file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/linux/*.cpp)
    	file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/cpp/*.cpp)
    else(DEBIAN_FOUND)
		    if (ANDROID) 
    		file(GLOB SOURCE_FILES_BLE_OS ${CMAKE_CURRENT_SOURCE_DIR}/ble/android/*.cpp)
    		file(GLOB SOURCE_FILES_DEVSCAN_OS ${CMAKE_CURRENT_SOURCE_DIR}/device_scanner/cpp/*.cpp)
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
endif (WIN32)
##################

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extensions)
	add_subdirectory(extensions)
ENDIF()

add_library(neurosdk${PLATFORM_SUFFIX} SHARED 
			${SOURCE_FILES_COMMON} 
			${SOURCE_FILES_BLE}  
			${SOURCE_FILES_BLE_OS}
			${SOURCE_FILES_CHANNELS}  
			${SOURCE_FILES_DEVICE}
			${SOURCE_FILES_DEVSCAN}
			${SOURCE_FILES_DEVSCAN_OS} 
			${SOURCE_FILES_UTILS_JNI} 
			${EXT_SOURCES} 

			${HEADER_FILES_COMMON} 
			${HEADER_FILES_BLE} 
			${HEADER_FILES_BLE_OS} 
			${HEADER_FILES_CHANNELS} 
			${HEADER_FILES_DEVICE} 
			${HEADER_FILES_DEVSCAN} 
			${HEADER_FILES_DEVSCAN_OS}
			${HEADER_FILES_SIGNAL} 
			${HEADER_FILES_UTILS_JNI} 
			${SOURCE_FILES_CWRAP} 
			${HEADER_FILES_CWRAP}
			${EXT_HEADERS})

source_group(c-wrap FILES ${SOURCE_FILES_CWRAP})
source_group(c-wrap\\include FILES ${HEADER_FILES_CWRAP})
source_group(extensions FILES ${EXT_SOURCES})
source_group(extensions\\include FILES ${EXT_HEADERS})
source_group(device FILES ${SOURCE_FILES_DEVICE})
source_group(device\\include FILES ${HEADER_FILES_DEVICE})
source_group(device\\ble FILES ${SOURCE_FILES_BLE} ${SOURCE_FILES_BLE_OS})
source_group(device\\ble\\include FILES ${HEADER_FILES_BLE} ${HEADER_FILES_BLE_OS})
source_group(channels FILES ${SOURCE_FILES_CHANNELS})
source_group(channels\\include FILES ${HEADER_FILES_CHANNELS})

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extensions)
	
ENDIF()

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
                                ${CMAKE_CURRENT_SOURCE_DIR}/include/device/brainbit.h
                                ${CMAKE_CURRENT_SOURCE_DIR}/include/device/callibri.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/device_parameters.h
                                ${CMAKE_CURRENT_SOURCE_DIR}/include/device/device_info.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/ma_device_params.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/mems_device_params.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/param_values.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/device/signal_device_params.h)
file(GLOB SHARED_HEADERS_BLE ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/platform_traits.h)
file(GLOB SHARED_HEADERS_BLE_WIN ${CMAKE_CURRENT_SOURCE_DIR}/include/ble/win/windows_ble_enumerator.h
								${CMAKE_CURRENT_SOURCE_DIR}/include/ble/win/ble_device_address.h)                                  
file(GLOB SHARED_HEADERS_DEVSCAN 	${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/device_scanner.h
									${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/scanner_factory.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/device_enumerator.h
                                    ${CMAKE_CURRENT_SOURCE_DIR}/include/device_scanner/enumeration_list.h)
file(GLOB SHARED_HEADERS_FILTERS ${CMAKE_CURRENT_SOURCE_DIR}/include/filter/*.h)
file(GLOB_RECURSE SHARED_HEADERS_SIGNAL ${CMAKE_CURRENT_SOURCE_DIR}/include/signal/*.h)
file(GLOB SHARED_HEADERS_CWRAP ${CMAKE_CURRENT_SOURCE_DIR}/include/c-wrap/*.h)
#########################

############################
##LINK AND INSTALL SECTION##
############################
if (UNIX)
  if (APPLE)
    set_target_properties(neurosdk${PLATFORM_SUFFIX} PROPERTIES
      FRAMEWORK TRUE
      FRAMEWORK_VERSION C
      MACOSX_FRAMEWORK_IDENTIFIER com.neuromd.neurosdk
      #MACOSX_FRAMEWORK_INFO_PLIST Info.plist
      PUBLIC_HEADER neurosdk.h
      #XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    )
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


if (ANDROID) 
install(TARGETS neurosdk${PLATFORM_SUFFIX} RUNTIME DESTINATION ${NEUROSDK})
else(ANDROID)
install(TARGETS neurosdk${PLATFORM_SUFFIX} DESTINATION ${NEUROSDK})
endif(ANDROID)
install(FILES ${SHARED_HEADERS_COMMON} DESTINATION ${NEUROSDK}/include)
install(FILES ${SHARED_HEADERS_CHANNELS} DESTINATION ${NEUROSDK}/include/channels)
install(FILES ${SHARED_HEADERS_CHANNELS_INFO} DESTINATION ${NEUROSDK}/include/channels/info)
install(FILES ${SHARED_HEADERS_DEVICE} DESTINATION ${NEUROSDK}/include/device)
install(FILES ${SHARED_HEADERS_BLE} DESTINATION ${NEUROSDK}/include/ble)
install(FILES ${SHARED_HEADERS_BLE_WIN} DESTINATION ${NEUROSDK}/include/ble/win)
install(FILES ${SHARED_HEADERS_DEVSCAN} DESTINATION ${NEUROSDK}/include/device_scanner)
install(FILES ${SHARED_HEADERS_FILTERS} DESTINATION ${NEUROSDK}/include/filter)
install(FILES ${SHARED_HEADERS_SIGNAL} DESTINATION ${NEUROSDK}/include/signal)
install(FILES ${HEADER_FILES_CWRAP} DESTINATION ${NEUROSDK}/include/c-wrap)
if (WIN32)  
install(FILES $<TARGET_PDB_FILE:neurosdk${PLATFORM_SUFFIX}> DESTINATION ${NEUROSDK} OPTIONAL)
endif (WIN32)

##################
