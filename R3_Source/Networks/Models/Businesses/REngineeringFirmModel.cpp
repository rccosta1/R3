#include "REngineeringFirmModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "REngineeringAssessmentRequest.h"
#include <algorithm>


REngineeringFirmModel::REngineeringFirmModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


REngineeringFirmModel::~REngineeringFirmModel()
{

}


int REngineeringFirmModel::resetTime()
{
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    numberOfNewPacketsCreatedHere = 0;

    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketIDList.clear();
    thePacketSupplyTypeList.clear();
    thePacketProcessingTimeList.clear();
    theBuildingRepairClass.clear();
    theBuildingOccupancyClass.clear();
    theBuildingAvailabilityOfFunds.clear();
    theTimeUnderProcessing.clear();
    thePriorityList.clear();

    // Define the random number of workers available in this scenario - remove this after paper
    theNumberOfSingleFamilyCrews = theBaselineNumberOfSingleFamilyCrews;// + samplingMultiplier * theBaselineNumberOfSingleFamilyCrews;
    theNumberOfMultiFamilyCrews = theBaselineNumberOfMultiFamilyCrews;// + samplingMultiplier * theBaselineNumberOfMultiFamilyCrews;

    double rngSF = rand()/static_cast<double>(RAND_MAX);
    double rngMF = rand()/static_cast<double>(RAND_MAX);
//    theNumberOfSingleFamilyCrews = theBaselineNumberOfSingleFamilyCrews + 2.0*rngSF * theBaselineNumberOfSingleFamilyCrews;
//    theNumberOfMultiFamilyCrews = theBaselineNumberOfMultiFamilyCrews + 2.0*rngMF * theBaselineNumberOfMultiFamilyCrews;

    samplingMultiplier += 0.1;

    return 0;
}



int REngineeringFirmModel::getBaselineNumberOfSingleFamilyCrews()
{
    return theBaselineNumberOfSingleFamilyCrews;
}

void REngineeringFirmModel::setBaselineNumberOfSingleFamilyCrews(int value)
{
    theBaselineNumberOfSingleFamilyCrews = value;
}


int REngineeringFirmModel::getBaselineNumberOfMultiFamilyCrews()
{
    return theBaselineNumberOfMultiFamilyCrews;
}

void REngineeringFirmModel::setBaselineNumberOfMultiFamilyCrews(int value)
{
    theBaselineNumberOfMultiFamilyCrews = value;
}


void REngineeringFirmModel::getRequests()
{

}


int REngineeringFirmModel::getNumberOfSingleFamilyCrews()
{
    return theNumberOfSingleFamilyCrews;
}

int REngineeringFirmModel::getNumberOfMultiFamilyCrews()
{
    return theNumberOfMultiFamilyCrews;
}


int REngineeringFirmModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    if (myPossiblePaths.size() == 0){
        getPossiblePaths();
    }

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    //this->collectRequests(QString("Assessment"));

    QList<REngineeringAssessmentRequest *> theAllRequestList = theDomain->findChildren<REngineeringAssessmentRequest *>();
    QList<RRequest *> theListOfCollectedRequests;
    QVector<QVector<QString>> auxOccupancyClass;
    QVector<QVector<double>> theDelaysForEachBuilding;


    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests
        if(theAllRequestList.at(i)->getContentType().contains("Assessment") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

            // Add this request to the list of requested taken
            theListOfCollectedRequests << theAllRequestList.at(i);

            //qDebug() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName() << theAllRequestList.at(i)->getRequestedAmountList();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
            theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            theBuildingRepairClass << theAllRequestList.at(i)->getRepairClasses();
            auxOccupancyClass << theAllRequestList.at(i)->getOccupancyClasses();
            theBuildingAvailabilityOfFunds << theAllRequestList.at(i)->getFundsAvailability();
            theDelaysForEachBuilding << theAllRequestList.at(i)->getDelayInTakingAction();
        }
    }


    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // Next, get hold of what each requester needs
    QVector<double> auxFunds;
    QVector<RModel*> unsortedNeighborhoodPriorityList;
    QVector<int> unsortedBuildingPriorityList;

    for (int i = 0; i < theRequestDestinationList.size(); i++){

        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);

        QVector<double> auxVector;
        auxVector.resize(theRequestedAmountList.at(i).size());
        auxVector.fill(0.0);
        theTimeUnderProcessing << auxVector; // just populating this vector

        // For each request received
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

            double medianOfDelay = 0.0;
            double dispersionOfDelay = 0.0;
            if (theBuildingRepairClass.at(i).at(j) == 1){
                medianOfDelay = 2.0; //6
                dispersionOfDelay = 0.32; //0.40
            } else if (theBuildingRepairClass.at(i).at(j) == 3){
                medianOfDelay = 4.0; // 12
                dispersionOfDelay = 0.54; // 0.40
            } else if (theBuildingRepairClass.at(i).at(j) == 4){
                medianOfDelay = 42.0; //50
                dispersionOfDelay = 0.45; // 0.32
            } else {
                qCritical() << "Unindentified Repair Class at" << this->objectName();
            }

            double m = medianOfDelay;
            double v = dispersionOfDelay * dispersionOfDelay;
            double phi = qSqrt(v + m * m);
            double mu = qLn(m * m / phi);
            double sigma = qSqrt(qLn((phi*phi) / (m*m)));

            double theUx = rand()/static_cast<double>(RAND_MAX);

            if (auxOccupancyClass.at(i).at(j).contains(QString("RES3"))){
                theDelaysForEachBuilding[i][j] +=  7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
            } else {
                theDelaysForEachBuilding[i][j] +=  7.0;
            }


            auxFunds << theBuildingAvailabilityOfFunds.at(i).at(j);
            unsortedNeighborhoodPriorityList << theRequestDestinationList.at(i);
            unsortedBuildingPriorityList << theRequesterIDList.at(i).at(j);
        }

        theBuildingOccupancyClass << auxOccupancyClass.at(i);
        thePacketProcessingTimeList << theDelaysForEachBuilding.at(i);
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theBuildingRepairClass.clear();
    theBuildingAvailabilityOfFunds.clear();


    int theNumberOfSingleFamilyCrewsAvailable = theNumberOfSingleFamilyCrews;
    int theNumberOfMultiFamilyCrewsAvailable = theNumberOfMultiFamilyCrews;

    for (int i = 0; i < thePacketDestinationList.size(); i++){
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

             if (theBuildingOccupancyClass.at(i).at(j).contains(QString("RES3")) && theNumberOfMultiFamilyCrewsAvailable >= thePacketAmountList.at(i).at(j)){
                 theTimeUnderProcessing[i][j] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
                 theNumberOfMultiFamilyCrewsAvailable -= thePacketAmountList.at(i).at(j);

             } else if (theBuildingOccupancyClass.at(i).at(j) == QString("RES1") && theNumberOfSingleFamilyCrewsAvailable >= thePacketAmountList.at(i).at(j)){
                 theTimeUnderProcessing[i][j] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
                 theNumberOfSingleFamilyCrewsAvailable -= thePacketAmountList.at(i).at(j);
             }
        }
    }


    // -------------------------------------------------------------------------------------------
    // Provide supplies
    // -------------------------------------------------------------------------------------------
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        // Making sure packets are only created when there is something to be delivered
        bool createPacket = false;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            if (theTimeUnderProcessing.at(i).at(j) > thePacketProcessingTimeList.at(i).at(j) && thePacketAmountList.at(i).at(j) > 0.0){
                createPacket = true;
            }
        }


        if (createPacket == true){

            // Creating a packet
            QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
            theDomain->createObject(packetName, "RPacket");
            RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
            this->getShortestPath(thePacketDestinationList.at(i));
            theNewPacket->setPacketContentsType(thePacketSupplyTypeList.at(i));
            theNewPacket->setDestinationList(thePacketDestinationList.at(i));


            for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

                double thePacketContents = 0.0;
                if (theTimeUnderProcessing.at(i).at(j) > thePacketProcessingTimeList.at(i).at(j) && thePacketAmountList.at(i).at(j) > 0){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                //qCritical() << "Packet for" << thePacketDestinationList.at(i) << theBuildingOccupancyClass.at(i).at(j) << thePacketIDList.at(i).at(j) << "with" << thePacketContents;

                thePacketAmountList[i][j] -= thePacketContents;
            } // for j

            packetsCurrentlyOnboard << theNewPacket;
        } // if create
    } // for i



    if (packetsCurrentlyOnboard.size() > 0){
        // Unload packets into the packet response
        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
    }


    // Remove the claims that were already paid from the list of claims to be paid
    QVector<int> indexesToRemove;
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        int ii = thePacketDestinationList.size() - (i + 1);

        double sum = 0.0;
        for (int j = 0; j < thePacketAmountList.at(ii).size(); j++){
            sum += thePacketAmountList.at(ii).at(j);
        } // for j

        if (sum < 0.01){
            indexesToRemove << ii;
        }
    } // for i



    for (int i = 0; i < indexesToRemove.size(); i++){
        thePacketDestinationList.removeAt(indexesToRemove.at(i));
        thePacketAmountList.removeAt(indexesToRemove.at(i));
        thePacketIDList.removeAt(indexesToRemove.at(i));
        thePacketSupplyTypeList.removeAt(indexesToRemove.at(i));
        thePacketProcessingTimeList.removeAt(indexesToRemove.at(i));
        theTimeUnderProcessing.removeAt(indexesToRemove.at(i));
        theBuildingOccupancyClass.removeAt(indexesToRemove.at(i));
    }


    lastEvaluationTime = theCurrentTime->getCurrentValue();
    return 1;
}


