
#include "RResidentialMobilityNeighborhoodModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RAHSPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RNetworkSamplingModel.h"


#include <math.h>

RResidentialMobilityNeighborhoodModel::RResidentialMobilityNeighborhoodModel(QObject *parent, QString name)
    : RNeighborhoodModel(parent, name)
{

    // Community
    theMedianHouseholdIncome = 100000.0;
    theNumberOfBuildings = 0;
    theTotalNumberOfDwellings = 0;
    theTotalBuildingArea  = 0.0;

    // Households
    theNumberOfPersonsGone = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    theNumberOfPersons = 0;
    theAverageDamageState = 0.0;
    theNumberOfRepairedDwellings = 0;

    // Requests
    numberOfWorkforceRequestsCreatedHere = 0;
    numberOfMoneyRequestsCreatedHere = 0;

    theInsuranceRequest = nullptr;
    thePublicLoanRequest = nullptr;
    thePrivateLoanRequest = nullptr;
    theContractorNegotiationRequest = nullptr;
    theSFContractorAllocationRequest = nullptr;
    theMFContractorAllocationRequest = nullptr;

    // Outputs - maybe remove after papers
    theNumberOfHabitableDwellings = 0;
    totalHabitableArea = 0.0;

    // Intensity counters
    theSa03Counter = theSamplingStartingPoint;
    theSa1Counter = theSamplingStartingPoint;

    // Response
    theResponse = nullptr;

    // Plots
    timeAnalysisBegan = 0.0;
    timeSinceLastPlot = 0.0;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

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

    QString negotiationRequestName = QString(this->objectName() + "ContractorNegotiation");
    theDomain->createObject(negotiationRequestName, "RContractorNegotiationRequest");
    theContractorNegotiationRequest = qobject_cast<RContractorNegotiationRequest *>(theDomain->getLastAddedObject());
    theContractorNegotiationRequest->setModel(this);
    theContractorNegotiationRequest->setContentType(QString("Negotiation"));

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

RResidentialMobilityNeighborhoodModel::~RResidentialMobilityNeighborhoodModel()
{
}


int RResidentialMobilityNeighborhoodModel::resetTime()
{
    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    theAverageDamageState = 0.0;
    numberOfBankLoans = 0.0;
    numberOfSBALoans = 0.0;
    numberOfCDBGDRGrants = 0.0;
    numberOfHouseholdsMissingFunds = 0.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;
    theNumberOfPersonsGone = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    numberOfWorkforceRequestsCreatedHere = 0;
    numberOfMoneyRequestsCreatedHere = 0;
    theNumberOfRepairedDwellings = 0;

    theDamagedBuildingsIndexes.clear();
    theNotableBuildingsIndexes.clear();
    theTickVector.clear();
    theLabelVector.clear();
    timeVector.clear();
    theHousingRecoveryVector.clear();
    theDisplacedPopulationVector.clear();
    theDelayInTakingAction.clear();
    theBuildingDelayToStartRepairs.clear();
    theBuildingAvailableFunds.clear();
    theBuildingExpectedFunds.clear();
    theNumberOfWorkersAvailable.clear();
    theImmediateEconomicLoss.clear();
    theImmediateContentLoss.clear();
    theBuildingRepairTime.clear();
    theBuildingAmountFromInsurance.clear();
    theBuildingAmountLendedFromSBA.clear();
    theBuildingAmountLendedFromBank.clear();
    theBuildingAmountGrantedByCDBGDR.clear();
    theBuildingAmountMissing.clear();
    theTimeNeededToGetFunding.clear();

    theBuildingReoccupancyTime.clear();
    theBuildingTimeSold.clear();
    theBuildingThresholdTimeToBeSold.clear();
    theBuildingThresholdTimeToMakeADecision.clear();
    theBuildingTimeUnderRepairs.clear();
    theBiddingProcess.clear();
    theBuildingNeedForRedesign.clear();
    theBuildingStrDamageState.clear();
    theBuildingNSDDamageState.clear();
    theBuildingNSADamageState.clear();
    alreadyRequestedNegotiation.clear();
    thePersonsGoneVector.clear();

    theSFContractorAllocationRequest->clearOccupancyClassList();
    theSFContractorAllocationRequest->clearRepairClassList();
    theSFContractorAllocationRequest->clearRequesterIDList();
    theSFContractorAllocationRequest->clearRequestedAmountList();
    theInsuranceRequest->clearRequesterIDList();
    theInsuranceRequest->clearRequestedAmountList();
    theInsuranceRequest->clearBuildingValueList();
    thePublicLoanRequest->clearRequesterIDList();
    thePublicLoanRequest->clearRequestedAmountList();
    thePublicLoanRequest->clearOccupancyTypeList();
    thePrivateLoanRequest->clearRequesterIDList();
    thePrivateLoanRequest->clearRequestedAmountList();
    thePrivateLoanRequest->clearIncomeList();
    theGrantRequest->clearRequesterIDList();
    theGrantRequest->clearRequestedAmountList();
    theGrantRequest->clearOccupancyTypeList();

    // --------
    theRecoveryTime.resize(theNumberOfBuildings);
    theRecoveryTime.fill(2555.0);

    // --------
    theNumberOfUninhabitableDwellingsVector.clear();
    theAmountMissingVector = 0.0;
    theBankLoansVector = 0.0;
    theSBALoanVector = 0.0;
    theCDBGDRGrantVector = 0.0;

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

    theBuildingThresholdTimeToMakeADecision.resize(theNumberOfBuildings);
    theBuildingThresholdTimeToMakeADecision.fill(0.0);

    theDelayInTakingAction.resize(theNumberOfBuildings);
    theDelayInTakingAction.fill(0.0);

    theBuildingDelayToStartRepairs.resize(theNumberOfBuildings);
    theBuildingDelayToStartRepairs.fill(0.0);

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

    theTimeNeededToGetFunding.resize(theNumberOfBuildings);
    theTimeNeededToGetFunding.fill(0.0);

    theNumberOfWorkersAvailable.resize(theNumberOfBuildings);
    theNumberOfWorkersAvailable.fill(0.0);

    theBuildingTimeUnderRepairs.resize(theNumberOfBuildings);
    theBuildingTimeUnderRepairs.fill(0.0);

    theBuildingNeedForRedesign.resize(theNumberOfBuildings);
    theBuildingNeedForRedesign.fill(false);

    theBiddingProcess.resize(theNumberOfBuildings);
    theBiddingProcess.fill(false);

    theRepairClass.resize(theNumberOfBuildings);
    theRepairClass.fill(0);

    alreadyRequestedFinancing.resize(theNumberOfBuildings);
    alreadyRequestedFinancing.fill(false);

    alreadyRequestedNegotiation.resize(theNumberOfBuildings);
    alreadyRequestedNegotiation.fill(false);

    theTickVector << 0;
    theLabelVector << QString("%1").arg(0);

    // -----------
    // Read the PGA and Sa values from those input to the location of this neighborhood
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
        thePGACounter = theSamplingStartingPoint;
    }

    theSa03Counter++;
    theSa1Counter++;
    thePGACounter++;
    // -----------


    return 0;
}


