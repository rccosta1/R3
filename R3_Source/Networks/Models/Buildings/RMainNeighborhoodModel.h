#ifndef RMainNeighborhoodModel_H
#define RMainNeighborhoodModel_H

#include "RNeighborhoodModel.h"

class RParameter;
class RResponse;

class RMainNeighborhoodModel : public RNeighborhoodModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *PreparednessEstimator READ getPreparednessEstimator WRITE setPreparednessEstimator)
    Q_PROPERTY(QObject *DisplacementEstimator READ getDisplacementEstimator WRITE setDisplacementEstimator)
    Q_PROPERTY(QObject *MorbidityEstimator READ getMorbidityEstimator WRITE setMorbidityEstimator)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)
    Q_PROPERTY(QString BuildingInspectorList READ getBuildingInspectorList WRITE setBuildingInspectorList)
    Q_PROPERTY(QString ContractorList READ getContractorList WRITE setContractorList)
    Q_PROPERTY(QString EngineeringFirmList READ getEngineeringFirmList WRITE setEngineeringFirmList)
    Q_PROPERTY(QString PermittingAuthorityList READ getPermittingAuthorityList WRITE setPermittingAuthorityList)
    Q_PROPERTY(QString MaterialSupplierList READ getMaterialSupplierList WRITE setMaterialSupplierList)
    Q_PROPERTY(QString FinancialResourcesSupplierList READ getFinancialResourcesSupplierList WRITE setFinancialResourcesSupplierList)

public:
    // Define the constructor and destructor
    RMainNeighborhoodModel(QObject *parent, QString name);
    ~RMainNeighborhoodModel();

    int resetTime();

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

    QString getBuildingInspectorList();
    void setBuildingInspectorList(QString value);

    QString getEngineeringFirmList();
    void setEngineeringFirmList(QString value);

    QString getPermittingAuthorityList();
    void setPermittingAuthorityList(QString value);

    QString getMaterialSupplierList();
    void setMaterialSupplierList(QString value);

    QString getFinancialResourcesSupplierList();
    void setFinancialResourcesSupplierList(QString value);

    QString getContractorList();
    void setContractorList(QString value);

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
    int getNumberOfPersonsTerminated();
    int getNumberOfPersonsLeavingVoluntarily();
    int getNumberOfPersonsInHotels();
    int getNumberOfPersonsLivingWithFamily();
    int getNumberOfPersonsInShelters();
    double getTotalTimeDisplaced();
    int getNumberOfDamageLowIncomeSFBuildings();
    int getNumberOfDamageModerateIncomeSFBuildings();
    int getNumberOfDamageHighIncomeSFBuildings();
    QVector<int> getDisplacedPopulationVector();
    QVector<int> getPopulationWithFamilyVector();
    QVector<int> getPopulationInSheltersVector();
    QVector<int> getPopulationInRentedHousingVector();
    QVector<int> getPopulationReplacedVector();
    QVector<int> getHousingRecoveryVector();
    QVector<double> getRecoveryStartTimeVector();
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

    QVector<double> getOwnerTerminationVector();
    QVector<double> getRenterTerminationVector();
    QVector<double> getLowIncomeTerminationVector();
    QVector<double> getModerateIncomeTerminationVector();
    QVector<double> getHighIncomeTerminationVector();
    QVector<double> getRecentImmigrantTerminationVector();
    QVector<double> getWhiteTerminationVector();
    QVector<double> getBlackTerminationVector();
    QVector<double> getHispanicTerminationVector();
    QVector<double> getAsianTerminationVector();
    QVector<double> getElderlyMemberTerminationVector();
    QVector<double> getChildrenMemberTerminationVector();
    QVector<double> getHigherEducatedTerminationVector();
    QVector<double> getSingleParentsTerminationVector();
    QVector<double> getDisableTerminationVector();

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

    QVector<double> getUninhabitableDwellingsVector();
    QVector<double> getVoluntaryRelocationVector();
    QVector<double> getTerminationVector();
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

    // Responses coming to this model
    RDisasterPreparednessEstimator* thePreparednessEstimator = nullptr;
    RPopulationDisplacementEstimator* theDisplacementEstimator = nullptr;
    RSutley2016MorbidityEstimator* theMorbidityEstimator = nullptr;

    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;
    QList<QPointer<RParameter>> theContractorList;
    QList<QPointer<RParameter>> theBuildingInspectorsList;
    QList<QPointer<RParameter>> theEngineeringFirmList;
    QList<QPointer<RParameter>> theBuildingPermitAuthorityList;
    QList<QPointer<RParameter>> theMaterialSupplierList;
    QList<QPointer<RParameter>> theFinancialResourcesSupplierList;

    // Utility networks
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    double theWaterShortageDuration;
    double thePowerShortageDuration;

    // Buildings
    double theTotalNumberOfRecentImmigrantDwellings;
    double theTotalNumberOfEstablishedImmigrantDwellings;
    double theTotalNumberOfCitizenDwellings;
    double theBuildingAverageRent;
    QVector<bool> theBuildingNeedForRedesign;
    QVector<bool> theBuildingInsuranceStatus;
    QVector<double> theBuildingTimeSold;
    QVector<double> theBuildingThresholdTimeToBeSold;
    QVector<double> theBuildingThresholdProbabilityToBeEvicted;
    QVector<double> theBuildingThresholdTimeToMakeADecision;
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
    QVector<double> theAvailableMaterials;
    QVector<double> theImmediateEconomicLoss;
    QVector<double> theImmediateContentLoss;
    QVector<double> theHomeownerIncomeValue;

    // Households
    int theNumberOfPeopleNotDisplaced;
    int theNumberOfPeopleSeekingShelter;
    int theNumberOfPeopleInHotels;
    int theNumberOfPersonsGone;
    int theNumberOfPeopleLivingWithTheirFamilies;
    int theNumberOfPersonsEvicted;
    int theNumberOfPersonsTerminated;
    int theNumberOfPersonsLeavingVoluntarily;
    double theSnowballFactor;
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

    // Outputs - maybe remove after papers
    int theNumberOfDamageLowIncomeSFBuildings;
    int theNumberOfDamageModerateIncomeSFBuildings;
    int theNumberOfDamageHighIncomeSFBuildings;
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

    QVector<double> theOwnerPersonsTerminatedVector;
    QVector<double> theRenterPersonsTerminatedVector;
    QVector<double> thePoorPersonsTerminatedVector;
    QVector<double> theModeratePersonsTerminatedVector;
    QVector<double> theRichPersonsTerminatedVector;
    QVector<double> theRecentImmigrantPersonsTerminatedVector;
    QVector<double> theHispanicPersonsTerminatedVector;
    QVector<double> theWhitePersonsTerminatedVector;
    QVector<double> theBlackPersonsTerminatedVector;
    QVector<double> theAsianPersonsTerminatedVector;
    QVector<double> theElderlyMemberPersonsTerminatedVector;
    QVector<double> theChildrenMemberPersonsTerminatedVector;
    QVector<double> theHigherEducatedPersonsTerminatedVector;
    QVector<double> theSingleParentsPersonsTerminatedVector;
    QVector<double> theDisablePersonsTerminatedVector;

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
    QVector<double> theTerminationVector;
    QVector<double> theEvictionVector;
    QVector<double> theNoFundsVector;
    QVector<double> theVoluntaryRelocationVector;


    // Response
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


    int plotCounter = 1;
};

#endif // RMainNeighborhoodModel_H
