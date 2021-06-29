#include "RRailwayDispatchFacilityModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RRailwayDispatchFacilityModel::RRailwayDispatchFacilityModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{

}


RRailwayDispatchFacilityModel::~RRailwayDispatchFacilityModel()
{
}


int RRailwayDispatchFacilityModel::getFacilityType() const
{
    return theFacilityType;
}

void RRailwayDispatchFacilityModel::setFacilityType(int value)
{
    theFacilityType = value;
}



void RRailwayDispatchFacilityModel::setFragilityCurves(int value)
{


    // Refer to HAZUS 2.1 pg. 324
    // Facility with anchored components w/ backup power
    if (value == 1){
        thePGADamageMedians << 0.15 << 0.35 << 0.80 << 1.50;
        thePGDDamageMedians << 0.75 << 0.65 << 0.80 << 0.80;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with anchored components w/o backup power
    else if (value == 2){
        thePGADamageMedians << 0.12 << 0.27 << 0.80 << 1.50;
        thePGDDamageMedians << 0.50 << 0.45 << 0.80 << 0.80;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/ backup power
    else if (value == 3){
        thePGADamageMedians << 0.13 << 0.28 << 0.80 << 1.50;
        thePGDDamageMedians << 0.55 << 0.50 << 0.80 << 0.80;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/o backup power
    else if (value == 4){
        thePGADamageMedians << 0.11 << 0.23 << 0.80 << 1.50;
        thePGDDamageMedians << 0.45 << 0.40 << 0.80 << 0.80;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}



int RRailwayDispatchFacilityModel::evaluateModel()
{

    // Setting the fragility curves
    setFragilityCurves(theFacilityType);

    // Earthquake
    //calculateIntensities();






    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    return 1;

}
