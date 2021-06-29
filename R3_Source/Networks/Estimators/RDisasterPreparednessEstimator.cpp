#include "RDisasterPreparednessEstimator.h"
#include "RMethod.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>


RDisasterPreparednessEstimator::RDisasterPreparednessEstimator(QObject *parent, QString name)
    : RObject(parent, name)
{
}


void RDisasterPreparednessEstimator::setTenure(QString value)
{
    theTenure = value;
}

void RDisasterPreparednessEstimator::setIncome(QString value)
{
    theHouseholdIncome = value;
}

void RDisasterPreparednessEstimator::setImmigrationStatus(QString value)
{
    theImmigrationStatus = value;
}

void RDisasterPreparednessEstimator::setChildren(bool value)
{
    thePresenceOfChildren = value;
}

void RDisasterPreparednessEstimator::setElders(bool value)
{
    thePresenceOfSeniors = value;
}

void RDisasterPreparednessEstimator::setSize(int value)
{
    theHouseholdSize = value;
}

void RDisasterPreparednessEstimator::setAge(QString value)
{
    theAge = value;
}

void RDisasterPreparednessEstimator::setCarOwnership(bool value)
{
    theCarOwnership = value;
}

void RDisasterPreparednessEstimator::setHispanicFlag(bool value)
{
    theHispanicFlag = value;
}

void RDisasterPreparednessEstimator::setHigherEducationFlag(bool value)
{
    theHigherEducationFlag = value;
}


RDisasterPreparednessEstimator::~RDisasterPreparednessEstimator()
{

}

bool RDisasterPreparednessEstimator::getGeneratorAvailability()
{
    return false;
}


bool RDisasterPreparednessEstimator::getWaterAvailability()
{
    return false;
}


bool RDisasterPreparednessEstimator::getSocialSupportAvailability()
{
    return false;
}

bool RDisasterPreparednessEstimator::getFinancialResourceAvailability()
{
    return false;
}

QString RDisasterPreparednessEstimator::getDisasterDestination()
{
    return "";
}

