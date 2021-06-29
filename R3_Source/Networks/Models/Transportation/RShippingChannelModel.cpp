#include "RShippingChannelModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RBridgeModel.h"
#include "RLocation.h"
#include <math.h>

RShippingChannelModel::RShippingChannelModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RShippingChannelModel::~RShippingChannelModel()
{
}


int RShippingChannelModel::resetTime()
{
    timePacketsCurrentlyOnboardWereReceived.clear();
    return 0;
}


QString RShippingChannelModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}

void RShippingChannelModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QObject * RShippingChannelModel::getFromLocation() const
{
    return theFromLocation;
}


void RShippingChannelModel::setFromLocation(QObject *value)
{
    theFromLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theFromLocation);
}



QObject * RShippingChannelModel::getToLocation() const
{
    return theToLocation;
}

void RShippingChannelModel::setToLocation(QObject *value)
{
    theToLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theToLocation);
}


RShippingChannelModel::RChannelType RShippingChannelModel::getChannelType() const
{
    return theChannelType;
}

void RShippingChannelModel::setChannelType(RShippingChannelModel::RChannelType value)
{
    theChannelType = value;


        thePGADamageMedians.clear();
        thePGADamageDispersions.clear();
        thePGDDamageMedians.clear();
        thePGDDamageDispersions.clear();
        theRepairTimeMedians.clear();
        theRepairTimeStandardDeviations.clear();

        // Placeholders
        theRepairTimeMedians << 0.9 << 1.9 << 32.0 << 95.0;
        theRepairTimeStandardDeviations << 0.3 << 1.2 << 31.0 << 65.0;

        // Placeholders
        if (value == 0){
            thePGADamageMedians << 10.00 << 10.00 << 10.00 << 10.00;
            thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

            thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
            thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        }

        // Placeholders
        else if (value == 1){
            thePGADamageMedians << 10.00 << 10.00 << 10.00 << 10.00;
            thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

            thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
            thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
        }

        else {

            qCritical() << " Invalid option for " << this->objectName();
        }


}


QString RShippingChannelModel::getBottleneckList()
{
    return parameterListToString(theBottleneckList);
}

void RShippingChannelModel::setBottleneckList(QString value)
{
    theBottleneckList = stringToParameterList(value);
}


QString RShippingChannelModel::getPGDIntensityList()
{
    return parameterListToString(thePGDIntensityList);
}

void RShippingChannelModel::setPGDIntensityList(QString value)
{
    thePGDIntensityList = stringToParameterList(value);
}


int RShippingChannelModel::evaluateModel()
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


    for (int i = 0; i < thePGDIntensityList.size(); i++){

        // get the PGD response getting to the model
        RResponse *thePGDResponse = qobject_cast<RResponse *>(thePGDIntensityList.at(i));


        // do something with the Landslide PGD
        // ****
        // ****
        // ****
    }




    // Load the packets received
    int numPackets = 0;
    int newPackets = packetsCurrentlyOnboard.size();
    this->loadPackets(thePacketList);
    numPackets = packetsCurrentlyOnboard.size();
    newPackets = numPackets - newPackets;

    if (newPackets > 0){
        for (int i = 0; i < newPackets; i++){
            timePacketsCurrentlyOnboardWereReceived << theCurrentTime->getCurrentValue();
        }
    }



    for (int i = 0; i < timePacketsCurrentlyOnboardWereReceived.size(); i++){
        int ii = timePacketsCurrentlyOnboardWereReceived.size() - (1 + i);

        // Get the distance between the start and end point of this road
        double EarthRadius = 6372.796924;

        double lat1 = (qobject_cast<RLocation *>(theFromLocation))->getLatitude() / 180.0 * 3.14159265359;
        double lng1 = (qobject_cast<RLocation *>(theFromLocation))->getLongitude() / 180.0 * 3.14159265359;

        double lat2 = (qobject_cast<RLocation *>(theToLocation))->getLatitude() / 180.0 * 3.14159265359;
        double lng2 = (qobject_cast<RLocation *>(theToLocation))->getLongitude() / 180.0 * 3.14159265359;

        double roadLength = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng2 - lng1)) * EarthRadius;

        double theVelocity = packetsCurrentlyOnboard.at(ii)->getPacketVelocity();


                if (roadLength < theVelocity * (theCurrentTime->getCurrentValue() - timePacketsCurrentlyOnboardWereReceived.at(ii)) * 24.0){
                    timePacketsCurrentlyOnboardWereReceived.removeAt(ii);
                    unloadPackets(qobject_cast<RPacketResponse *>(theResponse),packetsCurrentlyOnboard);
                }

    }


    return 1;
}








