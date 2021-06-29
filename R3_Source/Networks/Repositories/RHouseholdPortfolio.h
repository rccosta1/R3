#ifndef RHouseholdPortfolio_H
#define RHouseholdPortfolio_H

#include "RModel.h"
#include "RResponse.h"
#include "RRequest.h"
#include "RNetworkModel.h"
#include "RBuildingPortfolio.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RHouseholdPortfolio : public RObject
{ 
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    //Q_PROPERTY(QObject *BuildingPortfolio READ getBuildingPortfolio WRITE setBuildingPortfolio)
    Q_PROPERTY(QString InstantiationMethod READ getInstantiationMethod WRITE setInstantiationMethod)
    Q_PROPERTY(QString Municipality READ getMunicipality WRITE setMunicipality)
    Q_PROPERTY(int TotalPopulation READ getTotalPopulation WRITE setTotalPopulation)
    Q_PROPERTY(double PercentageSingleFamilyDwellings READ getPercentageSingleFamilyDwellings WRITE setPercentageSingleFamilyDwellings)
    Q_PROPERTY(double MedianHouseholdIncome READ getMedianHouseholdIncome WRITE setMedianHouseholdIncome)
    Q_PROPERTY(double PercentageLowIncome READ getPercentageLowIncome WRITE setPercentageLowIncome)
    Q_PROPERTY(double PercentageModerateIncome READ getPercentageModerateIncome WRITE setPercentageModerateIncome)
    Q_PROPERTY(double PercentageHighIncome READ getPercentageHighIncome WRITE setPercentageHighIncome)
    Q_PROPERTY(QString HouseholdSizeDistribution READ getHouseholdSizeDistribution WRITE setHouseholdSizeDistribution)
    Q_PROPERTY(double PercentageMarried READ getPercentageMarried WRITE setPercentageMarried)
    Q_PROPERTY(double PercentageMaleHouseholders READ getPercentageMaleHouseholders WRITE setPercentageMaleHouseholders)
    Q_PROPERTY(double PercentageOfChildren READ getPercentageOfChildren WRITE setPercentageOfChildren)
    Q_PROPERTY(double PercentageOfSeniors READ getPercentageOfSeniors WRITE setPercentageOfSeniors)
    Q_PROPERTY(double PercentageOfRecentImmigrants READ getPercentageOfRecentImmigrants WRITE setPercentageOfRecentImmigrants)
    Q_PROPERTY(double PercentageOfEstablishedImmigrants READ getPercentageOfEstablishedImmigrants WRITE setPercentageOfEstablishedImmigrants)
    Q_PROPERTY(double PercentageRecentlyMovedIn READ getPercentageRecentlyMovedIn WRITE setPercentageRecentlyMovedIn)
    Q_PROPERTY(QString AgeDistribution READ getAgeDistribution WRITE setAgeDistribution)
    Q_PROPERTY(QString HousingCostDistribution READ getHousingCostDistribution WRITE setHousingCostDistribution)
    Q_PROPERTY(double PercentageOfWhiteHouseholds READ getPercentageOfWhiteHouseholds  WRITE setPercentageOfWhiteHouseholds )
    Q_PROPERTY(double PercentageOfBlackHouseholds READ getPercentageOfBlackHouseholds WRITE setPercentageOfBlackHouseholds)
    Q_PROPERTY(double PercentageOfAsianHouseholds READ getPercentageOfAsianHouseholds WRITE setPercentageOfAsianHouseholds)
    Q_PROPERTY(double PercentageOfNativeAmericanHouseholds READ getPercentageOfNativeAmericanHouseholds WRITE setPercentageOfNativeAmericanHouseholds)
    Q_PROPERTY(double PercentageOfWhiteHispanicHouseholds READ getPercentageOfWhiteHispanicHouseholds  WRITE setPercentageOfWhiteHispanicHouseholds )
    Q_PROPERTY(double PercentageOfBlackHispanicHouseholds READ getPercentageOfBlackHispanicHouseholds WRITE setPercentageOfBlackHispanicHouseholds)
    Q_PROPERTY(double PercentageOfAsianHispanicHouseholds READ getPercentageOfAsianHispanicHouseholds WRITE setPercentageOfAsianHispanicHouseholds)
    Q_PROPERTY(double PercentageOfNativeAmericanHispanicHouseholds READ getPercentageOfNativeAmericanHispanicHouseholds WRITE setPercentageOfNativeAmericanHispanicHouseholds)
    //Q_PROPERTY(double PercentageOfHispanicHouseholds READ getPercentageOfHispanicHouseholds WRITE setPercentageOfHispanicHouseholds)
    Q_PROPERTY(double PercentageHigherEducation READ getPercentageHigherEducation WRITE setPercentageHigherEducation)
    Q_PROPERTY(double PercentageWithDisability READ getPercentageWithDisability WRITE setPercentageWithDisability)
    Q_PROPERTY(double PercentageOfRentedBuildings READ getPercentageOfRentedBuildings WRITE setPercentageOfRentedBuildings)
    Q_PROPERTY(double AverageRent READ getAverageRent WRITE setAverageRent)
    Q_PROPERTY(double PercentageWithMortgage READ getPercentageWithMortgage WRITE setPercentageWithMortgage)
    Q_PROPERTY(double PercentageOfHouseholdsWithCars READ getPercentageOfHouseholdsWithCars WRITE setPercentageOfHouseholdsWithCars)
    Q_PROPERTY(double PercentageUnaffordableOwnerDwellings READ getPercentageUnaffordableOwnerDwellings WRITE setPercentageUnaffordableOwnerDwellings)
    Q_PROPERTY(double PercentageUnaffordableRenterDwellings READ getPercentageUnaffordableRenterDwellings WRITE setPercentageUnaffordableRenterDwellings)
    Q_PROPERTY(double PercentageFinancialyStrainedOwnerDwellings READ getPercentageFinancialyStrainedOwnerDwellings WRITE setPercentageFinancialyStrainedOwnerDwellings)
    Q_PROPERTY(double PercentageFinancialyStrainedRenterDwellings READ getPercentageFinancialyStrainedRenterDwellings WRITE setPercentageFinancialyStrainedRenterDwellings)
    Q_PROPERTY(double PercentageUnsafeDwellings READ getPercentageUnsafeDwellings WRITE setPercentageUnsafeDwellings)
    Q_PROPERTY(double PercentageOvercrowdedOwnerOccupiedDwellings READ getPercentageOvercrowdedOwnerOccupiedDwellings WRITE setPercentageOvercrowdedOwnerOccupiedDwellings)
    Q_PROPERTY(double PercentageOvercrowdedRenterOccupiedDwellings READ getPercentageOvercrowdedRenterOccupiedDwellings WRITE setPercentageOvercrowdedRenterOccupiedDwellings)
    Q_PROPERTY(double PercentageDwellingsUsedFoodStamps READ getPercentageDwellingsUsedFoodStamps WRITE setPercentageDwellingsUsedFoodStamps)
    Q_PROPERTY(QString BoundaryPointsLatitude READ getBoundaryPointsLatitude WRITE setBoundaryPointsLatitude)
    Q_PROPERTY(QString BoundaryPointsLongitude READ getBoundaryPointsLongitude WRITE setBoundaryPointsLongitude)

    Q_OBJECT

public:

    RHouseholdPortfolio(QObject *parent, QString name);
    ~RHouseholdPortfolio();

    int resetTime();

    // Attributes
    QObject *getLocation() const;
    void setLocation(QObject *value);

    //QObject *getBuildingPortfolio();
    //void setBuildingPortfolio(QObject *value);

    QString getInstantiationMethod();
    void setInstantiationMethod(QString value);

    int getTotalPopulation() const;
    void setTotalPopulation(int value);

    double getPercentageSingleFamilyDwellings() const;
    void setPercentageSingleFamilyDwellings(double value);

    double getMedianHouseholdIncome() const;
    void setMedianHouseholdIncome(double value);

    double getPercentageOfRentedBuildings() const;
    void setPercentageOfRentedBuildings(double value);

    double getAverageRent() const;
    void setAverageRent(double value);

    double getPercentageWithMortgage() const;
    void setPercentageWithMortgage(double value);

    double getPercentageLowIncome() const;
    void setPercentageLowIncome(double value);

    double getPercentageModerateIncome() const;
    void setPercentageModerateIncome(double value);

    double getPercentageHighIncome() const;
    void setPercentageHighIncome(double value);

    QString getHouseholdSizeDistribution();
    void setHouseholdSizeDistribution(QString value);

    double getPercentageMarried() const;
    void setPercentageMarried(double value);

    double getPercentageMaleHouseholders() const;
    void setPercentageMaleHouseholders(double value);

    double getPercentageOfChildren() const;
    void setPercentageOfChildren(double value);

    double getPercentageOfSeniors() const;
    void setPercentageOfSeniors(double value);

    QString getAgeDistribution();
    void setAgeDistribution(QString value);

    QString getHousingCostDistribution();
    void setHousingCostDistribution(QString value);

    double getPercentageOfWhiteHouseholds() const;
    void setPercentageOfWhiteHouseholds(double value);

    double getPercentageOfBlackHouseholds() const;
    void setPercentageOfBlackHouseholds(double value);

    double getPercentageOfAsianHouseholds() const;
    void setPercentageOfAsianHouseholds(double value);

    double getPercentageOfNativeAmericanHouseholds() const;
    void setPercentageOfNativeAmericanHouseholds(double value);

    double getPercentageOfWhiteHispanicHouseholds() const;
    void setPercentageOfWhiteHispanicHouseholds(double value);

    double getPercentageOfBlackHispanicHouseholds() const;
    void setPercentageOfBlackHispanicHouseholds(double value);

    double getPercentageOfAsianHispanicHouseholds() const;
    void setPercentageOfAsianHispanicHouseholds(double value);

    double getPercentageOfNativeAmericanHispanicHouseholds() const;
    void setPercentageOfNativeAmericanHispanicHouseholds(double value);

    double getPercentageOfHispanicHouseholds() const;

    double getPercentageHigherEducation() const;
    void setPercentageHigherEducation(double value);

    double getPercentageWithDisability() const;
    void setPercentageWithDisability(double value);

    double getPercentageOfHouseholdsWithCars() const;
    void setPercentageOfHouseholdsWithCars(double value);

    double getPercentageOfInsuredBuildings() const;
    void setPercentageOfInsuredBuildings(double value);

    double getPercentageOfRecentImmigrants() const;
    void setPercentageOfRecentImmigrants(double value);

    double getPercentageOfEstablishedImmigrants() const;
    void setPercentageOfEstablishedImmigrants(double value);

    double getPercentageRecentlyMovedIn() const;
    void setPercentageRecentlyMovedIn(double value);

    double getPercentageUnaffordableOwnerDwellings() const;
    void setPercentageUnaffordableOwnerDwellings(double value);

    double getPercentageUnaffordableRenterDwellings() const;
    void setPercentageUnaffordableRenterDwellings(double value);

    double getPercentageFinancialyStrainedOwnerDwellings() const;
    void setPercentageFinancialyStrainedOwnerDwellings(double value);

    double getPercentageFinancialyStrainedRenterDwellings() const;
    void setPercentageFinancialyStrainedRenterDwellings(double value);

    double getPercentageUnsafeDwellings() const;
    void setPercentageUnsafeDwellings(double value);

    double getPercentageOvercrowdedOwnerOccupiedDwellings() const;
    void setPercentageOvercrowdedOwnerOccupiedDwellings(double value);

    double getPercentageOvercrowdedRenterOccupiedDwellings() const;
    void setPercentageOvercrowdedRenterOccupiedDwellings(double value);

    double getPercentageDwellingsUsedFoodStamps() const;
    void setPercentageDwellingsUsedFoodStamps(double value);

    QString getMunicipality() const;
    void setMunicipality(QString value);

    QString getBoundaryPointsLatitude() const;
    void setBoundaryPointsLatitude(QString value);

    QString getBoundaryPointsLongitude() const;
    void setBoundaryPointsLongitude(QString value);

    // Instantiators
    int instantiateHouseholds();
    int getHouseholdsFromFile(QString theCaller);
    int getNumberOfBuildings() const;
    int getNumberOfSingleFamilyBuildings() const;
    int getNumberOfMultiFamilyBuildings() const;

    double getMedianBuildingValue() const;
    QVector<int> getBuildingStoreysVector() const;
    QVector<int> getBuildingDwellingsVector() const;
    QVector<bool> getBuildingInsuranceStatus() const;
    QVector<double> getBuildingAreaVector() const;
    QVector<double> getLandValueVector() const;
    QVector<double> getBuildingValueVector() const;
    QVector<double> getHomeownerIncomeValueVector() const;
    QVector<QString> getBuildingBlockGroup() const;
    QVector<QString> getBuildingTypesVector() const;
    QVector<QString> getBuildingCodeLevelVector() const;
    QVector<QString> getBuildingOccupancyClassVector() const;
    QVector<QString> getBuildingOwnerIncomeVector() const;
    QVector<QString> getBuildingOwnerImmigrantStatusVector() const;
    QVector<RLocation*> getBuildingLocationVector() const;

    QVector<QVector<int>> getHouseholdSizeVector() const;
    QVector<QVector<bool>> getMortgageVector() const;
    QVector<QVector<bool>> getHouseholdCarOwnershipVector() const;
    QVector<QVector<bool>> getHouseholdDisabilityStatus() const;
    QVector<QVector<bool>> getHouseholdEducationStatus() const;
    QVector<QVector<bool>> getHouseholdRecentMoverStatus() const;
    QVector<QVector<bool>> getHouseholdUnsafeHousingVector() const;
    QVector<QVector<bool>> getHouseholdUnaffordableHousingVector() const;
    QVector<QVector<bool>> getHouseholdOvercrowdedHousingVecto() const;
    QVector<QVector<bool>> getHouseholdUnsatisfiedNeighborhood() const;
    QVector<QVector<bool>> getHouseholdFinancialStrain() const;
    QVector<QVector<QString>> getHouseholdMaritalStatus() const;
    QVector<QVector<QString>> getHouseholdAgeCategoryVector() const;
    QVector<QVector<QString>> getHouseholdImmigrantStatusVector() const;
    QVector<QVector<QString>> getHouseholdIncomeVector() const;
    QVector<QVector<QString>> getHouseholdRaceVector() const;
    QVector<QVector<QString>> getHouseholdTenureVector() const;
    QVector<QVector<QString>> getHouseholdArchetypeVector() const;

    double getFatalityMultiplier() const;
    QVector<QVector<double>> getBoundariesVector();

protected:


private: 

    //RLocation * instantiateBuildingLocation();

    // DATA MEMBERS
    // Input data members are provided by the user as strings
    QList<RLocation *> theBoundaryLocationList;

    RBuildingPortfolio * theBuildingPortfolio;

    RParameter* theLocation;
    int theTotalPopulation;
    double theMedianHouseholdIncome;
    double inputPercentageSingleFamily;
    double inputPercentageLowIncome;
    double inputPercentageModerateIncome;
    double inputPercentageHighIncome;
    double inputPercentageOfMarriedHouseholds;
    double inputPercentageOfMaleHouseholders;
    double inputPercentageOfChildren;
    double inputPercentageOfSeniors;
    double inputPercentageOfRenters;
    double inputAverageRent;
    double inputPercentageWithMortgage;
    double inputPercentageWithCars;
    double inputPercentageInsured;
    double inputPercentageOfRecentImmigrants;
    double inputPercentageOfEstablishedImmigrants;
    double inputPercentageRecentlyMovedIn;
    double inputPercentageOfUnaffordableOwnerDwellings;
    double inputPercentageOfUnaffordableRenterDwellings;
    double inputPercentageOfFinancialyStrainedOwnerDwellings;
    double inputPercentageOfFinancialyStrainedRenterDwellings;
    double inputPercentageOfUnsafeDwellings;
    double inputPercentageOfOvercrowdedOwnerDwellings;
    double inputPercentageOfOvercrowdedRenterDwellings;
    double inputPercentageUsedFoodStampsDwellings;
    double inputPercentageOfWhiteHouseholds;
    double inputPercentageOfBlackHouseholds;
    double inputPercentageOfAsianHouseholds;
    double inputPercentageOfNativeAmericanHouseholds;
    double inputPercentageOfWhiteHispanicHouseholds;
    double inputPercentageOfBlackHispanicHouseholds;
    double inputPercentageOfAsianHispanicHouseholds;
    double inputPercentageOfNativeAmericanHispanicHouseholds;
    double inputPercentageHigherEducation;
    double inputPercentageWithDisability;
    double medianMunicipalHouseholdIncome;
    double insuranceTakeUp;
    QString inputBoundaryPointsLatitudes;
    QString inputBoundaryPointsLongitudes;
    QVector<double> inputHouseholdSizeVector;
    QVector<double> inputAgeDistributionVector;
    QVector<double> inputHousingCostDistributionVector;
    QVector<double> inputBuildingAreas;
    QVector<double> inputBuildingValues;

    QString theInstantiationMethod;
    QString inputMunicipality;
    QString inputBuildingDensity;

    // Properties of the buildings
    RLocation *theBuildingLocation;
    int theNumberOfBuildings;
    int theNumberOfSingleFamilyBuildings;
    int theNumberOfMultiFamilyBuildings;
    QVector<RLocation*> theBuildingLocationVector;
    double theMedianBuildingValue;
    QVector<int> theBuildingStoreysVector;
    QVector<int> theNumberOfDwellingsPerBuilding;
    QVector<int> theBuildingYearBuilt;
    QVector<int> theBuildingNumberOfRooms;
    QVector<bool> theBuildingInsuranceVector;
    QVector<double> theBuildingAreasVector;
    QVector<double> theLandValueVector;
    QVector<double> theBuildingValueVector;
    QVector<double> theHomeownerIncomeValueVector;
    QVector<QString> theBuildingBlockGroup;
    QVector<QString> theBuildingOccupancyClassVector;
    QVector<QString> theBuildingCodeLevelVector;
    QVector<QString> theBuildingTypesVector;
    QVector<QString> theBuildingTenureVector;
    QVector<QString> theBuildingOwnerIncomeVector;
    QVector<QString> theBuildingOwnerImmigrantStatusVector;

    // Properties of the Household
    QVector<QVector<int>> theHouseholdSizeVector;
    QVector<QVector<bool>> theHouseholdDisabilityVector;
    QVector<QVector<bool>> theHouseholdHigherEdVector;
    QVector<QVector<bool>> theHouseholdMovedRecentlyVector;
    QVector<QVector<bool>> theHouseholdCarOwnershipVector;
    QVector<QVector<bool>> theHouseholdMortgageVector;
    QVector<QVector<bool>> theHouseholdUnsafeHousingVector;
    QVector<QVector<bool>> theHouseholdUnaffordableHousingVector;
    QVector<QVector<bool>> theHouseholdOvercrowdedHousingVector;
    QVector<QVector<bool>> theHouseholdUnsatisfiedNeighborhood;
    QVector<QVector<bool>> theHouseholdFinancialStrainVector;
    QVector<QVector<QString>> theHouseholdMaritalStatusVector;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHouseholdImmigrantStatusVector;
    QVector<QVector<QString>> theHouseholdIncomeVector;    
    QVector<QVector<QString>> theHouseholdTenureVector;
    QVector<QVector<QString>> theHouseholdArchetypeVector;
    QVector<QVector<QString>> theHouseholdRaceVector;

    QPointer<RContinuousRandomVariable> theLocationTheta1;
    QPointer<RContinuousRandomVariable> theLocationTheta2;

};

#endif
