#include "RStorageTankModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>


RStorageTankModel::RStorageTankModel(QObject *parent, QString name)
    : RObject(parent, name)
{

}

RStorageTankModel::~RStorageTankModel()
{

}



void RStorageTankModel::setFragilityCurves(double value)
{
    PGADamageMedians.clear();
    PGADamageDispersions.clear();
    PGDDamageMedians.clear();
    PGDDamageDispersions.clear();
    restorationMeans.clear();
    restorationStdvs.clear();

    // Hazus page 428
    restorationMeans << 0.9 << 7.0 << 28.0 << 70.0;
    restorationStdvs << 0.5 << 7.0 << 26.0 << 55.0;
    damageRatios << 0.0 << 0.13 << 0.40 << 0.80 << 1.0;

    // These were obtained using curve fitting based on the data presented by
    // ALA - American Lifeline Alliance, Seismic fragility formulation for water system: Part 1-Guideline, ASCE, 2001a.

    PGDDamageMedians << 10.0 << 10.0 << 10.0 << 10.0;  //20% is assumed to be complete
    PGDDamageDispersions << 1.2 << 1.2 << 1.2 << 1.2;

    // ALA - American Lifeline Alliance, Seismic fragility formulation for water system: Part 1-Guideline, ASCE, 2001a.
    if(value < 0.5){

        PGADamageMedians << 0.56 << 2.0 << 10.0 << 10.0;
        PGADamageDispersions << 0.8 << 0.4 << 0.8 << 0.8;
        //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel less than 50% since it is " << theFillLevel << " full.";
    }

    else if(value >= 0.5 && theFillLevel < 0.6){

        PGADamageMedians << 0.18 << 0.73 << 1.14 << 1.16;
        PGADamageDispersions << 0.8 << 0.8 << 0.8 << 0.40;
        //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel between 50% and 60% since it is " << theFillLevel << " full.";
    }

    else if(value >= 0.6 && theFillLevel < 0.9){

        PGADamageMedians << 0.22 << 0.70 << 1.09 << 1.16;
        PGADamageDispersions << 0.80 << 0.80 << 0.80 << 0.41;
        //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel between 60% and 90% since it is " << theFillLevel << " full.";
    }

    else if(value >= 0.9){

        PGADamageMedians << 0.13 << 0.67 << 1.01 << 1.15;
        PGADamageDispersions << 0.07 << 0.80 << 0.80 << 0.10;
        //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel > 90% since it is " << theFillLevel << " full.";
    }

}


// I need to pass the location to this function
int RFuelStorageTank::calculateIntensityAndDamage()
{
    // INTENSITY
    double intensity = calculateIntensity();

    // DAMAGE
    if (intensity > 0){
        int aux = calculateDamageProbabilities();
    }
    meanDamageRatio = calculateMeanDamageRatio();

    // RESTORATION
    meanRestorationRatio = calculateMeanRestorationRatio();
    double timeUntilRestoration = calculateTimeUntilGivenRestorationRatio(0.8);

    return 0;
}



