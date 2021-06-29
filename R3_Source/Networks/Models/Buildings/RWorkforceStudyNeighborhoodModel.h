#ifndef RWorkforceStudyNeighborhoodModel_H
#define RWorkforceStudyNeighborhoodModel_H

#include "RModel.h"
#include "RNeighborhoodModel.h"
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


class RParameter;
class RResponse;

class RWorkforceStudyNeighborhoodModel : public RNeighborhoodModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    //Q_PROPERTY(QObject *Sa READ getSa WRITE setSa)
    Q_PROPERTY(QObject *BuildingDamageModel READ getBuildingDamageModel WRITE setBuildingDamageModel)
    Q_PROPERTY(QObject *PreparednessEstimator READ getPreparednessEstimator WRITE setPreparednessEstimator)
    Q_PROPERTY(QObject *DisplacementEstimator READ getDisplacementEstimator WRITE setDisplacementEstimator)
    Q_PROPERTY(QObject *MorbidityEstimator READ getMorbidityEstimator WRITE setMorbidityEstimator)
    Q_PROPERTY(QString ContractorList READ getContractorList WRITE setContractorList)
    Q_PROPERTY(QString FinancialResourcesSupplierList READ getFinancialResourcesSupplierList WRITE setFinancialResourcesSupplierList)
    Q_PROPERTY(double ProbabilityOfLiquefaction READ getProbabilityOfLiquefaction WRITE setProbabilityOfLiquefaction)
    Q_PROPERTY(int SamplingStartingPoint READ getSamplingStartingPoint WRITE setSamplingStartingPoint)

