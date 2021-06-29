#include "RHouseholdPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"

#include <math.h>


RHouseholdPortfolio::RHouseholdPortfolio(QObject *parent, QString name)
    : RObject(parent, name)
{
    theNumberOfBuildings = 0;
    theNumberOfSingleFamilyBuildings = 0;
    theNumberOfMultiFamilyBuildings = 0;
    inputPercentageInsured = 0.0;
    theRetrofitLevel = 0.0;
    
    this->setMunicipality(QString("SanFrancisco"));

    // Create the standard normal random variable
    theDomain->createObject(objectName() + "_theta0tenure", "RContinuousRandomVariable");
    theta0_tenure = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theta0_tenure->setDistributionType(QString("Normal (mean, stdv)"));
    theta0_tenure->setMean("-1.7177");
    theta0_tenure->setStandardDeviation("0.1138");

    theDomain->createObject(objectName() + "_thetaSF", "RContinuousRandomVariable");
    thetaSF = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    thetaSF->setDistributionType(QString("Normal (mean, stdv)"));
    thetaSF->setMean("2.8715");
    thetaSF->setStandardDeviation("0.1362");
}


RHouseholdPortfolio::~RHouseholdPortfolio()
{
    
}


int RHouseholdPortfolio::resetTime()
{
    return 0;
}

void RHouseholdPortfolio::setMunicipality(QString value)
{
    inputMunicipality = value;
    
    if (inputMunicipality == QString("Vancouver")){
        medianMunicipalHouseholdIncome = 56908.0;
        insuranceTakeUp = 0.45;
        
    } else if (inputMunicipality == QString("SanFrancisco")){
        medianMunicipalHouseholdIncome = 93448.0;
        insuranceTakeUp = 0.15;
        
    } else {
        qCritical() << "The" << this->objectName() << "could not identify the municipality. Using default values for insurace take-up rate and building values!!";
        medianMunicipalHouseholdIncome = 50000.0;
        insuranceTakeUp = 0.25;
    }
}


QObject * RHouseholdPortfolio::getLocation() const
{
    return theLocation;
}

void RHouseholdPortfolio::setLocation(QObject *value)
{
    theLocation = qobject_cast<RParameter *>(value);
    
    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theLocation);
}


QObject * RHouseholdPortfolio::getBuildingPortfolio()
{
    return theBuildingPortfolio;
}

void RHouseholdPortfolio::setBuildingPortfolio(QObject *value)
{
    theBuildingPortfolio = qobject_cast<RBuildingPortfolio*>(value);
}

QString RHouseholdPortfolio::getHouseholdSizeDistribution()
{
    return doubleVectorToString(inputHouseholdSizeVector);
}

void RHouseholdPortfolio::setHouseholdSizeDistribution(QString value)
{
    inputHouseholdSizeVector = stringToDoubleVector(value);
}

double RHouseholdPortfolio::getMedianHouseholdIncome() const
{
    return theMedianHouseholdIncome;
}

void RHouseholdPortfolio::setMedianHouseholdIncome(double value)
{
    theMedianHouseholdIncome = value;
}

double RHouseholdPortfolio::getPercentageLowIncome() const
{
    return inputPercentageLowIncome;
}

void RHouseholdPortfolio::setPercentageLowIncome(double value)
{
    inputPercentageLowIncome = value;
}


double RHouseholdPortfolio::getPercentageModerateIncome() const
{
    return inputPercentageModerateIncome;
}

void RHouseholdPortfolio::setPercentageModerateIncome(double value)
{
    inputPercentageModerateIncome = value;
}


double RHouseholdPortfolio::getPercentageHighIncome() const
{
    return inputPercentageHighIncome;
}

void RHouseholdPortfolio::setPercentageHighIncome(double value)
{
    inputPercentageHighIncome = value;
}


double RHouseholdPortfolio::getPercentageOfChildren() const
{
    return inputPercentageOfChildren;
}

void RHouseholdPortfolio::setPercentageOfChildren(double value)
{
    inputPercentageOfChildren = value;
}

