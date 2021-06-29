#ifndef RResidentialMobilityNeighborhoodModel_H
#define RResidentialMobilityNeighborhoodModel_H

#include "RNeighborhoodModel.h"

class RParameter;
class RResponse;

class RResidentialMobilityNeighborhoodModel : public RNeighborhoodModel
{
    Q_OBJECT

    Q_PROPERTY(double ProbabilityOfLiquefaction READ getProbabilityOfLiquefaction WRITE setProbabilityOfLiquefaction)

public:
    // Define the constructor and destructor
    RResidentialMobilityNeighborhoodModel(QObject *parent, QString name);
    ~RResidentialMobilityNeighborhoodModel();

    int resetTime();

    double getProbabilityOfLiquefaction();
    void setProbabilityOfLiquefaction(double value);

    QVector<RLocation*> getBuildingLocations();

    void getRequests();

    // Define the method where the actual model is coded
    int evaluateModel();

    // Member functions called by the sampling model
    int getNumberOfDwellings();
    int getNumberOfHabitableDwellings();
    int getNumberOfPeople();
    void compileResults();

    QVector<double> getUninhabitableDwellingsVector();
    double getBankLoanVector();
    double getMissingFundsVector();
    double getSBALoanVector();
    double getCDBGDRGrantVector();

private:

    // MEMBER FUNCTIONS
    void plotUsableBuildings(double habitableArea);
    void plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name);
    void plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name);
    void plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name);
    void plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name);

    // DATA MEMBERS

    // Responses coming to this model
    double theProbabilityOfLiquefaction;

    // Buildings
    QVector<bool> theBuildingNeedForRedesign;
    QVector<bool> theBuildingInsuranceStatus;
    QVector<double> theBuildingTimeSold;
    QVector<double> theBuildingThresholdTimeToBeSold;
    QVector<double> theBuildingThresholdTimeToMakeADecision;
    QVector<QString> theBuildingOwnerIncome;
    QVector<QString> theBuildingOwnerImmigrationStatus;
    QVector<bool> theBiddingProcess;
    QVector<int> theRepairClass;
    QVector<int> theNumberOfWorkersAvailable;
    QVector<double> theDelayInTakingAction;
    QVector<double> theBuildingDelayToStartRepairs;
    QVector<double> theBuildingAvailableFunds;
    QVector<double> theBuildingExpectedFunds;
    QVector<double> theImmediateEconomicLoss;
    QVector<double> theImmediateContentLoss;
    QVector<double> theHomeownerIncomeValue;
    QVector<double> theBuildingAmountFromInsurance;
    QVector<double> theBuildingAmountLendedFromSBA;
    QVector<double> theBuildingAmountLendedFromBank;
    QVector<double> theBuildingAmountGrantedByCDBGDR;
    QVector<double> theBuildingAmountMissing;
    QVector<double> theTimeNeededToGetFunding;

    // Households
    int theNumberOfPersonsGone;
    int theNumberOfPeopleLivingWithTheirFamilies;
    QVector<QVector<int>> theHouseholdSize;
    QVector<QVector<bool>> theHouseholdMortgage;
    QVector<QVector<bool>> theHouseholdDisabilityStatus;
    QVector<QVector<bool>> theHouseholdHigherEducationStatus;
    QVector<QVector<bool>> theHouseholdRecentMoverStatus;
    QVector<QVector<QString>> theHouseholdMaritalStatus;
    QVector<QVector<QString>> theHouseholdIncome;
    QVector<QVector<QString>> theHouseholdAgeCategory;
    QVector<QVector<QString>> theHouseholdRace;
    QVector<QVector<QString>> theHouseholdTenureStatus;
    QVector<QVector<QString>> theHouseholdImmigrantStatus;

    // Outputs - maybe remove after papers
    double totalHabitableArea;
    double totalSFPersons;
    double totalMFPersons;
    double totalPreCodeHouseholds;
    double totalLowCodeHouseholds;
    double totalModCodeHouseholds;
    double totalHighCodeHouseholds;
    double totalOwnerHouseholds;
    double totalRenterHouseholds;
    double totalPoorHouseholds;
    double totalModerateHouseholds;
    double totalRichHouseholds;
    double totalInsuredHouseholds;
    double totalNonInsuredHouseholds;
    double totalRecentImmigrantHouseholds;
    double totalEstablishedImmigrantHouseholds;
    double totalNonImmigrantHouseholds;
    double totalWhiteHouseholds;
    double totalBlackHouseholds;
    double totalAsianHouseholds;
    double totalNativeAmericanHouseholds;
    double totalHispanicHouseholds;
    double totalNonHispanicHouseholds;
    double totalElderlyMemberHouseholds;
    double totalNoElderlyMemberHouseholds;
    double totalChildrenMemberHouseholds;
    double totalNoChildrenMemberHouseholds;
    double totalDisabledMemberHouseholds;
    double totalNoDisabledMemberHouseholds;
    double totalHigherEducatedHouseholds;
    double totalNonHigherEducatedHouseholds;
    double totalMarriedCouplesHouseholds;
    double totalSingleParentsHouseholds;
    double totalRecentMoverHouseholds;
    double totalNotRecentMoverHouseholds;
    QVector<double> theRecoveryTime;

    // Residential Mobility Paper
    QVector<double> theNumberOfUninhabitableDwellingsVector;
    double numberOfBankLoans;
    double numberOfSBALoans;
    double numberOfCDBGDRGrants;
    double numberOfHouseholdsMissingFunds;
    QVector<double> theVoluntaryRelocationVector;
    double theAmountMissingVector;
    double theBankLoansVector;
    double theCDBGDRGrantVector;
    double theSBALoanVector;

    // Plots
    double timeAnalysisBegan;
    double timeSinceLastPlot;
    QVector<int> theDisplacedPopulationVector;
    QVector<int> thePersonsGoneVector;
    QVector<int> theHousingRecoveryVector;
    QVector<double> theTickVector;
    QVector<QString> theLabelVector;
    QPointer<QCustomPlot> buildingRepairsPlot;
    QPointer<QCustomPlot> displacedPopulationPlot;

    int plotCounter = 1;
};

#endif // RResidentialMobilityNeighborhoodModel_H