public:
    // Define the constructor and destructor
    RWorkforceStudyNeighborhoodModel(QObject *parent, QString name);
    ~RWorkforceStudyNeighborhoodModel();

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

    QString getFinancialResourcesSupplierList();
    void setFinancialResourcesSupplierList(QString value);

    QString getContractorList();
    void setContractorList(QString value);

    double getProbabilityOfLiquefaction();
    void setProbabilityOfLiquefaction(double value);

    int getSamplingStartingPoint();
    void setSamplingStartingPoint(int value);

    QVector<RLocation*> getBuildingLocations();

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel();

    // Member functions called by the sampling model
    int getNumberOfDwellings();
    int getNumberOfHabitableDwellings();
    int getNumberOfPeople();
    int getNumberOfDisplacedPersons();
    int getNumberOfPermanentlyDisplaced();
    int getNumberOfPersonsEvicted();
    int getNumberOfPersonsLeavingVoluntarily();
    int getNumberOfPersonsInHotels();
    int getNumberOfPersonsLivingWithFamily();
    int getNumberOfPersonsInShelters();
    double getTotalTimeDisplaced();
    int getNumberOfDamageLowIncomeSFBuildings();
    int getNumberOfDamageModerateIncomeSFBuildings();
    int getNumberOfDamageHighIncomeSFBuildings();
    int getNumberOfLowIncomeSFBuildings();
    int getNumberOfModerateIncomeSFBuildings();
    int getNumberOfHighIncomeSFBuildings();
    QVector<int> getDisplacedPopulationVector();
    QVector<int> getPopulationWithFamilyVector();
    QVector<int> getPopulationInSheltersVector();
    QVector<int> getPopulationInRentedHousingVector();
    QVector<int> getPopulationReplacedVector();
    QVector<double> getTimeVector();
    QVector<int> getHousingRecoveryVector();
    QVector<double> getRecoveryStartTimeVector();
    QVector<double> getReceivedFundTimeVector();
    QVector<double> getLowIncomeReceivedFundTimeVector();
    QVector<double> getModerateIncomeReceivedFundTimeVector();
    QVector<double> getHighIncomeReceivedFundTimeVector();
    QVector<double> getRecoveryTimeVector();
    QVector<double> getLowIncomeRecoveryStartTimeVector();
    QVector<double> getModerateIncomeRecoveryStartTimeVector();
    QVector<double> getHighIncomeRecoveryStartTimeVector();
    QVector<double> getLowIncomeRecoveryTimeVector();
    QVector<double> getModerateIncomeRecoveryTimeVector();
    QVector<double> getHighIncomeRecoveryTimeVector();

    // Member functions called by the sampling model
    QVector<double> getPoorVector();
    QVector<double> getModIncomeVector();
    QVector<double> getRichVector();
    QVector<double> getPreCodeVector();
    QVector<double> getLowCodeVector();
    QVector<double> getModCodeVector();
    QVector<double> getHighCodeVector();
    QVector<double> getSFVector();
    QVector<double> getMFVector();
    QVector<double> getInsured();
    QVector<double> getNoInsured();

    QVector<double> getOwnerVoluntaryMobilityVector();
    QVector<double> getRenterVoluntaryMobilityVector();
    QVector<double> getLowIncomeVoluntaryMobilityVector();
    QVector<double> getModerateIncomeVoluntaryMobilityVector();
    QVector<double> getHighIncomeVoluntaryMobilityVector();
    QVector<double> getRecentImmigrantVoluntaryMobilityVector();
    QVector<double> getWhiteVoluntaryMobilityVector();
    QVector<double> getBlackVoluntaryMobilityVector();
    QVector<double> getHispanicVoluntaryMobilityVector();
    QVector<double> getAsianVoluntaryMobilityVector();
    QVector<double> getElderlyMemberVoluntaryMobilityVector();
    QVector<double> getChildrenMemberVoluntaryMobilityVector();
    QVector<double> getHigherEducatedVoluntaryMobilityVector();
    QVector<double> getSingleParentsVoluntaryMobilityVector();
    QVector<double> getDisableVoluntaryMobilityVector();

    QVector<double> getOwnerEvictionVector();
    QVector<double> getRenterEvictionVector();
    QVector<double> getLowIncomeEvictionVector();
    QVector<double> getModerateIncomeEvictionVector();
    QVector<double> getHighIncomeEvictionVector();
    QVector<double> getRecentImmigrantEvictionVector();
    QVector<double> getWhiteEvictionVector();
    QVector<double> getBlackEvictionVector();
    QVector<double> getHispanicEvictionVector();
    QVector<double> getAsianEvictionVector();
    QVector<double> getElderlyMemberEvictionVector();
    QVector<double> getChildrenMemberEvictionVector();
    QVector<double> getHigherEducatedEvictionVector();
    QVector<double> getSingleParentsEvictionVector();
    QVector<double> getDisableEvictionVector();

    QVector<double> getOwnerNoFundsVector();
    QVector<double> getRenterNoFundsVector();
    QVector<double> getLowIncomeNoFundsVector();
    QVector<double> getModerateIncomeNoFundsVector();
    QVector<double> getHighIncomeNoFundsVector();
    QVector<double> getRecentImmigrantNoFundsVector();
    QVector<double> getWhiteNoFundsVector();
    QVector<double> getBlackNoFundsVector();
    QVector<double> getHispanicNoFundsVector();
    QVector<double> getAsianNoFundsVector();
    QVector<double> getElderlyMemberNoFundsVector();
    QVector<double> getChildrenMemberNoFundsVector();
    QVector<double> getHigherEducatedNoFundsVector();
    QVector<double> getSingleParentsNoFundsVector();
    QVector<double> getDisableNoFundsVector();

    void compileResults();
    QVector<double> getUninhabitableDwellingsVector();
    QVector<double> getVoluntaryRelocationVector();
    QVector<double> getEvictionVector();
    QVector<double> getNoFundsVector();

