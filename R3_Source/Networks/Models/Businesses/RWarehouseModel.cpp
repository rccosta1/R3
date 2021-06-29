#include "RWarehouseModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RWarehouseModel::RWarehouseModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RWarehouseModel::~RWarehouseModel()
{
}


int RWarehouseModel::resetTime()
{
    packetsCurrentlyOnboard.clear();

    currentStock = initialStock;
    theNumberOfTrucks = 0;
    theTruckCapacity = 0.0;

    return 0;
}


QString RWarehouseModel::getConstructionMaterialsSupplierList()
{
    return parameterListToString(theConstructionMaterialsSupplierList);
}

void RWarehouseModel::setConstructionMaterialsSupplierList(QString value)
{
    theConstructionMaterialsSupplierList = stringToParameterList(value);
}


double RWarehouseModel::getStock()
{
    return initialStock;
}

void RWarehouseModel::setStock(double value)
{
    initialStock = value;
}

int RWarehouseModel::getNumberOfTrucks()
{
    return theNumberOfTrucks;
}

void RWarehouseModel::setNumberOfTrucks(int value){
    theNumberOfTrucks = value;
}


double RWarehouseModel::getTruckCapacity(){
    return theTruckCapacity;
}

void RWarehouseModel::setTruckCapacity(double value){
    theTruckCapacity = value;
}


double RWarehouseModel::getTruckVelocity(){
    return theTruckVelocity;
}

void RWarehouseModel::setTruckVelocity(double value){
    theTruckVelocity = value;
}


