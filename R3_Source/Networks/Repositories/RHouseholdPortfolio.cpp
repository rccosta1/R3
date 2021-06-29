
#include "RHouseholdPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"
#include "RDomain.h"

#include <math.h>


RHouseholdPortfolio::RHouseholdPortfolio(QObject *parent, QString name)
    : RObject(parent, name)
{
    theBuildingPortfolio = nullptr;

    theLocation = nullptr;
    theTotalPopulation = 0;
    theMedianHouseholdIncome = 0.0;
    inputPercentageLowIncome = 0.0;
    inputPercentageModerateIncome = 0.0;
    inputPercentageHighIncome = 0.0;
    inputPercentageOfMarriedHouseholds = 0.0;
    inputPercentageOfMaleHouseholders = 0.0;
    inputPercentageOfChildren = 0.0;
    inputPercentageOfSeniors = 0.0;
    inputPercentageOfRenters = 0.0;
    inputPercentageWithMortgage = 0.0;
    inputPercentageWithCars = 0.0;
    inputPercentageInsured = 0.0;
    inputPercentageOfRecentImmigrants = 0.0;
    inputPercentageOfEstablishedImmigrants = 0.0;
    inputPercentageRecentlyMovedIn = 0.0;
    inputPercentageOfUnaffordableOwnerDwellings = 0.0;
    inputPercentageOfUnaffordableRenterDwellings = 0.0;
    inputPercentageOfFinancialyStrainedOwnerDwellings = 0.0;
    inputPercentageOfFinancialyStrainedRenterDwellings = 0.0;
    inputPercentageOfUnsafeDwellings = 0.0;
    inputPercentageOfOvercrowdedOwnerDwellings = 0.0;
    inputPercentageOfOvercrowdedRenterDwellings = 0.0;
    inputPercentageUsedFoodStampsDwellings = 0.0;
    inputPercentageOfWhiteHouseholds = 0.0;
    inputPercentageOfBlackHouseholds = 0.0;
    inputPercentageOfAsianHouseholds = 0.0;
    inputPercentageOfNativeAmericanHouseholds = 0.0;
    inputPercentageOfWhiteHispanicHouseholds = 0.0;
    inputPercentageOfBlackHispanicHouseholds = 0.0;
    inputPercentageOfAsianHispanicHouseholds = 0.0;
    inputPercentageOfNativeAmericanHispanicHouseholds = 0.0;
    inputPercentageHigherEducation = 0.0;
    inputPercentageWithDisability = 0.0;
    medianMunicipalHouseholdIncome = 0.0;
    insuranceTakeUp = 0.0;

    inputBoundaryPointsLatitudes = "";
    inputBoundaryPointsLongitudes = "";
    inputMunicipality = "";
    inputBuildingDensity = "";
    theInstantiationMethod = "";

    // Properties of the buildings
    theNumberOfBuildings = 0;
    theNumberOfSingleFamilyBuildings = 0;
    theNumberOfMultiFamilyBuildings = 0;

    theBuildingLocation = nullptr;
    theDomain->createObject(objectName() + "BuildingLocation" , "RLocation");
    theBuildingLocation = qobject_cast<RLocation *>(theDomain->getLastAddedObject());

    this->setMunicipality(QString("SanFrancisco"));

    // Set the two uniformly distributed random variables (theta1, beta2)
    theDomain->createObject(objectName() + "_theta1", "RContinuousRandomVariable");
    theLocationTheta1 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theLocationTheta1->setDistributionType(QString("Uniform (a, b)"));
    theLocationTheta1->setParameter1("0.0");
    theLocationTheta1->setParameter2("1.0");

    theDomain->createObject(objectName() + "_theta2", "RContinuousRandomVariable");
    theLocationTheta2 = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theLocationTheta2->setDistributionType(QString("Uniform (a, b)"));
    theLocationTheta2->setParameter1("0.0");
    theLocationTheta2->setParameter2("1.0");

    // High income
    //    theDomain->createObject(objectName() + "_theta0_HighIncome", "RContinuousRandomVariable");
    //    beta0_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_HighIncome->setMean("-1.59");
    //    beta0_HighIncome->setStandardDeviation("0.11");

    //    theDomain->createObject(objectName() + "_thetaRenter_HighIncome", "RContinuousRandomVariable");
    //    betaRenter_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaRenter_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaRenter_HighIncome->setMean("-1.00");
    //    betaRenter_HighIncome->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaSize_HighIncome", "RContinuousRandomVariable");
    //    betaSize_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSize_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSize_HighIncome->setMean("0.51");
    //    betaSize_HighIncome->setStandardDeviation("0.02");

    //    theDomain->createObject(objectName() + "_thetaChildren_HighIncome", "RContinuousRandomVariable");
    //    betaChildren_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaChildren_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaChildren_HighIncome->setMean("0.93");
    //    betaChildren_HighIncome->setStandardDeviation("0.07");

    //    theDomain->createObject(objectName() + "_thetaElderly_HighIncome", "RContinuousRandomVariable");
    //    betaElderly_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_HighIncome->setMean("-0.87");
    //    betaElderly_HighIncome->setStandardDeviation("0.06");

    //    theDomain->createObject(objectName() + "_thetaImmigration_HighIncome", "RContinuousRandomVariable");
    //    betaImmigration_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaImmigration_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaImmigration_HighIncome->setMean("-2.13");
    //    betaImmigration_HighIncome->setStandardDeviation("0.10");

    //    theDomain->createObject(objectName() + "_thetaHispanic_HighIncome", "RContinuousRandomVariable");
    //    betaHispanic_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaHispanic_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaHispanic_HighIncome->setMean("-0.86");
    //    betaHispanic_HighIncome->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaMoved_HighIncome", "RContinuousRandomVariable");
    //    betaMoved_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_HighIncome->setMean("0.74");
    //    betaMoved_HighIncome->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaMarried_HighIncome", "RContinuousRandomVariable");
    //    betaMarried_HighIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMarried_HighIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMarried_HighIncome->setMean("0.76");
    //    betaMarried_HighIncome->setStandardDeviation("0.05");

    //    // Low income
    //    theDomain->createObject(objectName() + "_theta0_LowIncome", "RContinuousRandomVariable");
    //    beta0_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_LowIncome->setMean("1.13");
    //    beta0_LowIncome->setStandardDeviation("0.13");

    //    theDomain->createObject(objectName() + "_thetaPre1940_LowIncome", "RContinuousRandomVariable");
    //    betaPre1940_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaPre1940_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaPre1940_LowIncome->setMean("-0.34");
    //    betaPre1940_LowIncome->setStandardDeviation("0.07");

    //    theDomain->createObject(objectName() + "_thetaPre1980_LowIncome", "RContinuousRandomVariable");
    //    betaPre1980_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaPre1980_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaPre1980_LowIncome->setMean("-0.15");
    //    betaPre1980_LowIncome->setStandardDeviation("0.07");

    //    theDomain->createObject(objectName() + "_thetaRenter_LowIncome", "RContinuousRandomVariable");
    //    betaRenter_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaRenter_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaRenter_LowIncome->setMean("1.11");
    //    betaRenter_LowIncome->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaSize_LowIncome", "RContinuousRandomVariable");
    //    betaSize_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSize_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSize_LowIncome->setMean("-0.59");
    //    betaSize_LowIncome->setStandardDeviation("0.03");

    //    theDomain->createObject(objectName() + "_thetaChildren_LowIncome", "RContinuousRandomVariable");
    //    betaChildren_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaChildren_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaChildren_LowIncome->setMean("-1.12");
    //    betaChildren_LowIncome->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaElderly_LowIncome", "RContinuousRandomVariable");
    //    betaElderly_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_LowIncome->setMean("0.94");
    //    betaElderly_LowIncome->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaImmigration_LowIncome", "RContinuousRandomVariable");
    //    betaImmigration_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaImmigration_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaImmigration_LowIncome->setMean("1.86");
    //    betaImmigration_LowIncome->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaHispanic_LowIncome", "RContinuousRandomVariable");
    //    betaHispanic_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaHispanic_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaHispanic_LowIncome->setMean("0.82");
    //    betaHispanic_LowIncome->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaMoved_LowIncome", "RContinuousRandomVariable");
    //    betaMoved_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_LowIncome->setMean("-0.54");
    //    betaMoved_LowIncome->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaMarried_LowIncome", "RContinuousRandomVariable");
    //    betaMarried_LowIncome = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMarried_LowIncome->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMarried_LowIncome->setMean("-0.79");
    //    betaMarried_LowIncome->setStandardDeviation("0.05");


    //    //Mortgage
    //    theDomain->createObject(objectName() + "_theta0Mortgage", "RContinuousRandomVariable");
    //    beta0_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_Mortgage->setMean("1.07");
    //    beta0_Mortgage->setStandardDeviation("0.10");

    //    theDomain->createObject(objectName() + "_thetaChildren_Mortgage", "RContinuousRandomVariable");
    //    betaChildren_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaChildren_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaChildren_Mortgage->setMean("-0.67");
    //    betaChildren_Mortgage->setStandardDeviation("0.09");

    //    theDomain->createObject(objectName() + "_thetaElderly_Mortgage", "RContinuousRandomVariable");
    //    betaElderly_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_Mortgage->setMean("-0.88");
    //    betaElderly_Mortgage->setStandardDeviation("0.07");

    //    theDomain->createObject(objectName() + "_thetaImmigration_Mortgage", "RContinuousRandomVariable");
    //    betaImmigration_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaImmigration_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaImmigration_Mortgage->setMean("-0.50");
    //    betaImmigration_Mortgage->setStandardDeviation("0.11");

    //    theDomain->createObject(objectName() + "_thetaMoved_Mortgage", "RContinuousRandomVariable");
    //    betaMoved_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_Mortgage->setMean("0.50");
    //    betaMoved_Mortgage->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaIncome_Mortgage", "RContinuousRandomVariable");
    //    betaIncome_Mortgage = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaIncome_Mortgage->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaIncome_Mortgage->setMean("0.79");
    //    betaIncome_Mortgage->setStandardDeviation("0.07");

    //    // Overcrowding
    //    theDomain->createObject(objectName() + "_theta0_Overcrowding", "RContinuousRandomVariable");
    //    beta0_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_Overcrowding->setMean("-7.31");
    //    beta0_Overcrowding->setStandardDeviation("0.28");

    //    theDomain->createObject(objectName() + "_thetaRenter_Overcrowding", "RContinuousRandomVariable");
    //    betaRenter_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaRenter_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaRenter_Overcrowding->setMean("1.01");
    //    betaRenter_Overcrowding->setStandardDeviation("0.14");

    //    theDomain->createObject(objectName() + "_thetaSize_Overcrowding", "RContinuousRandomVariable");
    //    betaSize_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSize_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSize_Overcrowding->setMean("1.60");
    //    betaSize_Overcrowding->setStandardDeviation("0.05");

    //    theDomain->createObject(objectName() + "_thetaElderly_Overcrowding", "RContinuousRandomVariable");
    //    betaElderly_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_Overcrowding->setMean("1.09");
    //    betaElderly_Overcrowding->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaImmigration_Overcrowding", "RContinuousRandomVariable");
    //    betaImmigration_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaImmigration_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaImmigration_Overcrowding->setMean("0.89");
    //    betaImmigration_Overcrowding->setStandardDeviation("0.14");

    //    theDomain->createObject(objectName() + "_thetaHigherEd_Overcrowding", "RContinuousRandomVariable");
    //    betaHigherEd_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaHigherEd_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaHigherEd_Overcrowding->setMean("-1.13");
    //    betaHigherEd_Overcrowding->setStandardDeviation("0.13");

    //    theDomain->createObject(objectName() + "_thetaSingleFamily_Overcrowding", "RContinuousRandomVariable");
    //    betaSingleFamily_Overcrowding = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSingleFamily_Overcrowding->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSingleFamily_Overcrowding->setMean("-1.12");
    //    betaSingleFamily_Overcrowding->setStandardDeviation("0.14");

    //    // Affordability
    //    theDomain->createObject(objectName() + "_theta0_Affordability", "RContinuousRandomVariable");
    //    beta0_Affordability = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_Affordability->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_Affordability->setMean("-1.46");
    //    beta0_Affordability->setStandardDeviation("0.11");

    //    theDomain->createObject(objectName() + "_thetaLowIncome_Affordability", "RContinuousRandomVariable");
    //    betaLowIncome_Affordability = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaLowIncome_Affordability->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaLowIncome_Affordability->setMean("2.66");
    //    betaLowIncome_Affordability->setStandardDeviation("0.14");

    //    theDomain->createObject(objectName() + "_thetaModerateIncome_Affordability", "RContinuousRandomVariable");
    //    betaModerateIncome_Affordability = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaModerateIncome_Affordability->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaModerateIncome_Affordability->setMean("1.50");
    //    betaModerateIncome_Affordability->setStandardDeviation("0.17");

    //    theDomain->createObject(objectName() + "_thetaMoved_Affordability", "RContinuousRandomVariable");
    //    betaMoved_Affordability = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_Affordability->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_Affordability->setMean("-0.63");
    //    betaMoved_Affordability->setStandardDeviation("0.13");


    //    // Unsafe building
    //    theDomain->createObject(objectName() + "_theta0_Unsafe", "RContinuousRandomVariable");
    //    beta0_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_Unsafe->setMean("-3.53");
    //    beta0_Unsafe->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaRenter_Unsafe", "RContinuousRandomVariable");
    //    betaRenter_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaRenter_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaRenter_Unsafe->setMean("1.94");
    //    betaRenter_Unsafe->setStandardDeviation("0.10");

    //    theDomain->createObject(objectName() + "_thetaLowIncome_Unsafe", "RContinuousRandomVariable");
    //    betaLowIncome_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaLowIncome_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaLowIncome_Unsafe->setMean("0.71");
    //    betaLowIncome_Unsafe->setStandardDeviation("0.09");

    //    theDomain->createObject(objectName() + "_thetaModerateIncome_Unsafe", "RContinuousRandomVariable");
    //    betaModerateIncome_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaModerateIncome_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaModerateIncome_Unsafe->setMean("0.45");
    //    betaModerateIncome_Unsafe->setStandardDeviation("0.11");

    //    theDomain->createObject(objectName() + "_thetaMoved_Unsafe", "RContinuousRandomVariable");
    //    betaMoved_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_Unsafe->setMean("1.04");
    //    betaMoved_Unsafe->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaChildren_Unsafe", "RContinuousRandomVariable");
    //    betaChildren_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaChildren_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaChildren_Unsafe->setMean("-0.43");
    //    betaChildren_Unsafe->setStandardDeviation("0.09");

    //    theDomain->createObject(objectName() + "_thetaElderly_Unsafe", "RContinuousRandomVariable");
    //    betaElderly_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_Unsafe->setMean("-0.82");
    //    betaElderly_Unsafe->setStandardDeviation("0.09");

    //    theDomain->createObject(objectName() + "_thetaDisability_Unsafe", "RContinuousRandomVariable");
    //    betaDisability_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaDisability_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaDisability_Unsafe->setMean("0.56");
    //    betaDisability_Unsafe->setStandardDeviation("0.09");

    //    theDomain->createObject(objectName() + "_thetaWhite_Unsafe", "RContinuousRandomVariable");
    //    betaWhite_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaWhite_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaWhite_Unsafe->setMean("0.79");
    //    betaWhite_Unsafe->setStandardDeviation("0.08");

    //    theDomain->createObject(objectName() + "_thetaMale_Unsafe", "RContinuousRandomVariable");
    //    betaMale_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMale_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMale_Unsafe->setMean("0.86");
    //    betaMale_Unsafe->setStandardDeviation("0.07");

    //    theDomain->createObject(objectName() + "_thetaSingleFamily_Unsafe", "RContinuousRandomVariable");
    //    betaSingleFamily_Unsafe = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSingleFamily_Unsafe->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSingleFamily_Unsafe->setMean("-0.61");
    //    betaSingleFamily_Unsafe->setStandardDeviation("0.08");

    //    // Neighborhood satisfaction
    //    theDomain->createObject(objectName() + "_thetaBldType_Neighborhood", "RContinuousRandomVariable");
    //    beta0_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    beta0_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    beta0_Neighborhood->setMean("4.64");
    //    beta0_Neighborhood->setStandardDeviation("0.25");

    //    theDomain->createObject(objectName() + "_thetaRenter_Neighborhood", "RContinuousRandomVariable");
    //    betaRenter_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaRenter_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaRenter_Neighborhood->setMean("-1.45");
    //    betaRenter_Neighborhood->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaLowIncome_Neighborhood", "RContinuousRandomVariable");
    //    betaLowIncome_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaLowIncome_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaLowIncome_Neighborhood->setMean("-1.58");
    //    betaLowIncome_Neighborhood->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaModerateIncome_Neighborhood", "RContinuousRandomVariable");
    //    betaModerateIncome_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaModerateIncome_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaModerateIncome_Neighborhood->setMean("0.05");
    //    betaModerateIncome_Neighborhood->setStandardDeviation("0.22");

    //    theDomain->createObject(objectName() + "_thetaSize_Neighborhood", "RContinuousRandomVariable");
    //    betaSize_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaSize_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaSize_Neighborhood->setMean("-0.39");
    //    betaSize_Neighborhood->setStandardDeviation("0.04");

    //    theDomain->createObject(objectName() + "_thetaMoved_Neighborhood", "RContinuousRandomVariable");
    //    betaMoved_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaMoved_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaMoved_Neighborhood->setMean("0.67");
    //    betaMoved_Neighborhood->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaChildren_Neighborhood", "RContinuousRandomVariable");
    //    betaChildren_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaChildren_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaChildren_Neighborhood->setMean("-1.17");
    //    betaChildren_Neighborhood->setStandardDeviation("0.13");

    //    theDomain->createObject(objectName() + "_thetaElderly_Neighborhood", "RContinuousRandomVariable");
    //    betaElderly_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaElderly_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaElderly_Neighborhood->setMean("1.55");
    //    betaElderly_Neighborhood->setStandardDeviation("0.15");

    //    theDomain->createObject(objectName() + "_thetaDisability_Neighborhood", "RContinuousRandomVariable");
    //    betaDisability_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaDisability_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaDisability_Neighborhood->setMean("-3.51");
    //    betaDisability_Neighborhood->setStandardDeviation("0.14");

    //    theDomain->createObject(objectName() + "_thetaWhite_Neighborhood", "RContinuousRandomVariable");
    //    betaWhite_Neighborhood = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    //    betaWhite_Neighborhood->setDistributionType(QString("Normal (mean, stdv)"));
    //    betaWhite_Neighborhood->setMean("1.76");
    //    betaWhite_Neighborhood->setStandardDeviation("0.12");
}


