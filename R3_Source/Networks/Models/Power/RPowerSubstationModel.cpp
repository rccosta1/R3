#include "RPowerSubstationModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RPowerLineModel.h"
#include "RPowerDamModel.h"
#include <math.h>

RPowerSubstationModel::RPowerSubstationModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

    theExpectedRepairTime = 0.0;
}

RPowerSubstationModel::~RPowerSubstationModel()
{
}


int RPowerSubstationModel::resetTime()
{
    timeOfEvent = 0.0;
    theResponse->setCurrentValue(0.0);
    theTimeUnderRepairs = 0.0;
    theExpectedRepairTime = 0.0;

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


RPowerSubstationModel::RFacilityType RPowerSubstationModel::getFacilityType() const
{
    return theFacilityType;
}

void RPowerSubstationModel::setFacilityType(RPowerSubstationModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 452
    theRepairTimeMedians << 1.0 << 3.0 << 7.0 << 30.0;
    theRepairTimeStandardDeviations << 0.5 << 1.5 << 3.5 << 15.0;


    // see Hazus 2.1 pg. 640
    theDamageRatios << 0.00 << 0.05 << 0.11 << 0.55 << 1.00;


    // Refer to HAZUS 2.1 pg. 454
    // Low voltage station with anchored subcomponents
    if (value == RPowerSubstationModel::LowVoltageAnchored){
        thePGADamageMedians << 0.15 << 0.29 << 0.45 << 0.90;
        thePGADamageDispersions << 0.70 << 0.55 << 0.45 << 0.45;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 10000000.0;
    }

    // Low voltage station with unanchored subcomponents
    else if (value == RPowerSubstationModel::LowVoltageUnanchored){
        thePGADamageMedians << 0.13 << 0.26 << 0.34 << 0.74;
        thePGADamageDispersions << 0.65 << 0.50 << 0.40 << 0.40;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 10000000.0;
    }

    // Medium voltage station with anchored subcomponents
    else if (value == RPowerSubstationModel::MediumVoltageAnchored){
        thePGADamageMedians << 0.15 << 0.25 << 0.35 << 0.70;
        thePGADamageDispersions << 0.60 << 0.50 << 0.40 << 0.40;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 20000000.0;
    }

    //  Medium voltage station with unanchored subcomponents
    else if (value == RPowerSubstationModel::MediumVoltageUnanchored){
        thePGADamageMedians << 0.10 << 0.20 << 0.30 << 0.50;
        thePGADamageDispersions << 0.60 << 0.50 << 0.40 << 0.40;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 20000000.0;
    }

    // High voltage station with anchored subcomponents
    else if (value == RPowerSubstationModel::HighVoltageAnchored){
        thePGADamageMedians << 0.11 << 0.15 << 0.20 << 0.47;
        thePGADamageDispersions << 0.50 << 0.45 << 0.35 << 0.40;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 50000000.0;
    }

    //  Medium voltage station with unanchored subcomponents
    else if (value == RPowerSubstationModel::HighVoltageUnanchored){
        thePGADamageMedians << 0.09 << 0.013 << 0.17 << 0.38;
        thePGADamageDispersions << 0.50 << 0.40 << 0.35 << 0.35;

        thePGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 50000000.0;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}



int RPowerSubstationModel::getYearBuilt() const
{
    return theYearBuilt;
}

void RPowerSubstationModel::setYearBuilt(int value)
{
    theYearBuilt = value;
}


QString RPowerSubstationModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RPowerSubstationModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}



int RPowerSubstationModel::evaluateModel()
{
    // Output to say this model is being analysed
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Initial declaration of some variables
    double myDamageRatio = 0.0;
    directLoss = 0.0;
    theFunctionality = 1.0;
    theResponse->setCurrentValue(0.0);

    // -------------------------------------------------------------------------------------------
    // 1 - HANDLING RESPONSES COMING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    // Checking the availability of the suppliers
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // Get hold of the response coming to this model
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        // Check if the response coming to this model is > 0, meaning the object supplying this object is under repairs
        if (theResponseComingIntoThisModel->getCurrentValue() > 0){
            theResponse->setCurrentValue(theResponseComingIntoThisModel->getCurrentValue());
        }
    }

    // -------------------------------------------------------------------------------------------
    // 2 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    // Earthquake
    //this->calculateIntensities(); // Getting the PGA

    // Fragility Functions
    RFragilityFunctions theFragilityFunction(theDomain);

    // Passing the fragility curves to the physical object that will calculate damage
    theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
    theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);
    theFragilityFunction.setRestorationMediansAndStandardDeviations(theRepairTimeMedians, theRepairTimeStandardDeviations);

    QCoreApplication::processEvents();
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
    if (theExpectedRepairTime > theCurrentTime->getCurrentValue() - timeOfEvent){
        theFunctionality = 0.0;
        theResponse->setCurrentValue(qMax(theResponse->getCurrentValue(), theExpectedRepairTime - (theCurrentTime->getCurrentValue() - timeOfEvent)));
        //theResponse->setCurrentValue(theCapacity * (theCurrentTime->getCurrentValue() - lastEvaluationTime));
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
