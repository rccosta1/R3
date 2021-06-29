#include "RRampLoadModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RFileResponse.h"
#include "RCommandResponse.h"
#include "RTimeSeries.h"
#include <QDir>
#include <QApplication>


RRampLoadModel::RRampLoadModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    //webHelpString = "http://www.inrisk.ubc.ca/windpressure/";

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RTimeSeriesResponse");
    theTimeSeriesResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theTimeSeriesResponse->setModel(this);

    theRampStartTimeParameter = 0;
    theRampStartTimeDouble = 0.0;

    theRampEndTimeParameter = 0;
    theRampEndTimeDouble = 0.0;
}

RRampLoadModel::~RRampLoadModel()
{

}



QObject * RRampLoadModel::getLoadIntensity() const
{
    return theLoadIntensity;
}

void RRampLoadModel::setLoadIntensity(QObject *value)
{
    theLoadIntensity = qobject_cast<RParameter *>(value);
}




QString RRampLoadModel::getRampStartTime()
{
    return getStringParameter(&theRampStartTimeDouble, theRampStartTimeParameter);
}
void RRampLoadModel::setRampStartTime(QString value)
{
    setStringParameter(value, &theRampStartTimeDouble, &theRampStartTimeParameter);
}
double RRampLoadModel::getRampStartTimeValue()
{
    return getDoubleParameter(&theRampStartTimeDouble, theRampStartTimeParameter);
}



QString RRampLoadModel::getRampEndTime()
{
    return getStringParameter(&theRampEndTimeDouble, theRampEndTimeParameter);
}
void RRampLoadModel::setRampEndTime(QString value)
{
    setStringParameter(value, &theRampEndTimeDouble, &theRampEndTimeParameter);
}
double RRampLoadModel::getRampEndTimeValue()
{
    return getDoubleParameter(&theRampEndTimeDouble, theRampEndTimeParameter);
}






QStringList RRampLoadModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}





int RRampLoadModel::evaluateModel()
{	
    // Get a pointer to the response time series, and clean it
    RTimeSeries *theOutputTimeSeries = parameterToTimeSeries(theTimeSeriesResponse);
    if (!theOutputTimeSeries) {
        qCritical() << "Error: The response object" << theTimeSeriesResponse->objectName() << "does not contain a times series object.";
        return -1;
    }
    theOutputTimeSeries->clean();


    // Set a time series that ramps up from zero to max  (for now apply the load in the x-direction)
    theOutputTimeSeries->setXYZvalues(getRampStartTimeValue(), 0.0, 0.0, 0.0);
    theOutputTimeSeries->setXYZvalues(getRampEndTimeValue(), theLoadIntensity->getCurrentValue(), 0.0, 0.0);

    return 0;
}




