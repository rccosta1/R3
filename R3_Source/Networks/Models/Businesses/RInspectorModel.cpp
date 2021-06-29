#include "RInspectorModel.h"
#include "RParameter.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include "RNormalDistribution.h"
#include <algorithm>


RInspectorModel::RInspectorModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Initialize class variables
    thePathMatrixNeed = false;
    theMeanInspectionTime = 1.0;
    theHypocentre = nullptr;
    theBaselineNumberOfInspectors = 0;
    theNumberOfInspectors = 0;
    theNumberOfDailyWorkHours = 0.0;
    theMeanInspectionTime = 0.0;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RInspectorModel::~RInspectorModel()
{

}


int RInspectorModel::resetTime()
{
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    numberOfNewPacketsCreatedHere = 0;

    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();
    thePacketTimeList.clear();


    // Define the random number of workers available in this scenario - remove this after paper
    double rngInspectors = rand()/static_cast<double>(RAND_MAX);
    theNumberOfInspectors = theBaselineNumberOfInspectors;// + samplingMultiplier * theBaselineNumberOfInspectors;


     samplingMultiplier += 0.1;

    return 0;
}


int RInspectorModel::getBaselineNumberOfInspectors()
{
    return theBaselineNumberOfInspectors;
}

void RInspectorModel::setBaselineNumberOfInspectors(int value)
{
    theBaselineNumberOfInspectors = value;
}

double RInspectorModel::getNumberOfDailyWorkHours()
{
    return theNumberOfDailyWorkHours;
}

void RInspectorModel::setNumberOfDailyWorkHours(double value)
{
    theNumberOfDailyWorkHours = value;
}


double RInspectorModel::getMeanInspectionTime()
{
    return theMeanInspectionTime;
}

void RInspectorModel::setMeanInspectionTime(double value)
{
    theMeanInspectionTime = value;
}


QObject* RInspectorModel::getEpicenterLocation()
{
    return theHypocentre;
}

void RInspectorModel::setEpicenterLocation(QObject* value)
{
    theHypocentre = qobject_cast<RLocation*>(value);
}


double RInspectorModel::computeSurfaceDistance(RLocation *requester, RLocation *epicentre)
{
    double lat1 = requester->getLatitude() / 180 * 3.14159265359;
    double lng1 = requester->getLongitude() / 180 * 3.14159265359;

    double lat2 = epicentre->getLatitude() / 180 * 3.14159265359;
    double lng2 = epicentre->getLongitude() / 180 * 3.14159265359;

    double distance = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng2 - lng1)) * EarthRadius;

    return distance;
}


void RInspectorModel::getRequests()
{

}


int RInspectorModel::getNumberOfInspectors()
{
    return theNumberOfInspectors;
}


int RInspectorModel::evaluateModel()
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

    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    //this->collectRequests(QString("Inspector"));
    QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();
    QList<RRequest *> theListOfCollectedRequests;

    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests that need construction materials
        if(theAllRequestList.at(i)->getContentType().contains("Inspector")){

            // Add this request to the list of requested taken
            theListOfCollectedRequests << theAllRequestList.at(i);

            //qDebug() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

            QVector<double> theRequestedAmounts = theAllRequestList.at(i)->getRequestedAmountList();
            QVector<int> theIDs = theAllRequestList.at(i)->getRequesterIDList();

            // Add the information from this request to the list of requests picked by this object
            theRequestDestinationList << theAllRequestList.at(i)->getModel();
            theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
            theRequestedAmountList << theRequestedAmounts;
            theRequesterIDList << theIDs;
        }
    }

    // Delete the requests that were picked up by this object
    int numberOfRequestsCollected = theListOfCollectedRequests.size();
    for (int i = 0; i < numberOfRequestsCollected; i++){
        theDomain->deleteObject(theListOfCollectedRequests.at(i)->objectName());
    }

    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------
    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // Next, get hold of what each requester needs
    for (int i = 0; i < theRequestDestinationList.size(); i++){
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);
        thePacketTimeList << theCurrentTime->getCurrentValue();
    }

    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();

    // -------------------------------------------------------------------------------------------
    // Provide money for the households that have the lowest claims
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object making a claim
    double timeSinceLastEvaluation = theCurrentTime->getCurrentValue() - lastEvaluationTime;
    int numberOfInspectionsInThisTimeStep = 0;

    double counter = 0.0;
    QVector<double> theVectorOfLimits;
    theVectorOfLimits << 0.0;
    if (thePacketDestinationList.size() > 0){
        // assume inspectors work 12h/d
        numberOfInspectionsInThisTimeStep += qFloor((theNumberOfDailyWorkHours / 24.0) * theNumberOfInspectors * (timeSinceLastEvaluation * 24.0) / theMeanInspectionTime);
    }

    QVector<QVector<bool>> selectedForInspection;
    for (int i = 0; i < thePacketDestinationList.size(); i++){
        QVector<bool> auxVector;
        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){
            auxVector << false;
        }
        selectedForInspection << auxVector;

        // I am distributing the probability of being inspected based on the number of buildings in need of inspection in each neighbourhood
        counter += thePacketAmountList.at(i).size();
        theVectorOfLimits << counter;
    }


//    for (int i = 0; i < numberOfInspectionsInThisTimeStep; i++){
//        int rngI = 0;
//        double theRng = rand()/static_cast<double>(RAND_MAX);

//        // Randomly choose one neighbourhood based on the distribution defined above
//        for (int j = 0; j < theVectorOfLimits.size() - 1; j++){
//            if (theRng >= theVectorOfLimits.at(j)/counter && theRng <  theVectorOfLimits.at(j+1)/counter){
//                rngI = j;
//            }
//        }

//        int maxJ = thePacketAmountList.at(rngI).size() - 1;
//        int rngJ = qFloor(maxJ * rand()/static_cast<double>(RAND_MAX));

//        selectedForInspection[rngI][rngJ] = true;
//    }

    for (int i = 0; i < thePacketDestinationList.size(); i++){

        // Creating a packet
        QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
        theDomain->createObject(packetName, "RPacket");
        RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
        this->getShortestPath(thePacketDestinationList.at(i));
        theNewPacket->setPacketContentsType(thePacketSupplyTypeList.at(i));
        theNewPacket->setDestinationList(thePacketDestinationList.at(i));

        for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

            double thePacketContents = 0.0;
            if (numberOfInspectionsInThisTimeStep > 0 && thePacketAmountList.at(i).at(j) > 0.0 ){ //&& selectedForInspection.at(i).at(j) == true
                thePacketContents = thePacketAmountList.at(i).at(j);
                numberOfInspectionsInThisTimeStep -= qFloor(thePacketContents);
            }


            theNewPacket->setPacketContents(thePacketContents);
            theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
            //qCritical() << "Packet for" << thePacketDestinationList.at(i)->objectName() << thePacketIDList.at(i).at(j) << "with" << thePacketContents;

            thePacketAmountList[i][j] -= thePacketContents;
        } // for j

        packetsCurrentlyOnboard << theNewPacket;
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
        selectedForInspection.removeAt(indexesToRemove.at(i));
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();
    return 1;
}





