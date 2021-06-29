#include "RRailwayFuelFacilityModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RRailwayFuelFacilityModel::RRailwayFuelFacilityModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a continuous response
    theDomain->createObject(objectName() + "ContinuousResponse", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RRailwayFuelFacilityModel::~RRailwayFuelFacilityModel()
{
}


int RRailwayFuelFacilityModel::getFacilityType() const
{
    return theFacilityType;
}

void RRailwayFuelFacilityModel::setFacilityType(int value)
{
    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 320
    theRepairTimeMedians << 0.9 << 1.5 << 15.0 << 65.0;
    theRepairTimeStandardDeviations << 0.05 << 1.5 << 15.0 << 50.0;



    // Refer to HAZUS 2.1 pg. 323
    // Facility with anchored components w/ backup power
    if (value == 1){
        thePGADamageMedians << 0.23 << 0.43 << 0.64 << 1.10;
        thePGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with anchored components w/o backup power
    else if (value == 2){
        thePGADamageMedians << 0.12 << 0.27 << 0.64 << 1.10;
        thePGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/ backup power
    else if (value == 3){
        thePGADamageMedians << 0.10 << 0.23 << 0.48 << 0.80;
        thePGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/o backup power
    else if (value == 4){
        thePGADamageMedians << 0.09 << 0.20 << 0.48 << 0.80;
        thePGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility w/ buried tanks
    else if (value == 5){
        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 4.0 << 8.0 << 24.0 << 24.0;
        thePGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }

    theFacilityType = value;
}




int RRailwayFuelFacilityModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Earthquake
    QVector<double> theIntensities = calculateIntensities();
    double thePGAIntensity = theIntensities.at(0);
    double thePGDIntensity = theIntensities.at(1);

    // Fragility Functions
    RFragilityFunctions theFragilityFunction(theDomain);


    // Passing the fragility curves to the physical object that will calculated damage
    theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
    theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);
    theFragilityFunction.setRestorationMediansAndStandardDeviations(theRepairTimeMedians, theRepairTimeStandardDeviations);


    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
        theDamageProbabilities = theFragilityFunction.getDamageProbabilities(thePGAIntensity, thePGDIntensity);
    }
    QVector<double> theRestorationTimes = theFragilityFunction.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);
    theFunctionality = 0.0;


    // Calculating the expected functionality
    for (int i = 0; i < theDamageProbabilities.size(); i++){
        theFunctionality += theDamageProbabilities.at(i) * theRestorationTimes.at(i);
    }

    

    if (theFunctionality >= theThresholdfunctionality) {
        
    }


    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    return 1;
}
