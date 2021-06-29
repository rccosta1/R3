#ifndef RDamageTestModel_H
#define RDamageTestModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RCensusInformation.h"
#include "RSimpleBuildingDamageModel.h"
#include "RSimpleBuildingLossModel.h"
#include "RSimpleBuildingResponseModel.h"
#include "RSimpleBuildingRepairNeedsModel.h"
#include "RHazusBuildingModel.h"


class RParameter;
class RResponse;

class RDamageTestModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *CensusInformation READ getCensusInformation WRITE setCensusInformation)
    Q_PROPERTY(QObject *TheIntensityList READ getIntensityList WRITE setIntensityList)
    Q_PROPERTY(QObject *BuildingDamageModel READ getBuildingDamageModel WRITE setBuildingDamageModel)

public:
    // Define the constructor and destructor
    RDamageTestModel(QObject *parent, QString name);
    ~RDamageTestModel();

    int resetTime();

    QObject* getBuildingDamageModel();
    void setBuildingDamageModel(QObject *value);

    QObject* getCensusInformation() const;
    void setCensusInformation(QObject *value);

    QObject* getIntensityList();
    void setIntensityList(QObject *value);

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel(RGradientType theGradientType);


    // Member functions called by the sampling model
    // Probably remove after paper
    QVector<double> getTimeVector();
    QVector<double> getHousingRecoveryVector();
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
    int getNumberOfShelteredOnSite();
    int getNumberOfPersonsLivingWithFamily();
    int getNumberOfPersonsInShelters();
    QVector<double> getRecoveryTimes();
    QVector<QString> getHouseholdArchetype();


    // Member functions called by the sampling model
    // Probably remove after paper
    QVector<double> getPoorVector();
    QVector<double> getModerateVector();
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


private:

    // MEMBER FUNCTIONS
    void plotUsableBuildings(double habitableArea);


    // DATA MEMBERS
    QString repairProgressFunction;
    QString theVariableOfInterest;


    // Responses coming to this model
    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;
    QList<QPointer<RParameter>> theContractorList;
    QList<QPointer<RParameter>> theBuildingInspectorsList;
    QList<QPointer<RParameter>> theEngineeringFirmsList;
    QList<QPointer<RParameter>> thePermittingAuthorityList;
    QList<QPointer<RParameter>> theMaterialSupplierList;
    QList<QPointer<RParameter>> theFinancialResourcesSupplierList;
    QPointer<RCensusInformation> theCensusInformation = nullptr;
    RResponse* theIntensityModelResponse = nullptr;
    RResponse* theDamageModelResponse = nullptr;
    RSimpleBuildingResponseModel* theResponseModel = nullptr;
    RHazusBuildingModel* theDamageModel = nullptr;


    // Utility networks
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    double theWaterShortageDuration;
    double thePowerShortageDuration;


    // Buildings
    int theNumberOfBuildings;
    double theTotalBuildingArea;
    int theTotalNumberOfDwellings;
    double theMedianHouseholdIncome;
    QVector<QString> theHouseholdArchetype;
    QVector<QString> theBuildingOwnerIncome;
    QVector<bool> theInsuranceStatus;
    QVector<QString> theImmigrantStatus;
    QVector<QString> theBuildingType;
    QVector<QString> theOccupancyClass;
    QVector<QString> theBuildingTenure;
    QVector<QString> theCodeLevel;
    QVector<int> theBuildingDamageState;
    QVector<int> theBuildingNumberOfStoreys;
    QVector<int> theNumberOfDwellingsInTheBuilding;
    QVector<double> theBuildingArea;
    QVector<double> theBuildingValue;
    QVector<double> theTimeUnderRepairs;
    QVector<double> theImmediateEconomicLoss;
    QVector<double> theImmediateContentLoss;
    QVector<double> theTimeNeededForRepairs;
    QVector<double> theTimeNeededForReoccupation;
    QVector<QPair<double,double>> theBuildingLocation;



    // Households
    int theNumberOfPeopleAtHome;
    int theSnowballFactor;
    int theNumberOfPeopleSeekingShelter;
    int theNumberOfPeopleInHotels;
    int theNumberOfPeopleGone;
    int theNumberOfPeopleLivingWithTheirFamilies;

    double theApparentDamage;
    double averageDamageState;

    QVector<QVector<double>> theTimeDisplaced;

    // Outputs - maybe remove after papers
    int theNumberOfDwellingsAtDS0;
    int theNumberOfDwellingsAtDS1;
    int theNumberOfDwellingsAtDS2;
    int theNumberOfDwellingsAtDS3;
    int theNumberOfDwellingsAtDS4;


    // Response
    RResponse* theResponse;
    RResponse* thePowerShortageDurationResponse;
    RResponse* theWaterShortageDurationResponse;
    double theNumberOfPeopleTimesDaysDisplaced;


    // Plots
    QPointer<QCustomPlot> buildingRepairsPlot;
    QPointer<QCustomPlot> displacedPopulationPlot;
    QVector<double> tickVector;
    QVector<QString> labelVector;
    double timeAnalysisBegan;
    double timeSinceLastPlot;
    QVector<double> timeVector;
    QVector<double> theHousingRecoveryVector;
    QVector<int> theDisplacedPopulationVector;
    QVector<int> thePersonsWithFamilyVector;
    QVector<int> thePersonsInShelterVector;
    QVector<int> thePersonsInRenterHousingVector;
    QVector<int> thePersonsGoneVector;


    double totalHabitableArea;
    int theNumberOfHabitableDwellings;
};

#endif // RDamageTestModel_H
