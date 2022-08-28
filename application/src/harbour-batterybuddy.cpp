/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2022 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Battery Buddy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>
#include <QStringList>

#include "logger.h"
#include "battery.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-batterybuddy.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    bool verbose = false;
    bool debug = false;

    if(QSysInfo::machineHostName().contains("SailfishEmul")) {
        verbose = true;
        debug = true;
    }

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i],"-v")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            return 0;
        }
        else if(!strcmp(argv[i],"--verbose")) {
            verbose = true;
            debug = false;
        }
        else if(!strcmp(argv[i],"--debug")) {
            verbose = true;
            debug = true;
        }
        else if(!strcmp(argv[i],"--help")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            printf("Usage:\n");
            printf("  --verbose     Set log level to medium)\n");
            printf("  --debug       Set log level to high\n");
            printf("  --help        Print version string and exit\n");
            return 0;
        }
    }

    QGuiApplication* app = SailfishApp::application(argc, argv);
    app->setApplicationName(APP_NAME);
    app->setOrganizationName(APP_NAME);

    QQuickView* view = SailfishApp::createView();

    Logger *logger = new Logger(verbose, debug, false);
    Settings* settings = new Settings(logger);
    Battery* battery = new Battery(settings, logger);

    QTimer* updater = new QTimer();
    QObject::connect(updater, SIGNAL(timeout()), battery, SLOT(updateData()));
    updater->start(3000);

    const QStringList frequencyNames = {
        QObject::tr("Never"),
        QObject::tr("30 seconds"),
        QObject::tr("2.5 minutes"),
        QObject::tr("5 minutes"),
        QObject::tr("10 minutes"),
        QObject::tr("15 minutes"),
        QObject::tr("30 minutes"),
        QObject::tr("1 hour"),
        QObject::tr("2 hours"),
        QObject::tr("4 hours"),
        QObject::tr("8 hours"),
        QObject::tr("10 hours"),
        QObject::tr("12 hours"),
        QObject::tr("24 hours"),
    };

    view->rootContext()->setContextProperty("battery", battery);
    view->rootContext()->setContextProperty("settings", settings);
    view->rootContext()->setContextProperty("logger", logger);
    view->rootContext()->setContextProperty("app_version", APP_VERSION);
    view->rootContext()->setContextProperty("frequencyNames", frequencyNames);

    view->setSource(SailfishApp::pathTo("qml/harbour-batterybuddy.qml"));
    view->showFullScreen();

    int retval = app->exec();

    updater->stop();

    delete updater;
    delete battery;
    delete settings;

    delete logger;

    return retval;
}
