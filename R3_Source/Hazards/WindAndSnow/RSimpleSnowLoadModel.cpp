#include "RSimpleSnowLoadModel.h"

#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RTime.h"

#include <math.h>

RSimpleSnowLoadModel::RSimpleSnowLoadModel(QObject *parent, QString name)
    : RSnowBaseClass(parent, name)
{
    // Instantiating the responses
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theSnowPressure = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theSnowPressure->setModel(this);
}

RSimpleSnowLoadModel::~RSimpleSnowLoadModel()
{
}



QObject * RSimpleSnowLoadModel::getSnowStormMagnitude() const
{
    return theSnowStormMagnitude;
}

void RSimpleSnowLoadModel::setSnowStormMagnitude(QObject *value)
{
    theSnowStormMagnitude = qobject_cast<RParameter *>(value);
}



bool RSimpleSnowLoadModel::isOn()
{
    double M = theSnowStormMagnitude->getCurrentValue();

    if (M != 0.0) {
        return true;
    }
    else {
        return false;
    }
}




int RSimpleSnowLoadModel::evaluateModel()
{
    // Exit quickly if this hazard isn't on
    if (!isOn()) {
        theSnowPressure->setCurrentValue(0.0);
        return 0;
    }

    double depth = theSnowStormMagnitude->getCurrentValue();

    // Density in kg/m^3 (should obviously create a random variable, or even a model, for this quantity)
    double density = 800.0;

    double load = depth * density*9.81;  // in N/m2 or Pa

    theSnowPressure->setCurrentValue(load);

    if (theOutputLevel >= RObject::Medium) {

        qDebug() << "An snowstorm just happened with magnitude" << load << " at time " << theCurrentTime->getTime();
    }

    if (theGradientType >= RGradientType::DDM)
    {
        qDebug() << "Quickly implement DDM in" << this->objectName() << "here... :)";
    }

    return 0;
}
