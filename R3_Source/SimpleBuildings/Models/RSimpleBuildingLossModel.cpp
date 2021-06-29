#include "RSimpleBuildingLossModel.h"

#include "RParameter.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include <math.h>

RSimpleBuildingLossModel::RSimpleBuildingLossModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theBuildingValue = 0.0;
    theOccupancyClass = QString("RES1");

    // Initializing the pointer data members
    theDomain->createObject(objectName() + "ImmediateLossResponse", "RGenericResponse");
    theBuildingLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theBuildingLossResponse->setModel(this);
    theBuildingLossResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "ContentLossResponse", "RGenericResponse");
    theContentLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theContentLossResponse->setModel(this);
    theContentLossResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "DisruptionCostResponse", "RGenericResponse");
    theDisruptionCostResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theDisruptionCostResponse->setModel(this);
    theDisruptionCostResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "TotalLossResponse", "RGenericResponse");
    theTotalLossResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTotalLossResponse->setModel(this);
    theTotalLossResponse->setCurrentValue(0.0);

}



RSimpleBuildingLossModel::~RSimpleBuildingLossModel()
{
}


int RSimpleBuildingLossModel::resetTime()
{
    return 0;
}



double RSimpleBuildingLossModel::getBuildingValue() const
{
    return theBuildingValue;
}

void RSimpleBuildingLossModel::setBuildingValue(double value)
{
    theBuildingValue = value;
}


QString RSimpleBuildingLossModel::getBuildingOccupancyClass() const
{
    return theOccupancyClass;
}

void RSimpleBuildingLossModel::setBuildingOccupancyClass(QString value)
{
    theOccupancyClass = value;
}



QObject* RSimpleBuildingLossModel::getStructuralDamage() const
{
    return theStructuralDamageRatioResponse;
}

void RSimpleBuildingLossModel::setStructuralDamage(QObject* value)
{
    theStructuralDamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject* RSimpleBuildingLossModel::getNSDriftDamage() const
{
    return theNDDamageRatioResponse;
}

void RSimpleBuildingLossModel::setNSDriftDamage(QObject* value)
{
    theNDDamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject* RSimpleBuildingLossModel::getNSAccelerationDamage() const
{
    return theNADamageRatioResponse;
}

void RSimpleBuildingLossModel::setNSAccelerationDamage(QObject* value)
{
    theNADamageRatioResponse = qobject_cast<RResponse *>(value);
}


QObject* RSimpleBuildingLossModel::getContentDamageRatios() const
{
    return theContentDamageRatiosResponse;
}

void RSimpleBuildingLossModel::setContentDamageRatios(QObject* value)
{
    theContentDamageRatiosResponse = qobject_cast<RResponse *>(value);
}





int RSimpleBuildingLossModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    theSRepairCost.clear();
    theNSARepairCost.clear();
    theNSDRepairCost.clear();
    theContentReplacementCost.clear();



    if (theOccupancyClass == QString("RES1")) {
        theSRepairCost << 0.0 << 0.5 << 2.3 << 11.7 << 23.4;
        theNSARepairCost << 0.0 << 0.5 << 2.7 << 8.0 << 26.6;
        theNSDRepairCost << 0.0 << 1.0 << 5.5 << 25.0 << 50.0;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;

    } else if (theOccupancyClass == QString("RES2")) {
        theSRepairCost << 0.0 << 0.4 << 2.4 << 7.3 << 24.4;
        theNSARepairCost << 0.0 << 0.8 << 3.8 << 11.3 << 37.8;
        theNSDRepairCost << 0.0 << 0.8 << 3.8 << 18.9 << 37.8;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;

    } else if (theOccupancyClass == QString("RES3")) {
        theSRepairCost << 0.0 << 0.3 << 1.4 << 6.9 << 13.8;
        theNSARepairCost << 0.0 << 0.8 << 4.3 << 13.1 << 43.7;
        theNSDRepairCost << 0.0 << 0.9 << 4.3 << 21.3 << 42.5;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;

    } else if (theOccupancyClass == QString("RES4")) {
        theSRepairCost << 0.0 << 0.2 << 1.4 << 6.8 << 13.6;
        theNSARepairCost << 0.0 << 0.9 << 4.3 << 13 << 43.2;
        theNSDRepairCost << 0.0 << 0.9 << 4.3 << 21.6 << 43.2;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;

    } else if (theOccupancyClass == QString("RES5")) {
        theSRepairCost << 0.0  << 0.4 << 1.9 << 9.4 << 18.8;
        theNSARepairCost << 0.0  << 0.8 << 4.1 << 12.4 << 41.2;
        theNSDRepairCost << 0.0  << 0.8 << 4 << 20 << 40;
        theContentReplacementCost << 0.0  << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;

    } else if (theOccupancyClass == QString("RES6")) {
        theSRepairCost << 0.0 << 0.4 << 1.8 << 9.2 << 18.4;
        theNSARepairCost << 0.0 << 0.8 << 4.1 << 12.2 << 40.8;
        theNSDRepairCost << 0.0 << 0.8 << 4.1 << 20.4 << 40.8;
        theContentReplacementCost << 0.0 << 1.0 << 5.0 << 25.0 << 50.0;
    }



    // Reset responses
    theBuildingLossResponse->setCurrentValue(0.0);
    theContentLossResponse->setCurrentValue(0.0);
    theDisruptionCostResponse->setCurrentValue(0.0);
    theTotalLossResponse->setCurrentValue(0.0);


    // End this quickly if there was no ground acceleration
    if (theStructuralDamageRatioResponse->getCurrentValue() < 0.001){
        return 0;
    }

    double theDamageRatio = 0.0;

    double theStructuralDamageRatio = theStructuralDamageRatioResponse->getCurrentValue() * theSRepairCost.at(4) / 100.0;
    double theDriftDamageRatio = theNDDamageRatioResponse->getCurrentValue() * theNSDRepairCost.at(4) / 100.0;
    double theAccelerationDamageRatio = theNADamageRatioResponse->getCurrentValue() * theNSARepairCost.at(4) / 100.0;
    double theContentDamageRatio = theContentDamageRatiosResponse->getCurrentValue() * theContentReplacementCost.at(4) / 100.0;
    theDamageRatio = theStructuralDamageRatio + theDriftDamageRatio + theAccelerationDamageRatio;

    theBuildingLossResponse->setCurrentValue(theDamageRatio * theBuildingValue);
    theContentLossResponse->setCurrentValue(theContentDamageRatio * theBuildingValue);
    theTotalLossResponse->setCurrentValue((theDamageRatio + theContentDamageRatio) * theBuildingValue);

    qDebug() <<"***************************************";
    qDebug() << "Building loss [$]:" << theBuildingLossResponse->getCurrentValue() << "(" << 100.0 * theBuildingLossResponse->getCurrentValue() / theBuildingValue << "% )";
    qDebug() << "Content loss [$]:" << theContentLossResponse->getCurrentValue() << "(" << 100.0 * theContentLossResponse->getCurrentValue() / theBuildingValue << "% )";
    qDebug() << "Total loss [$]:" << theTotalLossResponse->getCurrentValue() << "(" << 100.0 * theTotalLossResponse->getCurrentValue() / theBuildingValue << "% )";
    qDebug() <<"***************************************";

    return 0;
}