RHouseholdPortfolio::~RHouseholdPortfolio()
{

}


int RHouseholdPortfolio::resetTime()
{
    theBoundaryLocationList.clear();
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


//QObject * RHouseholdPortfolio::getBuildingPortfolio()
//{
//    return theBuildingPortfolio;
//}

//void RHouseholdPortfolio::setBuildingPortfolio(QObject *value)
//{
//    theBuildingPortfolio = qobject_cast<RBuildingPortfolio*>(value);
//}

QString RHouseholdPortfolio::getHouseholdSizeDistribution()
{
    return doubleVectorToString(inputHouseholdSizeVector);
}

QString RHouseholdPortfolio::getInstantiationMethod()
{
    return theInstantiationMethod;
}
void RHouseholdPortfolio::setInstantiationMethod(QString value)
{
    theInstantiationMethod = value;
}

void RHouseholdPortfolio::setHouseholdSizeDistribution(QString value)
{
    inputHouseholdSizeVector = stringToDoubleVector(value);
}

int RHouseholdPortfolio::getTotalPopulation() const
{
    return theTotalPopulation;
}

void RHouseholdPortfolio::setTotalPopulation(int value)
{
    theTotalPopulation = value;
}

double RHouseholdPortfolio::getPercentageSingleFamilyDwellings() const
{
    return inputPercentageSingleFamily;
}

void RHouseholdPortfolio::setPercentageSingleFamilyDwellings(double value)
{
    inputPercentageSingleFamily = value;
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

double RHouseholdPortfolio::getPercentageMarried() const
{
    return inputPercentageOfMarriedHouseholds;
}

void RHouseholdPortfolio::setPercentageMarried(double value)
{
    inputPercentageOfMarriedHouseholds = value;
}

double RHouseholdPortfolio::getPercentageMaleHouseholders() const
{
    return inputPercentageOfMaleHouseholders;
}

void RHouseholdPortfolio::setPercentageMaleHouseholders(double value)
{
    inputPercentageOfMaleHouseholders = value;
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

QString RHouseholdPortfolio::getHousingCostDistribution()
{
    return doubleVectorToString(inputHousingCostDistributionVector);
}

void RHouseholdPortfolio::setHousingCostDistribution(QString value)
{
    inputHousingCostDistributionVector =  stringToDoubleVector(value);
}


double RHouseholdPortfolio::getPercentageOfWhiteHouseholds() const
{
    return inputPercentageOfWhiteHouseholds;
}

void RHouseholdPortfolio::setPercentageOfWhiteHouseholds(double value)
{
    inputPercentageOfWhiteHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfBlackHouseholds() const
{
    return inputPercentageOfBlackHouseholds;
}

void RHouseholdPortfolio::setPercentageOfBlackHouseholds(double value)
{
    inputPercentageOfBlackHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfAsianHouseholds() const
{
    return inputPercentageOfAsianHouseholds;
}

void RHouseholdPortfolio::setPercentageOfAsianHouseholds(double value)
{
    inputPercentageOfAsianHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfNativeAmericanHouseholds() const
{
    return inputPercentageOfNativeAmericanHouseholds;
}

void RHouseholdPortfolio::setPercentageOfNativeAmericanHouseholds(double value)
{
    inputPercentageOfNativeAmericanHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfWhiteHispanicHouseholds() const
{
    return inputPercentageOfWhiteHispanicHouseholds;
}

void RHouseholdPortfolio::setPercentageOfWhiteHispanicHouseholds(double value)
{
    inputPercentageOfWhiteHispanicHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfBlackHispanicHouseholds() const
{
    return inputPercentageOfBlackHispanicHouseholds;
}

void RHouseholdPortfolio::setPercentageOfBlackHispanicHouseholds(double value)
{
    inputPercentageOfBlackHispanicHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfAsianHispanicHouseholds() const
{
    return inputPercentageOfAsianHispanicHouseholds;
}

void RHouseholdPortfolio::setPercentageOfAsianHispanicHouseholds(double value)
{
    inputPercentageOfAsianHispanicHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfNativeAmericanHispanicHouseholds() const
{
    return inputPercentageOfNativeAmericanHispanicHouseholds;
}

void RHouseholdPortfolio::setPercentageOfNativeAmericanHispanicHouseholds(double value)
{
    inputPercentageOfNativeAmericanHispanicHouseholds = value;
}

double RHouseholdPortfolio::getPercentageOfHispanicHouseholds() const
{
    return inputPercentageOfWhiteHispanicHouseholds + inputPercentageOfBlackHispanicHouseholds + inputPercentageOfAsianHispanicHouseholds + inputPercentageOfNativeAmericanHispanicHouseholds;
}

double RHouseholdPortfolio::getPercentageHigherEducation() const
{
    return inputPercentageHigherEducation;
}

void RHouseholdPortfolio::setPercentageHigherEducation(double value)
{
    inputPercentageHigherEducation = value;
}

double RHouseholdPortfolio::getPercentageWithDisability() const
{
    return inputPercentageWithDisability;
}

void RHouseholdPortfolio::setPercentageWithDisability(double value)
{
    inputPercentageWithDisability = value;
}

double RHouseholdPortfolio::getPercentageOfRentedBuildings() const
{
    return inputPercentageOfRenters;
}

void RHouseholdPortfolio::setPercentageOfRentedBuildings(double value)
{
    inputPercentageOfRenters = value;
}

double RHouseholdPortfolio::getAverageRent() const
{
    return inputAverageRent;
}

void RHouseholdPortfolio::setAverageRent(double value)
{
    inputAverageRent = value;
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

double RHouseholdPortfolio::getPercentageRecentlyMovedIn() const
{
    return inputPercentageRecentlyMovedIn;
}

void RHouseholdPortfolio::setPercentageRecentlyMovedIn(double value)
{
    inputPercentageRecentlyMovedIn = value;
}


double RHouseholdPortfolio::getPercentageUnaffordableOwnerDwellings() const
{
    return inputPercentageOfUnaffordableOwnerDwellings;
}

void RHouseholdPortfolio::setPercentageUnaffordableOwnerDwellings(double value)
{
    inputPercentageOfUnaffordableOwnerDwellings = value;
}

double RHouseholdPortfolio::getPercentageUnaffordableRenterDwellings() const
{
    return inputPercentageOfUnaffordableRenterDwellings;
}

void RHouseholdPortfolio::setPercentageUnaffordableRenterDwellings(double value)
{
    inputPercentageOfUnaffordableRenterDwellings = value;
}

double RHouseholdPortfolio::getPercentageFinancialyStrainedOwnerDwellings() const
{
    return inputPercentageOfFinancialyStrainedOwnerDwellings;
}

void RHouseholdPortfolio::setPercentageFinancialyStrainedOwnerDwellings(double value)
{
    inputPercentageOfFinancialyStrainedOwnerDwellings = value;
}

double RHouseholdPortfolio::getPercentageFinancialyStrainedRenterDwellings() const
{
    return inputPercentageOfFinancialyStrainedRenterDwellings;
}

void RHouseholdPortfolio::setPercentageFinancialyStrainedRenterDwellings(double value)
{
    inputPercentageOfFinancialyStrainedRenterDwellings = value;
}


double RHouseholdPortfolio::getPercentageUnsafeDwellings() const
{
    return inputPercentageOfUnsafeDwellings;
}

void RHouseholdPortfolio::setPercentageUnsafeDwellings(double value)
{
    inputPercentageOfUnsafeDwellings = value;
}


double RHouseholdPortfolio::getPercentageOvercrowdedOwnerOccupiedDwellings() const
{
    return inputPercentageOfOvercrowdedOwnerDwellings;
}

void RHouseholdPortfolio::setPercentageOvercrowdedOwnerOccupiedDwellings(double value)
{
    inputPercentageOfOvercrowdedOwnerDwellings = value;
}

double RHouseholdPortfolio::getPercentageOvercrowdedRenterOccupiedDwellings() const
{
    return inputPercentageOfOvercrowdedRenterDwellings;
}

void RHouseholdPortfolio::setPercentageOvercrowdedRenterOccupiedDwellings(double value)
{
    inputPercentageOfOvercrowdedRenterDwellings = value;
}


double RHouseholdPortfolio::getPercentageDwellingsUsedFoodStamps() const
{
    return inputPercentageUsedFoodStampsDwellings;
}

void RHouseholdPortfolio::setPercentageDwellingsUsedFoodStamps(double value)
{
    inputPercentageUsedFoodStampsDwellings = value;
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


QVector<bool> RHouseholdPortfolio::getBuildingInsuranceStatus() const
{
    return theBuildingInsuranceVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getMortgageVector() const
{
    return theHouseholdMortgageVector;
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

double RHouseholdPortfolio::getMedianBuildingValue() const
{
    return theMedianBuildingValue;
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

QVector<double> RHouseholdPortfolio::getLandValueVector() const
{
    return theLandValueVector;
}


QVector<double> RHouseholdPortfolio::getBuildingValueVector() const
{
    return theBuildingValueVector;
}


QVector<RLocation*> RHouseholdPortfolio::getBuildingLocationVector() const
{
    return theBuildingLocationVector;
}


QVector<QString> RHouseholdPortfolio::getBuildingOwnerIncomeVector() const
{
    return theBuildingOwnerIncomeVector;
}


QVector<QString> RHouseholdPortfolio::getBuildingOwnerImmigrantStatusVector() const
{
    return theBuildingOwnerImmigrantStatusVector;
}


QVector<QVector<int>> RHouseholdPortfolio::getHouseholdSizeVector() const
{
    return theHouseholdSizeVector;
}

QVector<double> RHouseholdPortfolio::getHomeownerIncomeValueVector() const
{
    return theHomeownerIncomeValueVector;
}

QVector<QString> RHouseholdPortfolio::getBuildingBlockGroup() const
{
    return theBuildingBlockGroup;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdAgeCategoryVector() const
{
    return theHouseholdAgeCategory;
}

QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdImmigrantStatusVector() const
{
    return theHouseholdImmigrantStatusVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdCarOwnershipVector() const
{
    return theHouseholdCarOwnershipVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdIncomeVector() const
{
    return theHouseholdIncomeVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdTenureVector() const
{
    return theHouseholdTenureVector;
}

QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdRaceVector() const
{
    return theHouseholdRaceVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdDisabilityStatus() const
{
    return theHouseholdDisabilityVector;
}


QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdEducationStatus() const
{
    return theHouseholdHigherEdVector;
}

QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdMaritalStatus() const
{
    return theHouseholdMaritalStatusVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdRecentMoverStatus() const
{
    return theHouseholdMovedRecentlyVector;
}


QVector<QVector<QString>> RHouseholdPortfolio::getHouseholdArchetypeVector() const
{
    return theHouseholdArchetypeVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdUnsafeHousingVector() const
{
    return theHouseholdUnsafeHousingVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdUnaffordableHousingVector() const
{
    return theHouseholdUnaffordableHousingVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdOvercrowdedHousingVecto() const
{
    return theHouseholdOvercrowdedHousingVector;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdUnsatisfiedNeighborhood() const
{
    return theHouseholdUnsatisfiedNeighborhood;
}

QVector<QVector<bool>> RHouseholdPortfolio::getHouseholdFinancialStrain() const
{
    return theHouseholdFinancialStrainVector;
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


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    if (theBoundaryLocationList.size() < 1){
        // Get the vector of longitudes and latitudes
        QVector<double> latitudeVector = stringToDoubleVector(inputBoundaryPointsLatitudes);
        QVector<double> longitudeVector = stringToDoubleVector(inputBoundaryPointsLongitudes);

        for (int i = 0; i < latitudeVector.size(); i++){
            theDomain->createObject(objectName() + "BuildingLocation" + QVariant(i).toString(), "RLocation");
            RLocation *thisBoundaryPoint = qobject_cast<RLocation *>(theDomain->getLastAddedObject());

            thisBoundaryPoint->setLatitude(latitudeVector.at(i));
            thisBoundaryPoint->setLongitude(longitudeVector.at(i));
            theBoundaryLocationList << thisBoundaryPoint;
        }
    }


    // Tell the building portifolio what is the median income to calculate building value
    theBuildingPortfolio->setMedianHouseholdIncome(medianMunicipalHouseholdIncome);

    // Tell the building portfolio to instantiate the buildings
    theBuildingPortfolio->instantiateBuildings();

    // Get the building properties
    theMedianBuildingValue          = theBuildingPortfolio->getBuildingValues();
    theNumberOfBuildings            = theBuildingPortfolio->getNumberOfBuildings();
    theBuildingStoreysVector        = theBuildingPortfolio->getBuildingStoreysVector();
    theNumberOfDwellingsPerBuilding = theBuildingPortfolio->getBuildingDwellingsVector();
    theBuildingYearBuilt            = theBuildingPortfolio->getYearBuiltVector();
    theBuildingAreasVector          = theBuildingPortfolio->getBuildingAreaVector();
    theLandValueVector              = theBuildingPortfolio->getLandValueVector();
    theBuildingValueVector          = theBuildingPortfolio->getBuildingValueVector();
    theBuildingTypesVector          = theBuildingPortfolio->getBuildingTypesVector();
    theBuildingOccupancyClassVector = theBuildingPortfolio->getBuildingOccupancyClassVector();
    theBuildingCodeLevelVector      = theBuildingPortfolio->getBuildingCodeLevelVector();
    theBuildingNumberOfRooms        = theBuildingPortfolio->getBuildingRoomsVector();

    // Temporary
    int theNumberOfUnsafeDwellings = 0;
    int theNumberOfUnsuitableDwellings = 0;
    int theNumberOfUnaffordableDwellings = 0;
    int theNumberOfUnsatisfiedDwellings = 0;
    int theNumberOfHouseholdsThatfinancialStrain = 0;
    int theNumberOfLowIncome = 0;
    int theNumberOfHighIncome = 0;
    int allDwellings = 0;

    double numberOfWhiteHouseholds = 0.0;
    double numberOfBlackHouseholds = 0.0;
    double averageIncomeOfWhiteHouseholds = 0.0;
    double averageIncomeOfBlackHouseholds = 0.0;


    for (int i = 0; i < theNumberOfBuildings; i++){

        // Household attributes
        QVector<int> auxHouseholdSizeVector;
        QVector<double> auxHomeownerIncomeValue;
        QVector<QString> auxOwnerIncomeVector;
        QVector<QString> auxTenureVector;
        QVector<QString> auxTenantIncomeVector;
        QVector<QString> auxrenterImmigrantStatusVector;
        QVector<QString> auxMaritalStatusVector;
        QVector<QString> auxRaceVector;
        QVector<QString> auxAgeCategory;
        QVector<bool> auxMortgageVector;
        QVector<bool> auxDisabilityVector;
        QVector<bool> auxHigherEdVector;
        QVector<bool> auxMovedRecentlyVector;
        QVector<bool> auxCarOwnershipVector;
        QVector<bool> auxUnsafeHousingVector;
        QVector<bool> auxUnaffordableHousingVector;
        QVector<bool> auxOvercrowdedHousingVector;
        QVector<bool> auxUnsatisfactionNeighborhoodVector;
        QVector<bool> auxFinancialStrainVector;


        //**************************
        // First, instantiate properties of the building/building owner, one per building
        //**************************
        QString ownerIncome = QString("High");
        double homeownerIncomeValue = 0.0;
        double tenantIncomeValue = 0.0;

        // Housing costs
        double rngCosts = rand() / static_cast<double>(RAND_MAX);
        double lowerLimit = 0.00001;
        double higherLimit = 0.00002;
        QVector<double> theVectorOfHousingCosts;
        theVectorOfHousingCosts << 300 << 500 << 700 << 1000 << 1500 << 2000 << 2500 << 3000 << 4000;
        double myHousingCosts = 0.0;


        for (int j = 0; j < inputHousingCostDistributionVector.size(); j++){
            higherLimit += inputHousingCostDistributionVector.at(j);

            if ((rngCosts - lowerLimit) * (rngCosts - higherLimit) < 0.0){
                myHousingCosts = theVectorOfHousingCosts.at(j);
            }
            lowerLimit += inputHousingCostDistributionVector.at(j);
        }


        // Insurance coverage
        // The dispersion for the income distribution is chosen so that 5% make more 100k - that is roughly the case for Vancouver
        // The dispersion for the insurance percentage is chosen so that P(1.0) ~ 1
        bool insuranceCoverage = false;
        inputPercentageInsured = exp(((1.0/0.35) * qLn(theMedianHouseholdIncome / medianMunicipalHouseholdIncome)) * 0.25 + qLn(insuranceTakeUp));
        double theProbabilityOfInsurance = inputPercentageInsured;
        double rngInsurance = rand()/static_cast<double>(RAND_MAX);

        if (inputMunicipality == QString("Vancouver") && theBuildingOccupancyClassVector.at(i) == QString("RES3")){
            insuranceCoverage = true;

        } else if (rngInsurance <= theProbabilityOfInsurance){
            insuranceCoverage = true;
        }
        else {
            insuranceCoverage = false;
        }


        // Immigrant status of owner
        QString immigrantStatusOwner;
        double rngImmigrantOwner = rand()/static_cast<double>(RAND_MAX);

        if (rngImmigrantOwner < inputPercentageOfRecentImmigrants){
            immigrantStatusOwner = QString("RecentImmigrant");
        }
        else if (rngImmigrantOwner >= inputPercentageOfRecentImmigrants && rngImmigrantOwner < inputPercentageOfRecentImmigrants + inputPercentageOfEstablishedImmigrants){
            immigrantStatusOwner = QString("EstablishedImmigrant");
        }
        else {
            immigrantStatusOwner = QString("NonImmigrant");
        }


        //**************************
        // Now, instantiate properties of the households/building renters, one per dwelling
        //**************************
        for (int j = 0; j < theNumberOfDwellingsPerBuilding.at(i); j++){
            allDwellings++;

            // Immigrant status of renter
            QString immigrantStatusRenter;
            double rngImmigrantrenter = rand()/static_cast<double>(RAND_MAX);

            if (rngImmigrantrenter < inputPercentageOfRecentImmigrants){
                immigrantStatusRenter = QString("RecentImmigrant");
            }
            else if (rngImmigrantrenter >= inputPercentageOfRecentImmigrants && rngImmigrantrenter < inputPercentageOfRecentImmigrants + inputPercentageOfEstablishedImmigrants){
                immigrantStatusRenter = QString("EstablishedImmigrant");
            }
            else {
                immigrantStatusRenter = QString("NonImmigrant");
            }

            //theHousehold size
            double rngSize = rand()/static_cast<double>(RAND_MAX);
            int theHouseholdSize = 0;


            // 1 member
            if (rngSize <= inputHouseholdSizeVector.at(0)){
                theHouseholdSize = 1;

                // 2 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1)) {
                theHouseholdSize = 2;

                // 2 - 3 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2)) {
                theHouseholdSize = 3;

                // 4 members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) && rngSize < inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) + inputHouseholdSizeVector.at(3)) {
                theHouseholdSize = 4;

                // 5+ members
            } else if(rngSize > inputHouseholdSizeVector.at(0) + inputHouseholdSizeVector.at(1) + inputHouseholdSizeVector.at(2) + inputHouseholdSizeVector.at(3)) {
                theHouseholdSize = 5 + qRound(2.0 * rand()/static_cast<double>(RAND_MAX));
            }

            // Married households
            QString maritalStatsCategory = QString("SingleParent");
            double rngMarried = rand()/static_cast<double>(RAND_MAX);

            if (rngMarried < inputPercentageOfMarriedHouseholds){
                maritalStatsCategory = QString("MarriedCouple");
            }

            // Male householder
            bool maleHouseholder = false;
            double rngMale = rand()/static_cast<double>(RAND_MAX);

            if (rngMale < inputPercentageOfMaleHouseholders){
                maleHouseholder = true;
            }


            // Vulnerable population
            QString ageCategory = QString("None");
            double rngChild = rand()/static_cast<double>(RAND_MAX);
            double rngSenior = rand()/static_cast<double>(RAND_MAX);
            double rngChildAndSenior = rand()/static_cast<double>(RAND_MAX);

            if (theHouseholdSize > 2 && rngChildAndSenior < inputPercentageOfChildren * inputPercentageOfSeniors){
                ageCategory = QString("ChildrenAndElderly");

            } else if (theHouseholdSize > 1 && rngChild < inputPercentageOfChildren){
                ageCategory = QString("Children");

            } else if (rngSenior < inputPercentageOfSeniors){
                ageCategory = QString("Elderly");
            }


            // Race
            QString ethnicCategory = QString("None");
            bool hispanicCategory = false;
            double rngRace = rand()/static_cast<double>(RAND_MAX);
            double thePercentageOfNonHispanic = inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds + inputPercentageOfAsianHouseholds + inputPercentageOfNativeAmericanHouseholds;

            if (rngRace < inputPercentageOfWhiteHouseholds){
                ethnicCategory = QString("White");
                numberOfWhiteHouseholds++;

            } else if (rngRace >= inputPercentageOfWhiteHouseholds && rngRace < inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds){
                ethnicCategory = QString("Black");
                numberOfBlackHouseholds++;

            } else if (rngRace >= inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds && rngRace < inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds + inputPercentageOfAsianHouseholds){
                ethnicCategory = QString("Asian");
                numberOfBlackHouseholds++;

            } else if (rngRace >= inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds + inputPercentageOfAsianHouseholds && rngRace < thePercentageOfNonHispanic){
                ethnicCategory = QString("NativeAmerican");
                numberOfBlackHouseholds++;

            } else if (rngRace >= thePercentageOfNonHispanic && rngRace < thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds){
                ethnicCategory = QString("WhiteHispanic");
                hispanicCategory = true;
                numberOfWhiteHouseholds++;

            } else if (rngRace >= thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds && rngRace < thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds){
                ethnicCategory = QString("BlackHispanic");
                hispanicCategory = true;
                numberOfBlackHouseholds++;

            } else if (rngRace >= thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds && rngRace < thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds + inputPercentageOfAsianHouseholds){
                ethnicCategory = QString("AsianHispanic");
                hispanicCategory = true;
                numberOfBlackHouseholds++;

            } else if (rngRace >= thePercentageOfNonHispanic + inputPercentageOfWhiteHouseholds + inputPercentageOfBlackHouseholds + inputPercentageOfAsianHouseholds){
                ethnicCategory = QString("NativeAmericanHispanic");
                hispanicCategory = true;
                numberOfBlackHouseholds++;

            } else {
                qCritical() << "The RHouseholdPortfolio cannot identify the Race for thistheHousehold";
            }

            // Person with disability
            bool disabilityCategory = false;
            double rngDisability = rand()/static_cast<double>(RAND_MAX);

            if (rngDisability < inputPercentageWithDisability){
                disabilityCategory = true;
            }

            // Higher education
            bool higherEducationCategory = false;
            double rngEducation = rand()/static_cast<double>(RAND_MAX);

            if (rngEducation < inputPercentageHigherEducation){
                higherEducationCategory = true;
            }


            // Define the dummy variables for the logit models
            int dummyElderly = 0;
            if (ageCategory == QString("Elderly")){
                dummyElderly = 1;
            }

            int dummyChildren = 0;
            if (ageCategory == QString("Children")){
                dummyChildren = 1;
            }

            int dummyDisability = 0;
            if (disabilityCategory == true){
                dummyDisability = 1;
            }

            int dummyImmigrant = 0;
            if (immigrantStatusOwner == QString("RecentImmigrant")){
                dummyImmigrant = 1;
            }

            int dummyHigherEd = 0;
            if (higherEducationCategory == true){
                dummyHigherEd = 1;
            }

            int dummyWhite = 0;
            if (ethnicCategory.contains(QString("White"))){
                dummyWhite = 1;
            }

            int dummyHispanic = 0;
            if (hispanicCategory == true){
                dummyHispanic = 1;
            }

            int dummyMarried = 0;
            if (maritalStatsCategory == QString("MarriedCouple")){
                dummyMarried = 1;
            }

            int dummyMale = 0;
            if (maleHouseholder == true){
                dummyMale = 1;
            }

            int dummyPre1940 = 0;
            if (theBuildingYearBuilt.at(i) <= 1940){
                dummyPre1940 = 1;
            }

            int dummyPre1980 = 0;
            if (theBuildingYearBuilt.at(i) > 1940 && theBuildingYearBuilt.at(i) <= 1980){
                dummyPre1980 = 1;
            }

            int dummySingleFamily = 0;
            if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
                dummySingleFamily = 1;
            }


            // Tenure
            QString tenure;
            double rngOwner = rand() / static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){

                double theCorrectionFactorForTenure = (1.0 - inputPercentageOfRenters) / 0.53489;
                double beta0_Tenure = -2.75333;
                double betaElderly_Tenure = 1.17070;
                double betaHigherEd_Tenure = 0.73040;
                double betaBedrooms_Tenure = 0.89125;
                double betaStories_Tenure = -0.34330;
                double betaBathrooms_Tenure = 0.27361;


                double ownerLogit = beta0_Tenure +
                        dummyElderly * betaElderly_Tenure +
                        dummyHigherEd * betaHigherEd_Tenure +
                        theBuildingNumberOfRooms.at(i) * betaBedrooms_Tenure +
                        theBuildingStoreysVector.at(i) * betaStories_Tenure +
                        2.0 * betaBathrooms_Tenure; // No data on this one

                double theProbOfOwner = theCorrectionFactorForTenure * exp(ownerLogit) / (1.0 + exp(ownerLogit));

                if (theProbOfOwner < 0){
                    theProbOfOwner = 0.0;
                } else if (theProbOfOwner > 1){
                    theProbOfOwner = 1.0;
                }

                if (rngOwner < theProbOfOwner){
                    tenure = QString("OwnerOccupied");
                    immigrantStatusRenter = immigrantStatusOwner;

                } else {
                    tenure = QString("RenterOccupied");
                }

            } else {
                if (rngOwner > inputPercentageOfRenters){
                    tenure =  QString("OwnerOccupied");
                    immigrantStatusRenter = immigrantStatusOwner;

                } else {
                    tenure = QString("RenterOccupied");

                }
            }

            int dummyRenter = 0;
            if (tenure == QString("RenterOccupied")){
                dummyRenter = 1;
            }


            // Recently moved in
            bool recentlyMovedInStatus = false;
            double rngMovedIn = rand()/static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){
                double betaRenter_Moved = 1.73808;
                double betaElderly_Moved = -1.48906;

                double moverLogit = dummyRenter * betaRenter_Moved +
                        dummyElderly * betaElderly_Moved;

                double theProbOfMover = exp(moverLogit) / (1.0 + exp(moverLogit));

                if (theProbOfMover < 0 || theProbOfMover > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for recent mover" << theProbOfMover;
                }

                if (rngMovedIn < theProbOfMover){
                    recentlyMovedInStatus = true;
                }

            } else {
                if (rngMovedIn < inputPercentageRecentlyMovedIn){
                    recentlyMovedInStatus = true;
                }
            }

            int dummyMoved = 0;
            if (recentlyMovedInStatus == true){
                dummyMoved = 1;
            }


            // Income
            // Pre-set some variables
            QString tenantIncome = QString("");
            double rngOwnerIncome = rand()/static_cast<double>(RAND_MAX);
            double rngTenantIncome = rand()/static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){
                //                AUC 0.75
                //                (Intercept)                   -2.04387    0.10925 -18.708  < 2e-16 ***
                //                HTenureRenter                 -0.80288    0.07556 -10.626  < 2e-16 ***
                //                HSize                          0.40305    0.02637  15.287  < 2e-16 ***
                //                HYearMovedInRecentMover        0.69961    0.08106   8.631  < 2e-16 ***
                //                HHImmigrationStatusNotCitizen  0.58248    0.09979   5.837 5.31e-09 ***
                //                HHHispanicYes                 -0.67621    0.11024  -6.134 8.57e-10 ***
                //                HHHigherEdYes                  0.89878    0.07707  11.662  < 2e-16 ***

                double theCorrectionFactorForHighIncome = inputPercentageHighIncome / 0.1983431;
                double beta0_HighIncome = -2.04387;
                double betaRenter_HighIncome = -0.80288;
                double betaSize_HighIncome = 0.40305;
                double betaMoved_HighIncome = 0.69961;
                double betaImmigrant_HighIncome = 0.58248;
                double betaHispanic_HighIncome = -0.67621;
                double betaHigherEd_HighIncome =  0.89878;

                //                AUC = 0.82
                //                (Intercept)              0.48353    0.18952   2.551 0.010730 *
                //                HTenureRenter            1.46044    0.14059  10.388  < 2e-16 ***
                //                HSize                   -0.40451    0.04614  -8.768  < 2e-16 ***
                //                HYearMovedInRecentMover -0.58381    0.15538  -3.757 0.000172 ***
                //                HElderlyYes              1.24175    0.14685   8.456  < 2e-16 ***
                //                HHHigherEdYes           -1.47109    0.12646 -11.633  < 2e-16 ***

                double theCorrectionFactorForLowIncome = inputPercentageLowIncome / 0.411306;
                double beta0_LowIncome = 0.48353;
                double betaRenter_LowIncome = 1.46044;
                double betaSize_LowIncome = -0.40451;
                double betaMoved_LowIncome = -0.58381;
                double betaElderly_LowIncome = 1.24175;
                double betaHighEd_LowIncome = -1.47109;

                // Owners
                double ownerHighIncomeLogit = beta0_HighIncome +
                        0.0 * betaRenter_HighIncome +
                        theHouseholdSize * betaSize_HighIncome +
                        dummyMoved * betaMoved_HighIncome +
                        dummyImmigrant * betaImmigrant_HighIncome +
                        dummyHispanic * betaHispanic_HighIncome +
                        dummyHigherEd * betaHigherEd_HighIncome;

                double ownerLowIncomeLogit = beta0_LowIncome +
                        0.0 * betaRenter_LowIncome +
                        theHouseholdSize * betaSize_LowIncome +
                        dummyMoved * betaMoved_LowIncome +
                        dummyElderly * betaElderly_LowIncome +
                        dummyHigherEd * betaHighEd_LowIncome;

                double theProbOfOwnerHighIncome = theCorrectionFactorForHighIncome * exp(ownerHighIncomeLogit) / (1.0 + exp(ownerHighIncomeLogit));
                if (theProbOfOwnerHighIncome < 0.0){
                    theProbOfOwnerHighIncome = 0.0;
                } else if (theProbOfOwnerHighIncome > 1.0) {
                    theProbOfOwnerHighIncome = 1.0;
                }


                double theProbOfOwnerLowIncome = theCorrectionFactorForLowIncome * exp(ownerLowIncomeLogit) / (1.0 + exp(ownerLowIncomeLogit));
                if (theProbOfOwnerLowIncome < 0.0){
                    theProbOfOwnerLowIncome = 0.0;
                } else if (theProbOfOwnerLowIncome > 1.0) {
                    theProbOfOwnerLowIncome = 1.0;
                }

                double theMu = 11.4786;
                double theSigma = 0.86795;
                double theLowIncomeThreshold = 0.3985537;
                double theHighIncomeThreshold = 0.7877394;

                // Set owner income
                if (rngOwnerIncome < theProbOfOwnerLowIncome){
                    ownerIncome = QString("Low");
                    double rngLowIncome = rand()/static_cast<double>(RAND_MAX) * theLowIncomeThreshold;
                    homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngLowIncome,1.0) * theSigma + theMu);

                } else if (rngOwnerIncome >= theProbOfOwnerLowIncome && rngOwnerIncome < theProbOfOwnerLowIncome + theProbOfOwnerHighIncome){
                    ownerIncome = QString("High");
                    double rngHighIncome = theHighIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (1.0 - theHighIncomeThreshold);
                    homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngHighIncome,1.0) * theSigma + theMu);

                } else {
                    ownerIncome = QString("Moderate");
                    double rngModerateIncome = theLowIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (theHighIncomeThreshold - theLowIncomeThreshold);
                    homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngModerateIncome,1.0) * theSigma + theMu);
                }


                // Renters
                double renterHighIncomeLogit = beta0_HighIncome +
                        1.0 * betaRenter_HighIncome +
                        theHouseholdSize * betaSize_HighIncome +
                        dummyMoved * betaMoved_HighIncome +
                        dummyHispanic * betaHispanic_HighIncome +
                        dummyHigherEd * betaHigherEd_HighIncome;

                double theProbOfRenterHighIncome = theCorrectionFactorForHighIncome * exp(renterHighIncomeLogit) / (1.0 + exp(renterHighIncomeLogit));
                if (theProbOfRenterHighIncome < 0.0){
                    theProbOfRenterHighIncome = 0.0;
                } else if (theProbOfRenterHighIncome > 1.0) {
                    theProbOfRenterHighIncome = 1.0;
                }

                double tenantLowIncomeLogit = beta0_LowIncome +
                        1.0 * betaRenter_LowIncome +
                        theHouseholdSize * betaSize_LowIncome +
                        dummyMoved * betaMoved_LowIncome +
                        dummyElderly * betaElderly_LowIncome +
                        dummyHigherEd * betaHighEd_LowIncome;

                double theProbOfRenterLowIncome = theCorrectionFactorForLowIncome * exp(tenantLowIncomeLogit) / (1.0 + exp(tenantLowIncomeLogit));
                if (theProbOfRenterLowIncome < 0.0){
                    theProbOfRenterLowIncome = 0.0;
                } else if (theProbOfRenterLowIncome > 1.0) {
                    theProbOfRenterLowIncome = 1.0;
                }

                // Set renter income
                if (rngTenantIncome < theProbOfRenterLowIncome){
                    tenantIncome = QString("Low");
                    double rngLowIncome = rand()/static_cast<double>(RAND_MAX) * theLowIncomeThreshold;
                    tenantIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngLowIncome,1.0) * theSigma + theMu);

                } else if (rngTenantIncome >= theProbOfRenterLowIncome && rngTenantIncome < theProbOfRenterLowIncome + theProbOfRenterHighIncome){
                    tenantIncome = QString("High");
                    double rngHighIncome = theHighIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (1.0 - theHighIncomeThreshold);
                    tenantIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngHighIncome,1.0) * theSigma + theMu);

                } else {
                    tenantIncome = QString("Moderate");
                    double rngModerateIncome = theLowIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (theHighIncomeThreshold - theLowIncomeThreshold);
                    tenantIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngModerateIncome,1.0) * theSigma + theMu);
                }

                if (tenure == QString("OwnerOccupied")){
                    tenantIncome = ownerIncome;
                }




            } else if (theInstantiationMethod == QString("MultipleRegression")){

                // Random variables for the owner income model
                double etaLowMod_Income = 1.188552125;
                double etaModHigh_Income = 2.090101358;
                double etaRenter_Income = -1.213274274;
                double etaSize_Income = 0.599991587;
                double etaMoved_Income = 0.459811928;
                double etaChildren_Income = -0.84529961;
                double etaElderly_Income = -1.3411497;
                double etaDisability_Income = -0.608502586;
                double etaImmigration_Income = -0.369545127;
                double etaWhite_Income = 0.487770473;
                double etaHispanic_Income = -0.948049367;
                double etaMale_Income = 0.403915757;
                double etaHighEd_Income = 1.323589423;

                double betaLowMod_Income =      etaLowMod_Income;
                double betaModHigh_Income =     etaModHigh_Income;
                double betaRenter_Income =      -etaRenter_Income;
                double betaSize_Income =        -etaSize_Income;
                double betaMoved_Income =       -etaMoved_Income;
                double betaChildren_Income =    -etaChildren_Income;
                double betaElderly_Income =     -etaElderly_Income;
                double betaDisability_Income =  -etaDisability_Income;
                double betaImmigration_Income = -etaImmigration_Income;
                double betaWhite_Income =       -etaWhite_Income;
                double betaHispanic_Income =    -etaHispanic_Income;
                double betaMale_Income =        -etaMale_Income;
                double betaHighEd_Income =      -etaHighEd_Income;

                // Owners
                double ownerLowIncomeLogit = betaLowMod_Income +
                        0.0              * betaRenter_Income +
                        theHouseholdSize * betaSize_Income +
                        dummyMoved       * betaMoved_Income +
                        dummyChildren    * betaChildren_Income +
                        dummyElderly     * betaElderly_Income +
                        dummyDisability  * betaDisability_Income +
                        dummyImmigrant   * betaImmigration_Income +
                        dummyWhite       * betaWhite_Income +
                        dummyHispanic    * betaHispanic_Income +
                        dummyMale        * betaMale_Income +
                        dummyHigherEd    * betaHighEd_Income;

                double ownerModIncomeLogit = betaModHigh_Income +
                        0.0              * betaRenter_Income +
                        theHouseholdSize * betaSize_Income +
                        dummyMoved       * betaMoved_Income +
                        dummyChildren    * betaChildren_Income +
                        dummyElderly     * betaElderly_Income +
                        dummyDisability  * betaDisability_Income +
                        dummyImmigrant   * betaImmigration_Income +
                        dummyWhite       * betaWhite_Income +
                        dummyHispanic    * betaHispanic_Income +
                        dummyMale        * betaMale_Income +
                        dummyHigherEd    * betaHighEd_Income;

                double theProbOfOwnerLowIncome = exp(ownerLowIncomeLogit) / (1.0 + exp(ownerLowIncomeLogit));
                double theProbOfOwnerModIncome = exp(ownerModIncomeLogit) / (1.0 + exp(ownerModIncomeLogit)) - theProbOfOwnerLowIncome;
                double theProbOfOwnerHighIncome = 1.0 - (theProbOfOwnerModIncome + theProbOfOwnerLowIncome);


                // Sanity check the probabilities
                if (theProbOfOwnerLowIncome < 0 || theProbOfOwnerLowIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }

                if (theProbOfOwnerModIncome < 0 || theProbOfOwnerModIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }

                if (theProbOfOwnerHighIncome < 0 || theProbOfOwnerHighIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }


                // Set owner income
                if (rngOwnerIncome < theProbOfOwnerLowIncome){
                    ownerIncome = QString("Low");

                } else if (rngOwnerIncome >= theProbOfOwnerLowIncome && rngOwnerIncome < theProbOfOwnerLowIncome + theProbOfOwnerModIncome){
                    ownerIncome = QString("Moderate");

                } else if (rngOwnerIncome >= theProbOfOwnerLowIncome + theProbOfOwnerModIncome){
                    ownerIncome = QString("High");
                }

                // Renters
                double renterLowIncomeLogit = betaLowMod_Income +
                        1.0              * betaRenter_Income +
                        theHouseholdSize * betaSize_Income +
                        dummyMoved       * betaMoved_Income +
                        dummyChildren    * betaChildren_Income +
                        dummyElderly     * betaElderly_Income +
                        dummyDisability  * betaDisability_Income +
                        dummyImmigrant   * betaImmigration_Income +
                        dummyWhite       * betaWhite_Income +
                        dummyHispanic    * betaHispanic_Income +
                        dummyMale        * betaMale_Income +
                        dummyHigherEd    * betaHighEd_Income;

                double renterModIncomeLogit = betaModHigh_Income +
                        1.0              * betaRenter_Income +
                        theHouseholdSize * betaSize_Income +
                        dummyMoved       * betaMoved_Income +
                        dummyChildren    * betaChildren_Income +
                        dummyElderly     * betaElderly_Income +
                        dummyDisability  * betaDisability_Income +
                        dummyImmigrant   * betaImmigration_Income +
                        dummyWhite       * betaWhite_Income +
                        dummyHispanic    * betaHispanic_Income +
                        dummyMale        * betaMale_Income +
                        dummyHigherEd    * betaHighEd_Income;

                double theProbOfRenterLowIncome = exp(renterLowIncomeLogit) / (1.0 + exp(renterLowIncomeLogit));
                double theProbOfRenterModIncome = exp(renterModIncomeLogit) / (1.0 + exp(renterModIncomeLogit)) - theProbOfRenterLowIncome;
                double theProbOfRenterHighIncome = 1.0 - (theProbOfRenterModIncome + theProbOfRenterLowIncome);

                // Sanity check the probabilities
                if (theProbOfRenterLowIncome < 0 || theProbOfRenterLowIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }

                if (theProbOfRenterModIncome < 0 || theProbOfRenterModIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }

                if (theProbOfRenterHighIncome < 0 || theProbOfRenterHighIncome > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for income";
                }


                // Set Renter income
                if (rngTenantIncome < theProbOfRenterLowIncome){
                    tenantIncome = QString("Low");

                } else if (rngTenantIncome >= theProbOfRenterLowIncome && rngTenantIncome < theProbOfRenterLowIncome + theProbOfRenterModIncome){
                    tenantIncome = QString("Moderate");

                } else if (rngTenantIncome >= theProbOfRenterLowIncome + theProbOfRenterModIncome){
                    tenantIncome = QString("High");
                }

            } else {
                // If this is an ownerHousehold
                if (tenure == QString("OwnerOccupied")){
                    if (rngOwnerIncome < inputPercentageLowIncome){
                        ownerIncome = QString("Low");

                    } else if (rngOwnerIncome >= inputPercentageLowIncome && rngOwnerIncome < inputPercentageLowIncome + inputPercentageModerateIncome){
                        ownerIncome = QString("Moderate");

                    } else if (rngOwnerIncome >= inputPercentageLowIncome + inputPercentageModerateIncome){
                        ownerIncome = QString("High");
                    }

                    tenantIncome = ownerIncome;

                } else {

                    ownerIncome = QString("High");

                    // If this is an renterHousehold
                    if (rngTenantIncome < inputPercentageLowIncome){
                        tenantIncome = QString("Low");

                    } else if (rngTenantIncome >= inputPercentageLowIncome && rngTenantIncome < inputPercentageLowIncome + inputPercentageModerateIncome){
                        tenantIncome = QString("Moderate");

                    } else if (rngTenantIncome >= inputPercentageLowIncome + inputPercentageModerateIncome){
                        tenantIncome = QString("High");
                    }
                }
            }

            // Set owner income
            double theMu = 11.4786;
            double theSigma = 0.86795;
            double theLowIncomeThreshold = 0.3985537;
            double theHighIncomeThreshold = 0.7877394;
            if (ownerIncome == QString("Low")){
                double rngLowIncome = rand()/static_cast<double>(RAND_MAX) * theLowIncomeThreshold;
                homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngLowIncome,1.0) * theSigma + theMu);

            } else if (ownerIncome == QString("High")){
                double rngHighIncome = theHighIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (1.0 - theHighIncomeThreshold);
                homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngHighIncome,1.0) * theSigma + theMu);

            } else {
                ownerIncome = QString("Moderate");
                double rngModerateIncome = theLowIncomeThreshold + rand()/static_cast<double>(RAND_MAX) * (theHighIncomeThreshold - theLowIncomeThreshold);
                homeownerIncomeValue = exp(theNormalDistribution.getInverseNormalCDF(rngModerateIncome,1.0) * theSigma + theMu);
            }


            if (ethnicCategory.contains(QString("White"))){
                averageIncomeOfWhiteHouseholds += homeownerIncomeValue;
            } else if (ethnicCategory.contains(QString("Black"))){
                averageIncomeOfBlackHouseholds += homeownerIncomeValue;
            }


            // Mortgage
            // If owner-occupied renter income = owner income
            bool mortgage = false;
            double rngMortgage = rand()/static_cast<double>(RAND_MAX);

            // Random variables for the mortgage model
            double beta0_Mortgage = 0.22;
            double betaChildren_Mortgage = 0.71;
            double betaElderly_Mortgage = -0.69;
            double betaImmigration_Mortgage = -0.44;
            double betaMoved_Mortgage = 0.73;
            double betaIncome_Mortgage = 0.74;


            if (1 > 2 && theInstantiationMethod == QString("LogisticRegression")){
                double theCorrectionFactorForMortgage = inputPercentageWithMortgage / 1.0;
                int dummyOwnerHighIncome = 0;
                if (ownerIncome == QString("High")){
                    dummyOwnerHighIncome = 1;
                }

                //double mortgageLogit = beta0_Mortgage->getCurrentValue() + dummyChildren * betaChildren_Mortgage->getCurrentValue() + dummyElderly * betaElderly_Mortgage->getCurrentValue() + dummyImmigrant * betaImmigration_Mortgage->getCurrentValue() + dummyMoved * betaMoved_Mortgage->getCurrentValue() + dummyOwnerHighIncome * betaIncome_Mortgage->getCurrentValue();
                double mortgageLogit = beta0_Mortgage + dummyChildren * betaChildren_Mortgage + dummyElderly * betaElderly_Mortgage + dummyImmigrant * betaImmigration_Mortgage + dummyMoved * betaMoved_Mortgage + dummyOwnerHighIncome * betaIncome_Mortgage;
                double theProbOfMortgage = exp(mortgageLogit) / (1.0 + exp(mortgageLogit));

                if (theProbOfMortgage < 0 || theProbOfMortgage > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for mortgage" << theProbOfMortgage;
                }

                if (rngMortgage < theProbOfMortgage){
                    mortgage = true;
                }

            } else {

                if (rngMortgage < inputPercentageWithMortgage){
                    mortgage = true;
                }
            }


            int dummyLowIncome = 0;
            if (tenantIncome == QString("Low")){
                dummyLowIncome = 1;
            }

            int dummyModerateIncome = 0;
            if (tenantIncome == QString("Moderate")){
                dummyModerateIncome = 1;
            }

            int dummyMortgage = 0;
            if (mortgage == true){
                dummyMortgage = 1;
            }



            // Housing safety
            bool unsafe = false;
            double rngSafety = rand()/static_cast<double>(RAND_MAX);

            //if (theInstantiationMethod == QString("LogisticRegression")){
            double theCorrectionFactorForUnsafe = 1.0; // census tract data not available from census

            // Random variables for the housing unsafe model
            double beta0_Unsafe = -1.5104;
            double betaLowIncome_Unsafe = -1.2159;
            double betaModerateIncome_Unsafe = 0.3794;
            double betaMoved_Unsafe = 1.1021;
            double betaElderly_Unsafe = -2.2747;
            double betaDisability_Unsafe = 2.0365;
            double betaWhite_Unsafe = 0.7566;
            double betaHispanic_Unsafe = 0.7003;
            double betaMale_Unsafe = -1.4923;
            double betaPre1940_Unsafe = 0.7747;
            double betaPre1980_Unsafe = 0.6716;

            double unsafeLogit = beta0_Unsafe +
                    dummyLowIncome   * betaLowIncome_Unsafe +
                    dummyModerateIncome * betaModerateIncome_Unsafe +
                    dummyMoved       * betaMoved_Unsafe +
                    dummyElderly     * betaElderly_Unsafe +
                    dummyDisability  * betaDisability_Unsafe +
                    dummyWhite       * betaWhite_Unsafe +
                    dummyHispanic    * betaHispanic_Unsafe +
                    dummyMale        * betaMale_Unsafe +
                    dummyPre1940     * betaPre1940_Unsafe +
                    dummyPre1980     * betaPre1980_Unsafe;

            double theProbOfUnsafe = theCorrectionFactorForUnsafe * exp(unsafeLogit) / (1.0 + exp(unsafeLogit));

            if (theProbOfUnsafe < 0){
                theProbOfUnsafe = 0.0;
            } else if (theProbOfUnsafe > 1){
                theProbOfUnsafe = 1.0;
            }

            if (rngSafety < theProbOfUnsafe){
                unsafe = true;
            }

            //            } else {
            //                if (rngSafety < inputPercentageOfUnsafeDwellings){
            //                    unsafe = true;
            //                }
            //            }


            // Overcrowding
            bool overcrowded = false;
            double rngOvercrowding = rand()/static_cast<double>(RAND_MAX);

            //if (theInstantiationMethod == QString("LogisticRegression")){
            double theCorrectionFactorForOwnerOvercrowded = inputPercentageOfOvercrowdedOwnerDwellings / 0.1065319;
            double theCorrectionFactorForRenterOvercrowded = inputPercentageOfOvercrowdedRenterDwellings / 0.06920684;
            double theCorrectionFactorForOvercrowded = 1.0;

            if (tenure == QString("Owner")){
                theCorrectionFactorForOvercrowded = theCorrectionFactorForOwnerOvercrowded;
            } else if (tenure == QString("Renter")){
                theCorrectionFactorForOvercrowded = theCorrectionFactorForRenterOvercrowded;
            }

            // logistic parameters
            double beta0_Overcrowding = -8.11071;
            double betaLowIncome_Overcrowding = 1.81546;
            double betaModerateIncome_Overcrowding = 0.93406;
            double betaSize_Overcrowding = 1.77589;
            double betaMover_Overcrowding = 0.70078;
            double betaWhite_Overcrowding = -0.67963;
            double betaHispanic_Overcrowding = 1.47947;

            // Overcrowding
            double overcrowdingLogit =    beta0_Overcrowding +
                    dummyLowIncome      * betaLowIncome_Overcrowding +
                    dummyModerateIncome * betaModerateIncome_Overcrowding +
                    theHouseholdSize    * betaSize_Overcrowding +
                    dummyMoved          * betaMover_Overcrowding +
                    dummyWhite          * betaWhite_Overcrowding +
                    dummyHispanic       * betaHispanic_Overcrowding;

            double theProbOfOvercrowding = theCorrectionFactorForOvercrowded * exp(overcrowdingLogit) / (1.0 + exp(overcrowdingLogit));

            if (theProbOfOvercrowding < 0.0){
                theProbOfOvercrowding = 0.0;

            } else if (theProbOfOvercrowding > 1.0){
                theProbOfOvercrowding = 1.0;
            }

            if (rngOvercrowding < theProbOfOvercrowding){
                overcrowded = true;
            }

            //            } else {
            //                if (tenure == QString("Owner") && rngOvercrowding < inputPercentageOfOvercrowdedOwnerDwellings){
            //                    overcrowded = true;

            //                } else if (tenure == QString("Renter") && rngOvercrowding < inputPercentageOfOvercrowdedRenterDwellings){
            //                    overcrowded = true;
            //                }
            //            }


            // Affordability
            bool unaffordable = false;
            double rngAffordability = rand()/static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){

                double theCorrectionFactorUnaffordable = 1.0;
                if (dummyRenter == 0 && dummyMortgage == true){
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableOwnerDwellings / 0.2581649;

                } else if (dummyRenter == 0 && dummyMortgage == false) {
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableOwnerDwellings / 100000000.0;

                } else {
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableRenterDwellings / 0.1213064;
                }


                double beta0_Affordability = -2.2192;
                double betaLowIncome_Affordability = 2.5286;
                double betaModerateIncome_Affordability = 1.4498;

                //double affordabilityLogit = beta0_Affordability->getCurrentValue() + dummyMoved * betaMoved_Affordability->getCurrentValue() + dummyLowIncome * betaLowIncome_Affordability->getCurrentValue() + dummyModerateIncome * betaModerateIncome_Affordability->getCurrentValue();
                double affordabilityLogit = beta0_Affordability +
                        dummyLowIncome      * betaLowIncome_Affordability +
                        dummyModerateIncome * betaModerateIncome_Affordability;

                double theProbOfNotAffordable = theCorrectionFactorUnaffordable * exp(affordabilityLogit) / (1.0 + exp(affordabilityLogit));

                if (theProbOfNotAffordable < 0){
                    theProbOfNotAffordable = 0.0;
                } else if (theProbOfNotAffordable > 1){
                    theProbOfNotAffordable = 1.0;
                }

                if (rngAffordability < theProbOfNotAffordable){
                    unaffordable = true;
                    theNumberOfUnaffordableDwellings++;
                }

            } else {

                if (dummyRenter == 0){
                    if (rngAffordability < inputPercentageOfUnaffordableOwnerDwellings){
                        unaffordable = true;
                    }
                } else {
                    if (rngAffordability < inputPercentageOfUnaffordableRenterDwellings){
                        unaffordable = true;
                    }
                }
            }


            // Neighborhood dissatisfaction
            bool satisfiedWithNeighborhood = true;
            double rngNeighborhood = rand()/static_cast<double>(RAND_MAX);
            int dummyNeighborhoodDissatisfaction = 0;

            //if (theInstantiationMethod == QString("LogisticRegression")){

            // Random variables for the neighborhood satisfaction model
            double beta0_Neighborhood =	        -0.4497;
            double betaLowIncome_Neighborhood = -0.28815;
            double betaModerateIncome_Neighborhood = -1.13332;
            double betaElderly_Neighborhood =   -1.24549;
            double betaDisability_Neighborhood = 1.50053;
            double betaWhite_Neighborhood =     -0.55273;
            double betaHispanic_Neighborhood =   1.6973;
            double betaMale_Neighborhood =      -0.40181;
            double betaPre1940_Neighborhood =   0.71054;
            double betaPre1980_Neighborhood =   0.16726;


            double neighborhoodLogit = beta0_Neighborhood +
                    dummyLowIncome   * betaLowIncome_Neighborhood +
                    dummyModerateIncome * betaModerateIncome_Neighborhood +
                    dummyElderly     * betaElderly_Neighborhood +
                    dummyDisability  * betaDisability_Neighborhood +
                    dummyWhite       * betaWhite_Neighborhood +
                    dummyHispanic    * betaHispanic_Neighborhood +
                    dummyMale        * betaMale_Neighborhood +
                    dummyPre1940     * betaPre1940_Neighborhood +
                    dummyPre1980     * betaPre1980_Neighborhood;

            double theProbOfNotSatisfiedWithNeighborhood = exp(neighborhoodLogit) / (1.0 + exp(neighborhoodLogit));

            if (theProbOfNotSatisfiedWithNeighborhood < 0 || theProbOfNotSatisfiedWithNeighborhood > 1){
                qCritical() << this->objectName() << "has a problem with the logit for overcrowding" << theProbOfNotSatisfiedWithNeighborhood;
            }

            if (rngNeighborhood < theProbOfNotSatisfiedWithNeighborhood){
                satisfiedWithNeighborhood = false;
                if (dummyWhite == 1){
                    theNumberOfUnsatisfiedDwellings++;
                }
                dummyNeighborhoodDissatisfaction = 1;
            }

            //}

            // Financial strain
            bool financialStrain = false;
            double rngStrain = rand()/static_cast<double>(RAND_MAX);

            if (dummyRenter == 1){
                if (theInstantiationMethod == QString("LogisticRegression")){

                    double theCorrectionFactorForFinancialStrain = 1.0;
                    if (dummyRenter == 0 && dummyMortgage == true){
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedOwnerDwellings / 0.1119751;

                    } else if (dummyRenter == 0 && dummyMortgage == false) {
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedOwnerDwellings / 100000000.0;

                    } else {
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedRenterDwellings / 0.05832037;
                    }

                    // Random variables for the food Strain model
                    double beta0_Strain = -3.4213;
                    double betaRenter_Strain = 0.5678;
                    double betaLowIncome_Strain = 3.8263;
                    double betaModerateIncome_Strain = 1.4155;
                    double betaImmigrant_Strain = 1.0808;

                    double strainLogit = beta0_Strain +
                            1.0 * betaRenter_Strain +
                            dummyLowIncome * betaLowIncome_Strain +
                            dummyModerateIncome * betaModerateIncome_Strain +
                            dummyImmigrant * betaImmigrant_Strain;

                    double theProbOfFinancialStrain = theCorrectionFactorForFinancialStrain * exp(strainLogit) / (1.0 + exp(strainLogit));

                    if (theProbOfFinancialStrain < 0.0){
                        theProbOfFinancialStrain = 0.0;

                    } else if (theProbOfFinancialStrain > 1.0){
                        theProbOfFinancialStrain = 1.0;
                    }

                    if (rngStrain < theProbOfFinancialStrain){
                        financialStrain = true;
                        theNumberOfHouseholdsThatfinancialStrain++;
                    }

                } else {
                    financialStrain = false;
                }
            }


            // Car ownership
            bool carOwnership;
            double rngCar = rand()/static_cast<double>(RAND_MAX);

            carOwnership = false;
            if (rngCar < inputPercentageWithCars){
                carOwnership = true;
            }

            // Build the vectors representing each building
            auxTenantIncomeVector << tenantIncome;
            auxTenureVector << tenure;
            auxMortgageVector << mortgage;
            auxrenterImmigrantStatusVector << immigrantStatusRenter;
            auxHouseholdSizeVector << theHouseholdSize;
            auxAgeCategory << ageCategory;
            auxRaceVector << ethnicCategory;
            auxDisabilityVector << disabilityCategory;
            auxHigherEdVector << higherEducationCategory;
            auxMaritalStatusVector << maritalStatsCategory;
            auxMovedRecentlyVector << recentlyMovedInStatus;
            auxCarOwnershipVector << carOwnership;
            auxUnsafeHousingVector << unsafe;
            auxUnaffordableHousingVector << unaffordable;
            auxOvercrowdedHousingVector << overcrowded;
            auxUnsatisfactionNeighborhoodVector << satisfiedWithNeighborhood;
            auxFinancialStrainVector << financialStrain;
            if (tenantIncome == QString("Low")){
                theNumberOfLowIncome++;
            } else if(tenantIncome == QString("High")){
                theNumberOfHighIncome++;
            }


        } // j


        if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
            theNumberOfSingleFamilyBuildings++;
        } else {
            theNumberOfMultiFamilyBuildings++;
        }

        theBuildingBlockGroup << "None";

        // Owner properties
        theBuildingOwnerIncomeVector << ownerIncome;
        theBuildingOwnerImmigrantStatusVector << immigrantStatusOwner;
        theBuildingInsuranceVector << insuranceCoverage;
        theHomeownerIncomeValueVector << homeownerIncomeValue;

        // renter properties
        theHouseholdTenureVector << auxTenureVector;
        theHouseholdMortgageVector << auxMortgageVector;
        theHouseholdIncomeVector << auxTenantIncomeVector;
        theHouseholdImmigrantStatusVector << auxrenterImmigrantStatusVector;
        theHouseholdSizeVector << auxHouseholdSizeVector;
        theHouseholdAgeCategory << auxAgeCategory;
        theHouseholdRaceVector << auxRaceVector;
        theHouseholdDisabilityVector << auxDisabilityVector;
        theHouseholdHigherEdVector << auxHigherEdVector;
        theHouseholdMaritalStatusVector << auxMaritalStatusVector;
        theHouseholdMovedRecentlyVector << auxMovedRecentlyVector;
        theHouseholdCarOwnershipVector << auxCarOwnershipVector;
        theHouseholdUnsafeHousingVector << auxUnsafeHousingVector;
        theHouseholdUnaffordableHousingVector << auxUnaffordableHousingVector;
        theHouseholdOvercrowdedHousingVector << auxOvercrowdedHousingVector;
        theHouseholdUnsatisfiedNeighborhood << auxUnsatisfactionNeighborhoodVector;
        theHouseholdFinancialStrainVector << auxFinancialStrainVector;
    } // i

    //qCritical() << "White:"<< averageIncomeOfWhiteHouseholds/numberOfWhiteHouseholds << "Black:" << averageIncomeOfBlackHouseholds/numberOfBlackHouseholds;

    QString theVariableBeingPlotted = QString("");
    QString theFilename = QString("");
    QFile file(theFilename);
    theFilename = QString("CensusStuff.txt");
    file.setFileName(theFilename);

    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);

        stream << this->objectName() << " , " << numberOfWhiteHouseholds  << " , " << theNumberOfUnsafeDwellings << " , " << theNumberOfUnsuitableDwellings << " , " << theNumberOfUnaffordableDwellings << " , " << theNumberOfUnsatisfiedDwellings << endl;
    }
    file.close();
    //qCritical() << this->objectName() << allDwellings << theNumberOfLowIncome << theNumberOfUnsafeDwellings << theNumberOfUnsuitableDwellings << theNumberOfUnaffordableDwellings << theNumberOfUnsatisfiedDwellings << theNumberOfHouseholdsThatfinancialStrain <<"";

    return 1;
}



int RHouseholdPortfolio::getHouseholdsFromFile(QString theCaller)
{
    // -------------------------------------------------------------------------------------------
    // 0 - Instantiate the owner based on census information
    // ------------------------------------------------------------------------------------------
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Instantiating owners and owners at" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);

    QVector<QString> readBlockGroup;
    QVector<QString> readBuildingType;
    QVector<QString> readBuildingOccupancyClasses;
    QVector<QString> readBuildingEra;
    QVector<QString> readBuildingTenure;
    QVector<QString> readBuildingMortgage;
    QVector<double> readBuildingValue;
    QVector<int> readBuildingDwellings;
    QVector<double> readHouseholdIncomeValue;
    QVector<int> readHouseholdSize;
    QVector<QString> readHouseholdRace;
    QVector<QString> readHouseholdChildren;
    QVector<QString> readHouseholdElderly;
    QVector<QString> readBuildingCode;

    // Properly read the file
    QString filename = QString(QString("CTs/CT") + theCaller.mid(2,6) + QString(".txt"));
    QFile file(filename);
    file.setFileName(filename);

    int skipper = 0;
    if(file.open(QIODevice::ReadOnly)){

        //QTextStream instream(&file);
        while(!file.atEnd()) {
            //QString line = instream.readLine();
            QString line = file.readLine();
            if (skipper == 0){
                skipper++;
            } else {

                //if(instream.status() != QTextStream::Ok) break; // check status before using the data
                //QStringList myList = line.split(",", QString::SkipEmptyParts);
                QStringList myList = line.split(',');
                readBlockGroup << myList.at(1);
                readBuildingType << myList.at(2);
                readBuildingOccupancyClasses << myList.at(3); // Occupancy;
                readBuildingEra << myList.at(4);
                readBuildingTenure << myList.at(5);
                readBuildingMortgage << myList.at(6);
                readBuildingValue << myList.at(7).toDouble();
                readHouseholdIncomeValue << myList.at(8).toDouble();
                readHouseholdSize << myList.at(9).toInt();
                readHouseholdRace << myList.at(10);
                readHouseholdChildren << myList.at(11);
                readHouseholdElderly << myList.at(12);
                readBuildingDwellings << 1;
                if (myList.at(4) == QString("Pre1940")){
                    readBuildingCode << QString("Pre");
                } else if (myList.at(4) == QString("Post1940")){
                    readBuildingCode << QString("Moderate");
                } else {
                    readBuildingCode << QString("High");
                }
            }
        }
        file.close();
    }

    theBuildingBlockGroup           = readBlockGroup;
    theNumberOfBuildings            = readBuildingType.size();
    theNumberOfDwellingsPerBuilding = readBuildingDwellings;
    theLandValueVector              = readBuildingValue;
    theBuildingValueVector          = readBuildingValue;
    theBuildingTypesVector          = readBuildingType;
    theBuildingOccupancyClassVector = readBuildingOccupancyClasses;
    theBuildingCodeLevelVector      = readBuildingCode;

    // Temporary
    int theNumberOfUnaffordableDwellings = 0;
    int theNumberOfUnsatisfiedDwellings = 0;
    int theNumberOfHouseholdsThatfinancialStrain = 0;
    int theNumberOfLowIncome = 0;
    int theNumberOfHighIncome = 0;
    int allDwellings = 0;

    for (int i = 0; i < theNumberOfBuildings; i++){

        // Household attributes
        QVector<int> auxHouseholdSizeVector;
        QVector<double> auxHomeownerIncomeValue;
        QVector<QString> auxOwnerIncomeVector;
        QVector<QString> auxTenureVector;
        QVector<QString> auxTenantIncomeVector;
        QVector<QString> auxrenterImmigrantStatusVector;
        QVector<QString> auxMaritalStatusVector;
        QVector<QString> auxRaceVector;
        QVector<QString> auxAgeCategory;
        QVector<bool> auxMortgageVector;
        QVector<bool> auxDisabilityVector;
        QVector<bool> auxHigherEdVector;
        QVector<bool> auxMovedRecentlyVector;
        QVector<bool> auxCarOwnershipVector;
        QVector<bool> auxUnsafeHousingVector;
        QVector<bool> auxUnaffordableHousingVector;
        QVector<bool> auxOvercrowdedHousingVector;
        QVector<bool> auxUnsatisfactionNeighborhoodVector;
        QVector<bool> auxFinancialStrainVector;


        //**************************
        // First, instantiate properties of the building/building owner, one per building
        //**************************
        QString ownerIncome = QString("High");
        double homeownerIncomeValue = readHouseholdIncomeValue.at(i);

        // Insurance coverage
        // The dispersion for the income distribution is chosen so that 5% make more 100k - that is roughly the case for Vancouver
        // The dispersion for the insurance percentage is chosen so that P(1.0) ~ 1
        bool insuranceCoverage = false;
        inputPercentageInsured = exp(((1.0/0.35) * qLn(theMedianHouseholdIncome / medianMunicipalHouseholdIncome)) * 0.25 + qLn(insuranceTakeUp));
        double theProbabilityOfInsurance = inputPercentageInsured;
        double rngInsurance = rand()/static_cast<double>(RAND_MAX);

        if (inputMunicipality == QString("Vancouver") && theBuildingOccupancyClassVector.at(i) == QString("RES3")){
            insuranceCoverage = true;

        } else if (rngInsurance <= theProbabilityOfInsurance){
            insuranceCoverage = true;
        }
        else {
            insuranceCoverage = false;
        }


        // Immigrant status of owner
        QString immigrantStatusOwner;
        double rngImmigrantOwner = rand()/static_cast<double>(RAND_MAX);

        if (rngImmigrantOwner < inputPercentageOfRecentImmigrants){
            immigrantStatusOwner = QString("RecentImmigrant");
        }
        else if (rngImmigrantOwner >= inputPercentageOfRecentImmigrants && rngImmigrantOwner < inputPercentageOfRecentImmigrants + inputPercentageOfEstablishedImmigrants){
            immigrantStatusOwner = QString("EstablishedImmigrant");
        }
        else {
            immigrantStatusOwner = QString("NonImmigrant");
        }


        //**************************
        // Now, instantiate properties of the households/building renters, one per dwelling
        //**************************
        for (int j = 0; j < theNumberOfDwellingsPerBuilding.at(i); j++){
            allDwellings++;

            // Immigrant status of renter
            QString immigrantStatusRenter;
            double rngImmigrantrenter = rand()/static_cast<double>(RAND_MAX);

            if (rngImmigrantrenter < inputPercentageOfRecentImmigrants){
                immigrantStatusRenter = QString("RecentImmigrant");
            }
            else if (rngImmigrantrenter >= inputPercentageOfRecentImmigrants && rngImmigrantrenter < inputPercentageOfRecentImmigrants + inputPercentageOfEstablishedImmigrants){
                immigrantStatusRenter = QString("EstablishedImmigrant");
            }
            else {
                immigrantStatusRenter = QString("NonImmigrant");
            }

            //theHousehold size
            int theHouseholdSize = readHouseholdSize.at(i);


            // Married households
            QString maritalStatsCategory = QString("SingleParent");
            double rngMarried = rand()/static_cast<double>(RAND_MAX);

            if (rngMarried < inputPercentageOfMarriedHouseholds){
                maritalStatsCategory = QString("MarriedCouple");
            }


            // Male householder
            bool maleHouseholder = false;
            double rngMale = rand()/static_cast<double>(RAND_MAX);

            if (rngMale < inputPercentageOfMaleHouseholders){
                maleHouseholder = true;
            }


            // Vulnerable population
            QString ageCategory = QString("None");
            if (readHouseholdChildren.at(i) == QString("TRUE") && readHouseholdElderly.at(i).contains(QString("TRUE"))){
                ageCategory = QString("ChildrenAndElderly");

            } else if (readHouseholdChildren.at(i) == QString("TRUE")){
                ageCategory = QString("Children");

            } else if (readHouseholdElderly.at(i) == QString("TRUE")){
                ageCategory = QString("Elderly");
            }


            // Race
            QString ethnicCategory = readHouseholdRace.at(i);
            bool hispanicCategory = false;
            if (readHouseholdRace.at(i).contains(QString("Hispanic"))){
                hispanicCategory = true;
            }


            // Person with disability
            bool disabilityCategory = false;
            double rngDisability = rand()/static_cast<double>(RAND_MAX);

            if (rngDisability < inputPercentageWithDisability){
                disabilityCategory = true;
            }


            // Higher education
            bool higherEducationCategory = false;
            double rngEducation = rand()/static_cast<double>(RAND_MAX);

            if (rngEducation < inputPercentageHigherEducation){
                higherEducationCategory = true;
            }


            // Define the dummy variables for the logit models
            int dummyElderly = 0;
            if (ageCategory == QString("Elderly")){
                dummyElderly = 1;
            }

            int dummyChildren = 0;
            if (ageCategory == QString("Children")){
                dummyChildren = 1;
            }

            int dummyDisability = 0;
            if (disabilityCategory == true){
                dummyDisability = 1;
            }

            int dummyImmigrant = 0;
            if (immigrantStatusOwner == QString("RecentImmigrant")){
                dummyImmigrant = 1;
            }

            int dummyHigherEd = 0;
            if (higherEducationCategory == true){
                dummyHigherEd = 1;
            }

            int dummyWhite = 0;
            if (ethnicCategory.contains(QString("White"))){
                dummyWhite = 1;
            }

            int dummyHispanic = 0;
            if (hispanicCategory == true){
                dummyHispanic = 1;
            }

            int dummyMarried = 0;
            if (maritalStatsCategory == QString("MarriedCouple")){
                dummyMarried = 1;
            }

            int dummyMale = 0;
            if (maleHouseholder == true){
                dummyMale = 1;
            }

            int dummyPre1940 = 0;
            if (readBuildingEra.at(i) == QString("Pre1940")){
                dummyPre1940 = 1;
            }

            int dummyPre1980 = 0;
            if (readBuildingEra.at(i) == QString("Pre1980")){
                dummyPre1980 = 1;
            }

            int dummySingleFamily = 0;
            if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
                dummySingleFamily = 1;
            }


            // Tenure
            QString tenure = readBuildingTenure.at(i);
            int dummyRenter = 0;
            if (tenure == QString("RenterOccupied")){
                dummyRenter = 1;
            }


            // Recently moved in
            bool recentlyMovedInStatus = false;
            double rngMovedIn = rand()/static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){
                double betaRenter_Moved = 1.73808;
                double betaElderly_Moved = -1.48906;

                double moverLogit = dummyRenter * betaRenter_Moved +
                        dummyElderly * betaElderly_Moved;

                double theProbOfMover = exp(moverLogit) / (1.0 + exp(moverLogit));

                if (theProbOfMover < 0 || theProbOfMover > 1){
                    qCritical() << this->objectName() << "has a problem with the logit for recent mover" << theProbOfMover;
                }

                if (rngMovedIn < theProbOfMover){
                    recentlyMovedInStatus = true;
                }

            } else {
                if (rngMovedIn < inputPercentageRecentlyMovedIn){
                    recentlyMovedInStatus = true;
                }
            }

            int dummyMoved = 0;
            if (recentlyMovedInStatus == true){
                dummyMoved = 1;
            }


            // Income
            // Pre-set some variables
            QString tenantIncome = QString("");
            if (readHouseholdIncomeValue.at(i) < 0.8 * 96500.0){
                tenantIncome = QString("Low");
            } else if (readHouseholdIncomeValue.at(i) > 2.0 * 96500.0){
                tenantIncome = QString("High");
            } else {
                tenantIncome = QString("Moderate");
            }

            if (tenure == "OwnerOccupied"){
                ownerIncome = tenantIncome;
            }

            int dummyLowIncome = 0;
            if (tenantIncome == QString("Low")){
                dummyLowIncome = 1;
            }

            int dummyModerateIncome = 0;
            if (tenantIncome == QString("Moderate")){
                dummyModerateIncome = 1;
            }


            // Mortgage
            // If owner-occupied renter income = owner income
            bool mortgage = false;
            if (readBuildingMortgage.at(i) == QString("TRUE")){
                mortgage = true;
            }

            int dummyMortgage = 0;
            if (mortgage == true){
                dummyMortgage = 1;
            }


            // Housing safety
            bool unsafe = false;
            double rngSafety = rand()/static_cast<double>(RAND_MAX);

            //if (theInstantiationMethod == QString("LogisticRegression")){
            double theCorrectionFactorForUnsafe = 1.0; // census tract data not available from census

            // Random variables for the housing unsafe model
            double beta0_Unsafe = -1.5104;
            double betaLowIncome_Unsafe = -1.2159;
            double betaModerateIncome_Unsafe = 0.3794;
            double betaMoved_Unsafe = 1.1021;
            double betaElderly_Unsafe = -2.2747;
            double betaDisability_Unsafe = 2.0365;
            double betaWhite_Unsafe = 0.7566;
            double betaHispanic_Unsafe = 0.7003;
            double betaMale_Unsafe = -1.4923;
            double betaPre1940_Unsafe = 0.7747;
            double betaPre1980_Unsafe = 0.6716;

            double unsafeLogit = beta0_Unsafe +
                    dummyLowIncome   * betaLowIncome_Unsafe +
                    dummyModerateIncome * betaModerateIncome_Unsafe +
                    dummyMoved       * betaMoved_Unsafe +
                    dummyElderly     * betaElderly_Unsafe +
                    dummyDisability  * betaDisability_Unsafe +
                    dummyWhite       * betaWhite_Unsafe +
                    dummyHispanic    * betaHispanic_Unsafe +
                    dummyMale        * betaMale_Unsafe +
                    dummyPre1940     * betaPre1940_Unsafe +
                    dummyPre1980     * betaPre1980_Unsafe;

            double theProbOfUnsafe = theCorrectionFactorForUnsafe * exp(unsafeLogit) / (1.0 + exp(unsafeLogit));

            if (theProbOfUnsafe < 0){
                theProbOfUnsafe = 0.0;
            } else if (theProbOfUnsafe > 1){
                theProbOfUnsafe = 1.0;
            }

            if (rngSafety < theProbOfUnsafe){
                unsafe = true;
            }


            // Overcrowding
            bool overcrowded = false;
            double rngOvercrowding = rand()/static_cast<double>(RAND_MAX);

            //if (theInstantiationMethod == QString("LogisticRegression")){
            double theCorrectionFactorForOwnerOvercrowded = inputPercentageOfOvercrowdedOwnerDwellings / 0.1065319;
            double theCorrectionFactorForRenterOvercrowded = inputPercentageOfOvercrowdedRenterDwellings / 0.06920684;
            double theCorrectionFactorForOvercrowded = 1.0;

            if (tenure == QString("Owner")){
                theCorrectionFactorForOvercrowded = theCorrectionFactorForOwnerOvercrowded;
            } else if (tenure == QString("Renter")){
                theCorrectionFactorForOvercrowded = theCorrectionFactorForRenterOvercrowded;
            }

            // logistic parameters
            double beta0_Overcrowding = -8.11071;
            double betaLowIncome_Overcrowding = 1.81546;
            double betaModerateIncome_Overcrowding = 0.93406;
            double betaSize_Overcrowding = 1.77589;
            double betaMover_Overcrowding = 0.70078;
            double betaWhite_Overcrowding = -0.67963;
            double betaHispanic_Overcrowding = 1.47947;

            // Overcrowding
            double overcrowdingLogit =    beta0_Overcrowding +
                    dummyLowIncome      * betaLowIncome_Overcrowding +
                    dummyModerateIncome * betaModerateIncome_Overcrowding +
                    theHouseholdSize    * betaSize_Overcrowding +
                    dummyMoved          * betaMover_Overcrowding +
                    dummyWhite          * betaWhite_Overcrowding +
                    dummyHispanic       * betaHispanic_Overcrowding;

            double theProbOfOvercrowding = theCorrectionFactorForOvercrowded * exp(overcrowdingLogit) / (1.0 + exp(overcrowdingLogit));

            if (theProbOfOvercrowding < 0.0){
                theProbOfOvercrowding = 0.0;

            } else if (theProbOfOvercrowding > 1.0){
                theProbOfOvercrowding = 1.0;
            }

            if (rngOvercrowding < theProbOfOvercrowding){
                overcrowded = true;
            }


            // Affordability
            bool unaffordable = false;
            double rngAffordability = rand()/static_cast<double>(RAND_MAX);

            if (theInstantiationMethod == QString("LogisticRegression")){

                double theCorrectionFactorUnaffordable = 1.0;
                if (dummyRenter == 0 && dummyMortgage == true){
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableOwnerDwellings / 0.2581649;

                } else if (dummyRenter == 0 && dummyMortgage == false) {
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableOwnerDwellings / 100000000.0;

                } else {
                    theCorrectionFactorUnaffordable = inputPercentageOfUnaffordableRenterDwellings / 0.1213064;
                }


                double beta0_Affordability = -2.2192;
                double betaLowIncome_Affordability = 2.5286;
                double betaModerateIncome_Affordability = 1.4498;

                //double affordabilityLogit = beta0_Affordability->getCurrentValue() + dummyMoved * betaMoved_Affordability->getCurrentValue() + dummyLowIncome * betaLowIncome_Affordability->getCurrentValue() + dummyModerateIncome * betaModerateIncome_Affordability->getCurrentValue();
                double affordabilityLogit = beta0_Affordability +
                        dummyLowIncome      * betaLowIncome_Affordability +
                        dummyModerateIncome * betaModerateIncome_Affordability;

                double theProbOfNotAffordable = theCorrectionFactorUnaffordable * exp(affordabilityLogit) / (1.0 + exp(affordabilityLogit));

                if (theProbOfNotAffordable < 0){
                    theProbOfNotAffordable = 0.0;
                } else if (theProbOfNotAffordable > 1){
                    theProbOfNotAffordable = 1.0;
                }

                if (rngAffordability < theProbOfNotAffordable){
                    unaffordable = true;
                    theNumberOfUnaffordableDwellings++;
                }

            } else {

                if (dummyRenter == 0){
                    if (rngAffordability < inputPercentageOfUnaffordableOwnerDwellings){
                        unaffordable = true;
                    }
                } else {
                    if (rngAffordability < inputPercentageOfUnaffordableRenterDwellings){
                        unaffordable = true;
                    }
                }
            }


            // Neighborhood dissatisfaction
            bool satisfiedWithNeighborhood = true;
            double rngNeighborhood = rand()/static_cast<double>(RAND_MAX);
            int dummyNeighborhoodDissatisfaction = 0;

            //if (theInstantiationMethod == QString("LogisticRegression")){

            // Random variables for the neighborhood satisfaction model
            double beta0_Neighborhood =	        -0.4497;
            double betaLowIncome_Neighborhood = -0.28815;
            double betaModerateIncome_Neighborhood = -1.13332;
            double betaElderly_Neighborhood =   -1.24549;
            double betaDisability_Neighborhood = 1.50053;
            double betaWhite_Neighborhood =     -0.55273;
            double betaHispanic_Neighborhood =   1.6973;
            double betaMale_Neighborhood =      -0.40181;
            double betaPre1940_Neighborhood =   0.71054;
            double betaPre1980_Neighborhood =   0.16726;


            double neighborhoodLogit = beta0_Neighborhood +
                    dummyLowIncome   * betaLowIncome_Neighborhood +
                    dummyModerateIncome * betaModerateIncome_Neighborhood +
                    dummyElderly     * betaElderly_Neighborhood +
                    dummyDisability  * betaDisability_Neighborhood +
                    dummyWhite       * betaWhite_Neighborhood +
                    dummyHispanic    * betaHispanic_Neighborhood +
                    dummyMale        * betaMale_Neighborhood +
                    dummyPre1940     * betaPre1940_Neighborhood +
                    dummyPre1980     * betaPre1980_Neighborhood;

            double theProbOfNotSatisfiedWithNeighborhood = exp(neighborhoodLogit) / (1.0 + exp(neighborhoodLogit));

            if (theProbOfNotSatisfiedWithNeighborhood < 0 || theProbOfNotSatisfiedWithNeighborhood > 1){
                qCritical() << this->objectName() << "has a problem with the logit for overcrowding" << theProbOfNotSatisfiedWithNeighborhood;
            }

            if (rngNeighborhood < theProbOfNotSatisfiedWithNeighborhood){
                satisfiedWithNeighborhood = false;
                if (dummyWhite == 1){
                    theNumberOfUnsatisfiedDwellings++;
                }
                dummyNeighborhoodDissatisfaction = 1;
            }

            //}

            // Financial strain
            bool financialStrain = false;
            double rngStrain = rand()/static_cast<double>(RAND_MAX);

            if (dummyRenter == 1){
                if (theInstantiationMethod == QString("LogisticRegression")){

                    double theCorrectionFactorForFinancialStrain = 1.0;
                    if (dummyRenter == 0 && dummyMortgage == true){
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedOwnerDwellings / 0.1119751;

                    } else if (dummyRenter == 0 && dummyMortgage == false) {
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedOwnerDwellings / 100000000.0;

                    } else {
                        theCorrectionFactorForFinancialStrain = inputPercentageOfFinancialyStrainedRenterDwellings / 0.05832037;
                    }

                    // Random variables for the food Strain model
                    double beta0_Strain = -3.4213;
                    double betaRenter_Strain = 0.5678;
                    double betaLowIncome_Strain = 3.8263;
                    double betaModerateIncome_Strain = 1.4155;
                    double betaImmigrant_Strain = 1.0808;

                    double strainLogit = beta0_Strain +
                            1.0 * betaRenter_Strain +
                            dummyLowIncome * betaLowIncome_Strain +
                            dummyModerateIncome * betaModerateIncome_Strain +
                            dummyImmigrant * betaImmigrant_Strain;

                    double theProbOfFinancialStrain = theCorrectionFactorForFinancialStrain * exp(strainLogit) / (1.0 + exp(strainLogit));

                    if (theProbOfFinancialStrain < 0.0){
                        theProbOfFinancialStrain = 0.0;

                    } else if (theProbOfFinancialStrain > 1.0){
                        theProbOfFinancialStrain = 1.0;
                    }

                    if (rngStrain < theProbOfFinancialStrain){
                        financialStrain = true;
                        theNumberOfHouseholdsThatfinancialStrain++;
                    }

                } else {
                    financialStrain = false;
                }
            }


            // Car ownership
            bool carOwnership;
            double rngCar = rand()/static_cast<double>(RAND_MAX);

            carOwnership = false;
            if (rngCar < inputPercentageWithCars){
                carOwnership = true;
            }

            // Build the vectors representing each building
            auxTenantIncomeVector << tenantIncome;
            auxTenureVector << tenure;
            auxMortgageVector << mortgage;
            auxrenterImmigrantStatusVector << immigrantStatusRenter;
            auxHouseholdSizeVector << theHouseholdSize;
            auxAgeCategory << ageCategory;
            auxRaceVector << ethnicCategory;
            auxDisabilityVector << disabilityCategory;
            auxHigherEdVector << higherEducationCategory;
            auxMaritalStatusVector << maritalStatsCategory;
            auxMovedRecentlyVector << recentlyMovedInStatus;
            auxCarOwnershipVector << carOwnership;
            auxUnsafeHousingVector << unsafe;
            auxUnaffordableHousingVector << unaffordable;
            auxOvercrowdedHousingVector << overcrowded;
            auxUnsatisfactionNeighborhoodVector << satisfiedWithNeighborhood;
            auxFinancialStrainVector << financialStrain;
            if (tenantIncome == QString("Low")){
                theNumberOfLowIncome++;
            } else if(tenantIncome == QString("High")){
                theNumberOfHighIncome++;
            }


        } // j


        if (theBuildingOccupancyClassVector.at(i) == QString("RES1")){
            theNumberOfSingleFamilyBuildings++;
        } else {
            theNumberOfMultiFamilyBuildings++;
        }

        // Owner properties
        theBuildingOwnerIncomeVector << ownerIncome;
        theBuildingOwnerImmigrantStatusVector << immigrantStatusOwner;
        theBuildingInsuranceVector << insuranceCoverage;
        theHomeownerIncomeValueVector << homeownerIncomeValue;

        // renter properties
        theHouseholdTenureVector << auxTenureVector;
        theHouseholdMortgageVector << auxMortgageVector;
        theHouseholdIncomeVector << auxTenantIncomeVector;
        theHouseholdImmigrantStatusVector << auxrenterImmigrantStatusVector;
        theHouseholdSizeVector << auxHouseholdSizeVector;
        theHouseholdAgeCategory << auxAgeCategory;
        theHouseholdRaceVector << auxRaceVector;
        theHouseholdDisabilityVector << auxDisabilityVector;
        theHouseholdHigherEdVector << auxHigherEdVector;
        theHouseholdMaritalStatusVector << auxMaritalStatusVector;
        theHouseholdMovedRecentlyVector << auxMovedRecentlyVector;
        theHouseholdCarOwnershipVector << auxCarOwnershipVector;
        theHouseholdUnsafeHousingVector << auxUnsafeHousingVector;
        theHouseholdUnaffordableHousingVector << auxUnaffordableHousingVector;
        theHouseholdOvercrowdedHousingVector << auxOvercrowdedHousingVector;
        theHouseholdUnsatisfiedNeighborhood << auxUnsatisfactionNeighborhoodVector;
        theHouseholdFinancialStrainVector << auxFinancialStrainVector;
    } // i
    return 0;
}

