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
    qDebug() << "Loading settings from" << mySettings.fileName();
    int tempValue;
    if(mySettings.contains("lowerLimit")) {
        tempValue = mySettings.value("lowerLimit").toInt();
        if(tempValue >= 10 && tempValue <= 50) {
            lowerLimit = tempValue;
            qDebug() << "Lower limit:" << lowerLimit;
            emit lowerLimitChanged();
        }
    }
    if(mySettings.contains("upperLimit")) {
        tempValue = mySettings.value("upperLimit").toInt();
        if(tempValue >= 60 && tempValue <= 100) {
            upperLimit = tempValue;
            qDebug() << "Upper limit:" << upperLimit;
            emit upperLimitChanged();
        }
    }
}

Settings::~Settings()
{
    mySettings.setValue("lowerLimit", QByteArray::number(lowerLimit));
    mySettings.setValue("upperLimit", QByteArray::number(upperLimit));
}

int Settings::getLowerLimit() { return lowerLimit; }

int Settings::getUpperLimit() { return upperLimit; }

QString Settings::getLowAlert() { return lowAlertFile; }

QString Settings::getHighAlert() { return highAlertFile; }

void Settings::setLowerLimit(int newLimit) { lowerLimit = newLimit; }

void Settings::setUpperLimit(int newLimit) { upperLimit = newLimit; }
