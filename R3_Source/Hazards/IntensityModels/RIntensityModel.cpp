#include "RIntensityModel.h"

#include "RParameter.h"
#include "RLocation.h"

RIntensityModel::RIntensityModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    thePeriod = 0;
    theLocation = 0;
}

RIntensityModel::~RIntensityModel()
{
}

int RIntensityModel::resetTime()
{
    theRjb = 0.0;
    return 0;
}

void RIntensityModel::setIntensityMeasure(QString theIntensityMeasure)
{
    if (theIntensityMeasure == QString("PGA")) {

        theResponseType = PGA;
    }
    else if (theIntensityMeasure == QString("Sa")) {

        theResponseType = Sa;
    }
    else if (theIntensityMeasure == QString("PGV")) {

        theResponseType = PGV;
    }
    else if (theIntensityMeasure == QString("PGD")) {

        theResponseType = PGD;
    }
    else {

        qCritical() << this->objectName() << "does not understand the intensity measure" << theIntensityMeasure;
    }
}


QObject * RIntensityModel::getLocation()
{
    return theLocation;
}

void RIntensityModel::setLocation(QObject *value)
{
    // First we need to cast from QObject to RParameter...
    RParameter *theParameter = qobject_cast<RParameter *>(value);

    if (!theParameter) {
        qCritical() << "setLocation in" << this->objectName() << "could not cast the input to an RParameter";
        return;
    }

    // Next, set it as a map location
    theLocation = parameterToLocation(theParameter);
}


double RIntensityModel::getRjb()
{
    return theRjb;
}


void RIntensityModel::setPeriod(double value)
{
    thePeriod = value;
}

