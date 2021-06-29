#include "RPopulationDisplacementEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>
#include "RScenarioModel.h"


RPopulationDisplacementEstimator::RPopulationDisplacementEstimator(QObject *parent, QString name)
    : RObject(parent, name)
{
}


RPopulationDisplacementEstimator::~RPopulationDisplacementEstimator()
{
}



void RPopulationDisplacementEstimator::setAvailabilityOfGenerator(bool value)
{
    theAvailabilityOfGenerator = value;
    // theAvailabilityOfGenerator = false;
}

void RPopulationDisplacementEstimator::setAvailabilityOfBottledWater(bool value)
{
    theAvailabilityOfWater = value;
    // theAvailabilityOfWater = false;
}

void RPopulationDisplacementEstimator::setDaysWithoutPower(double value)
{
    thePowerShortageDuration = value;
}

void RPopulationDisplacementEstimator::setDaysWithoutWater(double value)
{
    theWaterShortageDuration = value;
}

void RPopulationDisplacementEstimator::setDS(int value)
{
    theDS = value;
}

void RPopulationDisplacementEstimator::setTenure(QString value)
{
    theTenure = value;
}

void RPopulationDisplacementEstimator::setAge(QString value)
{
    theAge = value;
}

void RPopulationDisplacementEstimator::setAverageDamageState(double value)
{
    theAverageDamageState = value;
}

void RPopulationDisplacementEstimator::setWeatherConditions(QString value)
{
    theWeatherConditions = value;
}

void RPopulationDisplacementEstimator::setCarOwnership(bool value)
{
    theCarOwnership = value;
}

void RPopulationDisplacementEstimator::setInsuranceStatus(bool value)
{
    theInsuranceStatus = value;
}

void RPopulationDisplacementEstimator::setIncome(QString value)
{
    theIncome = value;
}

void RPopulationDisplacementEstimator::setSize(int value)
{
    theSize = value;
}


void RPopulationDisplacementEstimator::setBuildingOccupancy(QString value)
{
    theBuildingOccupancy = value;
}

void RPopulationDisplacementEstimator::setRace(QString value)
{
    theRace = value;
}

void RPopulationDisplacementEstimator::setImmigrationStatus(QString value)
{
    theImmigrationStatus = value;
}

void RPopulationDisplacementEstimator::setRecentMoverStatus(bool value)
{
    theRecentMoverStatus = value;
}

void RPopulationDisplacementEstimator::setDisabilityStatus(bool value)
{
    theDisabilityStatus = value;
}

void RPopulationDisplacementEstimator::setHigherEdStatus(bool value)
{
    theHigherEdStatus = value;
}


void RPopulationDisplacementEstimator::setDisasterDestination(QString value)
{
    theDisasterDestination = value;
}


void RPopulationDisplacementEstimator::setUnsafeHousingStatus (bool value)
{
    theUnsafeHousingStatus = value;
}

void RPopulationDisplacementEstimator::setUnaffordableHousingStatus (bool value)
{
    theUnaffordableHousingStatus  = value;
}

void RPopulationDisplacementEstimator::setOvercrowdedHousingStatus (bool value)
{
    theOvercrowdedHousingStatus = value;
}

void RPopulationDisplacementEstimator::setUnsatisfactionWithNeighboorhoodStatus (bool value)
{
    theUnsatisfactionWithNeighborhoodStatus = value;
}

void RPopulationDisplacementEstimator::setFatalityIncidence(bool value)
{
    theHouseholdFatalityIncidence = value;
}

void RPopulationDisplacementEstimator::setSnowballFactor(double value)
{
    theSnowballFactor = value;
}

void RPopulationDisplacementEstimator::setTimeSinceTheEvent(double value)
{
    theTimeSinceTheEvent = value;
}


void RPopulationDisplacementEstimator::setCurrentLivingConditions(QString value)
{
    theCurrentLivingConditions = value;
}

void RPopulationDisplacementEstimator::setRepairCost(double value)
{
    theRepairCost = value;
}

void RPopulationDisplacementEstimator::setLandValue(double value)
{
    theLandValue = value;
}

void RPopulationDisplacementEstimator::setBuildingValue(double value)
{
    theBuildingValue = value;
}

void RPopulationDisplacementEstimator::setSellingFee(double value)
{
    theSellingFee = value;
}


void RPopulationDisplacementEstimator::setAverageRent(double value)
{
    theAverageRent = value;
}


void  RPopulationDisplacementEstimator::setAvailableFunds(double value)
{
    theAvailableFunds = value;
}

QString RPopulationDisplacementEstimator::getReasonForDisplacement()
{
    return theReasonForDisplacement;
}

bool RPopulationDisplacementEstimator::evaluateDisplacementStatus()
{
    return false;
}

QString RPopulationDisplacementEstimator::evaluateShortTermDestination()
{
    return QString("");
}

double RPopulationDisplacementEstimator::evaluateDisplacementDuration()
{
    return 0.0;
}

QString RPopulationDisplacementEstimator::evaluateLongTermDestination()
{
    return QString("");
}

bool RPopulationDisplacementEstimator::evaluateWillingnessToSellOfLandlords()
{
    return false;
}

QString RPopulationDisplacementEstimator::evaluateDecisionOfOccupants()
{
    return "";
}

QString RPopulationDisplacementEstimator::evaluateSatisfaction()
{
    return "";
}

