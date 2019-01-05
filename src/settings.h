#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int     lowerLimit     READ getLowerLimit  WRITE setLowerLimit  NOTIFY lowerLimitChanged)
    Q_PROPERTY(int     upperLimit     READ getUpperLimit  WRITE setUpperLimit  NOTIFY upperLimitChanged)
    Q_PROPERTY(QString lowAlertFile   READ getLowAlert                         NOTIFY lowAlertChanged)
    Q_PROPERTY(QString highAlertFile  READ getHighAlert                        NOTIFY highAlertChanged)

public:
    Settings(QObject* parent = nullptr);
    ~Settings();

    // QML-exposed stuff
    int getLowerLimit();
    int getUpperLimit();
    QString getLowAlert();
    QString getHighAlert();
    void setLowerLimit(int newLimit);
    void setUpperLimit(int newLimit);

private:
    int lowerLimit;
    int upperLimit;
    QString lowAlertFile;
    QString highAlertFile;

    void load();
    void save();

signals:
    int lowerLimitChanged();
    int upperLimitChanged();
    QString lowAlertChanged();
    QString highAlertChanged();
};

#endif // SETTINGS_H
