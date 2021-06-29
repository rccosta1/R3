#include "RConcretePlantModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include <math.h>


RConcretePlantModel::RConcretePlantModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theNumberOfDailyDeliveries = 1;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RConcretePlantModel::~RConcretePlantModel()
{
}


int RConcretePlantModel::resetTime()
{
    packetsCurrentlyOnboard.clear();
    thePacketDestinationList.clear();
    thePacketAmountList.clear();
    thePacketIDList.clear();
    thePacketSupplyTypeList.clear();

    currentConcreteStock = initialConcreteStock;
    currentCementStock = 0.0;
    currentSandStock = 0.0;
    currentGravelStock = 0.0;


    numberOfCementRequestsCreatedHere = 0;
    currentCementStock = initialConcreteStock;

    numberOfSandRequestsCreatedHere = 0;
    currentSandStock = initialConcreteStock;

    numberOfGravelRequestsCreatedHere = 0;
    currentGravelStock = initialConcreteStock;

    return 0;
}


QString RConcretePlantModel::getMaterialsSupplierList()
{
    return parameterListToString(theMaterialsMaterialsSupplierList);
}

void RConcretePlantModel::setMaterialsSupplierList(QString value)
{
    theMaterialsMaterialsSupplierList = stringToParameterList(value);
}


double RConcretePlantModel::getConcreteStock() const
{
    return initialConcreteStock;
}

void RConcretePlantModel::setConcreteStock(double value)
{
    initialConcreteStock = value;
}


int RConcretePlantModel::getNumberOfDailyDeliveries() const
{
    return theNumberOfDailyDeliveries;
}

void RConcretePlantModel::setNumberOfDailyDeliveries(int value)
{
    theNumberOfDailyDeliveries = value;
}



void RConcretePlantModel::getRequests()
{

}



int RConcretePlantModel::evaluateModel()
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
    // 1 - Checking responses coming to this model
    // -------------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the households in this zone have an object to request disaster aid from
    for (int i = 0; i < theMaterialsMaterialsSupplierList.size(); i++){

        // Cast the input as a response
        RPacketResponse *thePacketResponseComingToThisModel = qobject_cast<RPacketResponse *>(theMaterialsMaterialsSupplierList.at(i));

        if (thePacketResponseComingToThisModel){
            thePacketList << theMaterialsMaterialsSupplierList.at(i);
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


            // If this packet contains cement products
            if (packetType == QString("Cement")){
                currentCementStock += amountReceived;
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "at" << theCurrentTime->getCurrentValue() << ". Current stocks " << currentCementStock;
                }
            }

            // If this packet contains sand products
            else if (packetType == QString("Sand")){
                currentSandStock += amountReceived;
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "at" << theCurrentTime->getCurrentValue() << ". Current stocks " << currentSandStock;
                }
            }


            // If this packet contains gravel products
            else if (packetType == QString("Gravel")){
                currentGravelStock += amountReceived;
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "at" << theCurrentTime->getCurrentValue() << ". Current stocks " << currentGravelStock;
                }
            }
        }
    }


    double minimumStock = qMin(currentGravelStock,qMin(currentCementStock,currentSandStock));
    currentConcreteStock += minimumStock;
    currentCementStock -= minimumStock;
    currentSandStock -= minimumStock;
    currentGravelStock -= minimumStock;


    this->handlePacketsThatReachedFinalDestination();




    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    this->collectRequests(QString("Concrete"));


    // Next, get hold of what each requester needs
    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);


        // For each request that the model passed
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

            amountOfConcreteRequested += theRequestedAmountList.at(i).at(j);
        }
    }



    // Cement
    if (amountOfConcreteRequested > currentConcreteStock && theRequestDestinationList.size() > 0){

        // Cement
        QString CementRequestName = QString(this->objectName() + "CementRequest%1").arg(numberOfCementRequestsCreatedHere++);
        theDomain->createObject(CementRequestName, "RRequest");
        RRequest *theNewCementProductsRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
        theNewCementProductsRequest->setModel(this);
        theNewCementProductsRequest->setContentType(QString("Cement"));
        theNewCementProductsRequest->setRequesterIDList(0);
        theNewCementProductsRequest->setRequestedAmountList(qMax(amountOfConcreteRequested - currentConcreteStock - currentCementStock,0.0));
        //qCritical() << this->objectName() << "is requesting" << CementRequestName << theNewCementProductsRequest->getRequestedAmountList() << theNewCementProductsRequest->getContentType();


        // Sand
        QString SandRequestName = QString(this->objectName() + "SandRequest%1").arg(numberOfSandRequestsCreatedHere++);
        theDomain->createObject(SandRequestName, "RRequest");
        RRequest *theNewSandProductsRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
        theNewSandProductsRequest->setModel(this);
        theNewSandProductsRequest->setContentType(QString("Sand"));
        theNewSandProductsRequest->setRequesterIDList(0);
        theNewSandProductsRequest->setRequestedAmountList(qMax(amountOfConcreteRequested - currentConcreteStock - currentSandStock,0.0));
        //qCritical() << this->objectName() << "is requesting" << SandRequestName;


        // Gravel
        QString GravelRequestName = QString(this->objectName() + "GravelRequest%1").arg(numberOfGravelRequestsCreatedHere++);
        theDomain->createObject(GravelRequestName, "RRequest");
        RRequest *theNewGravelProductsRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
        theNewGravelProductsRequest->setModel(this);
        theNewGravelProductsRequest->setContentType(QString("Gravel"));
        theNewGravelProductsRequest->setRequesterIDList(0);
        theNewGravelProductsRequest->setRequestedAmountList(qMax(amountOfConcreteRequested - currentConcreteStock - currentGravelStock,0.0));
        //qCritical() << this->objectName() << "is requesting" << GravelRequestName;
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
