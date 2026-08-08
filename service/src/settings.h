// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021 Peter G. <sailfish@nephros.org>

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
