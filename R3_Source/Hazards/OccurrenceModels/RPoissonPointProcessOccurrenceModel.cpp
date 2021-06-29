#include "RPoissonPointProcessOccurrenceModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include <math.h>

RPoissonPointProcessOccurrenceModel::RPoissonPointProcessOccurrenceModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theStartTime = 0;
    theEndTime = 0;

    theReturnPeriodInYearsParameter = 0;
    theReturnPeriodInYearsDouble = 0.0;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RPoissonPointProcessOccurrenceModel::~RPoissonPointProcessOccurrenceModel()
{

}


QObject * RPoissonPointProcessOccurrenceModel::getStartTime() const
{
    return theStartTime;
}

void RPoissonPointProcessOccurrenceModel::setStartTime(QObject *value)
{
    theStartTime = qobject_cast<RParameter *>(value);
    resetTime();
}




QObject * RPoissonPointProcessOccurrenceModel::getEndTime() const
{
    return theEndTime;
}

void RPoissonPointProcessOccurrenceModel::setEndTime(QObject *value)
{
    theEndTime = qobject_cast<RParameter *>(value);
    resetTime();
}


QString RPoissonPointProcessOccurrenceModel::getReturnPeriodInYears()
{
    return getStringParameter(&theReturnPeriodInYearsDouble, theReturnPeriodInYearsParameter);
}
void RPoissonPointProcessOccurrenceModel::setReturnPeriodInYears(QString value)
{
    setStringParameter(value, &theReturnPeriodInYearsDouble, &theReturnPeriodInYearsParameter);
    resetTime();
}
double RPoissonPointProcessOccurrenceModel::getReturnPeriodInYearsValue()
{
    return getDoubleParameter(&theReturnPeriodInYearsDouble, theReturnPeriodInYearsParameter);
}



QList<QPair<double, QString> > RPoissonPointProcessOccurrenceModel::getTriggerTimes(double startTime, double endTime)
{
    return occurrenceTimes;
}


int RPoissonPointProcessOccurrenceModel::evaluateModel()
{	
    // If any of the occurrence times matches the current time, then say the hazard is "on"
    for (int i=0; i<occurrenceTimes.size(); i++) {

        if (fabs(occurrenceTimes.at(i).first - theCurrentTime->getCurrentValue()) < 1.0/(24.0*60.0)) {

            theResponse->setCurrentValue(1.0);

            return 0;
        }
    }

    // No match, so hazard is "off"
    theResponse->setCurrentValue(0.0);

    return 0;
}


int RPoissonPointProcessOccurrenceModel::resetTime()
{
    // Clear whatever was set before
    occurrenceTimes.clear();

    // Only proceed if we have start time, end time, and return period
    if (theStartTime != 0 && theEndTime != 0 && (theReturnPeriodInYearsParameter != 0 || theReturnPeriodInYearsDouble != 0.0) ) {

        if (getReturnPeriodInYearsValue() <= 0.0) {

            qCritical() << "Error: Negative or zero return period in" << this->objectName();
            return 0;
        }

        // Get start time and end time as doubles rather than time objects
        double startTime = theStartTime->getCurrentValue();
        double endTime = theEndTime->getCurrentValue();
        double lambda = 1.0 / (getReturnPeriodInYearsValue() * 365.0); // Notice conversion to days

        // Initial declarations
        QString thisEventName = this->objectName();
        double aGeneratedTime = startTime;
        double timeIncrement;
        int randomNumberBetween0AndRAND_MAX;
        double randomNumberBetween0And1;

        // Set seed for random number generator, incremented by one, in perpetuity, and generate & discard one number
        srand(seed++);
        rand();

        // Generate outcomes of trigger times, from start time until end time
        while (aGeneratedTime <= endTime) {

            // Generate a number between 0 and RAND_MAX
            randomNumberBetween0AndRAND_MAX = rand();

            // Modify it so that the value lies between 0 and 1
            randomNumberBetween0And1 = (double)randomNumberBetween0AndRAND_MAX/RAND_MAX;

            // Treat two special cases
            if (randomNumberBetween0And1 == 0.0) {
                randomNumberBetween0And1 = 0.0000001;
            }
            if (randomNumberBetween0And1 == 1.0) {
                randomNumberBetween0And1 = 0.9999999;
            }

            // Transform that number into an exponential variable using Finv=-ln(1-p)/lambda and F(x)=x for x~UN(0,1)
            timeIncrement = -log(1.0-randomNumberBetween0And1)/lambda;

            // Set the newly generated time
            aGeneratedTime = aGeneratedTime + timeIncrement;

            // Append it to the list of trigger times, unless we just passed the end time
            if (aGeneratedTime < endTime) {
                occurrenceTimes.append(qMakePair(aGeneratedTime, thisEventName));
            }
        }
    }

    return 0;
}
