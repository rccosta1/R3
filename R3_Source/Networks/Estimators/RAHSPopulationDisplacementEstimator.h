#ifndef RAHSPopulationDisplacementEstimator_H
#define RAHSPopulationDisplacementEstimator_H

#include "RPopulationDisplacementEstimator.h"
#include "RContinuousRandomVariable.h"
#include <math.h>

class RParameter;
class RResponse;

class RAHSPopulationDisplacementEstimator : public RPopulationDisplacementEstimator
{

    Q_OBJECT

public:

    RAHSPopulationDisplacementEstimator(QObject *parent, QString name);
    ~RAHSPopulationDisplacementEstimator();

    bool evaluateDisplacementStatus();
    QString evaluateShortTermDestination();
    double evaluateDisplacementDuration();
    QString evaluateLongTermDestination();


protected:

private:

};

#endif
