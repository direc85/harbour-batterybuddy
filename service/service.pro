# 0 = Compatible with SFOS 4.0+
# 1 = Compatible SFOS 2.2+
LEGACY_BUILD = 0

TARGET = harbour-batterybuddy-daemon

CONFIG += console

contains(LEGACY_BUILD, 0) { QT = core dbus }
contains(LEGACY_BUILD, 1) { QT = core dbus multimedia }

PKGCONFIG += nemonotifications-qt5

# Keep this in sync with application.pro and .spec
VERSION = 3.16.2-1

DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"
DEFINES += LEGACY_BUILD=$${LEGACY_BUILD}

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

LIBS += -lkeepalive -lnemonotifications-qt5

HEADERS += \
    src/battery.h \
    src/logger.h \
    src/settings.h

SOURCES += \
    src/battery.cpp \
    src/logger.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy-daemon.cpp

contains(LEGACY_BUILD, 1) {
    HEADERS += src/profile.h \
               src/mynotification_sfos2.h
    SOURCES += src/profile.cpp \
               src/mynotification_sfos2.cpp
}

contains(LEGACY_BUILD, 0) {
    HEADERS += src/mynotification.h
    SOURCES += src/mynotification.cpp
}

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
