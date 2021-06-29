#include "RJoynerBoore1981IntensityModel.h"	

#include "RParameter.h"	
#include "RResponse.h"	
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RMainWindow.h"

#include <math.h>

RJoynerBoore1981IntensityModel::RJoynerBoore1981IntensityModel(QObject *parent, QString name)	
    : REarthquakeBaseClass(parent, name)
{	
    // Initializing the pointer data members
    theMagnitude = 0;
    theAlpha = 0;
    theBeta = 0;
    theB = 0;
    theEpsilon = 0;
    theHypocentreLocation = 0;

    // Create the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}	

RJoynerBoore1981IntensityModel::~RJoynerBoore1981IntensityModel()	
{	

}

QObject * RJoynerBoore1981IntensityModel::getMagnitude() const	
{	
    return theMagnitude;
}	

void RJoynerBoore1981IntensityModel::setMagnitude(QObject *value)	
{	
    theMagnitude = qobject_cast<RParameter *>(value);
}

QObject * RJoynerBoore1981IntensityModel::getAlpha() const	
{	
    return theAlpha;
}	

void RJoynerBoore1981IntensityModel::setAlpha(QObject *value)	
{	
    theAlpha = qobject_cast<RParameter *>(value);
}

QObject * RJoynerBoore1981IntensityModel::getBeta() const	
{	
    return theBeta;
}	

void RJoynerBoore1981IntensityModel::setBeta(QObject *value)	
{	
    theBeta = qobject_cast<RParameter *>(value);
}

QObject * RJoynerBoore1981IntensityModel::getB() const	
{	
    return theB;
}	

void RJoynerBoore1981IntensityModel::setB(QObject *value)	
{	
    theB = qobject_cast<RParameter *>(value);
}

QObject * RJoynerBoore1981IntensityModel::getEpsilon() const	
{	
    return theEpsilon;
}	

void RJoynerBoore1981IntensityModel::setEpsilon(QObject *value)	
{	
    theEpsilon = qobject_cast<RParameter *>(value);
}

QObject * RJoynerBoore1981IntensityModel::getHypocentreLocation() const
{	
    return theHypocentreLocation;
}	

void RJoynerBoore1981IntensityModel::setHypocentreLocation(QObject *value)
{	
    theHypocentreLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theHypocentreLocation);
}	

bool RJoynerBoore1981IntensityModel::isOn()
{
    double M = theMagnitude->getCurrentValue();

    if (M != 0.0) {
        return true;
    }
    else {
        return false;
    }
}


int RJoynerBoore1981IntensityModel::evaluateModel()
{
    // NOTE: IN THIS MODEL, h IS NOT DEPTH. IT'S A PARAMETER EQUAL TO 7.3. THIS SHOULD BE FIXED.

    // First check that we a location where the intensity shall be computed
    if (!theLocation) {
        theResponse->setCurrentValue(0.0);
        return 1;
    }

    // Also check for magnitude
    if (!isOn()) {
        theResponse->setCurrentValue(0.0);
        return 0.0;
    }


    // For convenice, look up the values of the parameters first
    double M = theMagnitude->getCurrentValue();
    double alpha = -1.02 * theAlpha->getCurrentValue();
    double beta = 0.249 * theBeta->getCurrentValue();
    double b = -0.00255 * theB->getCurrentValue();
    double epsilon = theEpsilon->getCurrentValue();

    // Verification that the user has given an RLocationResponse
    RLocation *theHypocentreLocationObject = parameterToLocation(theHypocentreLocation);
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theHypocentreLocation->objectName() << "does not include any Location object.";
        return -1;
    }
    double alt = theHypocentreLocationObject->getAltitude();
    double h = qAbs(alt);


    // Calculate distance R
    double R = theHypocentreLocationObject->computeSurfaceDistance(theLocation);

    // Calulate the Joyner-Boore R
    double r = sqrt(R * R + h * h);

    double logPGA = alpha + beta * M - log10(r) + b * r + epsilon * 0.26;//???????????????????????
    double PGA = pow(10.0, logPGA);


    if (theGradientType >= RGradientType::DDM) {
        double log_10 = log(10.0);

        double dPGA_dAlpha = -1.02 * PGA * log_10;
        double dPGA_dBeta = 0.249 * M * PGA * log_10;
        double dPGA_dM = beta * PGA * log_10;
        double dPGA_dEpsilon = 0.26 * PGA * log_10;
        double dPGA_db = -0.00255 * r * PGA * log_10;

        double dPGA_dr = (-1.0 / r + b * log_10) * PGA;
        double dr_dR = R / sqrt(R * R + h * h);
        double dr_dh = h / sqrt(R * R + h * h);

        double dR_dLat;
        double dR_dLng;
        theHypocentreLocationObject->computeSurfaceDistanceGradient(theLocation, &dR_dLat, &dR_dLng);
        double dPGA_dLat = dPGA_dr * dr_dR * dR_dLat;
        double dPGA_dLng = dPGA_dr * dr_dR * dR_dLng;

        double dh_dAlt = alt > 0 ? 1 : (alt < 0 ? -1 : 0); // signum function
        double dPGA_dAlt = dPGA_dr * dr_dh * dh_dAlt;

        QMap<RParameter *, double> thePGAGradientMap;
        thePGAGradientMap.insertMulti(theMagnitude, dPGA_dM);
        thePGAGradientMap.insertMulti(theAlpha, dPGA_dAlpha);
        thePGAGradientMap.insertMulti(theBeta, dPGA_dBeta);
        thePGAGradientMap.insertMulti(theB, dPGA_db);
        thePGAGradientMap.insertMulti(theEpsilon, dPGA_dEpsilon);
        thePGAGradientMap.insertMulti(theHypocentreLocation, dPGA_dLat);
        thePGAGradientMap.insertMulti(theHypocentreLocation, dPGA_dLng);
        thePGAGradientMap.insertMulti(theHypocentreLocation, dPGA_dAlt);

        theDDMMap.insertMulti(theResponse, thePGAGradientMap);
    }

    theResponse->setCurrentValue(PGA);

    return 1;
}
