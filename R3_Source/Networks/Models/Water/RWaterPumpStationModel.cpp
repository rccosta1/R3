#include "RWaterPumpStationModel.h"
#include "RParameter.h"
#include <math.h>

RWaterPumpStationModel::RWaterPumpStationModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;
    replacementCost = 0.0;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
    theExpectedRepairTime = 0.0;
    theTimeUnderRepairs = 0.0;
}


RWaterPumpStationModel::~RWaterPumpStationModel()
{
}


int RWaterPumpStationModel::resetTime()
{
    timeOfEvent = 0.0;
    theTimeUnderRepairs = 0.0;
    theExpectedRepairTime = 0.0;
    theAvailabilityOfWater = false;
    theAvailabilityOfPower = false;

    handlePacketsThatReachedFinalDestination();
    packetsCurrentlyOnboard.clear();
    theDamageProbabilities.clear();
    theResponse->setCurrentValue(0.0);

    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> thePGAs = stringToDoubleVector(qobject_cast<RLocation*>(theLocation)->getPGAIntensity());

    if (thePGACounter > (thePGAs.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough PGA values!";
        thePGACounter = 0;
    }

    thePGAIntensity = thePGAs.at(thePGACounter);
    thePGACounter++;

    return 0;
}


RWaterPumpStationModel::RFacilityType RWaterPumpStationModel::getFacilityType() const
{
    return theFacilityType;
}

void RWaterPumpStationModel::setFacilityType(RWaterPumpStationModel::RFacilityType value)
{
    theFacilityType = value;
    timeOfEvent = 0.0;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 394
    theRepairTimeMedians << 0.9 << 3.1 << 13.5 << 35.0;
    theRepairTimeStandardDeviations << 0.3 << 2.7 << 10.0 << 18.0;

    // see Hazus 2.1 pg. 638
    theDamageRatios << 0.00 << 0.05 << 0.38 << 0.8 << 1.0;

    // Refer to HAZUS 2.1 pg. 399 - same as for water
    // Small plants with anchored subcomponents
    if (value == RWaterPumpStationModel::NoDamage){
        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        replacementCost = 150000.0;
    }

    else if (value == RWaterPumpStationModel::SmallAnchored){
        thePGADamageMedians << 0.15 << 0.36 << 0.66 << 1.50;
        thePGADamageDispersions << 0.70 << 0.65 << 0.65 << 0.80;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 150000.0;
    }

    // Small plants with unanchored subcomponents
    else if (value == RWaterPumpStationModel::SmallUnanchored){
        thePGADamageMedians << 0.13 << 0.28 << 0.66 << 1.50;
        thePGADamageDispersions << 0.60 << 0.50 << 0.65 << 0.80;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 150000.0;
    }

    // Medium/Large plants with anchored subcomponents
    else if (value == RWaterPumpStationModel::LargeAnchored){
        thePGADamageMedians << 0.15 << 0.36 << 0.77 << 1.50;
        thePGADamageDispersions << 0.75 << 0.65 << 0.65 << 0.80;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 525000.0;
    }

    // Medium/Large plants with unanchored subcomponents
    else if (value == RWaterPumpStationModel::LargeUnanchored){
        thePGADamageMedians << 0.13 << 0.28 << 0.77 << 1.50;
        thePGADamageDispersions << 0.60 << 0.50 << 0.65 << 0.80;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 525000.0;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}


int RWaterPumpStationModel::getYearBuilt() const
{
    return theYearBuilt;
}

void RWaterPumpStationModel::setYearBuilt(int value)
{
    theYearBuilt = value;
}


double RWaterPumpStationModel::getPumpingCapacity() const
{
    return thePumpingCapacity;
}

void RWaterPumpStationModel::setPumpingCapacity(double value)
{
    thePumpingCapacity = value;
}


QString RWaterPumpStationModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterPumpStationModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}


QString RWaterPumpStationModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RWaterPumpStationModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


int RWaterPumpStationModel::evaluateModel()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Reseting the responses
    double myDamageRatio = 0.0;
    directLoss = 0.0;
    theFunctionality = 1.0;
    theResponse->setCurrentValue(0.0);
    theAvailabilityOfWater = false;
    theAvailabilityOfPower = false;

    // -------------------------------------------------------------------------------------------
    // 1 - HANDLING RESPONSES COMING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    // Define a list of responses to be loaded
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        if (theResponseComingIntoThisModel == nullptr){
            qCritical() << "There is a problem with supplier list for" << this->objectName();
        }

        // If this is a continuous response - this is what the model needs to function
        if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
            theAvailabilityOfPower = true;

        } else {
            // Set the response of this model as the time needed to restore power
            theResponse->setCurrentValue(qMin(theResponse->getCurrentValue(), theResponseComingIntoThisModel->getCurrentValue()));
        }
    }

    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel == nullptr){
            qCritical() << "There is a problem with supplier list for" << this->objectName();
        }

        // If this is a continuous response - this is what the model needs to function
        if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
            theAvailabilityOfWater = true;

        } else {
            // The response of this model is how long it will take for it to be operational again
            // Hence why I am taking the max between the time for water and power to be restored
            theResponse->setCurrentValue(qMin(theResponse->getCurrentValue(), theResponseComingIntoThisModel->getCurrentValue()));
        }
    }

    // -------------------------------------------------------------------------------------------
    // 2 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    // Earthquake
    //this->calculateIntensities();

    // Fragility Functions
    RFragilityFunctions theFragilityFunction(theDomain);

    // Passing the fragility curves to the physical object that will calculated damage
    theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
    theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);
    theFragilityFunction.setRestorationMediansAndStandardDeviations(theRepairTimeMedians, theRepairTimeStandardDeviations);

    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 && timeOfEvent == 0.0){
        timeOfEvent = theCurrentTime->getCurrentValue();

        theDamageProbabilities = theFragilityFunction.getDamageProbabilities(thePGAIntensity, thePGDIntensity);

        // Calculating the expected functionality
        for (int i = 0; i < theDamageProbabilities.size(); i++){
            myDamageRatio += theDamageProbabilities.at(i) * theDamageRatios.at(i);
        }

        directLoss = myDamageRatio * replacementCost;
        theExpectedRepairTime = theFragilityFunction.getExpectedRepairTime(theThresholdfunctionality, theDamageProbabilities);
    }

    // If the repair time has passed, the facility is not functional
    if (theExpectedRepairTime > theCurrentTime->getCurrentValue() - timeOfEvent || theAvailabilityOfWater == false || theAvailabilityOfPower == false){
        theFunctionality = 0.0;
        theResponse->setCurrentValue(qMax(theResponse->getCurrentValue(), theExpectedRepairTime - (theCurrentTime->getCurrentValue() - timeOfEvent)));
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
