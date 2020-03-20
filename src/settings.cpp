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
    tempValue = mySettings.value(sLowAlert, lowAlert).toInt();
    if(tempValue >= 10 && tempValue <= 99) {
        lowAlert = tempValue;
        emit lowAlertChanged();
    }

    tempValue = mySettings.value(sHighAlert, highAlert).toInt();
    if(tempValue >= 10 && tempValue <= 99) {
        highAlert = tempValue;
        emit highAlertChanged();
    }

    tempValue = mySettings.value(sInterval, interval).toInt();
    if(tempValue >= 60 && tempValue <= 600) {
        interval = tempValue;
        emit intervalChanged();
    }

    limitEnabled = (mySettings.value(sLimitEnabled, sLimitEnabled).toInt() == 1);
    emit limitEnabledChanged();
    tempValue = mySettings.value(sLowLimit).toInt();
    if(tempValue >= 20 && tempValue <= 95) {
        lowLimit = tempValue;
        emit lowLimitChanged();
    }

    tempValue = mySettings.value(sHighLimit, highLimit).toInt();
    if(tempValue >= 20 && tempValue <= 95) {
        highLimit = tempValue;
        emit highLimitChanged();
    }
}

Settings::~Settings()
{
    mySettings.setValue(sLowAlert, QByteArray::number(lowAlert));
    mySettings.setValue(sHighAlert, QByteArray::number(highAlert));
    mySettings.setValue(sInterval, QByteArray::number(interval));
    mySettings.setValue(sLimitEnabled, QByteArray::number(limitEnabled ? 1 : 0));
    mySettings.setValue(sLowLimit, QByteArray::number(lowLimit));
    mySettings.setValue(sHighLimit, QByteArray::number(highLimit));
}
