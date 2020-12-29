TARGET = harbour-batterybuddy-daemon

CONFIG = sailfishapp qt console c++11 sailfish_build

QT = core network dbus multimedia dbus

PKGCONFIG += nemonotifications-qt5

# Keep this in sync with "application.pro"
VER = 3.7
REL = 1

VERSION = $${VER}-$${REL}
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

HEADERS += \
    src/battery.h \
    src/mynotification.h \
    src/profile.h \
    src/settings.h

SOURCES += \
    src/battery.cpp \
    src/mynotification.cpp \
    src/profile.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy-daemon.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service
