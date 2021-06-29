#ifndef RCensusBuildingPortfolio_H
#define RCensusBuildingPortfolio_H

#include "RObject.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;
class RRandomVariable;

class RCensusBuildingPortfolio : public RObject
{ 
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QString Municipality READ getMunicipality WRITE setMunicipality)
    Q_PROPERTY(double MedianHouseholdIncome READ getMedianHouseholdIncome WRITE setMedianHouseholdIncome)
    Q_PROPERTY(double PercentageLowIncome READ getPercentageLowIncome WRITE setPercentageLowIncome)
    Q_PROPERTY(double PercentageModerateIncome READ getPercentageModerateIncome WRITE setPercentageModerateIncome)
    Q_PROPERTY(double PercentageHighIncome READ getPercentageHighIncome WRITE setPercentageHighIncome)
    Q_PROPERTY(QString HouseholdSizeDistribution READ getHouseholdSizeDistribution WRITE setHouseholdSizeDistribution)
    Q_PROPERTY(double PercentageOfChildren READ getPercentageOfChildren WRITE setPercentageOfChildren)
    Q_PROPERTY(double PercentageOfSeniors READ getPercentageOfSeniors WRITE setPercentageOfSeniors)
    Q_PROPERTY(double PercentageOfRecentImmigrants READ getPercentageOfRecentImmigrants WRITE setPercentageOfRecentImmigrants)
    Q_PROPERTY(double PercentageOfEstablishedImmigrants READ getPercentageOfEstablishedImmigrants WRITE setPercentageOfEstablishedImmigrants)
    Q_PROPERTY(QString AgeDistribution READ getAgeDistribution WRITE setAgeDistribution)
    Q_PROPERTY(double PercentageOfRentedBuildings READ getPercentageOfRentedBuildings WRITE setPercentageOfRentedBuildings)
    Q_PROPERTY(double PercentageWithMortgage READ getPercentageWithMortgage WRITE setPercentageWithMortgage)
    Q_PROPERTY(double PercentageOfHouseholdsWithCars READ getPercentageOfHouseholdsWithCars WRITE setPercentageOfHouseholdsWithCars)
    Q_PROPERTY(double PercentageUnaffordableDwellings READ getPercentageUnaffordableDwellings WRITE setPercentageUnaffordableDwellings)
    Q_PROPERTY(double PercentageUnsafeDwellings READ getPercentageUnsafeDwellings WRITE setPercentageUnsafeDwellings)
    Q_PROPERTY(double PercentageNotSuitableDwellings READ getPercentageNotSuitableDwellings WRITE setPercentageNotSuitableDwellings)
    Q_PROPERTY(double PercentagePre1940Buildings READ getPercentagePre1940Buildings WRITE setPercentagePre1940Buildings)
    Q_PROPERTY(double PercentagePre1975Buildings READ getPercentagePre1975Buildings WRITE setPercentagePre1975Buildings)
    Q_PROPERTY(double PercentagePost1975Buildings READ getPercentagePost1975Buildings WRITE setPercentagePost1975Buildings)
    Q_PROPERTY(QString BuildingTypes READ getBuildingTypes WRITE setBuildingTypes)
    Q_PROPERTY(QString BuildingOccupancyClasses READ getBuildingOccupancyClasses WRITE setBuildingOccupancyClasses)
    Q_PROPERTY(QString BuildingNumberPerType READ getBuildingNumberPerType WRITE setBuildingNumberPerType)
    Q_PROPERTY(QString BuildingAreas READ getBuildingAreas WRITE setBuildingAreas)
    Q_PROPERTY(QString BuildingValues READ getBuildingValues WRITE setBuildingValues)
    Q_PROPERTY(QString BuildingDensity READ getBuildingDensity WRITE setBuildingDensity)
    Q_PROPERTY(QString BoundaryPointsLatitude READ getBoundaryPointsLatitude WRITE setBoundaryPointsLatitude)
    Q_PROPERTY(QString BoundaryPointsLongitude READ getBoundaryPointsLongitude WRITE setBoundaryPointsLongitude)
    Q_PROPERTY(double RetrofitLevel READ getRetrofitLevel WRITE setRetrofitLevel)

    Q_OBJECT
    Q_ENUMS(RNumberOfStories)
