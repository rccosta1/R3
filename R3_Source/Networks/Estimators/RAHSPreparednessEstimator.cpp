#include "RAHSPreparednessEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>

RAHSPreparednessEstimator::RAHSPreparednessEstimator(QObject *parent, QString name)
    : RDisasterPreparednessEstimator(parent, name)
{
}


RAHSPreparednessEstimator::~RAHSPreparednessEstimator()
{

}


bool RAHSPreparednessEstimator::getGeneratorAvailability()
{
    double theProbability = 45.0/715.0; // 45 responses out of 715 indicated presence of generator
    double rng = theRNG.generateDouble();

    if(rng < theProbability){
        return true;

    } else {
        return false;
    }
}


bool RAHSPreparednessEstimator::getWaterAvailability()
{
    // AHS data: 414 No, 594 Yes
    double rng = theRNG.generateDouble();
    double theProbability = 594 / static_cast<double>(594 + 414);

    if(rng < theProbability){
        return true;

    } else {
        return false;
    }
}



bool RAHSPreparednessEstimator::getFinancialResourceAvailability()
{
    // AHS data: 124 No, 872 Yes
    double theProbability = 872 / static_cast<double>(124 + 872);
    double rng = theRNG.generateDouble();

    if(rng < theProbability){
        return true;

    } else {
        return false;
    }
}


QString RAHSPreparednessEstimator::getDisasterDestination()
{
    QString theDestination = "Displaced";
    return theDestination;
}