double RResidentialMobilityNeighborhoodModel::getProbabilityOfLiquefaction()
{
    return theProbabilityOfLiquefaction;
}

void RResidentialMobilityNeighborhoodModel::setProbabilityOfLiquefaction(double value)
{
    theProbabilityOfLiquefaction = value;
}


void RResidentialMobilityNeighborhoodModel::compileResults()
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
                      theBuildingOwnerIncome.at(i) << "," << theHomeownerIncomeValue.at(i) << "," << theHouseholdIncome.at(i).at(0) << "," << theHouseholdMortgage.at(i).at(0) << "," << theHouseholdTenureStatus.at(i).at(0) << "," <<
                      theBuildingCodeLevel.at(i) << "," << theBuildingValue.at(i) << "," << theImmediateEconomicLoss.at(i) << "," << theBuildingAmountFromInsurance.at(i) << "," << theBuildingAmountLendedFromBank.at(i)  << "," << theBuildingAmountLendedFromSBA.at(i) << "," << theBuildingAmountGrantedByCDBGDR.at(i)  << "," <<  theBuildingAmountMissing.at(i)  << "," <<
                      theHouseholdRace.at(i).at(0) << "," << theHouseholdRecentMoverStatus.at(i).at(0) << "," << theHouseholdAgeCategory.at(i).at(0) << "," << theHouseholdDisabilityStatus.at(i).at(0) << "," << theHouseholdImmigrantStatus.at(i).at(0) << "," <<
                      theHouseholdHigherEducationStatus.at(i).at(0) << "," << theHouseholdSize.at(i).at(0) << "," <<
                      theBuildingDelayToStartRepairs.at(i) << "," << theBuildingRepairTime.at(i);
            stream << endl;
        }
    }
    file.close();
}