//RLocation * RHouseholdPortfolio::instantiateBuildingLocation()
//{
//    RContinuousRandomVariable *theLatitudeRandomVariable = nullptr;

//    if (!theLatitudeRandomVariable) {
//        QMap<double, double> latLngMap;
//        for (int i = 0; i < theBoundaryLocationList.count(); i++) {
//            latLngMap.insertMulti(theBoundaryLocationList[i]->getLatitude(), theBoundaryLocationList[i]->getLongitude());
//        }

//        QVector<double> latVector = latLngMap.uniqueKeys().toVector();
//        QVector<double> PDFVector;
//        for (int i = 0; i < latVector.count(); i++) {
//            double lat = latVector[i];

//            QVector<double> lngVector;

//            for (int j = 0; j < theBoundaryLocationList.count(); j++) {
//                // k is the next point
//                int k = j + 1;
//                if (k == theBoundaryLocationList.count()) {
//                    k = 0;
//                }
//                if ((theBoundaryLocationList[j]->getLatitude() - lat) * (theBoundaryLocationList[k]->getLatitude() - lat) < 0) {
//                    // The line between j and k intersects with this latitude
//                    double intersectionLng;
//                    /*// TEMP....................................................................................................................................................................
//                    double lng1 = theBoundaryLocationList[j]->getLongitude();
//                    double lng2 = theBoundaryLocationList[k]->getLongitude();
//                    double lat1 = theBoundaryLocationList[j]->getLatitude();
//                    double lat2 = theBoundaryLocationList[k]->getLatitude();
//                    intersectionLng = lng1 + (lng2 - lng1)/(lat2 - lat1) * (lat - lat1);*/

