#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTime>
#include <iostream>
#include <fstream>

// Use like this:
// logV("Settings loaded");

// Errors (always written - for general messages, too!)
#define logE logger->log

// Verbose (log, if enabled)
#define logV if(logger->verbose) logger->log

// Debug (log, if enabled)
#define logD if(logger->debug) logger->log

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
