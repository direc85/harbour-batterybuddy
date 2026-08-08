#!/bin/bash

# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2025-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

PARAMS="application/src application/qml -ts application/translations/harbour-batterybuddy.ts application/translations/harbour-batterybuddy.ts application/translations/harbour-batterybuddy-*.ts"
if [ -f "/usr/bin/lupdate-qt5" ]; then
  /usr/bin/lupdate-qt5 $PARAMS
else
  /usr/bin/lupdate $PARAMS
fi
