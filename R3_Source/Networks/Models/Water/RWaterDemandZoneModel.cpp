#include "RWaterDemandZoneModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include "RAHSPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"


#include <math.h>

RWaterDemandZoneModel::RWaterDemandZoneModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

    theSa03Counter = -1;
    theSa1Counter = -1;
    thePGACounter = -1;

    theDomain->createObject(objectName() + "PowerResponse", "RGenericResponse");
    thePowerShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePowerShortageDurationResponse->setModel(this);
    thePowerShortageDurationResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "WaterResponse", "RGenericResponse");
    theWaterShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theWaterShortageDurationResponse->setModel(this);
    theWaterShortageDurationResponse->setCurrentValue(0.0);

    //    (Intercept)                   1.3804     0.1263  10.928  < 2e-16 ***
    theDomain->createObject(objectName() + "theta0", "RContinuousRandomVariable");
    theta0 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theta0->setDistributionType(QString("Normal (mean, stdv)"));
    theta0->setMean("1.3804");
    theta0->setStandardDeviation("0.1263");

    //    IncomeLow                    -0.9093     0.1207  -7.536 4.85e-14 ***
    theDomain->createObject(objectName() + "thetaLowIncome", "RContinuousRandomVariable");
    thetaLowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaLowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    thetaLowIncome->setMean("-0.9093");
    thetaLowIncome->setStandardDeviation("0.1207");

    //    IncomeModerate                0.3711     0.1992   1.863   0.0625 .
    theDomain->createObject(objectName() + "thetaModerateIncome", "RContinuousRandomVariable");
    thetaModerateIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaModerateIncome->setDistributionType(QString("Normal (mean, stdv)"));
    thetaModerateIncome->setMean("0.3711");
    thetaModerateIncome->setStandardDeviation("0.1992");

    //    ImmigrationStatusNotCitizen  -0.8557     0.1330  -6.435 1.24e-10 ***
    theDomain->createObject(objectName() + "thetaImmigration", "RContinuousRandomVariable");
    thetaImmigration = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaImmigration->setDistributionType(QString("Normal (mean, stdv)"));
    thetaImmigration->setMean("-0.8557");
    thetaImmigration->setStandardDeviation("0.1330");

    //    DisabilityYes                -1.0874     0.1299  -8.371  < 2e-16 ***
    theDomain->createObject(objectName() + "thetaDisability", "RContinuousRandomVariable");
    thetaDisability = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaDisability->setDistributionType(QString("Normal (mean, stdv)"));
    thetaDisability->setMean("-1.0874");
    thetaDisability->setStandardDeviation("0.1299");

    //    HispanicYes                  -1.2748     0.1266 -10.070  < 2e-16 ***
    theDomain->createObject(objectName() + "thetaHispanic", "RContinuousRandomVariable");
    thetaHispanic = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaHispanic->setDistributionType(QString("Normal (mean, stdv)"));
    thetaHispanic->setMean("-1.2748");
    thetaHispanic->setStandardDeviation("0.1266");

    //    HigherEdYes                   0.7341     0.1120   6.555 5.56e-11 ***
    theDomain->createObject(objectName() + "thetaHigherEd", "RContinuousRandomVariable");
    thetaHigherEd = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaHigherEd->setDistributionType(QString("Normal (mean, stdv)"));
    thetaHigherEd->setMean("0.7341");
    thetaHigherEd->setStandardDeviation("0.1120");
}


RWaterDemandZoneModel::~RWaterDemandZoneModel()
{
}


int RWaterDemandZoneModel::resetTime()
{
    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    thePowerShortageDuration = 0.0;
    theWaterShortageDuration = 0.0;
    theExpectedRepairTime = 0.0;

    thePoorVector.clear();
    theModerateVector.clear();
    theRichVector.clear();
    thePreCodeVector.clear();
    theLowCodeVector.clear();
    theModCodeVector.clear();
    theHighCodeVector.clear();
    theOwnerVector.clear();
    theRenterVector.clear();
    theSFVector.clear();
    theMFVector.clear();
    theInsuredVector.clear();
    theNotInsuredVector.clear();
    theRecentVector.clear();
    theEstablishedVector.clear();
    theResidentVector.clear();
    theFunctionalHabitabilityVector.clear();
    thePerceivedHabitabilityVector.clear();

    // -----------
    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());

    if (thePGACounter > (theSa0_3Vector.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough Sa values!";
        thePGACounter = 0;
        theSa03Counter = 0;
        theSa1Counter = 0;
    }

    thePGACounter++;
    theSa03Counter++;
    theSa1Counter++;
    // -----------

    return 0;
}


QObject * RWaterDemandZoneModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RWaterDemandZoneModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}



QString RWaterDemandZoneModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RWaterDemandZoneModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


QString RWaterDemandZoneModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterDemandZoneModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}

QVector<double> RWaterDemandZoneModel::getPoorVector()
{
    return thePoorVector;
}

QVector<double> RWaterDemandZoneModel::getModerateCodeVector()
{
    return theModerateVector;
}