public:
    enum RFacilityType{SF, MF2to4, MF5to9, MF10to20};

    RCensusBuildingPortfolio(QObject *parent, QString name);
    ~RCensusBuildingPortfolio();

    int resetTime();

    // Attributes
    QObject *getLocation() const;
    void setLocation(QObject *value);

    double getMedianHouseholdIncome() const;
    void setMedianHouseholdIncome(double value);

    double getPercentageLowIncome() const;
    void setPercentageLowIncome(double value);

    double getPercentageModerateIncome() const;
    void setPercentageModerateIncome(double value);

    double getPercentageHighIncome() const;
    void setPercentageHighIncome(double value);

    QString getHouseholdSizeDistribution();
    void setHouseholdSizeDistribution(QString value);

    double getPercentageOfChildren() const;
    void setPercentageOfChildren(double value);

    double getPercentageOfSeniors() const;
    void setPercentageOfSeniors(double value);

    QString getAgeDistribution();
    void setAgeDistribution(QString value);

    double getPercentageOfRentedBuildings() const;
    void setPercentageOfRentedBuildings(double value);

    double getPercentageWithMortgage() const;
    void setPercentageWithMortgage(double value);

    double getPercentageOfHouseholdsWithCars() const;
    void setPercentageOfHouseholdsWithCars(double value);

    double getPercentageOfInsuredBuildings() const;
    void setPercentageOfInsuredBuildings(double value);

    double getPercentageOfRecentImmigrants() const;
    void setPercentageOfRecentImmigrants(double value);

    double getPercentageOfEstablishedImmigrants() const;
    void setPercentageOfEstablishedImmigrants(double value);

    double getPercentageUnaffordableDwellings() const;
    void setPercentageUnaffordableDwellings(double value);

    double getPercentageUnsafeDwellings() const;
    void setPercentageUnsafeDwellings(double value);

    double getPercentageNotSuitableDwellings() const;
    void setPercentageNotSuitableDwellings(double value);

    QString getMunicipality() const;
    void setMunicipality(QString value);

    QString getBoundaryPointsLatitude() const;
    void setBoundaryPointsLatitude(QString value);

    QString getBoundaryPointsLongitude() const;
    void setBoundaryPointsLongitude(QString value);

    double getPercentagePre1940Buildings() const;
    void setPercentagePre1940Buildings(double value);

    double getPercentagePre1975Buildings() const;
    void setPercentagePre1975Buildings(double value);

    double getPercentagePost1975Buildings() const;
    void setPercentagePost1975Buildings(double value);

    QString getBuildingTypes();
    void setBuildingTypes(QString value);

    QString getBuildingOccupancyClasses();
    void setBuildingOccupancyClasses(QString value);

    QString getBuildingNumberPerType();
    void setBuildingNumberPerType(QString value);

    QString getBuildingAreas();
    void setBuildingAreas(QString value);

    QString getBuildingValues();
    void setBuildingValues(QString value);

    QString getBuildingDensity();
    void setBuildingDensity(QString value);

    double getRetrofitLevel();
    void setRetrofitLevel(double value);

    // Instantiators
    int instantiateBuildings();

    // Extractors
    int getNumberOfBuildings() const;
    int getNumberOfSingleFamilyBuildings() const;
    int getNumberOfMultiFamilyBuildings() const;

    QVector<QString> getBuildingTenureVector() const;
    QVector<QString> getBuildingArchetypeVector() const;
    QVector<QString> getBuildingTypesVector() const;
    QVector<QString> getBuildingCodeLevelVector() const;
    QVector<QString> getBuildingOccupancyClassVector() const;
    QVector<int> getBuildingStoreysVector() const;
    QVector<int> getBuildingDwellingsVector() const;
    QVector<double> getBuildingAreaVector() const;
    QVector<double> getBuildingValueVector() const;
    QVector<QPair<double, double>> getBuildingLocationVector() const;
    QVector<QVector<bool>> getMortgageVector() const;
    QVector<QVector<int>> getHouseholdSizeVector() const;
    QVector<QVector<QString>> getHouseholdAgeCategoryVector() const;
    QVector<QVector<QString>> getHouseholdImmigrantStatusVector() const;
    QVector<QVector<bool>> getHouseholdCarOwnershipVector() const;
    QVector<QVector<bool>> getHouseholdInsuranceStatus() const;
    QVector<QVector<QString>> getHouseholdHouseholdIncomeVector() const;
    QVector<QVector<QString>> getHouseholdTenureVector() const;
    QVector<QVector<QString>> getHouseholdArchetypeVector() const;
    QVector<QVector<bool>> getHouseholdPredispositionToLeave() const;
    double getFatalityMultiplier() const;
    QVector<QVector<double>> getBoundariesVector();

