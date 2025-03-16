#!/bin/bash
PARAMS="application/src application/qml -ts application/translations/harbour-batterybuddy.ts application/translations/harbour-batterybuddy.ts application/translations/harbour-batterybuddy-*.ts"
if [ -f "/usr/bin/lupdate-qt5" ]; then
  /usr/bin/lupdate-qt5 $PARAMS
else
  /usr/bin/lupdate $PARAMS
fi
