#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T12:00:39
#
#-------------------------------------------------

QT       -= core gui

TARGET = neurosdk
TEMPLATE = lib

DEFINES += LIBNEUROSDK_LIBRARY

INCLUDEPATH += ../../core/common/include
INCLUDEPATH += ../../core/linux/include

QMAKE_CXXFLAGS += -gdwarf-3
QMAKE_CXXFLAGS += -std=c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
    LIBS += -lbluetooth -lgattlib
}

HEADERS += \
    ../../core/common/include/algorithm.h \
    ../../core/common/include/method/base_method_device.h \
    ../../core/common/include/device/ble_device.h \
    ../../core/common/include/signal/channel.h \
    ../../core/common/include/device/colibri_protocol.h \
    ../../core/common/include/device/device_info.h \
    ../../core/common/include/device_scanner.h \
    ../../core/common/include/device/device_state.h \
    ../../core/common/include/method/ecg_device.h \
    ../../core/common/include/signal/filter_factory.h \
    ../../core/common/include/logger.h \
    ../../core/common/include/method_connection.h \
    ../../core/common/include/neuro_connection.h \
    ../../core/common/include/device/neuro_device.h \
    ../../core/common/include/method/r_peak.h \
    ../../core/common/include/saturation_cast.h \
    ../../core/common/include/scanner_factory.h \
    ../../core/common/include/signal/signal_buffer.h \
    ../../core/common/include/signal/signal_filter.h \
    ../../core/common/include/signal/signal_reader.h \
    ../../core/common/include/signal/signal_subsystem.h \
    ../../core/common/include/spectrum.h \
    ../../core/common/include/method/ma_device.h \
    ../../core/common/include/subsystem/stimulation_subsystem.h \
    ../../core/common/include/method/stimulation_wrap.h \
    ../../core/linux/include/ble_device_z.h \
    ../../core/linux/include/device_scanner_z.h \
    ../../core/common/include/method/bfb_device.h \
    ../../core/common/include/event_observer.h \
    ../../core/common/include/device/request_handler.h \
    ../../core/common/include/device/device_command.h \
    ../../core/common/include/device/colibri_command.h \
    ../../core/common/include/device/brainbit_command.h \
    ../../core/common/include/device/brainbit_protocol.h

SOURCES += \
    ../../core/common/device/device_info.cpp \
    ../../core/common/device/neuro_device.cpp \
    ../../core/common/device/colibri_device.cpp \
    ../../core/common/device/brainbit_device.cpp \
    ../../core/common/method/base_method_device.cpp \
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
    ../../core/common/signal/signal_buffer.cpp \
    ../../core/common/signal/signal_reader.cpp \
    ../../core/common/signal/simple_reader.cpp \
    ../../core/common/subsystem/brainbit_signal_subsystem.cpp \
    ../../core/common/subsystem/signal_subsystem.cpp \
    ../../core/common/subsystem/colibri_signal_subsystem.cpp \
    ../../core/common/subsystem/colibri_stimulation_subsystem.cpp \
    ../../core/common/logger.cpp \
    ../../core/common/neuro_connection.cpp \
    ../../core/common/spectrum.cpp \
    ../../core/linux/ble_device_z.cpp \
    ../../core/linux/device_scanner_z.cpp
