#include "RStatisticsCanadaPreparednessEstimator.h"
#include "RDisasterPreparednessEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>


RStatisticsCanadaPreparednessEstimator::RStatisticsCanadaPreparednessEstimator(QObject *parent, QString name)
    : RDisasterPreparednessEstimator(parent, name)
{
}


RStatisticsCanadaPreparednessEstimator::~RStatisticsCanadaPreparednessEstimator()
{

}

bool RStatisticsCanadaPreparednessEstimator::getGeneratorAvailability()
{
    bool theAvailabilityOfGenerator = false;
    double theProbabilityOfHavingAGenerator = 0.0;
    double theProbabilityOfHavingAGeneratorForAOwner = 0.1956;
    double theProbabilityOfHavingAGeneratorForARenter = 0.1095;
    double theProbabilityOfHavingAGeneratorForALowIncome = 0.1381;
    double theProbabilityOfHavingAGeneratorForAModIncome = 0.1632;
    double theProbabilityOfHavingAGeneratorForAHighIncome = 0.1570;
    double theProbabilityOfHavingAGeneratorForAResident = 0.1909;
    double theProbabilityOfHavingAGeneratorForAEstablished = 0.1101;
    double theProbabilityOfHavingAGeneratorForARecent = 0.0734;
    double theProbabilityOfHavingAGeneratorForANoChildrenOrSenior = 0.1509;
    double theProbabilityOfHavingAGeneratorForAChildrenOrSenior = 0.1475;
    double theProbabilityOfHavingAGeneratorForAOnePerson = 0.1084;
    double theProbabilityOfHavingAGeneratorForATwoPersons = 0.1880;
    double theProbabilityOfHavingAGeneratorForAThreePersons = 0.1590;
    double theProbabilityOfHavingAGeneratorForAFourPersons = 0.1590;
    double theProbabilityOfHavingAGeneratorForAFivePersons = 0.1807;

    // Housing Tenure
    if (theTenure == QString("RenterOccupied")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForARenter);
    } else if (theTenure == QString("OwnerOccupied")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAOwner);
    }

    // Income
    if (theHouseholdIncome == QString("Low")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForALowIncome);
    } else if (theHouseholdIncome == QString("Moderate")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAModIncome);
    } else if (theHouseholdIncome == QString("High")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAHighIncome);
    }

    // Immigration status
    if (theImmigrationStatus == QString("RecentImmigrant")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForARecent);
    } else if (theImmigrationStatus == QString("EstablishedImmigrant")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAEstablished);
    } else if (theImmigrationStatus == QString("NonImmigrant")){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAResident);
    }

    // Age of members
    if (thePresenceOfChildren == true || thePresenceOfSeniors == true){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAChildrenOrSenior);

    } else {
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForANoChildrenOrSenior);
    }


    // Household size
    if (theHouseholdSize == 1){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAOnePerson);
    } else if (theHouseholdSize == 2){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForATwoPersons);
    } else if (theHouseholdSize == 3){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAThreePersons);
    } else if (theHouseholdSize == 4){
        theProbabilityOfHavingAGenerator = theProbabilityOfHavingAGeneratorForAFourPersons;
    } else if (theHouseholdSize == 5){
        theProbabilityOfHavingAGenerator = qMax(theProbabilityOfHavingAGenerator,theProbabilityOfHavingAGeneratorForAFivePersons);
    }


    double rngPower = rand()/static_cast<double>(RAND_MAX);
    if (rngPower < theProbabilityOfHavingAGenerator){
        theAvailabilityOfGenerator = true;

    } else {
        theAvailabilityOfGenerator = false;
    }


    return theAvailabilityOfGenerator;
}


