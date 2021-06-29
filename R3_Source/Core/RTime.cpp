#include "RTime.h"
#include "RMainWindow.h"
#include <math.h>

RTime::RTime(QObject *parent, QString name)
    : RParameter(parent, name)
{
    format = RTime::getTimeFormat();
    theTime = QDateTime::currentDateTime();
    constTime = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0, 0));
    theReferenceTime = QDateTime(constTime);
    updateValue();
}

RTime::~RTime()
{

}


QString RTime::getTime() const
{	
    return theTime.toString(format);
}	

void RTime::setTime(QString value)
{	
    theTime = QDateTime().fromString(value, format);
    updateValue();
}


void RTime::setCurrentValue(double value)
{
    RParameter::setCurrentValue(value);
    updateTime();
}

void RTime::updateTime()
{
    theTime = theReferenceTime.addDays(floor(theCurrentValue));
    theTime = theTime.addMSecs((theCurrentValue - floor(theCurrentValue)) * 86400000.0);
    //theTime = theTime.addMSecs(-3600000); // Terje says: When the time one hour off then this strangely makes sense...
}

void RTime::updateValue()
{
    double temp = 0;
    temp = theReferenceTime.daysTo(theTime);
    temp += theReferenceTime.time().msecsTo(theTime.time()) / 86400000.0;
    RParameter::setCurrentValue(temp);
}

int RTime::getYear() const
{
    return theTime.date().year();
}

int RTime::getMonth() const
{
    return theTime.date().month();
}

int RTime::getDay() const
{
    return theTime.date().day();
}

QString RTime::getTimeFormat()
{
    return QString("yyyy-MM-dd hh:mm:ss.zzz");
}