int RFuelStorageTank::calculateIntensity()
{
    PGAIntensity = 0.0;
    SaIntensity = 0.0;
    PGVIntensity = 0.0;
    PGDIntensity = 0.0;

    RResponse *theResponseObject;
    RIntensityModel *theIntensityModel;

    for (int i = 0; i < theIntensityList.count(); i++) {

        theResponseObject = qobject_cast<RResponse *>(theIntensityList[i]);
        theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());
        theIntensityModel->setLocation(theLocation);


        if (PGADamageMedians.size() > 0 && i == 0) {
            theIntensityModel->setIntensityMeasure("PGA");
            theIntensityModel->evaluateModel(RGradientType::NONE);
            PGAIntensity += theResponseObject->getCurrentValue();
        }

        if (SaDamageMedians.size() > 0 && i == 100) {
            theIntensityModel->setIntensityMeasure("Sa");
            theIntensityModel->setPeriod(0.5);
            theIntensityModel->evaluateModel(RGradientType::NONE);
            SaIntensity += theResponseObject->getCurrentValue();
        }

        if (PGVDamageMedians.size() > 0 && i == 100) {
            theIntensityModel->setIntensityMeasure("PGV");
            theIntensityModel->evaluateModel(RGradientType::NONE);
            PGVIntensity += theResponseObject->getCurrentValue();
        }

        if (PGDDamageMedians.size() > 0 && i == 1) {
            theIntensityModel->setIntensityMeasure("PGD");
            theIntensityModel->evaluateModel(RGradientType::NONE);
            PGDIntensity += theResponseObject->getCurrentValue();
        }
    }


     for (int i=0; i<20; i++) {

        if (PGAIntensity >= 0.1*i && PGAIntensity < 0.1*(i+1)) {

            theDamageBins[i] += 1.0;
        }
    }


    int intensity = 0;
    if (PGAIntensity != 0.0 || SaIntensity != 0.0 || PGVIntensity != 0.0 || PGDIntensity != 0.0) {

        intensity = 1;
    }
    return intensity;
}




int RFuelStorageTank::calculateDamageProbabilities()
{
    // If any intensity is non-zero then an earthquake just happended; set that time
    if (PGAIntensity != 0.0 || SaIntensity != 0.0 || PGVIntensity != 0.0 || PGDIntensity != 0.0) {

        timeOfEvent = theCurrentTime->getCurrentValue();
    }

        PGAProbabilityVector.clear();
        SaProbabilityVector.clear();
        PGVProbabilityVector.clear();
        PGDProbabilityVector.clear();

    // Initial declarations, and create an auxiliary Normal distribution
    RNormalDistribution theNormalDistribution(theDomain);

    // Calculate damage state probabilities for PGA
    if (PGADamageMedians.size() > 0) {


        PGAProbabilityVector.clear();


        for (int i = 0; i < PGADamageMedians.count(); i++) {

            if (PGAIntensity != 0.0) {

                PGAProbabilityVector << theNormalDistribution.getNormalCDF(1.0 / PGADamageDispersions[i] * log(PGAIntensity / PGADamageMedians[i]), 1);

            }
            else {

                PGAProbabilityVector << 0.0;
            }
        }
    }


    // Calculate damage state probabilities for Sa
    if (SaDamageMedians.size() > 0) {

        SaProbabilityVector.clear();

        for (int i = 0; i < SaDamageMedians.count(); i++) {

            if (SaIntensity != 0.0) {

                SaProbabilityVector << theNormalDistribution.getNormalCDF(1.0 / SaDamageDispersions[i] * log(SaIntensity / SaDamageMedians[i]), 1);
            }
            else {

                SaProbabilityVector << 0.0;
            }
        }
    }



    // Calculate damage state probabilities for PGV
    if (PGVDamageMedians.size() > 0) {

        PGVProbabilityVector.clear();

        for (int i = 0; i < PGVDamageMedians.count(); i++) {

            if (PGVIntensity != 0.0) {

                PGVProbabilityVector << theNormalDistribution.getNormalCDF(1.0 / PGVDamageDispersions[i] * log(PGVIntensity / PGVDamageMedians[i]), 1);
            }
            else {

                PGVProbabilityVector << 0.0;
            }
        }
    }

    // Calculate damage state probabilities for PGD
    if (PGDDamageMedians.size() > 0) {

        PGDProbabilityVector.clear();

        for (int i = 0; i < PGDDamageMedians.count(); i++) {

            if (PGDIntensity != 0.0) {

                PGDProbabilityVector << theNormalDistribution.getNormalCDF(1.0 / PGDDamageDispersions[i] * log(PGDIntensity / PGDDamageMedians[i]), 1);
            }
            else {

                PGDProbabilityVector << 0.0;
            }
        }
    }
    return 0;
}



