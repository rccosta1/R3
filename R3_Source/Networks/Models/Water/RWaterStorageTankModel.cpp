#include "RWaterStorageTankModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RWaterStorageTankModel::RWaterStorageTankModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;
    theShortageDuration = 0.0;
    theFillLevel = 0;
    theMaxStorageCapacity = 0.0;
    theNumberOfTanks = 1;
    theReserve = 0.0;
    lastEvaluationTime = 0.0;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}

RWaterStorageTankModel::~RWaterStorageTankModel()
{

}


int RWaterStorageTankModel::resetTime()
{
    timeOfEvent = 0.0;
    theReserve = theMaxStorageCapacity * theFillLevel->getCurrentValue();
    theCurrentCapacity = theMaxStorageCapacity;

    theTimeUnderRepairsVector.clear();
    theTankDamageStateVector.clear();
    theDamageProbabilities.clear();
    theResponse->setCurrentValue(0.0);
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


RWaterStorageTankModel::RFacilityType RWaterStorageTankModel::getFacilityType() const
{
    return theFacilityType;
}

void RWaterStorageTankModel::setFacilityType(RWaterStorageTankModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 394
    theRepairTimeMedians << 0.0 << 1.2  << 3.1  << 93.0  << 155.0 ;
    theRepairTimeStandardDeviations << 0.0 << 0.4 << 2.7 << 85.0 << 120.0;

    // See Hazus 2.1 pg. 639
    theDamageRatios << 0.00 << 0.20 << 0.4 << 0.8 << 1.0;


    // If the tank cannot suffer damage
    if (value == RWaterStorageTankModel::NoDamage){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }


    // Refer to HAZUS 2.1 pg. 394 - same as for water
    // On-Ground Anchored Concrete Tank
    else if (value == RWaterStorageTankModel::OnGroundConcreteAnchored){

        thePGADamageMedians << 0.25 << 0.52 << 0.95 << 1.64;
        thePGADamageDispersions << 0.55 << 0.70 << 0.60 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 1500000.0;
    }


    // On-Ground Unanchored Concrete Tank
    else if (value == RWaterStorageTankModel::OnGroundConcreteAnchored){

        thePGADamageMedians << 0.18 << 0.42 << 0.70 << 1.04;
        thePGADamageDispersions << 0.60 << 0.70 << 0.55 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 1500000.0;
    }


    // On-Ground Anchored Steel Tank
    else if (value == RWaterStorageTankModel::OnGroundConcreteUnanchored){

        thePGADamageMedians << 0.30 << 0.70 << 1.25 << 1.60;
        thePGADamageDispersions << 0.60 << 0.60 << 0.65 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 800000.0;
    }


    // On-Ground Unanchored Steel Tank
    else if (value == RWaterStorageTankModel::OnGroundSteelUnanchored){

        thePGADamageMedians << 0.15 << 0.35 << 0.68 << 0.95;
        thePGADamageDispersions << 0.70 << 0.75 << 0.75 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 800000.0;
    }


    // Above-Ground Steel Tank
    else if (value == RWaterStorageTankModel::AboveGroundSteel){

        thePGADamageMedians << 0.18 << 0.55 << 1.15 << 1.50;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 800000.0;
    }


    // Above-ground wood
    else if (value == RWaterStorageTankModel::AboveGroundWood){

        thePGADamageMedians << 0.15 << 0.40 << 0.70 << 0.90;
        thePGADamageDispersions << 0.60 << 0.60 << 0.70 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;

        replacementCost = 80000.0;
    }


    // On-Ground Wood Tank
    else if (value == RWaterStorageTankModel::BuriedConcrete){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 1.0 << 1.0 << 1.0 << 1.0;

        thePGDDamageMedians << 2.0 << 4.0 << 8.0 << 12.0;
        thePGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;

        replacementCost = 30000.0;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}


int RWaterStorageTankModel::getYearBuilt() const
{
    return theYearBuilt;
}

void RWaterStorageTankModel::setYearBuilt(int value)
{
    theYearBuilt = value;
}


double RWaterStorageTankModel::getMaxCapacity() const
{
    return theMaxStorageCapacity;
}

void RWaterStorageTankModel::setMaxCapacity(double value)
{
    theMaxStorageCapacity = value;
    theReserve = theMaxStorageCapacity * theFillLevel->getCurrentValue();
}

void RWaterStorageTankModel::changeCurrentCapacity(double value)
{
    theCurrentCapacity = value;
}


int RWaterStorageTankModel::getNumberOfStorageTanks() const
{
    return theNumberOfTanks;
}

void RWaterStorageTankModel::setNumberOfStorageTanks(int value)
{
    theNumberOfTanks = value;
}

QString RWaterStorageTankModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterStorageTankModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}


QObject * RWaterStorageTankModel::getFillLevel() const
{
    return theFillLevel;
}

void RWaterStorageTankModel::setFillLevel(QObject *value)
{
    theFillLevel = qobject_cast<RParameter *>(value);
}


void RWaterStorageTankModel::setReserve(double value)
{
    theReserve = value;
    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
    }
}


