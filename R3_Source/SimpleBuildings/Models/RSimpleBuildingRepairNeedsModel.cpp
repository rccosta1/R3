#include "RSimpleBuildingRepairNeedsModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RNormalDistribution.h"
#include <math.h>

RSimpleBuildingRepairNeedsModel::RSimpleBuildingRepairNeedsModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    this->setOccupancyClass("RES1");

    theDomain->createObject(objectName() + "RepairTimeResponse", "RGenericResponse");
    theRepairTimeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theRepairTimeResponse->setModel(this);
    theRepairTimeResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "BusinessInterruptionTimeResponse", "RGenericResponse");
    theBusinessInterruptionTimeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theBusinessInterruptionTimeResponse->setModel(this);
    theBusinessInterruptionTimeResponse->setCurrentValue(0.0);
}

RSimpleBuildingRepairNeedsModel::~RSimpleBuildingRepairNeedsModel()
{
}


int RSimpleBuildingRepairNeedsModel::resetTime()
{
    return 0;
}

QString RSimpleBuildingRepairNeedsModel::getOccupancyClass() const
{
    return theOccupancyClass;
}

void RSimpleBuildingRepairNeedsModel::setOccupancyClass(QString value)
{
    serviceInterruptionMultipliers.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    if (value == QString("RES1")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 2.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("RES2")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 2.0 << 10.0 << 30.0 << 60.0;
    }
    else if (value == QString("RES3")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("RES4")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("RES5")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("RES6")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.5  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }


    else if (value == QString("COM1")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.1  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("COM2")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("COM3")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("COM4")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.1  <<  0.2  <<  0.3;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("COM5")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.05  <<  0.03  <<  0.03;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("COM6")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.5  <<  0.5  <<  0.5;
        theRepairTimeMedians << 0.0001 << 10.0 << 45.0 << 180.0 << 360.0;
    }
    else if (value == QString("COM7")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.5  <<  0.5  <<  0.5;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("COM8")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  1  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 90.0 << 180.0;
    }
    else if (value == QString("COM9")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  1  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 5.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("COM10")){
        serviceInterruptionMultipliers  <<  0.1  <<  0.1  <<  1  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 2.0 << 20.0 << 80.0 << 160.0;
    }


    else if (value == QString("IND1")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.5  <<  1  <<  1  <<  1;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("IND2")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("IND3")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.2  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("IND4")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.2  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("IND5")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.2  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("IND6")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.2  <<  0.3  <<  0.4;
        theRepairTimeMedians << 0.0001 << 5.0 << 20.0 << 80.0 << 160.0;
    }


    else if (value == QString("AGR1")){
        serviceInterruptionMultipliers  <<  0  <<  0  <<  0.05  <<  0.1  <<  0.2;
        theRepairTimeMedians << 0.0001 << 2.0 << 10.0 << 30.0 << 60.0;
    }


    else if (value == QString("REL1")){
        serviceInterruptionMultipliers  <<  1  <<  0.2  <<  0.05  <<  0.03  <<  0.03;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }


    else if (value == QString("GOV1")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.02  <<  0.03  <<  0.03;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("GOV2")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.02  <<  0.03  <<  0.03;
        theRepairTimeMedians << 0.0001 << 5.0 << 20.0 << 90.0 << 180.0;
    }


    else if (value == QString("EDU1")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.02  <<  0.05  <<  0.05;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else if (value == QString("EDU2")){
        serviceInterruptionMultipliers  <<  0.5  <<  0.1  <<  0.02  <<  0.03  <<  0.03;
        theRepairTimeMedians << 0.0001 << 10.0 << 30.0 << 120.0 << 240.0;
    }
    else {
        qCritical() << this->objectName() << "has a problem with the Occupancy Class.";
    }

    // theRepairTimeStandardDeviations << 0.3 * theRepairTimeMedians.at(0) + 0.001 << 0.3 * theRepairTimeMedians.at(1) << 0.3 * theRepairTimeMedians.at(2) << 0.3 * theRepairTimeMedians.at(3) << 0.3 * theRepairTimeMedians.at(4);
    theRepairTimeStandardDeviations << 0.00001 << 0.00001 << 0.00001 << 0.00001 << 0.00001 ;
}



QObject* RSimpleBuildingRepairNeedsModel::getStructuralDamage() const
{
    return theStructuralDamageRatioResponse;
}

void RSimpleBuildingRepairNeedsModel::setStructuralDamage(QObject* value)
{
    theStructuralDamageRatioResponse = qobject_cast<RResponse*>(value);
}


QObject* RSimpleBuildingRepairNeedsModel::getIrreparabilityMultiplier() const
{
    return theIrreparabilityMultiplierResponse;
}

void RSimpleBuildingRepairNeedsModel::setIrreparabilityMultiplier(QObject* value)
{
    theIrreparabilityMultiplierResponse = qobject_cast<RResponse *>(value);
}



int RSimpleBuildingRepairNeedsModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    theRepairTimeResponse->setCurrentValue(0.0);
    theBusinessInterruptionTimeResponse->setCurrentValue(0.0);

    // End this quickly if there was no ground acceleration
    if (theStructuralDamageRatioResponse->getCurrentValue() < 0.001){
        return 0;
    }


    // "Functionality vs time" plot
    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }


    double theRepairtime = 0.0;
    double theReoccupancyTime = 0.0;
    double theIrreparabilityMultiplier = theIrreparabilityMultiplierResponse->getCurrentValue();
    double StructuralDamage = theStructuralDamageRatioResponse->getCurrentValue();


    if (theIrreparabilityMultiplier >= 1.0){
        theRepairtime = StructuralDamage * theRepairTimeMedians.at(4);

    } else {
        theRepairtime = theIrreparabilityMultiplier * theRepairTimeMedians.at(4);
    }


    theRepairTimeResponse->setCurrentValue(theRepairtime);
    theBusinessInterruptionTimeResponse->setCurrentValue(theReoccupancyTime);

    return 0;
}



