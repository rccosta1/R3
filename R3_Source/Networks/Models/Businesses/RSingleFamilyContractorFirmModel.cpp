#include "RSingleFamilyContractorFirmModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RSingleFamilyContractorRequest.h"
#include <algorithm>
#include "RNeighborhoodModel.h"
#include "RNetworkSamplingModel.h"

RSingleFamilyContractorFirmModel::RSingleFamilyContractorFirmModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    packetsCurrentlyOnboard.clear();
}


RSingleFamilyContractorFirmModel::~RSingleFamilyContractorFirmModel()
{
}


int RSingleFamilyContractorFirmModel::resetTime()
{ 
    theTimeOfEvent = 0.0;
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    theTimeOfFirstRequest = 0.0;
    theTimeSinceLastUpdate = 0.0;
    numberOfNewPacketsCreatedHere = 0;
    theRemainingAccomodationCapacity = 0;
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();
    theBuildingOccupancy.clear();
    thePacketBid.clear();
    theBuildingAvailabilityOfFunds.clear();
    shouldGetWorkers.clear();
    theAllocatedWorkersVector.clear();
    theAvailableWorkersVector.clear();
    theTotalDemandForWorkersVector.clear();
    theNumberOfDwellingsWaitingVector.clear();
    theRemainingAccomodationCapacityVector.clear();

    // Reset the number of workers available
    theNumberOfSingleFamilyCrews = theAvailableWorkersPreDisaster;
    theNumberOfSingleFamilyWorkersWaitingToBeAllocated = theAvailableWorkersPreDisaster;
    theNumberOfSingleFamilyWorkersCurrentlyAllocated = 0;

    return 0;
}


int RSingleFamilyContractorFirmModel::getLocalCrews()
{
    return theAvailableWorkersPreDisaster;
}

void RSingleFamilyContractorFirmModel::setLocalCrews(int value)
{
    theAvailableWorkersPreDisaster = value;
}


QString RSingleFamilyContractorFirmModel::getBehavior()
{
    return theBehavior;
}

void RSingleFamilyContractorFirmModel::setBehavior(QString value)
{
    theBehavior = value;
}

double RSingleFamilyContractorFirmModel::getDemandRatio()
{
    return theDemandRatio;//should be between 0 and 1
}

void RSingleFamilyContractorFirmModel::setDemandRatio(double value)
{
    theDemandRatio = value;
}

double RSingleFamilyContractorFirmModel::getNewAccommodations()
{
    return theInterventionApproachID;
}

void RSingleFamilyContractorFirmModel::setNewAccommodations(double value)
{
    theInterventionApproachID = value;
}


void RSingleFamilyContractorFirmModel::dismissTeamOfWorkers(QString theOccupancy, int value)
{
    if (theOccupancy == QString("RES1")){
        theNumberOfSingleFamilyWorkersWaitingToBeAllocated += value;
        theNumberOfSingleFamilyWorkersCurrentlyAllocated -= value;
    }
}



void RSingleFamilyContractorFirmModel::getRequests()
{

}


int RSingleFamilyContractorFirmModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    if (lastEvaluationTime <= 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    if (myPossiblePaths.size() == 0){
        getPossiblePaths();
    }

    // Get the time of event as reference
    if(theTimeOfEvent == 0.0){
        theTimeOfEvent = theCurrentTime->getCurrentValue();
    }

    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    //this->collectRequests(QString("Allocation"));
    QList<RSingleFamilyContractorRequest *> theAllRequestList = theDomain->findChildren<RSingleFamilyContractorRequest *>();
    QList<RRequest *> theListOfCollectedRequests;

    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests for contractors
        if(theAllRequestList.at(i)->getContentType().contains("Allocation") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

            // set the time of first request
            if(theTimeOfFirstRequest == 0.0){
                theTimeOfFirstRequest = theCurrentTime->getCurrentValue();
            }

            // Add this request to the list of requested taken
            theListOfCollectedRequests << theAllRequestList.at(i);

            //qDebug() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName() << thePacketDestinationList.size();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
            theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            theBuildingOccupancy << theAllRequestList.at(i)->getOccupancyClasses();
            theBuildingAvailabilityOfFunds << theAllRequestList.at(i)->getBid();
        }
    }


    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // Next, get hold of what each requester needs
    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);
        thePacketBid << theBuildingAvailabilityOfFunds.at(i);

        // For each request received
        // Creating a list of priorities. Setting all to false to begin with
        QVector<bool> auxWorkers;
        auxWorkers.resize(theRequestedAmountList.at(i).size());
        auxWorkers.fill(false);
        shouldGetWorkers << auxWorkers;
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theNeighborhoodIncome.clear();
    theBuildingAvailabilityOfFunds.clear();

    // Delta t
    theTimeSinceLastUpdate += theCurrentTime->getCurrentValue() - lastEvaluationTime;

    // calculate the number of dwellings waiting to be repaired
    double theNumberOfDwellingsWaiting = 0.0;
    for (int i = 0; i < thePacketDestinationList.size(); i++){
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){
            theNumberOfDwellingsWaiting += thePacketAmountList.at(i).at(j);

        }
    }

    // -------------------Accommodation Intervention------------------------------------------
    // Try different approaches to increase the accomodation capacity thereby helping recovery
    //----------------------------------------------------------------------------------------
    // provides no accomodation at all (no new workers)
    if (theInterventionApproachID == 0){
        theRemainingAccomodationCapacity = 0;

    }  // Skip the need for accomodation
    else if (theInterventionApproachID > 1){
        theRemainingAccomodationCapacity = 9999999999;

    } // Otherwise use a fraction of the number of dwellings repaired to house out-of-town workers
    else if(theInterventionApproachID > 0.0 && theInterventionApproachID <= 1.0){
        int theAvailableTemporaryHousing = 0;

        // Get hold of all neighborhoods
        QList<RNeighborhoodModel*> theListOfNeighborhoods = theDomain->findChildren<RNeighborhoodModel*>();
        for (int i = 0; i < theListOfNeighborhoods.size(); i++){
            theAvailableTemporaryHousing += theInterventionApproachID * theListOfNeighborhoods.at(i)->getNumberOfRepairedDwellings();
        }
        theRemainingAccomodationCapacity += qFloor(theAvailableTemporaryHousing/3.0);
    }

    // if demand*ratio > supply, then bring in more workers
    if (theDemandRatio * (theNumberOfDwellingsWaiting + theNumberOfSingleFamilyWorkersCurrentlyAllocated) > (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated)){

        double theGapBetweenSupplyAndDemand = theDemandRatio * (theNumberOfDwellingsWaiting + theNumberOfSingleFamilyWorkersCurrentlyAllocated) - (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated);
        // if there are enough rooms for out-of-town workers, bring them all
        if (theRemainingAccomodationCapacity > theGapBetweenSupplyAndDemand){
            theRemainingAccomodationCapacity -= theGapBetweenSupplyAndDemand;
            theNumberOfSingleFamilyWorkersWaitingToBeAllocated += theGapBetweenSupplyAndDemand;
        } else{
            // only bring in what can be accomodated
            theNumberOfSingleFamilyWorkersWaitingToBeAllocated += theRemainingAccomodationCapacity;
            theRemainingAccomodationCapacity = 0.0;
        }

        // else if demand*ratio < supply, then some out-of-town workers might leave
    } else if(theDemandRatio * (theNumberOfDwellingsWaiting + theNumberOfSingleFamilyWorkersCurrentlyAllocated) < (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated)) {
        double theGapBetweenSupplyAndDemand = (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated) - theDemandRatio * (theNumberOfDwellingsWaiting + theNumberOfSingleFamilyWorkersCurrentlyAllocated);
        // assume a fraction of theGapBetweenSupplyAndDemand number of out-of-town workers will leave when the target is met
        theSurplusWorkers = 0.5;
        // check if the supply is still larger than baseline supply if a fraction of workers are to leave
        if ((theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated) - theSurplusWorkers * theGapBetweenSupplyAndDemand >  theAvailableWorkersPreDisaster){
            theRemainingAccomodationCapacity += theSurplusWorkers * theGapBetweenSupplyAndDemand;
            theNumberOfSingleFamilyWorkersWaitingToBeAllocated -= theSurplusWorkers * theGapBetweenSupplyAndDemand;
            // the supply cannot go below the baseline
        } else{
            theRemainingAccomodationCapacity += (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated) - theAvailableWorkersPreDisaster;
            theNumberOfSingleFamilyWorkersWaitingToBeAllocated -= (theNumberOfSingleFamilyWorkersWaitingToBeAllocated + theNumberOfSingleFamilyWorkersCurrentlyAllocated) - theAvailableWorkersPreDisaster;
        }
    } else {
        //qCritical()<<"The supply target is met!";
    }

    theAllocatedWorkersVector << theNumberOfSingleFamilyWorkersCurrentlyAllocated;
    theAvailableWorkersVector << theNumberOfSingleFamilyWorkersWaitingToBeAllocated;
    theTotalDemandForWorkersVector << theNumberOfDwellingsWaiting + theNumberOfSingleFamilyWorkersCurrentlyAllocated;
    theNumberOfDwellingsWaitingVector << theNumberOfDwellingsWaiting;
    theRemainingAccomodationCapacityVector << theRemainingAccomodationCapacity;


    // Now I need to define which building will have an contractor allocated to them
    QVector<double> theCriteriaList;
    QVector<QPair<int,int>> thePriorityList;
    for (int i = 0; i < thePacketDestinationList.size(); i++){
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            // These temporary vectors save the I and J of each building.
            // I use this temporary lists because I want to be able to remove items from them without messing up with the analysis.
            // I is the neighborhood to which the building belongs.
            // J is the number of the building within that neighborhood.
            // The packet bid is how much the building is willing to pay for an engineer.
            thePriorityList << qMakePair(i,j);
            theCriteriaList << thePacketBid.at(i).at(j);
        }
    }



    // QVector<QPair<RModel*,int>> auxPriorityList = thePriorityList;
    int nTries = theAvailableWorkersPreDisaster;

    if (thePacketDestinationList.size() > 0){
        if (theBehavior == QString("Greedy")){

            // I am doing this for single-family only here. Will have to account for multi-family eventually.
            while (theNumberOfSingleFamilyWorkersWaitingToBeAllocated > 0 && theCriteriaList.size() > 0){

                // Figure out how much is the highest offer for engineering services
                double theHighestBid = *std::max_element(theCriteriaList.constBegin(), theCriteriaList.constEnd());
                //                qCritical() << "The highest bidder is a building with 1/DS equal to: "<< theCriteriaList.indexOf(theHighestBid);

                //  Figure out which building is making that offer
                QPair<int,int> theHighestPriorityObject = thePriorityList.at(theCriteriaList.indexOf(theHighestBid));

                // Get the I and J of that building
                int theHighestPriorityI = theHighestPriorityObject.first;
                int theHighestPriorityJ = theHighestPriorityObject.second;

                // Now, check if you have enough contractors to work on this building
                if (theNumberOfSingleFamilyWorkersWaitingToBeAllocated >= thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ) && theBuildingOccupancy.at(theHighestPriorityI).at(theHighestPriorityJ) == QString("RES1") && thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ) > 0){
                    // If so, allocate an engineer to that building - This vector will be important later!!
                    shouldGetWorkers[theHighestPriorityI][theHighestPriorityJ] = true;

                    //qCritical() <<"Building" << this->objectName() << " is allocated engineers at time " << theCurrentTime->getTime();
                    //qCritical() << "The highest bid for contractors is:" << theHighestBid << theNumberOfSingleFamilyWorkersWaitingToBeAllocated << " ";

                    // If contractors are allocated, remove the amount of contractors allocated from the pool of contractors available
                    theNumberOfSingleFamilyWorkersWaitingToBeAllocated -= thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ);
                    theNumberOfSingleFamilyWorkersCurrentlyAllocated += thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ);
                }

                // Since you provided this building with an engineer, remove it from the list of bidders
                // This is the advantage of having saved these values in these temporary lists
                // I can delete them without losing any important information (i.e., I am not deleting things from the class variables)
                // Note: I removing this building from the list even if not engineer was allocated to it (i.e., I am outside of the IF conditional above)
                // This is because the IF statement will fail if there are not enough contractors to allocate to this building
                // However, another building needing fewer contractors may still have contractors allocated to it
                // So I will remove this from the list, even though it has the highest priority, because at this time step I cannot supply what they need
                thePriorityList.removeAt(theCriteriaList.indexOf(theHighestBid));
                theCriteriaList.removeAt(theCriteriaList.indexOf(theHighestBid));
            }

            // Behavior == greedy
        } else if (theBehavior == QString("FCFS")){

            // Distribute resources to whoever requested first
            for (int i = 0; i < thePacketDestinationList.size(); i++){
                for (int j = 0; j < thePacketAmountList.at(i).size(); j++){
                    if (theNumberOfSingleFamilyWorkersWaitingToBeAllocated >= thePacketAmountList.at(i).at(j) && theBuildingOccupancy.at(i).at(j) == QString("RES1") && thePacketAmountList.at(i).at(j) > 0){
                        shouldGetWorkers[i][j] = true;
                        theNumberOfSingleFamilyWorkersWaitingToBeAllocated -= thePacketAmountList.at(i).at(j);
                        theNumberOfSingleFamilyWorkersCurrentlyAllocated += thePacketAmountList.at(i).at(j);
                    }
                }
            }

            // Unbiased
        } else if (theBehavior == QString("Unbiased")){

            while (theNumberOfSingleFamilyWorkersWaitingToBeAllocated > 0 && nTries > 0){
                nTries -= 1;
                int maxI = thePacketDestinationList.size() - 1;
                int selectedI = qFloor(maxI * theRNG.generateDouble());

                int maxJ = thePacketAmountList.at(selectedI).size() - 1;
                int selectedJ = qFloor(maxJ * theRNG.generateDouble());

                if (shouldGetWorkers[selectedI][selectedJ] == false && theNumberOfSingleFamilyWorkersWaitingToBeAllocated >= thePacketAmountList.at(selectedI).at(selectedJ) && theBuildingOccupancy.at(selectedI).at(selectedJ) == QString("RES1") && thePacketAmountList.at(selectedI).at(selectedJ) > 0){
                    shouldGetWorkers[selectedI][selectedJ] = true;
                    theNumberOfSingleFamilyWorkersWaitingToBeAllocated -= thePacketAmountList.at(selectedI).at(selectedJ);
                    theNumberOfSingleFamilyWorkersCurrentlyAllocated += thePacketAmountList.at(selectedI).at(selectedJ);
                }

            } // while
        } else {
            qCritical() << this->objectName() << "Has an unknown behavior named" << theBehavior <<" and is not going to distributed resources!!";
        }
    }


    // -------------------------------------------------------------------------------------------
    // 3 - Provide supplies
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object requesting
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        // Making sure packets are only created when there is something to be delivered
        bool createPacket = false;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            //if (selectedToReceiveSupplies.at(i).at(j) == true){
            if (shouldGetWorkers.at(i).at(j) == true){
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

                if (shouldGetWorkers.at(i).at(j) == true && thePacketAmountList.at(i).at(j) > 0){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                    thePacketAmountList[i][j] -= thePacketContents;
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
            } // for j

            packetsCurrentlyOnboard << theNewPacket;
        } // if create
    } // for i


    if (packetsCurrentlyOnboard.size() > 0){
        // Unload packets into the packet response
        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
    }


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
        theBuildingOccupancy.removeAt(indexesToRemove.at(i));
        thePacketBid.removeAt(indexesToRemove.at(i));
        shouldGetWorkers.removeAt(indexesToRemove.at(i));
    }


    theResponse->setCurrentValue(theCurrentHourlyWages);
    lastEvaluationTime = theCurrentTime->getCurrentValue();
    return 1;
}

