#include "RFixedTimeOccurrenceModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include <math.h>

RFixedTimeOccurrenceModel::RFixedTimeOccurrenceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theOccurrenceTime = 0;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RFixedTimeOccurrenceModel::~RFixedTimeOccurrenceModel()
{

}


QObject * RFixedTimeOccurrenceModel::getOccurrenceTime() const
{
    return theOccurrenceTime;
}

void RFixedTimeOccurrenceModel::setOccurrenceTime(QObject *value)
{
    theOccurrenceTime = qobject_cast<RTime *>(value);


    if (!theOccurrenceTime) {
        qCritical() << "These days the occurrence model in" << this->objectName() << "must be an RTime object.";
    }
}




QList<QPair<double, QString> > RFixedTimeOccurrenceModel::getTriggerTimes(double startTime, double endTime)
{
    // Here we don't have to return anything, because there exist a time object that will be picked up instead

    QList<QPair<double, QString> > occurrenceTimes;

    return occurrenceTimes;
}



int RFixedTimeOccurrenceModel::evaluateModel()
{	
    double occurrenceTime = theOccurrenceTime->getCurrentValue();
    double currentTime = theCurrentTime->getCurrentValue();

    if (qAbs(occurrenceTime-currentTime) < 1e-6) {

        theResponse->setCurrentValue(1.0);
    }
    else {

        theResponse->setCurrentValue(0.0);
    }

    return 1;
}

