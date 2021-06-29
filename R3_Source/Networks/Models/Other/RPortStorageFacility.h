#ifndef RPORTSTORAGEFACILITY_H
#define RPORTSTORAGEFACILITY_H

#include "RModel.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RFixedLocationNetworkModel.h"
#include "QVector"

class RParameter;
class RResponse;

class RPortStorageFacility :  public RFixedLocationNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(double MeanRecordedDamageRatio READ getMeanRecordedDamageRatio)
    Q_PROPERTY(double MeanRecordedTimeUntilRecovery READ getMeanRecordedTimeUntilRecovery)

public:

    RPortStorageFacility(QObject *parent, QString name);
    ~RPortStorageFacility();

    double getMeanRecordedDamageRatio() const;
    void setMeanRecordedDamageRatio(double value);

    double getMeanRecordedTimeUntilRecovery() const;
    void setMeanRecordedTimeUntilRecovery(double value);

    int evaluateModel(RGradientType theGradientType);

};

#endif // RPORTSTORAGEFACILITY_H
