#include "logger.h"

Logger::Logger(const bool enableVerbose, const bool enableDebug,
               const bool useLogFile) {
    buffer.resize(512);
    verbose = enableVerbose;
    debug = enableDebug;
    if(useLogFile) {
        enableLogFile();
    }
}

Logger::~Logger() {
    if(logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const QString message) {
    buffer = QString("[%1] %2\n").arg(QTime::currentTime().toString("HH:mm:ss"), message);
    if(logFileEnabled && logFile.is_open()) {
        logFile << buffer.toStdString();
        std::flush(logFile);
    }
    else {
        std::cout << buffer.toStdString();
        std::flush(std::cout);
    }
};

void Logger::log(const char* message) {
    log(QString(message));
};

bool Logger::getDebug() { return debug; }

bool Logger::getVerbose() { return verbose; }

void Logger::enableLogFile() {
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    filename = QString(logDir + "/" + APP_NAME + ".log");

    std::cout << "Logging to " << filename.toStdString() << std::endl;

    QDir qDir;
    if(!qDir.exists(logDir)) {
        qDir.mkpath(logDir);
    }

    logFile.open(filename.toLocal8Bit().constData());

    logFileEnabled = logFile.is_open();
    if(!logFileEnabled) {
        std::cout << "Could not open log file. Logging disabled." << std::endl;
    }
}

QString Logger::getLogFilename() {
    return filename;
}

QString Logger::readLogfile(QString logFilename) {
    QFile logFile(logFilename);
    QString contents;
    if(logFile.open(QIODevice::ReadOnly)) {
        contents = QString(logFile.readAll());
        logFile.close();
    }
    else {
        contents = "Couldn't open log file:\n" + logFilename;
    }

    return contents;
}