//while (auxFunds.size() > 0){
//    double highestPriorityObject = *std::max_element(auxFunds.constBegin(), auxFunds.constEnd());
//    thePriorityList << qMakePair(unsortedNeighborhoodPriorityList.at(auxFunds.indexOf(highestPriorityObject)),unsortedBuildingPriorityList.at(auxFunds.indexOf(highestPriorityObject)));

//    unsortedNeighborhoodPriorityList.removeAt(auxFunds.indexOf(highestPriorityObject));
//    unsortedBuildingPriorityList.removeAt(auxFunds.indexOf(highestPriorityObject));
//    auxFunds.removeAt(auxFunds.indexOf(highestPriorityObject));
//}



//for (int i = 0; i < thePriorityList.size(); i++){

//    int highestPriorityI = thePacketDestinationList.indexOf(thePriorityList.at(i).first);
//    int highestPriorityJ = thePacketIDList.at(highestPriorityI).indexOf(thePriorityList.at(i).second);


//    // On every time step allocate engineers to 'work' on the projects that still need design
//    if (theBuildingOccupancyClass.at(highestPriorityI).at(highestPriorityJ) == QString("RES3") && theNumberOfMultiFamilyCrewsAvailable >= thePacketAmountList.at(highestPriorityI).at(highestPriorityJ)){
//        theTimeUnderProcessing[highestPriorityI][highestPriorityJ] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
//        theNumberOfMultiFamilyCrewsAvailable -= thePacketAmountList.at(highestPriorityI).at(highestPriorityJ);
//        //qCritical() << this->objectName() << theBuildingOccupancyClass.at(highestPriorityI).at(highestPriorityJ) << theTimeUnderProcessing[highestPriorityI][highestPriorityJ] <<thePacketProcessingTimeList[highestPriorityI][highestPriorityJ] ;

//    } else if (theBuildingOccupancyClass.at(highestPriorityI).at(highestPriorityJ) != QString("RES3") && theNumberOfSingleFamilyCrewsAvailable >= thePacketAmountList.at(highestPriorityI).at(highestPriorityJ)){
//        theTimeUnderProcessing[highestPriorityI][highestPriorityJ] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
//        theNumberOfSingleFamilyCrewsAvailable -= thePacketAmountList.at(highestPriorityI).at(highestPriorityJ);
//        qCritical() << this->objectName() << theBuildingOccupancyClass.at(highestPriorityI).at(highestPriorityJ) << theTimeUnderProcessing[highestPriorityI][highestPriorityJ] << thePacketProcessingTimeList[highestPriorityI][highestPriorityJ] ;
//    }
//}






