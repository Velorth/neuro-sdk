QT -= core gui

TARGET = neurosdk
TEMPLATE = lib

DEFINES += LIBNEUROSDK_LIBRARY
CONFIG(release, debug|release){
    DEFINES += GSL_THROW_ON_CONTRACT_VIOLATION
    DEFINES += NDEBUG
}
CONFIG(debug, debug|release) {
    DEFINES += GSL_TERMINATE_ON_CONTRACT_VIOLATION
}

INCLUDEPATH += ../../core/common/include
INCLUDEPATH += "../../utils/network"
INCLUDEPATH += "../../utils/gsl/include"
linux:!android: INCLUDEPATH += ../../core/linux/include
android: INCLUDEPATH += ../../core/android/include
win32: INCLUDEPATH += ../../core/windows/include


CONFIG += c++14
!msvc:QMAKE_CXXFLAGS += -Wall -pedantic-errors

linux:!android {
    target.path = /usr/lib
    INSTALLS += target
    LIBS += -lbluetooth -lgattlib
}
win32:LIBS += -lws2_32

HEADERS += \
    ../../core/common/include/algorithm.h \
    ../../core/common/include/logger.h \
    ../../core/common/include/saturation_cast.h \
    ../../core/common/include/scanner_factory.h \
    ../../core/common/include/spectrum.h \
    ../../core/common/include/event_observer.h \
    ../../core/common/include/device/ble_device.h \
    ../../core/common/include/device/request_handler.h \
    ../../core/common/include/device/brainbit_command.h \
    ../../core/common/include/device/brainbit_protocol.h \
    ../../core/common/include/device/neuro_device.h \
    ../../core/common/include/method_connection.h \
    ../../core/common/include/method/ecg_device.h \
    ../../core/common/include/method/r_peak.h \
    ../../core/common/include/method/ma_device.h \
    ../../core/common/include/method/bfb_device.h \
    ../../core/common/include/signal/channel.h \
    ../../core/common/include/signal/filter_factory.h \
    ../../core/common/include/signal/signal_filter.h \
    ../../core/common/include/signal/signal_reader.h \
    ../../core/common/include/signal/signal_subsystem.h \
    ../../core/common/include/emulator_client.h \
    ../../core/common/include/device/emulated_device.h \
    ../../utils/network/net_types.h \
    ../../utils/network/socket.h \
    ../../utils/network/windows/win_socket.h \
    ../../utils/network/posix/posix_socket.h \
    ../../core/common/include/channels/base_channel.h \
    ../../core/common/include/common_types.h \
    ../../core/common/include/channels/channel_info.h \
    ../../core/common/include/device/device.h \
    ../../utils/gsl/include/gsl/gsl \
    ../../utils/gsl/include/gsl/gsl_algorithm \
    ../../utils/gsl/include/gsl/gsl_assert \
    ../../utils/gsl/include/gsl/gsl_byte \
    ../../utils/gsl/include/gsl/gsl_util \
    ../../utils/gsl/include/gsl/multi_span \
    ../../utils/gsl/include/gsl/pointers \
    ../../utils/gsl/include/gsl/span \
    ../../utils/gsl/include/gsl/string_span \
    ../../core/common/include/device/device_parameters.h \
    ../../core/common/include/subsystem/ma_device_params.h \
    ../../core/common/include/device/signal_device_params.h \
    ../../core/common/include/device/device_impl.h \
    ../../core/common/include/device/param_values.h \
    ../../core/common/include/device/command_data.h \
    ../../core/common/include/ble_scanner.h \
    ../../core/common/include/device_scanner.h \
    ../../core/common/include/device/ble_device_info.h \
    ../../core/common/include/device/parameter_reader.h \
    ../../core/common/include/device/callibri_parameter_reader.h \
    ../../core/common/include/device_factory.h \
    ../../core/common/include/device/callibri_impl.h \
    ../../core/common/include/device/callibri_command.h \
    ../../core/common/include/device/brainbit_impl.h \
    ../../core/common/include/device/brainbit_parameter_reader.h \
    ../../core/common/include/device/parameter_writer.h \
    ../../core/common/include/device/brainbit_parameter_writer.h \
    ../../core/common/include/device/callibri_common_parameters.h \
    ../../core/common/include/device/callibri_parameter_writer.h \
    ../../core/common/include/device/callibri_protocol.h \
    ../../core/common/include/channels/battery_channel.h \
    ../../core/common/include/channels/electrode_state_channel.h \
    ../../core/common/include/channels/signal_channel.h \
    ../../core/common/include/signal/circular_buffer.h \
    ../../core/common/include/signal/base_buffer.h \
    ../../core/common/include/signal/safe_buffer.h \
    ../../core/common/include/device/callibri_signal_buffer.h

