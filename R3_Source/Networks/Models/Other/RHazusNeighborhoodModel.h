#ifndef RHazusNeighborhoodModel_H
#define RHazusNeighborhoodModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RHouseholdPortfolio.h"
#include "RHazusBuildingModel.h"


class RParameter;
class RResponse;

class RHazusNeighborhoodModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    //Q_PROPERTY(QObject *Sa READ getSa WRITE setSa)
    Q_PROPERTY(QObject *BuildingDamageModel READ getBuildingDamageModel WRITE setBuildingDamageModel)

public:
    // Define the constructor and destructor
    RHazusNeighborhoodModel(QObject *parent, QString name);
    ~RHazusNeighborhoodModel();

    int resetTime();

    QObject* getHouseholdPortfolio() const;
    void setHouseholdPortfolio(QObject *value);

    QObject* getSa();
    void setSa(QObject *value);

    QObject* getBuildingDamageModel();
    void setBuildingDamageModel(QObject *value);

    // Define the method where the actual model is coded
    int evaluateModel(RGradientType theGradientType);

    // Member functions called by the sampling model
    // Probably remove after paper
    int getNumberOfDwellings();
    int getNumberOfPeople();


    //added by Chenbo
    int getNumberOfPeopleRequiringShortermHousing();
    double getNumberOfPeopleTimesDaysDisplaced();



private:


    // Responses coming to this model
    QPointer<RHouseholdPortfolio> theHouseholdPortfolio = nullptr;    
    RResponse* theIntensityModelResponse = nullptr;
    RResponse* theBuildingModelResponse = nullptr;
    RHazusBuildingModel* theBuildingModel = nullptr;

    // Utility networks

    // Community
    QVector<double> HouseholdSizeDistribution;
    double theMedianHouseholdIncome;
    double thePercentageLowIncome;
    double thePercentageModerateIncome;
    double thePercentageHighIncome;
    double thePercentageLowIncomePerson;
    double thePercentageModerateIncomePerson;
    double thePercentageHighIncomePerson;
    double thePercentageRecentImmigrants;
    double thePercentageRecentImmigrantsPerson;
    double thePercentageEstablishedImmigrants;
    double thePercentageEstablishedImmigrantsPerson;
    double thePercentageResidents;
    double thePercentageResidentsPerson;
    double thePercentageOfHouseholdWithChild;
    double thePercentageOfHouseholdWithSenior;

    double thePercentageSingleFamilyModerateDS;
    double thePercentageSingleFamilyExtensiveDS;
    double thePercentageSingleFamilyCompleteDS;
    double thePercentageMultiFamilyModerateDS;
    double thePercentageMultiFamilyExtensiveDS;
    double thePercentageMultiFamilyCompleteDS;

    int theNumberOfSingleFamilyDwellings;
    int theNumberOfMultiFamilyDwellings;
    int theNumberOfSingleFamilyBuildings;
    int theNumberOfMultiFamilyBuildings;

    double thePercentageOfHI1;
    double thePercentageOfHI2;
    double thePercentageOfHI3;
    double thePercentageOfHI4;
    double thePercentageOfHI5;

    double thePercentageOfHO1;
    double thePercentageOfHO2;

    double theHA1;
    double theHA2;
    double theHA3;

    QVector<double> theHIVector;
    QVector<double> theHEVector;
    QVector<double> theHOVector;
    QVector<double> theHAVector;

    QVector<double> theWeightFactorsOfSingleFamily;
    QVector<double> theWeightFactorsOfMultiFamily;
    double thePercentageOfDisplacedSingleFamilyHouseholds;
    double thePercentageOfDisplacedMultiFamilyHouseholds;

    QVector<double> theIncomeModificationFactors;
    QVector<double> theEthnicityModificationFactors;
    QVector<double> theOwnershipModificationFactors;
    QVector<double> theAgeModificationFactors;

    double theAverageDamageState;
    double thePercentageDS0;
    double thePercentageDS1;
    double thePercentageDS2;
    double thePercentageDS3;
    double thePercentageDS4;
    double theDS0;
    double theDS1;
    double theDS2;
    double theDS3;
    double theDS4;


    // Buildings
    int theNumberOfBuildings;
    double theTotalBuildingArea;
    int theTotalNumberOfDwellings;
    QVector<int> theNumberOfDwellingsInTheBuilding;
    QVector<QString> theBuildingType;
    QVector<QString> theOccupancyClass;
    QVector<QString> theCodeLevel;
    QVector<int> theStructuralDamageState;

    // Households
    int theTotalNumberOfPeople;
    QVector<QVector<int>> theHouseholdSize;
    QVector<QVector<QString>> theHouseholdIncome;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHousingTenure;
    QVector<QVector<QString>> theHouseholdImmigrantStatus;

    // Intensity counters
    int theSa03Counter;
    int theSa1Counter;

    // Response
    RResponse* theResponse;
    double theNumberOfDisplacedHouseholds;
    double theNumberOfPeopleRequiringShortermHousing;
    double theNumberOfPeopleTimesDaysDisplaced;

};

#endif // RHazusNeighborhoodModel_H
