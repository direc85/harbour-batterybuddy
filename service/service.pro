TARGET = harbour-batterybuddy-daemon

QT = core dbus

PKGCONFIG += nemonotifications-qt5

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

LIBS += -lkeepalive -lnemonotifications-qt5

INCLUDEPATH += "/usr/include/nemonotifications-qt5"
INCLUDEPATH += "../common"

HEADERS += \
    ../common/batterybase.h \
    ../common/logger.h \
    ../common/settingsbase.h \
    src/battery.h \
    src/settings.h \
    src/mynotification.h

SOURCES += \
    ../common/batterybase.cpp \
    ../common/logger.cpp \
    ../common/settingsbase.cpp \
    src/battery.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy-daemon.cpp \
    src/mynotification.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