//                    int result = theBoundaryLocationList[j]->computeIntersectionWithLatitude(lat, theBoundaryLocationList[k], &intersectionLng);
//                    if (result < 0) {
//                        qCritical() << "Error 2: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
//                    }
//                    lngVector << intersectionLng;

//                    // OR CALCULATE THE INTERSECTION USING A LINEAR RELATIONSHIP BETWEEN LAT AND LNG. THIS IS HOW WE HAVE EVALAUTE THE PDF OF LATITUDE AFTER ALL.

//                } else if (theBoundaryLocationList[j]->getLatitude() == lat) {
//                    // The point j iteself is on the latitude

//                    // l is the previous point
//                    int l = j - 1;
//                    if (l == -1) {
//                        l = theBoundaryLocationList.count() - 1;
//                    }

//                    // Add the point j's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
//                    if ((theBoundaryLocationList[l]->getLatitude() - lat) * (theBoundaryLocationList[k]->getLatitude() - lat) <= 0) {
//                        lngVector << theBoundaryLocationList[j]->getLongitude();
//                    }
//                }
//            }

//            // Terje
//            if (remainder(lngVector.size(), 2.0) != 0.0) {
//                qCritical() << "Something is wrong in" << this->objectName() << " Please check your coordinates..." << lngVector.size();
//                return 0;
//            }

