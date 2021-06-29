#include "RShelterModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RSimpleBuildingDamageModel.h"
#include "RSimpleBuildingLossModel.h"
#include "RSimpleBuildingRepairNeedsModel.h"
#include <math.h>

RShelterModel::RShelterModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RShelterModel::~RShelterModel()
{
}


int RShelterModel::resetTime()
{
    theAvailabilityOfLPG = true;
    theAvailabilityOfWater = true;
    theAvailabilityOfPower = true;
    lastEvaluationTime = 0;
    currentNumberOfPeople = 0;
    timeNeededForRepairs = 0.0;
    theTimeUnderRepairs = 0.0;
    thePowerShortageDuration = 0.0;
    theWaterShortageDuration = 0.0;


    structuralDamageProbabilities.clear();
    nonStructuralDriftDamageProbabilities.clear();
    nonStructuralAccelerationDamageProbabilities.clear();
    structuralDamageRatios.clear();
    nonStructuralDriftDamageRatios.clear();
    nonStructuralAccelerationDamageRatios.clear();
    timeToResumeServices = 0.0;
    waterNetworkExpectedRepairTime = 1000.0;
    powerNetworkExpectedRepairTime = 1000.0;

    return 0;
}



int RShelterModel::getNumberOfBedsAvailable() const
{
    return theNumberOfBedsAvailable;
}

void RShelterModel::setNumberOfBedsAvailable(int value)
{
    theNumberOfBedsAvailable = value;
}



int RShelterModel::getNumberOfBedsThatCanBeCreated() const
{
    return theNumberOfBedsToBeCreated;
}

void RShelterModel::setNumberOfBedsThatCanBeCreated(int value)
{
    theNumberOfBedsToBeCreated = value;
}



int RShelterModel::getNumberOfStoreys() const
{
    return theNumberOfStoreys;
}

void RShelterModel::setNumberOfStoreys(int value)
{
    theNumberOfStoreys = value;
}



int RShelterModel::getFootPrintArea() const
{
    return theFootPrintArea;
}

void RShelterModel::setFootPrintArea(int value)
{
    theFootPrintArea = value;
}



QString RShelterModel::getBuildingType() const
{
    return theBuildingType;
}

void RShelterModel::setBuildingType(QString value)
{
    theBuildingType = value;
}



QString RShelterModel::getOccupancyClass() const
{
    return theOccupancyClass;
}

void RShelterModel::setOccupancyClass(QString value)
{
    theOccupancyClass = value;
}



QString RShelterModel::getDesignLevel() const
{
    return theDesignLevel;
}

void RShelterModel::setDesignLevel(QString value)
{
    theDesignLevel = value;
}


QObject * RShelterModel::getStructuralDamage() const
{
    return theStructuralDamageResponse;
}

void RShelterModel::setStructuralDamage(QObject *value)
{
    theStructuralDamageResponse = qobject_cast<RResponse *>(value);
}


QObject * RShelterModel::getDriftDamage() const
{
    return theNDDamageResponse;
}

void RShelterModel::setDriftDamage(QObject *value)
{
    theNDDamageResponse = qobject_cast<RResponse *>(value);
}


QObject * RShelterModel::getAccelerationDamage() const
{
    return theNADamageResponse;
}

void RShelterModel::setAccelerationDamage(QObject *value)
{
    theNADamageResponse = qobject_cast<RResponse *>(value);
}


QObject * RShelterModel::getDirectEconomicLoss() const
{
    return theDirectEconomicLossResponse;
}

void RShelterModel::setDirectEconomicLoss(QObject* value)
{
    theDirectEconomicLossResponse = qobject_cast<RResponse *>(value);
}


QObject * RShelterModel::getRepairTime() const
{
    return theRepairTimeResponse;
}

void RShelterModel::setRepairTime(QObject* value)
{
    theRepairTimeResponse = qobject_cast<RResponse *>(value);
}


QString RShelterModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RShelterModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}



QString RShelterModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RShelterModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}



QString RShelterModel::getHouseholdsList()
{
    return parameterListToString(theHouseholdList);
}

void RShelterModel::setHouseholdsList(QString value)
{
    theHouseholdList = stringToParameterList(value);
}



