#ifndef RSineLoadModel_H
#define RSineLoadModel_H

#include "RModel.h"
#include "RTimeSeriesResponse.h"

class RSineLoadModel : public RModel
{
	Q_OBJECT

    Q_PROPERTY(QString LoadAmplitude READ getLoadAmplitude WRITE setLoadAmplitude)
    Q_PROPERTY(QString StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QString EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString Period READ getPeriod WRITE setPeriod)

public:

    RSineLoadModel(QObject *parent, QString name);
    ~RSineLoadModel();


    QString getLoadAmplitude();
    void setLoadAmplitude(QString value);
    double getLoadAmplitudeValue();

    QString getStartTime();
    void setStartTime(QString value);
    double getStartTimeValue();

    QString getEndTime();
    void setEndTime(QString value);
    double getEndTimeValue();

    QString getPeriod();
    void setPeriod(QString value);
    double getPeriodValue();

    QStringList getActionList();

    int evaluateModel();

private:

    QPointer<RParameter> theLoadAmplitudeParameter;
    double theLoadAmplitudeDouble;

    RResponse *theTimeSeriesResponse;

    QPointer<RParameter> theStartTimeParameter;
    double theStartTimeDouble;

    QPointer<RParameter> theEndTimeParameter;
    double theEndTimeDouble;

    QPointer<RParameter> thePeriodParameter;
    double thePeriodDouble;

};

#endif
