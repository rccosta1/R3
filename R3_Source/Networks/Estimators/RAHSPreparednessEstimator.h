#ifndef RAHSPreparednessEstimator_H
#define RAHSPreparednessEstimator_H

#include "RDisasterPreparednessEstimator.h"
#include "RContinuousRandomVariable.h"
#include <math.h>

class RDomain;
class RRandomVariable;

class RAHSPreparednessEstimator : public RDisasterPreparednessEstimator
{

    Q_OBJECT

public:

    RAHSPreparednessEstimator(QObject *parent, QString name);
    ~RAHSPreparednessEstimator();

    bool getGeneratorAvailability();
    bool getWaterAvailability();
    bool getFinancialResourceAvailability();
    QString getDisasterDestination();

protected:

private:


};

#endif
