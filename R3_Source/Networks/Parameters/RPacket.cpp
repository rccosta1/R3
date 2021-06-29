#include "RPacket.h"
#include "RMainWindow.h"
#include "RModel.h"

RPacket::RPacket(QObject *parent, QString name)
    : RParameter(parent, name)
{
    theNextTravelStep = 0;
    setNextModel = false;
    thePacketCapacity = 0.0;
    thePacketTravelVelocity = 0.0;

    //    qCritical() << "a new packet was created";
}

RPacket::~RPacket()
{

}


QString RPacket::getPacketList()
{
    return parameterListToString(thePacketList);
}

void RPacket::setPacketList(QString value)
{
    thePacketList = stringToParameterList(value);

    // Checking if all the passed parameters are either RPacket or RPacketResponse
    parameterListToPacketList(thePacketList);
}


RModel * RPacket::getNextTravelStep()
{
    return theNextTravelStep;
}

void RPacket::setNextTravelStep(RModel *currentModel)
{
    int theCurrentModelIndex = myDestinationList.indexOf(currentModel);

    if (theCurrentModelIndex >= myDestinationList.size() - 1){
       // qCritical() << "The current model index is greater than the size of the destination list!";
    }
    else {

        theNextTravelStep = myDestinationList.at(theCurrentModelIndex + 1);
    }

}





QVector<double> RPacket::getPacketContents() const
{
    return thePacketContents;
}

void RPacket::setPacketContents(double packetContents)
{
    thePacketContents << packetContents;
}


QVector<int> RPacket::getPacketOwner() const
{
    return thePacketOwnerID;
}

void RPacket::setPacketOwner(int packetContents)
{
    thePacketOwnerID << packetContents;
}


void RPacket::setPacketContentsType(QString theType)
{
    thePacketContentsType = theType;
}

QString RPacket::getPacketContentsType()
{
    return thePacketContentsType;
}

double RPacket::getPacketCapacity()
{
    return thePacketCapacity;
}

void RPacket::setPacketCapacity(double value)
{
    thePacketCapacity = value;
}



double RPacket::getPacketVelocity()
{
    return thePacketTravelVelocity;
}

void RPacket::setPacketVelocity(double value)
{
    thePacketTravelVelocity = value;
}


QStringList RPacket::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


void RPacket::setDestinationList(QVector<RModel *> theList)
{
    myDestinationList.clear();
    myDestinationList = theList;
}

void RPacket::setDestinationList(RModel *theModel)
{
    myDestinationList.clear();
    myDestinationList << theModel;
}

QVector<RModel *> RPacket::getDestinationList()
{
    return myDestinationList;
}



