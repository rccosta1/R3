#ifndef RPoissonPointProcessOccurrenceModel_H
#define RPoissonPointProcessOccurrenceModel_H

#include "RModel.h"
#include "RResponse.h"
#include "ROccurrenceModel.h"

class RParameter;
class RResponse;

#ifdef NETWORKS
#include "RPacket.h"
#endif

class RPoissonPointProcessOccurrenceModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QObject *EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString ReturnPeriodInYears READ getReturnPeriodInYears WRITE setReturnPeriodInYears)

public:

    RPoissonPointProcessOccurrenceModel(QObject *parent, QString name);
    ~RPoissonPointProcessOccurrenceModel();

    QString getReturnPeriodInYears();
    void setReturnPeriodInYears(QString value);
    double getReturnPeriodInYearsValue();

    QObject *getStartTime() const;
    void setStartTime(QObject *value);

    QObject *getEndTime() const;
    void setEndTime(QObject *value);

    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);
    int resetTime();
    int evaluateModel();

private:

    int seed = 0;
    QList<QPair<double, QString> > occurrenceTimes;

    QPointer<RParameter> theReturnPeriodInYearsParameter;
    double theReturnPeriodInYearsDouble;
    RResponse *theResponse;

    QPointer<RParameter> theStartTime;
    QPointer<RParameter> theEndTime;
};

#endif
