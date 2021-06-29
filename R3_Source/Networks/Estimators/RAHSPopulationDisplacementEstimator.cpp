#include "RAHSPopulationDisplacementEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>
#include "RScenarioModel.h"


RAHSPopulationDisplacementEstimator::RAHSPopulationDisplacementEstimator(QObject *parent, QString name)
    : RPopulationDisplacementEstimator(parent, name)
{
    theReasonForDisplacement = QString("None");
}


RAHSPopulationDisplacementEstimator::~RAHSPopulationDisplacementEstimator()
{
}


bool RAHSPopulationDisplacementEstimator::evaluateDisplacementStatus()
{
    //*************************************************
    // Functional Habitability
    //*************************************************
    bool theDisplacementStatus = false;
    theReasonForDisplacement = QString("None");
    QString theFunctionalHabitability = QString("High");

    // Determine the how many days this household can stay without power and water
    double powerAutonomy = 4.0;
    double waterAutonomy = 4.0;

    // If a generator is available
    if (theAvailabilityOfGenerator == true){
        powerAutonomy = 10000.0;
    }

    // If water is available
    if (theAvailabilityOfWater == true){
        waterAutonomy = 7.0;
    }


    // Evaluate building functionality
    // If complete damage
    if (theDS == 4){
        theFunctionalHabitability = QString("VeryLow");
    }

    // If extensive damage
    else if (theDS == 3 || thePowerShortageDuration > powerAutonomy || theWaterShortageDuration > waterAutonomy){
        theFunctionalHabitability = QString("Low");
    }

    // If moderate damage
    else if (theDS == 2 && thePowerShortageDuration > powerAutonomy && theWaterShortageDuration > waterAutonomy){
        theFunctionalHabitability = QString("Low");
    }

    // If no damage
    else if ((theDS == 0 || theDS == 1) && thePowerShortageDuration < 2.0 && theWaterShortageDuration < 1.0){
        theFunctionalHabitability = QString("High");
    }

    // If slight damage
    else {
        theFunctionalHabitability = QString("Moderate");
    }


    //*************************************************
    // Perceived habitability
    //*************************************************
    bool desireToLeave = false;

    // If the household has low functional habitability, is a renter, and has Seniors or children or neighbourhood apparent damage is significant
    if (theFunctionalHabitability == QString("Low") && theTenure == QString("RenterOccupied") && (theRace == QString("Hispanic") || theAge != QString("None") || theAverageDamageState > 3.0)){
        desireToLeave = true;

    } else if (theFunctionalHabitability == QString("Low") && theRace == QString("Hispanic") && theAge != QString("None")){
        desireToLeave = true;

    } else if (theFunctionalHabitability == QString("Moderate") && theWeatherConditions == QString("Bad") && theAverageDamageState > 3.0){
        desireToLeave = true;
    }

    //*************************************************
    // Displaced population
    //*************************************************
    // Conditions to define number of displaced persons
    // If the building is too damage, people will leave
    if (theFunctionalHabitability == QString("VeryLow")){
        theDisplacementStatus = true;
        theReasonForDisplacement = QString("FunctionalHabitability");

        // If people have the desire to leave and have a car, people will leave
    } else if (desireToLeave == true){ //&& theCarOwnership == true
        theDisplacementStatus = true;
        theReasonForDisplacement = QString("PerceivedHabitability");

    } else {
        theDisplacementStatus = false;
    }


    return theDisplacementStatus;
}


QString RAHSPopulationDisplacementEstimator::evaluateShortTermDestination()
{
    //*************************************************
    // Decision to leave in the short term
    //*************************************************
    if (theCurrentLivingConditions != QString("NotDisplaced")){
        return theCurrentLivingConditions;
    }


    theCurrentLivingConditions = QString("Displaced");
    return theCurrentLivingConditions;
}


