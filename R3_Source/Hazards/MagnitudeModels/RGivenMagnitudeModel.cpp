#include "RGivenMagnitudeModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include <math.h>

RGivenMagnitudeModel::RGivenMagnitudeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theMagnitudeDouble = 0.0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMagnitudeResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMagnitudeResponse->setModel(this);
}

RGivenMagnitudeModel::~RGivenMagnitudeModel()
{}

QString RGivenMagnitudeModel::getMagnitude()
{
    return getStringParameter(&theMagnitudeDouble, theMagnitudeParameter);
}

void RGivenMagnitudeModel::setMagnitude(QString value)
{
    setStringParameter(value, &theMagnitudeDouble, &theMagnitudeParameter);
}

double RGivenMagnitudeModel::getMagnitudeValue()
{
    return getDoubleParameter(&theMagnitudeDouble, theMagnitudeParameter);
}

QObject * RGivenMagnitudeModel::getOccurrence() const
{
    return theOccurrence;
}

void RGivenMagnitudeModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}


int RGivenMagnitudeModel::evaluateModel()
{
    if (theOccurrence->getCurrentValue() == 0.0) {

        theMagnitudeResponse->setCurrentValue(0.0);

        return 0;
    }

    theMagnitudeResponse->setCurrentValue(getMagnitudeValue());

    return 1;
}