//            std::sort(lngVector.begin(), lngVector.end());

//            double length = 0;
//            for (int j = 0; j < lngVector.count(); j += 2) {
//                length += qAbs(lngVector[j+1] - lngVector[j]);
//            }
//            length *= cos(lat / 180 * 3.14159265359);

//            PDFVector << length;
//        }

//        theLatitudeRandomVariable = new RContinuousRandomVariable(0, objectName() + "LatRV");
//        theLatitudeRandomVariable->setDistributionType("User Defined");
//        theLatitudeRandomVariable->setUserDefinedPDF(latVector, PDFVector);
//    }


//    double beta1 = rand()/static_cast<double>(RAND_MAX);
//    double beta2 = rand()/static_cast<double>(RAND_MAX);

//    double latitude = theLatitudeRandomVariable->getInverseCDFValue(theta1);

//    bool isDifferentiable = true;

//    QVector<double> lngVector;
//    QVector<double> dLngVector;
//    double topOrBottomMostLongitude;
//    for (int i = 0; i < theBoundaryLocationList.count(); i++) {
//        int j = i + 1;
//        if (j == theBoundaryLocationList.count()) {
//            j = 0;
//        }
//        if ((theBoundaryLocationList[i]->getLatitude() - latitude) * (theBoundaryLocationList[j]->getLatitude() - latitude) < 0) {
//            double intersectionLng;
//            int result = theBoundaryLocationList[i]->computeIntersectionWithLatitude(latitude, theBoundaryLocationList[j], &intersectionLng);
//            if (result < 0) {
//                qCritical() << "Error 1: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
//            }
//            lngVector << intersectionLng;