int RWaterStorageTankModel::extractSupply(double amount)
{
    theReserve -= amount;

    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
        return 0;
    }

    else {
        return 1;
    }
}


int RWaterStorageTankModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    //Fix to the "reserve vs time" plot
    if(lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    // Reseting the responses
    directLoss = 0.0;
    theFunctionality = 1.0;
    theResponse->setCurrentValue(0.0);
    theAvailabilityOfWater = false;

    // -------------------------------------------------------------------------------------------
    // 1 - HANDLING RESPONSES COMING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    // Check responses coming to this model
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


    // Define the tank capacity equal for all tanks (note: a tank farm contains several storage tanks)
    theTankCapacity = theMaxStorageCapacity / theNumberOfTanks;

    // -------------------------------------------------------------------------------------------
    // 2 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    if (theFacilityType != RWaterStorageTankModel::NoDamage){

        // Earthquake
        //this->calculateIntensities();

        // Fragility Functions
        RFragilityFunctions theFragilityFunction(theDomain);

        // Passing the fragility curves to the physical object that will calculated damage
        theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
        theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);

        // Getting the damage and restoration results
        if(thePGAIntensity > 0.0 && timeOfEvent == 0.0){
            timeOfEvent = theCurrentTime->getCurrentValue();

            theDamageProbabilities = theFragilityFunction.getDamageProbabilities(thePGAIntensity, thePGDIntensity);

            // |----DS0----|---DS1---|--DS2--|--DS3--|-DS4-|
            // 0----------lim1------lim2----lim3----lim4---1
            lim1 = theDamageProbabilities.at(0);
            lim2 = lim1 + theDamageProbabilities.at(1);
            lim3 = lim2 + theDamageProbabilities.at(2);
            lim4 = lim3 + theDamageProbabilities.at(3);

            // Getting the damage and restoration realizations
            for (int i = 0; i < theNumberOfTanks; i++){
                double rng = rand();
                theTimeUnderRepairsVector << 0.0;

                // Generating realizations of a discrete random variable for the damage of the individual tanks
                if (rng < lim1){
                    theTankDamageStateVector << 0;

                } else if (rng >= lim1 && rng < lim2){
                    theTankDamageStateVector << 1;

                } else if (rng >= lim2 && rng < lim3){
                    theTankDamageStateVector << 2;

                } else if (rng >= lim3 && rng < lim4){
                    theTankDamageStateVector << 3;

                } else if (rng >= lim4){
                    theTankDamageStateVector << 4;

                } else {
                    qCritical() << "Something went wrong in the RTankFarmModel, the rng is" << rng;
                }

                if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
                    directLoss += theDamageProbabilities.at(theTankDamageStateVector.at(i)) * replacementCost;
                }
            }
        }

        // Check it is needed to compute functionality of each tank
        if (timeOfEvent > 0.0){

            changeCurrentCapacity(theMaxStorageCapacity);

            // Calculate functionality
            for (int i = 0; i < theNumberOfTanks; i++){

                // If the tank is repaired, change its original damage state
                if (theTimeUnderRepairsVector.at(i) > theRepairTimeMedians.at(theTankDamageStateVector.at(i))){
                    theTankDamageStateVector[i] = 0;
                }

                // If the tank is under repairs
                if (theTankDamageStateVector.at(i) > 0) {
                    theTimeUnderRepairsVector[i] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
                    theExpectedRepairTime = qMax(theTimeUnderRepairsVector[i],theExpectedRepairTime);

                    // If this tank is damaged, reduce the tank farm capacity
                    changeCurrentCapacity(theCurrentCapacity - theTankCapacity);

                    if (theCurrentCapacity > 0.0){
                        this->setReserve(qMin(theReserve - theTankCapacity * (theReserve / theMaxStorageCapacity),0.0));
                    }
                } // if
            } //
        } // if
    }

    // Conditions that define availability
    if (theExpectedRepairTime > 0.0 || theAvailabilityOfWater == false){
        theFunctionality = 0.0;
        theResponse->setCurrentValue(qMax(theResponse->getCurrentValue(),theExpectedRepairTime));
        if (theOutputLevel == RDomain::Maximum){
            qCritical() << "Expected repair time for:"<< this->objectName() << ":" << theExpectedRepairTime - (theCurrentTime->getCurrentValue() - timeOfEvent);
        }
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}

