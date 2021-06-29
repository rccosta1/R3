#include "RSutley2016MorbidityEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>

RSutley2016MorbidityEstimator::RSutley2016MorbidityEstimator(QObject *parent, QString name)
    : RObject(parent, name)
{
}


RSutley2016MorbidityEstimator::~RSutley2016MorbidityEstimator()
{
}

void RSutley2016MorbidityEstimator::setIncome(QString value)
{
    theIncome = value;
}

void RSutley2016MorbidityEstimator::setBuildingType(QString value)
{
    theBuildingType  = value;
}

void RSutley2016MorbidityEstimator::setTime(double value)
{
    theTime = value;
}

void RSutley2016MorbidityEstimator::setDamageState(int value)
{
    theDS = value;
}


bool RSutley2016MorbidityEstimator::estimateFatalities()
{
    // HAZUS 2.1 - TABLE 13.3 THROUGH 13.6 - INDOOR CASUALTY RATES
    QVector<double> severity1RatesIndoor;
    QVector<double> severity2RatesIndoor;
    QVector<double> severity3RatesIndoor;
    QVector<double> severity4RatesIndoor;

    severity1RatesIndoor << 0.00;
    severity2RatesIndoor << 0.00;
    severity3RatesIndoor << 0.00;
    severity4RatesIndoor << 0.00;

    if(theBuildingType == QString("W1")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }

    else if(theBuildingType == QString("W2")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }

    else if(theBuildingType == QString("S1L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }

    else if(theBuildingType == QString("S1M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("S1H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }

    else if(theBuildingType == QString("S2L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }

    else if(theBuildingType == QString("S2M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("S2H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }

    else if(theBuildingType == QString("S3L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }

    else if(theBuildingType == QString("S4L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }

    else if(theBuildingType == QString("S4M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("S4H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }

    else if(theBuildingType == QString("S5L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 7.80;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.52;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.41;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.81;
    }

    else if(theBuildingType == QString("S5M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("S5H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.31;
    }

    else if(theBuildingType == QString("C1L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("C1M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("C1H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("C2L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("C2M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("C2H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("C3L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }

    else if(theBuildingType == QString("C3M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("C3H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("PC1")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }

    else if(theBuildingType == QString("PC2L")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 10.25;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.85;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.76;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.51;
    }

    else if(theBuildingType == QString("PC2M")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("PC2H")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("RM1L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("RM1M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("RM2L")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 9.55;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 3.47;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.66;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.31;
    }

    else if(theBuildingType == QString("RM2M")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 8.50;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 2.90;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.01;
    }

    else if(theBuildingType == QString("RM2H")){
        severity1RatesIndoor << 0.05 << 0.20 << 1.00 << 6.75;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.95;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.26;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.51;
    }

    else if(theBuildingType == QString("URML")){
        severity1RatesIndoor << 0.05 << 0.35 << 2.00 << 14.50;
        severity2RatesIndoor << 0.00 << 0.40 << 0.20 << 4.70;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.77;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.52;
    }

    else if(theBuildingType == QString("URMM")){
        severity1RatesIndoor << 0.05 << 0.35 << 2.00 << 14.50;
        severity2RatesIndoor << 0.00 << 0.40 << 0.20 << 4.70;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.77;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 1.52;
    }

    else if(theBuildingType == QString("MH")){
        severity1RatesIndoor << 0.05 << 0.25 << 1.00 << 6.05;
        severity2RatesIndoor << 0.00 << 0.03 << 0.10 << 1.57;
        severity3RatesIndoor << 0.00 << 0.00 << 0.00 << 0.10;
        severity4RatesIndoor << 0.00 << 0.00 << 0.00 << 0.16;
    }

    else {
        qCritical() << "The morbidity calculator has a problem with the property Building Type." << theBuildingType;
    }

    // Time of day modification factor
    double occupancyOverTheDay = 1.0;

    // 0 < t < 0.25 is between 12:00am and 6:00am - night time
    if (theTime < 0.25){
        occupancyOverTheDay = 0.999 * 0.99;
    }
    // 0.25 < t < 0.375 is between 6:00am and 9:00am - rush time in the morning
    else if(theTime >= 0.25 && theTime < 0.375) {
        occupancyOverTheDay = 0.70 * 0.50;
    }
    // 0.375 < t < 0.666 is between 9:00am and 4:00pm - calmer part of the day, everyone is at work
    else if(theTime >= 0.375 && theTime < 0.666){
        occupancyOverTheDay = 0.70 * 0.075;
    }
    // 0.666 < t 0.8333 is between 4:00pm and 8:00pm - rush hour, people going home
    else if(theTime >= 0.666 && theTime < 0.8333){
        occupancyOverTheDay = 0.70 * 0.50;
    }
    // t > 0.8333 is past 8pm, calmer times again
    else if (theTime >= 0.833){
        occupancyOverTheDay = 0.999 * 0.99;
    }


    // ***************************************************************
    // * Socieconomic demographics
    // ***************************************************************
    // 1. INCOME
    double incomeInjuryFactor;
    double incomeFatalityFactor;

    if (theIncome == QString("Low")){
        incomeInjuryFactor = 3.485;
        incomeFatalityFactor = 3.485;
    }
    else if (theIncome == QString("Medium")){
        incomeInjuryFactor = 1.850;
        incomeFatalityFactor = 1.850;
    }
    else {
        incomeInjuryFactor = 1.000;
        incomeFatalityFactor = 1.000;
    }

    // 4. BUILT ENVIRONMENT
    double builtEnvironmentInjuryFactor = 1.00;
    double builtEnvironmentFatalityFactor = 1.00;

    double myProbabilityOfFatality = incomeFatalityFactor * builtEnvironmentFatalityFactor * occupancyOverTheDay * severity4RatesIndoor.at(theDS) / 100.0;
    double rngFatality = theRNG.generateDouble();

    // If any member of the household dies
    if (rngFatality < myProbabilityOfFatality){
        return true;

    } else {
        return false;
    }
}