double RAHSPopulationDisplacementEstimator::evaluateDisplacementDuration()
{
    // Ask the scenario model what is the end time for the simulation
    RScenarioModel * theScenarioModel = theDomain->findChild<RScenarioModel*>();
    double startTime = qobject_cast<RParameter *>((theScenarioModel->getStartTime()))->getCurrentValue();
    double endTime = qobject_cast<RParameter *>((theScenarioModel->getEndTime()))->getCurrentValue();
    double deltaTime = endTime - startTime;

    if (deltaTime > 40.0){
        qCritical() << "This model was designed with short-term displacement in mind. Are you sure its assumptions hold for displacements after" << deltaTime << "days?";
    }

    //Initialize the variable of interest to be zero
    double theTimeDisplaced = 0.0;

    //initialize number of days a dwelling is willing to stay at home without power and without water and electricity
    double Aw=0.0;
    double Ap=0.0;
    if (theAvailabilityOfGenerator == true){
        //if there is a generator in the household, set Ap to inifinity
        Ap=1000000.0;
    }

    if (theAvailabilityOfWater == true){
        //if there are alternative water resources, set Aw to 7 days
        Aw=7;
    }


    //if DS = 4 (Hf = very low), displaced for the entire time duration investigated
    if (theDS == 4){
        theTimeDisplaced = deltaTime;
        theReasonForDisplacement = QString("FunctionalHabitability");

        //if DS =3 (Hf = low)
    } else if (theDS == 3){
        //if a renter with children or seniors | a renter with bad neighbor conditions, displaced for the entire time duration investigated
        if (theTenure == QString("RenterOccupied") && (theRace == QString("Hispanic") || theAge != QString("None") || theAverageDamageState > 3.0)){
            theTimeDisplaced = deltaTime;
            theReasonForDisplacement = QString("PerceivedHabitability");

        } else if (theRace == QString("Hispanic") && theAge != QString("None")) {
            theTimeDisplaced = deltaTime;
            theReasonForDisplacement = QString("PerceivedHabitability");

            //otherwise it's acceptable
        } else {
            theTimeDisplaced = 0.0;
        }

        //qCritical() << "Displacement due to DS=3";
    }
    //if Td > Ap, or Tw > Aw (Hf = low)
    else if (thePowerShortageDuration > Ap || theWaterShortageDuration > Aw ){
        //if a renter with children or seniors | a renter with bad neighbor conditions, max (Td - Ap, Tw - Aw)
        if (theTenure == QString("RenterOccupied") && (theRace == QString("Hispanic") || theAge != QString("None") || theAverageDamageState > 3.0)){
            theTimeDisplaced = qMax(thePowerShortageDuration - Ap, theWaterShortageDuration - Aw) ;
            theReasonForDisplacement = QString("PerceivedHabitability");

        } else if (theRace == QString("Hispanic") && theAge != QString("None")) {
            theTimeDisplaced = deltaTime;
            theReasonForDisplacement = QString("PerceivedHabitability");

        } else {
            theTimeDisplaced = 0.0;
        }
        //qCritical() << "Displacement due to water/power" << thePowerShortageDuration << Ap << theWaterShortageDuration << Aw ;
    }
    //if DS = 2 and Td > Ap and Tw > Aw (Hf = low)
    else if (theDS == 2 && thePowerShortageDuration > Ap && theWaterShortageDuration > Aw){
        //if a renter with children or seniors | a renter with bad neighbor conditions:  min (Td - Ap, Tw - Aw)
        if (theTenure == QString("RenterOccupied") && (theRace == QString("Hispanic") || theAge != QString("None") || theAverageDamageState > 3.0)){
            theTimeDisplaced = qMin(thePowerShortageDuration - Ap, theWaterShortageDuration - Aw) ;
            theReasonForDisplacement = QString("PerceivedHabitability");

        } else if (theRace == QString("Hispanic") && theAge != QString("None")) {
            theTimeDisplaced = deltaTime;
            theReasonForDisplacement = QString("PerceivedHabitability");

        } else {
            theTimeDisplaced = 0.0;
        }
        //qCritical() << "Displacement due to DS=2";
    }
    //the remaining households are "Hf = Moderate"
    //for Hf = Moderate, it is only Unacceptable when both neighborhood and weather are Bad
    else if (theAverageDamageState > 2 && theWeatherConditions == QString("Bad")){
        theTimeDisplaced = qMax(0.0, qMin(thePowerShortageDuration - Ap, theWaterShortageDuration - Aw));
        theReasonForDisplacement = QString("PerceivedHabitability");
        //qCritical() << "Displacement due to weather";
    } else {
        theTimeDisplaced = 0.0;
        //qCritical() << this->objectName() << theDS << theAge << theAverageDamageState;
    }

    return theTimeDisplaced * theSize;
}


