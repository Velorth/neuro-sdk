QT -= core gui

TARGET = neurosdk
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LIBNEUROSDK_LIBRARY
DEFINES += GSL_THROW_ON_CONTRACT_VIOLATION
CONFIG(release, debug|release){    
    DEFINES += NDEBUG
}

INCLUDEPATH += ../../../../core/include
INCLUDEPATH += ../../../../utils/network/
INCLUDEPATH += ../../../../utils/gsl/include
linux:!android:!ios:!macx: INCLUDEPATH += ../../../../core/linux/include
android: INCLUDEPATH += ../../../../core/android/include
win32: INCLUDEPATH += ../../../core..//windows/include
ios: macx: INCLUDEPATH += ../../../../core/ios/include


CONFIG += c++14
!msvc:QMAKE_CXXFLAGS += -Wall -pedantic-errors

linux:!android {
    neurosdk.path = /usr/lib
    INSTALLS += neurosdk
    LIBS += -lbluetooth -lgattlib
}
win32 {
    neurosdk.path = $$(NEUROSDK)
    INSTALLS += neurosdk
    LIBS += -lws2_32
}

HEADERS += $$files(../../../../core/include/*.h)
HEADERS += $$files(../../../../core/include/ble/*.h)
HEADERS += $$files(../../../../core/include/ble/emulator/*.h)
HEADERS += $$files(../../../../core/include/channels/*.h)
HEADERS += $$files(../../../../core/include/device/*.h)
HEADERS += $$files(../../../../core/include/device/brainbit/*.h)
HEADERS += $$files(../../../../core/include/device/callibri/*.h)
HEADERS += $$files(../../../../core/include/signal/*.h)
HEADERS += $$files(../../../../utils/network/*.h)
HEADERS += $$files(../../../../utils/gsl/include/gsl/*)

win32{
HEADERS += $$files(../../../../core/include/ble/win/*.h)
HEADERS += $$files(../../../../utils/network/windows/*.h)
}

linux:!android:!ios:!macx:{
HEADERS += $$files(../../../../core/include/ble/linux/*.h)
HEADERS += $$files(../../../../utils/network/posix/*.h)
}

android{
HEADERS += $$files(../../../../core/include/ble/android/*.h)
HEADERS += $$files(../../../../utils/network/posix/*.h)
}

ios: macx:{
HEADERS += $$files(../../../../core/include/ble/ios/*.h)
HEADERS += $$files(../../../../utils/network/posix/*.h)
}

SOURCES += $$files(../../../../core/*.cpp)
SOURCES += $$files(../../../../core/ble/*.cpp)
SOURCES += $$files(../../../../core/ble/emulator/*.cpp)
SOURCES += $$files(../../../../core/channels/*.cpp)
SOURCES += $$files(../../../../core/device/*.cpp)
SOURCES += $$files(../../../../core/device/brainbit/*.cpp)
SOURCES += $$files(../../../../core/device/callibri/*.cpp)
SOURCES += $$files(../../../../utils/network/*.cpp)

linux:!android:!ios:!macx:{
SOURCES+=$$files(../../../../core/ble/linux/*.cpp)
SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}

android{
SOURCES+=$$files(../../../../core/ble/android/*.cpp)
SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}

ios: macx:{
SOURCES+=$$files(../../../../core/ble/ios/*.mm)
SOURCES += $$files(../../../../utils/network/posix/*.cpp)
}

win32:{
SOURCES+=$$files(../../../../core/ble/win/*.cpp)
SOURCES += $$files(../../../../utils/network/windows/*.cpp)
}
