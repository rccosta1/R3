#include "RMainNeighborhoodModel.h"
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

RMainNeighborhoodModel::RMainNeighborhoodModel(QObject *parent, QString name)
    : RNeighborhoodModel(parent, name)
{
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

RMainNeighborhoodModel::~RMainNeighborhoodModel()
{
}


int RMainNeighborhoodModel::resetTime()
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

    if (thePGACounter > (theSa0_3Vector.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough Sa values!";
        thePGACounter = 0;
        theSa03Counter = 0;
        theSa1Counter = 0;
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
    theTickVector.clear();
    theLabelVector.clear();
    timeVector.clear();
    theHousingRecoveryVector.clear();
    theDisplacedPopulationVector.clear();
    theDelayInTakingAction.clear();
    theBuildingAvailableFunds.clear();
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


    thePoorPersonsGoneVoluntarilyVector.clear();
    theModeratePersonsGoneVoluntarilyVector.clear();
    theRichPersonsGoneVoluntarilyVector.clear();
    theOwnerPersonsGoneVoluntarilyVector.clear();
    theRenterPersonsGoneVoluntarilyVector.clear();
    theRecentImmigrantPersonsGoneVoluntarilyVector.clear();
    theWhitePersonsGoneVoluntarilyVector.clear();
    theBlackPersonsGoneVoluntarilyVector.clear();
    theAsianPersonsGoneVoluntarilyVector.clear();
    theHispanicPersonsGoneVoluntarilyVector.clear();
    theDisablePersonsGoneVoluntarilyVector.clear();
    theElderlyMemberPersonsGoneVoluntarilyVector.clear();
    theChildrenMemberPersonsGoneVoluntarilyVector.clear();
    theHigherEducatedPersonsGoneVoluntarilyVector.clear();
    theSingleParentsPersonsGoneVoluntarilyVector.clear();

    thePoorPersonsTerminatedVector.clear();
    theModeratePersonsTerminatedVector.clear();
    theRichPersonsTerminatedVector.clear();
    theOwnerPersonsTerminatedVector.clear();
    theRenterPersonsTerminatedVector.clear();
    theRecentImmigrantPersonsTerminatedVector.clear();
    theWhitePersonsTerminatedVector.clear();
    theBlackPersonsTerminatedVector.clear();
    theAsianPersonsTerminatedVector.clear();
    theHispanicPersonsTerminatedVector.clear();
    theDisablePersonsTerminatedVector.clear();
    theElderlyMemberPersonsTerminatedVector.clear();
    theChildrenMemberPersonsTerminatedVector.clear();
    theHigherEducatedPersonsTerminatedVector.clear();
    theSingleParentsPersonsTerminatedVector.clear();

    thePoorPersonsEvictedVector.clear();
    theModeratePersonsEvictedVector.clear();
    theRichPersonsEvictedVector.clear();
    theOwnerPersonsEvictedVector.clear();
    theRenterPersonsEvictedVector.clear();
    theRecentImmigrantPersonsEvictedVector.clear();
    theWhitePersonsEvictedVector.clear();
    theBlackPersonsEvictedVector.clear();
    theAsianPersonsEvictedVector.clear();
    theHispanicPersonsEvictedVector.clear();
    theDisablePersonsEvictedVector.clear();
    theElderlyMemberPersonsEvictedVector.clear();
    theChildrenMemberPersonsEvictedVector.clear();
    theHigherEducatedPersonsEvictedVector.clear();
    theSingleParentsPersonsEvictedVector.clear();

    thePoorPersonsGoneNoFundsVector.clear();
    theModeratePersonsGoneNoFundsVector.clear();
    theRichPersonsGoneNoFundsVector.clear();
    theOwnerPersonsGoneNoFundsVector.clear();
    theRenterPersonsGoneNoFundsVector.clear();
    theRecentImmigrantPersonsGoneNoFundsVector.clear();
    theWhitePersonsGoneNoFundsVector.clear();
    theBlackPersonsGoneNoFundsVector.clear();
    theAsianPersonsGoneNoFundsVector.clear();
    theHispanicPersonsGoneNoFundsVector.clear();
    theDisablePersonsGoneNoFundsVector.clear();
    theElderlyMemberPersonsGoneNoFundsVector.clear();
    theChildrenMemberPersonsGoneNoFundsVector.clear();
    theHigherEducatedPersonsGoneNoFundsVector.clear();
    theSingleParentsPersonsGoneNoFundsVector.clear();


    // --------
    // Chenbo's vectors
    theRecoveryTime.resize(theNumberOfBuildings);
    theRecoveryTime.fill(2555.0);

    // --------
    theNumberOfUninhabitableDwellingsVector.clear();
    theTerminationVector.clear();
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

    theTickVector << 0;
    theLabelVector << QString("%1").arg(0);

    return 0;
}




QObject* RMainNeighborhoodModel::getPreparednessEstimator()
{
    return thePreparednessEstimator;
}

void RMainNeighborhoodModel::setPreparednessEstimator(QObject *value)
{
    thePreparednessEstimator = qobject_cast<RDisasterPreparednessEstimator*>(value);
}

QObject* RMainNeighborhoodModel::getDisplacementEstimator()
{
    return theDisplacementEstimator;
}

void RMainNeighborhoodModel::setDisplacementEstimator(QObject *value)
{
    theDisplacementEstimator = qobject_cast<RPopulationDisplacementEstimator*>(value);
}

QObject* RMainNeighborhoodModel::getMorbidityEstimator()
{
    return theMorbidityEstimator;
}

void RMainNeighborhoodModel::setMorbidityEstimator(QObject *value)
{
    theMorbidityEstimator = qobject_cast<RSutley2016MorbidityEstimator*>(value);
}


QString RMainNeighborhoodModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RMainNeighborhoodModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RMainNeighborhoodModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}

QString RMainNeighborhoodModel::getBuildingInspectorList()
{
    return parameterListToString(theBuildingInspectorsList);
}

void RMainNeighborhoodModel::setBuildingInspectorList(QString value)
{
    theBuildingInspectorsList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getEngineeringFirmList()
{
    return parameterListToString(theEngineeringFirmList);
}

void RMainNeighborhoodModel::setEngineeringFirmList(QString value)
{
    theEngineeringFirmList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getPermittingAuthorityList()
{
    return parameterListToString(theBuildingPermitAuthorityList);
}

void RMainNeighborhoodModel::setPermittingAuthorityList(QString value)
{
    theBuildingPermitAuthorityList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getContractorList()
{
    return parameterListToString(theContractorList);
}

void RMainNeighborhoodModel::setContractorList(QString value)
{
    theContractorList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getMaterialSupplierList()
{
    return parameterListToString(theMaterialSupplierList);
}

void RMainNeighborhoodModel::setMaterialSupplierList(QString value)
{
    theMaterialSupplierList = stringToParameterList(value);
}


QString RMainNeighborhoodModel::getFinancialResourcesSupplierList()
{
    return parameterListToString(theFinancialResourcesSupplierList);
}

void RMainNeighborhoodModel::setFinancialResourcesSupplierList(QString value)
{
    theFinancialResourcesSupplierList = stringToParameterList(value);
}




QVector<int> RMainNeighborhoodModel::getHousingRecoveryVector()
{
    if (1 > 2){
        QString theVariableBeingPlotted = QString("");
        QString theFilename = QString("");
        QFile file(theFilename);
        QVector<QVector<double>> theVectorOfResults;


        QVector<QString> theVectorOfDemographics;
        theVectorOfDemographics << QString("Income") << QString("Tenure") << QString("MaritalStatus") << QString("Immigration") << QString("Race") << QString("Hispanic") << QString("Elderly") << QString("Education");

        // Income
        QVector<QString> theVectorOfIncomeCategories;
        theVectorOfIncomeCategories << QString("Low_Income") << QString("High_Income");

        // Tenure
        QVector<QString> theVectorOfTenureCategories;
        theVectorOfTenureCategories << QString("Renter") << QString("Owner");

        // MaritalStatus
        QVector<QString> theVectorOfMaritalStatusCategories;
        theVectorOfMaritalStatusCategories << QString("Single_Householder") << QString("Married_Couple");

        // Immigration
        QVector<QString> theVectorOfImmigrationCategories;
        theVectorOfImmigrationCategories << QString("Recent_Immigrant") << QString("Established_Immigrant") << QString("Non_Immigrant");

        // Race
        QVector<QString> theVectorOfRaceCategories;
        theVectorOfRaceCategories << QString("White") << QString("Black") << QString("Asian");

        // Hispanic
        QVector<QString> theVectorOfHispanicCategories;
        theVectorOfHispanicCategories << QString("Hispanic") << QString("Non_Hispanic");

        // Elderly
        QVector<QString> theVectorOfElderlyCategories;
        theVectorOfElderlyCategories << QString("Has_Elderly") << QString("No_Elderly");

        // Higher education
        QVector<QString> theVectorOfEducationCategories;
        theVectorOfEducationCategories << QString("Has_Degree") << QString("No_Degree");

        // All
        QVector<QVector<QString>> theMatrixOfCategories;
        theMatrixOfCategories << theVectorOfIncomeCategories << theVectorOfTenureCategories << theVectorOfMaritalStatusCategories << theVectorOfImmigrationCategories << theVectorOfRaceCategories << theVectorOfHispanicCategories << theVectorOfElderlyCategories << theVectorOfEducationCategories;


        for (int i = 0; i < theVectorOfDemographics.size(); i++){

            // Housing recovery
            if (theVectorOfDemographics.at(i) == QString("Income")){
                theVectorOfResults << thePoorUninhabitableDwellingsVector << theRichUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Tenure")){
                theVectorOfResults << theRenterUninhabitableDwellingsVector << theOwnerUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("MaritalStatus")){
                theVectorOfResults << theSingleParentsUninhabitableDwellingsVector << theMarriedCouplesUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Immigration")) {
                theVectorOfResults << theRecentImmigrantUninhabitableDwellingsVector << theEstablishedImmigrantUninhabitableDwellingsVector << theNonImmigrantUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Race")){
                theVectorOfResults << theWhiteUninhabitableDwellingsVector << theBlackUninhabitableDwellingsVector << theAsianUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Hispanic")){
                theVectorOfResults << theHispanicUninhabitableDwellingsVector << theNonHispanicUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Elderly")){
                theVectorOfResults << theElderlyMemberUninhabitableDwellingsVector << theNoElderlyMemberUninhabitableDwellingsVector;
            } else if (theVectorOfDemographics.at(i) == QString("Education")){
                theVectorOfResults << theHigherEducatedUninhabitableDwellingsVector << theNonHigherEducatedUninhabitableDwellingsVector;
            }


            theFilename = QString(QString(this->objectName() + "%1").arg(theSa1Counter) +"_HousingRecoveryBy" + theVectorOfDemographics.at(i) +".txt");
            file.setFileName(theFilename);

            if (file.open(QIODevice::Append)) {
                QTextStream stream(&file);
                for (int j = 0; j < theVectorOfResults.size(); j++){

                    QVector<double> output = theVectorOfResults.at(j);
                    stream << theMatrixOfCategories.at(i).at(j);
                    stream << " , ";
                    for (QVector<double>::iterator iter = output.begin(); iter != output.end(); iter++){
                        stream << *iter << " , ";
                    }
                    stream << endl;
                }
                stream << "" << endl;
            }
            theVectorOfResults.clear();
            file.close();


            // Voluntary relocations
            //            if (theVectorOfDemographics.at(i) == QString("Income")){
            //                theVectorOfResults << thePoorPersonsGoneVoluntarilyVector << theRichPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Tenure")){
            //                theVectorOfResults << theRenterPersonsGoneVoluntarilyVector << theOwnerPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("MaritalStatus")){
            //                theVectorOfResults << theSingleParentsPersonsGoneVoluntarilyVector << theMarriedCouplesPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Immigration")) {
            //                theVectorOfResults << theRecentImmigrantPersonsGoneVoluntarilyVector << theEstablishedImmigrantPersonsGoneVoluntarilyVector << theNonImmigrantPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Race")){
            //                theVectorOfResults << theWhitePersonsGoneVoluntarilyVector << theBlackPersonsGoneVoluntarilyVector << theAsianPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Hispanic")){
            //                theVectorOfResults << theHispanicPersonsGoneVoluntarilyVector << theNonHispanicPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Elderly")){
            //                theVectorOfResults << theElderlyMemberPersonsGoneVoluntarilyVector << theNoElderlyMemberPersonsGoneVoluntarilyVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Education")){
            //                theVectorOfResults << theHigherEducatedPersonsGoneVoluntarilyVector << theNonHigherEducatedPersonsGoneVoluntarilyVector;
            //            }

            theFilename = QString(QString(this->objectName() + "%1").arg(theSa1Counter) + "_VoluntaryMobilityBy" + theVectorOfDemographics.at(i) +".txt");
            file.setFileName(theFilename);

            if (file.open(QIODevice::Append)) {
                QTextStream stream(&file);
                for (int j = 0; j < theVectorOfResults.size(); j++){

                    QVector<double> output = theVectorOfResults.at(j);
                    stream << theMatrixOfCategories.at(i).at(j);
                    stream << " , ";
                    for (QVector<double>::iterator iter = output.begin(); iter != output.end(); iter++){
                        stream << *iter << " , ";
                    }
                    stream << endl;
                }
                stream << "" << endl;
            }
            theVectorOfResults.clear();
            file.close();


            // Evictions
            //            if (theVectorOfDemographics.at(i) == QString("Income")){
            //                theVectorOfResults << thePoorPersonsEvictedVector << theRichPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Tenure")){
            //                theVectorOfResults << theRenterPersonsEvictedVector << theOwnerPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("MaritalStatus")){
            //                theVectorOfResults << theSingleParentsPersonsEvictedVector << theMarriedCouplesPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Immigration")) {
            //                theVectorOfResults << theRecentImmigrantPersonsEvictedVector << theEstablishedImmigrantPersonsEvictedVector << theNonImmigrantPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Race")){
            //                theVectorOfResults << theWhitePersonsEvictedVector << theBlackPersonsEvictedVector << theAsianPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Hispanic")){
            //                theVectorOfResults << theHispanicPersonsEvictedVector << theNonHispanicPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Elderly")){
            //                theVectorOfResults << theElderlyMemberPersonsEvictedVector << theNoElderlyMemberPersonsEvictedVector;
            //            } else if (theVectorOfDemographics.at(i) == QString("Education")){
            //                theVectorOfResults << theHigherEducatedPersonsEvictedVector << theNonHigherEducatedPersonsEvictedVector;
            //            }

            theFilename = QString(QString(this->objectName() + "%1").arg(theSa1Counter) +"_EvictionsBy" + theVectorOfDemographics.at(i) +".txt");
            file.setFileName(theFilename);

            if (file.open(QIODevice::Append)) {
                QTextStream stream(&file);
                for (int j = 0; j < theVectorOfResults.size(); j++){

                    QVector<double> output = theVectorOfResults.at(j);
                    stream << theMatrixOfCategories.at(i).at(j);
                    stream << " , ";
                    for (QVector<double>::iterator iter = output.begin(); iter != output.end(); iter++){
                        stream << *iter << " , ";
                    }
                    stream << endl;
                }
                stream << "" << endl;
            }
            theVectorOfResults.clear();
            file.close();
        }

        //    qCritical() << "Gone:" << thePersonsGoneVector << "";
        //    qCritical() << "Evicted:" << theEvictionVector << "";
        //    qCritical() << "Voluntarily:" << theVoluntaryRelocationVector << "";

    }

    return theHousingRecoveryVector;
}

int RMainNeighborhoodModel::getNumberOfDamageLowIncomeSFBuildings(){
    return theNumberOfDamageLowIncomeSFBuildings;
}

int RMainNeighborhoodModel::getNumberOfDamageModerateIncomeSFBuildings(){
    return theNumberOfDamageModerateIncomeSFBuildings;
}

int RMainNeighborhoodModel::getNumberOfDamageHighIncomeSFBuildings(){
    return theNumberOfDamageHighIncomeSFBuildings;
}


QVector<double> RMainNeighborhoodModel::getRecoveryTimeVector()
{
    return theRecoveryTime;
}


QVector<int> RMainNeighborhoodModel::getDisplacedPopulationVector()
{
    return theDisplacedPopulationVector;
}

QVector<int> RMainNeighborhoodModel::getPopulationWithFamilyVector()
{
    return thePersonsWithFamilyVector;
}

QVector<int> RMainNeighborhoodModel::getPopulationInSheltersVector()
{
    return thePersonsInShelterVector;
}

QVector<int> RMainNeighborhoodModel::getPopulationInRentedHousingVector()
{
    return thePersonsInRenterHousingVector;
}

QVector<int> RMainNeighborhoodModel::getPopulationReplacedVector()
{
    return thePersonsGoneVector;
}

// Voluntary relocation
// *************
QVector<double> RMainNeighborhoodModel::getLowIncomeVoluntaryMobilityVector()
{
    return thePoorPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getModerateIncomeVoluntaryMobilityVector()
{
    return theModeratePersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getHighIncomeVoluntaryMobilityVector()
{
    return theRichPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getOwnerVoluntaryMobilityVector()
{
    return theOwnerPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getRenterVoluntaryMobilityVector()
{
    return theRenterPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getRecentImmigrantVoluntaryMobilityVector()
{
    return theRecentImmigrantPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getWhiteVoluntaryMobilityVector()
{
    return theWhitePersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getBlackVoluntaryMobilityVector()
{
    return theBlackPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getHispanicVoluntaryMobilityVector()
{
    return theHispanicPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getAsianVoluntaryMobilityVector()
{
    return theAsianPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getElderlyMemberVoluntaryMobilityVector()
{
    return theElderlyMemberPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getChildrenMemberVoluntaryMobilityVector()
{
    return theChildrenMemberPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getHigherEducatedVoluntaryMobilityVector()
{
    return theHigherEducatedPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getSingleParentsVoluntaryMobilityVector()
{
    return theSingleParentsPersonsGoneVoluntarilyVector;
}

QVector<double> RMainNeighborhoodModel::getDisableVoluntaryMobilityVector()
{
    return theDisablePersonsGoneVoluntarilyVector;
}
// *******

// Terminations
// *************
QVector<double> RMainNeighborhoodModel::getLowIncomeTerminationVector()
{
    return thePoorPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getModerateIncomeTerminationVector()
{
    return theModeratePersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getHighIncomeTerminationVector()
{
    return theRichPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getOwnerTerminationVector()
{
    return theOwnerPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getRenterTerminationVector()
{
    return theRenterPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getRecentImmigrantTerminationVector()
{
    return theRecentImmigrantPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getWhiteTerminationVector()
{
    return theWhitePersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getBlackTerminationVector()
{
    return theBlackPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getHispanicTerminationVector()
{
    return theHispanicPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getAsianTerminationVector()
{
    return theAsianPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getElderlyMemberTerminationVector()
{
    return theElderlyMemberPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getChildrenMemberTerminationVector()
{
    return theChildrenMemberPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getHigherEducatedTerminationVector()
{
    return theHigherEducatedPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getSingleParentsTerminationVector()
{
    return theSingleParentsPersonsTerminatedVector;
}

QVector<double> RMainNeighborhoodModel::getDisableTerminationVector()
{
    return theDisablePersonsTerminatedVector;
}
// *******


// Evictions
// *************
QVector<double> RMainNeighborhoodModel::getLowIncomeEvictionVector()
{
    return thePoorPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getModerateIncomeEvictionVector()
{
    return theModeratePersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getHighIncomeEvictionVector()
{
    return theRichPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getOwnerEvictionVector()
{
    return theOwnerPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getRenterEvictionVector()
{
    return theRenterPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getRecentImmigrantEvictionVector()
{
    return theRecentImmigrantPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getWhiteEvictionVector()
{
    return theWhitePersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getBlackEvictionVector()
{
    return theBlackPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getHispanicEvictionVector()
{
    return theHispanicPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getAsianEvictionVector()
{
    return theAsianPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getElderlyMemberEvictionVector()
{
    return theElderlyMemberPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getChildrenMemberEvictionVector()
{
    return theChildrenMemberPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getHigherEducatedEvictionVector()
{
    return theHigherEducatedPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getSingleParentsEvictionVector()
{
    return theSingleParentsPersonsEvictedVector;
}

QVector<double> RMainNeighborhoodModel::getDisableEvictionVector()
{
    return theDisablePersonsEvictedVector;
}
// *******


// No funds
// *************
QVector<double> RMainNeighborhoodModel::getLowIncomeNoFundsVector()
{
    return thePoorPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getModerateIncomeNoFundsVector()
{
    return theModeratePersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getHighIncomeNoFundsVector()
{
    return theRichPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getOwnerNoFundsVector()
{
    return theOwnerPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getRenterNoFundsVector()
{
    return theRenterPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getRecentImmigrantNoFundsVector()
{
    return theRecentImmigrantPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getWhiteNoFundsVector()
{
    return theWhitePersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getBlackNoFundsVector()
{
    return theBlackPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getHispanicNoFundsVector()
{
    return theHispanicPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getAsianNoFundsVector()
{
    return theAsianPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getElderlyMemberNoFundsVector()
{
    return theElderlyMemberPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getChildrenMemberNoFundsVector()
{
    return theChildrenMemberPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getHigherEducatedNoFundsVector()
{
    return theHigherEducatedPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getSingleParentsNoFundsVector()
{
    return theSingleParentsPersonsGoneNoFundsVector;
}

QVector<double> RMainNeighborhoodModel::getDisableNoFundsVector()
{
    return theDisablePersonsGoneNoFundsVector;
}
// *******

QVector<double> RMainNeighborhoodModel::getUninhabitableDwellingsVector()
{
    return theNumberOfUninhabitableDwellingsVector;
}

QVector<double> RMainNeighborhoodModel::getVoluntaryRelocationVector()
{
    return theVoluntaryRelocationVector;
}

QVector<double> RMainNeighborhoodModel::getTerminationVector()
{
    return theTerminationVector;
}

QVector<double> RMainNeighborhoodModel::getEvictionVector()
{
    return theEvictionVector;
}

QVector<double> RMainNeighborhoodModel::getNoFundsVector()
{
    return theNoFundsVector;
}

// ---

int RMainNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RMainNeighborhoodModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}

int RMainNeighborhoodModel::getNumberOfPeople()
{
    return theNumberOfPersons;
}

int RMainNeighborhoodModel::getNumberOfDisplacedPersons()
{
    return theNumberOfPersons - theNumberOfPeopleNotDisplaced;
}

int RMainNeighborhoodModel::getNumberOfPermanentlyDisplaced()
{
    return theNumberOfPersonsGone;
}

int RMainNeighborhoodModel::getNumberOfPersonsEvicted()
{
    return theNumberOfPersonsEvicted;
}

int RMainNeighborhoodModel::getNumberOfPersonsTerminated()
{
    return theNumberOfPersonsTerminated;
}

int RMainNeighborhoodModel::getNumberOfPersonsLeavingVoluntarily()
{
    return theNumberOfPersonsLeavingVoluntarily;
}

int RMainNeighborhoodModel::getNumberOfPersonsInHotels()
{
    return theNumberOfPeopleInHotels;
}

int RMainNeighborhoodModel::getNumberOfPersonsLivingWithFamily()
{
    return theNumberOfPeopleLivingWithTheirFamilies;
}

int RMainNeighborhoodModel::getNumberOfPersonsInShelters()
{
    return theNumberOfPeopleSeekingShelter;
}

double RMainNeighborhoodModel::getTotalTimeDisplaced()
{
    return theTotalTimeDisplaced;
}

void RMainNeighborhoodModel::getRequests()
{

}

QVector<RLocation*> RMainNeighborhoodModel::getBuildingLocations()
{
    if (theBuildingLocation.size() == 0){
        theBuildingLocation = theHouseholdPortfolio->getBuildingLocationVector();
    }
    return theBuildingLocation;
}


int RMainNeighborhoodModel::evaluateModel()
{
    bool considerEviction = false;
    bool considerVoluntaryMobility = true;

    if (theAverageDamageState < 2){
        //considerEviction = false;
        //considerVoluntaryMobility = false;
    }

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
        theHouseholdPortfolio->instantiateHouseholds();

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
            theTotalBuildingArea += theBuildingArea.at(i);

            QVector<QString> auxDestination;

            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
                theNumberOfPersons += theHouseholdSize.at(i).at(j);
                auxDestination << QString("NotDisplaced");
            }

            theHouseholdDestination << auxDestination;
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
        theRecoveryTime.fill(2555.0);

        alreadyRequestedFinancing.resize(theNumberOfBuildings);
        alreadyRequestedFinancing.fill(false);

        alreadyRequestedEngineers.resize(theNumberOfBuildings);
        alreadyRequestedEngineers.fill(false);

        alreadyRequestedMaterials.resize(theNumberOfBuildings);
        alreadyRequestedMaterials.fill(false);

        alreadyRequestedNegotiation.resize(theNumberOfBuildings);
        alreadyRequestedNegotiation.fill(false);
    } // if

    // --------------------------------------------------------------------------------------
    // 2 - Evaluate the availability of water and power to the neighborhood
    // --------------------------------------------------------------------------------------
    // Checking responses coming to this model and the availability of utilities
    theAvailabilityOfWater = false;
    theAvailabilityOfPower = false;

    // Go over all the responses getting to this object and take actions accordingly
    // Check if power is available in the residential the (i.e., not at the residential building)
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        if (theResponseComingIntoThisModel){

            // Check for availability of power
            // Power is false if the response > 0 because the response means 'time until repairs are finished'
            if (theResponseComingIntoThisModel->getCurrentValue() <= 0){
                theAvailabilityOfPower = true;

                // qCritical() << "Power received:" << theResponseComingIntoThisModel->getCurrentValue() << "Power needs:" << powerNeededInThisCT;
            } else {
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "does not have electrical power available from" << theResponseComingIntoThisModel->getModel()->objectName()  << "days. Expected restoration in"  << theResponseComingIntoThisModel->getCurrentValue() << "";
                }
            }
        }
    }

    // Check if water is available in the residential the (i.e., not at the residential building)
    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel){

            // Check for availability of water
            // Water is false if the response > 0 because the response means 'time until repairs are finished'
            if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
                theAvailabilityOfWater = true;

            } else {
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "does not have water available from" << theResponseComingIntoThisModel->getModel()->objectName()  << "days. Expected restoration in"  << theResponseComingIntoThisModel->getCurrentValue() << "";
                }
            }
        }
    }

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
    // 3 - Handle packets delivered on this time step
    // --------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the buildings in this neighborhood have an object to request inspectors from
    for (int i = 0; i < theBuildingInspectorsList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theBuildingInspectorsList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theBuildingInspectorsList.at(i);
        }
    }


    // Check if the buildings in this neighborhood have an object to request engineering assessments from
    for (int i = 0; i < theEngineeringFirmList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theEngineeringFirmList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theEngineeringFirmList.at(i);
        }
    }


    // Check if the buildings in this neighborhood have an object to request permits from
    for (int i = 0; i < theBuildingPermitAuthorityList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theBuildingPermitAuthorityList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theBuildingPermitAuthorityList.at(i);
        }
    }


    // Check if the buildings in this neighborhood have an object to request workers from
    for (int i = 0; i < theContractorList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theContractorList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theContractorList.at(i);
        }
    }


    // Check if the buildings in this neighborhood have an object to request construction materials from
    for (int i = 0; i < theMaterialSupplierList.size(); i++){

        // Cast the the as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theMaterialSupplierList.at(i));
        if (theResponseComingToThisModel){
            thePacketList << theMaterialSupplierList.at(i);
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


    // Extract contents from the packets received
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {

        // Get hold of what is the content of this packet
        QString packetType = packetsCurrentlyOnboard.at(i)->getPacketContentsType();
        QVector<int> packetOwnerID = packetsCurrentlyOnboard.at(i)->getPacketOwner();// size of packetOwnerID equals the number of buildings
        QVector<double> contentsReceived = packetsCurrentlyOnboard.at(i)->getPacketContents();

        for (int j = 0; j < contentsReceived.size(); j++){
            int packetOwnerI = packetOwnerID.at(j);
            double amountReceived = contentsReceived.at(j);

            // If this packet contains inspectors
            if (packetType == QString("Inspector")){
                if (amountReceived > 0){
                    theInspectionState[packetOwnerI] = true;
                    if (theOutputLevel == RDomain::Maximum){
                        //qDebug() << this->objectName() << "unit" << packetOwnerI  << "has been inspected after" << theCurrentTime->getCurrentValue() - timeOfEvent << ". Immediate loss = " << theImmediateEconomicLoss.at(packetOwnerI);
                        //qCritical() << this->objectName() << "unit" << packetOwnerI  << "has been inspected after" << theCurrentTime->getCurrentValue() - timeOfEvent << ". Immediate loss = " << theImmediateEconomicLoss.at(packetOwnerI);
                    }
                }
            }

            // If this packet contains Money
            else if (packetType.contains(QString("Money"))){
                if (amountReceived > 0){
                    theBuildingAvailableFunds[packetOwnerI] += amountReceived;
                    if (theOutputLevel == RDomain::Maximum){
                        if (theBuildingOccupancyClass.at(packetOwnerI) == QString("RES1")){
                            //qDebug() << this->objectName() << theBuildingOwnerIncome.at(packetOwnerI) << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)   <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                            //qCritical() << this->objectName() << theBuildingOwnerIncome.at(packetOwnerI) << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)   <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                        }
                    }
                }
            }

            // If this packet contains workforce
            else if (packetType == QString("Negotiation")){
                if (amountReceived > 0){
                    theBiddingProcess[packetOwnerI] = true;
                    if (theOutputLevel == RDomain::Maximum){
                        //qDebug() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                        //qCritical() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                    }
                }
            }

            // If this packet contains workforce
            else if (packetType == QString("Allocation")){
                if (amountReceived > 0){
                    theNumberOfWorkersAvailable[packetOwnerI] += static_cast<int>(amountReceived);
                    if (theOutputLevel == RDomain::Maximum){
                        // qDebug() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                        //qCritical() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                    }
                }
            }

            // If this packet contains Materials
            else if (packetType.contains(QString("ConstructionMaterials"))){
                if (amountReceived > 0){
                    theAvailableMaterials[packetOwnerI] += amountReceived;
                    if (theOutputLevel == RDomain::Maximum){
                        //qDebug() << this->objectName() << "unit" << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "after" << theCurrentTime->getCurrentValue() - timeOfEvent;
                        //qCritical() << this->objectName() << "unit" << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName() << "after" << theCurrentTime->getCurrentValue() - timeOfEvent;
                    }
                }
            }

            // If this packet contains engineering evaluation
            else if (packetType.contains(QString("Assessment"))){
                if (amountReceived > 0){
                    theEngineeringAssessment[packetOwnerI] = true;
                    if (theOutputLevel == RDomain::Maximum){
                        //qDebug() << this->objectName() << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                        //qCritical() << this->objectName() << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                    }
                }
            }

            // If this packet contains permits
            else if (packetType.contains(QString("Permit"))){
                if (amountReceived > 0){
                    theBuildingPermitStatus[packetOwnerI] = true;
                    if (theOutputLevel == RDomain::Maximum){
                        //  qDebug() << this->objectName() << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                        //qCritical() << this->objectName() << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)  <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
                    }
                }
            }
        }
    }

    // Delete the packets whose final destinations was this model
    this->handlePacketsThatReachedFinalDestination();


    // --------------------------------------------------------------------------------------
    // 4 - Calculate everything that happens at the time of the earthquake (t = t_0)
    // --------------------------------------------------------------------------------------
    // Immediate losses
    // Considering only one earthquake in this analysis - think about aftershocks later
    if (timeOfEvent == 0.0){

        // Create the request for building inspection which we will add the damaged buildings to
        QString inspectionRequestName = QString(this->objectName() + "InspectionRequest%1").arg(numberOfInspectionRequestsCreatedHere++);
        theDomain->createObject(inspectionRequestName, "RRequest");
        RRequest *theInspectionRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
        theInspectionRequest->setModel(this);
        theInspectionRequest->setContentType(QString("Inspector"));

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

//                        QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
//                        double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

//                        QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
//                        double Sa1 =  theSa1Vector.at(theSa1Counter);

            //            // ***********
                        QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
                        double Sa0_3 = theMultiplier * theSa0_3Vector.at(1);

                        QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
                        double Sa1 =  theMultiplier * theSa1Vector.at(1);


            //            Sa0_3 = 1;
            //            Sa1 = 1;
            // ***********


            // Give the building model the properties of this building
            theBuildingDamageModel->setBuildingType(theBuildingType.at(i));
            theBuildingDamageModel->setCodeDesignLevel(theBuildingCodeLevel.at(i));
            theBuildingDamageModel->setOccupancyClass(theBuildingOccupancyClass.at(i));
            theBuildingDamageModel->setSa1(Sa1);
            theBuildingDamageModel->setSa2(Sa0_3);
            theBuildingDamageModel->evaluateModel();

            // Get the damage responses
            theBuildingStrDamageState[i] = theBuildingDamageModel->getStrDS();
            theBuildingNSDDamageState[i] = theBuildingDamageModel->getNSDDS();
            theBuildingNSADamageState[i] = theBuildingDamageModel->getNSADS();
            theAverageDamageState += theBuildingStrDamageState[i];

            // Considering only one earthquake in this analysis - think about aftershocks later
            if (theBuildingStrDamageState[i] > 0.0 || theBuildingNSDDamageState.at(i) > 0.0 || theBuildingNSADamageState.at(i) > 0.0){

                // Get hold of the damaged buildings
                if (theBuildingStrDamageState[i] > 2){
                    theDamagedBuildingsIndexes << i;
                }

                // If there was damage to any building set the time of the event as the current time
                timeOfEvent = theCurrentTime->getCurrentValue();

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

                // Inspection requests
                theInspectionRequest->setRequesterIDList(i);
                theInspectionRequest->setRequestedAmountList(1.0);

                // Calculate the number of damaged single-family housing of each income group
                if (theBuildingStrDamageState[i] > 2 && theBuildingOccupancyClass.at(i) == QString("RES1")){
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

            } // if DS > 0

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

            // Threshold prob of being evicted
            theBuildingThresholdProbabilityToBeEvicted[i] = rand() / static_cast<double>(RAND_MAX);

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

        // Delete the inspection request if no building needs inspection
        if (theInspectionRequest->getRequestedAmountList().size() == 0){
            theDomain->deleteObject(inspectionRequestName);
        }
    } // if timeOfEvent == 0.0

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
    totalHabitableArea = theTotalBuildingArea;
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

    double numberOfDisplacedPeople = 0.0;
    double numberOfLowHouseholdIncomeDisplacedPeople = 0.0;
    double numberOfMediumHouseholdIncomeDisplacedPeople = 0.0;
    double numberOfHighHouseholdIncomeDisplacedPeople = 0.0;
    double numberOfInsuredDisplacedPeople = 0.0;
    double numberOfUninsuredDisplacedPeople = 0.0;
    double numberOfRecentImmigrantDisplacedPeople = 0.0;
    double numberOfEstablishedImmigrantDisplacedPeople = 0.0;
    double numberOfCitizenDisplacedPeople = 0.0;
    double numberOfPreCodeDisplacedPeople = 0.0;
    double numberOfLowCodeDisplacedPeople = 0.0;
    double numberOfModerateCodeDisplacedPeople = 0.0;
    double numberOfHighCodeDisplacedPeople = 0.0;
    double numberOfDetachedDisplacedPeople = 0.0;
    double numberOfMultifamilyDisplacedPeople = 0.0;
    double numberOfOwnerOccupiedDisplacedPeople = 0.0;
    double numberOfRenterOccupiedDisplacedPeople = 0.0;
    double numberOfHigherEducatedDisplacedPeople = 0.0;
    double numberOfMarriedCouplesDisplacedPeople = 0.0;
    double numberOfSingleParentsDisplacedPeople = 0.0;
    double numberOfRecentMoversDisplacedPeople = 0.0;
    double numberOfNotRecentMoversDisplacedPeople = 0.0;
    double numberOfNonHigherEducatedDisplacedPeople = 0.0;
    double numberOfElderlyDisplacedPeople = 0.0;
    double numberOfNoElderlyDisplacedPeople = 0.0;
    double numberOfWithDisabilityDisplacedPeople = 0.0;
    double numberOfNonDisabilityDisplacedPeople = 0.0;
    double numberOfWhiteDisplacedPeople = 0.0;
    double numberOfBlackDisplacedPeople = 0.0;
    double numberOfAsianDisplacedPeople = 0.0;


    double numberOfLowHouseholdIncomePersonsGoneVoluntarily = 0.0;
    double numberOfMediumHouseholdIncomePersonsGoneVoluntarily = 0.0;
    double numberOfHighHouseholdIncomePersonsGoneVoluntarily = 0.0;
    double numberOfRecentImmigrantPersonsGoneVoluntarily = 0.0;
    double numberOfOwnerOccupiedPersonsGoneVoluntarily = 0.0;
    double numberOfRenterOccupiedPersonsGoneVoluntarily = 0.0;
    double numberOfHigherEducatedPersonsGoneVoluntarily = 0.0;
    double numberOfSingleParentsPersonsGoneVoluntarily = 0.0;
    double numberOfElderlyPersonsGoneVoluntarily = 0.0;
    double numberOfChildrenPersonsGoneVoluntarily = 0.0;
    double numberOfWhitePersonsGoneVoluntarily = 0.0;
    double numberOfBlackPersonsGoneVoluntarily = 0.0;
    double numberOfAsianPersonsGoneVoluntarily = 0.0;
    double numberOfHispanicPersonsGoneVoluntarily = 0.0;
    double numberOfDisablePersonsGoneVoluntarily = 0.0;

    double numberOfLowHouseholdIncomePersonsTerminated = 0.0;
    double numberOfMediumHouseholdIncomePersonsTerminated = 0.0;
    double numberOfHighHouseholdIncomePersonsTerminated = 0.0;
    double numberOfRecentImmigrantPersonsTerminated = 0.0;
    double numberOfOwnerOccupiedPersonsTerminated = 0.0;
    double numberOfRenterOccupiedPersonsTerminated = 0.0;
    double numberOfHigherEducatedPersonsTerminated = 0.0;
    double numberOfSingleParentsPersonsTerminated = 0.0;
    double numberOfElderlyPersonsTerminated = 0.0;
    double numberOfChildrenPersonsTerminated = 0.0;
    double numberOfWhitePersonsTerminated = 0.0;
    double numberOfBlackPersonsTerminated = 0.0;
    double numberOfAsianPersonsTerminated = 0.0;
    double numberOfHispanicPersonsTerminated = 0.0;
    double numberOfDisablePersonsTerminated = 0.0;

    double numberOfLowHouseholdIncomePersonsEvicted = 0.0;
    double numberOfMediumHouseholdIncomePersonsEvicted = 0.0;
    double numberOfHighHouseholdIncomePersonsEvicted = 0.0;
    double numberOfRecentImmigrantPersonsEvicted = 0.0;
    double numberOfOwnerOccupiedPersonsEvicted = 0.0;
    double numberOfRenterOccupiedPersonsEvicted = 0.0;
    double numberOfHigherEducatedPersonsEvicted = 0.0;
    double numberOfSingleParentsPersonsEvicted = 0.0;
    double numberOfElderlyPersonsEvicted = 0.0;
    double numberOfChildrenPersonsEvicted = 0.0;
    double numberOfWhitePersonsEvicted = 0.0;
    double numberOfBlackPersonsEvicted = 0.0;
    double numberOfAsianPersonsEvicted = 0.0;
    double numberOfHispanicPersonsEvicted = 0.0;
    double numberOfDisablePersonsEvicted = 0.0;

    double numberOfLowHouseholdIncomePersonsGoneNoFunds = 0.0;
    double numberOfMediumHouseholdIncomePersonsGoneNoFunds = 0.0;
    double numberOfHighHouseholdIncomePersonsGoneNoFunds = 0.0;
    double numberOfRecentImmigrantPersonsGoneNoFunds = 0.0;
    double numberOfOwnerOccupiedPersonsGoneNoFunds = 0.0;
    double numberOfRenterOccupiedPersonsGoneNoFunds = 0.0;
    double numberOfHigherEducatedPersonsGoneNoFunds = 0.0;
    double numberOfSingleParentsPersonsGoneNoFunds = 0.0;
    double numberOfElderlyPersonsGoneNoFunds = 0.0;
    double numberOfChildrenPersonsGoneNoFunds = 0.0;
    double numberOfWhitePersonsGoneNoFunds = 0.0;
    double numberOfBlackPersonsGoneNoFunds = 0.0;
    double numberOfAsianPersonsGoneNoFunds = 0.0;
    double numberOfHispanicPersonsGoneNoFunds = 0.0;
    double numberOfDisablePersonsGoneNoFunds = 0.0;




    theNumberOfPeopleSeekingShelter = 0;
    theNumberOfPeopleInHotels = 0;
    theNumberOfPersonsGone = 0;
    theNumberOfPersonsEvicted = 0;
    theNumberOfPersonsTerminated = 0;
    theNumberOfPersonsLeavingVoluntarily = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;


    // Go over everything that will be evaluated on each 'delta t'
    if (timeOfEvent > 0.0) {

        // Handle all requests that need to be made on this time step
        QVector<int> theInsuranceRequestID;
        QVector<int> thePrivateRequestID;
        QVector<int> thePublicRequestID;
        QVector<int> theGrantRequestID;
        QVector<int> theWoodRequestID;
        QVector<int> theConcreteRequestID;
        QVector<int> theContractorNegotiationRequestID;
        QVector<int> theEngineeringAssessmentRequestID;
        QVector<int> theBuildingPermitStatusRequestID;
        QVector<int> theSFContractorAllocationRequestID;
        QVector<int> theMFContractorAllocationRequestID;

        // ***********************************************
        // Voluntary relocation from undamaged buildings
        // ***********************************************
        if (considerVoluntaryMobility == true){
            for (int i = 0; i < theNumberOfBuildings; i++){
                // If repairs are not needed
                if (theBuildingStrDamageState.at(i) <= 2 && theBuildingRepairTime.at(i) <= 0.0){
                    theBuildingTimeUnderRepairs[i] = theBuildingRepairTime.at(i) + 1.0; // i.e., time under repair > time needed to repair
                    theRecoveryTime[i] = 0.0;

                    // Loop over all households in the buildings to check if voluntary displacements will happen
                    for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){

                        // If the building is not repaired nor the occupants are gone yet
                        if (theHouseholdDestination.at(i).at(j).contains(QString("Gone")) == false && theHouseholdDestination.at(i).at(j).contains(QString("Recovery")) == false){

                            theDisplacementEstimator->setDS(theBuildingStrDamageState.at(i));
                            theDisplacementEstimator->setBuildingOccupancy(theBuildingOccupancyClass.at(i));
                            theDisplacementEstimator->setInsuranceStatus(theBuildingInsuranceStatus.at(i));
                            theDisplacementEstimator->setTenure(theHouseholdTenureStatus.at(i).at(j));
                            theDisplacementEstimator->setUnsafeHousingStatus(theHouseholdUnsafeHousingVector.at(i).at(j));
                            theDisplacementEstimator->setUnaffordableHousingStatus(theHouseholdUnaffordableHousingVector.at(i).at(j));
                            theDisplacementEstimator->setOvercrowdedHousingStatus(theHouseholdOvercrowdedHousingVector.at(i).at(j));
                            theDisplacementEstimator->setUnsatisfactionWithNeighboorhoodStatus(theHouseholdUnsatisfiedNeighborhood.at(i).at(j));
                            theDisplacementEstimator->setTimeSinceTheEvent(theCurrentTime->getCurrentValue() - timeOfEvent);
                            theDisplacementEstimator->setSnowballFactor(theSnowballFactor);
                            theDisplacementEstimator->setRepairCost(theImmediateEconomicLoss.at(i));
                            theDisplacementEstimator->setBuildingValue(theBuildingValue.at(i));
                            theDisplacementEstimator->setSellingFee(0.05 * theBuildingValue.at(i));
                            theDisplacementEstimator->setLandValue(theBuildingLandValue.at(i));
                            theDisplacementEstimator->setAverageRent(theBuildingAverageRent);

                            // check if they will willingly abandon the building
                            double thePoissonProbOfLeaving = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 180.0);
                            QString theDecisionOfOccupants = theDisplacementEstimator->evaluateDecisionOfOccupants();

                            if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied") && theDecisionOfOccupants == QString("Move") && thePoissonProbOfLeaving > theBuildingThresholdTimeToMakeADecision.at(i)){
                                theHouseholdDestination[i][j] = QString("GoneVoluntarily");
                            }
                            else if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied") && theDecisionOfOccupants == QString("Move") && thePoissonProbOfLeaving > theBuildingThresholdTimeToMakeADecision.at(i)) {
                                theHouseholdDestination[i][j] = QString("GoneVoluntarily");
                            }
                        } // not displaced
                    } // j
                } // if no damage
            }// i
        } // if consider


        // ********************************
        // Evictions from rented buildings
        // ********************************
        if (considerEviction == true) {
            for (int i = 0; i < theNumberOfBuildings; i++){

                // Loop over all households in the buildings to check if voluntary displacements will happen
                for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){

                    // Check if the landlord wants to sell the building and in consequence displace the tenant
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied") && theHouseholdDestination.at(i).at(j).contains(QString("Gone")) == false && theHouseholdDestination.at(i).at(j).contains(QString("Recovery")) == false){

                        // First, consider evictions due to owner wanting to sell
                        theDisplacementEstimator->setUnsafeHousingStatus(theHouseholdUnsafeHousingVector.at(i).at(j));
                        theDisplacementEstimator->setUnaffordableHousingStatus(theHouseholdUnaffordableHousingVector.at(i).at(j));
                        theDisplacementEstimator->setOvercrowdedHousingStatus(theHouseholdOvercrowdedHousingVector.at(i).at(j));
                        theDisplacementEstimator->setUnsatisfactionWithNeighboorhoodStatus(theHouseholdUnsatisfiedNeighborhood.at(i).at(j));
                        theDisplacementEstimator->setTimeSinceTheEvent(theCurrentTime->getCurrentValue() - timeOfEvent);
                        theDisplacementEstimator->setSnowballFactor(theSnowballFactor);
                        theDisplacementEstimator->setRepairCost(theImmediateEconomicLoss.at(i));
                        theDisplacementEstimator->setBuildingValue(theBuildingValue.at(i));
                        theDisplacementEstimator->setSellingFee(0.05 * theBuildingValue.at(i));
                        theDisplacementEstimator->setLandValue(theBuildingLandValue.at(i));
                        theDisplacementEstimator->setAverageRent(theBuildingAverageRent);

                        double thePoissonProbOfSellingRentedBuilding = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 180.0);
                        bool theWillingnessToSellOfTheLandlord = theDisplacementEstimator->evaluateWillingnessToSellOfLandlords();

                        if (theWillingnessToSellOfTheLandlord == true && thePoissonProbOfSellingRentedBuilding > theBuildingThresholdTimeToBeSold.at(i)){
                            //theHouseholdDestination[i][j] = QString("GoneTerminated");
                            RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                            theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(i), theNumberOfWorkersAvailable.at(i));
                            theNumberOfWorkersAvailable[i] = 0.0;
                            theBuildingTimeSold[i] = theCurrentTime->getCurrentValue();
                            //qCritical() << i << " I am being evicted because the landlord cant repair at" << theCurrentTime->getCurrentValue() - timeOfEvent << theImmediateEconomicLoss.at(i);
                        }

                        // Now, consider evictions due to the renter not being able to pay rent
                        // Considering that renters who are short in money before the event (i.e., have used foods stamps last year)
                        // will be pushed beyond their limits by the event and will fall behind in rent payments
                        double thePoissonProbOfBeingEvicted = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 90.0);
                        if (theHouseholdFiancialStrain.at(i).at(j) == true && thePoissonProbOfBeingEvicted > theBuildingThresholdProbabilityToBeEvicted.at(i)){
                            //theHouseholdDestination[i][j] = QString("GoneEvicted");
                            //qCritical() << " I am being evicted because I cant pay rent at" << theCurrentTime->getCurrentValue() - timeOfEvent;
                        }
                    }
                } // for j
            } // for i
        } // if evictions



        // ***************************************************************
        // Loop over all damaged buildings
        // ***************************************************************
        for (int i = 0; i < theDamagedBuildingsIndexes.size(); i++){

            // NOTE: we are only looping over damaged buildings, make sure to properly use the counter 'ii' instead 'i'
            int ii = theDamagedBuildingsIndexes.at(i);
            if (theBuildingTimeUnderRepairs.at(ii) > theBuildingRepairTime.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){
                //qCritical() << "there is something wrong here" << ii << theHouseholdDestination.at(ii).at(0);
            }

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

                // ****************
                // Repair and leave
                // ****************
                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){
                    // If this building was marked to repair and sell, after repairs are completed the owner will leave
                    if (theHouseholdDestination.at(ii).at(j) == QString("RecoveryUnderWayAndSellAfter")){
                        theHouseholdDestination[ii][j] = QString("GoneVoluntarily");
                    }
                }
            }

            // If repairs are progressing normally
            else if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){

                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){
                    // ***************************
                    // Building started repairing
                    // ***************************
                    if (theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == false){
                        theHouseholdDestination[ii][j] = QString("RecoveryUnderWay");
                    }
                }

                theBuildingTimeUnderRepairs[ii] += static_cast<double>(theCurrentTime->getCurrentValue() - lastEvaluationTime);

                // If repairs are going on, the household has not left, and the building can be reoccupied
                if (theBuildingReoccupancyTime.at(ii) <= theBuildingTimeUnderRepairs.at(ii)){
                    for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){

                        // ****************
                        // Building finished repairs
                        // ****************
                        if (theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == false){
                            theHouseholdDestination[ii][j] = QString("RecoveryCompleted");
                        }
                    }
                }
            }


            // Lastly, the building is not repaired and some resource is still missing, request it
            if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && (theAvailableMaterials.at(ii) <= 0.0 || theBuildingAvailableFunds.at(ii) <= 0.0 || theNumberOfWorkersAvailable.at(ii) <= 0)){
                if (theCurrentTime->getCurrentValue() - timeOfEvent > 600.0){
                    //qCritical() << ii << theBuildingType.at(ii) << theBuildingOccupancyClass.at(ii) << "Materials:" << theAvailableMaterials.at(ii) << "Funds:" << theBuildingAvailableFunds.at(ii) << "Workers:" << theNumberOfWorkersAvailable.at(ii) << "Engineering Assessment:" << theEngineeringAssessment.at(ii) << "Permit:" << theBuildingPermitStatus.at(ii) << "Bidding:" << theBiddingProcess.at(ii);
                }

                // ***************************************************************
                // Loop over all dwellings
                // ***************************************************************
                // Loop over all households in the buildings
                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){

                    double theDeductible = 0.15 * theBuildingValue.at(ii);// theImmediateEconomicLoss.at(ii);
                    // qCritical() << this->objectName() << "The deductible is" << theDeductible;

                    // If this is a damaged multi-family building
                    if (theBuildingOccupancyClass.at(ii).contains(QString("RES3")) && alreadyRequestedFinancing.at(ii) == false){

                        // If this a multi-family building assume money will come from private loan
                        thePrivateRequestID << ii;
                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100.0; // Ask me if you have questions about this line
                        alreadyRequestedFinancing[ii] = true; // Set this flag to true so the building will not ask for financing again


                        // If this is a single-family building
                    } else if (theBuildingOccupancyClass.at(ii) == QString("RES1") && alreadyRequestedFinancing.at(ii) == false){

                        QString theCity = QString("SanFrancisco");
                        bool skipFunding = true;

                        if (skipFunding == true){
                            qCritical() << "Funds for everyone!!";
                            thePublicRequestID << ii;
                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                            // If this analysis is for San Francisco, use this financing decision model
                        } else if (theCity == QString("SanFrancisco")){

                            // Single family buildings
                            if (theBuildingOccupancyClass.at(ii) == QString("RES1")){

                                // If the building is owner occupied
                                if (theHouseholdTenureStatus.at(ii).at(j) == QString("OwnerOccupied")){

                                    // If high income
                                    if (theBuildingOwnerIncome.at(ii) == QString("High")){

                                        // If insured and can pay deductible out-of-pocket
                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
                                            theInsuranceRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                                            // If not insured but no mortgage, get a private loan
                                        } else if (theHouseholdMortgage.at(ii).at(j) == false) {
                                            thePrivateRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                            // If not insured or cannot pay deductible and has a mortgage, get a loan and use savings but only if its enough to pay for repairs
                                        } else if (theImmediateEconomicLoss.at(ii) < 200000.0 + theHomeownerIncomeValue.at(ii)) {
                                            thePublicRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;
                                        }
                                    }
                                    else if (theBuildingOwnerIncome.at(ii) == QString("Moderate")){
                                        double rngGetCDBGDRMoney = rand()/static_cast<double>(RAND_MAX);

                                        // If insured and can pay deductible out-of-pocket
                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
                                            theInsuranceRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                                            // If not insured but no mortgage, get a private loan
                                        } else if (theHouseholdMortgage.at(ii).at(j) == false){
                                            thePrivateRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                            // If not insured or cannot pay deductible and has a mortgage, get a loan and use savings but only if its enough to pay for repairs
                                            // Note: the maximum amount of loan is less than for high income
                                        } else if (theImmediateEconomicLoss.at(ii) < 150000.0 + theHomeownerIncomeValue.at(ii)) {
                                            thePublicRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (rngGetCDBGDRMoney < 0.25){
                                            theGrantRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
                                        }
                                    }

                                    // If LMI, get a small grant that may not be enough

                                    else if (theBuildingOwnerIncome.at(ii) == QString("Low")){

                                        // If insured and can pay deductible out-of-pocket
                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
                                            theInsuranceRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                                            // If not insured but no mortgage, get a private loan
                                        }
                                        // Note: the maximum amount of loan is less than for high and moderate income
                                        else if (theImmediateEconomicLoss.at(ii) < 50000.0 + theHomeownerIncomeValue.at(ii)){
                                            thePublicRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;
                                        }
                                        // If not wait for a grant from CDBG-DR
                                        else {
                                            theGrantRequestID << ii;
                                            theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
                                        }
                                    }

                                    // If the building is renter-occupied
                                } else if (theHouseholdTenureStatus.at(ii).at(j) == QString("RenterOccupied")) {

                                    // Assuming that renter-occupied buildings will depend on private loan
                                    if (theBuildingOwnerIncome.at(ii) == QString("High") && theImmediateEconomicLoss.at(ii) < 200000.0 + theHomeownerIncomeValue.at(ii)) {
                                        thePrivateRequestID << ii;
                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;
                                    }
                                    // Assuming that renter-occupied buildings will depend on private loan
                                    if (theBuildingOwnerIncome.at(ii) == QString("Moderate") && theImmediateEconomicLoss.at(ii) < 100000.0 + theHomeownerIncomeValue.at(ii)) {
                                        thePrivateRequestID << ii;
                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                        // Assume small grants will be given to landlords of LMI homes
                                        // Note, this income is the TENANT income
                                    } else if (theHouseholdIncome.at(ii).at(j) == QString("Low") && theImmediateEconomicLoss.at(ii) < 150000.0 + theHomeownerIncomeValue.at(ii)){
                                        //theGrantRequestID << ii;
                                        //theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
                                    }

                                } else {
                                    qCritical() << this->objectName() << "has a problem with the building tenure.";
                                }


                            } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))) {

                                // Smaller buildings
                                if (theBuildingNumberOfDwellings.at(ii) < 25 && theImmediateEconomicLoss.at(ii) / theBuildingNumberOfDwellings.at(ii) < 50000.0){
                                    // apply for FRM
                                } else if (theBuildingNumberOfDwellings.at(ii) >= 25 && theImmediateEconomicLoss.at(ii) / theBuildingNumberOfDwellings.at(ii) < 50000.0) {
                                    // apply for LRRP but takes longer
                                }
                            }

                            // If the analysis is not for San Francisco use this generic financing algorithm
                        } else if (theCity != QString("SanFrancisco")) {
                            if (theBuildingOwnerIncome.at(ii) == QString("Low")){

                                if (theBuildingInsuranceStatus.at(ii) == true){
                                    // Low EW and insured
                                    if (theImmediateEconomicLoss.at(ii) > theDeductible){
                                        // Insurance was triggered
                                        if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                            thePrivateRequestID << i;
                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                        } // immigrant status

                                    } else {
                                        // Insurance is not triggered
                                        if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                            thePrivateRequestID << i;
                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                        } // immigrant status
                                    }

                                } else if (theBuildingInsuranceStatus.at(ii) == false){
                                    // Low EW and not insured
                                    if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                        thePublicRequestID << i;
                                        theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                    } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                        thePublicRequestID << i;
                                        theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                    } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                        thePrivateRequestID << i;
                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                    } // Immigrant status
                                }// Insurance
                            }// Low economic welfare

                            // For moderate income owners
                            else if (theBuildingOwnerIncome.at(ii) == QString("Moderate")) {

                                if (theBuildingInsuranceStatus.at(ii) == true){
                                    // Moderate EW and insured
                                    if(theImmediateEconomicLoss.at(ii) > theDeductible){

                                        // Insurance was triggered
                                        if (theDeductible < 20000.0 && theHouseholdMortgage.at(ii).at(j) == false){
                                            // Can pay deductible
                                            theInsuranceRequestID << i;
                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                                        } else {
                                            // Cannot pay deductible
                                            if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                                thePublicRequestID << i;
                                                theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                            } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                                thePublicRequestID << i;
                                                theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                            } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                                thePrivateRequestID << i;
                                                theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                            } // Immigrant status
                                        } // Deductible

                                    } else {
                                        // Insurance not triggered
                                        if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                            thePublicRequestID << i;
                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                        } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                            thePrivateRequestID << i;
                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                        } // Immigrant status
                                    } // Loss

                                } else if (theBuildingInsuranceStatus.at(ii) == false){
                                    // Moderate EW and not insured
                                    if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("RecentImmigrant")){
                                        thePublicRequestID << i;
                                        theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                    } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("EstablishedImmigrant")) {
                                        thePublicRequestID << i;
                                        theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

                                    } else if (theBuildingOwnerImmigrationStatus.at(ii).at(j) == QString("NonImmigrant")) {
                                        thePrivateRequestID << i;
                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                    } // Immigrant status
                                } // No insurance
                            }// Moderate economic welfare

                            // For high-income owners
                            else if (theBuildingOwnerIncome.at(ii) == QString("High")){

                                if (theHouseholdMortgage.at(ii).at(j) == false){
                                    // Self-fund repairs
                                    theBuildingAvailableFunds[ii] = theImmediateEconomicLoss.at(ii);

                                } else {
                                    if (theBuildingInsuranceStatus.at(ii) == true && theImmediateEconomicLoss.at(ii) > theDeductible){
                                        // Can pay deductible
                                        theInsuranceRequestID << i;
                                        theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

                                    } else {
                                        // Not insured and with mortgage
                                        thePrivateRequestID << i;
                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

                                    } // Insurance
                                } // Mortgage
                            } // High economic welfare
                        } // SF or not SF
                    } // Multi-family vs Single-family
                    alreadyRequestedFinancing[ii] = true;


                    // ***************************************************************
                    // Evaluate if people are displaced
                    // ***************************************************************
                    // If repairs have not finished yet, nor the household has relocated
                    // For all other households who have not finished repairs nor relocated yet
                    if (theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == false && theHouseholdDestination.at(ii).at(j).contains(QString("Recovery")) == false){

                        // ****************
                        // Displaced, no funds
                        // ****************
                        if (theBuildingStrDamageState.at(ii) > 2 && theBuildingAvailableFunds.at(ii) <= 0.0 && theBuildingOccupancyClass.at(ii) == QString("RES1")){

                            double thePoissonTimeToMakeADecision = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 180.0);
                            // In case the building owner left the neighbourhood, sell the building
                            if (theHouseholdTenureStatus.at(ii).at(0) == QString("OwnerOccupied") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToBeSold.at(ii)){
                                //theHouseholdDestination[ii][j] = QString("GoneNoFunds");
                                RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                                theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                                theNumberOfWorkersAvailable[ii] = 0.0;
                                //qCritical() << ii << "Owner-occupied building being sold. I needed" << theImmediateEconomicLoss.at(ii);// / theBuildingValue.at(ii);
                                theBuildingTimeSold[ii] = theCurrentTime->getCurrentValue();
                            }
                            // In case the building owner cannot repair a renter-occupied building, evict the renter
                            else if (theHouseholdTenureStatus.at(ii).at(0) == QString("RenterOccupied") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToBeSold.at(ii)){
                                //theHouseholdDestination[ii][j] = QString("GoneNoFunds");
                                RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                                theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                                theNumberOfWorkersAvailable[ii] = 0.0;
                                //qCritical() << ii << "Renter-occupied building being sold. I needed" << theImmediateEconomicLoss.at(ii);// / theBuildingValue.at(ii);
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

                                // For everyone else, check if they are displaced
                                // Check if the household is displaced
                                bool theDisplacementStatus = theDisplacementEstimator->evaluateDisplacementStatus();

                                // If the household is displaced now but wasnt before, figure out its immediate destination
                                // Only check this if they are not already displaced somewhere
                                if (theDisplacementStatus == true){

                                    // Check where the household goes temporarily
                                    theHouseholdDestination[ii][j] = theDisplacementEstimator->evaluateShortTermDestination();
                                }
                            } // if not displaced

                            // Tell the displacement status if the household is displaced
                            theDisplacementEstimator->setCurrentLivingConditions(theHouseholdDestination.at(ii).at(j));

                            double thePoissonTimeToMakeADecision = 1.0 - qExp(-(theCurrentTime->getCurrentValue() - timeOfEvent) / 180.0);

                            // *******************************************
                            // Voluntary mobility from damaged buildings
                            // *******************************************
                            if (considerVoluntaryMobility == true){

                                // For everyone who has not been displaced by their tenants or is an owner
                                // check if they will willingly abandon the building
                                theDisplacementEstimator->setDS(theBuildingStrDamageState.at(ii));
                                theDisplacementEstimator->setBuildingOccupancy(theBuildingOccupancyClass.at(ii));
                                theDisplacementEstimator->setInsuranceStatus(theBuildingInsuranceStatus.at(ii));
                                theDisplacementEstimator->setTenure(theHouseholdTenureStatus.at(ii).at(j));
                                theDisplacementEstimator->setUnsafeHousingStatus(theHouseholdUnsafeHousingVector.at(ii).at(j));
                                theDisplacementEstimator->setUnaffordableHousingStatus(theHouseholdUnaffordableHousingVector.at(ii).at(j));
                                theDisplacementEstimator->setOvercrowdedHousingStatus(theHouseholdOvercrowdedHousingVector.at(ii).at(j));
                                theDisplacementEstimator->setUnsatisfactionWithNeighboorhoodStatus(theHouseholdUnsatisfiedNeighborhood.at(ii).at(j));
                                theDisplacementEstimator->setTimeSinceTheEvent(theCurrentTime->getCurrentValue() - timeOfEvent);
                                theDisplacementEstimator->setSnowballFactor(theSnowballFactor);
                                theDisplacementEstimator->setRepairCost(theImmediateEconomicLoss.at(ii));
                                theDisplacementEstimator->setBuildingValue(theBuildingValue.at(ii));
                                theDisplacementEstimator->setSellingFee(0.05 * theBuildingValue.at(ii));
                                theDisplacementEstimator->setLandValue(theBuildingLandValue.at(ii));
                                theDisplacementEstimator->setAverageRent(theBuildingAverageRent);

                                QString theDecisionOfOccupants = theDisplacementEstimator->evaluateDecisionOfOccupants();

                                if (theHouseholdTenureStatus.at(ii).at(j) == QString("OwnerOccupied")){
                                    if (theDecisionOfOccupants == QString("RepairAndSell") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToMakeADecision.at(ii)){
                                        //qCritical() << "Building" << ii << "will be repaired and sold.";
                                        //qCritical() << "";
                                        // Define the household status as RecoveryUnderWayBecause it will eventually be repaired
                                        // A little ad hoc solution
                                        theHouseholdDestination[ii][j] = QString("RecoveryUnderWayAndSellAfter");
                                    }
                                    else if (theDecisionOfOccupants == QString("RepairAndReoccupy") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToMakeADecision.at(ii)){
                                        //qCritical() << "Building" << ii << "will be repaired and reoccupied.";
                                        //qCritical() << "";
                                        // Define the household status as RecoveryUnderWayBecause it will eventually be repaired
                                        // A little ad hoc solution
                                        theHouseholdDestination[ii][j] = QString("RecoveryUnderWay");
                                    }
                                    else if (theDecisionOfOccupants == QString("Abandon") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToMakeADecision.at(ii)){
                                        //qCritical() << "Building" << ii << "will be abandoned.";
                                        //qCritical() << "";
                                        theHouseholdDestination[ii][j] = QString("GoneVoluntarily");
                                        RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                                        theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                                        theNumberOfWorkersAvailable[ii] = 0.0;
                                        // Do not set the building time sold
                                    }
                                    // If the building will be sold without repairing
                                    else if (theDecisionOfOccupants == QString("SellWithoutRepairing") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToMakeADecision.at(ii)){
                                        //qCritical() << "Building" << ii << "will be sold without repairs.";
                                        //qCritical() << "";
                                        theHouseholdDestination[ii][j] = QString("GoneVoluntarily");
                                        RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
                                        theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
                                        theNumberOfWorkersAvailable[ii] = 0.0;
                                        theBuildingTimeSold[ii] = theCurrentTime->getCurrentValue();
                                    }
                                }
                                else if (theHouseholdTenureStatus.at(ii).at(j) == QString("RenterOccupied")){
                                    if (theDecisionOfOccupants == QString("Move") && thePoissonTimeToMakeADecision > theBuildingThresholdTimeToMakeADecision.at(ii)){
                                        theHouseholdDestination[ii][j] = QString("GoneVoluntarily");
                                    }
                                }
                            }
                        }
                    } // If not gone

                    // If gone and sold
                    if (theBuildingOccupancyClass.at(ii) == QString("RES1") && theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == true && theHouseholdDestination.at(ii).at(j).contains(QString("Abandon")) == false && theCurrentTime->getCurrentValue() - theBuildingTimeSold.at(ii) > 180.0){
                        // If the building was abandoned and sold, restart the resource procurement phase 180 days after the sell
                        // This time assume funding will be immediately available from the new owner
                        theBuildingAvailableFunds[ii] = theImmediateEconomicLoss.at(ii) + 1.0;
                        alreadyRequestedEngineers[ii] = false;
                        alreadyRequestedMaterials[ii] = false;
                        alreadyRequestedNegotiation[ii] = false;
                        theBuildingTimeSold[ii] = 1000000.0;
                    }
                } // for j number of dwellings

                // Above, we defined if household would be able to finance repairs
                // and if so, where the households would get their money from.
                // For the households that will be able to finance their repairs, even if they do not have the money yet,
                // start negotiating resources.
                if (theBuildingAvailableFunds.at(ii) > 0 && theBuildingStrDamageState.at(ii) > 2){

                    // ***************************************************************
                    // Materials
                    // ***************************************************************
                    if (alreadyRequestedMaterials.at(ii) == false){

                        // Chenbo: why request materials depending on theBuildingOccupancyClass? Assume single family buildings to be wooden?

                        if (theBuildingOccupancyClass.at(ii) == QString("RES1") || theBuildingOccupancyClass.at(ii) ==  QString("RES2")){
                            //theWoodRequestID << ii;
                            //qDebug() << "household" << ii << "in building" << ii << "is requesting timber";

                        } else if(theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
                            //theConcreteRequestID << ii;
                            //qDebug() << "household" << ii << "in building" << ii << "is requesting concrete";

                        } else {
                            qCritical() << "=======================================" << theBuildingType.at(ii) << "at" << this->objectName() << "is not a valid building type";
                        }

                        theAvailableMaterials[ii] = true;
                        alreadyRequestedMaterials[ii] = true;
                    }


                    // ***************************************************************
                    // Contractor mobilization
                    // ***************************************************************
                    if (alreadyRequestedNegotiation.at(ii) == false){
                        //theContractorNegotiationRequestID << ii;
                        theBiddingProcess[ii] = true;
                        alreadyRequestedNegotiation[ii] = true;
                    }


                    // ***************************************************************
                    // Engineering evaluation
                    // ***************************************************************
                    if (alreadyRequestedEngineers.at(ii) == false){
                        //theEngineeringAssessmentRequestID << ii;
                        theEngineeringAssessment[ii] = true;
                        alreadyRequestedEngineers[ii] = true;
                    }

                } // if promise of money
            } // if some resource is still missing


            // ***************************************************************
            // Permit
            // ***************************************************************
            // Once the engineering assessment is received, request the permit assessment
            if (theBuildingStrDamageState.at(ii) > 2 && theEngineeringAssessment.at(ii) == true && theBuildingPermitStatus.at(ii) == false){
                //theBuildingPermitStatusRequestID << ii;
                theBuildingPermitStatus[ii] = true;
                theEngineeringAssessment[ii] = false;
            }


            //  If repairs are needed and power is restored and all resources were obtained and permit was obtained and materials were received
            if (theBuildingStrDamageState.at(ii) > 2 && theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theAvailabilityOfPower == true && theBuildingAvailableFunds.at(ii) >= theImmediateEconomicLoss.at(ii) && theBuildingPermitStatus.at(ii) == true && theAvailableMaterials.at(ii) > 0.0 && theBiddingProcess.at(ii) == true){

                // ***************************************************************
                // Finally hire the contractor to perform repairs
                // ***************************************************************
                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
                    theSFContractorAllocationRequestID << ii;
                    //qDebug() << this->objectName() << "Building" << ii << "with occupancy" << theBuildingOccupancyClass.at(ii) << "is requesting" << 1 << "workers" "(" << theImmediateEconomicLoss.at(ii) << ")" << theHouseholdDestination.at(ii);

                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
                    theMFContractorAllocationRequestID << ii;
                    //qDebug() << "Building" << ii << "with occupancy" << theBuildingOccupancyClass.at(ii) << "is requesting" << 1 << "workers" "(" << theImmediateEconomicLoss.at(ii) << ")";
                }

                // Reduce the funds available assuming that half of it goes towards labour costs
                theBuildingAvailableFunds[ii] -= theImmediateEconomicLoss.at(ii) / 2.0;
            } // can make request
        } // for i number of buildings


        // The following lines manage passing requests to business agents
        // Request insurance payments
        theInsuranceRequest->clearRequesterIDList();
        theInsuranceRequest->clearRequestedAmountList();
        theInsuranceRequest->clearBuildingValueList();

        if (theInsuranceRequestID.size()){
            for (int i = 0; i < theInsuranceRequestID.size(); i++){
                int theID = theInsuranceRequestID.at(i);
                theInsuranceRequest->setRequesterBuildingValue(theBuildingValue.at(theID));
                theInsuranceRequest->setRequesterIDList(theID);
                theInsuranceRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));
            }
        }

        // Request publicly-backed loan
        thePublicLoanRequest->clearRequesterIDList();
        thePublicLoanRequest->clearRequestedAmountList();
        thePublicLoanRequest->clearOccupancyTypeList();

        if (thePublicRequestID.size() > 0){
            for (int i = 0; i < thePublicRequestID.size(); i++){
                int theID = thePublicRequestID.at(i);
                thePublicLoanRequest->setRequesterIDList(theID);
                thePublicLoanRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));

                if (theBuildingOccupancyClass.at(theID) == QString("RES1")){
                    thePublicLoanRequest->setRequesterOccupancyType(theHouseholdTenureStatus.at(theID).at(0));
                } else {
                    thePublicLoanRequest->setRequesterOccupancyType(QString(""));
                }
                totalPublicLoanRequest += theImmediateEconomicLoss.at(theID);
            }
        }

        // Request private loan
        thePrivateLoanRequest->clearRequesterIDList();
        thePrivateLoanRequest->clearRequestedAmountList();

        if (thePrivateRequestID.size() > 0){
            for (int i = 0; i < thePrivateRequestID.size(); i++){
                int theID = thePrivateRequestID.at(i);
                thePrivateLoanRequest->setRequesterIDList(theID);
                thePrivateLoanRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));
            }
        }

        // Request grant money
        theGrantRequest->clearRequesterIDList();
        theGrantRequest->clearRequestedAmountList();
        theGrantRequest->clearOccupancyTypeList();

        if (theGrantRequestID.size() > 0){
            for (int i = 0; i < theGrantRequestID.size(); i++){
                int theID = theGrantRequestID.at(i);
                theGrantRequest->setRequesterIDList(theID);
                theGrantRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));

                if (theBuildingOccupancyClass.at(theID) == QString("RES1")){
                    theGrantRequest->setRequesterOccupancyType(theHouseholdTenureStatus.at(theID).at(0));
                } else {
                    theGrantRequest->setRequesterOccupancyType(QString(""));
                }
                totalGrantMoneyRequest += theImmediateEconomicLoss.at(theID);
            }
        }

        // Request wood materials
        theWoodRequest->clearRequesterIDList();
        theWoodRequest->clearRequestedAmountList();

        if (theWoodRequestID.size() > 0){
            // Assuming building will request 1 'delivery' and not one specific amount of material - room for for improvement
            double amountRequested = 1.0;
            for (int i = 0; i < theWoodRequestID.size(); i++){
                int theID = theWoodRequestID.at(i);
                theWoodRequest->setRequesterIDList(theID);
                theWoodRequest->setRequestedAmountList(amountRequested);
            }
        }

        // Request concrete materials
        theConcreteRequest->clearRequesterIDList();
        theConcreteRequest->clearRequestedAmountList();

        if (theConcreteRequestID.size() > 0){
            // Assuming building will request 1 'delivery' and not one specific amount of material - room for for improvement
            double amountRequested = 1.0;
            for (int i = 0; i < theConcreteRequestID.size(); i++){
                int theID = theConcreteRequestID.at(i);
                theConcreteRequest->setRequesterIDList(theID);
                theConcreteRequest->setRequestedAmountList(amountRequested);
            }
        }

        // Request workforce
        theContractorNegotiationRequest->clearRequesterIDList();
        theContractorNegotiationRequest->clearRequestedAmountList();
        theContractorNegotiationRequest->clearRepairClassList();
        theContractorNegotiationRequest->clearNumberOfStoreysList();
        theContractorNegotiationRequest->clearOccupancyClassList();

        if (theContractorNegotiationRequestID.size() > 0){
            for (int i = 0; i < theContractorNegotiationRequestID.size(); i++){
                int theID = theContractorNegotiationRequestID.at(i);

                if (theBuildingStrDamageState.at(theID) <= 2){
                    //theContractorNegotiationRequest->setRepairClasses(0);
                } else if (theBuildingStrDamageState.at(theID) > 2){
                    // If extensive damage is observed
                    theContractorNegotiationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                    theContractorNegotiationRequest->setRepairClasses(3);
                    theContractorNegotiationRequest->setRequesterIDList(theID);
                    theContractorNegotiationRequest->setRequestedAmountList(1.0);
                    theContractorNegotiationRequest->setNumberOfStoreys(theBuildingNumberOfStoreys.at(theID));

                } else {
                    // If slight or moderate damage is observed
                    theContractorNegotiationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                    theContractorNegotiationRequest->setRepairClasses(1);
                    theContractorNegotiationRequest->setRequesterIDList(theID);
                    theContractorNegotiationRequest->setRequestedAmountList(1.0);
                    theContractorNegotiationRequest->setNumberOfStoreys(theBuildingNumberOfStoreys.at(theID));
                }
            }
        }


        // Request engineering evaluation
        theEngineeringAssessmentRequest->clearRequesterIDList();
        theEngineeringAssessmentRequest->clearRequestedAmountList();
        theEngineeringAssessmentRequest->clearRepairClassList();
        theEngineeringAssessmentRequest->clearOccupancyClassList();
        theEngineeringAssessmentRequest->clearFundsAvailabilityList();
        theEngineeringAssessmentRequest->clearDelayInTakingActionList();

        if (theEngineeringAssessmentRequestID.size() > 0){

            for (int i = 0; i < theEngineeringAssessmentRequestID.size(); i++){
                int theID = theEngineeringAssessmentRequestID.at(i);
                theEngineeringAssessmentRequest->setDelayInTakingAction(0.0); //theDelayInTakingAction.at(theID)


                if (theBuildingStrDamageState.at(theID) <= 2){
                    // If an assessment is not necessary
                    theEngineeringAssessment[theID] = true;
                    theBuildingPermitStatus[theID] = true;


                } else if (theBuildingStrDamageState.at(theID) > 3){
                    // If extensive damage is observed
                    theEngineeringAssessmentRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                    theEngineeringAssessmentRequest->setRequesterIDList(theID);
                    theEngineeringAssessmentRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                    theEngineeringAssessmentRequest->setFundsAvailability(theBuildingAvailableFunds.at(theID));

                    if (theBuildingNeedForRedesign.at(theID) == true){
                        theEngineeringAssessmentRequest->setRepairClasses(4); // this is equal to 3* in REDi
                    } else {
                        theEngineeringAssessmentRequest->setRepairClasses(3);
                    }

                } else {
                    // If slight or moderate damage is observed
                    theEngineeringAssessmentRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                    theEngineeringAssessmentRequest->setRequesterIDList(theID);
                    theEngineeringAssessmentRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                    theEngineeringAssessmentRequest->setRepairClasses(1);
                    theEngineeringAssessmentRequest->setFundsAvailability(theBuildingAvailableFunds.at(theID));
                    //qDebug() << "household" << i << "in building" << i << "is requesting" << 1 << "evaluation" << "(" << theImmediateEconomicLoss.at(theID) << ")";
                }
            }
        }

        // Request permitting
        theBuildingPermitStatusRequest->clearRequesterIDList();
        theBuildingPermitStatusRequest->clearRequestedAmountList();
        theBuildingPermitStatusRequest->clearRepairClassList();
        theBuildingPermitStatusRequest->clearOccupancyClassList();

        if (theBuildingPermitStatusRequestID.size() > 0){
            for (int i = 0; i < theBuildingPermitStatusRequestID.size(); i++){
                int theID = theBuildingPermitStatusRequestID.at(i);
                theBuildingPermitStatusRequest->setRequesterIDList(theID);
                theBuildingPermitStatusRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                theBuildingPermitStatusRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));

                if (theBuildingStrDamageState.at(theID) > 2){
                    theBuildingPermitStatusRequest->setRepairClasses(3);
                } else {
                    theBuildingPermitStatusRequest->setRepairClasses(1);
                }

                //qDebug() << "household" << i << "in building" << i << "is requesting" << 1 << "permit" << "(" << theImmediateEconomicLoss.at(i) << ")";
            }
        }

        // Request contractor allocation
        theSFContractorAllocationRequest->clearOccupancyClassList();
        theSFContractorAllocationRequest->clearRepairClassList();
        theSFContractorAllocationRequest->clearRequesterIDList();
        theSFContractorAllocationRequest->clearRequestedAmountList();

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
                theSFContractorAllocationRequest->setBid(theBid * theBuildingValue.at(i));
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
                theMFContractorAllocationRequest->setBid(theBuildingValue.at(i));
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

                if (theHouseholdDestination.at(i).at(j).contains(QString("Terminated"))){
                    theNumberOfPersonsTerminated += theHouseholdSize.at(i).at(j);
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


                // Displaced
                if (theHouseholdDestination.at(i).at(j) != QString("NotDisplaced") && theHouseholdDestination.at(i).at(j).contains(QString("Recovery"))==false){

                    numberOfDisplacedPeople += theHouseholdSize.at(i).at(j);
                    // BuildingType
                    if (theBuildingOccupancyClass.at(i) == QString("RES1") || theBuildingOccupancyClass.at(i) == QString("MH")){
                        numberOfDetachedDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else {
                        numberOfMultifamilyDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Disability
                    if (theHouseholdDisabilityStatus.at(i).at(j) == true){
                        numberOfWithDisabilityDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else{
                        numberOfNonDisabilityDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhiteDisplacedPeople += 1.0;
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackDisplacedPeople += 1.0;
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianDisplacedPeople += 1.0;
                    }

                    // Building design code
                    if (theBuildingCodeLevel.at(i) == QString("Pre")){
                        numberOfPreCodeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("Low")){
                        numberOfLowCodeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("Moderate")){
                        numberOfModerateCodeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("High")){
                        numberOfHighCodeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedDisplacedPeople += theHouseholdSize.at(i).at(j);

                    } else {
                        numberOfOwnerOccupiedDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomeDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Insurance coverage
                    if (theBuildingInsuranceStatus.at(i) == true){
                        numberOfInsuredDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else {
                        numberOfUninsuredDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("EstablishedImmigrant")){
                        numberOfEstablishedImmigrantDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("NonImmigrant")){
                        numberOfCitizenDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Age
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                    else {
                        numberOfNoElderlyDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedDisplacedPeople += theHouseholdSize.at(i).at(j);

                    } else {
                        numberOfNonHigherEducatedDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("MarriedCouple")){
                        numberOfMarriedCouplesDisplacedPeople += theHouseholdSize.at(i).at(j);
                    } else {
                        numberOfSingleParentsDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }

                    // Recent mover status
                    if (theHouseholdRecentMoverStatus.at(i).at(j) == true) {
                        numberOfRecentMoversDisplacedPeople += theHouseholdSize.at(i).at(j);
                    } else {
                        numberOfNotRecentMoversDisplacedPeople += theHouseholdSize.at(i).at(j);
                    }
                } // Displaced


                // Voluntary relocation
                if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied") && (theHouseholdDestination.at(i).at(j) == (QString("GoneVoluntarily")) || theHouseholdDestination.at(i).at(j) == QString("RecoveryUnderWayAndSellAfter"))){

                    // Disability
                    if (theHouseholdDisabilityStatus.at(i).at(j) == true){
                        numberOfDisablePersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomePersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomePersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomePersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }
                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                        numberOfOwnerOccupiedPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Elderly
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Children
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                        numberOfChildrenPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("SingleParent")){
                        numberOfSingleParentsPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhitePersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }

                    // Hispanic
                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                        numberOfHispanicPersonsGoneVoluntarily += theHouseholdSize.at(i).at(j);
                    }
                } // Gone


                // Terminations
                if (theHouseholdDestination.at(i).at(j) == (QString("GoneTerminated"))){

                    // Disability
                    if (theHouseholdDisabilityStatus.at(i).at(j) == true){
                        numberOfDisablePersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomePersonsTerminated += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomePersonsTerminated += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomePersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }
                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                        numberOfOwnerOccupiedPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Elderly
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Children
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                        numberOfChildrenPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("SingleParent")){
                        numberOfSingleParentsPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhitePersonsTerminated += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackPersonsTerminated += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }

                    // Hispanic
                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                        numberOfHispanicPersonsTerminated += theHouseholdSize.at(i).at(j);
                    }
                } // Terminated

                // Evictions
                if (theHouseholdDestination.at(i).at(j) == (QString("GoneEvicted"))){

                    // Disability
                    if (theHouseholdDisabilityStatus.at(i).at(j) == true){
                        numberOfDisablePersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomePersonsEvicted += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomePersonsEvicted += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomePersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }
                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                        numberOfOwnerOccupiedPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Elderly
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Children
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                        numberOfChildrenPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("SingleParent")){
                        numberOfSingleParentsPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhitePersonsEvicted += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackPersonsEvicted += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }

                    // Hispanic
                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                        numberOfHispanicPersonsEvicted += theHouseholdSize.at(i).at(j);
                    }
                } // Evicted

                // Voluntary relocation
                if (theHouseholdDestination.at(i).at(j) == (QString("GoneNoFunds"))){

                    // Disability
                    if (theHouseholdDisabilityStatus.at(i).at(j) == true){
                        numberOfDisablePersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Income
                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
                        numberOfLowHouseholdIncomePersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
                        numberOfMediumHouseholdIncomePersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }
                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
                        numberOfHighHouseholdIncomePersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Tenure
                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
                        numberOfRenterOccupiedPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }
                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
                        numberOfOwnerOccupiedPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Immigrant status
                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
                        numberOfRecentImmigrantPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Elderly
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
                        numberOfElderlyPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Children
                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
                        numberOfChildrenPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // HigherEducated degree
                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
                        numberOfHigherEducatedPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Marital status of householder
                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("SingleParent")){
                        numberOfSingleParentsPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Race
                    if (theHouseholdRace.at(i).at(j).contains("White")) {
                        numberOfWhitePersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
                        numberOfBlackPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
                        numberOfAsianPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }

                    // Hispanic
                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
                        numberOfHispanicPersonsGoneNoFunds += theHouseholdSize.at(i).at(j);
                    }
                } // Gone no funds

            } // for j
        } // for i
    } // if timeOfEvent > 0.0

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
        thePersonsInShelterVector << theNumberOfPeopleSeekingShelter;
        thePersonsInRenterHousingVector << theNumberOfPeopleInHotels;
        thePersonsGoneVector << theNumberOfPersonsGone;
        theVoluntaryRelocationVector << theNumberOfPersonsLeavingVoluntarily;
        theTerminationVector << theNumberOfPersonsTerminated;
        theEvictionVector << theNumberOfPersonsEvicted;
        theNoFundsVector << theNumberOfPersonsGone - theNumberOfPersonsLeavingVoluntarily - theNumberOfPersonsTerminated - theNumberOfPersonsEvicted;

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


        theRenterPersonsGoneVoluntarilyVector << numberOfRenterOccupiedPersonsGoneVoluntarily; // / totalRenterPersons;
        theOwnerPersonsGoneVoluntarilyVector << numberOfOwnerOccupiedPersonsGoneVoluntarily; // / totalOwnerPersons;
        thePoorPersonsGoneVoluntarilyVector << numberOfLowHouseholdIncomePersonsGoneVoluntarily; // / totalPoorPersons;
        theModeratePersonsGoneVoluntarilyVector << numberOfMediumHouseholdIncomePersonsGoneVoluntarily; // / totalModeratePersons;
        theRichPersonsGoneVoluntarilyVector << numberOfHighHouseholdIncomePersonsGoneVoluntarily; // / totalRichPersons;
        theRecentImmigrantPersonsGoneVoluntarilyVector << numberOfRecentImmigrantPersonsGoneVoluntarily; // / totalRecentImmigrantPersons;
        theHigherEducatedPersonsGoneVoluntarilyVector << numberOfHigherEducatedPersonsGoneVoluntarily; // / totalHigherEducatedPersons;
        theElderlyMemberPersonsGoneVoluntarilyVector << numberOfElderlyPersonsGoneVoluntarily; // / totalElderlyMemberPersons;
        theChildrenMemberPersonsGoneVoluntarilyVector << numberOfChildrenPersonsGoneVoluntarily; // / totalChildrenMemberPersons;
        theSingleParentsPersonsGoneVoluntarilyVector << numberOfSingleParentsPersonsGoneVoluntarily; // / totalSingleParentsPersons;
        theWhitePersonsGoneVoluntarilyVector << numberOfWhitePersonsGoneVoluntarily; // / totalWhitePersons;
        theBlackPersonsGoneVoluntarilyVector << numberOfBlackPersonsGoneVoluntarily; // / totalBlackPersons;
        theAsianPersonsGoneVoluntarilyVector << numberOfAsianPersonsGoneVoluntarily; // / totalAsianPersons;
        theHispanicPersonsGoneVoluntarilyVector << numberOfHispanicPersonsGoneVoluntarily; // / totalHispanicPersons;
        theDisablePersonsGoneVoluntarilyVector << numberOfDisablePersonsGoneVoluntarily;

        theRenterPersonsTerminatedVector << numberOfRenterOccupiedPersonsTerminated; // / totalRenterPersons;
        theOwnerPersonsTerminatedVector << numberOfOwnerOccupiedPersonsTerminated; // / totalOwnerPersons;
        thePoorPersonsTerminatedVector << numberOfLowHouseholdIncomePersonsTerminated; // / totalPoorPersons;
        theModeratePersonsTerminatedVector << numberOfMediumHouseholdIncomePersonsTerminated; // / totalModeratePersons;
        theRichPersonsTerminatedVector << numberOfHighHouseholdIncomePersonsTerminated; // / totalRichPersons;
        theRecentImmigrantPersonsTerminatedVector << numberOfRecentImmigrantPersonsTerminated; // / totalRecentImmigrantPersons;
        theHigherEducatedPersonsTerminatedVector << numberOfHigherEducatedPersonsTerminated; // / totalHigherEducatedPersons;
        theElderlyMemberPersonsTerminatedVector << numberOfElderlyPersonsTerminated; // / totalElderlyMemberPersons;
        theChildrenMemberPersonsTerminatedVector << numberOfChildrenPersonsTerminated; // / totalChildrenMemberPersons;
        theSingleParentsPersonsTerminatedVector << numberOfSingleParentsPersonsTerminated; // / totalSingleParentsPersons;
        theWhitePersonsTerminatedVector << numberOfWhitePersonsTerminated; // / totalWhitePersons;
        theBlackPersonsTerminatedVector << numberOfBlackPersonsTerminated; // / totalBlackPersons;
        theAsianPersonsTerminatedVector << numberOfAsianPersonsTerminated; // / totalAsianPersons;
        theHispanicPersonsTerminatedVector << numberOfHispanicPersonsTerminated; // / totalHispanicPersons;
        theDisablePersonsTerminatedVector << numberOfDisablePersonsTerminated;

        theRenterPersonsEvictedVector << numberOfRenterOccupiedPersonsEvicted; // / totalRenterPersons;
        theOwnerPersonsEvictedVector << numberOfOwnerOccupiedPersonsEvicted; // / totalOwnerPersons;
        thePoorPersonsEvictedVector << numberOfLowHouseholdIncomePersonsEvicted; // / totalPoorPersons;
        theModeratePersonsEvictedVector << numberOfMediumHouseholdIncomePersonsEvicted; // / totalModeratePersons;
        theRichPersonsEvictedVector << numberOfHighHouseholdIncomePersonsEvicted; // / totalRichPersons;
        theRecentImmigrantPersonsEvictedVector << numberOfRecentImmigrantPersonsEvicted; // / totalRecentImmigrantPersons;
        theHigherEducatedPersonsEvictedVector << numberOfHigherEducatedPersonsEvicted; // / totalHigherEducatedPersons;
        theElderlyMemberPersonsEvictedVector << numberOfElderlyPersonsEvicted; // / totalElderlyMemberPersons;
        theChildrenMemberPersonsEvictedVector << numberOfChildrenPersonsEvicted; // / totalChildrenMemberPersons;
        theSingleParentsPersonsEvictedVector << numberOfSingleParentsPersonsEvicted; // / totalSingleParentsPersons;
        theWhitePersonsEvictedVector << numberOfWhitePersonsEvicted; // / totalWhitePersons;
        theBlackPersonsEvictedVector << numberOfBlackPersonsEvicted; // / totalBlackPersons;
        theAsianPersonsEvictedVector << numberOfAsianPersonsEvicted; // / totalAsianPersons;
        theHispanicPersonsEvictedVector << numberOfHispanicPersonsEvicted; // / totalHispanicPersons;
        theDisablePersonsEvictedVector << numberOfDisablePersonsEvicted;

        theRenterPersonsGoneNoFundsVector << numberOfRenterOccupiedPersonsGoneNoFunds; // / totalRenterPersons;
        theOwnerPersonsGoneNoFundsVector << numberOfOwnerOccupiedPersonsGoneNoFunds; // / totalOwnerPersons;
        thePoorPersonsGoneNoFundsVector << numberOfLowHouseholdIncomePersonsGoneNoFunds; // / totalPoorPersons;
        theModeratePersonsGoneNoFundsVector << numberOfMediumHouseholdIncomePersonsGoneNoFunds; // / totalModeratePersons;
        theRichPersonsGoneNoFundsVector << numberOfHighHouseholdIncomePersonsGoneNoFunds; // / totalRichPersons;
        theRecentImmigrantPersonsGoneNoFundsVector << numberOfRecentImmigrantPersonsGoneNoFunds; // / totalRecentImmigrantPersons;
        theHigherEducatedPersonsGoneNoFundsVector << numberOfHigherEducatedPersonsGoneNoFunds; // / totalHigherEducatedPersons;
        theElderlyMemberPersonsGoneNoFundsVector << numberOfElderlyPersonsGoneNoFunds; // / totalElderlyMemberPersons;
        theChildrenMemberPersonsGoneNoFundsVector << numberOfChildrenPersonsGoneNoFunds; // / totalChildrenMemberPersons;
        theSingleParentsPersonsGoneNoFundsVector << numberOfSingleParentsPersonsGoneNoFunds; // / totalSingleParentsPersons;
        theWhitePersonsGoneNoFundsVector << numberOfWhitePersonsGoneNoFunds; // / totalWhitePersons;
        theBlackPersonsGoneNoFundsVector << numberOfBlackPersonsGoneNoFunds; // / totalBlackPersons;
        theAsianPersonsGoneNoFundsVector << numberOfAsianPersonsGoneNoFunds; // / totalAsianPersons;
        theHispanicPersonsGoneNoFundsVector << numberOfHispanicPersonsGoneNoFunds; // / totalHispanicPersons;
        theDisablePersonsGoneNoFundsVector << numberOfDisablePersonsGoneNoFunds;

    }

    return 1;
}


void RMainNeighborhoodModel::plotUsableBuildings(double habitableArea)
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



void RMainNeighborhoodModel::plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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
           // buildingRepairsPlot->xAxis->setAutoTicks(false);
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


void RMainNeighborhoodModel::plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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
           // buildingRepairsPlot->xAxis->setAutoTicks(false);
           // buildingRepairsPlot->xAxis->setAutoTickLabels(false);
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
       // buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        //buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}


void RMainNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name)
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
       // displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}

// For Chenbo: create a copy of this function and use it to plot the
//             displaced persons at low, moderate, and high income
// Tip: you need to create a new plotter object - ctrl F on the displacedPopulationPlot
//      and see where it is defined, initiated, and so on
void RMainNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name)
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
       // displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
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