//            if (theGradientType >= RGradientType::DDM) {
//                double lngForDiff;
//                double perturb = 0.0001;
//                int result = theBoundaryLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + perturb), theBoundaryLocationList[j], &lngForDiff);
//                if (result < 0) {
//                    isDifferentiable = false;
//                }
//                double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (perturb * latitude);
//                dLngVector << dIntersectionLongitude_dLatitude;
//            }
//        } else if (theBoundaryLocationList[i]->getLatitude() == latitude) {
//            // The point i iteself is on the latitude

//            // k is the previous point
//            int k = i - 1;
//            if (k == -1) {
//                k = theBoundaryLocationList.count() - 1;
//            }

//            // Add the point i's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
//            if ((theBoundaryLocationList[k]->getLatitude() - latitude) * (theBoundaryLocationList[j]->getLatitude() - latitude) <= 0) {
//                lngVector << theBoundaryLocationList[i]->getLongitude();
//            } else {
//                topOrBottomMostLongitude = theBoundaryLocationList[i]->getLongitude();
//            }

//            if (theGradientType >= RGradientType::DDM) {
//                if ((theBoundaryLocationList[k]->getLatitude() - latitude) * (theBoundaryLocationList[j]->getLatitude() - latitude) < 0) {
//                    isDifferentiable = false;
//                } else {
//                    // This condition is only for the cases when DDM is needed and the point is a vertex whose outgoing lines go in the same side of the latitude or one of the lines goes on the latitude
//                    double lngForDiff;
//                    double perturb = 0.0001;
//                    double intersectionLng = theBoundaryLocationList[i]->getLongitude();

