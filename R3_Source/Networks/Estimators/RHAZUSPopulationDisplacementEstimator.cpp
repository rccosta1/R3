#include "RHAZUSPopulationDisplacementEstimator.h"
#include "RMethod.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>
#include "RScenarioModel.h"


RHAZUSPopulationDisplacementEstimator::RHAZUSPopulationDisplacementEstimator(QObject *parent, QString name)
    : RPopulationDisplacementEstimator(parent, name)
{
    theReasonForDisplacement = QString("None");
}


RHAZUSPopulationDisplacementEstimator::~RHAZUSPopulationDisplacementEstimator()
{
}


bool RHAZUSPopulationDisplacementEstimator::evaluateDisplacementStatus()
{
    bool theDisplacementStatus = false;
    theReasonForDisplacement = QString("None");

    // Building occupancy weight
    double theOccupancyWeight = 0.0;
    if (theBuildingOccupancy == QString("RES1")){
        if (theDS == 2){
            theOccupancyWeight = 0.0;
        } else if (theDS == 3){
            theOccupancyWeight = 0.0;
        } else if (theDS == 4){
            theOccupancyWeight = 1.0;
        }

    } else if (theBuildingOccupancy.contains(QString("RES3"))){
        if (theDS == 2){
            theOccupancyWeight = 0.0;
        } else if (theDS == 3){
            theOccupancyWeight = 0.9;
        } else if (theDS == 4){
            theOccupancyWeight = 1.0;
        }
    } else {
        qCritical() << "The HAZUS population displacement estimator does not recognize occupancy type" << theBuildingOccupancy;
    }

    // The first parenthesis in Eq. 14-1 is the probability that,
    // given occupancy and damage, the building is not occupiable

    // Flipping a coin for the decision to be displaced or not
    double theRNG = rand()/static_cast<double>(RAND_MAX);
    if (theRNG < theOccupancyWeight){
        theDisplacementStatus = true;
        theReasonForDisplacement = QString("FunctionalHabitability");
    }

    return theDisplacementStatus;
}


QString RHAZUSPopulationDisplacementEstimator::evaluateShortTermDestination()
{
    //*************************************************
    // Decision to leave in the short term
    //*************************************************
    QString theCurrentLivingConditions = QString("UnknownDestination");

    //*************************************************
    // Calculate "alpha" for this household - Equation 14-3 in HAZUS
    //*************************************************

    // Get the income modification factor
    // Note: 1994 median income for SF is $36,238.5 - linearly interpolated from https://fred.stlouisfed.org/series/MHICA06075A052NCEN
    //       Table 14.3 was developed with data from 1992-1994
    // Thus IM5 > $35,000 in Table 14.3 is far from equating high income, i.e., 1.2 * median income = $43,485
    //      IM3 is close to the limit for low-income
    // ***** THIS CAN BE IMPROVED
    double theIncomeModificationFactor = 0.0;
    if (theIncome == QString("Low")){
        theIncomeModificationFactor = 0.29;
    } else if (theIncome == QString("Moderate")){
        theIncomeModificationFactor = 0.22;
    } else if (theIncome == QString("High")){
        theIncomeModificationFactor = 0.13;
    } else {
        qCritical() << "RHAZUSPopulationDisplacement could not identify income" << theIncome;
    }


    // Get the Race modification factor
    double theRaceModificationFactor = 0.0;
    if (theRace == QString("White")){
        theRaceModificationFactor = 0.24;
    } else if (theRace == QString("Black")){
        theRaceModificationFactor = 0.47;
    } else if (theRace == QString("Hispanic")){
        theRaceModificationFactor = 0.47;
    } else if (theRace == QString("Asian")){
        theRaceModificationFactor = 0.26;
    } else if (theRace == QString("NativeAmerican")){
        theRaceModificationFactor = 0.26;
    } else {
        qCritical() << "RHAZUSPopulationDisplacement could not identify Race" << theRace;
    }


    // Get the tenure modification factor
    // Note: it does not make any difference
    double theOwnershipModificationFactor = 0.0;
    if (theTenure == QString("OwnerOccupied")){
        theOwnershipModificationFactor = 0.40;
    } else if (theTenure == QString("RenterOccupied")){
        theOwnershipModificationFactor = 0.40;
    } else {
        qCritical() << "RHAZUSPopulationDisplacement could not identify ownership" << theTenure;
    }


    // Get the Race modification factor
    // Note: it does not make any difference
    double theAgeModificationFactor = 0.0;
    if (theAge == QString("Children")){
        theAgeModificationFactor = 0.40;
    } else if (theAge == QString("Elderly")){
        theAgeModificationFactor = 0.40;
    } else {
        theAgeModificationFactor = 0.40;
    }


    // Weights
    double theIncomeWeight = 0.73;
    double theRaceWeight = 0.27;
    double theOwnershipWeight = 0.00;
    double theAgeWeight = 0.00;


    // Alpha
    double theAlpha = theIncomeWeight * theIncomeModificationFactor + theRaceWeight * theRaceModificationFactor + theOwnershipWeight * theOwnershipModificationFactor + theAgeWeight * theAgeModificationFactor;

    // Note that in the HAZUS methodology alpha is the probability that a household with
    // certain demographic characteristics will go to a shelter.
    // It is multiplied by the "percentage of the displaced population that has these demographics" in equation 14-2

    // Here, we will 'flip a coin' to determine if the family is going to a public shelter or not
    double theRNG = rand()/static_cast<double>(RAND_MAX);
    if (theRNG < theAlpha){
        theCurrentLivingConditions = QString("PublicShelter");

    } else {

        // If the household IS NOT going to a public shelter,
        // flip another 50/50 coin to put them in a hotel of with family
        double theRNGForHotel = rand()/static_cast<double>(RAND_MAX);

        if(theRNGForHotel < 0.50){
            theCurrentLivingConditions = QString("RentalUnit");
        } else {
            theCurrentLivingConditions = QString("Relatives");
        }
    }

    return theCurrentLivingConditions;
}


