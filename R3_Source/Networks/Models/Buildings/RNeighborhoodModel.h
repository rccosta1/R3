#ifndef RNeighborhoodModel_H
#define RNeighborhoodModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RHouseholdPortfolio.h"
#include "RHazusBuildingModel.h"
#include "RPopulationDisplacementEstimator.h"
#include "RDisasterPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RFinancingRequest.h"
#include "REngineeringAssessmentRequest.h"
#include "RPermitRequest.h"
#include "RContractorNegotiationRequest.h"
#include "RSingleFamilyContractorRequest.h"
#include "RMultiFamilyContractorRequest.h"
#include "RSingleFamilyContractorFirmModel.h"
#include "RMultiFamilyContractorFirmModel.h"
#include "RAlisjahbana2021FinancingTimeEstimator.h"

class RParameter;
class RResponse;

class RNeighborhoodModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    Q_PROPERTY(QObject *BuildingDamageModel READ getBuildingDamageModel WRITE setBuildingDamageModel)
    Q_PROPERTY(QObject *DisplacementEstimator READ getDisplacementEstimator WRITE setDisplacementEstimator)
    Q_PROPERTY(QObject *FinancingTimeEstimator READ getFinancingTimeEstimator WRITE setFinancingTimeEstimator)
    Q_PROPERTY(QString ContractorList READ getContractorList WRITE setContractorList)
    //Q_PROPERTY(QString FinancialResourcesSupplierList READ getFinancialResourcesSupplierList WRITE setFinancialResourcesSupplierList)
    Q_PROPERTY(int SamplingStartingPoint READ getSamplingStartingPoint WRITE setSamplingStartingPoint)


public:
    // Define the constructor and destructor
    RNeighborhoodModel(QObject *parent, QString name);
    ~RNeighborhoodModel();

    // MEMBER FUNCTIONS
    QObject* getHouseholdPortfolio() const;
    void setHouseholdPortfolio(QObject *value);

    QObject* getSa();
    void setSa(QObject *value);

    QObject* getBuildingDamageModel();
    void setBuildingDamageModel(QObject *value);

    QObject* getDisplacementEstimator();
    void setDisplacementEstimator(QObject *value);

    QObject* getFinancingTimeEstimator();
    void setFinancingTimeEstimator(QObject *value);

//    QString getFinancialResourcesSupplierList();
//    void setFinancialResourcesSupplierList(QString value);

    QString getContractorList();
    void setContractorList(QString value);

    int getSamplingStartingPoint();
    void setSamplingStartingPoint(int value);

    QVector<double> getTimeVector();

    // DATA MEMBERS
    // Responses coming to this model
    QPointer<RHouseholdPortfolio> theHouseholdPortfolio = nullptr;
    RResponse* theIntensityModelResponse = nullptr;
    RResponse* theBuildingDamageModelResponse = nullptr;
    RHazusBuildingModel* theBuildingDamageModel = nullptr;
    RPopulationDisplacementEstimator* theDisplacementEstimator = nullptr;
    RAlisjahbana2021FinancingTimeEstimator *theFinancingTimeEstimator = nullptr;
    QList<QPointer<RParameter>> theContractorList;
    //QList<QPointer<RParameter>> theFinancialResourcesSupplierList;

    // Community
    double theMedianHouseholdIncome;
    double theAverageDamageState;

    // Buildings
    int theNumberOfBuildings;
    int theTotalNumberOfDwellings;
    double theNumberOfHabitableDwellings;
    double theTotalBuildingArea;

    int getNumberOfRepairedDwellings();
    QVector<int> theDamagedBuildingsIndexes;
    QVector<int> theNotableBuildingsIndexes;
    QVector<int> theBuildingNumberOfDwellings;
    QVector<int> theBuildingNumberOfStoreys;
    QVector<int> theBuildingStrDamageState;
    QVector<int> theBuildingNSDDamageState;
    QVector<int> theBuildingNSADamageState;
    QVector<double> theBuildingArea;
    QVector<double> theBuildingLandValue;
    QVector<double> theBuildingValue;
    QVector<double> theBuildingRepairTime;
    QVector<double> theBuildingReoccupancyTime;
    QVector<double> theBuildingTimeUnderRepairs;
    QVector<QString> theBuildingBlockGroup;
    QVector<QString> theBuildingType;
    QVector<QString> theBuildingOccupancyClass;
    QVector<QString> theBuildingCodeLevel;
    QVector<RLocation*> theBuildingLocation;

    // Households
    int theNumberOfPersons;
    int theNumberOfRepairedDwellings;

    // Requests
    int numberOfWorkforceRequestsCreatedHere;
    int numberOfMoneyRequestsCreatedHere;

    RFinancingRequest *theInsuranceRequest;
    RFinancingRequest *thePublicLoanRequest;
    RFinancingRequest *thePrivateLoanRequest;
    RFinancingRequest *theGrantRequest;
    RContractorNegotiationRequest *theContractorNegotiationRequest;
    RSingleFamilyContractorRequest *theSFContractorAllocationRequest;
    RMultiFamilyContractorRequest *theMFContractorAllocationRequest;
    QVector<bool> alreadyRequestedFinancing;
    QVector<bool> alreadyRequestedNegotiation;

    // Intensity counters
    int theSa03Counter;
    int theSa1Counter;
    int theSamplingStartingPoint = -1;

    // Response
    RResponse* theResponse;

    // Plots
    QVector<double> timeVector;

private:
};

#endif // RNeighborhoodModel_H