int RShelterModel::evaluateModel(RGradientType theGradientType)
{
//    if (theOutputLevel == RDomain::Maximum) {
//        qDebug() << "";
//        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
//    }

//    if (lastEvaluationTime == 0.0){
//        lastEvaluationTime = theCurrentTime->getCurrentValue();
//    }



//    // -------------------------------------------------------------------------------------------
//    // 1 - Checking responses coming to this model and the availability of utilities
//    // -------------------------------------------------------------------------------------------
//    theAvailabilityOfWater = false;
//    theAvailabilityOfPower = false;
//    QList<QPointer <RParameter>> thePacketList;


//    // Go over all the responses getting to this object and take actions accordingly
//    // Check if water is available
//    for (int i = 0; i < theWaterSupplierList.size(); i++){

//        // Cast the input as a response
//        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));
//        waterNetworkExpectedRepairTime = qMin(theResponseComingIntoThisModel->getCurrentValue(),waterNetworkExpectedRepairTime);

//        if (theResponseComingIntoThisModel){

//            // Check for availability of water
//            if (theResponseComingIntoThisModel->getCurrentValue() == 0.0){
//                theAvailabilityOfWater = true;
//            }
//            else {
//                // qCritical() << this->objectName() << "does not have water available";
//                theWaterShortageDuration += theCurrentTime->getCurrentValue() - lastEvaluationTime;
//            }
//        }
//    }


//    // Check if power is available in the residential zone (i.e., not at the residential building)
//    for (int i = 0; i < thePowerSupplierList.size(); i++){

//        // Cast the input as a response
//        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));
//        powerNetworkExpectedRepairTime = qMin(theResponseComingIntoThisModel->getCurrentValue(),powerNetworkExpectedRepairTime);

//        if (theResponseComingIntoThisModel){

//            // Check for availability of power
//            if (theResponseComingIntoThisModel->getCurrentValue() == 0.0){
//                theAvailabilityOfPower = true;
//            }
//            else {
//                // qCritical() << this->objectName() << "does not have electrical power available";
//                thePowerShortageDuration += theCurrentTime->getCurrentValue() - lastEvaluationTime;
//            }
//        }
//    }



//    // Check if the households in this zone have an object to request disaster aid from
//    for (int i = 0; i < theHouseholdList.size(); i++){

//        // Cast the input as a response
//        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theHouseholdList.at(i));

//        if (theResponseComingToThisModel){
//            thePacketList << theHouseholdList.at(i);
//        }
//    }



//    // Load the packets received
//    int numPackets = 0;
//    this->loadPackets(thePacketList);
//    numPackets = packetsCurrentlyOnboard.size();


//    if (numPackets > 0){

//        // Extract contents
//        for (int j = 0; j < numPackets; j++) {

//            // Get hold of what is the content of this packet
//            QVector<double> packetContents = packetsCurrentlyOnboard[j]->getPacketContents();
//            QString packetType = packetsCurrentlyOnboard[j]->getPacketContentsType();

//            if (packetType == QString("People")){

//                for (int k = 0; k < packetContents.size(); k++){
//                    if (theOutputLevel == RObject::Maximum) {
//                        qCritical() << "";
//                        qDebug() << this->objectName() << "is now extracting" << packetContents.at(k) << packetType << "from" << packetsCurrentlyOnboard[j]->objectName();
//                    }

//                    currentNumberOfPeople += packetContents.at(k);

//                }
//            }
//        }
//    }






//    // -------------------------------------------------------------------------------------------
//    // 2 - Building damage and direct losses
//    // -------------------------------------------------------------------------------------------
//    // Get hold of the models
//    RSimpleBuildingDamageModel *theDamageModel = qobject_cast<RSimpleBuildingDamageModel *>(theStructuralDamageResponse->getModel());
//    RSimpleBuildingLossModel *theLossModel = qobject_cast<RSimpleBuildingLossModel *>(theDirectEconomicLossResponse->getModel());
//    RSimpleBuildingRepairNeedsModel *theRepairModel = qobject_cast<RSimpleBuildingRepairNeedsModel *>(theRepairTimeResponse->getModel());




////    // Considering only one earthquake in this analysis - think about aftershocks later
////    if (structuralDamageProbabilities.size() < 4){


////        // ***************************************************************
////        // * Building damage
////        // ***************************************************************
////        // Passing the properties of this building to the building model
////        theDamageModel->setLocation(theLocation);
////        theDamageModel->setBuildingType(theBuildingType);
////        theDamageModel->setOccupancyClass(theOccupancyClass);
////        theDamageModel->setCodeDesignLevel(theDesignLevel);
////        theDamageModel->evaluateModel(RGradientType::NONE);


////        // If some loss was calculated in the building damage model
////        if (theStructuralDamageResponse->getCurrentValue() > 0.0 || theNonStructuralDriftDamageResponse->getCurrentValue() > 0.0 || theNonStructuralAccelerationDamageResponse->getCurrentValue() > 0.0){


////            // Passing the properties of this building to the loss model
////            theLossModel->setBuildingType(buildingType.at(i));
////            theLossModel->setOccupancyClass(buildingOccupancyClass.at(i));
////            theLossModel->setCodeDesignLevel(buildingDesignLevel.at(i));
////            theLossModel->evaluateModel(RGradientType::NONE);



////            // Passing the properties of this building to the repair model
////            theRepairModel->setOccupancyClass(buildingOccupancyClass.at(i));
////            theRepairModel->evaluateModel(RGradientType::NONE);

////            // Structural damage
////             structuralDamageProbabilities = theStructuralDamageResponse->getValueVector();

////            // Nonstructural drift related damage
////            nonStructuralDriftDamageProbabilities = (theNDDamageResponse->getValueVector()).at(j);

////            // Nonstructural acceleration related damage
////            nonStructuralAccelerationDamageProbabilities = (theNADamageResponse->getValueVector()).at(j);

////            // Direct economic loss
////            buildingDirectEconomicLoss[i][j] += (theDirectEconomicLossResponse->getValueVector()).at(j) * householdArchetypeCoefficient.at(i) * zoneTotalNumberOfBuildings * buildingFootPrintArea.at(i) * buildingNumberOfStoreys.at(i) * buildingValuePerFoot2.at(i);

////            // Time for repairs
////            buildingTimeNeededForRepairs[i][j] += (theRepairTimeResponse->getValueVector()).at(j);

////            // Time for reoccupation
////            buildingTimeNeededForReoccupation[i][j] += (theReoccupationTimeResponse->getValueVector()).at(j);


////            timeToResumeServices += qMax(theMeanBusinessInterruptionTimeResponse->getCurrentValue(),qMax(waterNetworkExpectedRepairTime,powerNetworkExpectedRepairTime)); // totalDowntime;
////        }
////    }


//    // ***************************************************************
//    // * Check if repairs are progressing
//    // ***************************************************************
////    if (theTimeUnderRepairs < timeNeededForRepairs && theAvailabilityOfPower == true){
////        theTimeUnderRepairs += (double)(theCurrentTime->getCurrentValue() - lastEvaluationTime) * 1.0;
////    }

////    if (timeToResumeServices > theTimeUnderRepairs){
////        qCritical() << this->objectName() << "is not functional!";
////    }




//    // -------------------------------------------------------------------------------------------
//    // 3 - Deploy new beds
//    // -------------------------------------------------------------------------------------------




//    // -------------------------------------------------------------------------------------------
//    // 4 - Check if there are people in need of shelter
//    // -------------------------------------------------------------------------------------------
//    // Get hold of all object in need of supplies
//    QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();
//    QList<RRequest *> theListOfCollectedRequests;


//    for (int i = 0; i < theAllRequestList.size(); i++){


//        // Collecting the requests that need money
//        if(theAllRequestList.at(i)->getContentType() == QString("Shelter")){


//            // Add this request to the list of requested taken
//            theListOfCollectedRequests << theAllRequestList.at(i);
//            // // qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();


//            QVector<double> theRequestedAmounts = theAllRequestList.at(i)->getRequestedAmountList();
//            QVector<QPair<int,int>> theIDs = theAllRequestList.at(i)->getRequesterIDList();
//            QVector<double> theCriteria = theAllRequestList.at(i)->getListOfCriteria();


//            // Add the information from this request to the list of requests picked by this object
//            this->setListOfRequesters(theAllRequestList.at(i)->getModel());
//            this->setListOfRequestedSupplies(theRequestedAmounts, theAllRequestList.at(i)->getContentType(), theCriteria, theIDs);
//        }
//    }


//    // Delete the requests that were picked up by this object
//    int numberOfRequestsCollected = theListOfCollectedRequests.size();
//    for (int i = 0; i < numberOfRequestsCollected; i++){
//        theDomain->deleteObject(theListOfCollectedRequests.at(i)->objectName());
//    }


//    double totalPeopleNeedingShelter = 0.0;
//    for (int i = 0; i < theRequestedAmountList.size(); i++){
//        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
//            totalPeopleNeedingShelter += theRequestedAmountList.at(i).at(j);
//        }
//    }

//    //qCritical() << this->objectName() << "was requested" << totalPeopleNeedingShelter << "beds.";











    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
