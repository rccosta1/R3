#include "RHardwareStoreModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RRequest.h"
#include "RNormalDistribution.h"
#include <math.h>

RHardwareStoreModel::RHardwareStoreModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theNumberOfDailyDeliveries = 1;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RHardwareStoreModel::~RHardwareStoreModel()
{
}


int RHardwareStoreModel::resetTime()
{
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();

    numberOfNewPacketsCreatedHere = 0;
    theNumberOfWoodRequesters = 0.0;
    amountOfWoodRequested = 0.0;

    numberOfWoodProductsRequestsCreatedHere = 0;
    currentStockOfWoodProducts = initialStockOfWoodProducts;

    return 0;
}


QString RHardwareStoreModel::getMaterialsSupplierList()
{
    return parameterListToString(theMaterialsSupplierList);
}

void RHardwareStoreModel::setMaterialsSupplierList(QString value)
{
    theMaterialsSupplierList = stringToParameterList(value);
}


double RHardwareStoreModel::getStockOfWoodProducts() const
{
    return initialStockOfWoodProducts;
}

void RHardwareStoreModel::setStockOfWoodProducts(double value)
{
    initialStockOfWoodProducts = value;
}


int RHardwareStoreModel::getNumberOfDailyDeliveries() const
{
    return theNumberOfDailyDeliveries;
}

void RHardwareStoreModel::setNumberOfDailyDeliveries(int value)
{
    theNumberOfDailyDeliveries = value;
}


QString RHardwareStoreModel::getTruckList()
{
    return parameterListToString(theTruckList);
}


void RHardwareStoreModel::setTruckList(QString value)
{
    theTruckList = stringToParameterList(value);
}


void RHardwareStoreModel::getRequests()
{
}



int RHardwareStoreModel::evaluateModel()
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
    // 2 - Checking responses coming to this model
    // -------------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the households in this zone have an object to request disaster aid from
    for (int i = 0; i < theMaterialsSupplierList.size(); i++){

        // Cast the input as a response
        RPacketResponse *thePacketResponseComingToThisModel = qobject_cast<RPacketResponse *>(theMaterialsSupplierList.at(i));

        if (thePacketResponseComingToThisModel){
            thePacketList << theMaterialsSupplierList.at(i);
        }

        // Cast the input as a response
        RGenericResponse *theContinuousResponseComingToThisModel = qobject_cast<RGenericResponse *>(theMaterialsSupplierList.at(i));

        if (theContinuousResponseComingToThisModel){
            RNetworkModel* theModel = qobject_cast<RNetworkModel*>(theContinuousResponseComingToThisModel->getModel());
            if (theModel->getFunctionality() == true){
                //currentStockOfWoodProducts = initialStockOfWoodProducts;
            }
        }
    }

    // Load the packets received
    this->loadPackets(thePacketList);



    // Extract contents from the packets received
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {


        // Get hold of what is the content of this packet
        QVector<double> contentsReceived = packetsCurrentlyOnboard.at(i)->getPacketContents();
        QString packetType = packetsCurrentlyOnboard.at(i)->getPacketContentsType();


        for (int j = 0; j < contentsReceived.size(); j++){
            double amountReceived = contentsReceived.at(j);


            currentStockOfWoodProducts += amountReceived;
            if (theOutputLevel == RDomain::Maximum){
                qDebug() << this->objectName() << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "at" << theCurrentTime->getCurrentValue() << " Current stocks " << currentStockOfWoodProducts;
            }
        }
    }

    this->handlePacketsThatReachedFinalDestination();




    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    this->collectRequests(QString("WoodConstructionMaterials"));


    // Next, get hold of what each requester needs
    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);


        // For each request that the model passed
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

            amountOfWoodRequested += theRequestedAmountList.at(i).at(j);
        }
    }



    // Wood
    if (amountOfWoodRequested > currentStockOfWoodProducts && theRequestDestinationList.size() > 0){

        // Request construction materials
        QString materialsRequestName = QString(this->objectName() + "WoodConstructionMaterialsRequest%1").arg(numberOfWoodProductsRequestsCreatedHere++);
        theDomain->createObject(materialsRequestName, "RRequest");
        RRequest *theNewWoodProductsRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
        theNewWoodProductsRequest->setModel(this);
        theNewWoodProductsRequest->setContentType(QString("WoodProducts"));
        theNewWoodProductsRequest->setRequesterIDList(0);
        theNewWoodProductsRequest->setRequestedAmountList(amountOfWoodRequested);
        //qCritical() << this->objectName() << "is requesting" << materialsRequestName << theNewWoodProductsRequest->getRequestedAmountList() << theNewWoodProductsRequest->getContentType();
    }


    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();


    // -------------------------------------------------------------------------------------------
    // Provide materials
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object requesting
    double timeSinceLastEvaluation = theCurrentTime->getCurrentValue() - lastEvaluationTime;
    int numberOfDeliveriesInThisTimeStep = 0;

    if (thePacketDestinationList.size() > 0){
        numberOfDeliveriesInThisTimeStep += theNumberOfDailyDeliveries * timeSinceLastEvaluation;
    }



    for (int i = 0; i < thePacketDestinationList.size(); i++){

        if (numberOfDeliveriesInThisTimeStep > 0){

            // Creating a packet
            QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
            theDomain->createObject(packetName, "RPacket");
            RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
            this->getShortestPath(thePacketDestinationList.at(i));
            theNewPacket->setPacketContentsType(thePacketSupplyTypeList.at(i));
            theNewPacket->setDestinationList(theShortestPath);


            for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

                double thePacketContents = 0.0;
                if (numberOfDeliveriesInThisTimeStep > 0  && thePacketAmountList.at(i).at(j) > 0.0){
                    thePacketContents = thePacketAmountList.at(i).at(j);
                    numberOfDeliveriesInThisTimeStep -= thePacketContents;
                    //qCritical() << this->objectName() << "will make this many deliveries" << numberOfDeliveriesInThisTimeStep;
                }

                theNewPacket->setPacketContents(thePacketContents);
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                thePacketAmountList[i][j] -= thePacketContents;
            } // for j

            packetsCurrentlyOnboard << theNewPacket;
        }
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
    }


    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
