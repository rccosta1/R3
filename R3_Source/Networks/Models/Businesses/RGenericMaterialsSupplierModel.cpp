#include "RGenericMaterialsSupplierModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RRequest.h"
#include "RNormalDistribution.h"
#include <math.h>

RGenericMaterialsSupplierModel::RGenericMaterialsSupplierModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

}


RGenericMaterialsSupplierModel::~RGenericMaterialsSupplierModel()
{
}


int RGenericMaterialsSupplierModel::resetTime()
{
    timeOfEvent = 0.0;
    packetsCurrentlyOnboard.clear();
    theMatrixOfIndexes.clear();
    thePacketDestinationList.clear();
    thePacketCriterionList.clear();
    thePacketAmountList.clear();
    thePacketSupplyTypeList.clear();
    thePacketIDList.clear();
    numberOfNewPacketsCreatedHere = 0;


    return 0;
}



QString RGenericMaterialsSupplierModel::getTruckList()
{
    return parameterListToString(theTruckList);
}


void RGenericMaterialsSupplierModel::setTruckList(QString value)
{
    theTruckList = stringToParameterList(value);
}


void RGenericMaterialsSupplierModel::getRequests()
{
}



int RGenericMaterialsSupplierModel::evaluateModel()
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
    // 1 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    this->collectRequests(QString("WoodProducts"));
    this->collectRequests(QString("Cement"));
    this->collectRequests(QString("Sand"));
    this->collectRequests(QString("Gravel"));



    // Next, get hold of what each requester needs
    for (int i = 0; i < theRequestDestinationList.size(); i++){

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        thePacketDestinationList << theRequestDestinationList.at(i);
        thePacketAmountList << theRequestedAmountList.at(i);
        thePacketSupplyTypeList << theRequestedSupplyTypeList.at(i);
        thePacketIDList << theRequesterIDList.at(i);
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
    for (int i = 0; i < thePacketDestinationList.size(); i++){

        QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
        theDomain->createObject(packetName, "RPacket");
        RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
        theNewPacket->setPacketContentsType(thePacketSupplyTypeList.at(i));
        theNewPacket->setDestinationList(theShortestPath);


        this->getShortestPath(thePacketDestinationList.at(i));

            for (int j = 0; j < thePacketAmountList.at(i).size(); j++){

                theNewPacket->setPacketContents(2.0*thePacketAmountList.at(i).at(j));
                theNewPacket->setPacketOwner(thePacketIDList.at(i).at(j));
                thePacketAmountList[i][j] = 0.0;
                //qCritical() << this->objectName() << "sending" << theNewPacket << "to" << thePacketDestinationList.at(i) << thePacketContents ;

            } // for j

            packetsCurrentlyOnboard << theNewPacket;

    } // for i



    unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);



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
        thePacketCriterionList.removeAt(indexesToRemove.at(i));
        thePacketIDList.removeAt(indexesToRemove.at(i));
        thePacketSupplyTypeList.removeAt(indexesToRemove.at(i));
    }


    return 1;
}