QVector<double> RWaterDemandZoneModel::getRichVector()
{
    return theRichVector;
}

QVector<double> RWaterDemandZoneModel::getPreCodeVector()
{
    return thePreCodeVector;
}

QVector<double> RWaterDemandZoneModel::getLowCodeVector()
{
    return theLowCodeVector;
}

QVector<double> RWaterDemandZoneModel::getModCodeVector()
{
    return theModCodeVector;
}

QVector<double> RWaterDemandZoneModel::getHighCodeVector()
{
    return theHighCodeVector;
}

QVector<double> RWaterDemandZoneModel::getOwnerVector()
{
    return theOwnerVector;
}

QVector<double> RWaterDemandZoneModel::getRenterVector()
{
    return theRenterVector;
}

QVector<double> RWaterDemandZoneModel::getSFVector()
{
    return theSFVector;
}

QVector<double> RWaterDemandZoneModel::getMFVector()
{
    return theMFVector;
}

QVector<double> RWaterDemandZoneModel::getFunctionalHabitability()
{
    return theFunctionalHabitabilityVector;
}

QVector<double> RWaterDemandZoneModel::getPerceivedHabitability()
{
    return thePerceivedHabitabilityVector;
}

QVector<double> RWaterDemandZoneModel::getMyResults()
{
    return myResults;
}

QVector<double> RWaterDemandZoneModel::getInsured()
{
    return theInsuredVector;
}

QVector<double> RWaterDemandZoneModel::getNoInsured()
{
    return theNotInsuredVector;
}

QVector<double> RWaterDemandZoneModel::getRecentVector()
{
    return theRecentVector;
}

QVector<double> RWaterDemandZoneModel::getEstablishedVector()
{
    return theEstablishedVector;
}

QVector<double> RWaterDemandZoneModel::getResidentVector()
{
    return theResidentVector;
}


void RWaterDemandZoneModel::getRequests()
{

}


int RWaterDemandZoneModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    RNormalDistribution theNormalDistribution(theDomain);

    // "Functionality vs time" plot
    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    theAvailabilityOfWater = false;
    double timeUntilWaterIsRestored = 0.0;
    double waterFunctionality = 0.0;

    // Check if water is available in the residential the (i.e., not at the residential building)
    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel == nullptr){
            qCritical() << "There is a problem with supplier list for" << this->objectName();
        }

        // Check for availability of water
        // Power is false if the response > 0 because the response means 'time until repairs are finished'
        if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
            theAvailabilityOfWater = true;

        } else {
            timeUntilWaterIsRestored = theResponseComingIntoThisModel->getCurrentValue();
        }

        waterFunctionality = theResponseComingIntoThisModel->getCurrentValue();
    }
    
    theExpectedRepairTime = qMax(theExpectedRepairTime,timeUntilWaterIsRestored);


    // Get household data for each CT
    double thePopulation = theHouseholdPortfolio->getTotalPopulation();
    double thePercentageOfLowIncome = theHouseholdPortfolio->getPercentageLowIncome();
    double thePercentageOfModerateIncome = theHouseholdPortfolio->getPercentageModerateIncome();
    double thePercentageOfImmigrant = theHouseholdPortfolio->getPercentageOfRecentImmigrants() + theHouseholdPortfolio->getPercentageOfEstablishedImmigrants();
    double thePercentageWithDisability = theHouseholdPortfolio->getPercentageWithDisability();
    double thePercentageOfHispanicHouseholds = theHouseholdPortfolio->getPercentageOfHispanicHouseholds();
    double thePercentageWithHigherEducation = theHouseholdPortfolio->getPercentageHigherEducation();
    double theImpact = 0.0;
    double thePopulationWithWater = 0.0;

    myResults.clear();
    lowIncomeInCriticalNeedOfWater = 0.0;
    modIncomeInCriticalNeedOfWater = 0.0;
    highIncomeInCriticalNeedOfWater = 0.0;
    hispanicInCriticalNeedOfWater = 0.0;
    notHispanicInCriticalNeedOfWater = 0.0;
    disabilityInCriticalNeedOfWater = 0.0;
    noDisabilityInCriticalNeedOfWater = 0.0;
    immigrantInCriticalNeedOfWater = 0.0;
    notImmigrantInCriticalNeedOfWater = 0.0;
    higherEdIncomeInCriticalNeedOfWater = 0.0;
    noHigherEdIncomeInCriticalNeedOfWater = 0.0;

    QVector<QString> theIncomeVector;
    theIncomeVector << QString("Low") << QString("Moderate") << QString("High");

    // Income
    for (int in = 0; in < theIncomeVector.size(); in++){
        int dummyLowIncome = 0.0;
        int dummyModerateIncome = 0.0;
        double alphaIncome = 0.0;
        if (in == 0){
            dummyLowIncome = 1;
            alphaIncome = thePercentageOfLowIncome;
        } else if (in == 1) {
            dummyModerateIncome = 1;
            alphaIncome = thePercentageOfModerateIncome;
        } else if (in == 2) {
            dummyModerateIncome = 1;
            alphaIncome = 1 - thePercentageOfModerateIncome - thePercentageOfLowIncome;
        }


        // Immigration status
        for (int im = 0; im < 2; im++){
            int dummyImmigrant = 0;
            double alphaImmigrant = 0.0;
            if (im == 0){
                dummyImmigrant = 0;
                alphaImmigrant = 1 - thePercentageOfImmigrant;
            } else if (im == 1) {
                dummyImmigrant = 1;
                alphaImmigrant = thePercentageOfImmigrant;
            }


            // Disability
            for (int di = 0; di < 2; di++){
                // Dummy variable for immigration status
                int dummyDisability = 0;
                double alphaDisability = 0.0;

                if (di == 0){
                    dummyDisability = 0;
                    alphaDisability = 1 - thePercentageWithDisability;
                } else if (di == 1) {
                    dummyDisability = 1;
                    alphaDisability = thePercentageWithDisability;
                }


                // Hispanic
                for (int is = 0; is < 2; is++){
                    // Dummy variable for immigration status
                    int dummyHispanic = 0;
                    double alphaHispanic = 0.0;

                    if (is == 0){
                        dummyHispanic = 0;
                        alphaHispanic = 1 - thePercentageOfHispanicHouseholds;
                    } else if (is == 1) {
                        dummyHispanic = 1;
                        alphaHispanic = thePercentageOfHispanicHouseholds;
                    }



                    // Higher education
                    for (int he = 0; he < 2; he++){
                        // Dummy variable for immigration status
                        int dummyHigherEd = 0;
                        double alphaHigherEd = 0.0;

                        if (he == 0){
                            dummyHigherEd = 0;
                            alphaHigherEd = 1 - thePercentageWithHigherEducation;
                        } else if (he == 1) {
                            dummyHigherEd = 1;
                            alphaHigherEd = thePercentageWithHigherEducation;
                        }


                        // Logit model
                        double theLogit = theta0->getCurrentValue() + dummyLowIncome * thetaLowIncome->getCurrentValue() + dummyModerateIncome * thetaModerateIncome->getCurrentValue() + dummyImmigrant * thetaImmigration->getCurrentValue() + dummyDisability * thetaDisability->getCurrentValue() + dummyHispanic * thetaHispanic->getCurrentValue() + dummyHigherEd * thetaHigherEd->getCurrentValue();
                        double theProbabilityOfHavingWater = exp(theLogit) / (1.0 + exp(theLogit));
                        double theAlpha = alphaIncome * alphaImmigrant * alphaDisability * alphaHispanic * alphaHigherEd;
                        double theAw = 7.0;

                        theImpact += theAlpha * thePopulation * (theProbabilityOfHavingWater * qMax(theExpectedRepairTime - theAw,0.0) + (1.0-theProbabilityOfHavingWater) *theExpectedRepairTime);
                        thePopulationWithWater += theAlpha * thePopulation * (theProbabilityOfHavingWater);
                        //theImpact += theAlpha * theProbabilityOfHavingWater;

                        if (dummyLowIncome == 1){
                            lowIncomeInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else if (dummyModerateIncome == 1){
                            modIncomeInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else if (dummyLowIncome == 0 && dummyModerateIncome == 0){
                            highIncomeInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        }

                        if (dummyHispanic == 1){
                            hispanicInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else {
                            notHispanicInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        }

                        if (dummyDisability == 1){
                            disabilityInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else {
                            noDisabilityInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        }

                        if (dummyImmigrant == 1){
                            immigrantInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else {
                           notImmigrantInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        }

                        if (dummyHigherEd == 1){
                            higherEdIncomeInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        } else {
                            noHigherEdIncomeInCriticalNeedOfWater += theAlpha * thePopulation * (1.0 - theProbabilityOfHavingWater);
                        }

                    } // higher education
                } // hispanic
            } // disability
        } // immigration
    } // income

    //qCritical() << this->objectName() << theExpectedRepairTime << theExpectedRepairTime * thePopulation << theImpact << "";
    qCritical() << this->objectName() << thePopulation << thePopulationWithWater / thePopulation << waterFunctionality << waterFunctionality * (1 - thePopulationWithWater / thePopulation) + 1 * thePopulationWithWater / thePopulation<< "";
    //qCritical() << this->objectName() << thePopulation << waterFunctionality << qMin((waterFunctionality) * (1.0 + thePopulationWithWater / thePopulation),1.0) << "";

    theHouseholdPortfolio->setPercentageOfRentedBuildings(theImpact);

    lastEvaluationTime = theCurrentTime->getCurrentValue();

myResults << lowIncomeInCriticalNeedOfWater << modIncomeInCriticalNeedOfWater << highIncomeInCriticalNeedOfWater;
myResults << hispanicInCriticalNeedOfWater << notHispanicInCriticalNeedOfWater << disabilityInCriticalNeedOfWater;
myResults << noDisabilityInCriticalNeedOfWater << immigrantInCriticalNeedOfWater << notImmigrantInCriticalNeedOfWater << higherEdIncomeInCriticalNeedOfWater << noHigherEdIncomeInCriticalNeedOfWater;

    return 1;
}

