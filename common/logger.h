#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTime>
#include <iostream>
#include <fstream>

#include "version.h"

// Use like this:
// logM("Settings loaded");

// Low setting - always logged
#define logL logger->log

// Medium setting - log more values
#define logM if(logger->verbose) logger->log

// High setting - log everything
#define logH if(logger->debug) logger->log

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger(const bool enableVerbose = false,
           const bool enableDebug = false,
           const bool useLogFile = false);
    ~Logger();

    Q_PROPERTY(bool debug READ getDebug)
    Q_PROPERTY(bool verbose READ getVerbose)
    Q_INVOKABLE void log(const QString message);
    Q_INVOKABLE QString readLogfile(QString logFilename);

    bool getDebug();
    bool getVerbose();
    void enableLogFile();
    QString getLogFilename();

    void log(const char* message);
    bool verbose = false;
    bool debug = false;

private:
    std::ofstream logFile;
    bool logFileEnabled = false;
    QString buffer;
    QString filename;
};

#endif
