#ifndef RSimpleNeighborhoodModel_H
#define RSimpleNeighborhoodModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RHouseholdPortfolio.h"
#include "RHazusBuildingModel.h"
#include "RPopulationDisplacementEstimator.h"
#include "RDisasterPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"


class RParameter;
class RResponse;

class RSimpleNeighborhoodModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    //Q_PROPERTY(QObject *Sa READ getSa WRITE setSa)
    Q_PROPERTY(QObject *BuildingDamageModel READ getBuildingDamageModel WRITE setBuildingDamageModel)
    Q_PROPERTY(QObject *PreparednessEstimator READ getPreparednessEstimator WRITE setPreparednessEstimator)
    Q_PROPERTY(QObject *DisplacementEstimator READ getDisplacementEstimator WRITE setDisplacementEstimator)
    Q_PROPERTY(QObject *MorbidityEstimator READ getMorbidityEstimator WRITE setMorbidityEstimator)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)

public:
    // Define the constructor and destructor
    RSimpleNeighborhoodModel(QObject *parent, QString name);
    ~RSimpleNeighborhoodModel();

    int resetTime();

    QObject* getHouseholdPortfolio() const;
    void setHouseholdPortfolio(QObject *value);

    QObject* getSa();
    void setSa(QObject *value);

    QObject* getBuildingDamageModel();
    void setBuildingDamageModel(QObject *value);

    QObject* getPreparednessEstimator();
    void setPreparednessEstimator(QObject *value);

    QObject* getDisplacementEstimator();
    void setDisplacementEstimator(QObject *value);

    QObject* getMorbidityEstimator();
    void setMorbidityEstimator(QObject *value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    QVector<RLocation*> getBuildingLocations();

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel();

    // Member functions called by the sampling model
    // Probably remove after paper
    QVector<double> getTimeVector();
    QVector<int> getHousingRecoveryVector();
    QVector<int> getDisplacedPopulationVector();
    QVector<int> getPopulationWithFamilyVector();
    QVector<int> getPopulationInSheltersVector();
    QVector<int> getPopulationInRentedHousingVector();
    QVector<int> getPopulationReplacedVector();
    int getNumberOfDwellings();
    int getNumberOfHabitableDwellings();
    int getNumberOfPeople();
    int getNumberOfDisplacedPersons();
    int getNumberOfPermanentlyDisplaced();
    int getNumberOfPersonsInHotels();
    int getNumberOfPersonsLivingWithFamily();
    int getNumberOfPersonsInShelters();
    double getTotalTimeDisplaced();

    // Member functions called by the sampling model
    QVector<double> getPoorVector();
    QVector<double> getModIncomeVector();
    QVector<double> getRichVector();
    QVector<double> getPreCodeVector();
    QVector<double> getLowCodeVector();
    QVector<double> getModCodeVector();
    QVector<double> getHighCodeVector();
    QVector<double> getOwnerVector();
    QVector<double> getRenterVector();
    QVector<double> getSFVector();
    QVector<double> getMFVector();
    QVector<double> getInsured();
    QVector<double> getNoInsured();
    QVector<double> getRecentVector();
    QVector<double> getEstablishedVector();
    QVector<double> getResidentVector();
    QVector<double> getWhiteVector();
    QVector<double> getBlackVector();
    QVector<double> getHispanicVector();
    QVector<double> getAsianVector();
    QVector<double> getNativeAmericanVector();
    QVector<double> getVulnerableMemberVector();
    QVector<double> getNoVulnerableMemberVector();
    QVector<double> getFunctionalHabitability();
    QVector<double> getPerceivedHabitability();

private:

    // MEMBER FUNCTIONS
    void plotUsableBuildings(double habitableArea);
    void plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name);
    void plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name);

    // DATA MEMBERS
    QString repairProgressFunction;
    QString theVariableOfInterest;

    // Responses coming to this model
    QPointer<RHouseholdPortfolio> theHouseholdPortfolio = nullptr;
    RResponse* theIntensityModelResponse = nullptr;
    RResponse* theBuildingModelResponse = nullptr;
    RDisasterPreparednessEstimator* thePreparednessEstimator = nullptr;
    RPopulationDisplacementEstimator* theDisplacementEstimator = nullptr;
    RSutley2016MorbidityEstimator* theMorbidityEstimator = nullptr;
    RHazusBuildingModel* theBuildingModel = nullptr;
    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;

    // Utility networks
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    double theWaterShortageDuration;
    double thePowerShortageDuration;

    // Community
    double theMedianHouseholdIncome;

    // Buildings
    int theNumberOfBuildings;
    double theTotalBuildingArea;
    double thePercentageOfBuildingsAtDS0;
    double thePercentageOfBuildingsAtDS1;
    double thePercentageOfBuildingsAtDS2;
    double thePercentageOfBuildingsAtDS3;
    double thePercentageOfBuildingsAtDS4;
    double thePercentageOfDwellingsAtDS0;
    double thePercentageOfDwellingsAtDS1;
    double thePercentageOfDwellingsAtDS2;
    double thePercentageOfDwellingsAtDS3;
    double thePercentageOfDwellingsAtDS4;
    int theTotalNumberOfDwellings;
    QVector<RLocation*> theBuildingLocation;
    QVector<int> theNumberOfDwellingsInTheBuilding;
    QVector<int> theBuildingNumberOfStoreys;
    QVector<int> theStructuralDamageState;
    QVector<int> theNSDDamageState;
    QVector<int> theNSADamageState;
    QVector<bool> theBuildingInsuranceStatus;
    QVector<double> theBuildingArea;
    QVector<double> theBuildingValue;
    QVector<QString> theBuildingType;
    QVector<QString> theBuildingOccupancyClass;
    QVector<QString> theBuildingCodeLevel;

    // Households
    int theNumberOfPeopleNotDisplaced;
    int theNumberOfPeopleSeekingShelter;
    int theNumberOfPeopleInHotels;
    int theNumberOfPeopleGone;
    int theNumberOfPeopleLivingWithTheirFamilies;
    int theTotalNumberOfPeople;
    double theAverageDamageState;
    double theOccupancyRate;
    QString theWeatherConditions;
    QVector<QVector<int>> theHouseholdSize;
    QVector<QVector<bool>> theHouseholdMortgage;
    QVector<QVector<bool>> theCarOwnership;
    QVector<QVector<bool>> theHouseholdDisabilityStatus;
    QVector<QVector<bool>> theHouseholdHigherEducationStatus;
    QVector<QVector<bool>> theHouseholdRecentlyMovedInStatus;
    QVector<QVector<bool>> theAvailabilityOfGenerator;
    QVector<QVector<bool>> theAvailabilityOfAlternativeWaterSource;
    QVector<QVector<bool>> theFamilyAsDestination;
    QVector<QVector<bool>> theHouseholdPredispositionToLeave;
    QVector<QVector<bool>> theHouseholdFatalityIncidence;
    QVector<QVector<double>> theTimeDisplaced;
    QVector<QVector<QString>> theDisasterDestination;
    QVector<QVector<QString>> theHouseholdIncome;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHouseholdEthnicity;
    QVector<QVector<QString>> theHousingTenure;
    QVector<QVector<QString>> theHouseholdImmigrantStatus;
    QVector<QVector<QString>> theHouseholdDestination; 

    // Outputs - maybe remove after papers
    double theRealTimeDisplaced;
    double theTotalTimeDisplaced;
    double totalPublicLoanRequest;

    QVector<double> theSFVector;
    QVector<double> theMFVector;
    QVector<double> thePreCodeVector;
    QVector<double> theLowCodeVector;
    QVector<double> theModCodeVector;
    QVector<double> theHighCodeVector;
    QVector<double> theOwnerVector;
    QVector<double> theRenterVector;
    QVector<double> thePoorVector;
    QVector<double> theModerateVector;
    QVector<double> theRichVector;
    QVector<double> theInsuredVector;
    QVector<double> theNotInsuredVector;
    QVector<double> theRecentVector;
    QVector<double> theEstablishedVector;
    QVector<double> theResidentVector;
    QVector<double> theWhiteVector;
    QVector<double> theBlackVector;
    QVector<double> theHispanicVector;
    QVector<double> theAsianVector;
    QVector<double> theNativeAmericanVector;
    QVector<double> theVulnerableMemberVector;
    QVector<double> theNoVulnerableMemberVector;
    QVector<double> theFunctionalHabitabilityVector;
    QVector<double> thePerceivedHabitabilityVector;


    // Intensity counters
    int theSa03Counter;
    int theSa1Counter;

    // Response
    RResponse* theResponse;
    RResponse* thePowerShortageDurationResponse;
    RResponse* theWaterShortageDurationResponse;

    // Plots
    QPointer<QCustomPlot> buildingRepairsPlot;
    QPointer<QCustomPlot> displacedPopulationPlot;

    double timeAnalysisBegan;
    double timeSinceLastPlot;
    QVector<double> theTickVector;
    QVector<QString> theLabelVector;

    int theNumberOfHabitableDwellings;
    double totalHabitableArea;
    QVector<int> theDisplacedPopulationVector;
    QVector<int> thePersonsWithFamilyVector;
    QVector<int> thePersonsInShelterVector;
    QVector<int> thePersonsInRenterHousingVector;
    QVector<int> thePersonsGoneVector;
    QVector<double> timeVector;
    QVector<int> theHousingRecoveryVector;

};

#endif // RSimpleNeighborhoodModel_H