//                    double sign = 1.0;
//                    if (latitude > theBoundaryLocationList[j]->getLatitude()) {
//                        sign = -1;
//                    }
//                    int result = theBoundaryLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + sign * perturb), theBoundaryLocationList[j], &lngForDiff);
//                    if (result >= 0) {
//                        double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (sign * perturb * latitude);
//                        dLngVector << dIntersectionLongitude_dLatitude;
//                    }

//                    sign = 1.0;
//                    if (latitude > theBoundaryLocationList[k]->getLatitude()) {
//                        sign = -1;
//                    }
//                    result = theBoundaryLocationList[i]->computeIntersectionWithLatitude(latitude * (1 + sign * perturb), theBoundaryLocationList[k], &lngForDiff);
//                    if (result >= 0) {
//                        double dIntersectionLongitude_dLatitude = (lngForDiff - intersectionLng) / (sign * perturb * latitude);
//                        dLngVector << dIntersectionLongitude_dLatitude;
//                    }

//                    // Correcting the order of the derivatives in the vector if this is a vertex whose outgoing lines go in the same side of the latitude
//                    if ((theBoundaryLocationList[k]->getLatitude() - latitude) * (theBoundaryLocationList[j]->getLatitude() - latitude) != 0) {
//                        if (theBoundaryLocationList[j]->getLongitude() > theBoundaryLocationList[k]->getLongitude()) {
//                            // Swapping
//                            dLngVector.append(dLngVector[0]);
//                            dLngVector.remove(0);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    QVector<double> lngVectorBackup = lngVector;
//    std::sort(lngVector.begin(), lngVector.end());