protected:


private: 

    // DATA MEMBERS
    QPair<double,double>  instantiateBuildingLocation();


    // Census data
    QList<QPointer<RParameter> > myCensusInformationList;


    // Input data members are provided by the user as strings
    RContinuousRandomVariable *theLatitudeRandomVariable;
    QList<RLocation *> theLocationList;

    RParameter* theLocation;
    double theMedianHouseholdIncome;
    double inputPercentageLowIncome;
    double inputPercentageModerateIncome;
    double inputPercentageHighIncome;
    QVector<double> inputHouseholdSizeVector;
    double inputPercentageOfChildren;
    double inputPercentageOfSeniors;
    double inputPercentageOfRenters;
    double inputPercentageWithMortgage;
    double inputPercentageWithCars;
    double inputPercentageInsured;
    double inputPercentageOfRecentImmigrants;
    double inputPercentageOfEstablishedImmigrants;
    double inputPercentageOfUnaffordableDwellings;
    double inputPercentageOfUnsafeDwellings;
    double inputPercentageOfNotSuitableDwellings;
    QString inputBoundaryPointsLatitudes;
    QString inputBoundaryPointsLongitudes;
    double inputPercentageOfPre1940Buildings;
    double inputPercentageOfPre1975Buildings;
    double inputPercentageOfPost1975Buildings;
    QString inputBuildingTypes;
    QString inputBuildingOccupancyClasses;
    QVector<double> inputAgeDistributionVector;
    QVector<int> inputDwellingsPerBuildingType;
    QVector<double> inputBuildingAreas;
    QVector<double> inputBuildingValues;
    QString inputMunicipality;
    QString inputBuildingDensity;


    double medianMunicipalHouseholdIncome;
    double insuranceTakeUp;
    double theRetrofitLevel;


    // Properties of the buildings
    int theNumberOfBuildings;
    QVector<QString> theBuildingOccupancyClassVector;
    QVector<QString> theBuildingCodeLevelVector;
    QVector<QString> theBuildingTypesVector;
    QVector<int> theBuildingStoreysVector;
    QVector<int> theNumberOfDwellingsPerBuilding;
    QVector<double> theBuildingAreasVector;
    QVector<double> theBuildingValueVector;
    QVector<QString> theBuildingArchetypeVector;
    QVector<QPair<double,double>> theBuildingLocationVector;
    QVector<QString> theBuildingTenureVector;

    // Properties of household households
    QVector<QVector<QString>> householdAgeCategory;
    QVector<QVector<QString>> householdImmigrantStatusVector;
    QVector<QVector<QString>> householdHouseholdIncomeVector;
    QVector<QVector<QString>> householdTenureVector;
    QVector<QVector<QString>> householdArchetypeVector;
    QVector<QVector<bool>> householdCarOwnershipVector;
    QVector<QVector<bool>> theBuildingInsuranceVector;
    QVector<QVector<bool>> householdMortgageVector;
    QVector<QVector<bool>> householdPredispositionToLeave;
    QVector<QVector<int>> householdSizeVector;


};

#endif