QVector<double> RResidentialMobilityNeighborhoodModel::getUninhabitableDwellingsVector()
{
    return theNumberOfUninhabitableDwellingsVector;
}

double RResidentialMobilityNeighborhoodModel::getSBALoanVector()
{
    return theSBALoanVector;
}

double RResidentialMobilityNeighborhoodModel::getCDBGDRGrantVector()
{
    return theCDBGDRGrantVector;
}

double RResidentialMobilityNeighborhoodModel::getMissingFundsVector()
{
    return theAmountMissingVector;
}

double RResidentialMobilityNeighborhoodModel::getBankLoanVector()
{
    return theBankLoansVector;
}


// ---

int RResidentialMobilityNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RResidentialMobilityNeighborhoodModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}

int RResidentialMobilityNeighborhoodModel::getNumberOfPeople()
{
    return theNumberOfPersons;
}


void RResidentialMobilityNeighborhoodModel::getRequests()
{

}

QVector<RLocation*> RResidentialMobilityNeighborhoodModel::getBuildingLocations()
{
    if (theBuildingLocation.size() == 0){
        theBuildingLocation = theHouseholdPortfolio->getBuildingLocationVector();
    }
    return theBuildingLocation;
}


int RResidentialMobilityNeighborhoodModel::evaluateModel()
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

        // Tell the Household Portfolio to read or instantiate the buildings
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
        theBuildingNumberOfDwellings      = theHouseholdPortfolio->getBuildingDwellingsVector();
        theBuildingOwnerIncome            = theHouseholdPortfolio->getBuildingOwnerIncomeVector();
        theBuildingOwnerImmigrationStatus = theHouseholdPortfolio->getBuildingOwnerImmigrantStatusVector();
        theBuildingInsuranceStatus        = theHouseholdPortfolio->getBuildingInsuranceStatus();

        // Get all information about the households
        theHomeownerIncomeValue           = theHouseholdPortfolio->getHomeownerIncomeValueVector();
        theHouseholdIncome                = theHouseholdPortfolio->getHouseholdIncomeVector();
        theHouseholdMortgage              = theHouseholdPortfolio->getMortgageVector();
        theHouseholdTenureStatus          = theHouseholdPortfolio->getHouseholdTenureVector();
        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();
        theHouseholdAgeCategory           = theHouseholdPortfolio->getHouseholdAgeCategoryVector();
        theHouseholdImmigrantStatus       = theHouseholdPortfolio->getHouseholdImmigrantStatusVector();
        theHouseholdRace                  = theHouseholdPortfolio->getHouseholdRaceVector();
        theHouseholdDisabilityStatus      = theHouseholdPortfolio->getHouseholdDisabilityStatus();
        theHouseholdHigherEducationStatus = theHouseholdPortfolio->getHouseholdEducationStatus();
        theHouseholdMaritalStatus         = theHouseholdPortfolio->getHouseholdMaritalStatus();
        theHouseholdRecentMoverStatus     = theHouseholdPortfolio->getHouseholdRecentMoverStatus();

        // initalize all these vectors
        theBuildingStrDamageState.resize(theNumberOfBuildings);
        theBuildingStrDamageState.fill(0);

        theBuildingNSDDamageState.resize(theNumberOfBuildings);
        theBuildingNSDDamageState.fill(0);

        theBuildingNSADamageState.resize(theNumberOfBuildings);
        theBuildingNSADamageState.fill(0);

        theTotalNumberOfDwellings = 0.0;
        theNumberOfPersons = 0;

        // Start some vectors with placeholder values
        for (int i = 0; i < theNumberOfBuildings; i++){
            theTotalNumberOfDwellings += theBuildingNumberOfDwellings.at(i);
        }

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

        theBuildingThresholdTimeToMakeADecision.resize(theNumberOfBuildings);
        theBuildingThresholdTimeToMakeADecision.fill(0.0);

        theDelayInTakingAction.resize(theNumberOfBuildings);
        theDelayInTakingAction.fill(0.0);

        theBuildingDelayToStartRepairs.resize(theNumberOfBuildings);
        theBuildingDelayToStartRepairs.fill(0.0);

        theBuildingAvailableFunds.resize(theNumberOfBuildings);
        theBuildingAvailableFunds.fill(0.01);

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

        theTimeNeededToGetFunding.resize(theNumberOfBuildings);
        theTimeNeededToGetFunding.fill(0.0);

        theNumberOfWorkersAvailable.resize(theNumberOfBuildings);
        theNumberOfWorkersAvailable.fill(0.0);

        theBuildingTimeUnderRepairs.resize(theNumberOfBuildings);
        theBuildingTimeUnderRepairs.fill(0.0);

        theBiddingProcess.resize(theNumberOfBuildings);
        theBiddingProcess.fill(false);

        theBuildingNeedForRedesign.resize(theNumberOfBuildings);
        theBuildingNeedForRedesign.fill(false);

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

        alreadyRequestedNegotiation.resize(theNumberOfBuildings);
        alreadyRequestedNegotiation.fill(false);
    } // if

    // --------------------------------------------------------------------------------------
    // 2 - Evaluate the availability of water and power to the neighborhood
    // --------------------------------------------------------------------------------------


    // --------------------------------------------------------------------------------------
    // 3 - Calculate everything that happens at the time of the earthquake (t = t_0)
    // --------------------------------------------------------------------------------------
    // Immediate losses
    // Considering only one earthquake in this analysis - think about aftershocks later
    if (timeOfEvent == 0.0){

        // Delete the packets whose final destinations was this model
        this->handlePacketsThatReachedFinalDestination();

        // Building damage model
        theBuildingDamageModel = qobject_cast<RHazusBuildingModel*>(theBuildingDamageModelResponse->getModel());

        //qCritical() << "Overwriting the Sa0.3 and Sa1!! Scale the first set of SA's by the sampling multiplier";
        // For all the buildings
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
            double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

            QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
            double Sa1 = theSa1Vector.at(theSa1Counter);

            QVector<double> thePGAVector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getPGAIntensity());
            double thePGA = theSa1Vector.at(thePGACounter);

            //double Sa0_3 = theSa0_3Vector.at(1);
            //double Sa1 = theSa1Vector.at(1);

            QString theSoilSusceptibility = QString("None");
            double rngSoil = theRNG.generateDouble();
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

            // If there was damage to any building set the time of the event as the current time
            timeOfEvent = theCurrentTime->getCurrentValue();

            // Estimate immediate impact
            theImmediateEconomicLoss[i] = theBuildingDamageModel->getLossRatio() * theBuildingValue.at(i);
            theImmediateContentLoss[i] = theBuildingDamageModel->getContentLossRatio() * theBuildingValue.at(i);
            theBuildingRepairTime[i] = theBuildingDamageModel->getRepairTime();
            theBuildingReoccupancyTime[i] = theBuildingDamageModel->getReoccupancyTime();

            // Get hold of the damaged buildings
            if (theImmediateEconomicLoss.at(i) > 0.0){
                theDamagedBuildingsIndexes << i;
            }

            // If the immediate loss is higher than the building value it means the building collapsed
            if (theImmediateEconomicLoss.at(i) > theBuildingValue.at(i)){
                theBuildingNeedForRedesign[i] = true;
            }

            // If this is a single-family renter-occupied building set a delay in taking action
            if (theBuildingOccupancyClass.at(i) == QString("RES1") && theHouseholdTenureStatus.at(i).at(0) == QString("RenterOccupied")){
                theDelayInTakingAction[i] = 30.0;
            }

            QVector<bool> auxHouseholdFatality;

            // Threshold prob of being sold
            theBuildingThresholdTimeToBeSold[i] = theRNG.generateDouble();

            // Threshold prob of leaving
            theBuildingThresholdTimeToMakeADecision[i] = theRNG.generateDouble();
        } // i

        theAverageDamageState /= theNumberOfBuildings;
    } // if timeOfEvent == 0.0


    // --------------------------------------------------------------------------------------
    // 4 - Handle packets delivered on this time step
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

            if (theNotableBuildingsIndexes.contains(packetOwnerI) == false){
                theNotableBuildingsIndexes << packetOwnerI;
            }

            // If this packet contains workforce
            else if (packetType == QString("Allocation")){
                if (amountReceived > 0){
                    theNumberOfWorkersAvailable[packetOwnerI] += static_cast<int>(amountReceived);
                    theBuildingDelayToStartRepairs[packetOwnerI] = theCurrentTime->getCurrentValue() - timeOfEvent;
                }
            }
        }
    }

    // Delete the packets whose final destinations was this model
    this->handlePacketsThatReachedFinalDestination();

    // --------------------------------------------------------------------------------------
    // 5 - Check if enough time has passed so that the building owner has obtained all financing needed
    // --------------------------------------------------------------------------------------
    for (int i = 0; i < theNumberOfBuildings; i++){
        if (theBuildingAvailableFunds.at(i) < theImmediateEconomicLoss.at(i) && theTimeNeededToGetFunding.at(i) < theCurrentTime->getCurrentValue() - timeOfEvent){
            theBuildingAvailableFunds[i] += theImmediateEconomicLoss.at(i) - theBuildingAvailableFunds.at(i);
            theNotableBuildingsIndexes << i;
        }
    }

    // --------------------------------------------------------------------------------------
    // 6 - Calculate everything that happens after the time of the earthquake (t > t_0)
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
    theNumberOfPersons = 0;
    QVector<int> indexesToRemove;

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
                theNumberOfRepairedDwellings++;

                // Remove this building from the list of notable buildings
                indexesToRemove << ii;
            }

            // If repairs are progressing normally
            else if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii)){

                // If minor
                if (theBuildingStrDamageState.at(ii) <= 2){
                    theBuildingTimeUnderRepairs[ii] += static_cast<double>(theCurrentTime->getCurrentValue() - lastEvaluationTime);

                } // If major damage
                else if (theBuildingStrDamageState.at(ii) > 2 && theNumberOfWorkersAvailable.at(ii) > 0){
                    theBuildingTimeUnderRepairs[ii] += static_cast<double>(theCurrentTime->getCurrentValue() - lastEvaluationTime);
                }
            }

            // Lastly, the building is not repaired and some resource is still missing, request it
            if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && (theBuildingAvailableFunds.at(ii) <= 0.0 || theNumberOfWorkersAvailable.at(ii) <= 0)){
                if (theCurrentTime->getCurrentValue() - timeOfEvent > 600.0){
                    //qCritical() << ii << theBuildingType.at(ii) << theBuildingOccupancyClass.at(ii) << theHouseholdTenureStatus.at(ii).at(0) << theHouseholdIncome.at(ii).at(0) << "Materials:" << "Funds:" << theBuildingAvailableFunds.at(ii) << theImmediateEconomicLoss.at(ii) << "Workers:" << theNumberOfWorkersAvailable.at(ii) << "Bidding:" << theBiddingProcess.at(ii);
                }

                // ***************************************************************
                // Loop over all dwellings
                // ***************************************************************
                // Loop over all households in the buildings
                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){
                    theTimeNeededToGetFunding[ii] = theFinancingTimeEstimator->evaluateFinancingTime(theHouseholdTenureStatus.at(ii).at(j), theHouseholdMortgage.at(ii).at(j),
                                                                                                     theHouseholdIncome.at(ii).at(j), theHomeownerIncomeValue.at(ii),
                                                                                                     theImmediateEconomicLoss.at(ii), theBuildingValue.at(ii));




                    alreadyRequestedFinancing[ii] = true;

                } // for j number of dwellings

                // Above, we defined if household would be able to finance repairs
                // and if so, where the households would get their money from.
                // For the households that will be able to finance their repairs, even if they do not have the money yet,
                // start negotiating resources.
                if (theImmediateEconomicLoss.at(ii) > 0.0){

                    // ***************************************************************
                    // Contractor mobilization
                    // ***************************************************************
                    if (alreadyRequestedNegotiation.at(ii) == false){
                        //theContractorNegotiationRequestID << ii;
                        theBiddingProcess[ii] = true;
                        alreadyRequestedNegotiation[ii] = true;
                    }
                } // if promise of money
            } // if some resource is still missing

            //  If repairs are needed and power is restored and all resources were obtained and permit was obtained and materials were received
            if (theBuildingStrDamageState.at(ii) > 2 && theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theBuildingAvailableFunds.at(ii) >= theImmediateEconomicLoss.at(ii) && theBiddingProcess.at(ii) == true){

                // ***************************************************************
                // Finally hire the contractor to perform repairs
                // ***************************************************************
                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
                    theSFContractorAllocationRequestID << ii;

                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
                    theMFContractorAllocationRequestID << ii;
                }

                // Reduce the funds available assuming that half of it goes towards labour costs
                theBuildingAvailableFunds[ii] -= theImmediateEconomicLoss.at(ii) / 2.0;
            } // can make request
        } // for i number of buildings


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

        // Request contractor allocation
        theSFContractorAllocationRequest->clearOccupancyClassList();
        theSFContractorAllocationRequest->clearRepairClassList();
        theSFContractorAllocationRequest->clearRequesterIDList();
        theSFContractorAllocationRequest->clearRequestedAmountList();

        if (theSFContractorAllocationRequestID.size() > 0){
            for (int i = 0; i < theSFContractorAllocationRequestID.size(); i++){
                int theID = theSFContractorAllocationRequestID.at(i);
                theSFContractorAllocationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
                theSFContractorAllocationRequest->setRepairClasses(1);
                theSFContractorAllocationRequest->setRequesterIDList(theID);
                theSFContractorAllocationRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
                theSFContractorAllocationRequest->setBid(theBuildingValue.at(i));
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

            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
                theNumberOfPersons += theHouseholdSize.at(i).at(j);
            }

            // Prepare data for plotting
            if (theBuildingStrDamageState.at(i) > 2 && theBuildingRepairTime.at(i) > theBuildingTimeUnderRepairs.at(i)){
                theNumberOfHabitableDwellings -= theBuildingNumberOfDwellings.at(i);
            }
        }
    } // if timeOfEvent > 0.0

    for (int i = 0; i < indexesToRemove.size(); i++){
        theNotableBuildingsIndexes.removeAt(theNotableBuildingsIndexes.indexOf(indexesToRemove.at(i)));
    }

    theResponse->setCurrentValue(theNumberOfPersonsGone);


    // -------------------------------------------------------------------------------------------
    // 7 - Create plots
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
            qDebug() << " | " << "Number of persons permanently displaced:" << theNumberOfPersonsGone;
            qDebug() << "-----------------------------------------------------";
            qDebug() << "";
        }


        timeSinceLastPlot = 0.0;
        timeVector << lastEvaluationTime - timeAnalysisBegan;
        theHousingRecoveryVector << theNumberOfHabitableDwellings;
        theNumberOfUninhabitableDwellingsVector << theTotalNumberOfDwellings - theNumberOfHabitableDwellings;
        theDisplacedPopulationVector << theNumberOfPersonsGone;

        thePersonsGoneVector << theNumberOfPersonsGone;
    }

    return 1;
}


void RResidentialMobilityNeighborhoodModel::plotUsableBuildings(double habitableArea)
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



void RResidentialMobilityNeighborhoodModel::plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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


void RResidentialMobilityNeighborhoodModel::plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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


void RResidentialMobilityNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name)
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
void RResidentialMobilityNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name)
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
