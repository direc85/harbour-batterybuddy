#include "battery.h"

Battery::Battery(QObject* parent) : QObject(parent)
{
    chargeFile = new QFile("/run/state/namespaces/Battery/ChargePercentage"); // Number, meaning percentage, e.g. 42
    chargingFile = new QFile("/run/state/namespaces/Battery/IsCharging"); // Number, 0 or 1

    // Default values...
    currentCharge = 100;
    isCharging = true;

    nextCharge = currentCharge;
    nextCharging = isCharging;

    this->updateData();
}

Battery::~Battery()
{

}

void Battery::updateData()
{
    if(chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readAll().toInt();
        if(nextCharge != currentCharge) {
            currentCharge = nextCharge;
            emit chargeChanged();
        }
        chargeFile->close();
    }
    if(chargingFile->open(QIODevice::ReadOnly)) {
        nextCharging = (chargingFile->readAll().toInt() == 0 ? false : true);
        if(nextCharging != isCharging) {
            isCharging = nextCharging;
            emit chargingChanged();
        }
        chargingFile->close();
    }
}

int Battery::getCharge()
{
    return currentCharge;
}

bool Battery::getCharging()
{
    return isCharging;
}
