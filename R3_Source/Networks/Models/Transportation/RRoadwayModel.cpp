#include "RRoadwayModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RBridgeModel.h"
#include "RLocation.h"
#include <math.h>

RRoadwayModel::RRoadwayModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RRoadwayModel::~RRoadwayModel()
{
}


int RRoadwayModel::resetTime()
{
    timePacketsCurrentlyOnboardWereReceived.clear();
    return 0;
}


QString RRoadwayModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}

void RRoadwayModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}



QObject * RRoadwayModel::getFromLocation() const
{
    return theFromLocation;
}

void RRoadwayModel::setFromLocation(QObject *value)
{
    theFromLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theFromLocation);
}



QObject * RRoadwayModel::getToLocation() const
{
    return theToLocation;
}

void RRoadwayModel::setToLocation(QObject *value)
{
    theToLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theToLocation);
}


RRoadwayModel::RRoadType RRoadwayModel::getRoadType() const
{
    return theRoadType;
}

void RRoadwayModel::setRoadType(RRoadwayModel::RRoadType value)
{
    theRoadType = value;
}


QString RRoadwayModel::getBottleneckList()
{
    return parameterListToString(theBottleneckList);
}

void RRoadwayModel::setBottleneckList(QString value)
{
    theBottleneckList = stringToParameterList(value);
}


bool RRoadwayModel::isOpen()
{
    // Initial declarations
    bool OpenRoute = true;

    // Loop over all bridges
    for (int i = 0; i < theBottleneckList.size(); i++) {

        RResponse *theBottleneckResponse = qobject_cast<RResponse *>(theBottleneckList.at(i));
        RBridgeModel *theBridgeModel = qobject_cast<RBridgeModel *>(theBottleneckResponse->getModel());

        if (theBridgeModel) {

            // Run the analysis in the bridge
            theBridgeModel->runAnalysis(RGradientType::NONE);

            if (theBridgeModel->getFunctionality() == true){
                OpenRoute = false;
            }
        }
    }

    return OpenRoute;
}



int RRoadwayModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }



    // -------------------------------------------------------------------------------------------
    // 1 - Checking responses coming to this model
    // -------------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the households in this zone have an object to request disaster aid from
    for (int i = 0; i < theInputParameterList.size(); i++){

        // Cast the input as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theInputParameterList.at(i));

        if (theResponseComingToThisModel){
            thePacketList << theInputParameterList.at(i);
        }
    }




    // Load the packets received
    int numPackets = 0;
    int newPackets = packetsCurrentlyOnboard.size();
    this->loadPackets(thePacketList);
    numPackets = packetsCurrentlyOnboard.size();
    newPackets = numPackets - newPackets;


    for (int i = 0; i < packetsCurrentlyOnboard.size();i++){
    //qCritical() << this->objectName() << packetsCurrentlyOnboard.size();
    }


    if (newPackets > 0){
        for (int i = 0; i < newPackets; i++){
            timePacketsCurrentlyOnboardWereReceived << theCurrentTime->getCurrentValue();
        }
    }


    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++){
        if (this->isOpen() == true){
            int ii = packetsCurrentlyOnboard.size() - (i + 1);

            // Get the distance between the start and end point of this road
            double EarthRadius = 6372.796924;

            double lat1 = (qobject_cast<RLocation *>(theFromLocation))->getLatitude() / 180.0 * 3.14159265359;
            double lng1 = (qobject_cast<RLocation *>(theFromLocation))->getLongitude() / 180.0 * 3.14159265359;

            double lat2 = (qobject_cast<RLocation *>(theToLocation))->getLatitude() / 180.0 * 3.14159265359;
            double lng2 = (qobject_cast<RLocation *>(theToLocation))->getLongitude() / 180.0 * 3.14159265359;

            double roadLength = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng2 - lng1)) * EarthRadius;


            double theVelocity = packetsCurrentlyOnboard.at(ii)->getPacketVelocity();


            //if (roadLength < theVelocity * (theCurrentTime->getCurrentValue() - timePacketsCurrentlyOnboardWereReceived.at(ii)) * 24.0){
            timePacketsCurrentlyOnboardWereReceived.removeAt(ii);
            unloadPackets(qobject_cast<RPacketResponse *>(theResponse),packetsCurrentlyOnboard);
            // }
        }
    }



    return 1;
}
