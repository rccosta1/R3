#include "RPermitAssessorModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include "RNormalDistribution.h"
#include "RPermitRequest.h"
#include <algorithm>


RPermitAssessorModel::RPermitAssessorModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    thePathMatrixNeed = false;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RPermitAssessorModel::~RPermitAssessorModel()
{

}


int RPermitAssessorModel::resetTime()
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
    theTimeUnderProcessing.clear();
    theBuildingRepairClass.clear();

    // Define the random number of workers available in this scenario - remove this after paper
    theNumberOfEngineers = theBaselineNumberOfEngineers;// + samplingMultiplier * theBaselineNumberOfEngineers;

    double rngEngineers = (double) rand()/RAND_MAX;
    //theNumberOfEngineers = theBaselineNumberOfEngineers + 2.0*rngEngineers * theBaselineNumberOfEngineers;

    samplingMultiplier += 0.1;

    return 0;
}


int RPermitAssessorModel::getBaselineNumberOfEngineers()
{
    return theBaselineNumberOfEngineers;
}

void RPermitAssessorModel::setBaselineNumberOfEngineers(int value)
{
    theBaselineNumberOfEngineers = value;
}


int RPermitAssessorModel::getNumberOfEngineers()
{
    return theNumberOfEngineers;
}


int RPermitAssessorModel::evaluateModel()
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
    //this->collectRequests(QString("Permit"));

    QList<RPermitRequest *> theAllRequestList = theDomain->findChildren<RPermitRequest *>();
    QList<RRequest *> theListOfCollectedRequests;
    QVector<QVector<QString>> theBuildingOccupancyClass;

    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests
        if(theAllRequestList.at(i)->getContentType().contains("Permit") && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){

                // Add this request to the list of requested taken
                theListOfCollectedRequests << theAllRequestList.at(i);

                //qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

                // Add the information from this request to the list of requests picked by this object
                theRequestDestinationList << theAllRequestList.at(i)->getModel();
                theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
                theRequestedAmountList << theAllRequestList.at(i)->getRequestedAmountList();
                theRequesterIDList << theAllRequestList.at(i)->getRequesterIDList();
                theBuildingRepairClass << theAllRequestList.at(i)->getRepairClasses();
                theBuildingOccupancyClass << theAllRequestList.at(i)->getOccupancyClasses();
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

        QVector<double> auxVector;
        auxVector.resize(theRequestedAmountList.at(i).size());
        auxVector.fill(0.0);
        theTimeUnderProcessing << auxVector; // just populating this vector


        // For each request received
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

            double medianOfDelay = 0.0;
            double dispersionOfDelay = 0.0;
            if (theBuildingRepairClass.at(i).at(j) == 1){
                medianOfDelay = 1.0;
                dispersionOfDelay = 0.86;
            } else if (theBuildingRepairClass.at(i).at(j) == 3){
                medianOfDelay = 8.0;
                dispersionOfDelay = 0.32;
            }

            double m = medianOfDelay;
            double v = dispersionOfDelay * dispersionOfDelay;
            double phi = qSqrt(v + m * m);
            double mu = qLn(m * m / phi);
            double sigma = qSqrt(qLn((phi*phi) / (m*m)));


            double theUx = (double) rand()/RAND_MAX;
            if (theBuildingOccupancyClass.at(i).at(j) == QString("RES3")){
                theDelaysForEachBuilding << 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
            } else {
                theDelaysForEachBuilding << 7.0;
            }

        }

        thePacketProcessingTimeList << theDelaysForEachBuilding;
    }


    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
    theBuildingRepairClass.clear();


    int theNumberOfEngineersAvailable = theNumberOfEngineers;


    for (int i = 0; i < thePacketDestinationList.size(); i++){
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            // On every time step allocate engineers to 'work' on the projects that still need design
            if (theNumberOfEngineersAvailable > thePacketAmountList.at(i).at(j)){
                theTimeUnderProcessing[i][j] += theCurrentTime->getCurrentValue() - lastEvaluationTime;
                theNumberOfEngineersAvailable -= thePacketAmountList.at(i).at(j);
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


    lastEvaluationTime = theCurrentTime->getCurrentValue();
    return 1;
}





