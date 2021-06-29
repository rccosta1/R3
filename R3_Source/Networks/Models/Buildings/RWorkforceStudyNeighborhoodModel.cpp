#include "RWorkforceStudyNeighborhoodModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include "RAHSPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RNetworkSamplingModel.h"


#include <math.h>

RWorkforceStudyNeighborhoodModel::RWorkforceStudyNeighborhoodModel(QObject *parent, QString name)
    : RNeighborhoodModel(parent, name)
{
    repairProgressFunction = "";
    theVariableOfInterest = "";

    // Utility networks
    theAvailabilityOfWater = true;
    theAvailabilityOfPower = true;
    theWaterShortageDuration = 0.0;
    thePowerShortageDuration = 0.0;

    // Community
    theMedianHouseholdIncome = 100000.0;
    theNumberOfBuildings = 0;
    theTotalNumberOfDwellings = 0;
    theTotalBuildingArea  = 0.0;

    // Households
    theNumberOfPeopleNotDisplaced = 0;
    theNumberOfPeopleSeekingShelter = 0;
    theNumberOfPeopleInHotels = 0;
    theNumberOfPersonsGone = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    theNumberOfPersons = 0;
    theSnowballFactor = 0.0;
    theAverageDamageState = 0.0;
    theOccupancyRate = 0.0;
    theWeatherConditions = "";

    // Requests
    numberOfWoodRequestsCreatedHere = 0;
    numberOfWorkforceRequestsCreatedHere = 0;
    numberOfConcreteRequestsCreatedHere = 0;
    numberOfMoneyRequestsCreatedHere = 0;
    numberOfInspectionRequestsCreatedHere = 0;
    numberOfEvaluationRequestsCreatedHere = 0;
    numberOfPermitRequestsCreatedHere = 0;

    theInsuranceRequest = nullptr;
    thePublicLoanRequest = nullptr;
    thePrivateLoanRequest = nullptr;
    theWoodRequest = nullptr;
    theConcreteRequest = nullptr;
    theContractorNegotiationRequest = nullptr;
    theEngineeringAssessmentRequest = nullptr;
    theBuildingPermitStatusRequest = nullptr;
    theSFContractorAllocationRequest = nullptr;
    theMFContractorAllocationRequest = nullptr;

    // Outputs - maybe remove after papers
    theNumberOfHabitableDwellings = 1;
    totalHabitableArea = 0.0;
    theTotalTimeDisplaced = 0.0;
    totalPublicLoanRequest = 0.0;
    totalGrantMoneyRequest = 0.0;

    // Intensity counters
    theSa03Counter = theSamplingStartingPoint;
    theSa1Counter = theSamplingStartingPoint;

    // Response
    theResponse = nullptr;
    thePowerShortageDurationResponse = nullptr;
    theWaterShortageDurationResponse = nullptr;

    // Plots
    timeAnalysisBegan = 0.0;
    timeSinceLastPlot = 0.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "PowerResponse", "RGenericResponse");
    thePowerShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePowerShortageDurationResponse->setModel(this);
    thePowerShortageDurationResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "WaterResponse", "RGenericResponse");
    theWaterShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theWaterShortageDurationResponse->setModel(this);
    theWaterShortageDurationResponse->setCurrentValue(0.0);

    QString insuranceRequestName = QString(this->objectName() + "InsuranceRequest");
    theDomain->createObject(insuranceRequestName, "RFinancingRequest");
    theInsuranceRequest = qobject_cast<RFinancingRequest *>(theDomain->getLastAddedObject());
    theInsuranceRequest->setModel(this);
    theInsuranceRequest->setContentType(QString("InsuranceMoney"));

    QString publicLoanRequestName = QString(this->objectName() + "PublicLoanRequest");
    theDomain->createObject(publicLoanRequestName, "RFinancingRequest");
    thePublicLoanRequest = qobject_cast<RFinancingRequest *>(theDomain->getLastAddedObject());
    thePublicLoanRequest->setModel(this);
    thePublicLoanRequest->setContentType(QString("PublicMoney"));

    QString privateLoanRequestName = QString(this->objectName() + "PrivateLoanRequest");
    theDomain->createObject(privateLoanRequestName, "RFinancingRequest");
    thePrivateLoanRequest = qobject_cast<RFinancingRequest *>(theDomain->getLastAddedObject());
    thePrivateLoanRequest->setModel(this);
    thePrivateLoanRequest->setContentType(QString("PrivateMoney"));

    QString grantRequestName = QString(this->objectName() + "GrantRequest");
    theDomain->createObject(grantRequestName, "RFinancingRequest");
    theGrantRequest = qobject_cast<RFinancingRequest *>(theDomain->getLastAddedObject());
    theGrantRequest->setModel(this);
    theGrantRequest->setContentType(QString("GrantMoney"));

    QString woodRequestName = QString(this->objectName() + "WoodMaterialsRequest");
    theDomain->createObject(woodRequestName, "RRequest");
    theWoodRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
    theWoodRequest->setModel(this);
    theWoodRequest->setContentType(QString("WoodConstructionMaterials"));

    QString concreteRequestName = QString(this->objectName() + "ConcreteMaterialsRequest");
    theDomain->createObject(concreteRequestName, "RRequest");
    theConcreteRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
    theConcreteRequest->setModel(this);
    theConcreteRequest->setContentType(QString("ConcreteConstructionMaterials"));

    QString negotiationRequestName = QString(this->objectName() + "ContractorNegotiation");
    theDomain->createObject(negotiationRequestName, "RContractorNegotiationRequest");
    theContractorNegotiationRequest = qobject_cast<RContractorNegotiationRequest *>(theDomain->getLastAddedObject());
    theContractorNegotiationRequest->setModel(this);
    theContractorNegotiationRequest->setContentType(QString("Negotiation"));

    QString engineeringAssessmentRequestName = QString(this->objectName() + "AssessmentRequest");
    theDomain->createObject(engineeringAssessmentRequestName, "REngineeringAssessmentRequest");
    theEngineeringAssessmentRequest = qobject_cast<REngineeringAssessmentRequest *>(theDomain->getLastAddedObject());
    theEngineeringAssessmentRequest->setModel(this);
    theEngineeringAssessmentRequest->setContentType(QString("Assessment"));

    QString permitRequestName = QString(this->objectName() + "PermitRequest");
    theDomain->createObject(permitRequestName, "RPermitRequest");
    theBuildingPermitStatusRequest = qobject_cast<RPermitRequest *>(theDomain->getLastAddedObject());
    theBuildingPermitStatusRequest->setModel(this);
    theBuildingPermitStatusRequest->setContentType(QString("Permit"));

    QString sfAllocationRequestName = QString(this->objectName() + "SFAllocationRequest");
    theDomain->createObject(sfAllocationRequestName, "RSingleFamilyContractorRequest");
    theSFContractorAllocationRequest = qobject_cast<RSingleFamilyContractorRequest *>(theDomain->getLastAddedObject());
    theSFContractorAllocationRequest->setModel(this);
    theSFContractorAllocationRequest->setContentType(QString("Allocation"));

    QString mfAllocationRequestName = QString(this->objectName() + "MFAllocationRequest");
    theDomain->createObject(mfAllocationRequestName, "RMultiFamilyContractorRequest");
    theMFContractorAllocationRequest = qobject_cast<RMultiFamilyContractorRequest *>(theDomain->getLastAddedObject());
    theMFContractorAllocationRequest->setModel(this);
    theMFContractorAllocationRequest->setContentType(QString("Allocation"));
}

RWorkforceStudyNeighborhoodModel::~RWorkforceStudyNeighborhoodModel()
{
}


