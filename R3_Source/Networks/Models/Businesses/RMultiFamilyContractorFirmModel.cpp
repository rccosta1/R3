#include "RMultiFamilyContractorFirmModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RMultiFamilyContractorRequest.h"
#include <algorithm>
#include "RMainNeighborhoodModel.h"


RMultiFamilyContractorFirmModel::RMultiFamilyContractorFirmModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    packetsCurrentlyOnboard.clear();

}


RMultiFamilyContractorFirmModel::~RMultiFamilyContractorFirmModel()
{

}


int RMultiFamilyContractorFirmModel::resetTime()
{ 
    timeOfEvent = 0.0;
    theTimeOfFirstRequest = 0.0;
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketBid.clear();
    thePacketIDList.clear();
    theBuildingOccupancy.clear();
    theBuildingAvailabilityOfFunds.clear();
    shouldGetWorkers.clear();


    numberOfNewPacketsCreatedHere = 0;
    theCurrentHourlyWages = theBaseHourlyWages;


    // Define the random number of workers available in this scenario - remove this after paper
    theNumberOfMultiFamilyCrews = theBaselineNumberOfMultiFamilyCrews;// + samplingMultiplier * theBaselineNumberOfMultiFamilyCrews;

    //double rngMF = rand()/static_cast<double>(RAND_MAX);
    //theNumberOfMultiFamilyCrews = theBaselineNumberOfMultiFamilyCrews + 2.0*rngMF * theBaselineNumberOfMultiFamilyCrews;

    theNumberOfMultiFamilyCrewsAvailable = theBaselineNumberOfMultiFamilyCrews;

    //samplingMultiplier += 0.2;

    return 0;
}



int RMultiFamilyContractorFirmModel::getBaselineNumberOfMultiFamilyCrews()
{
    return theBaselineNumberOfMultiFamilyCrews;
}

void RMultiFamilyContractorFirmModel::setBaselineNumberOfMultiFamilyCrews(int value)
{
    theBaselineNumberOfMultiFamilyCrews = value;
}


double RMultiFamilyContractorFirmModel::getBaseHourlyWages()
{
    return theBaseHourlyWages;
}

void RMultiFamilyContractorFirmModel::setBaseHourlyWages(double value)
{
    theBaseHourlyWages = value;
}

QString RMultiFamilyContractorFirmModel::getBehavior()
{
    return theBehavior;
}

void RMultiFamilyContractorFirmModel::setBehavior(QString value)
{
    theBehavior = value;
}


void RMultiFamilyContractorFirmModel::dismissTeamOfWorkers(QString theOccupancy, int value)
{
    if (theOccupancy.contains(QString("RES3"))){
        theNumberOfMultiFamilyCrewsAvailable += value;
        //qCritical() << this->objectName() << " ======================================>> Received workers" << value << " back";
    }
}

void RMultiFamilyContractorFirmModel::setNumberOfWorkersAvailable(int theNumberOfDwellingsNeededRepair,int theTotalNumberOfDwellings)
{
    theNumberOfMultiFamilyCrewsAvailable = static_cast<int>((static_cast<double>(theNumberOfDwellingsNeededRepair)/static_cast<double>(theTotalNumberOfDwellings))*theBaselineNumberOfMultiFamilyCrews);
}

void RMultiFamilyContractorFirmModel::getRequests()
{

}

int RMultiFamilyContractorFirmModel::getNumberOfMultiFamilyCrews()
{
    return theNumberOfMultiFamilyCrews;
}


