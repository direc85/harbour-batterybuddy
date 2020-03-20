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
    if(mySettings.contains(sLowAlert)) {
        tempValue = mySettings.value(sLowAlert).toInt();
        if(tempValue >= 10 && tempValue <= 99) {
            lowAlert = tempValue;
            emit lowAlertChanged();
        }
    }
    if(mySettings.contains(sHighAlert)) {
        tempValue = mySettings.value(sHighAlert).toInt();
        if(tempValue >= 10 && tempValue <= 99) {
            highAlert = tempValue;
            emit highAlertChanged();
        }
    }
    if(mySettings.contains(sInterval)) {
        tempValue = mySettings.value(sInterval).toInt();
        if(tempValue >= 60 && tempValue <= 600) {
            interval = tempValue;
            emit intervalChanged();
        }
    }
    if(mySettings.contains(sLimitEnabled)) {
        limitEnabled = (mySettings.value(sLimitEnabled).toInt() == 1);
        emit limitEnabledChanged();
    }
    if(mySettings.contains(sLowLimit)) {
        tempValue = mySettings.value(sLowLimit).toInt();
        if(tempValue >= 20 && tempValue <= 95) {
            lowLimit = tempValue;
            emit lowLimitChanged();
        }
    }
    if(mySettings.contains(sHighLimit)) {
        tempValue = mySettings.value(sHighLimit).toInt();
        if(tempValue >= 20 && tempValue <= 95) {
            highLimit = tempValue;
            emit highLimitChanged();
        }
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
