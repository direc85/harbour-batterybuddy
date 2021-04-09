/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2020 Matti Viljanen
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
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include "battery.h"
#include <iostream>
#include <signal.h>

int main(int argc, char** argv)
{
    const char* logEnvVar = "QT_LOGGING_RULES";
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i],"-v")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            return EXIT_SUCCESS;
        }
        else if(!strcmp(argv[i],"--verbose"))
            qputenv(logEnvVar, "*.info=true;*.debug=false");
        else if(!strcmp(argv[i],"--debug"))
            qputenv(logEnvVar, "*.info=true");
        else if(!strcmp(argv[i],"--help")) {
            printf("%s %s\n\n", APP_NAME, APP_VERSION);
            printf("This binary is meant to run as a service as normal user,\n");
            printf("but it can be run manually for debugging purposes, too.\n\n");
            printf("Usage:\n");
            printf("  --verbose     Enable informational messages\n");
            printf("  --debug       Enable informational and debugging messages\n");
            printf("  --help        Print version string and exit\n");
            return EXIT_SUCCESS;
        }
    }
    if(!qEnvironmentVariableIsSet(logEnvVar)) {
        if(!QHostInfo::localHostName().contains("SailfishEmul"))
            qputenv(logEnvVar, "*.info=false;*.debug=false");
        else
            qputenv(logEnvVar, "*.info=true");
    }

    QCoreApplication app(argc, argv);

    Battery battery;

    // Exit gracefully on Ctrl-C and service stop
    QObject::connect(&app, SIGNAL(aboutToQuit()), &battery, SLOT(shutdown()));
    signal(SIGINT, app.exit);
    signal(SIGTERM, app.exit);

    int retval = app.exec();

    return retval;
}
