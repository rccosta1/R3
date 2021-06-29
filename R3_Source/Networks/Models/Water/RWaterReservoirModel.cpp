#include "RWaterReservoirModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RFragilityFunctions.h"
#include <math.h>

RWaterReservoirModel::RWaterReservoirModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theCapacity = 1;

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RWaterReservoirModel::~RWaterReservoirModel()
{

}


int RWaterReservoirModel::resetTime()
{
    theResponse->setCurrentValue(0.0);
    timeOfEvent = 0.0;
    theCurrentVolume = theCapacity;
    theAvailabilityOfWater = false;

    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> thePGAs = stringToDoubleVector(qobject_cast<RLocation*>(theLocation)->getPGAIntensity());

    if (thePGACounter > (thePGAs.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough PGA values!";
        thePGACounter = 0;
    }

    thePGAIntensity = thePGAs.at(thePGACounter);
    thePGACounter++;

    return 0;
}


double RWaterReservoirModel::getCapacity() const
{
    return theCapacity;
}

void RWaterReservoirModel::setCapacity(double value)
{
    theCapacity = value;
}

QString RWaterReservoirModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterReservoirModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}

int RWaterReservoirModel::evaluateModel()
{

    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // reset the response of this model
    theResponse->setCurrentValue(0.0);
    theAvailabilityOfWater = false;


    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel == nullptr){
            qCritical() << "There is a problem with supplier list for" << this->objectName();
        }

        // If this is a continuous response - this is what the model needs to function
        if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
            theAvailabilityOfWater = true;

        } else {
            // Calculate the expected days without water
            // Daily demand: 49 gallons / day / person in SF in 2013 https://sfwater.org/modules/showdocument.aspx?documentid=6543
            // Total reservoir capacity in SF: 372,000,000 gallons (Draft report state of the LWS table 3-5)
            // SF population in 2013 840,000 persons
            // Assuming even contribution from all reservoirs, 372,000,000 / (840,000 * 49) ~ 9 days
            double theDaysWithoutWater = qMax(theResponse->getCurrentValue(), theResponseComingIntoThisModel->getCurrentValue()) - 9.0;
            theResponse->setCurrentValue(theDaysWithoutWater);
        }

    }

    lastEvaluationTime = theCurrentTime ->getCurrentValue();

    return 1;
}
