#ifndef RHAZUSPopulationDisplacementEstimator_H
#define RHAZUSPopulationDisplacementEstimator_H

#include "RPopulationDisplacementEstimator.h"
#include <math.h>

class RParameter;
class RResponse;

class RHAZUSPopulationDisplacementEstimator : public RPopulationDisplacementEstimator
{

    Q_OBJECT

public:

    RHAZUSPopulationDisplacementEstimator(QObject *parent, QString name);
    ~RHAZUSPopulationDisplacementEstimator();


    bool evaluateDisplacementStatus();
    QString evaluateShortTermDestination();
    double evaluateDisplacementDuration();
    // double evaluateWillingnessToRelocate(QString theCurrentLivingConditions, QString theHousingOwnership, bool theInsuranceStatus, double theAvailableFunds, bool theHouseholdPredispositionToLeave, bool theHouseholdFatalityIncidence, double theApparentDamage, double theSnowballFactor, double timeOfEvent);
    double evaluateUtility();

protected:

private:


};

#endif
