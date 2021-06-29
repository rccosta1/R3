#ifndef RFragilityFunctions_H
#define RFragilityFunctions_H

#include "RMethod.h"
#include "RDomain.h"
#include "RTime.h"

class RFragilityFunctions : public RMethod
{

public:
    RFragilityFunctions(RDomain *theDomain);
    ~RFragilityFunctions();


    QVector<double> getDamageProbabilities(double PGAintensity, double PGDIntensity);
    QVector<double> getRestorationPercentages(double timeOfEvent);
    QVector<double> getRestorationPercentagesGivenDelta(double delta);
    double getTimeUntilGivenRestorationPercentage(double desiredRestorationPercentage, QVector<double> damageProbabilities, double timeOfEvent);
    double getExpectedRepairTime(double desiredPercentage, QVector<double> damageProbabilities);


    int setPGADamageMediansAndDispersions(QVector<double> medians, QVector<double> dispersions);
    int setPGDDamageMediansAndDispersions(QVector<double> medians, QVector<double> dispersions);
    int setRestorationMediansAndStandardDeviations(QVector<double> means, QVector<double> standardDeviations);

private:

    double calculateMean(QVector<double> probabilities, QVector<double> states);



    QVector<double> thePGADamageMedians;
    QVector<double> thePGADamageDispersions;
    QVector<double> thePGDDamageMedians;
    QVector<double> thePGDDamageDispersions;

    QVector<double> theDamageRatios;

    QVector<double> theRepairTimeMedians;
    QVector<double> theRepairTimeStandardDeviations;

    RModel *theModel;

};

#endif
