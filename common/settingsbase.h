#ifndef SETTINGSBASE_H
#define SETTINGSBASE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include "logger.h"

class SettingsBase : public QObject
{
    Q_OBJECT

public:
    SettingsBase(Logger* newLogger, QObject* parent = nullptr);

    int  getLowAlert();
    int  getHighAlert();
    int  getMaxChargeCurrent();
    int  getHighNotificationsInterval();
    int  getLowNotificationsInterval();
    int  getHealthAlert();
    int  getHealthNotificationsInterval();
    int  getLowLimit();
    int  getHighLimit();
    int  getLogLevel();
    int  getMaxSupportedChargeCurrent();
    bool getLimitEnabled();
    QString getLowAlertFile();
    QString getHighAlertFile();
    QString getHealthAlertFile();
    QString getNotificationTitle();
    QString getNotificationLowText();
    QString getNotificationHighText();
    QString getNotificationHealthTitle();
    QString getNotificationHealthWarnText();
    QString getNotificationHealthCritText();

protected:
    QSettings *settings = nullptr;
    const char* appName = APP_NAME;
    Logger* logger = nullptr;

    void setLowAlert(const int newLimit);
    void setHighAlert(const int newLimit);
    void setHealthAlert(const int newLimit);
    void setHighNotificationsInterval(const int newInterval);
    void setLowNotificationsInterval(const int newInterval);
    void setHealthNotificationsInterval(const int newInterval);
    void setLowLimit(const int newLimit);
    void setHighLimit(const int newLimit);
    void setMaxChargeCurrent(const int newCurrent);
    void setLimitEnabled(const bool newEnabled);
    void setNotificationTitle(const QString newText);
    void setNotificationLowText(const QString newText);
    void setNotificationHighText(const QString newText);
    void setNotificationHealthTitle(const QString newText);
    void setNotificationHealthWarnText(const QString newText);
    void setNotificationHealthCritText(const QString newText);
    void setLogLevel(const int newLogLevel);

    QString getLogFilename();

    // Default values
    int lowAlert = 25;
    int highAlert = 75;
    int healthAlert = 1; // warn
    int highNotificationsInterval = 60;
    int lowNotificationsInterval = 60;
    int healthNotificationsInterval = 60;
    int limitEnabled = 1; // Converted to boolean for QML
    int lowLimit = 65;
    int highLimit = 70;
    int maxChargeCurrent = 0; // micro amps
    int maxSupportedChargeCurrent = 0; // micro amps
    const QString lowAlertFile = QStringLiteral("/usr/share/sounds/jolla-ambient/stereo/general_warning.wav");
    const QString highAlertFile = QStringLiteral("/usr/share/sounds/jolla-ambient/stereo/positive_confirmation.wav");
    const QString healthAlertFile = lowAlertFile;

    // These are updated and localized from the config file
    QString notificationTitle = "Battery charge %1%";
    QString notificationLowText = "Please connect the charger.";
    QString notificationHighText = "Please disconnect the charger.";
    QString notificationHealthTitle = "Battery health %1";
    QString notificationHealthWarnText = "Battery health is not good";
    QString notificationHealthCritText = "Battery health is critical";

    QString logFilename;
    int logLevel = 1;

    // To avoid repeating the same string over and over and over...
    const QString sLowAlert = QStringLiteral("lowAlert");
    const QString sHighAlert = QStringLiteral("highAlert");
    const QString sHealthAlert = QStringLiteral("healthAlert");
    const QString sHighNotificationsInterval = QStringLiteral("highNotificationsInterval");
    const QString sLowNotificationsInterval = QStringLiteral("lowNotificationsInterval");
    const QString sHealthNotificationsInterval = QStringLiteral("healthNotificationsInterval");
    const QString sLimitEnabled = QStringLiteral("limitEnabled");
    const QString sLowLimit = QStringLiteral("lowLimit");
    const QString sHighLimit = QStringLiteral("highLimit");
    const QString sMaxSupportedChargeCurrent = QStringLiteral("maxSupportedChargeCurrent");
    const QString sMaxChargeCurrent = QStringLiteral("maxChargeCurrent");
    const QString sLowAlertFile = QStringLiteral("lowAlertFile");
    const QString sHighAlertFile = QStringLiteral("highAlertFile");
    const QString sHealthAlertFile = QStringLiteral("healthAlertFile");
    const QString sNotificationTitle = QStringLiteral("notificationTitle");
    const QString sNotificationLowText = QStringLiteral("notificationLowText");
    const QString sNotificationHighText = QStringLiteral("notificationHighText");
    const QString sNotificationHealthTitle = QStringLiteral("notificationHealthTitle");
    const QString sNotificationHealthWarnText = QStringLiteral("notificationHealthWarnText");
    const QString sNotificationHealthCritText = QStringLiteral("notificationHealthCritText");
    const QString sLogFilename = QStringLiteral("logFilename");
    const QString sLogLevel = QStringLiteral("logLevel");

    bool loadInteger(const QString key, int &currValue, const int min, const int max);
    bool loadString(const QString key, QString & currValue);
    bool saveInteger(const QString key, const int &newValue, int &currValue);
    bool saveString(const QString key, const QString &newValue, QString &currValue);
    void save();

signals:
    void lowAlertChanged(int);
    void highAlertChanged(int);
    void healthAlertChanged(int);
    void highNotificationsIntervalChanged(int);
    void lowNotificationsIntervalChanged(int);
    void healthNotificationsIntervalChanged(int);
    void limitEnabledChanged(bool);
    void lowLimitChanged(int);
    void highLimitChanged(int);
    void maxChargeCurrentChanged(int);
    void maxSupportedChargeCurrentChanged(int);
    void lowAlertFileChanged(QString);
    void highAlertFileChanged(QString);
    void healthAlertFileChanged(QString);
    void notificationTitleChanged(QString);
    void notificationLowTextChanged(QString);
    void notificationHighTextChanged(QString);
    void notificationHealthTitleChanged(QString);
    void notificationHealthWarnTextChanged(QString);
    void notificationHealthCritTextChanged(QString);
    void logFilenameChanged(QString);
    void logLevelChanged(int);
};

#endif // SETTINGSBASE_H