bool RStatisticsCanadaPreparednessEstimator::getWaterAvailability()
{
    bool theAvailabilityOfWaterSource = false;
    double theProbabilityOfAlternativeWaterSource = 0.0;
    double theProbabilityOfWaterForAOwner = 0.5089;
    double theProbabilityOfWaterForARenter = 0.3789;
    double theProbabilityOfWaterForALowIncome = 0.4159;
    double theProbabilityOfWaterForAModIncome = 0.4655;
    double theProbabilityOfWaterForAHighIncome = 0.4555;
    double theProbabilityOfWaterForAResident = 0.4915;
    double theProbabilityOfWaterForAEstablished = 0.4274;
    double theProbabilityOfWaterForARecent = 0.3205;
    double theProbabilityOfWaterForAChildrenOrSenior = 0.4374;
    double theProbabilityOfWaterForANoChildrenOrSenior = 0.4475;
    double theProbabilityOfWaterForAOnePerson = 0.3978;
    double theProbabilityOfWaterForATwoPersons = 0.471;
    double theProbabilityOfWaterForAThreePersons = 0.4815;
    double theProbabilityOfWaterForAFourPersons = 0.4187;
    double theProbabilityOfWaterForAFivePersons = 0.4920;


    // Tenure
    if (theTenure == QString("RenterOccupied")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForARenter);
    } else if (theTenure == QString("OwnerOccupied")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAOwner);
    }

    // Income
    if (theHouseholdIncome == QString("Low")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForALowIncome);
    } else if (theHouseholdIncome == QString("Moderate")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAModIncome);
    } else if (theHouseholdIncome == QString("High")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAHighIncome);
    }

    // Immigrant status
    if (theImmigrationStatus == QString("RecentImmigrant")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForARecent);
    } else if (theImmigrationStatus == QString("EstablishedImmigrant")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAEstablished);
    } else if (theImmigrationStatus == QString("NonImmigrant")){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAResident);
    }

    // Age of members
    if (thePresenceOfChildren == true || thePresenceOfSeniors == true){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAChildrenOrSenior);
    } else {
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForANoChildrenOrSenior);
    }

    // Household size
    if (theHouseholdSize == 1){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAOnePerson);
    } else if (theHouseholdSize == 2){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForATwoPersons);
    } else if (theHouseholdSize == 3){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAThreePersons);
    } else if (theHouseholdSize == 4){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAFourPersons);
    } else if (theHouseholdSize == 5){
        theProbabilityOfAlternativeWaterSource = qMax(theProbabilityOfAlternativeWaterSource, theProbabilityOfWaterForAFivePersons);
    }


    double rngWater = rand()/static_cast<double>(RAND_MAX);
    if (rngWater < theProbabilityOfAlternativeWaterSource){
        theAvailabilityOfWaterSource = true;

    } else {
        theAvailabilityOfWaterSource = false;
    }

    return theAvailabilityOfWaterSource;
}


bool RStatisticsCanadaPreparednessEstimator::getSocialSupportAvailability()
{
    bool theAvailabilityOfSocialSupport = false;
    double theProbabilityOfHavingSocialSupport = 0.0;
    double theProbabilityOfHavingSocialSupportForALowIncome = 0.20;
    double theProbabilityOfHavingSocialSupportForAModIncome = 0.25;
    double theProbabilityOfHavingSocialSupportForAHighIncome = 0.25;
    double theProbabilityOfHavingSocialSupportForAResident = 0.24;
    double theProbabilityOfHavingSocialSupportForAEstablished = 0.16;
    double theProbabilityOfHavingSocialSupportForARecent = 0.15;

    // Income
    if (theHouseholdIncome == QString("Low")){
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForALowIncome);
    } else if (theHouseholdIncome == QString("Moderate")) {
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForAModIncome);
    } else if (theHouseholdIncome == QString("High")){
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForAHighIncome);
    }

    // Immigration status
    if (theImmigrationStatus == QString("RecentImmigrant")){
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForARecent);
    } else if (theImmigrationStatus == QString("EstablishedImmigrant")) {
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForAEstablished);
    } else if (theImmigrationStatus == QString("NonImmigrant")){
        theProbabilityOfHavingSocialSupport = qMax(theProbabilityOfHavingSocialSupport, theProbabilityOfHavingSocialSupportForAResident);
    }


    double rngSupport = rand()/static_cast<double>(RAND_MAX);
    if (rngSupport < theProbabilityOfHavingSocialSupport){
        theAvailabilityOfSocialSupport = true;

    } else {
        theAvailabilityOfSocialSupport = false;
    }

    return theAvailabilityOfSocialSupport;
}
