TARGET = harbour-batterybuddy-daemon

CONFIG += sailfishapp console

QT = core dbus

PKGCONFIG += nemonotifications-qt5

# Keep this in sync with "application.pro"
VER = 3.9
REL = 2

VERSION = $${VER}-$${REL}
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += \
    src/battery.h \
    src/mynotification.h \
    src/logger.h \
    src/settings.h

SOURCES += \
    src/battery.cpp \
    src/mynotification.cpp \
    src/logger.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy-daemon.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