double RHAZUSPopulationDisplacementEstimator::evaluateDisplacementDuration()
{
    // Ask the scenario model what are the start and end time for the simulation
    RScenarioModel * theScenarioModel = theDomain->findChild<RScenarioModel*>();
    double startTime = qobject_cast<RParameter *>((theScenarioModel->getStartTime()))->getCurrentValue();
    double endTime = qobject_cast<RParameter *>((theScenarioModel->getEndTime()))->getCurrentValue();
    double deltaTime = endTime - startTime;

    if (deltaTime > 40.0){
        qCritical() << "This model was designed with short-term displacement in mind. Are you sure its assumptions hold for displacements after" << endTime - theCurrentTime->getCurrentValue() << "days?";
    }

    bool theDisplacementStatus = false;
    theReasonForDisplacement = QString("None");

    // Building occupancy weight
    double theOccupancyWeight = 0.0;
    if (theBuildingOccupancy == QString("RES1")){
        if (theDS == 2){
            theOccupancyWeight = 0.0;
        } else if (theDS == 3){
            theOccupancyWeight = 0.0;
        } else if (theDS == 4){
            theOccupancyWeight = 1.0;
        }

    } else if (theBuildingOccupancy.contains(QString("RES3"))){
        if (theDS == 2){
            theOccupancyWeight = 0.0;
        } else if (theDS == 3){
            theOccupancyWeight = 0.9;
        } else if (theDS == 4){
            theOccupancyWeight = 1.0;
        }
    } else {
        qCritical() << "The HAZUS population displacement estimator does not recognize occupancy type" << theBuildingOccupancy;
    }

    // The first parenthesis in Eq. 14-1 is the probability that,
    // given occupancy and damage, the building is not occupiable

    // Flipping a coin for the decision to be displaced or not
    double theRNG = rand()/static_cast<double>(RAND_MAX);
    if (theRNG < theOccupancyWeight){
        theDisplacementStatus = true;
        theReasonForDisplacement = QString("FunctionalHabitability");
        return deltaTime * theSize;

    } else {
        return 0.0;
    }


}

double RHAZUSPopulationDisplacementEstimator::evaluateUtility()
{
    return 0;
}
