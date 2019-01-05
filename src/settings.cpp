#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    // Defaults
    lowerLimit = 25;
    upperLimit = 75;

    // Just to be sure...
    QDir datadir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if(!datadir.exists())
        datadir.mkdir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    // Use Sailfish-provided sounds
    lowAlertFile = "/usr/share/sounds/jolla-ambient/general_warning.wav";
    highAlertFile = "/usr/share/sounds/jolla-ambient/positive_confirmation.wav";

    load();
}

Settings::~Settings()
{
    save();
}

void Settings::load()
{
    QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QFile file;

    file.setFileName("lowerLimit");
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        int nextLimit = file.readAll().toInt();
        if(nextLimit >= 10 && nextLimit <= 50) {
            lowerLimit = nextLimit;
            qDebug() << "Lower limit loaded:" << lowerLimit;
            emit lowerLimitChanged();
        }
        file.close();
    }
    file.setFileName("upperLimit");
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        int nextLimit = file.readAll().toInt();
        if(nextLimit >= 60 && nextLimit <= 99) {
            upperLimit = nextLimit;
            qDebug() << "Upper limit loaded:" << upperLimit;
            emit upperLimitChanged();
        }
        file.close();
    }
}

void Settings::save()
{
    QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QFile file;

    file.setFileName("lowerLimit");
    if(file.open(QIODevice::WriteOnly)) {
        if(file.write(QString("%1").arg(lowerLimit).toUtf8())) {
            qDebug() << "Lower limit saved:" << lowerLimit;
        }
        file.close();
    }

    file.setFileName("upperLimit");
    if(file.open(QIODevice::WriteOnly)) {
        if(file.write(QString("%1").arg(upperLimit).toUtf8())) {
            qDebug() << "Upper limit saved:" << upperLimit;
        }
        file.close();
    }
}

int Settings::getLowerLimit() { return lowerLimit; }

int Settings::getUpperLimit() { return upperLimit; }

QString Settings::getLowAlert() { return lowAlertFile; }

QString Settings::getHighAlert() { return highAlertFile; }

void Settings::setLowerLimit(int newLimit) { lowerLimit = newLimit; }

void Settings::setUpperLimit(int newLimit) { upperLimit = newLimit; }
