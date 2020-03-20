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
    // Migrate old settings
    if(mySettings.contains("lowerLimit")) {
        mySettings.setValue(sLowAlert, mySettings.value("lowerLimit"));
        mySettings.remove("lowerLimit");
        qInfo() << "Migrated old lowerLimit value";
    }

    if(mySettings.contains("upperLimit")) {
        mySettings.setValue(sHighAlert, mySettings.value("upperLimit"));
        mySettings.remove("upperLimit");
        qInfo() << "Migrated old upperLimit value";
    }

    // Read in the values
    loadInteger(sLowAlert, &lowAlert, 10, 99);
    loadInteger(sHighAlert, &highAlert, 11, 100);
    loadInteger(sInterval, &interval, 60, 600);
    loadInteger(sLimitEnabled, &limitEnabled, 0, 1);
    loadInteger(sLowLimit, &lowLimit, 20, 94);
    loadInteger(sHighLimit, &highLimit, 21, 95);
    qInfo() << "Loaded" << sLimitEnabled << limitEnabled;
}

Settings::~Settings()
{
    mySettings.setValue(sLowAlert, QByteArray::number(lowAlert));
    mySettings.setValue(sHighAlert, QByteArray::number(highAlert));
    mySettings.setValue(sInterval, QByteArray::number(interval));
    mySettings.setValue(sLimitEnabled, QByteArray::number(limitEnabled));
    mySettings.setValue(sLowLimit, QByteArray::number(lowLimit));
    mySettings.setValue(sHighLimit, QByteArray::number(highLimit));
    qInfo() << "Settings saved";
}

int Settings::bound(int value, int min, int max) {
    return (value <= min ? min : (value >= max ? max : value));
}

void Settings::loadInteger(const char* key, int *value, int min, int max) {
    *value = bound(mySettings.value(key, *value).toInt(), min, max);
    qInfo() << "Loaded" << key << *value;
}
