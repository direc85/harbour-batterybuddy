#ifndef PROCESS_H
#define PROCESS_H

// Source: http://www.xargs.com/qml/process.html
// Copyright © 2015 John Temples

#include <QProcess>
#include <QVariant>
//#include <QDebug>

class Process : public QProcess {
    Q_OBJECT

public:
    Process(QObject *parent = nullptr) : QProcess(parent) {

    }

    Q_INVOKABLE void start(const QString &program, const QVariantList &arguments) {
        QStringList args;

        // convert QVariantList from QML to QStringList for QProcess

        for (int i = 0; i < arguments.length(); i++)
            args << arguments[i].toString();

        //qDebug() << program + " " + args.join(" ");
        QProcess::start(program, args);
    }

    Q_INVOKABLE QByteArray readAll() {
        return QProcess::readAll();
    }

    Q_INVOKABLE int errorCode() {
        return QProcess::exitCode();
    }
};

#endif // PROCESS_H
