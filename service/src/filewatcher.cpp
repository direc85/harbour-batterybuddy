#include "filewatcher.h"

FileWatcher::FileWatcher(QObject *parent) : QObject(parent)
{
    QString filename;

    // Number: charge percentage, e.g. 42
    chargeFile = new QFile("/sys/class/power_supply/battery/capacity", this);
    qInfo() << "Reading capacity from" << chargeFile->fileName();

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile = new QFile("/sys/class/power_supply/battery/status", this);
    qInfo() << "Reading charge state from" << stateFile->fileName();

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    qInfo() << "Reading charger status from" << chargerConnectedFile->fileName();

    // Find and use the file to control the charger

    // e.g. for Sony Xperia XA2
    filename = "/sys/class/power_supply/battery/input_suspend";
    if(!chargerControlFile && QFile::exists(filename)) {
        chargerControlFile = new QFile(filename, this);
        enableChargingValue = 0;
        disableChargingValue = 1;
    }

    // e.g. for Sony Xperia Z3 Compact Tablet
    filename = "/sys/class/power_supply/battery/charging_enabled";
    if(!chargerControlFile && QFile::exists(filename)) {
        chargerControlFile = new QFile(filename, this);
        enableChargingValue = 1;
        disableChargingValue = 0;
    }

    // e.g. for Jolla Phone
    filename = "/sys/class/power_supply/usb/charger_disable";
    if(!chargerControlFile && QFile::exists(filename)) {
        chargerControlFile = new QFile(filename, this);
        enableChargingValue = 0;
        disableChargingValue = 1;
    }


    if(!chargerControlFile) {
        qWarning() << "Charger control file not found!";
        qWarning() << "Please contact the developer with your device model!";
    }

    // If we found a usable file, check that it is writable
    if(chargerControlFile) {
        if(chargerControlFile->open(QIODevice::WriteOnly)) {
            qInfo() << "Controlling charging via" << chargerControlFile->fileName();
            chargerControlFile->close();
        }
        else {
            delete chargerControlFile;
            chargerControlFile = Q_NULLPTR;
            qWarning() << "Charger control file" << chargerControlFile->fileName() << "is not writable";
            qWarning() << "Charger control feature disabled";
        }
    }

    chargeWatcher = new QFileSystemWatcher(QStringList(chargeFile->fileName()), this);
    stateWatcher = new QFileSystemWatcher(QStringList(stateFile->fileName()), this);
    chargerConnectedWatcher = new QFileSystemWatcher(QStringList(chargerConnectedFile->fileName()), this);

    connect(chargeWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateCharge()));
    connect(stateWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateState()));
    connect(chargerConnectedWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateChargerConnected()));

    return;
}

FileWatcher::~FileWatcher()
{

}

void FileWatcher::updateCharge()
{

}

void FileWatcher::updateState()
{

}

void FileWatcher::updateChargerConnected()
{

}
