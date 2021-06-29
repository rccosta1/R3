#include "RSingleFamilyContractorMobilizationModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RSingleFamilyContractorRequest.h"
#include <algorithm>


RSingleFamilyContractorMobilizationModel::RSingleFamilyContractorMobilizationModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    packetsCurrentlyOnboard.clear();

}


RSingleFamilyContractorMobilizationModel::~RSingleFamilyContractorMobilizationModel()
{

}


int RSingleFamilyContractorMobilizationModel::resetTime()
{ 
    timeOfEvent = 0.0;
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();
    theBuildingOccupancy.clear();
    thePriorityList.clear();


    numberOfNewPacketsCreatedHere = 0;
    theCurrentHourlyWages = theBaseHourlyWages;


    // Define the random number of workers available in this scenario - remove this after paper
    theNumberOfSingleFamilyCrews = theBaselineNumberOfSingleFamilyCrews;// + samplingMultiplier * theBaselineNumberOfSingleFamilyCrews;

    double rngSF = rand()/static_cast<double>(RAND_MAX);
    //theNumberOfSingleFamilyCrews = theBaselineNumberOfSingleFamilyCrews + 2.0*rngSF * theBaselineNumberOfSingleFamilyCrews;

    theNumberOfSingleFamilyCrewsAvailable = theBaselineNumberOfSingleFamilyCrews;

    samplingMultiplier += 0.2;

    return 0;
}


int RSingleFamilyContractorMobilizationModel::getBaselineNumberOfSingleFamilyCrews()
{
    return theBaselineNumberOfSingleFamilyCrews;
}

void RSingleFamilyContractorMobilizationModel::setBaselineNumberOfSingleFamilyCrews(int value)
{
    theBaselineNumberOfSingleFamilyCrews = value;
}


double RSingleFamilyContractorMobilizationModel::getBaseHourlyWages()
{
    return theBaseHourlyWages;
}

void RSingleFamilyContractorMobilizationModel::setBaseHourlyWages(double value)
{
    theBaseHourlyWages = value;
}


void RSingleFamilyContractorMobilizationModel::dismissTeamOfWorkers(QString theOccupancy, int value)
{
    if (theOccupancy == QString("RES1")){
        theNumberOfSingleFamilyCrewsAvailable += value;
    }
}


void RSingleFamilyContractorMobilizationModel::getRequests()
{

}


int RSingleFamilyContractorMobilizationModel::getNumberOfSingleFamilyCrews()
{
    return theNumberOfSingleFamilyCrews;
}


int RSingleFamilyContractorMobilizationModel::evaluateModel(RGradientType theGradientType)
{

    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
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
    QList<RSingleFamilyContractorRequest *> theAllRequestList = theDomain->findChildren<RSingleFamilyContractorRequest *>();
    QList<RRequest *> theListOfCollectedRequests;

      for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests for contractors
        if(theAllRequestList.at(i)->getContentType().contains("Allocation") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

            // Add this request to the list of requested taken
            theListOfCollectedRequests << theAllRequestList.at(i);

            //qDebug() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName() << thePacketDestinationList.size();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
            theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            theBuildingOccupancy << theAllRequestList.at(i)->getOccupancyClasses();
            theNeighborhoodIncome << theAllRequestList.at(i)->getMedianIncome();
            //}
        }
    }


    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // Next, get hold of what each requester needs
    QVector<double> auxFunds;
    QVector<RModel*> unsortedNeighborhoodPriorityList;

    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);

        // For each request received
        QVector<bool> auxWorkers;
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
            auxWorkers << false;
        }

        auxFunds << theNeighborhoodIncome.at(i);
        unsortedNeighborhoodPriorityList << theRequestDestinationList.at(i);

        shouldGetWorkers << auxWorkers;
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theNeighborhoodIncome.clear();


    QVector<QVector<bool>> selectedToReceiveSupplies;
    for (int i = 0; i < thePacketDestinationList.size(); i++){
        QVector<bool> auxVector;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){
            auxVector << false;
        }
        selectedToReceiveSupplies << auxVector;
    }


    double counter = 0.0;
    QVector<double> theVectorOfLimits;
    theVectorOfLimits << 0.0;
    int theLimit = 400;

    if (thePacketDestinationList.size() > 0 && 1 > 2){
        // I am distributing the probability of receiving resources based on the number of buildings in need of inspection in each neighbourhood
        for (int i = 0; i < thePacketDestinationList.size(); i++){
            counter += thePacketAmountList.at(i).size();
            theVectorOfLimits << counter;
        }


        //for (int i = 0; i < thePacketDestinationList.size(); i++){
        while (theLimit > 0){

            int rngI = 0;
            double theRng = rand()/static_cast<double>(RAND_MAX);

            // Randomly choose one neighbourhood based on the distribution defined above
            for (int i = 0; i < theVectorOfLimits.size() - 1; i++){
                if (theRng >= theVectorOfLimits.at(i)/counter && theRng <  theVectorOfLimits.at(i+1)/counter){
                    rngI = i;
                }
            }

            int maxJ = thePacketAmountList.at(rngI).size() - 1;
            int rngJ = qFloor(maxJ * rand()/static_cast<double>(RAND_MAX));

            if (selectedToReceiveSupplies[rngI][rngJ] == false && thePacketAmountList.at(rngI).at(rngJ) > 0.0 && theNumberOfSingleFamilyCrewsAvailable > thePacketAmountList.at(rngI).at(rngJ)){
                selectedToReceiveSupplies[rngI][rngJ] = true;
                theNumberOfSingleFamilyCrewsAvailable = qMax(theNumberOfSingleFamilyCrewsAvailable - thePacketAmountList.at(rngI).at(rngJ), 0.0);
            }

            theLimit -= 1;
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
            if (theNumberOfSingleFamilyCrewsAvailable > thePacketAmountList.at(i).at(j)){
                createPacket = true;
                selectedToReceiveSupplies[i][j] = true;
                theNumberOfSingleFamilyCrewsAvailable -= thePacketAmountList.at(i).at(j); // comment this line out to use random allocation
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

                if (selectedToReceiveSupplies.at(i).at(j) == true){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                thePacketAmountList[i][j] -= thePacketContents;

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
    }

    theResponse->setCurrentValue(theCurrentHourlyWages);


    return 1;
}

