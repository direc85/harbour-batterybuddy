# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-batterybuddy

CONFIG += console sailfishapp sailfishapp_i18n

# Keep this in sync with "service.pro"
VER = 3.16.2
REL = 1

VERSION = $${VER}-$${REL}
system(bash update-spec-version.sh $$TARGET $$VER $$REL)
DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += \
    src/battery.h \
    src/logger.h \
    src/settings.h \
    src/process.h

SOURCES += src/harbour-batterybuddy.cpp \
    src/battery.cpp \
    src/logger.cpp \
    src/settings.cpp

DISTFILES += qml/harbour-batterybuddy.qml \
    qml/components/AboutLabel.qml \
    qml/components/MyLabel.qml \
    qml/pages\LicensePage.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-batterybuddy.spec \
    service/harbour-batterybuddy.service \
    service/harbour-batterybuddy-oneshot.service \
    service/restore-write-permissions.sh \
    service/set-write-permissions.sh \
    translations/*.ts \
    harbour-batterybuddy.desktop \
    qml/pages/MainPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/InfoPage.qml \
    rpm/harbour-batterybuddy.changes \
    qml/components/BatteryGraph.qml \
    qml/pages/SettingsPage.qml \
    qml/components/MyDetailItem.qml \
    qml/components/AdjustmentButtons.qml \
    qml/components/MySlider.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

TRANSLATIONS += translations/harbour-batterybuddy.ts \
   translations/harbour-batterybuddy-de_DE.ts \
   translations/harbour-batterybuddy-fi.ts \
   translations/harbour-batterybuddy-fr.ts \
   translations/harbour-batterybuddy-hu.ts \
   translations/harbour-batterybuddy-pl.ts \
   translations/harbour-batterybuddy-sv.ts \
   translations/harbour-batterybuddy-zh_CN.ts

# Executable provided by "service.pro"
# Two services, actually
OTHER_FILES += service/harbour-batterybuddy.service \
    service/harbour-batterybuddy-oneshot.service \
    service/restore-write-permissions.sh \
    service/set-write-permissions.sh
service.files = service
service.path = /usr/share/$${TARGET}

# Include images
images.files = images
images.path = /usr/share/$${TARGET}

INSTALLS += service images

# Begin: include sound files

#OTHER_FILES += sounds/upperLimit.ogg \
#    sounds/lowerLimit.ogg \

#sounds.files = sounds
#sounds.path = /usr/share/$${TARGET}

#INSTALLS += sounds

# End: include sound files
