#include "RResidenceFactoryModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RResidenceRequest.h"
#include "RWorkforceStudyNeighborhoodModel.h"
#include <algorithm>


RResidenceFactoryModel::RResidenceFactoryModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RResidenceFactoryModel::~RResidenceFactoryModel()
{

}


int RResidenceFactoryModel::resetTime()
{
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    numberOfNewPacketsCreatedHere = 0;
    theTotalDemandForResidences = 0;

    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketIDList.clear();
    thePacketSupplyTypeList.clear();
    thePacketProcessingTimeList.clear();
    theTimeUnderProcessing.clear();
    theBuildingRepairClass.clear();

    return 0;
}


int RResidenceFactoryModel::getBaselineRentalHomes()
{
    return theBaselineRentalHomes;
}

void RResidenceFactoryModel::setBaselineRentalHomes(int value)
{
    theBaselineRentalHomes = value;
}


int RResidenceFactoryModel::getInterventionID()
{
    return theInterventionID;
}

void RResidenceFactoryModel::setInterventionID(int value)
{
    theInterventionID = value;
}


int RResidenceFactoryModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    if (theTotalDemandForResidences == 0){
        QList<RWorkforceStudyNeighborhoodModel*> theListOfNeighborhoods = theDomain->findChildren<RWorkforceStudyNeighborhoodModel*>();
        for (int i = 0; i < theListOfNeighborhoods.size();i++){
            theTotalDemandForResidences += theListOfNeighborhoods.at(i)->getNumberOfPersonsInHotels() +
                    theListOfNeighborhoods.at(i)->getNumberOfPersonsInShelters() +
                    theListOfNeighborhoods.at(i)->getNumberOfPersonsLivingWithFamily();
        }
    }

    // Get hold of the residences added in this time step
    theNumberOfHomesCreatedInThisTimeStep = 0;
    if (theInterventionID == 1){
        // Add this many homes per day
        theNumberOfHomesCreatedInThisTimeStep = qFloor((theCurrentTime->getCurrentValue() - lastEvaluationTime) * 10.0); // 10 homes per day
    }


    // Check if


    // Ignore this part
    if (1 > 2){
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
        //this->collectRequests(QString("Permit"));

        QList<RResidenceRequest *> theAllRequestList = theDomain->findChildren<RResidenceRequest *>();
        QList<RRequest *> theListOfCollectedRequests;
        QVector<QVector<QString>> theBuildingOccupancyClass;

        for (int i = 0; i < theAllRequestList.size(); i++){

            // Collecting the requests
            if(theAllRequestList.at(i)->getContentType().contains("Residence") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

                // Add this request to the list of requested taken
                theListOfCollectedRequests << theAllRequestList.at(i);

                //qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

                // Add the information from this request to the list of requests picked by this object
                theRequestDestinationList << theAllRequestList.at(i)->getModel();
                theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
                theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
                theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
            }
        }




        // -------------------------------------------------------------------------------------------
        // Get hold of the requests received
        // -------------------------------------------------------------------------------------------
        // First I get hold of all requests received from different objects. These are stored in a single-column vector
        // Next, get hold of what each requester needs
        for (int i = 0; i < theRequestDestinationList.size(); i++){

            thePacketDestinationList << theRequestDestinationList.at(i);
            thePacketAmountList << theRequestedAmountList.at(i);
            thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
            thePacketIDList << theRequesterIDList.at(i);
            QVector<double> theDelaysForEachBuilding;

        }

        theRequesterIDList.clear();
        theRequestedSupplyTypeList.clear();
        theRequestedAmountList.clear();
        theRequestDestinationList.clear();
        theBuildingRepairClass.clear();




        // -------------------------------------------------------------------------------------------
        // Provide supplies
        // -------------------------------------------------------------------------------------------
        for (int i = 0; i < thePacketDestinationList.size(); i++){


            // Making sure packets are only created when there is something to be delivered
            bool createPacket = false;
            for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

                if (theTimeUnderProcessing.at(i).at(j) >= thePacketProcessingTimeList.at(i).at(j) && thePacketAmountList.at(i).at(j) > 0.0){
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
                    if (theTimeUnderProcessing.at(i).at(j) >= thePacketProcessingTimeList.at(i).at(j)){
                        thePacketContents = thePacketAmountList.at(i).at(j);
                    }

                    theNewPacket->setPacketContents(thePacketContents);
                    theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                    //qCritical() << "Packet for" << thePacketIDList.at(i).at(j) << "with" << thePacketContents;

                    thePacketAmountList[i][j] -= thePacketContents;
                } // for j

                packetsCurrentlyOnboard << theNewPacket;
            }
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
            theTimeUnderProcessing.removeAt(indexesToRemove.at(i));
            thePacketProcessingTimeList.removeAt(indexesToRemove.at(i));
        }
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();
    return 1;
}





