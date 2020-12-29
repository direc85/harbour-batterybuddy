#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QtDBus/QtDBus>

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(QObject *parent = nullptr);
    uint getRingtoneVolume();

private:
    QDBusConnection connection = QDBusConnection::sessionBus();
};

#endif // PROFILE_H