win32{
HEADERS += ../../core/windows/include/ble_device_win.h
HEADERS += ../../core/windows/include/ble_scanner_win.h
}

linux:!android{
HEADERS += ../../core/linux/include/ble_device_z.h
HEADERS += ../../core/linux/include/device_scanner_z.h
}

android{
HEADERS += \
HEADERS += ../../core/android/include/connection/ble_device_jni.h
HEADERS += ../../core/android/include/connection/bluetooth_scanner.h
}

SOURCES += \
    ../../core/common/device/neuro_device.cpp \
    ../../core/common/device/colibri_device.cpp \
    ../../core/common/device/brainbit_device.cpp \
    ../../core/common/method/bfb_device.cpp \
    ../../core/common/method/ecg_device.cpp \
    ../../core/common/method/ecg_strategy_plain.cpp \
    ../../core/common/method/ecg_strategy_mathmod.cpp \
    ../../core/common/method/eeg_device.cpp \
    ../../core/common/method/eeg_state_calculator.cpp \
    ../../core/common/method/ma_device.cpp \
    ../../core/common/method/rpeak.cpp \
    ../../core/common/method/stimulation_wrap.cpp \
    ../../core/common/signal/artifact_zone.cpp \
    ../../core/common/signal/brainbit_reader.cpp \
    ../../core/common/signal/channel.cpp \
    ../../core/common/signal/signal_reader.cpp \
    ../../core/common/signal/simple_reader.cpp \
    ../../core/common/subsystem/brainbit_signal_subsystem.cpp \
    ../../core/common/subsystem/signal_subsystem.cpp \
    ../../core/common/subsystem/colibri_signal_subsystem.cpp \
    ../../core/common/subsystem/colibri_stimulation_subsystem.cpp \
    ../../core/common/logger.cpp \
    ../../core/common/spectrum.cpp \
    ../../core/windows/ble_device_win.cpp \
    ../../core/windows/ble_scanner_win.cpp \
    ../../core/common/emulator_client.cpp \
    ../../core/common/device/emulated_device.cpp \
    ../../utils/network/base_socket.cpp \
    ../../utils/network/tcp_socket.cpp \
    ../../utils/network/udp_socket.cpp \
    ../../utils/network/windows/win_socket.cpp \
    ../../utils/network/posix/posix_socket.cpp \
    ../../core/common/channels/channel_info.cpp \
    ../../core/common/device/device.cpp \
    ../../core/common/device_scanner.cpp \
    ../../core/common/device/ble_device_info.cpp \
    ../../core/common/device/param_values.cpp \
    ../../core/common/device/parameter_reader.cpp \
    ../../core/common/device/device_impl.cpp \
    ../../core/common/device_factory.cpp \
    ../../core/common/device/callibri_impl.cpp \
    ../../core/common/device/callibri_parameter_reader.cpp \
    ../../core/common/device/brainbit_impl.cpp \
    ../../core/common/device/brainbit_parameter_reader.cpp \
    ../../core/common/device/parameter_writer.cpp \
    ../../core/common/device/brainbit_parameter_writer.cpp \
    ../../core/common/device/callibri_common_parameters.cpp \
    ../../core/common/device/callibri_parameter_writer.cpp \
    ../../core/common/channels/battery_channel.cpp \
    ../../core/common/channels/electrode_state_channel.cpp \
    ../../core/common/channels/signal_channel.cpp

linux:!android{
SOURCES+=\
    ../../core/linux/ble_device_z.cpp \
    ../../core/linux/device_scanner_z.cpp
}

android{
SOURCES+=\
    ../../core/android/connection/ble_device_jni.cpp \
    ../../core/android/connection/bluetooth_scanner.cpp
}
