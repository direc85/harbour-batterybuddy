/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2022 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Battery Buddy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QThread>
#include "logger.h"
#include "settingsbase.h"

class Settings : public SettingsBase
{
    Q_OBJECT

public:
    Settings(Logger* newLogger, QObject* parent = nullptr);

    void setMaxSupportedCurrent(int newCurrent);

private:
    QFileSystemWatcher *watcher = nullptr;

    // Converted to boolean for QML
    int daemonEnabled = 1;

private slots:
    void updateConfig(const QString path);

signals:
    void resetTimers();
    void setMaxChargeCurrent(int newCurrent);
};

#endif // SETTINGS_H