QString RAHSPopulationDisplacementEstimator::evaluateLongTermDestination()
{
    // Evaluate if the household will relocate at this time step
    double theWillingnessToRelocate = 0.0;
    double theMultiplierForCurrentLivingConditions = 1.0;

    bool theHouseholdPredispositionToLeave = false;
    if (theUnsafeHousingStatus == true || theUnaffordableHousingStatus == true || theOvercrowdedHousingStatus == true || theUnsatisfactionWithNeighborhoodStatus == true){
        theHouseholdPredispositionToLeave = true;
    }

    // If the building is recovered assume people will stay
    if (theCurrentLivingConditions == QString("Recovered") || theCurrentLivingConditions.contains(QString("Gone"))){
        return theCurrentLivingConditions;
    }

    // For households that are not displaced but were not happy with their homes to begin with
    else if (theCurrentLivingConditions == QString("NotDisplaced") && theRecentMoverStatus == true && (theHouseholdPredispositionToLeave == true || theHouseholdFatalityIncidence == true)){
        theWillingnessToRelocate = 1.5 * (1.0 + theAverageDamageState / 4.0) * (theSnowballFactor - 1.0);

        // In case the household is displaced
    } else if (theCurrentLivingConditions != QString("NotDisplaced")){

        double theThresholdForRentersToWantToMoveOut = 120.0 + 360.0 * rand() / static_cast<double>(RAND_MAX);
        double theThresholdForOwnersToWantToMoveOut = 360.0 + 720.0 * rand() / static_cast<double>(RAND_MAX);

        // If renter
        if (theTenure == QString("RenterOccupied")){
            theWillingnessToRelocate = (1.0 / theThresholdForRentersToWantToMoveOut) * (theTimeSinceTheEvent);

            // If owner
        } else if (theTenure == QString("OwnerOccupied")){
            theWillingnessToRelocate = (1.0 / theThresholdForOwnersToWantToMoveOut) * (theTimeSinceTheEvent);

        }

        // Calculate the baseline willingness to leave
        // If the family is in a hotel
        else if (theCurrentLivingConditions == QString("RentalUnit")){
            theMultiplierForCurrentLivingConditions = 1.0;

            // If the family is in a public shelter
        } else if (theCurrentLivingConditions == QString("PublicShelter")){
            theMultiplierForCurrentLivingConditions = 1.5;

            // If the family is living with family or friends
        } else if (theCurrentLivingConditions == QString("Relatives")){
            theMultiplierForCurrentLivingConditions = 1.25;
        }

        if (theHouseholdPredispositionToLeave == true || theHouseholdFatalityIncidence == true){
            theWillingnessToRelocate *= qMin(2.0, 1.5 * (1.0 + theAverageDamageState / 4.0) * theSnowballFactor * theMultiplierForCurrentLivingConditions);

        } else {
            theWillingnessToRelocate *= qMin(2.0, 1.0 * (1.0 + theAverageDamageState / 4.0) * theSnowballFactor * theMultiplierForCurrentLivingConditions);
        }
    }

    if (theWillingnessToRelocate > 0.5){
        theCurrentLivingConditions = QString("GoneVoluntarily");
    }

    return theCurrentLivingConditions;
}


