QT -= core gui

TARGET = neurosdk
TEMPLATE = lib
CONFIG += shared

###################
##DEFINES SECTION##
###################
DEFINES += LIBNEUROSDK_LIBRARY
DEFINES += GSL_THROW_ON_CONTRACT_VIOLATION
win32 {
    DEFINES += EXPORT_SYMBOLS
}
CONFIG(release, debug|release){    
    DEFINES += NDEBUG
}
#####################


########################
##BUILD CONFIG SECTION##
########################
CONFIG += c++14
!msvc {
    QMAKE_CXXFLAGS += -Wall -pedantic-errors
    QMAKE_CXXFLAGS_RELEASE += -O3
}
msvc{
    QMAKE_CXXFLAGS_DEBUG = /MDd
    QMAKE_CXXFLAGS_RELEASE = /O2 /MD
}
build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
}
########################


#########################
##INCLUDE PATHS SECTION##
#########################
INCLUDEPATH += ../../../../core/include
INCLUDEPATH += ../../../../utils/network/
INCLUDEPATH += ../../../../utils/gsl/include
INCLUDEPATH += ../../../../utils/dsp/include
#############################


###################
##HEADERS SECTION##
###################
HEADERS += $$files(../../../../core/include/*.h)
HEADERS += $$files(../../../../core/include/ble/*.h)
HEADERS += $$files(../../../../core/include/ble/emulator/*.h)
HEADERS += $$files(../../../../core/include/channels/*.h)
HEADERS += $$files(../../../../core/include/channels/info/*.h)
HEADERS += $$files(../../../../core/include/device/*.h)
HEADERS += $$files(../../../../core/include/device_scanner/*.h)
HEADERS += $$files(../../../../core/include/device/brainbit/*.h)
HEADERS += $$files(../../../../core/include/device/callibri/*.h)
HEADERS += $$files(../../../../core/include/signal/*.h)
HEADERS += $$files(../../../../utils/network/*.h)
HEADERS += $$files(../../../../utils/dsp/include/*.h)
HEADERS += $$files(../../../../utils/dsp/include/filter/*.h)
HEADERS += $$files(../../../../utils/gsl/include/gsl/*)
win32 {
    HEADERS += $$files(../../../../core/include/ble/win/*.h)
    HEADERS += $$files(../../../../utils/network/windows/*.h)
}
linux:!android:!ios:!macx {
    HEADERS += $$files(../../../../core/include/ble/linux/*.h)
    HEADERS += $$files(../../../../utils/network/posix/*.h)
}
android {
    HEADERS += $$files(../../../../core/include/ble/android/*.h)
    HEADERS += $$files(../../../../utils/network/posix/*.h)
}
ios:macx {
    HEADERS += $$files(../../../../core/include/ble/ios/*.h)
    HEADERS += $$files(../../../../utils/network/posix/*.h)
}
##################


###################
##SOURCES SECTION##
###################
SOURCES += $$files(../../../../core/*.cpp)
SOURCES += $$files(../../../../core/ble/*.cpp)
SOURCES += $$files(../../../../core/ble/emulator/*.cpp)
SOURCES += $$files(../../../../core/channels/*.cpp)
SOURCES += $$files(../../../../core/channels/info/*.cpp)
SOURCES += $$files(../../../../core/device/*.cpp)
SOURCES += $$files(../../../../core/device_scanner/*.cpp)
SOURCES += $$files(../../../../core/device/brainbit/*.cpp)
SOURCES += $$files(../../../../core/device/callibri/*.cpp)
SOURCES += $$files(../../../../utils/network/*.cpp)
linux:!android:!ios:!macx {
    SOURCES+=$$files(../../../../core/ble/linux/*.cpp)
    SOURCES+=$$files(../../../../core/device_scanner/cpp/*.cpp)
    SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}
android {
    SOURCES+=$$files(../../../../core/ble/android/*.cpp)
    SOURCES+=$$files(../../../../core/device_scanner/cpp/*.cpp)
    SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}
ios:macx {
    SOURCES+=$$files(../../../../core/ble/ios/*.mm)
    SOURCES+=$$files(../../../../core/device_scanner/mm/*.mm)
    SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}
win32 {
    SOURCES+=$$files(../../../../core/ble/win/*.cpp)
    SOURCES+=$$files(../../../../core/device_scanner/cpp/*.cpp)
    SOURCES += $$files(../../../../utils/network/windows/*.cpp)
}
##################


##########################
##EXPORT HEADERS SECTION##
##########################
shared_headers.files = ../../../../core/include/common_types.h
shared_headers.files += ../../../../core/include/event_listener.h
shared_headers.files += ../../../../core/include/event_notifier.h
shared_headers.files += ../../../../core/include/loop.h
shared_headers.files += ../../../../core/include/logger.h
shared_headers.files += ../../../../core/include/task_queue.h
shared_headers.files += ../../../../core/include/lib_export.h
shared_headers_signal.files += ../../../../core/include/signal/*.h
shared_headers_channels.files += ../../../../core/include/channels/base_channel.h
shared_headers_channels.files += ../../../../core/include/channels/battery_channel.h
shared_headers_channels.files += ../../../../core/include/channels/connection_stats_channel.h
shared_headers_channels.files += ../../../../core/include/channels/electrode_state_channel.h
shared_headers_channels.files += ../../../../core/include/channels/mems_channel.h
shared_headers_channels.files += ../../../../core/include/channels/orientation_channel.h
shared_headers_channels.files += ../../../../core/include/channels/resistance_channel.h
shared_headers_channels.files += ../../../../core/include/channels/respiration_channel.h
shared_headers_channels.files += ../../../../core/include/channels/signal_channel.h
shared_headers_channels.files += ../../../../core/include/channels/spectrum_channel.h
shared_headers_channels_info.files += ../../../../core/include/channels/info/channel_info.h
shared_headers_channels_info.files += ../../../../core/include/channels/info/mems_data.h
shared_headers_channels_info.files += ../../../../core/include/channels/info/quaternion.h
shared_headers_channels_info.files += ../../../../core/include/channels/info/electrode_state.h
shared_headers_device.files += ../../../../core/include/device/device.h
shared_headers_device.files += ../../../../core/include/device/device_parameters.h
shared_headers_device.files += ../../../../core/include/device/ma_device_params.h
shared_headers_device.files += ../../../../core/include/device/mems_device_params.h
shared_headers_device.files += ../../../../core/include/device/param_values.h
shared_headers_device.files += ../../../../core/include/device/signal_device_params.h
shared_headers_device_scanner.files += ../../../../core/include/device_scanner/device_scanner.h
shared_headers_device_scanner.files += ../../../../core/include/device_scanner/scanner_factory.h
shared_headers_filter.files += ../../../../utils/dsp/include/filter/*.h
#########################


##################
##DEPLOY SECTION##
##################
linux:!android {
    neurosdk.path = /usr/lib
    INSTALLS += neurosdk
    LIBS += -lbluetooth -lgattlib
}
win32 {
    shared_headers.path = $$(NEUROSDK)/include
    shared_headers_channels.path = $$(NEUROSDK)/include/channels
    shared_headers_channels_info.path = $$(NEUROSDK)/include/channels/info
    shared_headers_device.path = $$(NEUROSDK)/include/device
    shared_headers_device_scanner.path = $$(NEUROSDK)/include/device_scanner
    shared_headers_filter.path = $$(NEUROSDK)/include/filter
    shared_headers_signal.path = $$(NEUROSDK)/include/signal
    target.path = $$(NEUROSDK)/$$ARCH
    INSTALLS += target shared_headers shared_headers_channels shared_headers_channels_info shared_headers_device shared_headers_device_scanner shared_headers_filter shared_headers_signal
    LIBS += -lws2_32
    LIBS += -lSetupApi
    LIBS += -lBluetoothAPIs
}
##################
