#include "RSineLoadModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RTimeSeries.h"
#include <QDir>
#include <QApplication>
#include <math.h>

RSineLoadModel::RSineLoadModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // webHelpString = "http://www.inrisk.ubc.ca/windpressure/";

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theTimeSeriesResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeSeriesResponse->setModel(this);

    theStartTimeParameter = 0;
    theStartTimeDouble = 0.0;

    theEndTimeParameter = 0;
    theEndTimeDouble = 0.0;

    thePeriodParameter = 0;
    thePeriodDouble = 0.0;

    theLoadAmplitudeParameter = 0;
    theLoadAmplitudeDouble = 0.0;

}

RSineLoadModel::~RSineLoadModel()
{

}


QString RSineLoadModel::getLoadAmplitude()
{
    return getStringParameter(&theLoadAmplitudeDouble, theLoadAmplitudeParameter);
}
void RSineLoadModel::setLoadAmplitude(QString value)
{
    setStringParameter(value, &theLoadAmplitudeDouble, &theLoadAmplitudeParameter);
}
double RSineLoadModel::getLoadAmplitudeValue()
{
    return getDoubleParameter(&theLoadAmplitudeDouble, theLoadAmplitudeParameter);
}






QString RSineLoadModel::getStartTime()
{
    return getStringParameter(&theStartTimeDouble, theStartTimeParameter);
}
void RSineLoadModel::setStartTime(QString value)
{
    setStringParameter(value, &theStartTimeDouble, &theStartTimeParameter);
}
double RSineLoadModel::getStartTimeValue()
{
    return getDoubleParameter(&theStartTimeDouble, theStartTimeParameter);
}



QString RSineLoadModel::getEndTime()
{
    return getStringParameter(&theEndTimeDouble, theEndTimeParameter);
}
void RSineLoadModel::setEndTime(QString value)
{
    setStringParameter(value, &theEndTimeDouble, &theEndTimeParameter);
}
double RSineLoadModel::getEndTimeValue()
{
    return getDoubleParameter(&theEndTimeDouble, theEndTimeParameter);
}



QString RSineLoadModel::getPeriod()
{
    return getStringParameter(&thePeriodDouble, thePeriodParameter);
}
void RSineLoadModel::setPeriod(QString value)
{
    setStringParameter(value, &thePeriodDouble, &thePeriodParameter);
}
double RSineLoadModel::getPeriodValue()
{
    return getDoubleParameter(&thePeriodDouble, thePeriodParameter);
}






QStringList RSineLoadModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}





int RSineLoadModel::evaluateModel()
{	
    // Get a pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theTimeSeriesResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theTimeSeriesResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();

    double dt = getPeriodValue() / 50.0;

    for (double time = getStartTimeValue(); time<=getEndTimeValue()+dt; time = time + dt) {

        // Set a time series that ramps up from zero to max  (for now apply the load in the x-direction)
        theOutputTimeSeries->setXYZvalues(time, getLoadAmplitudeValue() * sin(2.0*PI/getPeriodValue()*time), 0.0, 0.0);
    }

    return 0;
}