//    double deltaLng = 0;
//    for (int i = 0; i < lngVector.count(); i += 2) {
//        deltaLng += qAbs(lngVector[i+1] - lngVector[i]);
//    }


//    double dLng_dLat = 0;
//    if (dLngVector.count() != 2) {
//        isDifferentiable = false;
//    } else {
//        if (theGradientType >= RGradientType::DDM) {
//            if (lngVector.count() == 2) {
//                // Correcting the order of the derivatives in the vector
//                if (lngVectorBackup[0] > lngVectorBackup[1]) {
//                    dLngVector.append(dLngVector[0]);
//                    dLngVector.remove(0);
//                }
//            }
//            dLng_dLat = (dLngVector[1] - dLngVector[0]) * beta2 +  dLngVector[0];
//        }
//    }

//    double lngPortion = deltaLng * beta2;
//    double lngSoFar = 0;
//    double longitude = 0;
//    for (int i = 0; i < lngVector.count(); i += 2) {
//        lngSoFar += qAbs(lngVector[i+1] - lngVector[i]);
//        if (lngSoFar >= lngPortion) {
//            longitude = lngVector[i + 1] - (lngSoFar - lngPortion);
//            break;
//        }
//    }

//    // If the point is topmost ot bottommost point
//    if (lngVector.isEmpty()) {
//        longitude = topOrBottomMostLongitude;
//    }

//    theBuildingLocation->setLatitude(latitude);
//    theBuildingLocation->setLongitude(longitude);
//}

