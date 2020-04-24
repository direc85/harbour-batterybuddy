#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFile>

#include <QDebug>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    FileWatcher(QObject* parent = nullptr);
    ~FileWatcher();

private:
    QFileSystemWatcher *chargeWatcher, *stateWatcher, *chargerConnectedWatcher;
    QFile *chargeFile, *stateFile, *chargerConnectedFile, *chargerControlFile;
    int enableChargingValue = 1, disableChargingValue = 0;

private slots:
    void updateCharge();
    void updateState();
    void updateChargerConnected();
};

#endif // FILEWATCHER_H
