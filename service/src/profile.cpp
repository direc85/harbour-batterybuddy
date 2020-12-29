#include "profile.h"

Profile::Profile(QObject *parent) : QObject(parent)
{

}

uint Profile::getRingtoneVolume() {
    QDBusInterface interface("com.nokia.profiled",
                              "/com/nokia/profiled",
                              "com.nokia.profiled",
                             connection);

    QDBusMessage message = interface.call("get_profile");
    QString profile = message.arguments().at(0).toString();
    qDebug() << "Active profile:" << profile;

    if(profile == "silent") {
        qDebug() << "Returning volume: 0";
        return 0;
    }

    message = interface.call("get_value", "profile", "ringing.alert.volume");
    int volume = message.arguments().at(0).toInt();
    qDebug() << "Ringtone volume:" << volume;

    return (volume >= 0 && volume <= 100 ? volume : 0);
}
