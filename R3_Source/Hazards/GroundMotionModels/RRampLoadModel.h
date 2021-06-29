#ifndef RRampLoadModel_H
#define RRampLoadModel_H

#include "RModel.h"
#include "RTimeSeriesResponse.h"

class RRampLoadModel : public RModel
{
	Q_OBJECT

    Q_PROPERTY(QObject *LoadIntensity READ getLoadIntensity WRITE setLoadIntensity)
    Q_PROPERTY(QString RampStartTime READ getRampStartTime WRITE setRampStartTime)
    Q_PROPERTY(QString RampEndTime READ getRampEndTime WRITE setRampEndTime)

public:

	RRampLoadModel(QObject *parent, QString name);
	~RRampLoadModel();

    QObject *getLoadIntensity() const;
    void setLoadIntensity(QObject *value);

    QString getRampStartTime();
    void setRampStartTime(QString value);
    double getRampStartTimeValue();

    QString getRampEndTime();
    void setRampEndTime(QString value);
    double getRampEndTimeValue();

    QStringList getActionList();

    int evaluateModel();

private:

    QPointer<RParameter> theLoadIntensity;

    RResponse *theTimeSeriesResponse;

    QPointer<RParameter> theRampStartTimeParameter;
    double theRampStartTimeDouble;

    QPointer<RParameter> theRampEndTimeParameter;
    double theRampEndTimeDouble;

};

#endif
