#include "RRecoveryStrategyStudyNeighborhoodModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RNetworkSamplingModel.h"


#include <math.h>

RRecoveryStrategyStudyNeighborhoodModel::RRecoveryStrategyStudyNeighborhoodModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    repairProgressFunction = "";
    theVariableOfInterest = "";

    // Community
    theMedianHouseholdIncome = 100000.0;
    theNumberOfBuildings = 0;
    theTotalNumberOfDwellings = 0;
    theTotalBuildingArea  = 0.0;

    // Households
    theNumberOfPersons = 0;
    theOccupancyRate = 0.0;


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

    // Response
    theResponse = nullptr;

    // Plots
    totalHabitableArea = 0.0;
    timeAnalysisBegan = 0.0;
    timeSinceLastPlot = 0.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;

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

RRecoveryStrategyStudyNeighborhoodModel::~RRecoveryStrategyStudyNeighborhoodModel()
{
}


int RRecoveryStrategyStudyNeighborhoodModel::resetTime()
{
    //qCritical() << this->objectName() << "is skipping the check for water/power!!";

    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;

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
    theDelayInTakingAction.clear();
    theBuildingAvailableFunds.clear();
    theBuildingReceivedFundTime.clear();
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

    alreadyRequestedEngineers.clear();
    alreadyRequestedMaterials.clear();
    alreadyRequestedNegotiation.clear();

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

    theRecoveryTime.resize(theNumberOfBuildings);
    theRecoveryTime.fill(2555.0);

    // --------
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

    theBuildingReceivedFundTime.resize(theNumberOfBuildings);
    theBuildingReceivedFundTime.fill(1825.0);

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


QObject * RRecoveryStrategyStudyNeighborhoodModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RRecoveryStrategyStudyNeighborhoodModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getBuildingInspectorList()
{
    return parameterListToString(theBuildingInspectorsList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setBuildingInspectorList(QString value)
{
    theBuildingInspectorsList = stringToParameterList(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getEngineeringFirmList()
{
    return parameterListToString(theEngineeringFirmList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setEngineeringFirmList(QString value)
{
    theEngineeringFirmList = stringToParameterList(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getPermittingAuthorityList()
{
    return parameterListToString(theBuildingPermitAuthorityList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setPermittingAuthorityList(QString value)
{
    theBuildingPermitAuthorityList = stringToParameterList(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getContractorList()
{
    return parameterListToString(theContractorList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setContractorList(QString value)
{
    theContractorList = stringToParameterList(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getMaterialSupplierList()
{
    return parameterListToString(theMaterialSupplierList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setMaterialSupplierList(QString value)
{
    theMaterialSupplierList = stringToParameterList(value);
}


QString RRecoveryStrategyStudyNeighborhoodModel::getFinancialResourcesSupplierList()
{
    return parameterListToString(theFinancialResourcesSupplierList);
}

void RRecoveryStrategyStudyNeighborhoodModel::setFinancialResourcesSupplierList(QString value)
{
    theFinancialResourcesSupplierList = stringToParameterList(value);
}

int RRecoveryStrategyStudyNeighborhoodModel::getSamplingStartingPoint()
{
    return theSamplingStartingPoint;
}

void RRecoveryStrategyStudyNeighborhoodModel::setSamplingStartingPoint(int value)
{
    theSamplingStartingPoint = value;
}


QVector<double> RRecoveryStrategyStudyNeighborhoodModel::getTimeVector()
{
    return timeVector;
}

QVector<int> RRecoveryStrategyStudyNeighborhoodModel::getHousingRecoveryVector()
{
    return theHousingRecoveryVector;
}


// ---

int RRecoveryStrategyStudyNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RRecoveryStrategyStudyNeighborhoodModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}

void RRecoveryStrategyStudyNeighborhoodModel::getRequests()
{

}

QVector<RLocation*> RRecoveryStrategyStudyNeighborhoodModel::getBuildingLocations()
{
    if (theBuildingLocation.size() == 0){
        theBuildingLocation = theHouseholdPortfolio->getBuildingLocationVector();
    }
    return theBuildingLocation;
}


int RRecoveryStrategyStudyNeighborhoodModel::evaluateModel()
{
//    // Output
//    if (theOutputLevel == RDomain::Maximum) {
//        qDebug() << "";
//        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
//    }

//    RNormalDistribution theNormalDistribution(theDomain);

//    // "Functionality vs time" plot
//    if (lastEvaluationTime == 0.0){
//        lastEvaluationTime = theCurrentTime->getCurrentValue();
//        timeAnalysisBegan = theCurrentTime->getCurrentValue();
//    }

//    // --------------------------------------------------------------------------------------
//    // 1 - Define the building and household archetypes
//    //     At this step all characteristics of buildings and households are defined
//    // --------------------------------------------------------------------------------------
//    if (theNumberOfBuildings < 1){

//        theMedianHouseholdIncome          = theHouseholdPortfolio->getMedianHouseholdIncome();

//        // Tell the Census Information to define the buildings
//        theHouseholdPortfolio->instantiateHouseholds();

//        // Get all information about the buildings
//        theNumberOfBuildings              = theHouseholdPortfolio->getNumberOfBuildings();
//        theBuildingType                   = theHouseholdPortfolio->getBuildingTypesVector();
//        theBuildingCodeLevel              = theHouseholdPortfolio->getBuildingCodeLevelVector();
//        theBuildingOccupancyClass         = theHouseholdPortfolio->getBuildingOccupancyClassVector();
//        theBuildingNumberOfDwellings      = theHouseholdPortfolio->getBuildingDwellingsVector();
//        theBuildingLocation               = theHouseholdPortfolio->getBuildingLocationVector();
//        theBuildingOwnerIncome            = theHouseholdPortfolio->getBuildingOwnerIncomeVector();
//        theBuildingOwnerImmigrationStatus = theHouseholdPortfolio->getBuildingOwnerImmigrantStatusVector();
//        theBuildingInsuranceStatus        = theHouseholdPortfolio->getBuildingInsuranceStatus();
//        theBuildingAverageRent            = theHouseholdPortfolio->getAverageRent();

//        // Get all information about the households
//        theHomeownerIncomeValue           = theHouseholdPortfolio->getHomeownerIncomeValueVector();
//        theHouseholdIncome                = theHouseholdPortfolio->getHouseholdIncomeVector();
//        theHouseholdMortgage              = theHouseholdPortfolio->getMortgageVector();
//        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();
//        theHouseholdTenureStatus          = theHouseholdPortfolio->getHouseholdTenureVector();

//        // initalize all these vectors
//        theTotalNumberOfDwellings = 0.0;
//        theNumberOfPersons = 0;

//        // Start some vectors with placeholder values
//        for (int i = 0; i < theNumberOfBuildings; i++){
//            theTotalNumberOfDwellings += theBuildingNumberOfDwellings.at(i);

//            QVector<QString> auxDestination;

//            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){
//                theNumberOfPersons += theHouseholdSize.at(i).at(j);
//                auxDestination << QString("NotDisplaced");
//            }
//        }


//        // initalize all these vectors
//        theImmediateEconomicLoss.resize(theNumberOfBuildings);
//        theImmediateEconomicLoss.fill(0.0);

//        theImmediateContentLoss.resize(theNumberOfBuildings);
//        theImmediateContentLoss.fill(0.0);

//        theBuildingRepairTime.resize(theNumberOfBuildings);
//        theBuildingRepairTime.fill(0.0);

//        theBuildingReoccupancyTime.resize(theNumberOfBuildings);
//        theBuildingReoccupancyTime.fill(0.0);

//        theDelayInTakingAction.resize(theNumberOfBuildings);
//        theDelayInTakingAction.fill(0.0);

//        theBuildingAvailableFunds.resize(theNumberOfBuildings);
//        theBuildingAvailableFunds.fill(0.0);

//        theBuildingThresholdTimeToBeSold.resize(theNumberOfBuildings);
//        theBuildingThresholdTimeToBeSold.fill(0.0);

//        theBuildingThresholdProbabilityToBeEvicted.resize(theNumberOfBuildings);
//        theBuildingThresholdProbabilityToBeEvicted.fill(0.0);

//        theBuildingThresholdTimeToMakeADecision.resize(theNumberOfBuildings);
//        theBuildingThresholdTimeToMakeADecision.fill(0.0);

//        theBuildingReceivedFundTime.resize(theNumberOfBuildings);
//        theBuildingReceivedFundTime.fill(1825.0);

//        theAvailableMaterials.resize(theNumberOfBuildings);
//        theAvailableMaterials.fill(0.0);

//        theNumberOfWorkersAvailable.resize(theNumberOfBuildings);
//        theNumberOfWorkersAvailable.fill(0.0);

//        theBuildingTimeUnderRepairs.resize(theNumberOfBuildings);
//        theBuildingTimeUnderRepairs.fill(0.0);

//        theInspectionState.resize(theNumberOfBuildings);
//        theInspectionState.fill(true);

//        theBiddingProcess.resize(theNumberOfBuildings);
//        theBiddingProcess.fill(false);

//        theBuildingNeedForRedesign.resize(theNumberOfBuildings);
//        theBuildingNeedForRedesign.fill(false);

//        theEngineeringAssessment.resize(theNumberOfBuildings);
//        theEngineeringAssessment.fill(true);

//        theBuildingPermitStatus.resize(theNumberOfBuildings);
//        theBuildingPermitStatus.fill(true);

//        theRepairClass.resize(theNumberOfBuildings);
//        theRepairClass.fill(0);

//        theRecoveryTime.resize(theNumberOfBuildings);
//        theRecoveryTime.fill(2555.0);

//        alreadyRequestedFinancing.resize(theNumberOfBuildings);
//        alreadyRequestedFinancing.fill(false);

//        alreadyRequestedEngineers.resize(theNumberOfBuildings);
//        alreadyRequestedEngineers.fill(false);

//        alreadyRequestedMaterials.resize(theNumberOfBuildings);
//        alreadyRequestedMaterials.fill(false);

//        alreadyRequestedNegotiation.resize(theNumberOfBuildings);
//        alreadyRequestedNegotiation.fill(false);
//    } // if


//    // --------------------------------------------------------------------------------------
//    // 2 - Handle packets delivered on this time step
//    // --------------------------------------------------------------------------------------
//    QList<QPointer <RParameter>> thePacketList;


//    // Check if the buildings in this neighborhood have an object to request workers from
//    for (int i = 0; i < theContractorList.size(); i++){

//        // Cast the the as a response
//        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theContractorList.at(i));
//        if (theResponseComingToThisModel){
//            thePacketList << theContractorList.at(i);
//        }
//    }



//    // Check if the buildings in this neighborhood have an object to request insurance money from
//    for (int i = 0; i < theFinancialResourcesSupplierList.size(); i++){

//        // Cast the the as a response
//        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theFinancialResourcesSupplierList.at(i));
//        if (theResponseComingToThisModel){
//            thePacketList << theFinancialResourcesSupplierList.at(i);
//        }
//    }


//    // Load the packets received
//    this->loadPackets(thePacketList);


//    // Extract contents from the packets received
//    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {

//        // Get hold of what is the content of this packet
//        QString packetType = packetsCurrentlyOnboard.at(i)->getPacketContentsType();
//        QVector<int> packetOwnerID = packetsCurrentlyOnboard.at(i)->getPacketOwner();// size of packetOwnerID equals the number of buildings
//        QVector<double> contentsReceived = packetsCurrentlyOnboard.at(i)->getPacketContents();

//        for (int j = 0; j < contentsReceived.size(); j++){
//            int packetOwnerI = packetOwnerID.at(j);
//            double amountReceived = contentsReceived.at(j);

//            // If this packet contains Money
//            if (packetType.contains(QString("Money"))){
//                if (amountReceived > 0){
//                    theBuildingAvailableFunds[packetOwnerI] += amountReceived;
//                    theBuildingReceivedFundTime[packetOwnerI] = theCurrentTime->getCurrentValue() - timeOfEvent;
//                    if (theOutputLevel == RDomain::Maximum){
//                        if (theBuildingOccupancyClass.at(packetOwnerI) == QString("RES1")){
//                            //qDebug() << this->objectName() << theBuildingOwnerIncome.at(packetOwnerI) << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)   <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                            //qCritical() << this->objectName() << theBuildingOwnerIncome.at(packetOwnerI) << "unit" << theBuildingOccupancyClass.at(packetOwnerI) << packetOwnerID.at(j)   <<  "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                        }
//                    }
//                }
//            }

//            // If this packet contains workforce
//            else if (packetType == QString("Negotiation")){
//                if (amountReceived > 0){
//                    theBiddingProcess[packetOwnerI] = true;
//                    if (theOutputLevel == RDomain::Maximum){
//                        //qDebug() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                        //qCritical() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                    }
//                }
//            }

//            // If this packet contains workforce
//            else if (packetType == QString("Allocation")){
//                if (amountReceived > 0){
//                    theNumberOfWorkersAvailable[packetOwnerI] += static_cast<int>(amountReceived);
//                    if (theOutputLevel == RDomain::Maximum){
//                        // qDebug() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                        //qCritical() << this->objectName() << "unit" << packetOwnerID.at(j)  << "received" << amountReceived << packetType << "from" << packetsCurrentlyOnboard.at(i)->objectName();
//                    }


//                }
//            }
//        }
//    }

//    // Delete the packets whose final destinations was this model
//    this->handlePacketsThatReachedFinalDestination();


//    // --------------------------------------------------------------------------------------
//    // 4 - Calculate everything that happens at the time of the earthquake (t = t_0)
//    // --------------------------------------------------------------------------------------
//    // Immediate losses
//    // Considering only one earthquake in this analysis - think about aftershocks later
//    if (timeOfEvent == 0.0){

//        // Create the request for building inspection which we will add the damaged buildings to
//        QString inspectionRequestName = QString(this->objectName() + "InspectionRequest%1").arg(numberOfInspectionRequestsCreatedHere++);
//        theDomain->createObject(inspectionRequestName, "RRequest");
//        RRequest *theInspectionRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
//        theInspectionRequest->setModel(this);
//        theInspectionRequest->setContentType(QString("Inspector"));

//        // Building damage model
//        theBuildingDamageModel = qobject_cast<RHazusBuildingModel*>(theBuildingDamageModelResponse->getModel());

//        // ******
//        RNetworkSamplingModel * theSamplingModel = theDomain->findChild<RNetworkSamplingModel*>();
//        double theMultiplier = theSamplingModel->getSamplingMultiplier();
//        // ******
//        theMultiplier = 1.0;
//        //qCritical()<< "The sampling multiplier is set to 1 for ground motions!!!";
//        //qCritical()<< "The 5th set of ground motion is used! ";

//        //qCritical() << "Overwriting the Sa0.3 and Sa1!! Scale the first set of SA's by the sampling multiplier";
//        // For all the buildings
//        for (int i = 0; i < theNumberOfBuildings; i++){


//            // Considering only one earthquake in this analysis - think about aftershocks later
//            if (theBuildingStrDamageState[i] > 0.0){

//                // Get hold of the damaged buildings
//                if (theBuildingStrDamageState[i] > 2){
//                    theDamagedBuildingsIndexes << i;
//                }

//                // If there was damage to any building set the time of the event as the current time
//                timeOfEvent = theCurrentTime->getCurrentValue();


//                // If the immediate loss is higher than the building value it means the building collapsed
//                if (theImmediateEconomicLoss.at(i) > theBuildingValue.at(i)){
//                    theBuildingNeedForRedesign[i] = true;
//                }

//                // If this is a single-family renter-occupied building set a delay in taking action
//                if (theBuildingOccupancyClass.at(i) == QString("RES1") && theHouseholdTenureStatus.at(i).at(0) == QString("RenterOccupied")){
//                    theDelayInTakingAction[i] = 30.0;
//                }

//                // Inspection requests
//                theInspectionRequest->setRequesterIDList(i);
//                theInspectionRequest->setRequestedAmountList(1.0);

//            } // if DS > 0

//            QVector<bool> auxHouseholdFatality;

//            // Threshold prob of being sold
//            theBuildingThresholdTimeToBeSold[i] = rand() / static_cast<double>(RAND_MAX);

//            // Threshold prob of being evicted
//            theBuildingThresholdProbabilityToBeEvicted[i] = rand() / static_cast<double>(RAND_MAX);

//            // Threshold prob of leaving
//            theBuildingThresholdTimeToMakeADecision[i] = rand() / static_cast<double>(RAND_MAX);

//        } // i

//        // Delete the inspection request if no building needs inspection
//        if (theInspectionRequest->getRequestedAmountList().size() == 0){
//            theDomain->deleteObject(inspectionRequestName);
//        }
//    } // if timeOfEvent == 0.0

//    // --------------------------------------------------------------------------------------
//    // 5 - Calculate everything that happens after the time of the earthquake (t > t_0)
//    // --------------------------------------------------------------------------------------
//    // Obtain resources for repairing the buildings

//    // ******************
//    // ******************
//    //                               Financing  ----------------------------\
//    // Earthquake  -- Inspection --  Materials ----------------------------- \ Workers ->REPAIRS
//    //                               Engineering mobilization --Permitting-- /
//    //                Utilities --------------------------------------------/
//    // ******************
//    // ******************

//    QElapsedTimer myTimer;
//    myTimer.start();
//    // Rebuilding processes
//    totalHabitableArea = theTotalBuildingArea;
//    theNumberOfHabitableDwellings = theTotalNumberOfDwellings;

//    double numberOfPreCodeUninhabitableDwellings = 0.0;
//    double numberOfLowCodeUninhabitableDwellings = 0.0;
//    double numberOfModerateCodeUninhabitableDwellings = 0.0;
//    double numberOfHighCodeUninhabitableDwellings = 0.0;
//    double numberOfDetachedUninhabitableDwellings = 0.0;
//    double numberOfMultifamilyUninhabitableDwellings = 0.0;
//    double numberOfLowHouseholdIncomeUninhabitableDwellings = 0.0;
//    double numberOfMediumHouseholdIncomeUninhabitableDwellings = 0.0;
//    double numberOfHighHouseholdIncomeUninhabitableDwellings = 0.0;
//    double numberOfRecentImmigrantUninhabitableDwellings = 0.0;
//    double numberOfEstablishedImmigrantUninhabitableDwellings = 0.0;
//    double numberOfCitizenUninhabitableDwellings = 0.0;
//    double numberOfOwnerOccupiedUninhabitableDwellings = 0.0;
//    double numberOfRenterOccupiedUninhabitableDwellings = 0.0;
//    double numberOfHigherEducatedUninhabitableDwellings = 0.0;
//    double numberOfMarriedCouplesUninhabitableDwellings = 0.0;
//    double numberOfSingleParentsUninhabitableDwellings = 0.0;
//    double numberOfRecentMoversUninhabitableDwellings = 0.0;
//    double numberOfNotRecentMoversUninhabitableDwellings = 0.0;
//    double numberOfNonHigherEducatedUninhabitableDwellings = 0.0;
//    double numberOfElderlyUninhabitableDwellings = 0.0;
//    double numberOfNoElderlyUninhabitableDwellings = 0.0;
//    double numberOfChildrenUninhabitableDwellings = 0.0;
//    double numberOfNoChildrenUninhabitableDwellings = 0.0;
//    double numberOfWhiteUninhabitableDwellings = 0.0;
//    double numberOfBlackUninhabitableDwellings = 0.0;
//    double numberOfAsianUninhabitableDwellings = 0.0;
//    double numberOfHispanicUninhabitableDwellings = 0.0;
//    double numberOfNonHispanicUninhabitableDwellings = 0.0;

//    // Go over everything that will be evaluated on each 'delta t'
//    if (timeOfEvent > 0.0) {

//        // Handle all requests that need to be made on this time step
//        QVector<int> theInsuranceRequestID;
//        QVector<int> thePrivateRequestID;
//        QVector<int> thePublicRequestID;
//        QVector<int> theGrantRequestID;
//        QVector<int> theContractorNegotiationRequestID;
//        QVector<int> theSFContractorAllocationRequestID;
//        QVector<int> theMFContractorAllocationRequestID;

//        // ***************************************************************
//        // Loop over all damaged buildings
//        // ***************************************************************
//        for (int i = 0; i < theDamagedBuildingsIndexes.size(); i++){

//            // NOTE: we are only looping over damaged buildings, make sure to properly use the counter 'ii' instead 'i'
//            int ii = theDamagedBuildingsIndexes.at(i);

//            // If repairs have just finished successfully
//            // NOTE: comparing theBuildingRepairTime (time needed to repair) and theBuildingTimeUnderRepairs is what defines recovery state
//            // NOTE: a building with theNumberOfWorkersAvailable > 0 means it is under repairs
//            if (theBuildingRepairTime.at(ii) <= theBuildingTimeUnderRepairs.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){

//                theRecoveryTime[ii] = theCurrentTime->getCurrentValue() - timeOfEvent;
//                //qCritical()<<"The recovery time is set for building "<<ii<<": "<< theRecoveryTime.at(ii);
//                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
//                    RSingleFamilyContractorFirmModel *theSFContractorAllocationModel = theDomain->findChild<RSingleFamilyContractorFirmModel*>(); // ******* REVIEW this eventually
//                    theSFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));

//                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
//                    RMultiFamilyContractorFirmModel *theMFContractorAllocationModel = theDomain->findChild<RMultiFamilyContractorFirmModel*>();
//                    theMFContractorAllocationModel->dismissTeamOfWorkers(theBuildingOccupancyClass.at(ii), theNumberOfWorkersAvailable.at(ii));
//                }

//                //qCritical() << "Repairs have finished here!" << "dismissing" << theBuildingType.at(ii) << theBuildingOccupancyClass.at(ii) << theNumberOfWorkersAvailable.at(ii) << "workers.";
//                theNumberOfWorkersAvailable[ii] = 0.0; // This defines that the building is not being repaired anymore
//            }

//            // If repairs are progressing normally
//            else if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theNumberOfWorkersAvailable.at(ii) > 0){

//                theBuildingTimeUnderRepairs[ii] += static_cast<double>(theCurrentTime->getCurrentValue() - lastEvaluationTime);

//                // If repairs are going on, the household has not left, and the building can be reoccupied
//                if (theBuildingReoccupancyTime.at(ii) <= theBuildingTimeUnderRepairs.at(ii)){
//                    for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){

//                        // ****************
//                        // Building finished repairs
//                        // ****************
//                    }
//                }
//            }


//            // Lastly, the building is not repaired and some resource is still missing, request it
//            if (theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && (theAvailableMaterials.at(ii) <= 0.0 || theBuildingAvailableFunds.at(ii) <= 0.0 || theNumberOfWorkersAvailable.at(ii) <= 0)){
//                if (theCurrentTime->getCurrentValue() - timeOfEvent > 600.0){
//                    //qCritical() << ii << theBuildingType.at(ii) << theBuildingOccupancyClass.at(ii) << "Materials:" << theAvailableMaterials.at(ii) << "Funds:" << theBuildingAvailableFunds.at(ii) << "Workers:" << theNumberOfWorkersAvailable.at(ii) << "Engineering Assessment:" << theEngineeringAssessment.at(ii) << "Permit:" << theBuildingPermitStatus.at(ii) << "Bidding:" << theBiddingProcess.at(ii);
//                }

//                // ***************************************************************
//                // Loop over all dwellings
//                // ***************************************************************
//                // Loop over all households in the buildings
//                for (int j = 0; j < theBuildingNumberOfDwellings.at(ii); j++){

//                    double theDeductible = 0.15 * theBuildingValue.at(ii);// theImmediateEconomicLoss.at(ii);
//                    // qCritical() << this->objectName() << "The deductible is" << theDeductible;

//                    // If this is a damaged multi-family building
//                    if (theBuildingOccupancyClass.at(ii).contains(QString("RES3")) && alreadyRequestedFinancing.at(ii) == false){

//                        // If this a multi-family building assume money will come from private loan
//                        thePrivateRequestID << ii;
//                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100.0; // Ask me if you have questions about this line
//                        alreadyRequestedFinancing[ii] = true; // Set this flag to true so the building will not ask for financing again


//                        // If this is a single-family building
//                    } else if (theBuildingOccupancyClass.at(ii) == QString("RES1") && alreadyRequestedFinancing.at(ii) == false){

//                        QString theCity = QString("SanFrancisco");
//                        bool skipFunding = false;

//                        if (skipFunding == true){
//                            qCritical() << "Funds for everyone!!";
//                            thePublicRequestID << ii;
//                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

//                            // If this analysis is for San Francisco, use this financing decision model
//                        } else if (theCity == QString("SanFrancisco")){

//                            // Single family buildings
//                            if (theBuildingOccupancyClass.at(ii) == QString("RES1")){

//                                // If the building is owner occupied
//                                if (theHouseholdTenureStatus.at(ii).at(j) == QString("OwnerOccupied")){

//                                    // If high income
//                                    if (theBuildingOwnerIncome.at(ii) == QString("High")){

//                                        // If insured and can pay deductible out-of-pocket
//                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
//                                            theInsuranceRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

//                                            // If not insured but no mortgage, get a private loan
//                                        } else if (theHouseholdMortgage.at(ii).at(j) == false) {
//                                            thePrivateRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

//                                            // If not insured or cannot pay deductible and has a mortgage, get a loan and use savings but only if its enough to pay for repairs
//                                        } else if (theImmediateEconomicLoss.at(ii) < 200000.0 + theHomeownerIncomeValue.at(ii)) {
//                                            thePublicRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;
//                                        }
//                                        // If not wait for a grant from CDBG-DR
//                                        else {
//                                            theGrantRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
//                                        }
//                                    }
//                                    else if (theBuildingOwnerIncome.at(ii) == QString("Moderate")){
//                                        double rngGetCDBGDRMoney = rand()/static_cast<double>(RAND_MAX);

//                                        // If insured and can pay deductible out-of-pocket
//                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
//                                            theInsuranceRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

//                                            // If not insured but no mortgage, get a private loan
//                                        } else if (theHouseholdMortgage.at(ii).at(j) == false){
//                                            thePrivateRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

//                                            // If not insured or cannot pay deductible and has a mortgage, get a loan and use savings but only if its enough to pay for repairs
//                                            // Note: the maximum amount of loan is less than for high income
//                                        } else if (theImmediateEconomicLoss.at(ii) < 150000.0 + theHomeownerIncomeValue.at(ii)) {
//                                            thePublicRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;

//                                        } else if (rngGetCDBGDRMoney < 0.25){
//                                            theGrantRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
//                                        }
//                                        // If not wait for a grant from CDBG-DR
//                                        else {
//                                            theGrantRequestID << ii;
//                                            //theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
//                                        }
//                                    }

//                                    // If LMI, get a small grant that may not be enough

//                                    else if (theBuildingOwnerIncome.at(ii) == QString("Low")){

//                                        // If insured and can pay deductible out-of-pocket
//                                        if (theBuildingInsuranceStatus.at(ii) == true && theDeductible <= theHomeownerIncomeValue.at(ii)){
//                                            theInsuranceRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 3.0 * theMedianHouseholdIncome / 100000.0;

//                                            // If not insured but no mortgage, get a private loan
//                                        }
//                                        // Note: the maximum amount of loan is less than for high and moderate income
//                                        else if (theImmediateEconomicLoss.at(ii) < 50000.0 + theHomeownerIncomeValue.at(ii)){
//                                            thePublicRequestID << ii;
//                                            theBuildingAvailableFunds[ii] += 1.0 * theMedianHouseholdIncome / 100000.0;
//                                        }
//                                    }

//                                    // If the building is renter-occupied
//                                } else if (theHouseholdTenureStatus.at(ii).at(j) == QString("RenterOccupied")) {

//                                    // Assuming that renter-occupied buildings will depend on private loan
//                                    if (theBuildingOwnerIncome.at(ii) == QString("High") && theImmediateEconomicLoss.at(ii) < 200000.0 + theHomeownerIncomeValue.at(ii)) {
//                                        thePrivateRequestID << ii;
//                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;
//                                    }
//                                    // Assuming that renter-occupied buildings will depend on private loan
//                                    if (theBuildingOwnerIncome.at(ii) == QString("Moderate") && theImmediateEconomicLoss.at(ii) < 100000.0 + theHomeownerIncomeValue.at(ii)) {
//                                        thePrivateRequestID << ii;
//                                        theBuildingAvailableFunds[ii] += 2.0 * theMedianHouseholdIncome / 100000.0;

//                                        // Assume small grants will be given to landlords of LMI homes
//                                        // Note, this income is the TENANT income
//                                    } else if (theHouseholdIncome.at(ii).at(j) == QString("Low") && theImmediateEconomicLoss.at(ii) < 150000.0 + theHomeownerIncomeValue.at(ii)){
//                                        //theGrantRequestID << ii;
//                                        //theBuildingAvailableFunds[ii] += 0.5 * theMedianHouseholdIncome / 100000.0;
//                                    }


//                                } else {
//                                    qCritical() << this->objectName() << "has a problem with the building tenure.";
//                                }


//                            } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))) {

//                                // Smaller buildings
//                                if (theBuildingNumberOfDwellings.at(ii) < 25 && theImmediateEconomicLoss.at(ii) / theBuildingNumberOfDwellings.at(ii) < 50000.0){
//                                    // apply for FRM
//                                } else if (theBuildingNumberOfDwellings.at(ii) >= 25 && theImmediateEconomicLoss.at(ii) / theBuildingNumberOfDwellings.at(ii) < 50000.0) {
//                                    // apply for LRRP but takes longer
//                                }
//                            }

//                            // If the analysis is not for San Francisco use this generic financing algorithm
//                        }

//                    } // Multi-family vs Single-family
//                    alreadyRequestedFinancing[ii] = true;

//                    // If gone and sold
//                    if (theBuildingOccupancyClass.at(ii) == QString("RES1") && theHouseholdDestination.at(ii).at(j).contains(QString("Gone")) == true && theHouseholdDestination.at(ii).at(j).contains(QString("Abandon")) == false && theCurrentTime->getCurrentValue() - theBuildingTimeSold.at(ii) > 180.0){
//                        // If the building was abandoned and sold, restart the resource procurement phase 180 days after the sell
//                        // This time assume funding will be immediately available from the new owner
//                        theBuildingAvailableFunds[ii] = theImmediateEconomicLoss.at(ii) + 1.0;
//                        alreadyRequestedEngineers[ii] = false;
//                        alreadyRequestedMaterials[ii] = false;
//                        alreadyRequestedNegotiation[ii] = false;
//                        theBuildingTimeSold[ii] = 1000000.0;
//                    }
//                } // for j number of dwellings

//                // Above, we defined if household would be able to finance repairs
//                // and if so, where the households would get their money from.
//                // For the households that will be able to finance their repairs, even if they do not have the money yet,
//                // start negotiating resources.
//                if (theBuildingAvailableFunds.at(ii) > 0 && theBuildingStrDamageState.at(ii) > 2){

//                    // ***************************************************************
//                    // Materials
//                    // ***************************************************************
//                    if (alreadyRequestedMaterials.at(ii) == false){

//                        // Chenbo: why request materials depending on theBuildingOccupancyClass? Assume single family buildings to be wooden?

//                        if (theBuildingOccupancyClass.at(ii) == QString("RES1") || theBuildingOccupancyClass.at(ii) ==  QString("RES2")){
//                            //theWoodRequestID << ii;
//                            //qDebug() << "household" << ii << "in building" << ii << "is requesting timber";

//                        } else if(theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
//                            //theConcreteRequestID << ii;
//                            //qDebug() << "household" << ii << "in building" << ii << "is requesting concrete";

//                        } else {
//                            qCritical() << "=======================================" << theBuildingType.at(ii) << "at" << this->objectName() << "is not a valid building type";
//                        }

//                        theAvailableMaterials[ii] = true;
//                        alreadyRequestedMaterials[ii] = true;
//                    }


//                    // ***************************************************************
//                    // Contractor mobilization
//                    // ***************************************************************
//                    if (alreadyRequestedNegotiation.at(ii) == false){
//                        theContractorNegotiationRequestID << ii;
//                        alreadyRequestedNegotiation[ii] = true;
//                    }

//                } // if promise of money
//            } // if some resource is still missing



//            //  If repairs are needed and power is restored and all resources were obtained and permit was obtained and materials were received
//            if (theBuildingStrDamageState.at(ii) > 2 && theBuildingRepairTime.at(ii) > theBuildingTimeUnderRepairs.at(ii) && theBiddingProcess.at(ii) == true && theBuildingAvailableFunds.at(ii) >= theImmediateEconomicLoss.at(ii)){

//                // ***************************************************************
//                // Finally hire the contractor to perform repairs
//                // ***************************************************************
//                if (theBuildingOccupancyClass.at(ii) == QString("RES1")){
//                    theSFContractorAllocationRequestID << ii;
//                } else if (theBuildingOccupancyClass.at(ii).contains(QString("RES3"))){
//                    theMFContractorAllocationRequestID << ii;
//                    //qDebug() << "Building" << ii << "with occupancy" << theBuildingOccupancyClass.at(ii) << "is requesting" << 1 << "workers" "(" << theImmediateEconomicLoss.at(ii) << ")";
//                }

//                // Reduce the funds available assuming that half of it goes towards labour costs
//                theBuildingAvailableFunds[ii] -= theImmediateEconomicLoss.at(ii) / 2.0;
//            } // can make request
//        } // for i number of buildings


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
//                theInsuranceRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));
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
//                thePublicLoanRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));

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

//        if (thePrivateRequestID.size() > 0){
//            for (int i = 0; i < thePrivateRequestID.size(); i++){
//                int theID = thePrivateRequestID.at(i);
//                thePrivateLoanRequest->setRequesterIDList(theID);
//                thePrivateLoanRequest->setRequestedAmountList(theImmediateEconomicLoss.at(theID));
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

//        // Request workforce
//        theContractorNegotiationRequest->clearRequesterIDList();
//        theContractorNegotiationRequest->clearRequestedAmountList();
//        theContractorNegotiationRequest->clearRepairClassList();
//        theContractorNegotiationRequest->clearNumberOfStoreysList();
//        theContractorNegotiationRequest->clearOccupancyClassList();

//        if (theContractorNegotiationRequestID.size() > 0){
//            for (int i = 0; i < theContractorNegotiationRequestID.size(); i++){
//                int theID = theContractorNegotiationRequestID.at(i);

//                if (theBuildingStrDamageState.at(theID) <= 2){
//                    //theContractorNegotiationRequest->setRepairClasses(0);
//                } else if (theBuildingStrDamageState.at(theID) > 2){
//                    // If extensive damage is observed
//                    theContractorNegotiationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
//                    theContractorNegotiationRequest->setRepairClasses(3);
//                    theContractorNegotiationRequest->setRequesterIDList(theID);
//                    theContractorNegotiationRequest->setRequestedAmountList(1.0);
//                    theContractorNegotiationRequest->setNumberOfStoreys(theBuildingNumberOfStoreys.at(theID));

//                } else {
//                    // If slight or moderate damage is observed
//                    theContractorNegotiationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
//                    theContractorNegotiationRequest->setRepairClasses(1);
//                    theContractorNegotiationRequest->setRequesterIDList(theID);
//                    theContractorNegotiationRequest->setRequestedAmountList(1.0);
//                    theContractorNegotiationRequest->setNumberOfStoreys(theBuildingNumberOfStoreys.at(theID));
//                }
//            }
//        }

//        // Request contractor allocation
//        theSFContractorAllocationRequest->clearOccupancyClassList();
//        theSFContractorAllocationRequest->clearRepairClassList();
//        theSFContractorAllocationRequest->clearRequesterIDList();
//        theSFContractorAllocationRequest->clearRequestedAmountList();

//        if (theSFContractorAllocationRequestID.size() > 0){
//            for (int i = 0; i < theSFContractorAllocationRequestID.size(); i++){
//                int theID = theSFContractorAllocationRequestID.at(i);

//                // Temporary fix
//                double theBid = 0.75;
//                if (theBuildingOwnerIncome.at(i) == QString("High")){
//                    theBid = 1.25;
//                } else if (theBuildingOwnerIncome.at(i)  == QString("Moderate")){
//                    theBid = 1.0;
//                }

//                theSFContractorAllocationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
//                theSFContractorAllocationRequest->setRepairClasses(1);
//                theSFContractorAllocationRequest->setRequesterIDList(theID);
//                theSFContractorAllocationRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
//                theSFContractorAllocationRequest->setBid(theBid * theBuildingValue.at(i));
//            }
//        }

//        theMFContractorAllocationRequest->clearOccupancyClassList();
//        theMFContractorAllocationRequest->clearRepairClassList();
//        theMFContractorAllocationRequest->clearRequesterIDList();
//        theMFContractorAllocationRequest->clearRequestedAmountList();

//        if (theMFContractorAllocationRequestID.size() > 0){
//            for (int i = 0; i < theMFContractorAllocationRequestID.size(); i++){
//                int theID = theMFContractorAllocationRequestID.at(i);
//                theMFContractorAllocationRequest->setOccupancyClasses(theBuildingOccupancyClass.at(theID));
//                theMFContractorAllocationRequest->setRepairClasses(1);
//                theMFContractorAllocationRequest->setRequesterIDList(theID);
//                theMFContractorAllocationRequest->setRequestedAmountList(theBuildingNumberOfDwellings.at(theID));
//                theMFContractorAllocationRequest->setMedianIncome(theMedianHouseholdIncome);
//                theMFContractorAllocationRequest->setBid(theBuildingValue.at(i));
//            }
//        }


//        // ***************************************************************
//        // Get hold of results to plot
//        // ***************************************************************
//        for (int i = 0; i < theNumberOfBuildings; i++){

//            // Prepare data for plotting
//            if (theBuildingStrDamageState.at(i) > 2 && theBuildingRepairTime.at(i) > theBuildingTimeUnderRepairs.at(i)){
//                theNumberOfHabitableDwellings -= theBuildingNumberOfDwellings.at(i);
//            }

//            for (int j = 0; j < theBuildingNumberOfDwellings.at(i); j++){

//                // ***************************************************************
//                // Gather the data to produce the recovery trajectories
//                // ***************************************************************
//                if (theBuildingTimeUnderRepairs.at(i) < theBuildingRepairTime.at(i)){

//                    // Building code
//                    if (theBuildingCodeLevel.at(i) == QString("Pre")){
//                        numberOfPreCodeUninhabitableDwellings += 1.0;
//                    } else if (theBuildingCodeLevel.at(i) == QString("Low")){
//                        numberOfLowCodeUninhabitableDwellings += 1.0;
//                    } else if (theBuildingCodeLevel.at(i) == QString("Moderate")){
//                        numberOfModerateCodeUninhabitableDwellings += 1.0;
//                    } else if (theBuildingCodeLevel.at(i) == QString("High")){
//                        numberOfHighCodeUninhabitableDwellings += 1.0;
//                    } else {
//                        qCritical() << "Cannot identify the building code level for this building!";
//                    }

//                    // Income
//                    if (theBuildingOwnerIncome.at(i) == QString("Low")){
//                        numberOfLowHouseholdIncomeUninhabitableDwellings += 1.0;
//                    }
//                    else if (theBuildingOwnerIncome.at(i) == QString("Moderate")){
//                        numberOfMediumHouseholdIncomeUninhabitableDwellings += 1.0;
//                    }
//                    else if (theBuildingOwnerIncome.at(i) == QString("High")){
//                        numberOfHighHouseholdIncomeUninhabitableDwellings += 1.0;
//                    }

//                    // Tenure
//                    if (theHouseholdTenureStatus.at(i).at(j) == QString("RenterOccupied")){
//                        numberOfRenterOccupiedUninhabitableDwellings += 1.0;
//                    }
//                    else if (theHouseholdTenureStatus.at(i).at(j) == QString("OwnerOccupied")){
//                        numberOfOwnerOccupiedUninhabitableDwellings += 1.0;
//                    }

//                    // Immigrant status
//                    if (theBuildingOwnerImmigrationStatus.at(i) == QString("RecentImmigrant")){
//                        numberOfRecentImmigrantUninhabitableDwellings += 1.0;
//                    }
//                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("EstablishedImmigrant")){
//                        numberOfEstablishedImmigrantUninhabitableDwellings += 1.0;
//                    }
//                    else if (theBuildingOwnerImmigrationStatus.at(i) == QString("NonImmigrant")){
//                        numberOfCitizenUninhabitableDwellings += 1.0;
//                    }

//                    // Elderly
//                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Elderly"))){
//                        numberOfElderlyUninhabitableDwellings += 1.0;
//                    }
//                    else {
//                        numberOfNoElderlyUninhabitableDwellings += 1.0;
//                    }

//                    // Children
//                    if (theHouseholdAgeCategory.at(i).at(j).contains(QString("Children"))){
//                        numberOfChildrenUninhabitableDwellings += 1.0;
//                    }
//                    else {
//                        numberOfNoChildrenUninhabitableDwellings += 1.0;
//                    }

//                    // HigherEducated degree
//                    if (theHouseholdHigherEducationStatus.at(i).at(j) == true){
//                        numberOfHigherEducatedUninhabitableDwellings += 1.0;

//                    } else {
//                        numberOfNonHigherEducatedUninhabitableDwellings += 1.0;
//                    }

//                    // Race
//                    if (theHouseholdRace.at(i).at(j).contains("White")) {
//                        numberOfWhiteUninhabitableDwellings += 1.0;
//                    } else if (theHouseholdRace.at(i).at(j).contains("Black")){
//                        numberOfBlackUninhabitableDwellings += 1.0;
//                    } else if (theHouseholdRace.at(i).at(j).contains("Asian")){
//                        numberOfAsianUninhabitableDwellings += 1.0;
//                    }

//                    // Hispanic
//                    if (theHouseholdRace.at(i).at(j).contains("Hispanic")) {
//                        numberOfHispanicUninhabitableDwellings += 1.0;
//                    } else {
//                        numberOfNonHispanicUninhabitableDwellings += 1.0;
//                    }

//                    // Marital status of householder
//                    if (theHouseholdMaritalStatus.at(i).at(j) == QString("MarriedCouple")){
//                        numberOfMarriedCouplesUninhabitableDwellings += 1.0;
//                    } else {
//                        numberOfSingleParentsUninhabitableDwellings += 1.0;
//                    }

//                    // Recent mover status
//                    if (theHouseholdRecentMoverStatus.at(i).at(j) == true) {
//                        numberOfRecentMoversUninhabitableDwellings += 1.0;
//                    } else {
//                        numberOfNotRecentMoversUninhabitableDwellings += 1.0;
//                    }
//                }
//            } // for j
//        } // for i


//    } // if timeOfEvent > 0.0


//    // -------------------------------------------------------------------------------------------
//    // 6 - Create plots
//    // -------------------------------------------------------------------------------------------
//    // If it has been more than one day since last plot
//    timeSinceLastPlot += theCurrentTime->getCurrentValue() - lastEvaluationTime;
//    lastEvaluationTime = theCurrentTime->getCurrentValue();
//    double maximumTimeBetweenPlots = 7.0;


//    if (timeSinceLastPlot >= maximumTimeBetweenPlots || qAbs(timeOfEvent - theCurrentTime->getCurrentValue()) < 0.01){

//        if (theOutputLevel == RDomain::Maximum){
//            qDebug() << "";
//            qDebug() << "-----------------------------------------------------";
//            qDebug() << "             " << this->objectName();
//            qDebug() << " | " << "Time since event: " << theCurrentTime->getCurrentValue() - timeOfEvent;
//            qDebug() << " | " << "Number of dwellings in need of repairs:" << theTotalNumberOfDwellings - theNumberOfHabitableDwellings ;
//            qDebug() << "-----------------------------------------------------";
//            qDebug() << "";
//        }


//        timeSinceLastPlot = 0.0;
//        timeVector << lastEvaluationTime - timeAnalysisBegan;
//        theHousingRecoveryVector << theNumberOfHabitableDwellings;

//        theSFUninhabitableDwellingsVector << numberOfDetachedUninhabitableDwellings;
//        theMFUninhabitableDwellingsVector << numberOfMultifamilyUninhabitableDwellings;
//        thePreCodeUninhabitableDwellingsVector << numberOfPreCodeUninhabitableDwellings;
//        theLowCodeUninhabitableDwellingsVector << numberOfLowCodeUninhabitableDwellings;
//        theModCodeUninhabitableDwellingsVector << numberOfModerateCodeUninhabitableDwellings;
//        theHighCodeUninhabitableDwellingsVector << numberOfHighCodeUninhabitableDwellings;
//        theRenterUninhabitableDwellingsVector << numberOfRenterOccupiedUninhabitableDwellings;
//        theOwnerUninhabitableDwellingsVector << numberOfOwnerOccupiedUninhabitableDwellings;
//        thePoorUninhabitableDwellingsVector << numberOfLowHouseholdIncomeUninhabitableDwellings;
//        theModerateUninhabitableDwellingsVector << numberOfMediumHouseholdIncomeUninhabitableDwellings;
//        theRichUninhabitableDwellingsVector << numberOfHighHouseholdIncomeUninhabitableDwellings;
//        theRecentImmigrantUninhabitableDwellingsVector << numberOfRecentImmigrantUninhabitableDwellings;
//        theEstablishedImmigrantUninhabitableDwellingsVector << numberOfEstablishedImmigrantUninhabitableDwellings;
//        theNonImmigrantUninhabitableDwellingsVector << numberOfCitizenUninhabitableDwellings;
//        theHigherEducatedUninhabitableDwellingsVector << numberOfHigherEducatedUninhabitableDwellings;
//        theNonHigherEducatedUninhabitableDwellingsVector << numberOfNonHigherEducatedUninhabitableDwellings;
//        theElderlyMemberUninhabitableDwellingsVector << numberOfElderlyUninhabitableDwellings;
//        theNoElderlyMemberUninhabitableDwellingsVector << numberOfNoElderlyUninhabitableDwellings;
//        theChildrenMemberUninhabitableDwellingsVector << numberOfChildrenUninhabitableDwellings;
//        theNoChildrenMemberUninhabitableDwellingsVector << numberOfNoChildrenUninhabitableDwellings;
//        theMarriedCouplesUninhabitableDwellingsVector << numberOfMarriedCouplesUninhabitableDwellings;
//        theSingleParentsUninhabitableDwellingsVector << numberOfSingleParentsUninhabitableDwellings;
//        theRecentMoverUninhabitableDwellingsVector << numberOfRecentMoversUninhabitableDwellings;
//        theNotRecentMoverUninhabitableDwellingsVector << numberOfNotRecentMoversUninhabitableDwellings;
//        theWhiteUninhabitableDwellingsVector << numberOfWhiteUninhabitableDwellings;
//        theBlackUninhabitableDwellingsVector << numberOfBlackUninhabitableDwellings;
//        theAsianUninhabitableDwellingsVector << numberOfAsianUninhabitableDwellings;
//        theHispanicUninhabitableDwellingsVector << numberOfHispanicUninhabitableDwellings;
//        theNonHispanicUninhabitableDwellingsVector << numberOfNonHispanicUninhabitableDwellings;
//    }

    return 1;
}


void RRecoveryStrategyStudyNeighborhoodModel::plotUsableBuildings(double habitableArea)
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
            buildingRepairsPlot->xAxis->setAutoTicks(false);
            buildingRepairsPlot->xAxis->setAutoTickLabels(false);
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
        buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}



void RRecoveryStrategyStudyNeighborhoodModel::plotUsableBuildings(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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
            buildingRepairsPlot->xAxis->setAutoTicks(false);
            buildingRepairsPlot->xAxis->setAutoTickLabels(false);
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
        buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}


void RRecoveryStrategyStudyNeighborhoodModel::plotRapidity(double habitableArea1, double habitableArea2, double habitableArea3, QString var1Name, QString var2Name, QString var3Name)
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
            buildingRepairsPlot->xAxis->setAutoTicks(false);
            buildingRepairsPlot->xAxis->setAutoTickLabels(false);
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
        buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}


void RRecoveryStrategyStudyNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name)
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
            displacedPopulationPlot->xAxis->setAutoTicks(false);
            displacedPopulationPlot->xAxis->setAutoTickLabels(false);
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
        displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}

// For Chenbo: create a copy of this function and use it to plot the
//             displaced persons at low, moderate, and high income
// Tip: you need to create a new plotter object - ctrl F on the displacedPopulationPlot
//      and see where it is defined, initiated, and so on
void RRecoveryStrategyStudyNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name)
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
            displacedPopulationPlot->xAxis->setAutoTicks(false);
            displacedPopulationPlot->xAxis->setAutoTickLabels(false);
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
        displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
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