int RWorkforceStudyNeighborhoodModel::resetTime()
{
    //qCritical() << this->objectName() << "is skipping the check for water/power!!";

    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;
    thePowerShortageDuration = 0.0;
    theWaterShortageDuration = 0.0;
    theAverageDamageState = 0.0;
    theSnowballFactor = 0.0;

    // Reset some vectors
    theNumberOfPeopleSeekingShelter = 0;
    theNumberOfPeopleInHotels = 0;
    theNumberOfPersonsGone = 0;
    theNumberOfPersonsEvicted = 0;
    theNumberOfPersonsLeavingVoluntarily = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    theTotalTimeDisplaced = 0.000001;

    // -----------
    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());

    if (1 > 2 && thePGACounter > (theSa0_3Vector.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough Sa values!";
        thePGACounter = 0;
        theSa03Counter = 0;
        theSa1Counter = 0;
    }
    if (theSa03Counter < theSamplingStartingPoint){
        theSa03Counter = theSamplingStartingPoint;
        theSa1Counter = theSamplingStartingPoint;
    }

    theSa03Counter++;
    theSa1Counter++;
    // -----------

    // Weather conditions
    theWeatherConditions = QString("Good");

    double rngWeather = rand()/static_cast<double>(RAND_MAX);
    if (rngWeather < 0.5){
        theWeatherConditions = QString("Bad");
    }

    numberOfWoodRequestsCreatedHere = 0;
    numberOfConcreteRequestsCreatedHere = 0;
    numberOfWorkforceRequestsCreatedHere = 0;
    numberOfMoneyRequestsCreatedHere = 0;
    numberOfInspectionRequestsCreatedHere = 0;
    numberOfEvaluationRequestsCreatedHere = 0;
    numberOfPermitRequestsCreatedHere = 0;

    theDamagedBuildingsIndexes.clear();
    theNotableBuildingsIndexes.clear();
    theTickVector.clear();
    theLabelVector.clear();
    timeVector.clear();
    theHousingRecoveryVector.clear();
    theDisplacedPopulationVector.clear();
    theDelayInTakingAction.clear();
    theBuildingAvailableFunds.clear();
    theBuildingExpectedFunds.clear();
    theBuildingAmountFromInsurance.clear();
    theBuildingAmountLendedFromSBA.clear();
    theBuildingAmountLendedFromBank.clear();
    theBuildingAmountGrantedByCDBGDR.clear();
    theBuildingAmountMissing.clear();
    theBuildingReceivedFundTime.clear();
    theLowIncomeBuildingReceivedFundTime.clear();
    theModerateIncomeBuildingReceivedFundTime.clear();
    theHighIncomeBuildingReceivedFundTime.clear();
    theAvailableMaterials.clear();
    theNumberOfWorkersAvailable.clear();
    theImmediateEconomicLoss.clear();
    theImmediateContentLoss.clear();
    theBuildingRepairTime.clear();

    theBuildingReoccupancyTime.clear();
    theBuildingTimeSold.clear();
    theBuildingThresholdTimeToBeSold.clear();
    theBuildingThresholdProbabilityToBeEvicted.clear();
    theBuildingThresholdTimeToMakeADecision.clear();
    theBuildingTimeUnderRepairs.clear();
    theInspectionState.clear();
    theBiddingProcess.clear();
    theBuildingNeedForRedesign.clear();
    theEngineeringAssessment.clear();
    theBuildingPermitStatus.clear();
    theBuildingStrDamageState.clear();
    theBuildingNSDDamageState.clear();
    theBuildingNSADamageState.clear();
    theHouseholdFatalityIncidence.clear();
    alreadyRequestedEngineers.clear();
    alreadyRequestedMaterials.clear();
    alreadyRequestedNegotiation.clear();
    alreadyRequestedAllocation.clear();
    thePersonsWithFamilyVector.clear();
    thePersonsInShelterVector.clear();
    thePersonsInRenterHousingVector.clear();
    thePersonsGoneVector.clear();
    theBuildingStrDamageState.clear();
    theBuildingNSDDamageState.clear();
    theBuildingNSADamageState.clear();
    theHouseholdFatalityIncidence.clear();
    totalSFPersons = 0.0;
    totalMFPersons = 0.0;
    theNumberOfLowIncomeSFBuildings = 0;
    theNumberOfModerateIncomeSFBuildings = 0;
    theNumberOfHighIncomeSFBuildings = 0;
    theNumberOfDamageLowIncomeSFBuildings = 0;
    theNumberOfDamageModerateIncomeSFBuildings = 0;
    theNumberOfDamageHighIncomeSFBuildings = 0;
    totalPreCodePersons = 0.0;
    totalLowCodePersons = 0.0;
    totalModCodePersons = 0.0;
    totalHighCodePersons = 0.0;
    totalOwnerPersons = 0.0;
    totalRenterPersons = 0.0;
    totalPoorPersons = 0.0;
    totalModeratePersons = 0.0;
    totalRichPersons = 0.0;
    totalInsuredPersons = 0.0;
    totalNotInsuredPersons = 0.0;
    totalRecentImmigrantPersons = 0.0;
    totalEstablishedImmigrantPersons = 0.0;
    totalNonImmigrantPersons = 0.0;
    totalWhitePersons = 0.0;
    totalBlackPersons = 0.0;
    totalAsianPersons = 0.0;
    totalNativeAmericanPersons = 0.0;
    totalHispanicPersons = 0.0;
    totalNonHispanicPersons = 0.0;
    totalElderlyMemberPersons = 0.0;
    totalNoElderlyMemberPersons = 0.0;
    totalChildrenMemberPersons = 0.0;
    totalNoChildrenMemberPersons = 0.0;
    totalHigherEducatedPersons = 0.0;
    totalNonHigherEducatedPersons = 0.0;
    totalMarriedCouplesPersons = 0.0;
    totalSingleParentsPersons = 0.0;
    totalRecentMoverPersons = 0.0;
    totalNotRecentMoverPersons = 0.0;
    theSFUninhabitableDwellingsVector.clear();
    theMFUninhabitableDwellingsVector.clear();
    thePreCodeUninhabitableDwellingsVector.clear();
    theLowCodeUninhabitableDwellingsVector.clear();
    theModCodeUninhabitableDwellingsVector.clear();
    theHighCodeUninhabitableDwellingsVector.clear();
    thePoorUninhabitableDwellingsVector.clear();
    theModerateUninhabitableDwellingsVector.clear();
    theRichUninhabitableDwellingsVector.clear();
    theOwnerUninhabitableDwellingsVector.clear();
    theRenterUninhabitableDwellingsVector.clear();
    theInsuredUninhabitableDwellingsVector.clear();
    theNotInsuredUninhabitableDwellingsVector.clear();
    theRecentImmigrantUninhabitableDwellingsVector.clear();
    theEstablishedImmigrantUninhabitableDwellingsVector.clear();
    theNonImmigrantUninhabitableDwellingsVector.clear();
    theWhiteUninhabitableDwellingsVector.clear();
    theBlackUninhabitableDwellingsVector.clear();
    theAsianUninhabitableDwellingsVector.clear();
    theNativeAmericanUninhabitableDwellingsVector.clear();
    theHispanicUninhabitableDwellingsVector.clear();
    theNonHispanicUninhabitableDwellingsVector.clear();
    theElderlyMemberUninhabitableDwellingsVector.clear();
    theNoElderlyMemberUninhabitableDwellingsVector.clear();
    theChildrenMemberUninhabitableDwellingsVector.clear();
    theNoChildrenMemberUninhabitableDwellingsVector.clear();
    theHigherEducatedUninhabitableDwellingsVector.clear();
    theNonHigherEducatedUninhabitableDwellingsVector.clear();
    theMarriedCouplesUninhabitableDwellingsVector.clear();
    theSingleParentsUninhabitableDwellingsVector.clear();
    theRecentMoverUninhabitableDwellingsVector.clear();
    theNotRecentMoverUninhabitableDwellingsVector.clear();


    theSFContractorAllocationRequest->clearOccupancyClassList();
    theSFContractorAllocationRequest->clearRepairClassList();
    theSFContractorAllocationRequest->clearRequesterIDList();
    theSFContractorAllocationRequest->clearRequestedAmountList();


    // --------
    // Chenbo's vectors
    theRecoveryTimeLowIncomeVector.clear();
    theRecoveryTimeModerateIncomeVector.clear();
    theRecoveryTimeHighIncomeVector.clear();
    theRecoveryStartTimeLowIncomeVector.clear();
    theRecoveryStartTimeModerateIncomeVector.clear();
    theRecoveryStartTimeHighIncomeVector.clear();

    theRecoveryTime.resize(theNumberOfBuildings);
    theRecoveryTime.fill(999999.0);

    theRecoveryStartTime.resize(theNumberOfBuildings);
    theRecoveryStartTime.fill(999999.0);

    theRecoveryTimeLowIncomeVector.resize(theNumberOfBuildings);
    theRecoveryTimeLowIncomeVector.fill(999999.0);

    theRecoveryTimeModerateIncomeVector.resize(theNumberOfBuildings);
    theRecoveryTimeModerateIncomeVector.fill(999999.0);

    theRecoveryTimeHighIncomeVector.resize(theNumberOfBuildings);
    theRecoveryTimeHighIncomeVector.fill(999999.0);

    theRecoveryStartTimeLowIncomeVector.resize(theNumberOfBuildings);
    theRecoveryStartTimeLowIncomeVector.fill(999999.0);

    theRecoveryStartTimeModerateIncomeVector.resize(theNumberOfBuildings);
    theRecoveryStartTimeModerateIncomeVector.fill(999999.0);

    theRecoveryStartTimeHighIncomeVector.resize(theNumberOfBuildings);
    theRecoveryStartTimeHighIncomeVector.fill(999999.0);

    // --------
    theNumberOfUninhabitableDwellingsVector.clear();
    theEvictionVector.clear();
    theNoFundsVector.clear();
    theVoluntaryRelocationVector.clear();

    theBuildingStrDamageState.resize(theNumberOfBuildings);
    theBuildingStrDamageState.fill(0);

    theBuildingNSDDamageState.resize(theNumberOfBuildings);
    theBuildingNSDDamageState.fill(0);

    theBuildingNSADamageState.resize(theNumberOfBuildings);
    theBuildingNSADamageState.fill(0);

    theImmediateEconomicLoss.resize(theNumberOfBuildings);
    theImmediateEconomicLoss.fill(0.0);

    theImmediateContentLoss.resize(theNumberOfBuildings);
    theImmediateContentLoss.fill(0.0);

    theBuildingRepairTime.resize(theNumberOfBuildings);
    theBuildingRepairTime.fill(0.0);

    theBuildingReoccupancyTime.resize(theNumberOfBuildings);
    theBuildingReoccupancyTime.fill(0.0);

    theBuildingTimeSold.resize(theNumberOfBuildings);
    theBuildingTimeSold.fill(100000000.0);

    theBuildingThresholdTimeToBeSold.resize(theNumberOfBuildings);
    theBuildingThresholdTimeToBeSold.fill(0.0);

    theBuildingThresholdProbabilityToBeEvicted.resize(theNumberOfBuildings);
    theBuildingThresholdProbabilityToBeEvicted.fill(0.0);

    theBuildingThresholdTimeToMakeADecision.resize(theNumberOfBuildings);
    theBuildingThresholdTimeToMakeADecision.fill(0.0);

    theDelayInTakingAction.resize(theNumberOfBuildings);
    theDelayInTakingAction.fill(0.0);

    theBuildingAvailableFunds.resize(theNumberOfBuildings);
    theBuildingAvailableFunds.fill(0.0);

    theBuildingExpectedFunds.resize(theNumberOfBuildings);
    theBuildingExpectedFunds.fill(0.0);

    theBuildingAmountLendedFromSBA.resize(theNumberOfBuildings);
    theBuildingAmountLendedFromSBA.fill(0.0);

    theBuildingAmountGrantedByCDBGDR.resize(theNumberOfBuildings);
    theBuildingAmountGrantedByCDBGDR.fill(0.0);

    theBuildingAmountFromInsurance.resize(theNumberOfBuildings);
    theBuildingAmountFromInsurance.fill(0.0);

    theBuildingAmountLendedFromBank.resize(theNumberOfBuildings);
    theBuildingAmountLendedFromBank.fill(0.0);

    theBuildingAmountMissing.resize(theNumberOfBuildings);
    theBuildingAmountMissing.fill(0.0);

    theBuildingReceivedFundTime.resize(theNumberOfBuildings);
    theBuildingReceivedFundTime.fill(0.0);

    theLowIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
    theLowIncomeBuildingReceivedFundTime.fill(99999999.0);

    theModerateIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
    theModerateIncomeBuildingReceivedFundTime.fill(99999999.0);

    theHighIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
    theHighIncomeBuildingReceivedFundTime.fill(99999999.0);

    theAvailableMaterials.resize(theNumberOfBuildings);
    theAvailableMaterials.fill(0.0);

    theNumberOfWorkersAvailable.resize(theNumberOfBuildings);
    theNumberOfWorkersAvailable.fill(0.0);

    theBuildingTimeUnderRepairs.resize(theNumberOfBuildings);
    theBuildingTimeUnderRepairs.fill(0.0);

    theInspectionState.resize(theNumberOfBuildings);
    theInspectionState.fill(false);

    theBiddingProcess.resize(theNumberOfBuildings);
    theBiddingProcess.fill(false);

    theBuildingNeedForRedesign.resize(theNumberOfBuildings);
    theBuildingNeedForRedesign.fill(false);

    theEngineeringAssessment.resize(theNumberOfBuildings);
    theEngineeringAssessment.fill(false);

    theBuildingPermitStatus.resize(theNumberOfBuildings);
    theBuildingPermitStatus.fill(false);

    theRepairClass.resize(theNumberOfBuildings);
    theRepairClass.fill(0);

    alreadyRequestedFinancing.resize(theNumberOfBuildings);
    alreadyRequestedFinancing.fill(false);

    alreadyRequestedEngineers.resize(theNumberOfBuildings);
    alreadyRequestedEngineers.fill(false);

    alreadyRequestedMaterials.resize(theNumberOfBuildings);
    alreadyRequestedMaterials.fill(false);

    alreadyRequestedNegotiation.resize(theNumberOfBuildings);
    alreadyRequestedNegotiation.fill(false);

    alreadyRequestedAllocation.resize(theNumberOfBuildings);
    alreadyRequestedAllocation.fill(false);

    theTickVector << 0;
    theLabelVector << QString("%1").arg(0);

    return 0;
}


QObject * RWorkforceStudyNeighborhoodModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RWorkforceStudyNeighborhoodModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}


QObject* RWorkforceStudyNeighborhoodModel::getSa()
{
    return theIntensityModelResponse;
}

void RWorkforceStudyNeighborhoodModel::setSa(QObject *value)
{
    theIntensityModelResponse =  qobject_cast<RResponse*>(value);
}


QObject* RWorkforceStudyNeighborhoodModel::getBuildingDamageModel()
{
    return theBuildingDamageModelResponse;
}

void RWorkforceStudyNeighborhoodModel::setBuildingDamageModel(QObject* value)
{
    theBuildingDamageModelResponse = qobject_cast<RResponse*>(value);
}

QObject* RWorkforceStudyNeighborhoodModel::getPreparednessEstimator()
{
    return thePreparednessEstimator;
}

void RWorkforceStudyNeighborhoodModel::setPreparednessEstimator(QObject *value)
{
    thePreparednessEstimator = qobject_cast<RDisasterPreparednessEstimator*>(value);
}

QObject* RWorkforceStudyNeighborhoodModel::getDisplacementEstimator()
{
    return theDisplacementEstimator;
}

void RWorkforceStudyNeighborhoodModel::setDisplacementEstimator(QObject *value)
{
    theDisplacementEstimator = qobject_cast<RPopulationDisplacementEstimator*>(value);
}

QObject* RWorkforceStudyNeighborhoodModel::getMorbidityEstimator()
{
    return theMorbidityEstimator;
}

void RWorkforceStudyNeighborhoodModel::setMorbidityEstimator(QObject *value)
{
    theMorbidityEstimator = qobject_cast<RSutley2016MorbidityEstimator*>(value);
}

QString RWorkforceStudyNeighborhoodModel::getContractorList()
{
    return parameterListToString(theContractorList);
}

void RWorkforceStudyNeighborhoodModel::setContractorList(QString value)
{
    theContractorList = stringToParameterList(value);
}

QString RWorkforceStudyNeighborhoodModel::getFinancialResourcesSupplierList()
{
    return parameterListToString(theFinancialResourcesSupplierList);
}

void RWorkforceStudyNeighborhoodModel::setFinancialResourcesSupplierList(QString value)
{
    theFinancialResourcesSupplierList = stringToParameterList(value);
}

double RWorkforceStudyNeighborhoodModel::getProbabilityOfLiquefaction()
{
    return theProbabilityOfLiquefaction;
}

void RWorkforceStudyNeighborhoodModel::setProbabilityOfLiquefaction(double value)
{
    theProbabilityOfLiquefaction = value;
}

int RWorkforceStudyNeighborhoodModel::getSamplingStartingPoint()
{
    return theSamplingStartingPoint;
}

void RWorkforceStudyNeighborhoodModel::setSamplingStartingPoint(int value)
{
    theSamplingStartingPoint = value;
}


