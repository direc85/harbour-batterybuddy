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

CONFIG += console sailfishapp sailfishapp_i18n dbus

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

INCLUDEPATH += "../common"

HEADERS += \
    ../common/batterybase.h \
    ../common/logger.h \
    ../common/settingsbase.h \
    src/battery.h \
    src/settings.h

SOURCES += \
    ../common/batterybase.cpp \
    ../common/logger.cpp \
    ../common/settingsbase.cpp \
    src/battery.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy.cpp \

DISTFILES += \
    harbour-batterybuddy.desktop \
    qml/components/AboutLabel.qml \
    qml/components/AdjustmentButtons.qml \
    qml/components/CoverLabel.qml \
    qml/components/BatteryGraph.qml \
    qml/components/ImageButton.qml \
    qml/components/MyDetailItem.qml \
    qml/components/MyLabel.qml \
    qml/components/MySlider.qml \
    qml/components/SystemdUserService.qml \
    qml/cover/CoverPage.qml \
    qml/harbour-batterybuddy.qml \
    qml/pages/AboutPage.qml \
    qml/pages/InfoPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/LicensePage.qml \
    rpm/harbour-batterybuddy.changes \
    rpm/harbour-batterybuddy.spec \
    service/harbour-batterybuddy-oneshot.service \
    service/harbour-batterybuddy.service \
    service/restore-write-permissions.sh \
    service/set-write-permissions.sh \
    $$files(translations/*.ts)

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

TRANSLATIONS += translations/harbour-batterybuddy.ts \
   $$files(translations/harbour-batterybuddy-*.ts)

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
