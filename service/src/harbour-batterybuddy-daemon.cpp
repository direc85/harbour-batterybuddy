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

#include "logger.h"
#include "battery.h"

#include <signal.h>

int main(int argc, char** argv)
{
    bool logLevelSet = false;
    bool verbose = false;
    bool debug = false;
    bool logfile = false;

    for(int i = 0; i < argc; i++) {
        if(!strcmp(argv[i],"-v")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            return 0;
        }
        else if(!strcmp(argv[i],"--verbose")) {
            verbose = true;
            debug = false;
            logLevelSet = true;
        }
        else if(!strcmp(argv[i],"--debug")) {
            verbose = true;
            debug = true;
            logLevelSet = true;
        }
        else if(!strcmp(argv[i],"--logfile")) {
            logfile = true;
            verbose = true;
            debug = false;
            logLevelSet = true;
        }
        else if(!strcmp(argv[i],"--help")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            printf("Usage:\n");
            printf("  --verbose     Enable informational messages\n");
            printf("  --debug       Enable informational and debugging messages\n");
            printf("  --help        Print version string and exit\n");
            printf("  --logfile     Write log to a file. Implies --verbose\n\n");
            printf("Log file: ~/.cache/harbour-batterybuddy-daemon/harbour-batterybuddy-daemon.log\n");
            return 0;
        }
    }

    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);

    Logger* logger = new Logger(verbose, debug, logfile);
    logE(QString("%1 %2").arg(APP_NAME, APP_VERSION));

    Battery* battery = new Battery(logger, logLevelSet, &app);

    // Exit gracefully on Ctrl-C and service stop
    QObject::connect(&app, SIGNAL(aboutToQuit()), battery, SLOT(shutdown()));
    signal(SIGINT, app.exit);
    signal(SIGTERM, app.exit);

    int retval = app.exec();

    delete battery;
    delete logger;

    return retval;
}
