#include "RCornell1979IntensityModel.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include <math.h>

#include "RMainWindow.h"


RCornell1979IntensityModel::RCornell1979IntensityModel(QObject *parent, QString name)
    : REarthquakeBaseClass(parent, name)
{
    thePeriod = 0.0;
    theResponseType = RCornell1979IntensityModel::PGA;


    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RCornell1979IntensityModel::~RCornell1979IntensityModel()
{
}

int RCornell1979IntensityModel::resetTime()
{
    return 0;
}


QString RCornell1979IntensityModel::getMagnitudeList()
{
    return parameterListToString(theMagnitudeList);
}

void RCornell1979IntensityModel::setMagnitudeList(QString value)
{
    theMagnitudeList = stringToParameterList(value);
}


QString RCornell1979IntensityModel::getHypocentreList()
{
    return parameterListToString(theHypocentreList);
}

void RCornell1979IntensityModel::setHypocentreList(QString value)
{
    theHypocentreList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theHypocentreList);
}


bool RCornell1979IntensityModel::isOn()
{
    //qDebug()<<"The magnitude is:"<<theMagnitudeList[0]->getCurrentValue();


    // Check the magnitude: that is the model associated with an occurrence model
    bool hereIsARealMagnitude = false;
    for (int i = 0; i < theMagnitudeList.count(); i++) {

        double M = theMagnitudeList[i]->getCurrentValue();

        if (M != 0.0) {
            hereIsARealMagnitude = true;
        }
    }

    if (hereIsARealMagnitude) {
        return true;
    }

    else {
        return false;
    }
}

int RCornell1979IntensityModel::evaluateModel()
{
    // First check that we have a location where the intensity shall be computed
    if (!theLocation) {
        theResponse->setCurrentValue(0.0);
        return 1;
    }


    // End this quickly if there is no magnitude
    if (!isOn()) {
        theResponse->setCurrentValue(0.0); // This line was not here
        return 0;
    }


    // Checking if the number of magnitudes is equal to the number of hypocentre Locations
    if (theHypocentreList.count() != theMagnitudeList.count()) {
        qCritical() << "Error: The number of magnitude parameters should be equal to the number of hypocentre Locations in the model" << objectName() << ".";
        return -1;
    }


    double T = 0.0;
    if (theResponseType == RCornell1979IntensityModel::PGA) {
        T = 0.0;
    } else if (theResponseType == RCornell1979IntensityModel::PGV) {
        T = -1.0;
    } else if (theResponseType == RCornell1979IntensityModel::Sa) {
        T = thePeriod;
    }

    // Verification that the user has given an RLocationResponse
    RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreList[0]);
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreList[0]->objectName() << "does not include any Location object.";
        return -1;
    }
    double Rjb = theHypocentreLocationObject->computeSurfaceDistance(theLocation);


    double lnY = -0.152 + 0.859 * theMagnitudeList.at(0)->getCurrentValue() - 1.803 * log(Rjb + 25.0);
    double Y = exp(lnY);


    theResponse->setCurrentValue(Y);

    return 1;
}


