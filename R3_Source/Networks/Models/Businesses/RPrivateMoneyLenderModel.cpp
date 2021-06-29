#include "RPrivateMoneyLenderModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RFinancingRequest.h"
#include <algorithm>


RPrivateMoneyLenderModel::RPrivateMoneyLenderModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RPrivateMoneyLenderModel::~RPrivateMoneyLenderModel()
{

}


int RPrivateMoneyLenderModel::resetTime()
{
    timeOfEvent = 0.0;
    numberOfNewPacketsCreatedHere = 0;

    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketIDList.clear();
    thePacketSupplyTypeList.clear();
    timeRequestsWereReceived.clear();
    thePacketProcessingTimeList.clear();

    return 0;
}


double RPrivateMoneyLenderModel::getMedianDelayForPayments()
{
    return theMedianDelayForPayments;
}

void RPrivateMoneyLenderModel::setMedianDelayForPayments(double value)
{
    theMedianDelayForPayments = value;
}


double RPrivateMoneyLenderModel::getDispersionOfDelayForPayments()
{
    return theDispersionOfDelayForPayments;
}

void RPrivateMoneyLenderModel::setDispersionOfDelayForPayments(double value)
{
    theDispersionOfDelayForPayments = value;
}


void RPrivateMoneyLenderModel::getRequests()
{

}


int RPrivateMoneyLenderModel::evaluateModel()
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
    QList<RFinancingRequest *> theAllRequestList = theDomain->findChildren<RFinancingRequest *>();
    QList<RRequest *> theListOfCollectedRequests;

    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests that need construction materials
        if(theAllRequestList.at(i)->getContentType().contains("PrivateMoney") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

                // Add this request to the list of requested taken
                theListOfCollectedRequests << theAllRequestList.at(i);

                //qDebug() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

                // Add the information from this request to the list of requests picked by this object
                theRequestDestinationList << theAllRequestList.at(i)->getModel();
                theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
                theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
                theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
                theRequesterIncomeList << theAllRequestList.at(i)->getRequesterIncome();

        }
    }


    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // For each model in need of financial assistance

    // Next, get hold of what each requester needs
    double amountRequested = 0.0;

    double m = theMedianDelayForPayments;
    double v = theDispersionOfDelayForPayments * theDispersionOfDelayForPayments;
    double phi = qSqrt(v + m * m);
    double mu = qLn(m * m / phi);
    double sigma = qSqrt(qLn((phi*phi) / (m*m)));


    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an Packetiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);

        QVector<double> theDelaysForEachBuilding;
        QVector<double> theTimeRequestsWereReceived;

        // For each request received
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
            amountRequested += theRequestedAmountList.at(i).at(j);

            double theUx = rand()/static_cast<double>(RAND_MAX);
            theDelaysForEachBuilding << 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
            theTimeRequestsWereReceived << theCurrentTime->getCurrentValue();

            if (theRequesterIncomeList.at(i).at(j) != QString("High")){
                theDelaysForEachBuilding += 45.0;
            }
        }

        thePacketProcessingTimeList << theDelaysForEachBuilding;
        timeRequestsWereReceived << theTimeRequestsWereReceived;
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theRequesterIncomeList.clear();


    // -------------------------------------------------------------------------------------------
    // Provide money for the households that have the lowest claims
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object making a claim
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        // Making sure packets are only created when there is something to be delivered
        bool createPacket = false;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            if (theCurrentTime->getCurrentValue() - timeRequestsWereReceived.at(i).at(j) > thePacketProcessingTimeList.at(i).at(j) && thePacketAmountList.at(i).at(j) > 0.0){
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
                if (theCurrentTime->getCurrentValue() - timeRequestsWereReceived.at(i).at(j) > thePacketProcessingTimeList.at(i).at(j)){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                thePacketAmountList[i][j] -= thePacketContents;
            } // for j

            packetsCurrentlyOnboard << theNewPacket;
        }
    } // for i


    // Unload packets into the packet response
    if (packetsCurrentlyOnboard.size() > 0){
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
        timeRequestsWereReceived.removeAt(indexesToRemove.at(i));
        thePacketProcessingTimeList.removeAt(indexesToRemove.at(i));
    }


    return 1;
}





