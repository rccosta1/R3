#include "RWaterDamModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RFragilityFunctions.h"
#include <math.h>

RWaterDamModel::RWaterDamModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;
    theFacilityType = 1;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RWaterDamModel::~RWaterDamModel()
{

}


int RWaterDamModel::resetTime()
{
    theResponse->setCurrentValue(0.0);
    timeOfEvent = 0.0;
    theTimeUnderRepairs = 0.0;
    return 0;
}


int RWaterDamModel::getFacilityType() const
{
    return theFacilityType;
}

void RWaterDamModel::setFacilityType(int value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    qCritical() << "Water reservoir has placeholder fragility curves";
    // Placeholders
    theRepairTimeMedians << 0.9 << 1.9 << 32.0 << 95.0;
    theRepairTimeStandardDeviations << 0.3 << 1.2 << 31.0 << 65.0;


    // Placeholders
    if (value == 0){
        thePGADamageMedians << 0.25 << 0.5 << 0.75 << 1.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}

QString RWaterDamModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterDamModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}

int RWaterDamModel::evaluateModel()
{

    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // Reseting the responses
    double myDamageRatio = 0.0;
    directLoss = 0.0;
    theFunctionality = 0.0;
    theResponse->setCurrentValue(0.0);
    theAvailabilityOfWater = false;


    // -------------------------------------------------------------------------------------------
    // 1 - HANDLING RESPONSES COMING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));


        // If this is a continuous response - this is what the model needs to function
        if (theResponseComingIntoThisModel && theResponseComingIntoThisModel->getCurrentValue() == 0.0){
            theAvailabilityOfWater = true;
        }
        else if (theResponseComingIntoThisModel->getCurrentValue() > theResponse->getCurrentValue()){
            theResponse->setCurrentValue(theResponseComingIntoThisModel->getCurrentValue());
        }
    }


    // -------------------------------------------------------------------------------------------
    // 1 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    this->calculateIntensities();

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

    if (timeOfEvent > 0.0){
        // Restoration times
        QVector<double> theRestorationTimes = theFragilityFunction.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);
        theFunctionality = 0.0;


        // Calculating the expected functionality
        for (int i = 0; i < theDamageProbabilities.size(); i++){
            theFunctionality += theDamageProbabilities.at(i) * theRestorationTimes.at(i);
        }
    }

    else {
        theFunctionality = 1.0;
    }


    if (theFunctionality > theThresholdfunctionality) {
        theResponse->setCurrentValue(0.0);
    }
    else {
        // Getting hold of how many days have passed since the facility lost functionality
        theTimeUnderRepairs += theCurrentTime->getCurrentValue() - lastEvaluationTime;

        double timeToRepair = theFragilityFunction.getTimeUntilGivenRestorationPercentage(theThresholdfunctionality, theDamageProbabilities, timeOfEvent);

        if (timeToRepair > theResponse->getCurrentValue()){
            theResponse->setCurrentValue(timeToRepair);
        }
    }

    // The fragility curves are placeholders, therefore
    theResponse->setCurrentValue(0.0);

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
