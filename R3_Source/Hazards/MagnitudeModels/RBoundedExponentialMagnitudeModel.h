#ifndef RBoundedExponentialMagnitudeModel_H
#define RBoundedExponentialMagnitudeModel_H

#include "RProbabilityDistributions.h"
#include "RModel.h"
#include "RContinuousRandomVariable.h"


class RParameter;
class RResponse;

class RBoundedExponentialMagnitudeModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *MinimumMagnitude READ getMinimumMagnitude WRITE setMinimumMagnitude)
    Q_PROPERTY(QObject *MaximumMagnitude READ getMaximumMagnitude WRITE setMaximumMagnitude)
    Q_PROPERTY(QObject *Beta READ getBeta WRITE setBeta)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)


public:
    RBoundedExponentialMagnitudeModel(QObject *parent, QString name);
    ~RBoundedExponentialMagnitudeModel();

    QObject *getMinimumMagnitude() const;
    void setMinimumMagnitude(QObject *value);

    QObject *getMaximumMagnitude() const;
    void setMaximumMagnitude(QObject *value);

    QObject *getBeta() const;
    void setBeta(QObject *value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);


    // Remove after paper
    double getMagnitudeForThisScenario();

    QList<RParameter *> getPhysicalParameterList();

    int evaluateModel();

private:
    QPointer<RParameter> theMinimumMagnitude;
    QPointer<RParameter> theMaximumMagnitude;
    QPointer<RParameter> theBeta;
    QPointer<RContinuousRandomVariable> theTheta;
    QPointer<RParameter> theOccurrence;

    RResponse *theMagnitude;
    double theScenarioMagnitude;
};

#endif
