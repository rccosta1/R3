#ifndef RStatisticsCanadaPreparednessEstimator_H
#define RStatisticsCanadaPreparednessEstimator_H

#include "RDisasterPreparednessEstimator.h"
#include "RObject.h"
#include <math.h>

class RDomain;
class RRandomVariable;

class RStatisticsCanadaPreparednessEstimator : public RDisasterPreparednessEstimator
{
    Q_OBJECT

public:

    RStatisticsCanadaPreparednessEstimator(QObject *parent, QString name);
    ~RStatisticsCanadaPreparednessEstimator();

    bool getGeneratorAvailability();
    bool getWaterAvailability();
    bool getSocialSupportAvailability();

protected:

private:


};

#endif
