#include "RContractorNegotiationModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RContractorNegotiationRequest.h"
#include <algorithm>


RContractorNegotiationModel::RContractorNegotiationModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    packetsCurrentlyOnboard.clear();
}


RContractorNegotiationModel::~RContractorNegotiationModel()
{

}


int RContractorNegotiationModel::resetTime()
{ 
    timeOfEvent = 0.0;
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();
    theBuildingRepairClass.clear();
    theBuildingNumberOfStoreys.clear();
    theBiddingDelayList.clear();
    timeRequestsWereReceived.clear();

    numberOfNewPacketsCreatedHere = 0;

    return 0;
}




void RContractorNegotiationModel::getRequests()
{

}


int RContractorNegotiationModel::evaluateModel()
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
    //this->collectRequests(QString("Mobilization"));
    QList<RContractorNegotiationRequest *> theAllRequestList = theDomain->findChildren<RContractorNegotiationRequest *>();
    QList<RRequest *> theListOfCollectedRequests;
    QVector<int> auxNumberOfStoreys;
    QVector<QVector<QString>> theBuildingOccupancyClass;

    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests for contractors
        if(theAllRequestList.at(i)->getContentType().contains("Negotiation") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

            // Add this request to the list of requested taken
            theListOfCollectedRequests << theAllRequestList.at(i);


            //qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
            theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            theBuildingRepairClass << theAllRequestList.at(i)->getRepairClasses();
            auxNumberOfStoreys << theAllRequestList.at(i)->getNumberOfStoreys();
            theBuildingOccupancyClass << theAllRequestList.at(i)->getOccupancyClasses();
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
        theBuildingNumberOfStoreys << auxNumberOfStoreys;

        QVector<double> theDelaysForEachBuilding;
        QVector<double> theTimeRequestsWereReceived;

        // For each request received
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
            theTimeRequestsWereReceived << theCurrentTime->getCurrentValue();
            double medianOfDelay = 0.0;
            double dispersionOfDelay = 0.0;

            if(auxNumberOfStoreys.at(j) >= 20){

                if (theBuildingRepairClass.at(i).at(j) == 1){
                    medianOfDelay = 3.0; //28
                    dispersionOfDelay = 0.66; //0.3
                } else if (theBuildingRepairClass.at(i).at(j) == 3){
                    medianOfDelay = 7.0; //40
                    dispersionOfDelay = 0.35; //0.31
                } else {
                    medianOfDelay = 0.0;
                    dispersionOfDelay = 0.0;
                }

            } else {

                if (theBuildingRepairClass.at(i).at(j) == 1){
                    medianOfDelay = 3.0; //11
                    dispersionOfDelay = 0.66; // 0.43
                } else if (theBuildingRepairClass.at(i).at(j) == 3){
                    medianOfDelay = 7.0; // 23
                    dispersionOfDelay = 0.35; //0.41
                } else {
                    medianOfDelay = 0.0;
                    dispersionOfDelay = 0.0;
                }
            }


            double m = medianOfDelay;
            double v = dispersionOfDelay * dispersionOfDelay;
            double phi = qSqrt(v + m * m);
            double mu = qLn(m * m / phi);
            double sigma = qSqrt(qLn((phi*phi) / (m*m)));


            double theUx = rand()/static_cast<double>(RAND_MAX);

            if (theBuildingOccupancyClass.at(i).at(j) == QString("RES3")){
                theDelaysForEachBuilding << 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
            } else {
                theDelaysForEachBuilding << 7.0;
            }
        }

        theBiddingDelayList << theDelaysForEachBuilding;
        timeRequestsWereReceived << theTimeRequestsWereReceived;
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theBuildingRepairClass.clear();



    // -------------------------------------------------------------------------------------------
    // 3 - Provide supplies
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object requesting
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        // Making sure packets are only created when there is something to be delivered
        bool createPacket = false;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            if (theCurrentTime->getCurrentValue() - timeRequestsWereReceived.at(i).at(j) > theBiddingDelayList.at(i).at(j) && thePacketAmountList.at(i).at(j) > 0.0){
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
                if (theCurrentTime->getCurrentValue() - timeRequestsWereReceived.at(i).at(j) > theBiddingDelayList.at(i).at(j)){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                //qCritical() << "Packet for" << thePacketIDList.at(i).at(j) << "with" << thePacketContents;

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
        theBuildingNumberOfStoreys.removeAt(indexesToRemove.at(i));
        timeRequestsWereReceived.removeAt(indexesToRemove.at(i));
        theBiddingDelayList.removeAt(indexesToRemove.at(i));
    }

    theResponse->setCurrentValue(1.0);


    return 1;
}