int RMultiFamilyContractorFirmModel::evaluateModel()
{

    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    int timeSinceFirstRequest = 0;
    if(theTimeOfFirstRequest != 0.0){
        timeSinceFirstRequest = static_cast<int>(theCurrentTime->getCurrentValue() - theTimeOfFirstRequest);
    }

    if (timeSinceFirstRequest%30 ==0 && timeSinceFirstRequest != 0){//evaluate every 30 days

        qCritical()<< "Time since first request: " << timeSinceFirstRequest;
        bool WorkersProportionalToHousing = true;
        if (WorkersProportionalToHousing == true){
            qCritical()<< "The number of workers are set proportional to the number of dwellings in need of repair!";
            int theNumberOfTotalDwellingsNeedRepair = 0;
            int theNumberOfTotalDwellings = 0;

            // loop through the list of neighborhoods to find the number of dwellings that still need repair
            QList<RMainNeighborhoodModel*> theListOfNeighborhoods = theDomain->findChildren<RMainNeighborhoodModel*>();
            for (int i = 0; i < theListOfNeighborhoods.size(); i++){
               theNumberOfTotalDwellingsNeedRepair += theListOfNeighborhoods.at(i)->getNumberOfDwellings() - theListOfNeighborhoods.at(i)->getNumberOfHabitableDwellings();
               theNumberOfTotalDwellings += theListOfNeighborhoods.at(i)->getNumberOfDwellings();
            }

            qCritical()<<"Later modify this to multi family dwellings only:";
            qCritical()<<"the Number Of Total Dwellings Need Repair: "<<theNumberOfTotalDwellingsNeedRepair;
            qCritical()<<"the Number Of Total Dwellings: "<<theNumberOfTotalDwellings;
            qCritical()<<"the Baseline Number Of Single Family Crews"<<theBaselineNumberOfMultiFamilyCrews;

            double theMinimumRatioOfDemandSatisfied = 0.20;//variable of interest for this sensitivity analysis
            if (theMinimumRatioOfDemandSatisfied*static_cast<double>(theNumberOfTotalDwellingsNeedRepair) > static_cast<double>(theNumberOfMultiFamilyCrewsAvailable)){
                theNumberOfMultiFamilyCrewsAvailable += static_cast<int>(theMinimumRatioOfDemandSatisfied*static_cast<double>(theNumberOfTotalDwellingsNeedRepair) - static_cast<double>(theNumberOfMultiFamilyCrewsAvailable));
                qCritical()<<"The number of single-family workers is increased by: "<<theNumberOfMultiFamilyCrewsAvailable << " at " <<theCurrentTime->getTime();
            }
        }
    }


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    if (myPossiblePaths.size() == 0){
        getPossiblePaths();
    }



    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    //this->collectRequests(QString("Allocation"));
    QList<RMultiFamilyContractorRequest *> theAllRequestList = theDomain->findChildren<RMultiFamilyContractorRequest *>();
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

            // qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName() << thePacketDestinationList.size();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
            theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            theBuildingOccupancy << theAllRequestList.at(i)->getOccupancyClasses();
            theBuildingAvailabilityOfFunds << theAllRequestList.at(i)->getBid();
            //}
        }
    }



    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // Next, get hold of what each requester needs
    QVector<double> auxFunds;

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

    int nTries = theBaselineNumberOfMultiFamilyCrews;

    if (thePacketDestinationList.size() > 0){
        if (theBehavior == QString("Greedy")){

            // I am doing this for single-family only here. Will have to account for multi-family eventually.
            while (theNumberOfMultiFamilyCrewsAvailable > 0 && theCriteriaList.size() > 0){

                // Figure out how much is the highest offer for engineering services
                double theHighestBid = *std::max_element(theCriteriaList.constBegin(), theCriteriaList.constEnd());

                //  Figure out which building is making that offer
                QPair<int,int> theHighestPriorityObject = thePriorityList.at(theCriteriaList.indexOf(theHighestBid));

                // Get the I and J of that building
                int theHighestPriorityI = theHighestPriorityObject.first;
                int theHighestPriorityJ = theHighestPriorityObject.second;

                // Now, check if you have enough contractors to work on this building
                if (theNumberOfMultiFamilyCrewsAvailable >= thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ) && theBuildingOccupancy.at(theHighestPriorityI).at(theHighestPriorityJ).contains(QString("RES3")) && thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ) > 0){
                    // If so, allocate an engineer to that building - This vector will be important later!!
                    shouldGetWorkers[theHighestPriorityI][theHighestPriorityJ] = true;
                    //qCritical() << "The highest bid for contractors is:" << theHighestBid << theNumberOfMultiFamilyCrewsAvailable << " ";

                    // If contractors are allocated, remove the amount of contractors allocated from the pool of contractors available
                    theNumberOfMultiFamilyCrewsAvailable -= thePacketAmountList.at(theHighestPriorityI).at(theHighestPriorityJ);
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
                    if (theNumberOfMultiFamilyCrewsAvailable >= thePacketAmountList.at(i).at(j) && theBuildingOccupancy.at(i).at(j).contains(QString("RES3")) && thePacketAmountList.at(i).at(j) > 0){
                        shouldGetWorkers[i][j] = true;
                        theNumberOfMultiFamilyCrewsAvailable -= thePacketAmountList.at(i).at(j);
                    }
                }
            }

            // Unbiased
        } else if (theBehavior == QString("Unbiased")){

            while (theNumberOfMultiFamilyCrewsAvailable > 0 && nTries > 0){
                nTries -= 1;
                int maxI = thePacketDestinationList.size() - 1;
                int selectedI = qFloor(maxI * rand()/static_cast<double>(RAND_MAX));

                int maxJ = thePacketAmountList.at(selectedI).size() - 1;
                int selectedJ = qFloor(maxJ * rand()/static_cast<double>(RAND_MAX));

                if (shouldGetWorkers[selectedI][selectedJ] == false && theNumberOfMultiFamilyCrewsAvailable >= thePacketAmountList.at(selectedI).at(selectedJ) && theBuildingOccupancy.at(selectedI).at(selectedJ).contains(QString("RES3")) && thePacketAmountList.at(selectedI).at(selectedJ) > 0){
                    shouldGetWorkers[selectedI][selectedJ] = true;
                    theNumberOfMultiFamilyCrewsAvailable -= thePacketAmountList.at(selectedI).at(selectedJ);
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



    return 1;
}