QVector<double> RWorkforceStudyNeighborhoodModel::getTimeVector()
{
    return timeVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getHousingRecoveryVector()
{
    return theHousingRecoveryVector;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfDamageLowIncomeSFBuildings(){
    return theNumberOfDamageLowIncomeSFBuildings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfDamageModerateIncomeSFBuildings(){
    return theNumberOfDamageModerateIncomeSFBuildings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfDamageHighIncomeSFBuildings(){
    return theNumberOfDamageHighIncomeSFBuildings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfLowIncomeSFBuildings(){
    return theNumberOfLowIncomeSFBuildings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfModerateIncomeSFBuildings(){
    return theNumberOfModerateIncomeSFBuildings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfHighIncomeSFBuildings(){
    return theNumberOfHighIncomeSFBuildings;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getReceivedFundTimeVector()
{
    return theBuildingReceivedFundTime;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeReceivedFundTimeVector()
{
    return theLowIncomeBuildingReceivedFundTime;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeReceivedFundTimeVector()
{
    return theModerateIncomeBuildingReceivedFundTime;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeReceivedFundTimeVector()
{
    return theHighIncomeBuildingReceivedFundTime;
}


QVector<double> RWorkforceStudyNeighborhoodModel::getRecoveryStartTimeVector()
{
    return theRecoveryStartTime;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRecoveryTimeVector()
{
    return theRecoveryTime;
}


QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeRecoveryStartTimeVector()
{
    return theRecoveryStartTimeLowIncomeVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeRecoveryStartTimeVector()
{
    return theRecoveryStartTimeModerateIncomeVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeRecoveryStartTimeVector()
{
    return theRecoveryStartTimeHighIncomeVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeRecoveryTimeVector()
{
    return theRecoveryTimeLowIncomeVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeRecoveryTimeVector()
{
    return theRecoveryTimeModerateIncomeVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeRecoveryTimeVector()
{
    return theRecoveryTimeHighIncomeVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getDisplacedPopulationVector()
{
    return theDisplacedPopulationVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getPopulationWithFamilyVector()
{
    return thePersonsWithFamilyVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getPopulationInSheltersVector()
{
    return thePersonsInShelterVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getPopulationInRentedHousingVector()
{
    return thePersonsInRenterHousingVector;
}

QVector<int> RWorkforceStudyNeighborhoodModel::getPopulationReplacedVector()
{
    return thePersonsGoneVector;
}

// Voluntary relocation
// *************
QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeVoluntaryMobilityVector()
{
    return thePoorPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeVoluntaryMobilityVector()
{
    return theModeratePersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeVoluntaryMobilityVector()
{
    return theRichPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getOwnerVoluntaryMobilityVector()
{
    return theOwnerPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRenterVoluntaryMobilityVector()
{
    return theRenterPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRecentImmigrantVoluntaryMobilityVector()
{
    return theRecentImmigrantPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getWhiteVoluntaryMobilityVector()
{
    return theWhitePersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getBlackVoluntaryMobilityVector()
{
    return theBlackPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHispanicVoluntaryMobilityVector()
{
    return theHispanicPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getAsianVoluntaryMobilityVector()
{
    return theAsianPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getElderlyMemberVoluntaryMobilityVector()
{
    return theElderlyMemberPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getChildrenMemberVoluntaryMobilityVector()
{
    return theChildrenMemberPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHigherEducatedVoluntaryMobilityVector()
{
    return theHigherEducatedPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getSingleParentsVoluntaryMobilityVector()
{
    return theSingleParentsPersonsGoneVoluntarilyVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getDisableVoluntaryMobilityVector()
{
    return theDisablePersonsGoneVoluntarilyVector;
}
// *******

// Evictions
// *************
QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeEvictionVector()
{
    return thePoorPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeEvictionVector()
{
    return theModeratePersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeEvictionVector()
{
    return theRichPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getOwnerEvictionVector()
{
    return theOwnerPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRenterEvictionVector()
{
    return theRenterPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRecentImmigrantEvictionVector()
{
    return theRecentImmigrantPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getWhiteEvictionVector()
{
    return theWhitePersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getBlackEvictionVector()
{
    return theBlackPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHispanicEvictionVector()
{
    return theHispanicPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getAsianEvictionVector()
{
    return theAsianPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getElderlyMemberEvictionVector()
{
    return theElderlyMemberPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getChildrenMemberEvictionVector()
{
    return theChildrenMemberPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHigherEducatedEvictionVector()
{
    return theHigherEducatedPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getSingleParentsEvictionVector()
{
    return theSingleParentsPersonsEvictedVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getDisableEvictionVector()
{
    return theDisablePersonsEvictedVector;
}
// *******


// Evictions
// *************
QVector<double> RWorkforceStudyNeighborhoodModel::getLowIncomeNoFundsVector()
{
    return thePoorPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getModerateIncomeNoFundsVector()
{
    return theModeratePersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHighIncomeNoFundsVector()
{
    return theRichPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getOwnerNoFundsVector()
{
    return theOwnerPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRenterNoFundsVector()
{
    return theRenterPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getRecentImmigrantNoFundsVector()
{
    return theRecentImmigrantPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getWhiteNoFundsVector()
{
    return theWhitePersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getBlackNoFundsVector()
{
    return theBlackPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHispanicNoFundsVector()
{
    return theHispanicPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getAsianNoFundsVector()
{
    return theAsianPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getElderlyMemberNoFundsVector()
{
    return theElderlyMemberPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getChildrenMemberNoFundsVector()
{
    return theChildrenMemberPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getHigherEducatedNoFundsVector()
{
    return theHigherEducatedPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getSingleParentsNoFundsVector()
{
    return theSingleParentsPersonsGoneNoFundsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getDisableNoFundsVector()
{
    return theDisablePersonsGoneNoFundsVector;
}
// *******

void RWorkforceStudyNeighborhoodModel::compileResults()
{
    QVector<double> thePGAVector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
    double PGA = thePGAVector.at(theSa1Counter);

    QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
    double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

    QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getPGAIntensity());
    double Sa1 = theSa1Vector.at(theSa1Counter);


    QString filename = QString("Sample_") + QString("%1").arg(theSa1Counter) + QString(".txt");
    QFile file(filename);
    file.setFileName(filename);

    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);
        for (int i = 0; i < theNumberOfBuildings; i++){

            stream << this->objectName() << "," << theBuildingBlockGroup.at(i) << "," << PGA << "," << Sa0_3 << "," << Sa1 << "," <<
                      theBuildingOwnerIncome.at(i) << "," << theHomeownerIncomeValue.at(i) << "," << theHouseholdIncome.at(i).at(0) << "," << theHouseholdTenureStatus.at(i).at(0) << "," <<
                      theImmediateEconomicLoss.at(i) << "," <<
                      theBuildingStrDamageState.at(i) <<  "," <<     theBuildingRepairTime.at(i) << "," << theRecoveryStartTime.at(i) << "," << theBuildingReceivedFundTime.at(i);
            stream << endl;
        }
    }
    file.close();
}

QVector<double> RWorkforceStudyNeighborhoodModel::getUninhabitableDwellingsVector()
{
    return theNumberOfUninhabitableDwellingsVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getVoluntaryRelocationVector()
{
    return theVoluntaryRelocationVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getEvictionVector()
{
    return theEvictionVector;
}

QVector<double> RWorkforceStudyNeighborhoodModel::getNoFundsVector()
{
    return theNoFundsVector;
}

// ---

int RWorkforceStudyNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPeople()
{
    return theNumberOfPersons;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfDisplacedPersons()
{
    return theNumberOfPersons - theNumberOfPeopleNotDisplaced;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPermanentlyDisplaced()
{
    return theNumberOfPersonsGone;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPersonsEvicted()
{
    return theNumberOfPersonsEvicted;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPersonsLeavingVoluntarily()
{
    return theNumberOfPersonsLeavingVoluntarily;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPersonsInHotels()
{
    return theNumberOfPeopleInHotels;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPersonsLivingWithFamily()
{
    return theNumberOfPeopleLivingWithTheirFamilies;
}

int RWorkforceStudyNeighborhoodModel::getNumberOfPersonsInShelters()
{
    return theNumberOfPeopleSeekingShelter;
}

double RWorkforceStudyNeighborhoodModel::getTotalTimeDisplaced()
{
    return theTotalTimeDisplaced;
}

void RWorkforceStudyNeighborhoodModel::getRequests()
{

}

QVector<RLocation*> RWorkforceStudyNeighborhoodModel::getBuildingLocations()
{
    if (theBuildingLocation.size() == 0){
        theBuildingLocation = theHouseholdPortfolio->getBuildingLocationVector();
    }
    return theBuildingLocation;
}


int RWorkforceStudyNeighborhoodModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    RNormalDistribution theNormalDistribution(theDomain);

    // "Functionality vs time" plot
    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
        timeAnalysisBegan = theCurrentTime->getCurrentValue();
    }

    // --------------------------------------------------------------------------------------
    // 1 - Define the building and household archetypes
    //     At this step all characteristics of buildings and households are defined
    // --------------------------------------------------------------------------------------
    if (theNumberOfBuildings < 1){

        theMedianHouseholdIncome          = theHouseholdPortfolio->getMedianHouseholdIncome();

        // Tell the Census Information to define the buildings
        //theHouseholdPortfolio->instantiateHouseholds();
        theHouseholdPortfolio->getHouseholdsFromFile(this->objectName());

        theBuildingBlockGroup             = theHouseholdPortfolio->getBuildingBlockGroup();

        // Get all information about the buildings
        theNumberOfBuildings              = theHouseholdPortfolio->getNumberOfBuildings();
        theBuildingType                   = theHouseholdPortfolio->getBuildingTypesVector();
        theBuildingCodeLevel              = theHouseholdPortfolio->getBuildingCodeLevelVector();
        theBuildingOccupancyClass         = theHouseholdPortfolio->getBuildingOccupancyClassVector();
        theBuildingArea                   = theHouseholdPortfolio->getBuildingAreaVector();
        theBuildingLandValue              = theHouseholdPortfolio->getLandValueVector();
        theBuildingValue                  = theHouseholdPortfolio->getBuildingValueVector();
        theBuildingNumberOfStoreys        = theHouseholdPortfolio->getBuildingStoreysVector();
        theBuildingNumberOfDwellings      = theHouseholdPortfolio->getBuildingDwellingsVector();
        theBuildingLocation               = theHouseholdPortfolio->getBuildingLocationVector();
        theBuildingOwnerIncome            = theHouseholdPortfolio->getBuildingOwnerIncomeVector();
        theBuildingOwnerImmigrationStatus = theHouseholdPortfolio->getBuildingOwnerImmigrantStatusVector();
        theBuildingInsuranceStatus        = theHouseholdPortfolio->getBuildingInsuranceStatus();
        theBuildingAverageRent            = theHouseholdPortfolio->getAverageRent();

        // Get all information about the households
        theHomeownerIncomeValue           = theHouseholdPortfolio->getHomeownerIncomeValueVector();
        theHouseholdIncome                = theHouseholdPortfolio->getHouseholdIncomeVector();
        theHouseholdMortgage              = theHouseholdPortfolio->getMortgageVector();
        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();
        theHouseholdTenureStatus          = theHouseholdPortfolio->getHouseholdTenureVector();
        theHouseholdCarOwnership          = theHouseholdPortfolio->getHouseholdCarOwnershipVector();
        theHouseholdAgeCategory           = theHouseholdPortfolio->getHouseholdAgeCategoryVector();
        theHouseholdImmigrantStatus       = theHouseholdPortfolio->getHouseholdImmigrantStatusVector();
        theHouseholdUnsafeHousingVector   = theHouseholdPortfolio->getHouseholdUnsafeHousingVector();
        theHouseholdUnaffordableHousingVector = theHouseholdPortfolio->getHouseholdUnaffordableHousingVector();
        theHouseholdOvercrowdedHousingVector = theHouseholdPortfolio->getHouseholdOvercrowdedHousingVecto();
        theHouseholdUnsatisfiedNeighborhood = theHouseholdPortfolio->getHouseholdUnsatisfiedNeighborhood();
        theHouseholdRace                  = theHouseholdPortfolio->getHouseholdRaceVector();
        theHouseholdDisabilityStatus      = theHouseholdPortfolio->getHouseholdDisabilityStatus();
        theHouseholdHigherEducationStatus = theHouseholdPortfolio->getHouseholdEducationStatus();
        theHouseholdMaritalStatus         = theHouseholdPortfolio->getHouseholdMaritalStatus();
        theHouseholdRecentMoverStatus     = theHouseholdPortfolio->getHouseholdRecentMoverStatus();
        theHouseholdFiancialStrain        = theHouseholdPortfolio->getHouseholdFinancialStrain();

        // initalize all these vectors
        theBuildingStrDamageState.resize(theNumberOfBuildings);
        theBuildingStrDamageState.fill(0);

        theBuildingNSDDamageState.resize(theNumberOfBuildings);
        theBuildingNSDDamageState.fill(0);

        theBuildingNSADamageState.resize(theNumberOfBuildings);
        theBuildingNSADamageState.fill(0);

        theTotalNumberOfDwellings = 0.0;
        theTotalNumberOfRecentImmigrantDwellings = 0.0;
        theTotalNumberOfEstablishedImmigrantDwellings = 0.0;
        theTotalNumberOfCitizenDwellings = 0.0;
        theNumberOfPersons = 0;

        // Start some vectors with placeholder values
        for (int i = 0; i < theNumberOfBuildings; i++){
            theTotalNumberOfDwellings += theBuildingNumberOfDwellings.at(i);
            QVector<QString> auxDestination;

            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
                theNumberOfPersons += theHouseholdSize.at(i).at(j);
                auxDestination << QString("NotDisplaced");
            }

            theHouseholdDestination << auxDestination;
            //            qCritical()<<i<<" auxDestination: "<<auxDestination;
        }

        // Use the household demographics to estimate its disaster preparedness
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<bool> auxAvailabilityOfGenerator;
            QVector<bool> auxAvailabilityOfWaterSource;
            QVector<QString> disasterDestination;

            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){

                // Check where these families would go if displaced according to AHS model
                thePreparednessEstimator->setTenure(theHouseholdTenureStatus.at(i).at(j));
                thePreparednessEstimator->setIncome(theHouseholdIncome.at(i).at(j));
                thePreparednessEstimator->setImmigrationStatus(theHouseholdImmigrantStatus.at(i).at(j));
                thePreparednessEstimator->setSize(theHouseholdSize.at(i).at(j));
                thePreparednessEstimator->setChildren(false);
                thePreparednessEstimator->setElders(false);
                thePreparednessEstimator->setHispanicFlag(false);
                thePreparednessEstimator->setHigherEducationFlag(theHouseholdHigherEducationStatus.at(i).at(j));

                if (theHouseholdRace.at(i).at(j).contains(QString("Hispanic"))){
                    thePreparednessEstimator->setHispanicFlag(true);
                }

                if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                    thePreparednessEstimator->setChildren(true);
                }
                if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                    thePreparednessEstimator->setElders(true);
                }

                // Alternative power source
                auxAvailabilityOfGenerator << thePreparednessEstimator->getGeneratorAvailability();

                // Alternative water source
                auxAvailabilityOfWaterSource << thePreparednessEstimator->getWaterAvailability();

                // Hotel
                disasterDestination << thePreparednessEstimator->getDisasterDestination();

            } //j

            theAvailabilityOfGenerator << auxAvailabilityOfGenerator;
            theAvailabilityOfAlternativeWaterSource << auxAvailabilityOfWaterSource;
            theDisasterDestination << disasterDestination;
        } // i

        // initalize all these vectors
        theImmediateEconomicLoss.resize(theNumberOfBuildings);
        theImmediateEconomicLoss.fill(0.0);

        theImmediateContentLoss.resize(theNumberOfBuildings);
        theImmediateContentLoss.fill(0.0);

        theBuildingRepairTime.resize(theNumberOfBuildings);
        theBuildingRepairTime.fill(0.0);

        theBuildingReoccupancyTime.resize(theNumberOfBuildings);
        theBuildingReoccupancyTime.fill(0.0);

        theBuildingTimeSold.resize(theNumberOfBuildings);
        theBuildingTimeSold.fill(100000000.0);

        theBuildingThresholdTimeToBeSold.resize(theNumberOfBuildings);
        theBuildingThresholdTimeToBeSold.fill(0.0);

        theBuildingThresholdProbabilityToBeEvicted.resize(theNumberOfBuildings);
        theBuildingThresholdProbabilityToBeEvicted.fill(0.0);

        theBuildingThresholdTimeToMakeADecision.resize(theNumberOfBuildings);
        theBuildingThresholdTimeToMakeADecision.fill(0.0);

        theDelayInTakingAction.resize(theNumberOfBuildings);
        theDelayInTakingAction.fill(0.0);

        theBuildingAvailableFunds.resize(theNumberOfBuildings);
        theBuildingAvailableFunds.fill(0.0);

        theBuildingExpectedFunds.resize(theNumberOfBuildings);
        theBuildingExpectedFunds.fill(0.0);

        theBuildingAmountLendedFromSBA.resize(theNumberOfBuildings);
        theBuildingAmountLendedFromSBA.fill(0.0);

        theBuildingAmountGrantedByCDBGDR.resize(theNumberOfBuildings);
        theBuildingAmountGrantedByCDBGDR.fill(0.0);

        theBuildingAmountFromInsurance.resize(theNumberOfBuildings);
        theBuildingAmountFromInsurance.fill(0.0);

        theBuildingAmountLendedFromBank.resize(theNumberOfBuildings);
        theBuildingAmountLendedFromBank.fill(0.0);

        theBuildingAmountMissing.resize(theNumberOfBuildings);
        theBuildingAmountMissing.fill(0.0);

        theBuildingReceivedFundTime.resize(theNumberOfBuildings);
        theBuildingReceivedFundTime.fill(0.0);

        theLowIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
        theLowIncomeBuildingReceivedFundTime.fill(99999999.0);

        theModerateIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
        theModerateIncomeBuildingReceivedFundTime.fill(99999999.0);

        theHighIncomeBuildingReceivedFundTime.resize(theNumberOfBuildings);
        theHighIncomeBuildingReceivedFundTime.fill(99999999.0);

        theAvailableMaterials.resize(theNumberOfBuildings);
        theAvailableMaterials.fill(0.0);

        theNumberOfWorkersAvailable.resize(theNumberOfBuildings);
        theNumberOfWorkersAvailable.fill(0.0);

        theBuildingTimeUnderRepairs.resize(theNumberOfBuildings);
        theBuildingTimeUnderRepairs.fill(0.0);

        theInspectionState.resize(theNumberOfBuildings);
        theInspectionState.fill(false);

        theBiddingProcess.resize(theNumberOfBuildings);
        theBiddingProcess.fill(false);

        theBuildingNeedForRedesign.resize(theNumberOfBuildings);
        theBuildingNeedForRedesign.fill(false);

        theEngineeringAssessment.resize(theNumberOfBuildings);
        theEngineeringAssessment.fill(false);

        theBuildingPermitStatus.resize(theNumberOfBuildings);
        theBuildingPermitStatus.fill(false);

        theRepairClass.resize(theNumberOfBuildings);
        theRepairClass.fill(0);

        theBuildingStrDamageState.resize(theNumberOfBuildings);
        theBuildingStrDamageState.fill(0);

        theBuildingNSDDamageState.resize(theNumberOfBuildings);
        theBuildingNSDDamageState.fill(0);

        theBuildingNSADamageState.resize(theNumberOfBuildings);
        theBuildingNSADamageState.fill(0);

        theRecoveryTime.resize(theNumberOfBuildings);
        theRecoveryTime.fill(999999.0);

        theRecoveryStartTime.resize(theNumberOfBuildings);
        theRecoveryStartTime.fill(999999.0);

        theRecoveryTimeLowIncomeVector.resize(theNumberOfBuildings);
        theRecoveryTimeLowIncomeVector.fill(999999.0);

        theRecoveryTimeModerateIncomeVector.resize(theNumberOfBuildings);
        theRecoveryTimeModerateIncomeVector.fill(999999.0);

        theRecoveryTimeHighIncomeVector.resize(theNumberOfBuildings);
        theRecoveryTimeHighIncomeVector.fill(999999.0);

        theRecoveryStartTimeLowIncomeVector.resize(theNumberOfBuildings);
        theRecoveryStartTimeLowIncomeVector.fill(999999.0);

        theRecoveryStartTimeModerateIncomeVector.resize(theNumberOfBuildings);
        theRecoveryStartTimeModerateIncomeVector.fill(999999.0);

        theRecoveryStartTimeHighIncomeVector.resize(theNumberOfBuildings);
        theRecoveryStartTimeHighIncomeVector.fill(999999.0);

        alreadyRequestedFinancing.resize(theNumberOfBuildings);
        alreadyRequestedFinancing.fill(false);

        alreadyRequestedEngineers.resize(theNumberOfBuildings);
        alreadyRequestedEngineers.fill(false);

        alreadyRequestedMaterials.resize(theNumberOfBuildings);
        alreadyRequestedMaterials.fill(false);

        alreadyRequestedNegotiation.resize(theNumberOfBuildings);
        alreadyRequestedNegotiation.fill(false);

        alreadyRequestedAllocation.resize(theNumberOfBuildings);
        alreadyRequestedAllocation.fill(false);
    } // if

    // --------------------------------------------------------------------------------------
    // 2 - Evaluate the availability of water and power to the neighborhood
    // --------------------------------------------------------------------------------------

    if (theCurrentTime->getCurrentValue() - timeOfEvent < 0.1){
        qCritical() << this->objectName() << "the availability of power is being overwritten!!";
    }

    // overwrite the availability of power and water to be true
    // The availability of power or water is not being accounted for
    // *************
    theAvailabilityOfPower = true;
    theAvailabilityOfWater = true;
    // *************

    // Power shortage
    if (theAvailabilityOfPower == true){
        thePowerShortageDuration = 0.0;
    } else {
        thePowerShortageDuration += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    // Water shortage
    if (theAvailabilityOfWater == true){
        theWaterShortageDuration = 0.0;
    } else {
        theWaterShortageDuration += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    thePowerShortageDurationResponse->setCurrentValue(thePowerShortageDuration);
    theWaterShortageDurationResponse->setCurrentValue(theWaterShortageDuration);


    // --------------------------------------------------------------------------------------
    // 3 - Get hold of packets delivered on this time step
    // --------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the buildings in this neighborhood have an object to request workers from
    for (int i = 0; i < theContractorList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theContractorList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theContractorList.at(i);
        }
    }


    // Check if the buildings in this neighborhood have an object to request insurance money from
    for (int i = 0; i < theFinancialResourcesSupplierList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theFinancialResourcesSupplierList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theFinancialResourcesSupplierList.at(i);
        }
    }

    // Load the packets received
    this->loadPackets(thePacketList);



    // --------------------------------------------------------------------------------------
    // 4 - Calculate everything that happens at the time of the earthquake (t = t_0)
    // --------------------------------------------------------------------------------------
    // Immediate losses
    // Considering only one earthquake in this analysis - think about aftershocks later
    if (timeOfEvent == 0.0){

        // Delete the packets whose final destinations was this model
        this->handlePacketsThatReachedFinalDestination();

        // If there was damage to any building set the time of the event as the current time
        timeOfEvent = theCurrentTime->getCurrentValue();

        // Delete the packets whose final destinations was this model
        this->handlePacketsThatReachedFinalDestination();

        // Building damage model
        theBuildingDamageModel = qobject_cast<RHazusBuildingModel*>(theBuildingDamageModelResponse->getModel());

        // ******
        RNetworkSamplingModel * theSamplingModel = theDomain->findChild<RNetworkSamplingModel*>();
        double theMultiplier = theSamplingModel->getSamplingMultiplier();
        // ******
        theMultiplier = 1.0;
        //qCritical()<< "The sampling multiplier is set to 1 for ground motions!!!";
        //qCritical()<< "The 5th set of ground motion is used! ";

        //qCritical() << "Overwriting the Sa0.3 and Sa1!! Scale the first set of SA's by the sampling multiplier";
        // For all the buildings
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
            double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

            QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
            double Sa1 =  theSa1Vector.at(theSa1Counter);

            QVector<double> thePGAVector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getPGAIntensity());
            double thePGA = theSa1Vector.at(thePGACounter);

            QString theSoilSusceptibility = QString("None");
            double rngSoil = rand()/static_cast<double>(RAND_MAX);
            if (rngSoil < theProbabilityOfLiquefaction){
                theSoilSusceptibility = "High";
            }


            // Give the building model the properties of this building
            theBuildingDamageModel->setBuildingType(theBuildingType.at(i));
            theBuildingDamageModel->setCodeDesignLevel(theBuildingCodeLevel.at(i));
            theBuildingDamageModel->setOccupancyClass(theBuildingOccupancyClass.at(i));
            theBuildingDamageModel->setSa1(Sa1);
            theBuildingDamageModel->setSa2(Sa0_3);
            theBuildingDamageModel->setPGA(thePGA);
            theBuildingDamageModel->setSoilSusceptibilityLevel(theSoilSusceptibility);
            theBuildingDamageModel->evaluateModel();

            // Get the damage responses
            theBuildingStrDamageState[i] = theBuildingDamageModel->getStrDS();
            theBuildingNSDDamageState[i] = theBuildingDamageModel->getNSDDS();
            theBuildingNSADamageState[i] = theBuildingDamageModel->getNSADS();
            theAverageDamageState += theBuildingStrDamageState[i];

            // Calculate the number of low-, moderate-, and high-income buildings

            if (theBuildingOccupancyClass.at(i) == QString("RES1")){
                if (theBuildingOwnerIncome.at(i) == QString("Low")){
                    theNumberOfLowIncomeSFBuildings += 1;
                }
                else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                    theNumberOfModerateIncomeSFBuildings += 1;
                }
                else if (theBuildingOwnerIncome.at(i) == QString("High")){
                    theNumberOfHighIncomeSFBuildings += 1;
                }
            }

            // Considering only one earthquake in this analysis - think about aftershocks later
            //if (theBuildingStrDamageState[i] > 0.0 || theBuildingNSDDamageState.at(i) > 0.0 || theBuildingNSADamageState.at(i) > 0.0){

            // Get hold of the damaged buildings
            if (theBuildingStrDamageState[i] > 2){
                theDamagedBuildingsIndexes << i;
            }

            // Estimate immediate impact
            theImmediateEconomicLoss[i] = theBuildingDamageModel->getLossRatio() * theBuildingValue.at(i);
            theImmediateContentLoss[i] = theBuildingDamageModel->getContentLossRatio() * theBuildingValue.at(i);
            theBuildingRepairTime[i] = theBuildingDamageModel->getRepairTime();
            theBuildingReoccupancyTime[i] = theBuildingDamageModel->getReoccupancyTime();

            // If the immediate loss is higher than the building value it means the building collapsed
            if (theImmediateEconomicLoss.at(i) > theBuildingValue.at(i)){
                theBuildingNeedForRedesign[i] = true;
            }

            // If this is a single-family renter-occupied building set a delay in taking action
            if (theBuildingOccupancyClass.at(i) == QString("RES1") && theHouseholdTenureStatus.at(i).at(0) == QString("RenterOccupied")){
                theDelayInTakingAction[i] = 30.0;
            }


            // Calculate the number of damaged single-family housing of each income group
            if (theBuildingStrDamageState.at(i) > 2 && theBuildingOccupancyClass.at(i) == QString("RES1")){
                if (theBuildingOwnerIncome.at(i) == QString("Low")){
                    theNumberOfDamageLowIncomeSFBuildings += 1;
                }
                else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                    theNumberOfDamageModerateIncomeSFBuildings += 1;
                }
                else if (theBuildingOwnerIncome.at(i) == QString("High")){
                    theNumberOfDamageHighIncomeSFBuildings += 1;
                }
            }
            //} // if DS > 0

            QVector<bool> auxHouseholdFatality;

            // For all dwellings in this building, check if there were fatalities/injuries
            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
                theMorbidityEstimator->setTime(remainder(theCurrentTime->getCurrentValue(), qFloor(theCurrentTime->getCurrentValue())));
                theMorbidityEstimator->setBuildingType(theBuildingType.at(i));
                theMorbidityEstimator->setDamageState(theBuildingStrDamageState.at(i));
                theMorbidityEstimator->setIncome(theHouseholdIncome.at(i).at(j));
                auxHouseholdFatality << theMorbidityEstimator->estimateFatalities();

                // Reset the destination of the households
                theHouseholdDestination[i][j] = QString("NotDisplaced");
            } // j

            theHouseholdFatalityIncidence << auxHouseholdFatality;

            // Threshold prob of being sold
            theBuildingThresholdTimeToBeSold[i] = rand() / static_cast<double>(RAND_MAX);

            // Threshold prob of leaving
            theBuildingThresholdTimeToMakeADecision[i] = rand() / static_cast<double>(RAND_MAX);

            // Get hold of the number of persons per demographic
            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
                // Building code
                if (theBuildingCodeLevel.at(i) == QString("Pre")){
                    totalPreCodePersons += theHouseholdSize.at(i).at(j);
                } else if (theBuildingCodeLevel.at(i) == QString("Low")){
                    totalLowCodePersons += theHouseholdSize.at(i).at(j);
                } else if (theBuildingCodeLevel.at(i) == QString("Moderate")){
                    totalModCodePersons += theHouseholdSize.at(i).at(j);
                } else if (theBuildingCodeLevel.at(i) == QString("High")){
                    totalHighCodePersons += theHouseholdSize.at(i).at(j);
                } else {
                    qCritical() << "Cannot identify the building code level for this building!";
                }

                // Income
                if (theBuildingOwnerIncome.at(i) == QString("Low")){
                    totalPoorPersons += theHouseholdSize.at(i).at(j);
                }
                else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                    totalModeratePersons += theHouseholdSize.at(i).at(j);
                }
                else if (theBuildingOwnerIncome.at(i) == QString("High")){
                    totalRichPersons += theHouseholdSize.at(i).at(j);
                }

                // Tenure
                if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                    totalRenterPersons += theHouseholdSize.at(i).at(j);
                }
                else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                    totalOwnerPersons += theHouseholdSize.at(i).at(j);
                }

                // Immigrant status
                if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                    totalRecentImmigrantPersons += theHouseholdSize.at(i).at(j);
                }
                else if (theBuildingOwnerImmigrationStatus.at(i) == QString("EstablishedImmigrant")){
                    totalEstablishedImmigrantPersons += theHouseholdSize.at(i).at(j);
                }
                else if (theBuildingOwnerImmigrationStatus.at(i) == QString("NonImmigrant")){
                    totalNonImmigrantPersons += theHouseholdSize.at(i).at(j);
                }

                // Elderly
                if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                    totalElderlyMemberPersons += theHouseholdSize.at(i).at(j);
                }
                else {
                    totalNoElderlyMemberPersons += theHouseholdSize.at(i).at(j);
                }

                // Children
                if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                    totalChildrenMemberPersons += theHouseholdSize.at(i).at(j);
                }
                else {
                    totalNoChildrenMemberPersons += theHouseholdSize.at(i).at(j);
                }

                // HigherEducated degree
                if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                    totalHigherEducatedPersons += theHouseholdSize.at(i).at(j);

                } else {
                    totalNonHigherEducatedPersons += theHouseholdSize.at(i).at(j);
                }

                // Marital status of householder
                if (theHouseholdMaritalStatus.at(i).at(j) == QString("MarriedCouple")){
                    totalMarriedCouplesPersons += theHouseholdSize.at(i).at(j);
                } else {
                    totalSingleParentsPersons += theHouseholdSize.at(i).at(j);
                }

                // Race
                if (theHouseholdRace.at(i).at(j).contains("White")) {
                    totalWhitePersons += theHouseholdSize.at(i).at(j);
                } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                    totalBlackPersons += theHouseholdSize.at(i).at(j);
                } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                    totalAsianPersons += theHouseholdSize.at(i).at(j);
                }

                // Hispanic
                if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                    totalHispanicPersons += theHouseholdSize.at(i).at(j);
                } else {
                    totalNonHispanicPersons += theHouseholdSize.at(i).at(j);
                }

                // Recent mover status
                if (theHouseholdRecentMoverStatus.at(i).at(j) == true) {
                    totalRecentMoverPersons += theHouseholdSize.at(i).at(j);
                } else {
                    totalNotRecentMoverPersons += theHouseholdSize.at(i).at(j);
                }
            }
        } // i

        theAverageDamageState /= theNumberOfBuildings;

    } // if timeOfEvent == 0.0


    // Extract contents from the packets received
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {

        // Get hold of what is the content of this packet
        QString packetType = packetsCurrentlyOnboard.at(i)->getPacketContentsType();
        QVector<int> packetOwnerID = packetsCurrentlyOnboard.at(i)->getPacketOwner();// size of packetOwnerID equals the number of buildings
        QVector<double> contentsReceived = packetsCurrentlyOnboard.at(i)->getPacketContents();

        for (int j = 0; j < contentsReceived.size(); j++){
            int packetOwnerI = packetOwnerID.at(j);
            double amountReceived = contentsReceived.at(j);

            if (theNotableBuildingsIndexes.contains(packetOwnerI) == false){
                theNotableBuildingsIndexes << packetOwnerI;
            }

            // If this packet contains Money
            if (packetType.contains(QString("Money"))){
                if (amountReceived > 0){
                    //theBuildingAvailableFunds[packetOwnerI] += amountReceived;
                }
            }

            // If this packet contains workforce
            else if (packetType == QString("Allocation")){
                if (amountReceived > 0){
                    theNumberOfWorkersAvailable[packetOwnerI] += static_cast<int>(amountReceived);
                    theRecoveryStartTime[packetOwnerI] = theCurrentTime->getCurrentValue() - timeOfEvent;
                    // qCritical()<<"packetOwnerI "<<packetOwnerI <<" theRecoveryStartTime.at(packetOwnerI)"<<theRecoveryStartTime.at(packetOwnerI);
                    // qCritical()<<"packetOwnerI "<<packetOwnerI << " Damage State: "<< theBuildingStrDamageState.at(packetOwnerI);
                }
            }
        }
    }

    // Delete the packets whose final destinations was this model
    this->handlePacketsThatReachedFinalDestination();

    for (int i = 0; i < theNumberOfBuildings; i++){
        if (theBuildingAvailableFunds.at(i) < theImmediateEconomicLoss.at(i) && theBuildingReceivedFundTime.at(i) < theCurrentTime->getCurrentValue() - timeOfEvent){
            theBuildingAvailableFunds[i] += theImmediateEconomicLoss.at(i) - theBuildingAvailableFunds.at(i);
            theNotableBuildingsIndexes << i;
        }
    }

    // --------------------------------------------------------------------------------------
    // 5 - Calculate everything that happens after the time of the earthquake (t > t_0)
    // --------------------------------------------------------------------------------------
    // Obtain resources for repairing the buildings

    // ******************
    // ******************
    //                               Financing  ----------------------------\
    // Earthquake  -- Inspection --  Materials ----------------------------- \ Workers ->REPAIRS
    //                               Engineering mobilization --Permitting-- /
    //                Utilities --------------------------------------------/
    // ******************
    // ******************

    QElapsedTimer myTimer;
    myTimer.start();
    // Rebuilding processes
    theNumberOfHabitableDwellings = theTotalNumberOfDwellings;

    double numberOfPreCodeUninhabitableDwellings = 0.0;
    double numberOfLowCodeUninhabitableDwellings = 0.0;
    double numberOfModerateCodeUninhabitableDwellings = 0.0;
    double numberOfHighCodeUninhabitableDwellings = 0.0;
    double numberOfDetachedUninhabitableDwellings = 0.0;
    double numberOfMultifamilyUninhabitableDwellings = 0.0;
    double numberOfLowHouseholdIncomeUninhabitableDwellings = 0.0;
    double numberOfMediumHouseholdIncomeUninhabitableDwellings = 0.0;
    double numberOfHighHouseholdIncomeUninhabitableDwellings = 0.0;
    double numberOfRecentImmigrantUninhabitableDwellings = 0.0;
    double numberOfEstablishedImmigrantUninhabitableDwellings = 0.0;
    double numberOfCitizenUninhabitableDwellings = 0.0;
    double numberOfOwnerOccupiedUninhabitableDwellings = 0.0;
    double numberOfRenterOccupiedUninhabitableDwellings = 0.0;
    double numberOfHigherEducatedUninhabitableDwellings = 0.0;
    double numberOfMarriedCouplesUninhabitableDwellings = 0.0;
    double numberOfSingleParentsUninhabitableDwellings = 0.0;
    double numberOfRecentMoversUninhabitableDwellings = 0.0;
    double numberOfNotRecentMoversUninhabitableDwellings = 0.0;
    double numberOfNonHigherEducatedUninhabitableDwellings = 0.0;
    double numberOfElderlyUninhabitableDwellings = 0.0;
    double numberOfNoElderlyUninhabitableDwellings = 0.0;
    double numberOfChildrenUninhabitableDwellings = 0.0;
    double numberOfNoChildrenUninhabitableDwellings = 0.0;
    double numberOfWhiteUninhabitableDwellings = 0.0;
    double numberOfBlackUninhabitableDwellings = 0.0;
    double numberOfAsianUninhabitableDwellings = 0.0;
    double numberOfHispanicUninhabitableDwellings = 0.0;
    double numberOfNonHispanicUninhabitableDwellings = 0.0;

    theNumberOfPeopleSeekingShelter = 0;
    theNumberOfPeopleInHotels = 0;
    theNumberOfPersonsGone = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    QVector<int> indexesToRemove;

    // Go over everything that will be evaluated on each 'delta t'
    if (timeOfEvent > 0.0) {

        // Handle all requests that need to be made on this time step
        QVector<int> theInsuranceRequestID;
        QVector<int> thePrivateRequestID;
        QVector<int> thePublicRequestID;
        QVector<int> theGrantRequestID;
        QVector<int> theSFContractorAllocationRequestID;
        QVector<int> theMFContractorAllocationRequestID;
        QVector<int> theBuildingsToBeLoopedOver;

        if (theCurrentTime->getCurrentValue() - timeOfEvent < 1.0){
            // If this is the time of event loop over all damaged buildings
            theBuildingsToBeLoopedOver = theDamagedBuildingsIndexes;
        } else {
            // In other time steps loop over only the buildigns that are being repaired
            theBuildingsToBeLoopedOver = theNotableBuildingsIndexes;
        }

        // ***************************************************************
        // Loop over all damaged buildings
        // ***************************************************************
        //for (int i = 0; i < theDamagedBuildingsIndexes.size(); i++){
        for (int i = 0; i < theBuildingsToBeLoopedOver.size(); i++){

            // NOTE: we are only looping over damaged buildings, make sure to properly use the counter 'ii' instead 'i'
            int ii = theBuildingsToBeLoopedOver.at(i);

            // If repairs have just finished successfully
            // NOTE: comparing theBuildingRepairTime (time needed to repair) and theBuildingTimeUnderRepairs is what defines recovery state
            // NOTE: a building with theNumberOfWorkersAvailable > 0 means it is under repairs
            if (theBuildingRepairTime.at(ii) <= theBuildingTimeUnderRepairs.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){

                theRecoveryTime[ii] = theCurrentTime->getCurrentValue() - timeOfEvent;
                //qCritical()<<"The recovery time is set for building "<<ii<<": "<< theRecoveryTime.at(ii);
                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
                    RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                    theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));

                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
                    RMultiFamilyContractorFirmModel *theMFContractorAllocationModel = theDomain->findChild<RMultiFamilyContractorFirmModel*>();
                    theMFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                }

                //qCritical() << "Repairs have finished here!" << "dismissing" << theBuildingType.at(ii) << theBuildingOccupancyClass.at(ii) << theNumberOfWorkersAvailable.at(ii) << "workers.";
                theNumberOfWorkersAvailable[ii] = 0.0; // This defines that the building is not being repaired anymore
                theBuildingStrDamageState[ii] = 0; // This defines that the building is repaired
                theBuildingNSDDamageState[ii] = 0;
                theBuildingNSADamageState[ii] = 0;

                indexesToRemove << ii;
            }

            // If repairs are progressing normally
            else if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){

                theBuildingTimeUnderRepairs[ii] += static_cast<double>(theCurrentTime->getCurrentValue() - lastEvaluationTime);
                // If repairs are going on, the household has not left, and the building can be reoccupied
                if (theBuildingReoccupancyTime.at(ii) <= theBuildingTimeUnderRepairs.at(ii)){
                    for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){
                        if (theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == false){
                            theHouseholdDestination[ii][j] = QString("RecoveryCompleted");
                        }
                    }
                }
            }


            // Lastly, the building is not repaired and some resource is still missing, request it
            // Rodrigo's changes
            if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii)  && (theBuildingAvailableFunds.at(ii) <= 0.0 || theNumberOfWorkersAvailable.at(ii) <= 0)){
                if (theCurrentTime->getCurrentValue() - timeOfEvent > 600.0){
                    //                  qCritical() << ii << theBuildingType.at(ii) << theBuildingRepairTime.at(ii) << theBuildingOwnerIncome.at(ii) << theBuildingAmountLendedFromSBA.at(ii) << theBuildingAmountLendedFromBank.at(ii) << "Funds:" << theBuildingAvailableFunds.at(ii)/theImmediateEconomicLoss.at(ii) << "Workers:" << theNumberOfWorkersAvailable.at(ii);
                }

                // ***************************************************************
                // Loop over all dwellings
                // ***************************************************************
                // Loop over all households in the buildings
                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){

                    if (theBuildingOccupancyClass.at(ii) == QString("RES1") && alreadyRequestedFinancing.at(ii) == false){

                        // Insurance
                        double theMedianDelayForPaymentsFromInsurance = 6.0;
                        double theDispersionOfDelayForPaymentsFromInsurance = 1.11;
                        double m = theMedianDelayForPaymentsFromInsurance;
                        double v = theDispersionOfDelayForPaymentsFromInsurance * theDispersionOfDelayForPaymentsFromInsurance;
                        double phi = qSqrt(v + m * m);
                        double mu = qLn(m * m / phi);
                        double sigma = qSqrt(qLn((phi*phi) / (m*m)));
                        double theUx = rand()/static_cast<double>(RAND_MAX);
                        double theDelaysFromInsurance = 30.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

                        // Bank
                        double theMedianDelayForPaymentsForBank = 15.0 ;
                        double theDispersionOfDelayForPaymentsFromBank = 0.68;
                        m = theMedianDelayForPaymentsForBank;
                        v = theDispersionOfDelayForPaymentsFromBank * theDispersionOfDelayForPaymentsFromBank;
                        phi = qSqrt(v + m * m);
                        mu = qLn(m * m / phi);
                        sigma = qSqrt(qLn((phi*phi) / (m*m)));
                        theUx = rand()/static_cast<double>(RAND_MAX);
                        double theDelaysFromBank = 90.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

                        // SBA
                        double theMedianDelayForPaymentsForSBA = 48.0;
                        double theDispersionOfDelayForPaymentsFromSBA = 0.57;
                        m = theMedianDelayForPaymentsForSBA;
                        v = theDispersionOfDelayForPaymentsFromSBA * theDispersionOfDelayForPaymentsFromSBA;
                        phi = qSqrt(v + m * m);
                        mu = qLn(m * m / phi);
                        sigma = qSqrt(qLn((phi*phi) / (m*m)));
                        theUx = rand()/static_cast<double>(RAND_MAX);
                        double theDelaysFromSBA = 90.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

                        // CDBG
                        double theProgramStartDelay = 1.5 * 365.0;
                        double theMedianDelayForPaymentsForCDBG = 208.0;
                        theUx = rand()/static_cast<double>(RAND_MAX);
                        double theDelaysFromCDBG = theProgramStartDelay + 7.0 * theUx * theMedianDelayForPaymentsForCDBG;

                        QString theCity = QString("SanFrancisco");
                        if (theCity == QString("SanFrancisco")){

                            // Single family buildings
                            if (theBuildingOccupancyClass.at(ii) == QString("RES1")){

                                theBuildingReceivedFundTime[ii] += 1.0;

                                // If the building is owner occupied
                                if (theHouseholdTenureStatus.at(ii).at(j) == QString("OwnerOccupied")){

                                    double rngBankLoan = rand()/static_cast<double>(RAND_MAX);
                                    double rngSBALoan = rand()/static_cast<double>(RAND_MAX);
                                    double rngInsurance = rand()/static_cast<double>(RAND_MAX);

                                    // If insured and considering a deductible of 15%
                                    if (rngInsurance < 0.15){
                                        theBuildingExpectedFunds[ii]       += qMax(theImmediateEconomicLoss.at(ii) - 0.15 * theBuildingValue.at(ii),0.0);
                                        theBuildingAmountFromInsurance[ii] += qMax(theImmediateEconomicLoss.at(ii) - 0.15 * theBuildingValue.at(ii),0.0);
                                        theBuildingReceivedFundTime[ii] += theDelaysFromInsurance;
                                    }

                                    // If high income
                                    if (theBuildingOwnerIncome.at(ii) == QString("High")){

                                        // If no mortgage get a full loan
                                        if (theHouseholdMortgage.at(ii).at(j) == false) {
                                            theBuildingAmountLendedFromBank[ii] += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            theBuildingExpectedFunds[ii]        += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            thePrivateRequestID << ii;
                                            theBuildingReceivedFundTime[ii] += theDelaysFromBank;
                                        } // If has mortgage, try bank loan for 50% of income

                                        else if (rngBankLoan < 0.91){
                                            theBuildingAmountLendedFromBank[ii] += qMin(0.5*theHomeownerIncomeValue.at(ii),theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii));
                                            theBuildingExpectedFunds[ii]        += qMin(0.5*theHomeownerIncomeValue.at(ii),theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii));
                                            thePrivateRequestID << ii;
                                            theBuildingReceivedFundTime[ii] += theDelaysFromBank;
                                        }

                                    } // Moderate income
                                    else if (theBuildingOwnerIncome.at(ii) == QString("Moderate")){

                                        // If no mortgage get a full loan
                                        if (theHouseholdMortgage.at(ii).at(j) == false) {
                                            theBuildingAmountLendedFromBank[ii] += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            theBuildingExpectedFunds[ii]        += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            thePrivateRequestID << ii;
                                            theBuildingReceivedFundTime[ii] += theDelaysFromBank;
                                        }

                                        // If still not enough
                                        if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                            // If has mortgage, try SBA loan with 47% to get up to 0.48*needed amount capped at 200,000
                                            if (rngSBALoan <= 0.47){
                                                theBuildingAmountLendedFromSBA[ii] += qMin(0.48*(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii)),200000.0);
                                                theBuildingExpectedFunds[ii]       += qMin(0.48*(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii)),200000.0);
                                                thePublicRequestID << ii;
                                                theBuildingReceivedFundTime[ii] += theDelaysFromSBA;

                                                // If still not enough after SBA
                                                if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                                    // Try Bank loan
                                                    // If SBA loan was approved, bank loan will be approved with max 50% of income
                                                    theBuildingAmountLendedFromBank[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    theBuildingExpectedFunds[ii]        += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    thePrivateRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromBank;

                                                    // If still not enough after Bank loan
                                                    if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){
                                                        // if all else fails use wait for CDBG-DR
                                                        theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theGrantRequestID << ii;
                                                        theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;
                                                    } // CDBG
                                                } // SBA and Bank
                                            } // If SBA was not approved
                                            else {
                                                // if SBA loan was not approved the chance to get a bank loan is 58%
                                                if (rngBankLoan < 0.58){
                                                    theBuildingAmountLendedFromBank[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    theBuildingExpectedFunds[ii]        += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    thePrivateRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromBank;

                                                    // If still not enough after Bank loan
                                                    if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                                        // if all else fails use wait for CDBG-DR
                                                        theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theGrantRequestID << ii;
                                                        theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;
                                                    } // CDBG
                                                } // If no bank loan

                                                else {
                                                    // if all else fails use wait for CDBG-DR
                                                    theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                    theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                    theGrantRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;
                                                }
                                            } // SBA
                                        } // If mortgage
                                    } // If moderate

                                    else if (theBuildingOwnerIncome.at(ii) == QString("Low")){

                                        // If no mortgage get a full loan
                                        if (theHouseholdMortgage.at(ii).at(j) == false) {
                                            theBuildingAmountLendedFromBank[ii] += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            theBuildingExpectedFunds[ii]        += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                            thePrivateRequestID << ii;
                                            theBuildingReceivedFundTime[ii] += theDelaysFromBank;
                                        }

                                        // If still not enough
                                        if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                            // If has mortgage, try SBA loan with 47% to get up to 0.48*needed amount capped at 200,000
                                            if (rngSBALoan <= 0.47){
                                                theBuildingAmountLendedFromSBA[ii] += qMin(0.48*(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii)),200000.0);
                                                theBuildingExpectedFunds[ii]       += qMin(0.48*(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii)),200000.0);
                                                thePublicRequestID << ii;
                                                theBuildingReceivedFundTime[ii] += theDelaysFromSBA;

                                                // If still not enough after SBA
                                                if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                                    // Try Bank loan
                                                    // If SBA loan was approved, bank loan will be approved with max 50% of income
                                                    theBuildingAmountLendedFromBank[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    theBuildingExpectedFunds[ii]        += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    thePrivateRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromBank;

                                                    // If still not enough after Bank loan
                                                    if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){
                                                        // if all else fails use wait for CDBG-DR
                                                        theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theGrantRequestID << ii;
                                                        theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;

                                                    } // CDBG
                                                } // SBA and Bank
                                            } // If SBA was not approved
                                            else {
                                                // if SBA loan was not approved the chance to get a bank loan is 19%
                                                if (rngBankLoan < 0.19){
                                                    theBuildingAmountLendedFromBank[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    theBuildingExpectedFunds[ii]        += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),0.5*theHomeownerIncomeValue.at(ii));
                                                    thePrivateRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromBank;

                                                    // If still not enough after Bank loan
                                                    if (theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii)){

                                                        // if all else fails use wait for CDBG-DR
                                                        theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                        theGrantRequestID << ii;
                                                        theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;
                                                    } // CDBG
                                                } // If no bank loan
                                                else {
                                                    // if all else fails use wait for CDBG-DR
                                                    theBuildingAmountGrantedByCDBGDR[ii] += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                    theBuildingExpectedFunds[ii]         += qMin(theImmediateEconomicLoss.at(ii)-theBuildingExpectedFunds.at(ii),150000.0);
                                                    theGrantRequestID << ii;
                                                    theBuildingReceivedFundTime[ii] += theDelaysFromCDBG;
                                                }
                                            } // SBA
                                        } // If mortgage
                                    } // Low income
                                    // If the building is renter-occupied
                                } else if (theHouseholdTenureStatus.at(ii).at(j) == QString("RenterOccupied")) {

                                    double rngBankLoan = rand()/static_cast<double>(RAND_MAX);

                                    // If no mortgage get a full loan
                                    if (rngBankLoan < 0.91) {
                                        thePrivateRequestID << ii;
                                        theBuildingAmountLendedFromBank[ii] += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                        theBuildingExpectedFunds[ii] += theImmediateEconomicLoss.at(ii) - theBuildingExpectedFunds.at(ii);
                                        theBuildingReceivedFundTime[ii] += 2.0 * theDelaysFromBank;
                                    }
                                    else {
                                        theBuildingReceivedFundTime[ii] += 4.0 * theDelaysFromBank;
                                    }

                                } else {
                                    qCritical() << this->objectName() << "has a problem with the building tenure.";
                                }

                                double theSavings = qMax(theImmediateEconomicLoss[ii] - theBuildingExpectedFunds[ii],0.0);;
                                //theBuildingAvailableFunds[ii] += theSavings;
                                theBuildingExpectedFunds[ii] += theSavings;

                                if (theBuildingAvailableFunds.at(i) >= theImmediateEconomicLoss.at(i)){
                                    theBuildingReceivedFundTime[ii] += rand()/static_cast<double>(RAND_MAX) * 180.0;
                                }

                            } // RES1
                        }
                    } // Multi-family vs Single-family
                    alreadyRequestedFinancing[ii] = true;
                    if (theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == false && theHouseholdDestination.at(ii).at(j).contains(QString("Recovery")) == false){

                        // ****************
                        // Displaced, no funds
                        // ****************
                        // Rodrigo's changes
                        // You have to compare with the expected funds here rather than the available
                        if (theBuildingStrDamageState.at(ii) > 2 && theBuildingExpectedFunds.at(ii) < theImmediateEconomicLoss.at(ii) && theBuildingOccupancyClass.at(ii) == QString("RES1")){

                            double thePoissonTimeToMakeADecision = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 180.0);
                            if (theHouseholdTenureStatus.at(ii).at(0) == QString("OwnerOccupied") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToBeSold.at(ii)){
                                theHouseholdDestination[ii][j] = QString("GoneNoFunds");
                                RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                                theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                                theNumberOfWorkersAvailable[ii] = 0.0;
                                theBuildingTimeSold[ii] = theCurrentTime->getCurrentValue();
                            }
                        }

                        // Provide the displacement estimator all the information needed to evaluate the status of the household
                        else {

                            // ****************
                            // Temporary displacement
                            // ****************
                            if (theHouseholdDestination.at(ii).at(j) == QString("NotDisplaced")){

                                theDisplacementEstimator->setDS(theBuildingStrDamageState.at(ii));
                                theDisplacementEstimator->setBuildingOccupancy(theBuildingOccupancyClass.at(ii));
                                theDisplacementEstimator->setAvailabilityOfGenerator(theAvailabilityOfGenerator.at(ii).at(j));
                                theDisplacementEstimator->setAvailabilityOfBottledWater(theAvailabilityOfAlternativeWaterSource.at(ii).at(j));
                                theDisplacementEstimator->setAge(theHouseholdAgeCategory.at(ii).at(j));
                                theDisplacementEstimator->setRace(theHouseholdRace.at(ii).at(j));
                                theDisplacementEstimator->setDisabilityStatus(theHouseholdDisabilityStatus.at(ii).at(j));
                                theDisplacementEstimator->setHigherEdStatus(theHouseholdHigherEducationStatus.at(ii).at(j));
                                theDisplacementEstimator->setAverageDamageState(theAverageDamageState);
                                theDisplacementEstimator->setWeatherConditions(theWeatherConditions);
                                theDisplacementEstimator->setCarOwnership(theHouseholdCarOwnership.at(ii).at(j));
                                theDisplacementEstimator->setInsuranceStatus(theBuildingInsuranceStatus.at(ii));
                                theDisplacementEstimator->setDisasterDestination(theDisasterDestination.at(ii).at(j));
                                theDisplacementEstimator->setTenure(theHouseholdTenureStatus.at(ii).at(j));
                                theDisplacementEstimator->setIncome(theHouseholdIncome.at(ii).at(j));
                                theDisplacementEstimator->setSize(theHouseholdSize.at(ii).at(j));
                                theDisplacementEstimator->setRecentMoverStatus(theHouseholdRecentMoverStatus.at(ii).at(j));
                                theDisplacementEstimator->setFatalityIncidence(theHouseholdFatalityIncidence.at(ii).at(j));
                                theDisplacementEstimator->setCurrentLivingConditions(theHouseholdDestination.at(ii).at(j));

                                // For everyone else, check if they are displaced
                                // Check if the household is displaced
                                bool theDisplacementStatus = theDisplacementEstimator->evaluateDisplacementStatus();

                                // If the household is displaced now but wasnt before, figure out its immediate destination
                                // Only check this if they are not already displaced somewhere
                                if (theDisplacementStatus == true){
                                    // Check where the household goes temporarily
                                    theHouseholdDestination[ii][j] = theDisplacementEstimator->evaluateShortTermDestination();
                                    //                                    qCritical()<<ii<<" theHouseholdDestination[ii][j]: "<<theHouseholdDestination.at(ii).at(j);
                                }
                            } // if not displaced
                        }
                    }

                    // If the building was sold
                    if (theBuildingOccupancyClass.at(ii) == QString("RES1") && theCurrentTime->getCurrentValue() - theBuildingTimeSold.at(ii) > 180.0){
                        // If the building was abandoned and sold, restart the resource procurement phase 180 days after the sell
                        // This time assume funding will be immediately available from the new owner
                        theBuildingAvailableFunds[ii] = theImmediateEconomicLoss.at(ii) + 1.0;
                        alreadyRequestedEngineers[ii] = false;
                        alreadyRequestedMaterials[ii] = false;
                        alreadyRequestedNegotiation[ii] = false;
                        theBuildingTimeSold[ii] = 1000000.0;
                        qCritical() << "This building has a new owner!";
                    }
                } // for j number of dwellings

                // Above, we defined if household would be able to finance repairs
                // and if so, where the households would get their money from.
                // For the households that will be able to finance their repairs, even if they do not have the money yet,
                // start negotiating resources.

                // Rodrigo's changes
                // If the owner knows that they will get money eventually, they can start on these
                // That is why we should compare to expected funds here
                // NOTE: we are actually skipping the process of requesting and just switching the indicator to 'true'
                // this is because this part is not crucial for this study and it takes some extra computational time
                if (theImmediateEconomicLoss.at(ii) > 0.0){

                    // ***************************************************************
                    // Contractor mobilization
                    // ***************************************************************
                    if (alreadyRequestedNegotiation.at(ii) == false){
                        //theContractorNegotiationRequestID << ii;
                        theBiddingProcess[ii] = true;
                        alreadyRequestedNegotiation[ii] = true;
                    }
                }
            } // if some resource is still missing


            // Rodrigo's changes
            // Here, we compare to the available funds because we want to bring the workers to the building and hence need to actual funding rather than a promise (aka expected) of funding
            //  If repairs are needed and power is restored and all resources were obtained and permit was obtained and materials were received
            if (theBuildingStrDamageState.at(ii) > 2 && theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theBuildingAvailableFunds.at(ii) >= theImmediateEconomicLoss.at(ii) && theNumberOfWorkersAvailable.at(ii) == 0 && alreadyRequestedAllocation[ii] == false){

                // ***************************************************************
                // Finally hire the contractor to perform repairs
                // ***************************************************************
                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
                    theSFContractorAllocationRequestID << ii;
                    //qDebug() << "Building" << ii << "with occupancy" << theBuildingOccupancyClass.at(ii) << "is requesting" << 1 << "workers" "(" << theImmediateEconomicLoss.at(ii) << ")";
                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
                    theMFContractorAllocationRequestID << ii;
                    //qDebug() << "Building" << ii << "with occupancy" << theBuildingOccupancyClass.at(ii) << "is requesting" << 1 << "workers" "(" << theImmediateEconomicLoss.at(ii) << ")";
                }

                // Reduce the funds available assuming that half of it goes towards labour costs
                theBuildingAvailableFunds[ii] -= theImmediateEconomicLoss.at(ii) / 2.0;
                alreadyRequestedAllocation[ii] = true;
            } // can make request
        } // for i number of buildings


        //        // The following lines manage passing requests to business agents
        //        // Request insurance payments
        //        theInsuranceRequest->clearRequesterIDList();
        //        theInsuranceRequest->clearRequestedAmountList();
        //        theInsuranceRequest->clearBuildingValueList();

        //        if (theInsuranceRequestID.size()){
        //            for (int i = 0; i < theInsuranceRequestID.size(); i++){
        //                int theID = theInsuranceRequestID.at(i);
        //                theInsuranceRequest->setRequesterBuildingValue(theBuildingValue.at(theID));
        //                theInsuranceRequest->setRequesterIDList(theID);
        //                theInsuranceRequest->setRequestedAmountList(qMax(theImmediateEconomicLoss.at(theID) - 0.15 * theBuildingValue.at(theID),0.0));
        //                //qCritical() << theID << "is making an insurance request";
        //            }
        //        }

        //        // Request publicly-backed loan
        //        thePublicLoanRequest->clearRequesterIDList();
        //        thePublicLoanRequest->clearRequestedAmountList();
        //        thePublicLoanRequest->clearOccupancyTypeList();

        //        if (thePublicRequestID.size() > 0){
        //            for (int i = 0; i < thePublicRequestID.size(); i++){
        //                int theID = thePublicRequestID.at(i);
        //                thePublicLoanRequest->setRequesterIDList(theID);
        //                thePublicLoanRequest->setRequestedAmountList(theBuildingAmountLendedFromSBA.at(theID));

        //                if (theBuildingOccupancyClass.at(theID) == QString("RES1")){
        //                    thePublicLoanRequest->setRequesterOccupancyType(theHouseholdTenureStatus.at(theID).at(0));
        //                } else {
        //                    thePublicLoanRequest->setRequesterOccupancyType(QString(""));
        //                }
        //                totalPublicLoanRequest += theImmediateEconomicLoss.at(theID);
        //            }
        //        }

        //        // Request private loan
        //        thePrivateLoanRequest->clearRequesterIDList();
        //        thePrivateLoanRequest->clearRequestedAmountList();
        //        thePrivateLoanRequest->clearIncomeList();

        //        if (thePrivateRequestID.size() > 0){
        //            for (int i = 0; i < thePrivateRequestID.size(); i++){
        //                int theID = thePrivateRequestID.at(i);
        //                thePrivateLoanRequest->setRequesterIncome(theBuildingOwnerIncome.at(theID));
        //                thePrivateLoanRequest->setRequesterIDList(theID);
        //                thePrivateLoanRequest->setRequestedAmountList(theBuildingAmountLendedFromBank.at(theID)+1.0);
        //            }
        //        }

        //        // Request grant money
        //        theGrantRequest->clearRequesterIDList();
        //        theGrantRequest->clearRequestedAmountList();
        //        theGrantRequest->clearOccupancyTypeList();

        //        if (theGrantRequestID.size() > 0){
        //            for (int i = 0; i < theGrantRequestID.size(); i++){
        //                int theID = theGrantRequestID.at(i);

        //                theGrantRequest->setRequesterIDList(theID);
        //                theGrantRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));

        //                if (theBuildingOccupancyClass.at(theID) == QString("RES1")){
        //                    theGrantRequest->setRequesterOccupancyType(theHouseholdTenureStatus.at(theID).at(0));
        //                } else {
        //                    theGrantRequest->setRequesterOccupancyType(QString(""));
        //                }
        //                totalGrantMoneyRequest += theImmediateEconomicLoss.at(theID);
        //            }
        //        }


        // Request contractor allocation
        theSFContractorAllocationRequest->clearOccupancyClassList();
        theSFContractorAllocationRequest->clearRepairClassList();
        theSFContractorAllocationRequest->clearRequesterIDList();
        theSFContractorAllocationRequest->clearRequestedAmountList();
        theSFContractorAllocationRequest->clearBidList();

        if (theSFContractorAllocationRequestID.size() > 0){
            for (int i = 0; i < theSFContractorAllocationRequestID.size(); i++){
                int theID = theSFContractorAllocationRequestID.at(i);

                // Temporary fix
                double theBid = 0.75;
                if (theBuildingOwnerIncome.at(i) == QString("High")){
                    theBid = 1.25;
                } else if (theBuildingOwnerIncome.at(i)  == QString("Moderate")){
                    theBid = 1.0;
                }

                theSFContractorAllocationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                theSFContractorAllocationRequest->setRepairClasses(1);
                theSFContractorAllocationRequest->setRequesterIDList(theID);
                theSFContractorAllocationRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                //                theSFContractorAllocationRequest->setBid(theBid * theBuildingValue.at(theID));
                theSFContractorAllocationRequest->setBid(theBid * 1.0/static_cast<double>(theBuildingStrDamageState.at(theID)));
            }
        }

        theMFContractorAllocationRequest->clearOccupancyClassList();
        theMFContractorAllocationRequest->clearRepairClassList();
        theMFContractorAllocationRequest->clearRequesterIDList();
        theMFContractorAllocationRequest->clearRequestedAmountList();

        if (theMFContractorAllocationRequestID.size() > 0){
            for (int i = 0; i < theMFContractorAllocationRequestID.size(); i++){
                int theID = theMFContractorAllocationRequestID.at(i);
                theMFContractorAllocationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                theMFContractorAllocationRequest->setRepairClasses(1);
                theMFContractorAllocationRequest->setRequesterIDList(theID);
                theMFContractorAllocationRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                theMFContractorAllocationRequest->setMedianIncome(theMedianHouseholdIncome);
                theMFContractorAllocationRequest->setBid(theBuildingValue.at(theID));
            }
        }


        // ***************************************************************
        // Get hold of results to plot
        // ***************************************************************
        for (int i = 0; i < theNumberOfBuildings; i++){

            // Prepare data for plotting
            if (theBuildingStrDamageState.at(i) > 2 && theBuildingRepairTime.at(i) > theBuildingTimeUnderRepairs.at(i)){
                theNumberOfHabitableDwellings -= theBuildingNumberOfDwellings.at(i);
            }

            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){

                //qCritical()<<"theHouseholdDestination.at(i).at(j): "<<theHouseholdDestination.at(i).at(j);

                // Shelter needs
                if (theHouseholdDestination.at(i).at(j) == QString("PublicShelter")){
                    theNumberOfPeopleSeekingShelter += theHouseholdSize.at(i).at(j);
                }

                // Hotels or rental units
                else if (theHouseholdDestination.at(i).at(j) == QString("RentalUnit")){
                    theNumberOfPeopleInHotels += theHouseholdSize.at(i).at(j);
                }

                // Living with families
                else if (theHouseholdDestination.at(i).at(j) == QString("Relatives")){
                    theNumberOfPeopleLivingWithTheirFamilies += theHouseholdSize.at(i).at(j);
                }

                // Gone
                else if (theHouseholdDestination.at(i).at(j).contains(QString("Gone"))){
                    theNumberOfPersonsGone += theHouseholdSize.at(i).at(j);
                }

                // Not displaced
                else if (theHouseholdDestination.at(i).at(j) == QString("NotDisplaced")) {

                }

                // Recovered
                else if (theHouseholdDestination.at(i).at(j).contains(QString("Recovery"))) {

                }

                // If a problem happened
                else {
                    //qCritical() << this->objectName() << "I dont know where this household went" << theHouseholdDestination.at(i).at(j);
                }


                if (theHouseholdDestination.at(i).at(j).contains(QString("Evicted"))){
                    theNumberOfPersonsEvicted += theHouseholdSize.at(i).at(j);
                }

                if (theHouseholdDestination.at(i).at(j).contains(QString("Voluntarily"))){
                    theNumberOfPersonsLeavingVoluntarily += theHouseholdSize.at(i).at(j);
                }


                // ***************************************************************
                // Gather the data to produce the recovery trajectories
                // ***************************************************************
                if (theBuildingTimeUnderRepairs.at(i) < theBuildingRepairTime.at(i)){

                    // Building code
                    if (theBuildingCodeLevel.at(i) == QString("Pre")){
                        numberOfPreCodeUninhabitableDwellings += 1.0;
                    } else if (theBuildingCodeLevel.at(i) == QString("Low")){
                        numberOfLowCodeUninhabitableDwellings += 1.0;
                    } else if (theBuildingCodeLevel.at(i) == QString("Moderate")){
                        numberOfModerateCodeUninhabitableDwellings += 1.0;
                    } else if (theBuildingCodeLevel.at(i) == QString("High")){
                        numberOfHighCodeUninhabitableDwellings += 1.0;
                    } else {
                        qCritical() << "Cannot identify the building code level for this building!";
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomeUninhabitableDwellings += 1.0;
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomeUninhabitableDwellings += 1.0;
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomeUninhabitableDwellings += 1.0;
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedUninhabitableDwellings += 1.0;
                    }
                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                        numberOfOwnerOccupiedUninhabitableDwellings += 1.0;
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantUninhabitableDwellings += 1.0;
                    }
                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("EstablishedImmigrant")){
                        numberOfEstablishedImmigrantUninhabitableDwellings += 1.0;
                    }
                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("NonImmigrant")){
                        numberOfCitizenUninhabitableDwellings += 1.0;
                    }

                    // Elderly
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyUninhabitableDwellings += 1.0;
                    }
                    else {
                        numberOfNoElderlyUninhabitableDwellings += 1.0;
                    }

                    // Children
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                        numberOfChildrenUninhabitableDwellings += 1.0;
                    }
                    else {
                        numberOfNoChildrenUninhabitableDwellings += 1.0;
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedUninhabitableDwellings += 1.0;

                    } else {
                        numberOfNonHigherEducatedUninhabitableDwellings += 1.0;
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhiteUninhabitableDwellings += 1.0;
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackUninhabitableDwellings += 1.0;
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianUninhabitableDwellings += 1.0;
                    }

                    // Hispanic
                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                        numberOfHispanicUninhabitableDwellings += 1.0;
                    } else {
                        numberOfNonHispanicUninhabitableDwellings += 1.0;
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("MarriedCouple")){
                        numberOfMarriedCouplesUninhabitableDwellings += 1.0;
                    } else {
                        numberOfSingleParentsUninhabitableDwellings += 1.0;
                    }

                    // Recent mover status
                    if (theHouseholdRecentMoverStatus.at(i).at(j) == true) {
                        numberOfRecentMoversUninhabitableDwellings += 1.0;
                    } else {
                        numberOfNotRecentMoversUninhabitableDwellings += 1.0;
                    }
                }
            } // for j
        } // for i

        //qCritical()<<"Time since event: theCurrentTime->getCurrentValue()-timeOfEvent"<<theCurrentTime->getCurrentValue()-timeOfEvent;
        if (theCurrentTime->getCurrentValue()-timeOfEvent > 720.0){
            // Prepare the repair start time and finished time data for plotting
            for (int i = 0; i < theNumberOfBuildings; i++){

                // building owner income
                if (theBuildingOwnerIncome.at(i) == QString("Low")){
                    theRecoveryStartTimeLowIncomeVector[i] = theRecoveryStartTime.at(i);
                    theRecoveryTimeLowIncomeVector[i] = theRecoveryTime.at(i);
                    theLowIncomeBuildingReceivedFundTime[i] = theBuildingReceivedFundTime.at(i);
                    //                qCritical()<<"Low income recovery time is set! "<< theRecoveryTime.at(i);
                }
                else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                    theRecoveryStartTimeModerateIncomeVector[i] = theRecoveryStartTime.at(i);
                    theRecoveryTimeModerateIncomeVector[i] = theRecoveryTime.at(i);
                    theModerateIncomeBuildingReceivedFundTime[i] = theBuildingReceivedFundTime.at(i);
                    //                qCritical()<<"Moderate income recovery time is set! "<< theRecoveryTime.at(i);
                }
                else if (theBuildingOwnerIncome.at(i) == QString("High")){
                    theRecoveryStartTimeHighIncomeVector[i] = theRecoveryStartTime.at(i);
                    theRecoveryTimeHighIncomeVector[i] = theRecoveryTime.at(i);
                    theHighIncomeBuildingReceivedFundTime[i] = theBuildingReceivedFundTime.at(i);
                    //                qCritical()<<"High income recovery time is set! "<< theRecoveryTime.at(i);
                }
            }
            //            qCritical()<<"theRecoveryTime"<<theRecoveryTime;
            //            qCritical()<<"theRecoveryStartTime"<<theRecoveryStartTime;
            //            qCritical()<<"theRecoveryStartTimeLowIncomeVector: "<<theRecoveryStartTimeLowIncomeVector;
            //            qCritical()<<"theRecoveryTimeLowIncomeVector: "<<theRecoveryTimeLowIncomeVector;
            //            qCritical()<<"theRecoveryStartTimeModerateIncomeVector: "<<theRecoveryStartTimeModerateIncomeVector;
            //            qCritical()<<"theRecoveryTimeModerateIncomeVector: "<<theRecoveryTimeModerateIncomeVector;
            //            qCritical()<<"theRecoveryStartTimeHighIncomeVector: "<<theRecoveryStartTimeHighIncomeVector;
            //            qCritical()<<"theRecoveryTimeHighIncomeVector: "<<theRecoveryTimeHighIncomeVector;
        }

    } // if timeOfEvent > 0.0

    for (int i = 0; i < indexesToRemove.size(); i++){
        theNotableBuildingsIndexes.removeAt(theNotableBuildingsIndexes.indexOf(indexesToRemove.at(i)));
    }

    theNumberOfPeopleNotDisplaced = theNumberOfPersons - (theNumberOfPeopleInHotels + theNumberOfPersonsGone + theNumberOfPeopleLivingWithTheirFamilies + theNumberOfPeopleSeekingShelter);
    theSnowballFactor = theNumberOfPersonsGone / static_cast<double>(theNumberOfPersons);//((theNumberOfPersons - theNumberOfPeopleNotDisplaced)/static_cast<double>((theNumberOfPersons - theNumberOfPersonsGone)));// qMin((theNumberOfPersons - theNumberOfPeopleNotDisplaced) / static_cast<double>((theNumberOfPersons - theNumberOfPersonsGone)),2.0);
    theResponse->setCurrentValue(theNumberOfPeopleNotDisplaced);

    // -------------------------------------------------------------------------------------------
    // 6 - Create plots
    // -------------------------------------------------------------------------------------------
    // If it has been more than one day since last plot
    timeSinceLastPlot += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    lastEvaluationTime = theCurrentTime->getCurrentValue();
    double maximumTimeBetweenPlots = 7.0;


    if (timeSinceLastPlot >= maximumTimeBetweenPlots || qAbs(timeOfEvent - theCurrentTime->getCurrentValue()) < 0.01){

        if (theOutputLevel == RDomain::Maximum){
            qDebug() << "";
            qDebug() << "-----------------------------------------------------";
            qDebug() << "             " << this->objectName();
            qDebug() << " | " << "Time since event: " << theCurrentTime->getCurrentValue() - timeOfEvent;
            qDebug() << " | " << "Number of dwellings in need of repairs:" << theTotalNumberOfDwellings - theNumberOfHabitableDwellings ;
            qDebug() << " | " << "Number of persons in shelters:" << theNumberOfPeopleSeekingShelter;
            qDebug() << " | " << "Number of persons in hotels:" << theNumberOfPeopleInHotels;
            qDebug() << " | " << "Number of persons living with their families:" << theNumberOfPeopleLivingWithTheirFamilies;
            qDebug() << " | " << "Number of persons permanently displaced:" << theNumberOfPersonsGone;
            qDebug() << "-----------------------------------------------------";
            qDebug() << "";
        }


        timeSinceLastPlot = 0.0;
        timeVector << lastEvaluationTime - timeAnalysisBegan;
        theHousingRecoveryVector << theNumberOfHabitableDwellings;
        theNumberOfUninhabitableDwellingsVector << theTotalNumberOfDwellings - theNumberOfHabitableDwellings;
        theDisplacedPopulationVector << theNumberOfPersons - theNumberOfPeopleNotDisplaced;
        thePersonsWithFamilyVector << theNumberOfPeopleLivingWithTheirFamilies;
        //qCritical()<<"thePersonsWithFamilyVector: "<<thePersonsWithFamilyVector;
        thePersonsInShelterVector << theNumberOfPeopleSeekingShelter;
        thePersonsInRenterHousingVector << theNumberOfPeopleInHotels;
        thePersonsGoneVector << theNumberOfPersonsGone;
        theVoluntaryRelocationVector << theNumberOfPersonsLeavingVoluntarily;
        theEvictionVector << theNumberOfPersonsEvicted;
        theNoFundsVector << theNumberOfPersonsGone - theNumberOfPersonsLeavingVoluntarily - theNumberOfPersonsEvicted;
        //        qCritical()<<"theHouseholdDestination: "<<theHouseholdDestination;

        theSFUninhabitableDwellingsVector << numberOfDetachedUninhabitableDwellings;
        theMFUninhabitableDwellingsVector << numberOfMultifamilyUninhabitableDwellings;
        thePreCodeUninhabitableDwellingsVector << numberOfPreCodeUninhabitableDwellings;
        theLowCodeUninhabitableDwellingsVector << numberOfLowCodeUninhabitableDwellings;
        theModCodeUninhabitableDwellingsVector << numberOfModerateCodeUninhabitableDwellings;
        theHighCodeUninhabitableDwellingsVector << numberOfHighCodeUninhabitableDwellings;
        theRenterUninhabitableDwellingsVector << numberOfRenterOccupiedUninhabitableDwellings;
        theOwnerUninhabitableDwellingsVector << numberOfOwnerOccupiedUninhabitableDwellings;
        thePoorUninhabitableDwellingsVector << numberOfLowHouseholdIncomeUninhabitableDwellings;
        theModerateUninhabitableDwellingsVector << numberOfMediumHouseholdIncomeUninhabitableDwellings;
        theRichUninhabitableDwellingsVector << numberOfHighHouseholdIncomeUninhabitableDwellings;
        theRecentImmigrantUninhabitableDwellingsVector << numberOfRecentImmigrantUninhabitableDwellings;
        theEstablishedImmigrantUninhabitableDwellingsVector << numberOfEstablishedImmigrantUninhabitableDwellings;
        theNonImmigrantUninhabitableDwellingsVector << numberOfCitizenUninhabitableDwellings;
        theHigherEducatedUninhabitableDwellingsVector << numberOfHigherEducatedUninhabitableDwellings;
        theNonHigherEducatedUninhabitableDwellingsVector << numberOfNonHigherEducatedUninhabitableDwellings;
        theElderlyMemberUninhabitableDwellingsVector << numberOfElderlyUninhabitableDwellings;
        theNoElderlyMemberUninhabitableDwellingsVector << numberOfNoElderlyUninhabitableDwellings;
        theChildrenMemberUninhabitableDwellingsVector << numberOfChildrenUninhabitableDwellings;
        theNoChildrenMemberUninhabitableDwellingsVector << numberOfNoChildrenUninhabitableDwellings;
        theMarriedCouplesUninhabitableDwellingsVector << numberOfMarriedCouplesUninhabitableDwellings;
        theSingleParentsUninhabitableDwellingsVector << numberOfSingleParentsUninhabitableDwellings;
        theRecentMoverUninhabitableDwellingsVector << numberOfRecentMoversUninhabitableDwellings;
        theNotRecentMoverUninhabitableDwellingsVector << numberOfNotRecentMoversUninhabitableDwellings;
        theWhiteUninhabitableDwellingsVector << numberOfWhiteUninhabitableDwellings;
        theBlackUninhabitableDwellingsVector << numberOfBlackUninhabitableDwellings;
        theAsianUninhabitableDwellingsVector << numberOfAsianUninhabitableDwellings;
        theHispanicUninhabitableDwellingsVector << numberOfHispanicUninhabitableDwellings;
        theNonHispanicUninhabitableDwellingsVector << numberOfNonHispanicUninhabitableDwellings;
    }

    return 1;
}


void RWorkforceStudyNeighborhoodModel::plotUsableBuildings(double habitableArea)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (buildingRepairsPlot == nullptr){
            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            buildingRepairsPlot = new QCustomPlot();
            QString theString = QString("Total habitable dwellings %1").arg(this->objectName());
            mainWindow->addSubWindow(buildingRepairsPlot, theString, true);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            buildingRepairsPlot->graph(0)->setName("Total habitable dwellings");
            buildingRepairsPlot->xAxis->setLabel("Time (days)");
            buildingRepairsPlot->yAxis->setLabel(theString);
            //buildingRepairsPlot->xAxis->setAutoTicks(false);
            //buildingRepairsPlot->xAxis->setAutoTickLabels(false);
            buildingRepairsPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            buildingRepairsPlot->legend->setFont(legendFont);
            buildingRepairsPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            buildingRepairsPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        buildingRepairsPlot->graph(0)->addData(timeElapsed, habitableArea);
        buildingRepairsPlot->yAxis->setRange(0, theTotalNumberOfDwellings);
        buildingRepairsPlot->xAxis->setRange(0, timeElapsed + 1);
        buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        //buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        //buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}



void RWorkforceStudyNeighborhoodModel::plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (buildingRepairsPlot == nullptr){
            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            buildingRepairsPlot = new QCustomPlot();
            QString theString = QString("Total habitable dwellings %1").arg(this->objectName());
            mainWindow->addSubWindow(buildingRepairsPlot, theString, true);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            buildingRepairsPlot->graph(0)->setName(var1Name);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(1)->setPen(QColor(0, 255, 0, 255));
            buildingRepairsPlot->graph(1)->setName(var2Name);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(2)->setPen(QColor(255, 0, 0, 255));
            buildingRepairsPlot->graph(2)->setName(var3Name);
            buildingRepairsPlot->xAxis->setLabel("Time (days)");
            buildingRepairsPlot->yAxis->setLabel(theString);
            //buildingRepairsPlot->xAxis->setAutoTicks(false);
            //buildingRepairsPlot->xAxis->setAutoTickLabels(false);
            buildingRepairsPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            buildingRepairsPlot->legend->setFont(legendFont);
            buildingRepairsPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            buildingRepairsPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        buildingRepairsPlot->graph(0)->addData(timeElapsed, habitableArea1);
        buildingRepairsPlot->graph(1)->addData(timeElapsed, habitableArea2);
        buildingRepairsPlot->graph(2)->addData(timeElapsed, habitableArea3);
        buildingRepairsPlot->yAxis->setRange(0, theTotalNumberOfDwellings);
        buildingRepairsPlot->xAxis->setRange(0, timeElapsed + 1);
        buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        //buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        //buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}


void RWorkforceStudyNeighborhoodModel::plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (buildingRepairsPlot == nullptr){
            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            buildingRepairsPlot = new QCustomPlot();
            QString theString = QString("Total habitable dwellings %1").arg(this->objectName());
            mainWindow->addSubWindow(buildingRepairsPlot, theString, true);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            buildingRepairsPlot->graph(0)->setName(var1Name);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(1)->setPen(QColor(0, 255, 0, 255));
            buildingRepairsPlot->graph(1)->setName(var2Name);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(2)->setPen(QColor(255, 0, 0, 255));
            buildingRepairsPlot->graph(2)->setName(var3Name);
            buildingRepairsPlot->xAxis->setLabel("Time (days)");
            buildingRepairsPlot->yAxis->setLabel(theString);
            //buildingRepairsPlot->xAxis->setAutoTicks(false);
            //buildingRepairsPlot->xAxis->setAutoTickLabels(false);
            buildingRepairsPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            buildingRepairsPlot->legend->setFont(legendFont);
            buildingRepairsPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            buildingRepairsPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        buildingRepairsPlot->graph(0)->addData(timeElapsed, habitableArea1);
        buildingRepairsPlot->graph(1)->addData(timeElapsed, habitableArea2);
        buildingRepairsPlot->graph(2)->addData(timeElapsed, habitableArea3);
        buildingRepairsPlot->yAxis->setRange(0, 1.0);
        buildingRepairsPlot->xAxis->setRange(0, timeElapsed + 1);
        buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        //buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        //buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}


void RWorkforceStudyNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (displacedPopulationPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            displacedPopulationPlot = new QCustomPlot();
            QString theString = QString("Number of Displaced People %1").arg(this->objectName());
            mainWindow->addSubWindow(displacedPopulationPlot, theString, true);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            displacedPopulationPlot->graph(0)->setPen(QColor(0, 255, 0, 255));
            displacedPopulationPlot->graph(0)->setName(var1Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            displacedPopulationPlot->graph(1)->setPen(QColor(0, 0, 255, 255));
            displacedPopulationPlot->graph(1)->setName(var2Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            displacedPopulationPlot->graph(2)->setPen(QColor(255, 0, 0, 255));
            displacedPopulationPlot->graph(2)->setName(var3Name);
            displacedPopulationPlot->xAxis->setLabel("Time (days)");
            displacedPopulationPlot->yAxis->setLabel(theString);
            //displacedPopulationPlot->xAxis->setAutoTicks(false);
            //displacedPopulationPlot->xAxis->setAutoTickLabels(false);
            displacedPopulationPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            displacedPopulationPlot->legend->setFont(legendFont);
            displacedPopulationPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            displacedPopulationPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        displacedPopulationPlot->graph(0)->addData(timeElapsed, var1);
        displacedPopulationPlot->graph(1)->addData(timeElapsed, var2);
        displacedPopulationPlot->graph(2)->addData(timeElapsed, var3);
        displacedPopulationPlot->yAxis->setRange(0, theNumberOfPersons);
        displacedPopulationPlot->xAxis->setRange(0, timeElapsed + 1);
        displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        //displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        //displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}

// For Chenbo: create a copy of this function and use it to plot the
//             displaced persons at low, moderate, and high income
// Tip: you need to create a new plotter object - ctrl F on the displacedPopulationPlot
//      and see where it is defined, initiated, and so on
void RWorkforceStudyNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name)
{
    // Only plots if the output level variable is Maximum
    if (theOutputLevel >= RObject::Medium) {

        // If the plotter object does not exist (null pointer), create a new object
        if (displacedPopulationPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            displacedPopulationPlot = new QCustomPlot();
            QString theString = QString("Total inhabitable residential building area %1").arg(this->objectName());
            mainWindow->addSubWindow(displacedPopulationPlot, theString, true);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            displacedPopulationPlot->graph(0)->setPen(QColor(0, 255, 0, 255)); // Hover over too see the colour
            displacedPopulationPlot->graph(0)->setName(var1Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            displacedPopulationPlot->graph(1)->setPen(QColor(0, 0, 255, 255)); // Numbers are R-G-B-opacity
            displacedPopulationPlot->graph(1)->setName(var2Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            displacedPopulationPlot->graph(2)->setPen(QColor(255, 0, 0, 255)); // Hover over too see the colour
            displacedPopulationPlot->graph(2)->setName(var3Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, 7));
            displacedPopulationPlot->graph(3)->setPen(QColor(255, 0, 255, 255)); // Hover over too see the colour
            displacedPopulationPlot->graph(3)->setName(var4Name);
            displacedPopulationPlot->xAxis->setLabel("Time (days)");
            displacedPopulationPlot->yAxis->setLabel(theString);
            //displacedPopulationPlot->xAxis->setAutoTicks(false);
            //displacedPopulationPlot->xAxis->setAutoTickLabels(false);
            displacedPopulationPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            displacedPopulationPlot->legend->setFont(legendFont);
            displacedPopulationPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            displacedPopulationPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        // If the plotter object already exists, just update it
        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        displacedPopulationPlot->graph(0)->addData(timeElapsed, var1);
        displacedPopulationPlot->graph(1)->addData(timeElapsed, var2);
        displacedPopulationPlot->graph(2)->addData(timeElapsed, var3);
        displacedPopulationPlot->graph(3)->addData(timeElapsed, var4);
        displacedPopulationPlot->yAxis->setRange(0, theNumberOfPersons);
        displacedPopulationPlot->xAxis->setRange(0, timeElapsed + 1);
        displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg((int) timeElapsed);
        //displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        //displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}

//// Tell that intensity model what we want
//theIntensityModel->setLocation(theBuildingLocation.at(i)); //theBuildingLocation.at(i)
//theIntensityModel->setIntensityMeasure(QString("Sa"));

//// Run the intensity model and get the response
//theIntensityModel->setPeriod(0.3);
//theIntensityModel->evaluateModel();
//double Sa0_3 = theResponseObject->getCurrentValue();

//theIntensityModel->setPeriod(1.0);
//theIntensityModel->evaluateModel();
//double Sa1 = theResponseObject->getCurrentValue();
