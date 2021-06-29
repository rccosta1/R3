#include "RStorageFacilityModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RMainWindow.h"
#include <math.h>



RStorageFacilityModel::RStorageFacilityModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    durationOfShortage = 0.0;
    theFillLevel = 0;
    maxStorageCapacity = 0.0;
    theNumberOfTanks = 1;
    theReserve = 0.0;
    lastEvaluationTime = 0.0;
}

RStorageFacilityModel::~RStorageFacilityModel()
{
}


double RStorageFacilityModel::getMaxCapacity() const
{
    return maxStorageCapacity;
}


void RStorageFacilityModel::setMaxCapacity(double value)
{
    maxStorageCapacity = value;
    theReserve = maxStorageCapacity * theFillLevel->getCurrentValue();
}


void RStorageFacilityModel::changeCurrentCapacity(double value)
{
    theCurrentCapacity = value;
}


int RStorageFacilityModel::getNumberOfStorageTanks() const
{
    return theNumberOfTanks;
}

void RStorageFacilityModel::setNumberOfStorageTanks(int value)
{
    theNumberOfTanks = value;
}


QObject * RStorageFacilityModel::getFillLevel() const
{
    return theFillLevel;
}

void RStorageFacilityModel::setFillLevel(QObject *value)
{
    theFillLevel = qobject_cast<RParameter *>(value);
}


void RStorageFacilityModel::setReserve(double value)
{
    theReserve = value;
    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
    }
}


void RStorageFacilityModel::setShortageDuration()
{
    shortageDuration << durationOfShortage;
}


void RStorageFacilityModel::printShortageDuration()
{
    qDebug() << this->objectName() << shortageDuration;
}


int RStorageFacilityModel::extractSupply(double amount)
{
    theReserve -= amount;

    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
        return 0;
    }

    else {
        return 1;
    }
}