double RFuelStorageTank::calculateMeanDamageRatio()
{
    // Initial declarations
    double meanDamageRatio = 0.0;
    double probabilityOfBeingInSpecificDamageState;
    double oldProb = 1.0;
    int numberOfDamageStates = 0;
    probabilityVector.clear();

    // Set the "probability vector" depending on what causes damage, e.g., only Sa, or perhaps PGA and PGD

    // First consider the case where damage depends only on Sa
    if (SaDamageMedians.size() > 0) {

        numberOfDamageStates = SaDamageMedians.size();

        probabilityVector = SaProbabilityVector;

    }
    // Then consider the case where damage is caused by PGA and PGD
    else if (PGADamageMedians.size() > 0 && PGDDamageMedians.size() > 0) {

        numberOfDamageStates = PGADamageMedians.size();

        for (int i = 0; i < numberOfDamageStates; i++) {

            probabilityVector <<  PGAProbabilityVector[i] + PGDProbabilityVector[i] - PGAProbabilityVector[i] * PGDProbabilityVector[i];

        }
    }

    double auxProb = PGAProbabilityVector[1] + PGDProbabilityVector[1] - PGAProbabilityVector[1] * PGDProbabilityVector[1];
    for (int i=0; i<20; i++) {

        if (auxProb >= 0.05*i && auxProb < 0.05*(i+1)) {

            theDamageBins[i] += 1.0;
        }
    }


    // Then compute the mediage damage, regardless of case
    if (probabilityVector.size() != 0) {


        for (int i=0; i<numberOfDamageStates + 1; i++) {

            // For the last fragility curve the probability is CurveValue - 0.0
            if (i==numberOfDamageStates) {

                probabilityOfBeingInSpecificDamageState = oldProb - 0.0;
            }
            // For all other cases, the probability is the distance between two fragility curves
            else {

                probabilityOfBeingInSpecificDamageState = oldProb - probabilityVector[i];
                oldProb = probabilityVector[i];
            }
            meanDamageRatio += probabilityOfBeingInSpecificDamageState * (i);
        }
    }

    return meanDamageRatio;
}


double RFuelStorageTank::calculateMeanRestorationRatio()
{
    // First check that there is any damage at all
    double probSum = 0.0;

    for (int i=0; i<probabilityVector.size(); i++) {

        probSum += probabilityVector.at(i);
    }

    // Proceed if there is damage
    //if (probSum != 0.0) {

        // Calculate the percent functional for each damage state at this time after the earthquake
        RNormalDistribution theNormalDistribution(theDomain);
        double timeSinceEvent = theCurrentTime->getCurrentValue()-timeOfEvent;
        QVector<double> percentFunctionalVector;
        percentFunctionalVector << 1.0;
        for (int i = 0; i < restorationMeans.count(); i++) {
            percentFunctionalVector << theNormalDistribution.getNormalCDF(timeSinceEvent - restorationMeans[i], restorationStdvs[i]);
        }


        // Then compute the mean functionality
        int numberOfDamageStates = probabilityVector.size();
        double meanRestorationRatio = 0.0;
        double probabilityOfBeingInSpecificDamageState = 0.0;
        double oldProb = 1.0;

        for (int i=0; i < numberOfDamageStates + 1; i++) {

            // For the last fragility curve the probability is CurveValue - 0.0
            if (i==numberOfDamageStates) {

                probabilityOfBeingInSpecificDamageState = oldProb - 0.0;
            }

            // For all other cases, the probability is the distance between two fragility curves
            else {

                probabilityOfBeingInSpecificDamageState = oldProb - probabilityVector[i];
                oldProb = probabilityVector[i];
            }


            //The restoration curves start at DS1 but we need to add the prob. of no damage to the restoration
            if(i==0){
                meanRestorationRatio += probabilityOfBeingInSpecificDamageState * 1.0;
             }

            else{
                meanRestorationRatio += probabilityOfBeingInSpecificDamageState * percentFunctionalVector[i-1];
            }
        }

    return meanRestorationRatio;
}












int RStorageTankModel::evaluateModel(RGradientType theGradientType)
{

    return 1;
}


