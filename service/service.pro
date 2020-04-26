TARGET = harbour-batterybuddy-daemon

CONFIG = qt console c++11 sailfish_build

QT = core network

# Keep this in sync with "application.pro"
VER = 2.2
REL = 2

VERSION = $${VER}-$${REL}
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

HEADERS += \
    src/filewatcher.h

SOURCES += \
    src/filewatcher.cpp \
    src/harbour-batterybuddy-daemon.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
