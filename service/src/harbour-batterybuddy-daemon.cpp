// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2020-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

#include <QCoreApplication>

#include <signal.h>

#include "logger.h"
#include "settings.h"
#include "battery.h"
#include "version.h"

int main(int argc, char** argv)
{
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
        }
        else if(!strcmp(argv[i],"--debug")) {
            verbose = true;
            debug = true;
        }
        else if(!strcmp(argv[i],"--logfile")) {
            logfile = true;
            verbose = true;
            debug = false;
        }
        else if(!strcmp(argv[i],"--help")) {
            printf("%s %s\n", APP_NAME, APP_VERSION);
            printf("Usage:\n");
            printf("  --verbose     Set log level to medium)\n");
            printf("  --debug       Set log level to high\n");
            printf("  --help        Print version string and exit\n");
            printf("  --logfile     Write log to a file. Implies --verbose\n\n");
            printf("Log file: ~/.cache/harbour-batterybuddy/harbour-batterybuddy-daemon.log\n");
            return 0;
        }
    }

    QCoreApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setOrganizationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);

    Logger* logger = new Logger(verbose, debug, logfile);
    Settings* settings = new Settings(logger);
    logL(QString("%1 %2").arg(APP_NAME, APP_VERSION));

    Battery* battery = new Battery(settings, logger, &app);

    // Exit gracefully on Ctrl-C and service stop
    QObject::connect(&app, &QCoreApplication::aboutToQuit, battery, &Battery::shutdown);
    signal(SIGINT, app.exit);
    signal(SIGTERM, app.exit);

    int retval = app.exec();

    delete battery;
    delete logger;
    delete settings;

    return retval;
}
