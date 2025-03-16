#include "settingsbase.h"

SettingsBase::SettingsBase(Logger* newLogger, QObject* parent) : QObject(parent)
{
    logger = newLogger;
}

// Getters condensed.
int     SettingsBase::getLowAlert()                    { return lowAlert; }
int     SettingsBase::getHighAlert()                   { return highAlert; }
int     SettingsBase::getHealthAlert()                 { return healthAlert; }
int     SettingsBase::getHighNotificationsInterval()   { return highNotificationsInterval; }
int     SettingsBase::getLowNotificationsInterval()    { return lowNotificationsInterval; }
int     SettingsBase::getHealthNotificationsInterval() { return healthNotificationsInterval; }
int     SettingsBase::getLowLimit()                    { return lowLimit; }
int     SettingsBase::getHighLimit()                   { return highLimit; }
int     SettingsBase::getMaxChargeCurrent()            { return maxChargeCurrent; }
int     SettingsBase::getMaxSupportedChargeCurrent()   { return maxSupportedChargeCurrent; }
bool    SettingsBase::getLimitEnabled()                { return limitEnabled == 1; }
QString SettingsBase::getLowAlertFile()                { return lowAlertFile; }
QString SettingsBase::getHighAlertFile()               { return highAlertFile; }
QString SettingsBase::getHealthAlertFile()             { return healthAlertFile; }
QString SettingsBase::getLogFilename()                 { return logFilename; }
QString SettingsBase::getNotificationTitle()           { return notificationTitle; }
QString SettingsBase::getNotificationLowText()         { return notificationLowText; }
QString SettingsBase::getNotificationHighText()        { return notificationHighText; }
QString SettingsBase::getNotificationHealthTitle()     { return notificationHealthTitle; }
QString SettingsBase::getNotificationHealthWarnText()  { return notificationHealthWarnText; }
QString SettingsBase::getNotificationHealthCritText()  { return notificationHealthCritText; }
int     SettingsBase::getLogLevel()                    { return logLevel; }

void SettingsBase::setLowAlert(const int newLimit) {
    if(saveInteger(sLowAlert, newLimit, lowAlert)) {
        emit lowAlertChanged(lowAlert);
    }
}

void SettingsBase::setHighAlert(const int newLimit) {
    if(saveInteger(sHighAlert, newLimit, highAlert)) {
        emit highAlertChanged(highAlert);
    }
}

void SettingsBase::setHealthAlert(const int newLimit) {
    if(saveInteger(sHealthAlert, newLimit, healthAlert)) {
        emit healthAlertChanged(healthAlert);
    }
}

void SettingsBase::setHighNotificationsInterval(const int newInterval) {
    if(saveInteger(sHighNotificationsInterval, newInterval, highNotificationsInterval)) {
        emit highNotificationsIntervalChanged(highNotificationsInterval);
    }
}

void SettingsBase::setLowNotificationsInterval(const int newInterval) {
    if(saveInteger(sLowNotificationsInterval, newInterval, lowNotificationsInterval)) {
        emit lowNotificationsIntervalChanged(lowNotificationsInterval);
    }
}

void SettingsBase::setHealthNotificationsInterval(const int newInterval) {
    if(saveInteger(sHealthNotificationsInterval, newInterval, healthNotificationsInterval)) {
        emit healthNotificationsIntervalChanged(healthNotificationsInterval);
    }
}

void SettingsBase::setLowLimit(const int newLimit) {
    if(saveInteger(sLowLimit, newLimit, lowLimit)) {
        emit lowLimitChanged(lowLimit);
    }
}

void SettingsBase::setHighLimit(const int newLimit) {
    if(saveInteger(sHighLimit, newLimit, highLimit))
        emit highLimitChanged(highLimit);
}

void SettingsBase::setMaxChargeCurrent(const int newCurrent) {
    if(saveInteger(sMaxChargeCurrent, newCurrent, maxChargeCurrent))
        emit maxChargeCurrentChanged(maxChargeCurrent);
}

void SettingsBase::setLimitEnabled(const bool newEnabled) {
    if(saveInteger(sLimitEnabled, (newEnabled ? 1 : 0), limitEnabled))
        emit limitEnabledChanged(limitEnabled);
}

void SettingsBase::setNotificationTitle(const QString newText) {
    if(saveString(sNotificationTitle, newText, notificationTitle))
        emit notificationTitleChanged(notificationTitle);
}

void SettingsBase::setNotificationLowText(const QString newText) {
    if(saveString(sNotificationLowText, newText, notificationLowText))
        emit notificationLowTextChanged(notificationLowText);
}

void SettingsBase::setNotificationHighText(const QString newText) {
    if(saveString(sNotificationHighText, newText, notificationHighText))
        emit notificationHighTextChanged(notificationHighText);
}

void SettingsBase::setNotificationHealthTitle(const QString newText) {
    if(saveString(sNotificationHealthTitle, newText, notificationTitle))
        emit notificationHealthTitleChanged(notificationTitle);
}

void SettingsBase::setNotificationHealthWarnText(const QString newText) {
    if(saveString(sNotificationHealthWarnText, newText, notificationHealthWarnText))
        emit notificationHealthWarnTextChanged(notificationHealthWarnText);
}

void SettingsBase::setNotificationHealthCritText(const QString newText) {
    if(saveString(sNotificationHealthCritText, newText, notificationHealthCritText))
        emit notificationHealthCritTextChanged(notificationHealthCritText);
}

void SettingsBase::setLogLevel(const int newLogLevel) {
    if(saveInteger(sLogLevel, newLogLevel, logLevel))
        emit logLevelChanged(logLevel);
}

bool SettingsBase::loadInteger(const QString key, int &currValue, const int min, const int max) {
    int newValue = settings->value(key, currValue).toInt();
    newValue = (newValue <= min ? min : (newValue >= max ? max : newValue));
    if(currValue == newValue) {
        logH(QString("Load: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    logM(QString("Load: %1 %2").arg(key).arg(currValue));
    return true;
}

bool SettingsBase::loadString(const QString key, QString & currValue) {
    QString newValue = settings->value(key, currValue).toString();
    if(currValue == newValue) {
        logH(QString("Load: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    logM(QString("Load: %1 %2").arg(key).arg(currValue));
    return true;
}

bool SettingsBase::saveInteger(const QString key, const int &newValue, int &currValue) {
    if(currValue == newValue) {
        logH(QString("Save: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    settings->setValue(key, QByteArray::number(newValue));
    logM(QString("Save: %1 %2").arg(key).arg(newValue));
    return true;
}

bool SettingsBase::saveString(const QString key, const QString &newValue, QString &currValue) {
    if(currValue == newValue) {
        return false;
    }
    currValue = newValue;
    settings->setValue(key, QString(newValue).replace("\"", "\\\"").toUtf8());
    logM(QString("Save: %1 %2").arg(key).arg(newValue));
    return true;
}
