#include "RPowerDamModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RPowerDamModel::RPowerDamModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;
    theFacilityType = RPowerDamModel::SmallUnanchored;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}

RPowerDamModel::~RPowerDamModel()
{
}


int RPowerDamModel::resetTime()
{
    timeOfEvent = 0.0;
    theExpectedRepairTime = 0.0;
    theTimeUnderRepairs = 0.0;
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


RPowerDamModel::RFacilityType RPowerDamModel::getFacilityType() const
{
    return theFacilityType;
}

void RPowerDamModel::setFacilityType(RPowerDamModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 452
    theRepairTimeMedians << 0.5 << 3.6 << 22.0 << 65.0;
    theRepairTimeStandardDeviations << 0.1 << 3.6 << 21.0 << 30.0;


    // see Hazus 2.1 pg. 640
    theDamageRatios << 0.00 << 0.08 << 0.35 << 0.72 << 1.00;


    // Refer to HAZUS 2.1 pg. 455 subcomponents
    // Small plants with anchored subcomponents
    if (value == RPowerDamModel::SmallAnchored){
        thePGADamageMedians << 0.10 << 0.21 << 0.48 << 0.78;
        thePGADamageDispersions << 0.55 << 0.55 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 100000000.0;
    }

    // Small plants with unanchored subcomponents
    else if (value == RPowerDamModel::SmallUnanchored){
        thePGADamageMedians << 0.10 << 0.17 << 0.42 << 0.58;
        thePGADamageDispersions << 0.50 << 0.50 << 0.50 << 0.55;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 100000000.0;
    }

    // Medium/large plants with anchored subcomponents
    else if (value == RPowerDamModel::LargeAnchored){
        thePGADamageMedians << 0.10 << 0.25 << 0.52 << 0.92;
        thePGADamageDispersions << 0.60 << 0.60 << 0.55 << 0.55;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 500000000.0;
    }

    // Medium/large plants with unanchored subcomponents
    else if (value == RPowerDamModel::LargeUnanchored){
        thePGADamageMedians << 0.10 << 0.22 << 0.49 << 0.79;
        thePGADamageDispersions << 0.60 << 0.55 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        replacementCost = 500000000.0;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}


double RPowerDamModel::getCapacity() const
{
    return theCapacity;
}

void RPowerDamModel::setCapacity(double value)
{
    theCapacity = value;
}


int RPowerDamModel::getYearBuilt() const
{
    return theYearBuilt;
}

void RPowerDamModel::setYearBuilt(int value)
{
    theYearBuilt = value;
}


double RPowerDamModel::getLength() const
{
    return theLength;
}

void RPowerDamModel::setLength(double value)
{
    theLength = value;
}


double RPowerDamModel::getHeight() const
{
    return theHeight;
}

void RPowerDamModel::setHeight(double value)
{
    theHeight = value;
}


QString RPowerDamModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RPowerDamModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


int RPowerDamModel::evaluateModel()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    double myDamageRatio = 0.0;
    directLoss = 0.0;
    theFunctionality = 1.0;
    theResponse->setCurrentValue(0.0);

    // -------------------------------------------------------------------------------------------
    // 1 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
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
        theResponse->setCurrentValue(theExpectedRepairTime - (theCurrentTime->getCurrentValue() - timeOfEvent));
        //theResponse->setCurrentValue(theCapacity * (theCurrentTime->getCurrentValue() - lastEvaluationTime));
    } else {
        theResponse->setCurrentValue(0.0);
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
