#include "RGivenIntensityModel.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include <math.h>

#include "RMainWindow.h"


RGivenIntensityModel::RGivenIntensityModel(QObject *parent, QString name)
    : REarthquakeBaseClass(parent, name)
{
    thePeriod = 0.0;
    theResponseType = RGivenIntensityModel::PGA;


    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RGivenIntensityModel::~RGivenIntensityModel()
{
}

int RGivenIntensityModel::resetTime()
{
    return 0;
}


QString RGivenIntensityModel::getMagnitudeList()
{
    return parameterListToString(theMagnitudeList);
}

void RGivenIntensityModel::setMagnitudeList(QString value)
{
    theMagnitudeList = stringToParameterList(value);
}


QString RGivenIntensityModel::getHypocentreList()
{
    return parameterListToString(theHypocentreList);
}

void RGivenIntensityModel::setHypocentreList(QString value)
{
    theHypocentreList = stringToParameterList(value);

    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(theHypocentreList);
}


bool RGivenIntensityModel::isOn()
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


double RGivenIntensityModel::getFixedIntensity()
{
    return theGivenIntensity;
}


void RGivenIntensityModel::setFixedIntensity(double value)
{
    theGivenIntensity = value;
}


int RGivenIntensityModel::evaluateModel()
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


    // Give as response the shaking intensity at the site provided as an attribute of the location
    // theResponse->setCurrentValue(theLocation->getShakingIntensity());

    return 1;
}


