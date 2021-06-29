#ifndef RPopulationDisplacementEstimator_H
#define RPopulationDisplacementEstimator_H

#include "RMethod.h"
#include <math.h>

class RParameter;
class RResponse;

class RPopulationDisplacementEstimator : public RObject
{

    Q_OBJECT


public:

    RPopulationDisplacementEstimator(QObject *parent, QString name);
    ~RPopulationDisplacementEstimator();

    void setAvailabilityOfGenerator(bool value);
    void setAvailabilityOfBottledWater(bool value);
    void setDaysWithoutPower(double value);
    void setDaysWithoutWater(double value);
    void setAverageDamageState(double value);
    void setBuildingOccupancy(QString value);
    void setDS(int value);
    void setSize(int value);
    void setTenure(QString value);
    void setAge(QString value);
    void setWeatherConditions(QString value);
    void setIncome(QString value);
    void setRace(QString value);
    void setImmigrationStatus(QString value);
    void setCarOwnership(bool value);
    void setInsuranceStatus(bool value);
    void setRecentMoverStatus(bool value);
    void setDisabilityStatus(bool value);
    void setHigherEdStatus(bool value);
    void setDisasterDestination(QString value);
    void setUnsafeHousingStatus (bool value);
    void setUnaffordableHousingStatus (bool value);
    void setOvercrowdedHousingStatus (bool value);
    void setUnsatisfactionWithNeighboorhoodStatus (bool value);
    void setFatalityIncidence(bool value);
    void setSnowballFactor(double value);
    void setPercentageGone(double value);
    void setTimeSinceTheEvent(double value);
    void setCurrentLivingConditions(QString value);
    void setRepairCost(double value);
    void setLandValue(double value);
    void setBuildingValue(double value);
    void setSellingFee(double value);
    void setAverageRent(double value);
    void setAvailableFunds(double value);

    QString getReasonForDisplacement();
    virtual bool evaluateDisplacementStatus();
    virtual QString evaluateShortTermDestination();
    virtual double evaluateDisplacementDuration();
    virtual QString evaluateLongTermDestination();
    virtual bool evaluateWillingnessToSellOfLandlords();
    virtual QString evaluateDecisionOfOccupants();
    virtual QString evaluateSatisfaction();

    QString theReasonForDisplacement;

protected:

    int theDS;
    int theSize;

    bool theAvailabilityOfGenerator;
    bool theAvailabilityOfWater;
    bool theRecentMoverStatus;
    bool theDisabilityStatus;
    bool theHigherEdStatus;
    bool theCarOwnership;
    bool theInsuranceStatus;
    bool theUnsafeHousingStatus;
    bool theUnaffordableHousingStatus;
    bool theOvercrowdedHousingStatus;
    bool theUnsatisfactionWithNeighborhoodStatus;
    bool theHouseholdFatalityIncidence;
    bool theWillingnessToSell;
    bool theWillingnessToLeave;
    QString theDisasterDestination;
    QString theDecisionOfOccupants;

    double thePowerShortageDuration;
    double theWaterShortageDuration;
    double theAverageDamageState;
    double theSnowballFactor;
    double theTimeSinceTheEvent;
    double theRepairCost;
    double theInsurancePayment;
    double theLandValue;
    double theBuildingValue;
    double theSellingFee;
    double theAverageRent;
    double theAvailableFunds;

    QString theTenure;
    QString theAge;
    QString theWeatherConditions;
    QString theIncome;
    QString theBuildingOccupancy;
    QString theRace;
    QString theImmigrationStatus;
    QString theCurrentLivingConditions;



private:


};

#endif
