#ifndef RTime_H
#define RTime_H

#include "RParameter.h"
#include <QDateTime>

class RTime : public RParameter
{
    Q_OBJECT

    // Current realization of the time in terms of days relative to the reference time
    Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue)

    // Current realization of time in the format of "yyyy-MM-dd hh:mm:ss.zzz"
    Q_PROPERTY(QString Time READ getTime WRITE setTime)

public:

    RTime(QObject *parent, QString name);
    ~RTime();

    void setCurrentValue(double value);

    QString getTime() const;
    void setTime(QString value);

    int getYear() const;
    int getMonth() const;
    int getDay() const;

    // Returns the format of the date and time, Currently, "yyyy-MM-dd hh:mm:ss.zzz" (zzz=milliseconds)
    static QString getTimeFormat();


private:

    void updateTime();
    void updateValue();

    QDateTime theTime;
    QDateTime theReferenceTime;
    QDateTime constTime;
    QString format;

};

#endif
