/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    int tempValue;
    if(mySettings.contains("lowerLimit")) {
        tempValue = mySettings.value("lowerLimit").toInt();
        if(tempValue >= 10 && tempValue <= 50) {
            lowerLimit = tempValue;
            emit lowerLimitChanged();
        }
    }
    if(mySettings.contains("upperLimit")) {
        tempValue = mySettings.value("upperLimit").toInt();
        if(tempValue >= 60 && tempValue <= 100) {
            upperLimit = tempValue;
            emit upperLimitChanged();
        }
    }
    if(mySettings.contains("interval")) {
        tempValue = mySettings.value("interval").toInt();
        if(tempValue >= 60 && tempValue <= 600) {
            interval = tempValue;
            emit intervalChanged();
        }
    }
    if(mySettings.contains("limitEnabled")) {
        limitEnabled = (mySettings.value("limitEnabled").toInt() == 1);
        emit limitEnabledChanged();
    }
    if(mySettings.contains("chargeLimit")) {
        tempValue = mySettings.value("chargeLimit").toInt();
        if(tempValue >= 50 && tempValue <= (upperLimit - 2)) {
            chargeLimit = tempValue;
            emit chargeLimitChanged();
        }
    }
}

Settings::~Settings()
{
    int limitValue = limitEnabled ? 1 : 0;
    mySettings.setValue("lowerLimit", QByteArray::number(lowerLimit));
    mySettings.setValue("upperLimit", QByteArray::number(upperLimit));
    mySettings.setValue("interval", QByteArray::number(interval));
    mySettings.setValue("limitEnabled", QByteArray::number(limitValue));
    mySettings.setValue("chargeLimit", QByteArray::number(chargeLimit));
}

int Settings::getLowerLimit() { return lowerLimit; }

int Settings::getUpperLimit() { return upperLimit; }

int Settings::getInterval() { return interval; }

QString Settings::getLowAlert() { return lowAlertFile; }

QString Settings::getHighAlert() { return highAlertFile; }

bool Settings::getLimitEnabled() { return limitEnabled; }

int Settings::getChargeLimit() { return chargeLimit; }

void Settings::setLowerLimit(int newLimit) { lowerLimit = newLimit; }

void Settings::setUpperLimit(int newLimit) { upperLimit = newLimit; }

void Settings::setInterval(int newInterval) { interval = newInterval; }

void Settings::setLimitEnabled(bool newEnabled) { limitEnabled = newEnabled; }

void Settings::setChargeLimit(int newLimit) { chargeLimit = newLimit; }