int RWarehouseModel::evaluateModel(RGradientType theGradientType)
{
//    // Output
//    if (theOutputLevel == RDomain::Maximum) {
//        qDebug() << "";
//        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
//    }



//    // -------------------------------------------------------------------------------------------
//    // 1 - Checking responses coming to this model
//    // -------------------------------------------------------------------------------------------
//    QList<QPointer <RParameter>> thePacketList;

//    // Check if the households in this zone have an object to request disaster aid from
//    for (int i = 0; i < theConstructionMaterialsSupplierList.size(); i++){

//        // Cast the input as a response
//        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theConstructionMaterialsSupplierList.at(i));

//        if (theResponseComingToThisModel){
//            thePacketList << theConstructionMaterialsSupplierList.at(i);
//        }
//    }







//    // -------------------------------------------------------------------------------------------
//    // 2 - Checking requests received
//    // -------------------------------------------------------------------------------------------
//    // Creating a list of paths that contain this model - getting hold of its connections
//    if (myPossiblePaths.size() == 0){
//        getPossiblePaths();
//    }


//    // Get hold of all object in need of supplies
//    QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();
//    QList<RRequest *> theListOfCollectedRequests;



//    for (int i = 0; i < theAllRequestList.size(); i++){

//        // Collecting the requests that need construction materials
//        if(theAllRequestList.at(i)->getContentType() == QString("ConstructionMaterials")){


//            // If this request is for money and this model can supply it
//            if (this->getShortestPath(theAllRequestList.at(i)->getModel()) != 0){

//                // Add this request to the list of requested taken
//                theListOfCollectedRequests << theAllRequestList.at(i);
//                qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i);

//                QVector<double> theRequestedAmounts = theAllRequestList.at(i)->getRequestedAmountList();
//                QVector<QPair<int,int>> theIDs = theAllRequestList.at(i)->getRequesterIDList();
//                QVector<double> theCriteria = theAllRequestList.at(i)->getListOfCriteria();

//                // Add the information from this request to the list of requests picked by this object
//                this->setListOfRequesters(theAllRequestList.at(i)->getModel());
//                this->setListOfRequestedSupplies(theRequestedAmounts, theAllRequestList.at(i)->getContentType(), theCriteria, theIDs);
//            }
//        }
//    }



//    // Delete the requests that were picked up by this object
//    int numberOfRequestsCollected = theListOfCollectedRequests.size();
//    for (int i = 0; i < numberOfRequestsCollected; i++){
//        theDomain->deleteObject(theListOfCollectedRequests.at(i)->objectName());
//    }


//    // -------------------------------------------------------------------------------------------
//    // Get hold of the requests received
//    // -------------------------------------------------------------------------------------------

//    // Next, get hold of what each requester needs
//    QVector<QVector<double>> materialsToBeProviveded;
//    QVector<QPair<int,int>> theMatrixOfIndexes;
//    QVector<double> theAuxCriterionList; // this is a copy of the list of bids
//    QVector<double> theAuxAmountList; // this is a copy of the list of requests


//    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
//    // For each model in need of financial assistance
//    for (int i = 0; i < theRequestDestinationList.size(); i++){

//        QVector<double> materials;

//        // For each request that the model passed
//        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

//            theMatrixOfIndexes << qMakePair(i,j);

//            // Create an auxiliary list with the variable used as criteria to determine priority from all requests
//            theAuxCriterionList << theRequestCriterionList.at(i).at(j);
//            theAuxAmountList << theRequestedAmountList.at(i).at(j);

//            materials << 0.0;
//        }

//        materialsToBeProviveded << materials;
//    }


//    // -------------------------------------------------------------------------------------------
//    // Provide materials for the households that have the highest bids
//    // -------------------------------------------------------------------------------------------
//    // While I can provide materials
//    while (theAuxCriterionList.size() > 0){

//        // Find the household with the highest bid
//        double highestPriorityObject = *std::max_element(theAuxCriterionList.constBegin(), theAuxCriterionList.constEnd());
//        int highestPriorityI = theMatrixOfIndexes.at(theAuxCriterionList.indexOf(highestPriorityObject)).first;
//        int highestPriorityJ = theMatrixOfIndexes.at(theAuxCriterionList.indexOf(highestPriorityObject)).second;


//        // Provide the requested assistance if possible
//        if (currentStock >= theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject))){

//            materialsToBeProviveded[highestPriorityI][highestPriorityJ] = theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject));
//            currentStock -= theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject));
//        }

//        // If not, provide as many as there are available
//        else {
//            materialsToBeProviveded[highestPriorityI][highestPriorityJ] = currentStock;
//            currentStock -= currentStock;
//            qCritical() << "*************************************************" << this->objectName() << "ran out of supplies!";
//        }


//        theAuxAmountList.removeAt(theAuxCriterionList.indexOf(highestPriorityObject));
//        theAuxCriterionList.removeAt(theAuxCriterionList.indexOf(highestPriorityObject));
//        theMatrixOfIndexes.removeAt(theMatrixOfIndexes.indexOf(qMakePair(highestPriorityI,highestPriorityJ)));
//    }


//    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
//    // For each object requesting financial aid
//    int requestCounter = 0;
//    for (int i = 0; i < theRequestDestinationList.size(); i++){

//        // Creating a packet
//        QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
//        theDomain->createObject(packetName, "RPacket");
//        RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
//        theNewPacket->setPacketContentsType(theRequestedSupplyTypeList.at(i));
//        theNewPacket->setDestinationList(theShortestPath);
//        theNewPacket->setPacketVelocity(theTruckVelocity);


//        // For each request that the model passed
//        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
//            double thePacketContents = materialsToBeProviveded.at(i).at(j);
//            theNewPacket->setPacketContents(thePacketContents);
//            theNewPacket->setPacketOwner(theRequesterIDList.at(i).at(j));
//            requestCounter += 1;
//        }


//        packetsCurrentlyOnboard << theNewPacket;
//    }


//    // Unload packets into the packet response
//    unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);


//    // Clear all lists
//    packetsCurrentlyOnboard.clear();
//    theRequestCriterionList.clear();
//    theRequesterIDList.clear();
//    theRequestedSupplyTypeList.clear();
//    theRequestedAmountList.clear();
//    theRequestDestinationList.clear();

    return 1;
}
