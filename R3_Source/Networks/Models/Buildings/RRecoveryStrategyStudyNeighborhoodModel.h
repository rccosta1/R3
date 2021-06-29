#ifndef RRecoveryStrategyStudyNeighborhoodModel_H
#define RRecoveryStrategyStudyNeighborhoodModel_H

#include "RModel.h"
#include "RNetworkModel.h"
#include "RLocation.h"
#include "RProbabilityDistributions.h"
#include "RHouseholdPortfolio.h"
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

class RRecoveryStrategyStudyNeighborhoodModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QObject *HouseholdPortfolio READ getHouseholdPortfolio WRITE setHouseholdPortfolio)
    Q_PROPERTY(QString BuildingInspectorList READ getBuildingInspectorList WRITE setBuildingInspectorList)
    Q_PROPERTY(QString ContractorList READ getContractorList WRITE setContractorList)
    Q_PROPERTY(QString EngineeringFirmList READ getEngineeringFirmList WRITE setEngineeringFirmList)
    Q_PROPERTY(QString PermittingAuthorityList READ getPermittingAuthorityList WRITE setPermittingAuthorityList)
    Q_PROPERTY(QString MaterialSupplierList READ getMaterialSupplierList WRITE setMaterialSupplierList)
    Q_PROPERTY(QString FinancialResourcesSupplierList READ getFinancialResourcesSupplierList WRITE setFinancialResourcesSupplierList)
    Q_PROPERTY(int SamplingStartingPoint READ getSamplingStartingPoint WRITE setSamplingStartingPoint)

public:
    // Define the constructor and destructor
    RRecoveryStrategyStudyNeighborhoodModel(QObject *parent, QString name);
    ~RRecoveryStrategyStudyNeighborhoodModel();

    int resetTime();

    QObject* getHouseholdPortfolio() const;
    void setHouseholdPortfolio(QObject *value);

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

    int getSamplingStartingPoint();
    void setSamplingStartingPoint(int value);

    QVector<RLocation*> getBuildingLocations();

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel();

    // Member functions called by the sampling model
    int getNumberOfDwellings();
    int getNumberOfHabitableDwellings();
    QVector<double> getTimeVector();
    QVector<int> getHousingRecoveryVector();

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

    QVector<double> getUninhabitableDwellingsVector();
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
    QList<QPointer<RParameter>> theContractorList;
    QList<QPointer<RParameter>> theBuildingInspectorsList;
    QList<QPointer<RParameter>> theEngineeringFirmList;
    QList<QPointer<RParameter>> theBuildingPermitAuthorityList;
    QList<QPointer<RParameter>> theMaterialSupplierList;
    QList<QPointer<RParameter>> theFinancialResourcesSupplierList;


    // Community
    double theMedianHouseholdIncome;

    // Buildings
    QVector<int> theDamagedBuildingsIndexes;

    int theNumberOfBuildings;
    int theTotalNumberOfDwellings;
    double theTotalBuildingArea;
    double theBuildingAverageRent;
    QVector<RLocation*> theBuildingLocation;
    QVector<int> theBuildingNumberOfDwellings;
    QVector<bool> theBuildingNeedForRedesign;
    QVector<bool> theBuildingInsuranceStatus;
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
    QVector<double> theBuildingTimeUnderRepairs;
    QVector<double> theAvailableMaterials;
    QVector<double> theImmediateEconomicLoss;
    QVector<double> theImmediateContentLoss;
    QVector<double> theBuildingRepairTime;
    QVector<double> theBuildingReoccupancyTime;
    QVector<double> theHomeownerIncomeValue;

    // Households
    int theNumberOfPersons;
    double theOccupancyRate;
    QVector<QVector<int>> theHouseholdSize;
    QVector<QVector<bool>> theHouseholdMortgage;
    QVector<QVector<bool>> theHouseholdCarOwnership;
    QVector<QVector<bool>> theHouseholdDisabilityStatus;
    QVector<QVector<bool>> theHouseholdHigherEducationStatus;
    QVector<QVector<bool>> theHouseholdRecentMoverStatus;
    QVector<QVector<QString>> theHouseholdMaritalStatus;
    QVector<QVector<QString>> theHouseholdIncome;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHouseholdRace;
    QVector<QVector<QString>> theHouseholdTenureStatus;
    QVector<QVector<QString>> theHouseholdImmigrantStatus;

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
    double totalHabitableArea;
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

    // Response
    RResponse* theResponse;

    // Plots
    double timeAnalysisBegan;
    double timeSinceLastPlot;
    QVector<double> timeVector;
    QVector<int> theHousingRecoveryVector;
    QVector<double> theTickVector;
    QVector<QString> theLabelVector;
    QPointer<QCustomPlot> buildingRepairsPlot;
    QPointer<QCustomPlot> displacedPopulationPlot;

    double theNumberOfHabitableDwellings;

    int plotCounter = 1;
    int theSamplingStartingPoint = -1;
};

#endif // RRecoveryStrategyStudyNeighborhoodModel_H
