TARGET = harbour-batterybuddy-daemon

CONFIG = qt c++11 sailfish_build

QT = dbus network xml

HEADERS += \
    src/filewatcher.h

SOURCES += \
    src/filewatcher.cpp \
    src/harbour-batterybuddy-daemon.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

#include($$PWD/../common/common.pri)

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