double RHouseholdPortfolio::getPercentageOfSeniors() const
{
    return  inputPercentageOfSeniors;
}

void RHouseholdPortfolio::setPercentageOfSeniors(double value)
{
    inputPercentageOfSeniors = value;
}

QString RHouseholdPortfolio::getAgeDistribution()
{
    return doubleVectorToString(inputAgeDistributionVector);
}

void RHouseholdPortfolio::setAgeDistribution(QString value)
{
    inputAgeDistributionVector = stringToDoubleVector(value);
}


double RHouseholdPortfolio::getPercentageOfRentedBuildings() const
{
    return inputPercentageOfRenters;
}

void RHouseholdPortfolio::setPercentageOfRentedBuildings(double value)
{
    inputPercentageOfRenters = value;
}


double RHouseholdPortfolio::getPercentageWithMortgage() const
{
    return inputPercentageWithMortgage;
}

void RHouseholdPortfolio::setPercentageWithMortgage(double value)
{
    inputPercentageWithMortgage = value;
}

double RHouseholdPortfolio::getPercentageOfRecentImmigrants() const
{
    return inputPercentageOfRecentImmigrants;
}

void RHouseholdPortfolio::setPercentageOfRecentImmigrants(double value)
{
    inputPercentageOfRecentImmigrants = value;
}


double RHouseholdPortfolio::getPercentageOfEstablishedImmigrants() const
{
    return inputPercentageOfEstablishedImmigrants;
}

void RHouseholdPortfolio::setPercentageOfEstablishedImmigrants(double value)
{
    inputPercentageOfEstablishedImmigrants = value;
}


double RHouseholdPortfolio::getPercentageUnaffordableDwellings() const
{
    return inputPercentageOfUnaffordableDwellings;
}

void RHouseholdPortfolio::setPercentageUnaffordableDwellings(double value)
{
    inputPercentageOfUnaffordableDwellings = value;
}

double RHouseholdPortfolio::getPercentageUnsafeDwellings() const
{
    return inputPercentageOfUnsafeDwellings;
}

void RHouseholdPortfolio::setPercentageUnsafeDwellings(double value)
{
    inputPercentageOfUnsafeDwellings = value;
}


double RHouseholdPortfolio::getPercentageNotSuitableDwellings() const
{
    return inputPercentageOfNotSuitableDwellings;
}

void RHouseholdPortfolio::setPercentageNotSuitableDwellings(double value)
{
    inputPercentageOfNotSuitableDwellings = value;
}


QString RHouseholdPortfolio::getMunicipality() const
{
    return inputMunicipality;
}

QString RHouseholdPortfolio::getBoundaryPointsLatitude() const
{
    return inputBoundaryPointsLatitudes;
}

void RHouseholdPortfolio::setBoundaryPointsLatitude(QString value)
{
    inputBoundaryPointsLatitudes = value;
}


QString RHouseholdPortfolio::getBoundaryPointsLongitude() const
{
    return inputBoundaryPointsLongitudes;
}

void RHouseholdPortfolio::setBoundaryPointsLongitude(QString value)
{
    inputBoundaryPointsLongitudes = value;
}


QVector<QVector<double>> RHouseholdPortfolio::getBoundariesVector()
{
    QVector<QVector<double>> returnVector;
    QVector<double> latitudeVector = stringToDoubleVector(inputBoundaryPointsLatitudes);
    QVector<double> longitudeVector = stringToDoubleVector(inputBoundaryPointsLongitudes);
    
    for (int i = 0; i < latitudeVector.size(); i++){
        QVector<double> theCoordinates;
        
        theCoordinates << latitudeVector.at(i) << longitudeVector.at(i);
        returnVector << theCoordinates;
    }
    
    return returnVector;
}


// Extractors
int RHouseholdPortfolio::getNumberOfBuildings() const
{
    return theNumberOfBuildings;
}

int RHouseholdPortfolio::getNumberOfSingleFamilyBuildings() const
{
    return theNumberOfSingleFamilyBuildings;
}

int RHouseholdPortfolio::getNumberOfMultiFamilyBuildings() const
{
    return theNumberOfMultiFamilyBuildings;
}


QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdInsuranceStatus() const
{
    return theBuildingInsuranceVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getMortgageVector() const
{
    return householdMortgageVector;
}

double RHouseholdPortfolio::getPercentageOfHouseholdsWithCars() const
{
    return inputPercentageWithCars;
}

void RHouseholdPortfolio::setPercentageOfHouseholdsWithCars(double value)
{
    inputPercentageWithCars = value;
}


QVector<QString> RHouseholdPortfolio::getBuildingTypesVector() const
{
    return theBuildingTypesVector;
}


QVector<QString> RHouseholdPortfolio::getBuildingCodeLevelVector() const
{
    return theBuildingCodeLevelVector;
}


QVector<QString> RHouseholdPortfolio::getBuildingOccupancyClassVector() const
{
    return theBuildingOccupancyClassVector;
}


QVector<int> RHouseholdPortfolio::getBuildingStoreysVector() const
{
    return theBuildingStoreysVector;
}

QVector<int> RHouseholdPortfolio::getBuildingDwellingsVector() const
{
    return theNumberOfDwellingsPerBuilding;
}

QVector<double> RHouseholdPortfolio::getBuildingAreaVector() const
{
    return theBuildingAreasVector;
}


QVector<double> RHouseholdPortfolio::getBuildingValueVector() const
{
    return theBuildingValueVector;
}


QVector<RLocation*> RHouseholdPortfolio::getBuildingLocationVector() const
{
    return theBuildingLocationVector;
}


QVector<QVector<int>> RHouseholdPortfolio::getHouseholdSizeVector() const
{
    return householdSizeVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdAgeCategoryVector() const
{
    return householdAgeCategory;
}

QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdImmigrantStatusVector() const
{
    return householdImmigrantStatusVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdCarOwnershipVector() const
{
    return householdCarOwnershipVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdHouseholdIncomeVector() const
{
    return householdHouseholdIncomeVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdTenureVector() const
{
    return householdTenureVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdArchetypeVector() const
{
    return householdArchetypeVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdPredispositionToLeave() const
{
    return householdPredispositionToLeave;
}


int RHouseholdPortfolio::instantiateHouseholds()
{
    // -------------------------------------------------------------------------------------------
    // 0 - Instantiate the owner based on census information
    // ------------------------------------------------------------------------------------------
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Instantiating owners and owners at" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Get the buildings
    theBuildingPortfolio->instantiateBuildings();
    theNumberOfBuildings            = theBuildingPortfolio->getNumberOfBuildings();
    theBuildingStoreysVector        = theBuildingPortfolio->getBuildingStoreysVector();
    theNumberOfDwellingsPerBuilding = theBuildingPortfolio->getBuildingDwellingsVector();
    theBuildingAreasVector          = theBuildingPortfolio->getBuildingAreaVector();
    theBuildingValueVector          = theBuildingPortfolio->getBuildingValueVector();
    theBuildingTypesVector          = theBuildingPortfolio->getBuildingTypesVector();
    theBuildingOccupancyClassVector = theBuildingPortfolio->getBuildingOccupancyClassVector();
    theBuildingCodeLevelVector      = theBuildingPortfolio->getBuildingCodeLevelVector();
    theBuildingLocationVector       = theBuildingPortfolio->getBuildingLocationVector();

    for (int i = 0; i < theNumberOfBuildings; i++){

        QVector<QString> auxTenureVector;
        QVector<bool> auxMortgageVector;
        QVector<QString> auxHouseholdIncomeVector;
        QVector<QString> auxImmigrantStatusVector;
        QVector<int> auxHouseholdSizeVector;
        QVector<bool> auxCarOwnershipVector;
        QVector<QString> auxAgeCategory;
        QVector<bool> auxInsuranceCoverage;
        QVector<bool> auxPredispositionToLeave;

        for (int j = 0; j < theNumberOfDwellingsPerBuilding.at(i); j++){

            // Tenure
            QString tenure;
            // Dummy variable for renters
            int dummySF = 0.0;
            if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
                dummySF = 1.0;
            }

            double theLogit = theta0_tenure->getCurrentValue() + dummySF * thetaSF->getCurrentValue();
            double theProbability = exp(theLogit) / (1.0 + exp(theLogit));
            double rng = rand() / static_cast<double>(RAND_MAX);

            if (rng < theProbability){
                tenure = QString("Owner");
            } else {
                tenure = QString("Renter");
            }


            QString income;
            double rngIncome = rand()/static_cast<double>(RAND_MAX);

            bool mortgage = false;
            double rngMortgage = rand()/static_cast<double>(RAND_MAX);

            // If this is an owner household
            if (tenure == QString("Owner")){

                // For Vancouver, use the relationship between tenure and income
                if (inputMunicipality == QString("Vancouver")){
                    double myProbabilityOfLowIncomeGivenOwner;
                    double myProbabilityOfModerateIncomeGivenOwner;
                    double myProbabilityOfHighIncomeGivenOwner;

                    // make sure the sum of probabilities is not > 1 and that probabilities are not negative
                    double sumOfLikelihoods = qMax((-3.5075/1000000.0) * theMedianHouseholdIncome + 0.64113,0.0) + qMax((-3.576/10000000.0) * theMedianHouseholdIncome + 0.3248,0.0) + qMax((3.802/1000000.0) * theMedianHouseholdIncome - 0.0489,0.0);

                    myProbabilityOfLowIncomeGivenOwner = qMax((-3.5075/1000000.0) * theMedianHouseholdIncome + 0.64113,0.0) / sumOfLikelihoods;
                    myProbabilityOfModerateIncomeGivenOwner = qMax((-3.576/10000000.0) * theMedianHouseholdIncome + 0.3248,0.0) / sumOfLikelihoods;
                    myProbabilityOfHighIncomeGivenOwner = qMax((3.802/1000000.0) * theMedianHouseholdIncome - 0.0489,0.0) / sumOfLikelihoods;

                    if (rngIncome < myProbabilityOfLowIncomeGivenOwner){
                        income = QString("Low");

                    } else if (rngIncome >= myProbabilityOfLowIncomeGivenOwner && rngIncome < myProbabilityOfLowIncomeGivenOwner + myProbabilityOfModerateIncomeGivenOwner){
                        income = QString("Moderate");

                    } else if (rngIncome >= myProbabilityOfLowIncomeGivenOwner + myProbabilityOfModerateIncomeGivenOwner){
                        income = QString("High");
                    }

                    // For San Francisco
                } else if (inputMunicipality == QString("SanFrancisco")){
                    double probLowIncome = 0.304; // Calculate from PUFs from American Housing Survey
                    double probModerateIncome = 0.172;
                    double probHighIncome = 0.524;

                    if (rngIncome < probLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= probLowIncome && rngIncome < probLowIncome + probModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= probLowIncome + probModerateIncome){
                        income = QString("High");
                    }

                    // If a municipality in the Bay Area but not San Francisco
                } else if (inputMunicipality == QString("BayArea")){
                    double probLowIncome = 0.317; // Calculate from PUFs from American Housing Survey
                    double probModerateIncome = 0.178;
                    double probHighIncome = 0.504;

                    if (rngIncome < probLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= probLowIncome && rngIncome < probLowIncome + probModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= probLowIncome + probModerateIncome){
                        income = QString("High");
                    }

                } else if (1>2) {

                    // For other municipalities for which we do not have better information
                    // consider tenure and income independent
                } else {
                    if (rngIncome < inputPercentageLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= inputPercentageLowIncome && rngIncome < inputPercentageLowIncome + inputPercentageModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= inputPercentageLowIncome + inputPercentageModerateIncome){
                        income = QString("High");
                    }
                }

                // Mortgage
                if (rngMortgage < inputPercentageWithMortgage){
                    mortgage = true;
                }

                // If this is a renter-occupied building
            } else if (tenure == QString("Renter")){

                // Income
                if (inputMunicipality == QString("Vancouver")){

                    double myProbabilityOfLowIncomeGivenRenter;
                    double myProbabilityOfModerateIncomeGivenRenter;
                    double myProbabilityOfHighIncomeGivenRenter;

                    // make sure the sum of probabilities is not > 1 and that probabilities are not negative
                    double sumOfLikelihoods = qMax((-7.72/1000000.0) * theMedianHouseholdIncome + 0.974837,0.0) + qMax((4.50/1000000.0) * theMedianHouseholdIncome + 0.0629,0.0) + qMax((3.365/1000000.0) * theMedianHouseholdIncome - 0.0241,0.0);

                    myProbabilityOfLowIncomeGivenRenter = qMax((-7.72/1000000.0) * theMedianHouseholdIncome + 0.974837,0.0) / sumOfLikelihoods;
                    myProbabilityOfModerateIncomeGivenRenter = qMax((-4.50/1000000.0) * theMedianHouseholdIncome + 0.0629,0.0) / sumOfLikelihoods;
                    myProbabilityOfHighIncomeGivenRenter = qMax((3.365/1000000.0) * theMedianHouseholdIncome - 0.0241,0.0) / sumOfLikelihoods;

                    if (rngIncome < myProbabilityOfLowIncomeGivenRenter){
                        income = QString("Low");
                    } else if (rngIncome >= myProbabilityOfLowIncomeGivenRenter && rngIncome < myProbabilityOfLowIncomeGivenRenter + myProbabilityOfModerateIncomeGivenRenter){
                        income = QString("Moderate");
                    } else if (rngIncome >= myProbabilityOfLowIncomeGivenRenter + myProbabilityOfModerateIncomeGivenRenter){
                        income = QString("High");
                    }

                    // For San Francisco
                } else if (inputMunicipality == QString("SanFrancisco")){
                    double probLowIncome = 0.552; // Calculate from PUFs from American Housing Survey
                    double probModerateIncome = 0.145;
                    double probHighIncome = 0.302;

                    if (rngIncome < probLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= probLowIncome && rngIncome < probLowIncome + probModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= probLowIncome + probModerateIncome){
                        income = QString("High");
                    }

                    // If a municipality in the Bay Area but not San Francisco
                } else if (inputMunicipality == QString("BayArea")){
                    double probLowIncome = 0.507; // Calculate from PUFs from American Housing Survey
                    double probModerateIncome = 0.187;
                    double probHighIncome = 0.306;

                    if (rngIncome < probLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= probLowIncome && rngIncome < probLowIncome + probModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= probLowIncome + probModerateIncome){
                        income = QString("High");
                    }

                    // For other municipalities for which we do not have better information
                    // consider tenure and income independent
                } else {
                    if (rngIncome < inputPercentageLowIncome){
                        income = QString("Low");

                    } else if (rngIncome >= inputPercentageLowIncome && rngIncome < inputPercentageLowIncome + inputPercentageModerateIncome){
                        income = QString("Moderate");

                    } else if (rngIncome >= inputPercentageLowIncome + inputPercentageModerateIncome){
                        income = QString("High");
                    }
                }
            } // end if tenure


            // Immigrant status
            QString immigrantStatus;
            double rngImmigrant = rand()/static_cast<double>(RAND_MAX);

            if (rngImmigrant <= inputPercentageOfRecentImmigrants){
                immigrantStatus = QString("RecentImmigrant");
            }
            else if (rngImmigrant > inputPercentageOfRecentImmigrants && rngImmigrant <= inputPercentageOfRecentImmigrants + inputPercentageOfEstablishedImmigrants){
                immigrantStatus = QString("EstablishedImmigrant");
            }
            else {
                immigrantStatus = QString("Resident");
            }


            // Household size
            double rngSize = rand()/static_cast<double>(RAND_MAX);
            int theHouseholdSize = 0;

            // 0 - one member
            if (rngSize <= inputHouseholdSizeVector.at(0)){
                theHouseholdSize = 1;

                // 1 - 2 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1)) {
                theHouseholdSize = 2;

                // 2 - 3 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2)) {
                theHouseholdSize = 3;

                // 3 - 4 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) + inputHouseholdSizeVector.at(3)) {
                theHouseholdSize = 4;

                // 4 - 5+ members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) + inputHouseholdSizeVector.at(3)) {
                theHouseholdSize = 5;
            }


            // Vulnerable population
            QString ageCategory;
            double rngChild = rand()/static_cast<double>(RAND_MAX);
            double rngSenior = rand()/static_cast<double>(RAND_MAX);
            double rngChildAndSenior = rand()/static_cast<double>(RAND_MAX);
            double probChildrenOrSenior = inputPercentageOfChildren + inputPercentageOfSeniors - inputPercentageOfChildren * inputPercentageOfSeniors;

            ageCategory = QString("None");

            if (theHouseholdSize > 2 && rngChildAndSenior < probChildrenOrSenior){
                ageCategory = QString("ChildrenOrSenior");

            } else if (theHouseholdSize > 1 && rngChild < inputPercentageOfChildren){
                ageCategory = QString("Children");

            } else if (rngSenior < inputPercentageOfSeniors){
                ageCategory = QString("Senior");
            }

            // Insurance coverage
            // The dispersion for the income distribution is chosen so that 5% make more 100k - that is roughly the case for Vancouver
            // The dispersion for the insurance percentage is chosen so that P(1.0) ~ 1
            inputPercentageInsured = exp(((1.0/0.35) * qLn(theMedianHouseholdIncome / medianMunicipalHouseholdIncome)) * 0.25 + qLn(insuranceTakeUp));

            double rngInsurance = rand()/static_cast<double>(RAND_MAX);
            if (inputMunicipality == QString("Vancouver") && theBuildingOccupancyClassVector.at(i).at(j) == QString("RES3")){
                auxInsuranceCoverage << true;

            } else if (rngInsurance <= inputPercentageInsured){
                auxInsuranceCoverage << true;
            }
            else {
                auxInsuranceCoverage << false;
            }

            // Car ownership
            bool carOwnership;
            double rngCar = rand()/static_cast<double>(RAND_MAX);

            carOwnership = false;
            if (rngCar < inputPercentageWithCars){
                carOwnership = true;
            }

            // Core housing need
            double rngAffordability = rand()/static_cast<double>(RAND_MAX);
            double rngSafety = rand()/static_cast<double>(RAND_MAX);
            double rngSuitability = rand()/static_cast<double>(RAND_MAX);

            bool preDispositionToLeavePermanently = false;

            // If unsafe
            if (rngSafety < inputPercentageOfUnsafeDwellings && auxHouseholdIncomeVector.at(j) != QString("Low")){
                preDispositionToLeavePermanently = true;

                // If unsuitable
            } else if (rngSuitability < inputPercentageOfNotSuitableDwellings && auxHouseholdIncomeVector.at(j) != QString("Low")){
                preDispositionToLeavePermanently = true;

                // If unsuitable
            } else if (rngAffordability < inputPercentageOfUnaffordableDwellings){
                preDispositionToLeavePermanently = true;
            }

            auxTenureVector << tenure;
            auxMortgageVector << mortgage;
            auxHouseholdIncomeVector << income;
            auxImmigrantStatusVector << immigrantStatus;
            auxHouseholdSizeVector << theHouseholdSize;
            auxAgeCategory << ageCategory;
            auxCarOwnershipVector << carOwnership;
            auxPredispositionToLeave << preDispositionToLeavePermanently;
        } // j


        if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
            theNumberOfSingleFamilyBuildings++;
        } else {
            theNumberOfMultiFamilyBuildings++;
        }

        householdTenureVector << auxTenureVector;
        householdMortgageVector << auxMortgageVector;
        householdHouseholdIncomeVector << auxHouseholdIncomeVector;
        householdImmigrantStatusVector << auxImmigrantStatusVector;
        householdSizeVector << auxHouseholdSizeVector;
        householdAgeCategory << auxAgeCategory;
        householdCarOwnershipVector << auxCarOwnershipVector;
        theBuildingInsuranceVector << auxInsuranceCoverage;
        householdPredispositionToLeave << auxPredispositionToLeave;
    } // i

    return 1;
}