private:

    // MEMBER FUNCTIONS
    void plotUsableBuildings(double habitableArea);
    void plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name);
    void plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name);
    void plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name);
    void plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name);

    // DATA MEMBERS
    QString repairProgressFunction;
    QString theVariableOfInterest;

    // Responses coming to this model
    QPointer<RHouseholdPortfolio> theHouseholdPortfolio = nullptr;
    RResponse* theIntensityModelResponse = nullptr;
    RResponse* theBuildingDamageModelResponse = nullptr;
    RDisasterPreparednessEstimator* thePreparednessEstimator = nullptr;
    RPopulationDisplacementEstimator* theDisplacementEstimator = nullptr;
    RSutley2016MorbidityEstimator* theMorbidityEstimator = nullptr;
    RHazusBuildingModel* theBuildingDamageModel = nullptr;
    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;
    QList<QPointer<RParameter>> theContractorList;
    QList<QPointer<RParameter>> theBuildingInspectorsList;
    QList<QPointer<RParameter>> theEngineeringFirmList;
    QList<QPointer<RParameter>> theBuildingPermitAuthorityList;
    QList<QPointer<RParameter>> theMaterialSupplierList;
    QList<QPointer<RParameter>> theFinancialResourcesSupplierList;
    double theProbabilityOfLiquefaction;

    // Utility networks
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    double theWaterShortageDuration;
    double thePowerShortageDuration;

    // Community
    double theMedianHouseholdIncome;

    // Buildings
    QVector<int> theDamagedBuildingsIndexes;

    int theNumberOfBuildings;
    int theTotalNumberOfDwellings;
    double theTotalNumberOfRecentImmigrantDwellings;
    double theTotalNumberOfEstablishedImmigrantDwellings;
    double theTotalNumberOfCitizenDwellings;
    double theTotalBuildingArea;
    double theBuildingAverageRent;
    QVector<RLocation*> theBuildingLocation;
    QVector<int> theBuildingNumberOfDwellings;
    QVector<int> theBuildingNumberOfStoreys;
    QVector<int> theBuildingStrDamageState;
    QVector<int> theBuildingNSDDamageState;
    QVector<int> theBuildingNSADamageState;
    QVector<bool> theBuildingNeedForRedesign;
    QVector<bool> theBuildingInsuranceStatus;
    QVector<double> theBuildingArea;
    QVector<double> theBuildingLandValue;
    QVector<double> theBuildingValue;
    QVector<double> theBuildingTimeSold;
    QVector<double> theBuildingThresholdTimeToBeSold;
    QVector<double> theBuildingThresholdProbabilityToBeEvicted;
    QVector<double> theBuildingThresholdTimeToMakeADecision;
    QVector<QString> theBuildingType;
    QVector<QString> theBuildingOccupancyClass;
    QVector<QString> theBuildingCodeLevel;
    QVector<QString> theBuildingOwnerIncome;
    QVector<QString> theBuildingOwnerImmigrationStatus;

    QVector<int> theRepairClass;
    QVector<int> theNumberOfWorkersAvailable;
    QVector<bool> theInspectionState;
    QVector<bool> theBiddingProcess;
    QVector<bool> theEngineeringAssessment;
    QVector<bool> theBuildingPermitStatus;
    QVector<double> theDelayInTakingAction;
    QVector<double> theBuildingAvailableFunds;
    QVector<double> theBuildingReceivedFundTime;

    QVector<double> theBuildingExpectedFunds;
    QVector<double> theBuildingAmountFromInsurance;
    QVector<double> theBuildingAmountLendedFromSBA;
    QVector<double> theBuildingAmountLendedFromBank;
    QVector<double> theBuildingAmountGrantedByCDBGDR;
    QVector<double> theBuildingAmountMissing;

    QVector<double> theLowIncomeBuildingReceivedFundTime;
    QVector<double> theModerateIncomeBuildingReceivedFundTime;
    QVector<double> theHighIncomeBuildingReceivedFundTime;
    QVector<double> theBuildingTimeUnderRepairs;
    QVector<double> theAvailableMaterials;
    QVector<double> theImmediateEconomicLoss;
    QVector<double> theImmediateContentLoss;
    QVector<double> theBuildingRepairTime;
    QVector<double> theBuildingReoccupancyTime;
    QVector<double> theHomeownerIncomeValue;


    // Households
    int theNumberOfPeopleNotDisplaced;
    int theNumberOfPeopleSeekingShelter;
    int theNumberOfPeopleInHotels;
    int theNumberOfPersonsGone;
    int theNumberOfPeopleLivingWithTheirFamilies;
    int theNumberOfPersons;
    int theNumberOfPersonsEvicted;
    int theNumberOfPersonsLeavingVoluntarily;
    double theSnowballFactor;
    double theAverageDamageState;
    double theOccupancyRate;
    QString theWeatherConditions;
    QVector<QVector<int>> theHouseholdSize;
    QVector<QVector<bool>> theHouseholdMortgage;
    QVector<QVector<bool>> theHouseholdCarOwnership;
    QVector<QVector<bool>> theHouseholdDisabilityStatus;
    QVector<QVector<bool>> theHouseholdHigherEducationStatus;
    QVector<QVector<bool>> theHouseholdRecentMoverStatus;
    QVector<QVector<bool>> theHouseholdFiancialStrain;
    QVector<QVector<bool>> theAvailabilityOfGenerator;
    QVector<QVector<bool>> theAvailabilityOfAlternativeWaterSource;
    QVector<QVector<bool>> theHouseholdUnsafeHousingVector;
    QVector<QVector<bool>> theHouseholdUnaffordableHousingVector;
    QVector<QVector<bool>> theHouseholdOvercrowdedHousingVector;
    QVector<QVector<bool>> theHouseholdUnsatisfiedNeighborhood;
    QVector<QVector<bool>> theHouseholdFatalityIncidence;
    QVector<QVector<QString>> theDisasterDestination;
    QVector<QVector<QString>> theHouseholdMaritalStatus;
    QVector<QVector<QString>> theHouseholdIncome;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHouseholdRace;
    QVector<QVector<QString>> theHouseholdTenureStatus;
    QVector<QVector<QString>> theHouseholdImmigrantStatus;
    QVector<QVector<QString>> theHouseholdDestination;

    // Requests
    int numberOfWoodRequestsCreatedHere;
    int numberOfWorkforceRequestsCreatedHere;
    int numberOfConcreteRequestsCreatedHere;
    int numberOfMoneyRequestsCreatedHere;
    int numberOfInspectionRequestsCreatedHere;
    int numberOfEvaluationRequestsCreatedHere;
    int numberOfPermitRequestsCreatedHere;

    RFinancingRequest *theInsuranceRequest;
    RFinancingRequest *thePublicLoanRequest;
    RFinancingRequest *thePrivateLoanRequest;
    RFinancingRequest *theGrantRequest;
    RRequest *theWoodRequest;
    RRequest *theConcreteRequest;
    RContractorNegotiationRequest *theContractorNegotiationRequest;
    REngineeringAssessmentRequest *theEngineeringAssessmentRequest;
    RPermitRequest *theBuildingPermitStatusRequest;
    RSingleFamilyContractorRequest *theSFContractorAllocationRequest;
    RMultiFamilyContractorRequest *theMFContractorAllocationRequest;
    QVector<bool> alreadyRequestedFinancing;
    QVector<bool> alreadyRequestedEngineers;
    QVector<bool> alreadyRequestedMaterials;
    QVector<bool> alreadyRequestedNegotiation;
    QVector<bool> alreadyRequestedAllocation;


    // Outputs - maybe remove after papers
    int theNumberOfDamageLowIncomeSFBuildings;
    int theNumberOfDamageModerateIncomeSFBuildings;
    int theNumberOfDamageHighIncomeSFBuildings;
    int theNumberOfLowIncomeSFBuildings;
    int theNumberOfModerateIncomeSFBuildings;
    int theNumberOfHighIncomeSFBuildings;
    double totalHabitableArea;
    double theTotalTimeDisplaced;
    double totalPublicLoanRequest;
    double totalGrantMoneyRequest;
    double totalSFPersons;
    double totalMFPersons;
    double totalPreCodePersons;
    double totalLowCodePersons;
    double totalModCodePersons;
    double totalHighCodePersons;
    double totalOwnerPersons;
    double totalRenterPersons;
    double totalPoorPersons;
    double totalModeratePersons;
    double totalRichPersons;
    double totalInsuredPersons;
    double totalNotInsuredPersons;
    double totalRecentImmigrantPersons;
    double totalEstablishedImmigrantPersons;
    double totalNonImmigrantPersons;
    double totalWhitePersons;
    double totalBlackPersons;
    double totalAsianPersons;
    double totalNativeAmericanPersons;
    double totalHispanicPersons;
    double totalNonHispanicPersons;
    double totalElderlyMemberPersons;
    double totalNoElderlyMemberPersons;
    double totalChildrenMemberPersons;
    double totalNoChildrenMemberPersons;
    double totalHigherEducatedPersons;
    double totalNonHigherEducatedPersons;
    double totalMarriedCouplesPersons;
    double totalSingleParentsPersons;
    double totalRecentMoverPersons;
    double totalNotRecentMoverPersons;
    QVector<double> theRecoveryTime;
    QVector<double> theSFUninhabitableDwellingsVector;
    QVector<double> theMFUninhabitableDwellingsVector;
    QVector<double> thePreCodeUninhabitableDwellingsVector;
    QVector<double> theLowCodeUninhabitableDwellingsVector;
    QVector<double> theModCodeUninhabitableDwellingsVector;
    QVector<double> theHighCodeUninhabitableDwellingsVector;
    QVector<double> theOwnerUninhabitableDwellingsVector;
    QVector<double> theRenterUninhabitableDwellingsVector;
    QVector<double> thePoorUninhabitableDwellingsVector;
    QVector<double> theModerateUninhabitableDwellingsVector;
    QVector<double> theRichUninhabitableDwellingsVector;
    QVector<double> theInsuredUninhabitableDwellingsVector;
    QVector<double> theNotInsuredUninhabitableDwellingsVector;
    QVector<double> theRecentImmigrantUninhabitableDwellingsVector;
    QVector<double> theEstablishedImmigrantUninhabitableDwellingsVector;
    QVector<double> theNonImmigrantUninhabitableDwellingsVector;
    QVector<double> theWhiteUninhabitableDwellingsVector;
    QVector<double> theBlackUninhabitableDwellingsVector;
    QVector<double> theAsianUninhabitableDwellingsVector;
    QVector<double> theNativeAmericanUninhabitableDwellingsVector;
    QVector<double> theHispanicUninhabitableDwellingsVector;
    QVector<double> theNonHispanicUninhabitableDwellingsVector;
    QVector<double> theElderlyMemberUninhabitableDwellingsVector;
    QVector<double> theNoElderlyMemberUninhabitableDwellingsVector;
    QVector<double> theChildrenMemberUninhabitableDwellingsVector;
    QVector<double> theNoChildrenMemberUninhabitableDwellingsVector;
    QVector<double> theHigherEducatedUninhabitableDwellingsVector;
    QVector<double> theNonHigherEducatedUninhabitableDwellingsVector;
    QVector<double> theMarriedCouplesUninhabitableDwellingsVector;
    QVector<double> theSingleParentsUninhabitableDwellingsVector;
    QVector<double> theRecentMoverUninhabitableDwellingsVector;
    QVector<double> theNotRecentMoverUninhabitableDwellingsVector;

    QVector<double> theOwnerPersonsGoneVoluntarilyVector;
    QVector<double> theRenterPersonsGoneVoluntarilyVector;
    QVector<double> thePoorPersonsGoneVoluntarilyVector;
    QVector<double> theModeratePersonsGoneVoluntarilyVector;
    QVector<double> theRichPersonsGoneVoluntarilyVector;
    QVector<double> theRecentImmigrantPersonsGoneVoluntarilyVector;
    QVector<double> theHispanicPersonsGoneVoluntarilyVector;
    QVector<double> theWhitePersonsGoneVoluntarilyVector;
    QVector<double> theBlackPersonsGoneVoluntarilyVector;
    QVector<double> theAsianPersonsGoneVoluntarilyVector;
    QVector<double> theElderlyMemberPersonsGoneVoluntarilyVector;
    QVector<double> theChildrenMemberPersonsGoneVoluntarilyVector;
    QVector<double> theHigherEducatedPersonsGoneVoluntarilyVector;
    QVector<double> theSingleParentsPersonsGoneVoluntarilyVector;
    QVector<double> theDisablePersonsGoneVoluntarilyVector;

    QVector<double> theOwnerPersonsEvictedVector;
    QVector<double> theRenterPersonsEvictedVector;
    QVector<double> thePoorPersonsEvictedVector;
    QVector<double> theModeratePersonsEvictedVector;
    QVector<double> theRichPersonsEvictedVector;
    QVector<double> theRecentImmigrantPersonsEvictedVector;
    QVector<double> theHispanicPersonsEvictedVector;
    QVector<double> theWhitePersonsEvictedVector;
    QVector<double> theBlackPersonsEvictedVector;
    QVector<double> theAsianPersonsEvictedVector;
    QVector<double> theElderlyMemberPersonsEvictedVector;
    QVector<double> theChildrenMemberPersonsEvictedVector;
    QVector<double> theHigherEducatedPersonsEvictedVector;
    QVector<double> theSingleParentsPersonsEvictedVector;
    QVector<double> theDisablePersonsEvictedVector;

    QVector<double> theOwnerPersonsGoneNoFundsVector;
    QVector<double> theRenterPersonsGoneNoFundsVector;
    QVector<double> thePoorPersonsGoneNoFundsVector;
    QVector<double> theModeratePersonsGoneNoFundsVector;
    QVector<double> theRichPersonsGoneNoFundsVector;
    QVector<double> theRecentImmigrantPersonsGoneNoFundsVector;
    QVector<double> theHispanicPersonsGoneNoFundsVector;
    QVector<double> theWhitePersonsGoneNoFundsVector;
    QVector<double> theBlackPersonsGoneNoFundsVector;
    QVector<double> theAsianPersonsGoneNoFundsVector;
    QVector<double> theElderlyMemberPersonsGoneNoFundsVector;
    QVector<double> theChildrenMemberPersonsGoneNoFundsVector;
    QVector<double> theHigherEducatedPersonsGoneNoFundsVector;
    QVector<double> theSingleParentsPersonsGoneNoFundsVector;
    QVector<double> theDisablePersonsGoneNoFundsVector;

    // Residential Mobility Paper
    QVector<double> theNumberOfUninhabitableDwellingsVector;
    QVector<double> theEvictionVector;
    QVector<double> theNoFundsVector;
    QVector<double> theVoluntaryRelocationVector;

    // Chenbo's vectors
    QVector<double> theRecoveryStartTime; // check if it being cleared/zero'ed when needed
    QVector<double> theRecoveryTimeLowIncomeVector;
    QVector<double> theRecoveryTimeModerateIncomeVector;
    QVector<double> theRecoveryTimeHighIncomeVector;
    QVector<double> theRecoveryStartTimeLowIncomeVector;
    QVector<double> theRecoveryStartTimeModerateIncomeVector;
    QVector<double> theRecoveryStartTimeHighIncomeVector;

    // Intensity counters
    int theSa03Counter;
    int theSa1Counter;

    // Response
    RResponse* theResponse;
    RResponse* thePowerShortageDurationResponse;
    RResponse* theWaterShortageDurationResponse;

    // Plots
    double timeAnalysisBegan;
    double timeSinceLastPlot;
    QVector<int> theDisplacedPopulationVector;
    QVector<int> thePersonsWithFamilyVector;
    QVector<int> thePersonsInShelterVector;
    QVector<int> thePersonsInRenterHousingVector;
    QVector<int> thePersonsGoneVector;
    QVector<int> theHousingRecoveryVector;
    QVector<double> theTickVector;
    QVector<QString> theLabelVector;
    QPointer<QCustomPlot> buildingRepairsPlot;
    QPointer<QCustomPlot> displacedPopulationPlot;

    double theNumberOfHabitableDwellings;

    int plotCounter = 1;
    int theSamplingStartingPoint = -1;
};

#endif // RWorkforceStudyNeighborhoodModel_H
