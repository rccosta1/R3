#include "RWaterTreatmentFacilityModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RPowerLineModel.h"
#include "RWaterPipelineModel.h"
#include <math.h>

RWaterTreatmentFacilityModel::RWaterTreatmentFacilityModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RWaterTreatmentFacilityModel::~RWaterTreatmentFacilityModel()
{

}


int RWaterTreatmentFacilityModel::resetTime()
{
    timeOfEvent = 0.0;
    theAvailabilityOfWater = false;
    theAvailabilityOfPower = false;
    theExpectedRepairTime = 0.0;
    theTimeUnderRepairs = 0.0;

    handlePacketsThatReachedFinalDestination();
    packetsCurrentlyOnboard.clear();
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


RWaterTreatmentFacilityModel::RFacilityType RWaterTreatmentFacilityModel::getFacilityType() const
{
    return theFacilityType;
}

void RWaterTreatmentFacilityModel::setFacilityType(RWaterTreatmentFacilityModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 394
    theRepairTimeMedians << 0.9 << 1.9 << 32.0 << 95.0;
    theRepairTimeStandardDeviations << 0.3 << 1.2 << 31.0 << 65.0;

    // see Hazus 2.1 pg.
    theDamageRatios << 0.00 << 0.08 << 0.4 << 0.77 << 1.00;


    // Refer to HAZUS 2.1 pg. 398
    // Small plants with anchored subcomponents < 50MGD
    if (value == RWaterTreatmentFacilityModel::PWT1){
        thePGADamageMedians << 0.25 << 0.38 << 0.53 << 0.83;
        thePGADamageDispersions << 0.50 << 0.50 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 30000000.0;
    }

    // Small plants with unanchored subcomponents < 50 MGD
    else if (value == RWaterTreatmentFacilityModel::PWT2){
        thePGADamageMedians << 0.16 << 0.27 << 0.53 << 0.83;
        thePGADamageDispersions << 0.40 << 0.40 << 0.60 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 30000000.0;
    }

    // Medium plants with anchored subcomponents 50-200 MGD
    else if (value == RWaterTreatmentFacilityModel::PWT3){
        thePGADamageMedians << 0.37 << 0.52 << 0.73 << 1.28;
        thePGADamageDispersions << 0.40 << 0.40 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 100000000.0;
    }

    // Medium plants with unanchored subcomponents 50-200 MGD
    else if (value == RWaterTreatmentFacilityModel::PWT4){
        thePGADamageMedians << 0.20 << 0.35 << 0.75 << 1.28;
        thePGADamageDispersions << 0.40 << 0.40 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 100000000.0;
    }

    // Large plants with anchored subcomponents > 200 MGD
    else if (value == RWaterTreatmentFacilityModel::PWT5){
        thePGADamageMedians << 0.44 << 0.58 << 0.87 << 1.57;
        thePGADamageDispersions << 0.40 << 0.40 << 0.45 << 0.45;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 360000000.0;
    }

    // Large plants with unanchored subcomponents > 200 MGD
    else if (value == RWaterTreatmentFacilityModel::PWT6){
        thePGADamageMedians << 0.22 << 0.35 << 0.87 << 1.57;
        thePGADamageDispersions << 0.40 << 0.40 << 0.45 << 0.45;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 360000000.0;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}


int RWaterTreatmentFacilityModel::getYearBuilt() const
{
    return theYearBuilt;
}

void RWaterTreatmentFacilityModel::setYearBuilt(int value)
{
    theYearBuilt = value;
}


double RWaterTreatmentFacilityModel::getCapacity() const
{
    return theCapacity;
}

void RWaterTreatmentFacilityModel::setCapacity(double value)
{
    theCapacity = value;
}


QString RWaterTreatmentFacilityModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterTreatmentFacilityModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}


QString RWaterTreatmentFacilityModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RWaterTreatmentFacilityModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}



int RWaterTreatmentFacilityModel::evaluateModel()
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
    // Define a list of packets to be loaded
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
