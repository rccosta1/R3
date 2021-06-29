#include "RPowerLineModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RPowerLineModel::RPowerLineModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RPowerLineModel::~RPowerLineModel()
{
}


int RPowerLineModel::resetTime()
{
    theResponse->setCurrentValue(0.0);
    return 0;
}


QObject * RPowerLineModel::getFromLocation() const
{
    return theFromLocation;
}

void RPowerLineModel::setFromLocation(QObject *value)
{
    theFromLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theFromLocation);
}



QObject * RPowerLineModel::getToLocation() const
{
    return theToLocation;
}

void RPowerLineModel::setToLocation(QObject *value)
{
    theToLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theToLocation);
}


QString RPowerLineModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RPowerLineModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}



int RPowerLineModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // Reseting the response
    theResponse->setCurrentValue(1.0);


    // Checking the availability of the suppliers
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // 2 - HANDLING RESPONSES COMING TO THIS MODEL
        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        if (theResponseComingIntoThisModel && theResponseComingIntoThisModel->getCurrentValue() == 0.0){

            theResponse->setCurrentValue(0.0);
        }
        else if (theResponseComingIntoThisModel->getCurrentValue() > theResponse->getCurrentValue()){
            theResponse->setCurrentValue(theResponseComingIntoThisModel->getCurrentValue());
        }
    }


    return 1;
}
