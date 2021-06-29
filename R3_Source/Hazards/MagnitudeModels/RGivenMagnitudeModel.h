#ifndef RGivenMagnitudeModel_H
#define RGivenMagnitudeModel_H

#include "RProbabilityDistributions.h"
#include "RModel.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;

class RGivenMagnitudeModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString Magnitude READ getMagnitude WRITE setMagnitude)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:
    RGivenMagnitudeModel(QObject *parent, QString name);
    ~RGivenMagnitudeModel();

    QString getMagnitude();
    double getMagnitudeValue();
    void setMagnitude(QString value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

    int evaluateModel();

private:

    QPointer<RParameter> theMagnitudeParameter = nullptr;
    double theMagnitudeDouble;
    QPointer<RParameter> theOccurrence = nullptr;
    RResponse *theMagnitudeResponse = nullptr;
};

#endif
